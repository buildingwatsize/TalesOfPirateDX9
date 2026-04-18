#include "StdAfx.h"
#include "procirculate.h"
#include "GameApp.h"
#include "PacketCmd.h"
#include "Character.h"
#include "ChaAttr.h"
#include "GameConfig.h"
#include "util2.h"
#include "LoginScene.h"
#include "netprotocol.h"
#include "AreaRecord.h"

using namespace std;
#include <windows.h>
#include <bcrypt.h>
#include <vector>
#include <cstring>
#pragma comment(lib, "bcrypt.lib")

namespace {
static const uint32_t blake2s_IV[8] = {
	0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
	0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};
static const uint8_t blake2s_sigma[10][16] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15},
	{14,10, 4, 8, 9,15,13, 6, 1,12, 0, 2,11, 7, 5, 3},
	{11, 8,12, 0, 5, 2,15,13,10,14, 3, 6, 7, 1, 9, 4},
	{ 7, 9, 3, 1,13,12,11,14, 2, 6, 5,10, 4, 0,15, 8},
	{ 9, 0, 5, 7, 2, 4,10,15,14, 1,11,12, 6, 8, 3,13},
	{ 2,12, 6,10, 0,11, 8, 3, 4,13, 7, 5,15,14, 1, 9},
	{12, 5, 1,15,14,13, 4,10, 0, 7, 6, 3, 9, 2, 8,11},
	{13,11, 7,14,12, 1, 3, 9, 5, 0,15, 4, 8, 6, 2,10},
	{ 6,15,14, 9,11, 3, 0, 8,12, 2,13, 7, 1, 4,10, 5},
	{10, 2, 8, 4, 7, 6, 1, 5,15,11, 9,14, 3,12,13, 0}
};

static inline uint32_t rotr32(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
static inline uint32_t load32(const void* p) {
	uint32_t v; memcpy(&v, p, 4); return v;
}

static void blake2s_compress(uint32_t h[8], const uint8_t block[64],
	uint64_t counter, bool last)
{
	uint32_t v[16], m[16];
	for (int i = 0; i < 8; i++) { v[i] = h[i]; v[i+8] = blake2s_IV[i]; }
	v[12] ^= (uint32_t)(counter);
	v[13] ^= (uint32_t)(counter >> 32);
	if (last) v[14] = ~v[14];
	for (int i = 0; i < 16; i++) m[i] = load32(block + 4*i);

	#define G(r,i,a,b,c,d) do { \
		a += b + m[blake2s_sigma[r][2*i]]; d = rotr32(d^a,16); \
		c += d; b = rotr32(b^c,12); \
		a += b + m[blake2s_sigma[r][2*i+1]]; d = rotr32(d^a,8); \
		c += d; b = rotr32(b^c,7); \
	} while(0)

	for (int r = 0; r < 10; r++) {
		G(r,0,v[0],v[4],v[ 8],v[12]); G(r,1,v[1],v[5],v[ 9],v[13]);
		G(r,2,v[2],v[6],v[10],v[14]); G(r,3,v[3],v[7],v[11],v[15]);
		G(r,4,v[0],v[5],v[10],v[15]); G(r,5,v[1],v[6],v[11],v[12]);
		G(r,6,v[2],v[7],v[ 8],v[13]); G(r,7,v[3],v[4],v[ 9],v[14]);
	}
	#undef G
	for (int i = 0; i < 8; i++) h[i] ^= v[i] ^ v[i+8];
}

static void blake2s_hash(const void* input, size_t inlen, uint8_t out[32]) {
	uint32_t h[8];
	for (int i = 0; i < 8; i++) h[i] = blake2s_IV[i];
	h[0] ^= 0x01010020; // fanout=1, depth=1, digestlen=32

	const uint8_t* p = (const uint8_t*)input;
	uint64_t counter = 0;
	uint8_t block[64];

	while (inlen > 64) {
		counter += 64;
		blake2s_compress(h, p, counter, false);
		p += 64; inlen -= 64;
	}
	memset(block, 0, 64);
	memcpy(block, p, inlen);
	counter += (uint64_t)inlen;
	blake2s_compress(h, block, counter, true);

	for (int i = 0; i < 8; i++) {
		out[4*i  ] = (uint8_t)(h[i]      );
		out[4*i+1] = (uint8_t)(h[i] >>  8);
		out[4*i+2] = (uint8_t)(h[i] >> 16);
		out[4*i+3] = (uint8_t)(h[i] >> 24);
	}
}
} // anonymous namespace
#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif

_DBC_USING


void CProCirculateCS::BeginAction( CCharacter* pCha, DWORD type, void* param, CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BEGINACTION);
#ifdef _TEST_CLIENT
	CTestClient* pClient = reinterpret_cast<CTestClient*>( g_NetIF->m_connect.GetDatasock()->GetPointer() );
	DWORD dwCharID=(pCha)?pCha->getAttachID():pClient->GetCharID();
	pk.WriteLong(dwCharID);
#else
	pk.WriteLong(pCha->getAttachID());
#endif

    char	szLogName[1024] = { "BeginAction" };

    if( pCha )
    {
        strcpy(szLogName, pCha->getLogName() );            
    }

	try
	{
		LG(szLogName, "$$$PacketID:\t%d\n", pCNetIf->m_ulPacketCount);

#ifdef defPROTOCOL_HAVE_PACKETID
		pk.WriteLong(pCNetIf->m_ulPacketCount++);
#endif
		pk.WriteChar((dbc::uChar)type);
		switch (type)
		{
		case	enumACTION_MOVE:
			{
				
				stNetMoveInfo *pMove = (stNetMoveInfo *)param;
				pk.WriteSequence((cChar *)pMove->pos_buf ,uShort(sizeof(Point) * pMove->pos_num));
				pCNetIf->SendPacketMessage(pk);


				char buffer[64] = {0};
				char buf[64] = {0};
				CCharacter	*pCha = CGameScene::GetMainCha();
				CGameScene	*pScene = g_pGameApp->GetCurScene();
				if(!pCha->IsBoat()) {
				int nArea = pScene->_pTerrain->GetTile(pCha->GetCurX()/100, pCha->GetCurY()/100 )->getIsland();
				CAreaInfo* pArea = GetAreaInfo( nArea );


				sprintf(buf, "In %s",pArea->szDataName);

				sprintf(buffer, "%s Lv%d %s", pCha->getHumanName(), pCha->getLv(), g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
				if(pCha->GetTeamLeaderID() > 0) {
				}else{

				updateDiscordPresence(buffer, buf );

				}
				}else {
					sprintf(buffer, "%s Lv%d %s", pCha->getHumanName(), pCha->getLv(), g_GetJobName((short)pCha->getGameAttr()->get(ATTR_JOB)));
					sprintf(buf, "Sailing");
					updateDiscordPresence(buffer, buf );
				}
				
				




				LG(szLogName, "###Send(Move):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Ping:\t%3d\n", pMove->dwAveragePing);
				LG(szLogName, "Point:\t%3d\n", pMove->pos_num);
				for (DWORD i = 0; i < pMove->pos_num; i++)
				{
					LG(szLogName, "\t%d, \t%d\n", pMove->pos_buf[i].x, pMove->pos_buf[i].y);
				}
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL:
			{
				stNetSkillInfo *pSkill = (stNetSkillInfo *)param;
				pk.WriteChar(pSkill->chMove);
				pk.WriteChar(pSkill->byFightID);
				if (pSkill->chMove == 2)
				{
					pk.WriteSequence((cChar *)pSkill->SMove.pos_buf, uShort(sizeof(POINT) * pSkill->SMove.pos_num));
				}
				pk.WriteLong(pSkill->lSkillID);
				pk.WriteLong(pSkill->lTarInfo1);
				pk.WriteLong(pSkill->lTarInfo2);

				//for (int n = 0; n < 200; ++n)
				//{
					pCNetIf->SendPacketMessage(pk);
				//}

				LG(szLogName, "###Send(Skill):\tTick:[%d]\n", GetTickCount());
				if (pSkill->chMove == 2)
				{
					LG(szLogName, "Ping:\t%3d\n", pSkill->SMove.dwAveragePing);
					LG(szLogName, "Point:\t%3d\n", pSkill->SMove.pos_num);
					for (DWORD i = 0; i < pSkill->SMove.pos_num; i++)
					{
						LG(szLogName, "\t%d, \t%d\n", pSkill->SMove.pos_buf[i].x, pSkill->SMove.pos_buf[i].y);
					}
				}
				LG(szLogName, "Skill:\t%3d, FightID:%d\n", pSkill->lSkillID, pSkill->byFightID);
				LG(szLogName, "Target:\t%u, \t%u\n", pSkill->lTarInfo1, pSkill->lTarInfo2);
				LG(szLogName, "\n");
				break;
			}
		case	enumACTION_STOP_STATE:
			{
				pk.WriteShort(*((short *)param));
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Stop Skill State %d):\tTick:[%d]\n", *((short *)param), GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_LEAN: // �п�
			{
				stNetLeanInfo *pSLean = (stNetLeanInfo *)param;
				pk.WriteLong(pSLean->lPose);
				pk.WriteLong(pSLean->lAngle);
				pk.WriteLong(pSLean->lPosX);
				pk.WriteLong(pSLean->lPosY);
				pk.WriteLong(pSLean->lHeight);
				pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Lean):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_ITEM_PICK: // �����
			{
				stNetItemPick *pPick = (stNetItemPick *)param;
				pk.WriteLong(pPick->lWorldID);
				pk.WriteLong(pPick->lHandle);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Pick):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_THROW: // ������
			{
				stNetItemThrow *pThrow = (stNetItemThrow *)param;
				pk.WriteShort(pThrow->sGridID);
				pk.WriteShort((short)pThrow->lNum);
				pk.WriteLong(pThrow->lPosX);
				pk.WriteLong(pThrow->lPosY);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Throw):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_USE:
			{
				stNetUseItem *pUseItem = (stNetUseItem *)param;
				pk.WriteShort(pUseItem->sGridID);
				pk.WriteShort(pUseItem->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Use Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, RES_STRING(CMISS_000322), pUseItem->sGridID, pUseItem->sTarGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_UNFIX: // жװ����
			{
				stNetItemUnfix *pUnfix = (stNetItemUnfix *)param;
				pk.WriteChar(pUnfix->chLinkID);
				pk.WriteShort(pUnfix->sGridID);
				if (pUnfix->sGridID < 0) // ��������
				{
					pk.WriteLong(pUnfix->lPosX);
					pk.WriteLong(pUnfix->lPosY);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Unfix):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_POS:
			{
				stNetItemPos *pChangePos = (stNetItemPos *)param;
				pk.WriteShort(pChangePos->sSrcGridID);
				pk.WriteShort(pChangePos->sSrcNum);
				pk.WriteShort(pChangePos->sTarGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item pos):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_DELETE:
			{
				stNetDelItem *pDelItem = (stNetDelItem *)param;
				pk.WriteShort(pDelItem->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Del Item):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, RES_STRING(CMISS_000323), pDelItem->sGridID);
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_ITEM_INFO:
			{
				stNetItemInfo *pItemInfo = (stNetItemInfo *)param;
				pk.WriteChar(pItemInfo->chType);
				pk.WriteShort(pItemInfo->sGridID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Item Info):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, RES_STRING(CMISS_000324), pItemInfo->chType, pItemInfo->sGridID);
                LG(szLogName, "\n");
				break;
				break;
			}
		case	enumACTION_SHORTCUT: // ���¿����
			{
				stNetShortCutChange *pShortcutChange = (stNetShortCutChange *)param;
				pk.WriteChar(pShortcutChange->chIndex);
				pk.WriteChar(pShortcutChange->chType);
				pk.WriteShort(pShortcutChange->shyGrid);
				//pk.WriteByte(pShortcutChange->shyGrid2==-1?0:1);
				//pk.WriteShort(pShortcutChange->shyGrid2);
				pCNetIf->SendPacketMessage(pk);
				break;
			}
		case	enumACTION_LOOK: // ������ۣ��紬�Ļ�װ��
			{
				stNetChangeChaPart *pSChaPart = (stNetChangeChaPart *)param;
				pk.WriteShort(pSChaPart->sTypeID);
				for (int i = 0; i < enumEQUIP_NUM; i++)
				{
					pk.WriteShort(pSChaPart->SLink[i].sID);
				}
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Look):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_TEMP: //��ʱ��װЭ��
			{
				stTempChangeChaPart *pSTempChaPart = (stTempChangeChaPart *)param;
				pk.WriteLong(pSTempChaPart->dwItemID);
				pk.WriteLong(pSTempChaPart->dwPartID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Temp):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_EVENT: // �����¼�
			{
				stNetActivateEvent *pEvent = (stNetActivateEvent *)param;
				pk.WriteLong(pEvent->lTargetID);
				pk.WriteLong(pEvent->lHandle);
				pk.WriteShort(pEvent->sEventID);
                pCNetIf->SendPacketMessage(pk);

				// log
				LG(szLogName, "###Send(Event):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "\n");
				//
				break;
			}
		case	enumACTION_FACE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteShort(pNetFace->sAngle);
				pk.WriteShort(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Face):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_SKILL_POSE:
			{
				stNetFace	*pNetFace = (stNetFace *)param;
				pk.WriteShort(pNetFace->sAngle);
				pk.WriteShort(pNetFace->sPose);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Skill Pos):\tTick:[%d]\n", GetTickCount());
				LG(szLogName, "Angle:%d, Pose:%d\n", pNetFace->sAngle, pNetFace->sPose );
				break;
			}
		case enumACTION_GUILDBANK:
		case	enumACTION_BANK:
			{
				stNetBank	*pNetBank = (stNetBank *)param;
				pk.WriteChar(pNetBank->chSrcType);
				pk.WriteShort(pNetBank->sSrcID);
				pk.WriteShort(pNetBank->sSrcNum);
				pk.WriteChar(pNetBank->chTarType);
				pk.WriteShort(pNetBank->sTarID);
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Req):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_CLOSE_BANK:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(Bank Close):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_REQUESTGUILDLOGS:
			{
				uShort* curSize = reinterpret_cast<uShort*> (param);
				pk.WriteShort(*curSize);

				pCNetIf->SendPacketMessage(pk);
				break;
			}	
		case	enumACTION_UPDATEGUILDLOGS:
			{
				pCNetIf->SendPacketMessage(pk);
				break;
			}

		
		case	enumACTION_REQUESTGUILDBANK:
			{
				pCNetIf->SendPacketMessage(pk);

                LG(szLogName, "###Send(request guild):\tTick:[%d]\n", GetTickCount());
                LG(szLogName, "\n");
				break;
			}
		case	enumACTION_KITBAGTMP_DRAG:	// �϶���ʱ����
			{
				stNetTempKitbag* pNetTempKitbag = (stNetTempKitbag*)param;

				pk.WriteShort(pNetTempKitbag->sSrcGridID);
				pk.WriteShort(pNetTempKitbag->sSrcNum);
				pk.WriteShort(pNetTempKitbag->sTarGridID);

				pCNetIf->SendPacketMessage(pk);
				break;
			}
		default:
			break;
		}
	}
	catch (...)
	{
		//MessageBox(0, "!!!!!!!!!!!!!!!!!!!!exception: Begin Action", "error", 0);fix compile again
	}
}

// Э��C->S : ����ֹͣ�ж���Ϣ
void CProCirculateCS::EndAction( CActionState* pState )
{
	WPacket pk	=pCNetIf->GetWPacket();

	pk.WriteCmd(CMD_CM_ENDACTION);	//�����ж�
	pCNetIf->SendPacketMessage(pk);

	// log
    CCharacter *pMainCha = CGameApp::GetCurScene()->GetMainCha();
	if (pMainCha)
	{
		LG(pMainCha->getLogName(), "###Send(EndAction):\tTick:[%d]\n", GetTickCount());
	}
	//
}

bool CProCirculate::Connect(const char *hostname,unsigned short port,unsigned long timeout)
{
	return pCNetIf->m_connect.Connect(hostname,port,timeout);
}
void CProCirculate::Disconnect(int reason)
{
	pCNetIf->m_connect.Disconnect(reason);
}

static bool RSA_OAEP_Encrypt_BCrypt(
	const std::vector<BYTE>& modulus, const std::vector<BYTE>& exponent,
	const unsigned char* plaintext, size_t ptLen,
	std::vector<BYTE>& cipherOut)
{
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] enter ptLen=%zu nLen=%zu eLen=%zu\n",ptLen,modulus.size(),exponent.size());fflush(_f);fclose(_f);}}

	size_t nLen = modulus.size();
	size_t eLen = exponent.size();

	size_t blobSize = sizeof(BCRYPT_RSAKEY_BLOB) + nLen + eLen;
	std::vector<BYTE> blob(blobSize, 0);
	BCRYPT_RSAKEY_BLOB* hdr = reinterpret_cast<BCRYPT_RSAKEY_BLOB*>(blob.data());
	hdr->Magic = BCRYPT_RSAPUBLIC_MAGIC;
	hdr->BitLength = (ULONG)(nLen * 8);
	hdr->cbPublicExp = (ULONG)eLen;
	hdr->cbModulus = (ULONG)nLen;
	hdr->cbPrime1 = 0;
	hdr->cbPrime2 = 0;

	memcpy(blob.data() + sizeof(BCRYPT_RSAKEY_BLOB), exponent.data(), eLen);
	memcpy(blob.data() + sizeof(BCRYPT_RSAKEY_BLOB) + eLen, modulus.data(), nLen);

	BCRYPT_ALG_HANDLE hAlg = NULL;
	NTSTATUS st = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RSA_ALGORITHM, NULL, 0);
	if (st != 0) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] OpenAlg fail 0x%08X\n",(unsigned)st);fflush(_f);fclose(_f);}}
		return false;
	}

	BCRYPT_KEY_HANDLE hKey = NULL;
	st = BCryptImportKeyPair(hAlg, NULL, BCRYPT_RSAPUBLIC_BLOB, &hKey, blob.data(), (ULONG)blobSize, 0);
	if (st != 0) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] ImportKey fail 0x%08X\n",(unsigned)st);fflush(_f);fclose(_f);}}
		BCryptCloseAlgorithmProvider(hAlg, 0);
		return false;
	}

	BCRYPT_OAEP_PADDING_INFO oaepInfo;
	oaepInfo.pszAlgId = BCRYPT_SHA1_ALGORITHM;
	oaepInfo.pbLabel = NULL;
	oaepInfo.cbLabel = 0;

	ULONG cbResult = 0;
	st = BCryptEncrypt(hKey, (PUCHAR)plaintext, (ULONG)ptLen, &oaepInfo, NULL, 0, NULL, 0, &cbResult, BCRYPT_PAD_OAEP);
	if (st != 0) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] size fail 0x%08X\n",(unsigned)st);fflush(_f);fclose(_f);}}
		BCryptDestroyKey(hKey);
		BCryptCloseAlgorithmProvider(hAlg, 0);
		return false;
	}

	cipherOut.resize(cbResult);
	st = BCryptEncrypt(hKey, (PUCHAR)plaintext, (ULONG)ptLen, &oaepInfo, NULL, 0, cipherOut.data(), cbResult, &cbResult, BCRYPT_PAD_OAEP);
	BCryptDestroyKey(hKey);
	BCryptCloseAlgorithmProvider(hAlg, 0);

	if (st != 0) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] encrypt fail 0x%08X\n",(unsigned)st);fflush(_f);fclose(_f);}}
		return false;
	}
	cipherOut.resize(cbResult);
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[BCRYPT] OK len=%lu\n",cbResult);fflush(_f);fclose(_f);}}
	return true;
}

bool CProCirculate::SendPrivateKey()
{
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] enter\n");fflush(_f);fclose(_f);}}
	try {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] modLen=%zu expLen=%zu\n",g_NetIF->srvModulus.size(),g_NetIF->srvExponent.size());fflush(_f);fclose(_f);}}

		std::vector<BYTE> cipher_private;
		bool ok = RSA_OAEP_Encrypt_BCrypt(
			g_NetIF->srvModulus, g_NetIF->srvExponent,
			g_NetIF->cliPrivateKey.data(), (DWORD)g_NetIF->cliPrivateKey.size(),
			cipher_private);
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] post RSA_OAEP ok=%d cipherLen=%llu\n",ok,(unsigned long long)cipher_private.size());fflush(_f);fclose(_f);}}
		if (!ok) {
			return false;
		}

		static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		string base64encoded_private;
		{
			const unsigned char* src = cipher_private.data();
			size_t len = cipher_private.size();
			base64encoded_private.reserve(((len + 2) / 3) * 4);
			for (size_t i = 0; i < len; i += 3) {
				unsigned int val = (unsigned int)src[i] << 16;
				if (i + 1 < len) val |= (unsigned int)src[i + 1] << 8;
				if (i + 2 < len) val |= (unsigned int)src[i + 2];
				base64encoded_private += b64[(val >> 18) & 0x3F];
				base64encoded_private += b64[(val >> 12) & 0x3F];
				base64encoded_private += (i + 1 < len) ? b64[(val >> 6) & 0x3F] : '=';
				base64encoded_private += (i + 2 < len) ? b64[val & 0x3F] : '=';
			}
		}
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] base64 len=%llu\n",(unsigned long long)base64encoded_private.size());fflush(_f);fclose(_f);}}

		WPacket pk = pCNetIf->GetWPacket();
		pk.WriteCmd(CMD_CM_SEND_PRIVATE_KEY);
		pk.WriteString(base64encoded_private.c_str());
		pCNetIf->SendPacketMessage(pk);
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] packet sent\n");fflush(_f);fclose(_f);}}

		g_NetIF->handshakeDone = true;
		g_NetIF->_comm_enc = true;
		g_NetIF->m_connect.CHAPSTR(false);
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] done OK\n");fflush(_f);fclose(_f);}}
		return true;
	}
	catch (const std::exception& ex) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] exception: %s\n",ex.what());fflush(_f);fclose(_f);}}
		return false;
	}
	catch (...) {
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[SPRK] unknown exception\n");fflush(_f);fclose(_f);}}
		return false;
	}
}


void CProCirculate::Login(const char *accounts,const char *password, const char* passport)
{
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] enter handshakeDone=%d _comm_enc=%d\n",g_NetIF->handshakeDone,(int)g_NetIF->_comm_enc);fflush(_f);fclose(_f);}}
	if (!g_NetIF->handshakeDone) 
	{
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] abort: handshake not done\n");fflush(_f);fclose(_f);}}
		return;
	}

	extern short g_sClientVer;
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGIN);
	pk.WriteString(accounts);
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] pre hash\n");fflush(_f);fclose(_f);}}

	string hexencoded;
	{
		uint8_t dig[32];
		blake2s_hash(password, strlen(password), dig);
		static const char hx[] = "0123456789ABCDEF";
		hexencoded.reserve(64);
		for (int i = 0; i < 32; i++) {
			hexencoded += hx[(dig[i] >> 4) & 0xF];
			hexencoded += hx[dig[i] & 0xF];
		}
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] blake2s OK hex=%s\n",hexencoded.c_str());fflush(_f);fclose(_f);}}
	}

	pk.WriteString(hexencoded.c_str());
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] post hash hex=%s\n",hexencoded.c_str());fflush(_f);fclose(_f);}}
	string strMac=GetMacString();
	if (strMac.empty()) strMac="Unknown";
	pk.WriteString(strMac.c_str());
	pk.WriteShort(911);
	pk.WriteShort(g_sClientVer);

	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] pre send\n");fflush(_f);fclose(_f);}}
	pCNetIf->SendPacketMessage(pk);
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[LOGIN] post send OK\n");fflush(_f);fclose(_f);}}
}

void CProCirculate::Logout()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_LOGOUT);
	pCNetIf->SyncSendPacketMessage(pk,3000);
}

void CProCirculate::BeginPlay(char cha_index)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_BGNPLAY);
	pk.WriteChar(cha_index);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::EndPlay()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_ENDPLAY);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::NewCha(const char* chaname, const char* birth, int type, int hair, int face)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_NEWCHA);
	pk.WriteString(chaname);
	pk.WriteString(birth);
	pk.WriteLong(type);
	pk.WriteLong(hair);
	pk.WriteLong(face);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::DelCha(uint8_t cha_index, const char szPassword2[])
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_DELCHA);
	pk.WriteChar(cha_index);
	pk.WriteString(szPassword2);

	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::OpenRankings()
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_RANK);
	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::Say(const char *content)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SAY);			
	pk.WriteSequence(content, uShort(strlen(content))+1);
	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::SynBaseAttribute(CChaAttr *pCAttr)
{
	char	chAttrNum = 0;
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
		if (pCAttr->GetChangeBitFlag(i))
			chAttrNum++;

	if (chAttrNum == 0)
		return;

	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SYNATTR);	
	pk.WriteChar(chAttrNum);
	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
		{
			pk.WriteShort(i);
			pk.WriteLong((uLong)pCAttr->GetAttr(i));
		}
	}

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, RES_STRING(CMISS_000325));

	for (int i = ATTR_STR; i <= ATTR_LUK; i++)
	{
		if (pCAttr->GetChangeBitFlag(i))
			LG(szReqChangeAttr, RES_STRING(CL_LANGUAGE_MATCH_326), i, pCAttr->GetAttr(i));
	}
	LG(szReqChangeAttr, "\n");

	pCNetIf->SendPacketMessage(pk);
}


void CProCirculate::RefreshChaData(long lWorldID, long lHandle)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_REFRESH_DATA);	
	pk.WriteLong(lWorldID);
	pk.WriteLong(lHandle);

	pCNetIf->SendPacketMessage(pk);
}

void CProCirculate::SkillUpgrade(short sSkillID, char chAddLv)
{
	WPacket pk	=pCNetIf->GetWPacket();
	pk.WriteCmd(CMD_CM_SKILLUPGRADE);	
	pk.WriteShort(sSkillID);
	pk.WriteChar(chAddLv);

	// log
	char szReqChangeAttr[256] = {0};
	strcpy(szReqChangeAttr, RES_STRING(CL_LANGUAGE_MATCH_327));

	LG(szReqChangeAttr, RES_STRING(CMISS_000328), sSkillID, chAddLv);

	pCNetIf->SendPacketMessage(pk);
}


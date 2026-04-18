#include "StdAfx.h"
#pragma warning(disable: 4018)
#include "NetIF.h"
#include "GameApp.h"
#include "GameAppMsg.h"
#include "Character.h"
#pragma warning(default: 4018)
#include "Algo.h"
#include "procirculate.h"
#include "GameConfig.h"
#include <vector>
#include <string>
#include "ProCirculate.h"
//=============��������ͷ�ļ�BEGIN=============
#include "PacketCmd.h"
#include "NetChat.h"
#include "NetGuild.h"
#include "uiequipform.h"
#include "uiglobalvar.h"
#include <fstream>
#include <iostream>
#ifdef _TEST_CLIENT
#include "..\..\TestClient\testclient.h"
#endif
//=============��������ͷ�ļ�END===============

//�κ��˲����޸�����2�У�лл������
uLong	NetBuffer[]		= {100,10,0};
bool	g_logautobak	= false;

//End
NetIF		*g_NetIF;
CLogName	 g_LogName;

extern short g_sClientVer;

#include "uidoublepwdform.h"
extern CDoublePwdMgr	g_stUIDoublePwd;
using namespace std;

//-------------------
// Packet��Ϣ��������
//-------------------

BOOL NetIF::HandlePacketMessage(DataSocket *datasock,LPRPACKET pk)
{
		
	if (!static_cast<bool>(pk)) return FALSE;
	unsigned short sCmdType = pk.ReadCmd();
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[HPM] cmd=%d\n",(int)sCmdType);fflush(_f);fclose(_f);}}

#ifdef _TEST_CLIENT
    m_connect.SwitchSocket( datasock, Connection::CNST_CONNECTED );
#endif

	BOOL bRet = FALSE;
	switch(sCmdType)
	{
	case CMD_MC_LOGIN:		 return SC_Login(pk);
	case CMD_MC_ENTERMAP:	 return SC_EnterMap(pk);
	case CMD_MC_BGNPLAY:     return SC_BeginPlay(pk);
	case CMD_MC_ENDPLAY:	 return SC_EndPlay(pk);
	case CMD_MC_NEWCHA:		 return SC_NewCha(pk);
	case CMD_MC_DELCHA:		 return SC_DelCha(pk);//===
	case CMD_MC_CREATE_PASSWORD2: return SC_CreatePassword2(pk);
	case CMD_MC_UPDATE_PASSWORD2: return SC_UpdatePassword2(pk);
	case CMD_MC_CHABEGINSEE: return SC_ChaBeginSee(pk);
	case CMD_MC_CHAENDSEE:	 return SC_ChaEndSee(pk);
	case CMD_MC_ADD_ITEM_CHA: return SC_AddItemCha(pk);
	case CMD_MC_DEL_ITEM_CHA: return SC_DelItemCha(pk);
	case CMD_MC_NOTIACTION:	 return SC_CharacterAction(pk);
	case CMD_MC_SAY:		 return SC_Say(pk);
	case CMD_MC_SYSINFO:	 return SC_SysInfo(pk);

	case CMD_PC_GUILDNOTICE:	 return SC_GuildInfo(pk);

	case CMD_MC_POPUP_NOTICE: return SC_PopupNotice(pk);
	case CMD_MC_ITEMBEGINSEE:return SC_ItemCreate(pk);
	case CMD_MC_ITEMENDSEE:	 return SC_ItemDestroy(pk);
	case CMD_MC_TALKPAGE:	 return SC_TalkInfo(pk);
	case CMD_MC_FUNCPAGE:	 return SC_FuncInfo(pk);
	case CMD_MC_CLOSETALK:	 return SC_CloseTalk(pk);
	case CMD_MC_HELPINFO:	 return SC_HelpInfo(pk);
	case CMD_MC_TRADEPAGE:	 return SC_TradeInfo(pk);
	case CMD_MC_BLACKMARKET_TRADEUPDATE: return SC_TradeUpdate(pk);
	case CMD_MC_TRADE_DATA:  return SC_TradeData(pk);
	case CMD_MC_TRADE_ALLDATA: return SC_TradeAllData(pk);
	case CMD_MC_TRADERESULT: return SC_TradeResult(pk);
	case CMD_MC_CHARTRADE:	 return SC_CharTradeInfo(pk);
	case CMD_MC_SYNATTR:	 return SC_SynAttribute(pk);
	case CMD_MC_SYNSKILLBAG: return SC_SynSkillBag(pk);
	case CMD_MC_SYNDEFAULTSKILL: return SC_SynDefaultSkill(pk);
	case CMD_MC_SYNASKILLSTATE: return SC_SynSkillState(pk);
	case CMD_MC_MISSION:	 return SC_MissionInfo(pk);
	case CMD_MC_MISPAGE:	 return SC_MisPage(pk);
	case CMD_MC_MISLOG:		 return SC_MisLog(pk);
	case CMD_MC_MISLOGINFO:	 return SC_MisLogInfo(pk);
	case CMD_MC_MISLOG_CLEAR: return SC_MisLogClear(pk);
	case CMD_MC_MISLOG_ADD:	 return SC_MisLogAdd(pk);
	case CMD_MC_MISLOG_CHANGE: return SC_MisLogState(pk);
	case CMD_MC_NPCSTATECHG: return SC_NpcStateChange(pk);
	
	case CMD_PC_ERRMSG :
	case CMD_MC_MAPCRASH:    return SC_MapCrash(pk);  
	
	case CMD_MC_TRIGGER_ACTION: return SC_TriggerAction(pk);
	case CMD_MC_BEGIN_ITEM_FORGE: return SC_Forge(pk);

	case CMD_MC_BEGIN_ITEM_UNITE: return SC_Unite(pk);
	case CMD_MC_BEGIN_ITEM_MILLING: return SC_Milling(pk);
	case CMD_MC_BEGIN_ITEM_FUSION:  return SC_Fusion(pk);
	case CMD_MC_BEGIN_ITEM_UPGRADE: return SC_Upgrade(pk);
	case CMD_MC_BEGIN_ITEM_EIDOLON_METEMPSYCHOSIS: return SC_EidolonMetempsychosis(pk);
	case CMD_MC_BEGIN_ITEM_EIDOLON_FUSION: return SC_Eidolon_Fusion(pk);
	case CMD_MC_BEGIN_ITEM_PURIFY: return SC_Purify(pk);
	case CMD_MC_BEGIN_ITEM_ENERGY: return SC_Energy(pk);
	case CMD_MC_BEGIN_GET_STONE: return SC_GetStone(pk);
	case CMD_MC_BEGIN_TIGER: return SC_Tiger(pk);
	case CMD_MC_BEGIN_ITEM_FIX: return SC_Fix(pk);
	case CMD_MC_BEGIN_GM_SEND: return SC_GMSend(pk);
	case CMD_MC_BEGIN_GM_RECV: return SC_GMRecv(pk);

	case CMD_MC_CREATEBOAT:	 return SC_CreateBoat(pk);
	case CMD_MC_UPDATEBOAT:	 return SC_UpdateBoat(pk);
	case CMD_MC_UPDATEBOAT_PART: return SC_UpdateBoatPart(pk);
	case CMD_MC_BERTH_LIST:	 return SC_BoatList(pk);
	case CMD_MC_BOATINFO:	 return SC_BoatInfo(pk);
	//case CMD_MC_BOAT_BAGLIST: return SC_BoatBagList(pk);
	case CMD_MC_BICKER_NOTICE: return SC_BickerNotice(pk);
	case CMD_MC_COLOUR_NOTICE: return SC_ColourNotice(pk);
	case CMD_MC_STALL_ALLDATA: return SC_StallInfo(pk);
	//case CMD_MC_STALL_UPDATE: return ;
	case CMD_MC_STALL_DELGOODS: return SC_StallDelGoods(pk);
	case CMD_MC_STALL_CLOSE: return SC_StallClose(pk);
	case CMD_MC_STALL_START: return SC_StallSuccess(pk);
	case CMD_MC_STARTEXIT: return SC_StartExit(pk);
	case CMD_MC_CANCELEXIT: return SC_CancelExit(pk);
	case CMD_MC_RANK: return SC_ShowRanking(pk);
	case CMD_MC_STALLSEARCH: return SC_ShowStallSearch(pk);
	case CMD_MC_UPDATEGUILDBANKGOLD : return SC_UpdateGuildGold(pk);
	
	case CMD_MC_UPDATEIMP : g_stUIEquip.UpdateIMP(pk.ReadLong()); return true;

	case CMD_MC_REQUESTPIN: {
								CCursor::I()->SetCursor(	CCursor::stNormal	);
								g_stUIDoublePwd.SetType(CDoublePwdMgr::MC_REQUEST);
								g_stUIDoublePwd.ShowDoublePwdForm();
								return true;
							}
	
	
	case CMD_PC_SAY2YOU:	 return PC_Say2You(pk);
	case CMD_PC_SAY2TEM:	 return PC_Say2Team(pk);
	case CMD_PC_SAY2GUD:	 return PC_Say2Gud(pk);
	case CMD_PC_SAY2ALL:	 {return PC_Say2All(pk);}
	case CMD_PC_SAY2TRADE:	 return PC_SAY2TRADE(pk);
	case CMD_PC_GM1SAY:		 return PC_GM1SAY(pk);
	case CMD_PC_GM1SAY1:	return PC_GM1SAY1(pk);//add by sunny.sun20080804
	
	case CMD_PC_SESS_CREATE:	return PC_SESS_CREATE(pk);
	case CMD_PC_SESS_ADD:	return PC_SESS_ADD(pk);
	case CMD_PC_SESS_LEAVE:	return PC_SESS_LEAVE(pk);
	case CMD_PC_SESS_SAY:	return PC_SESS_SAY(pk);

	case CMD_PC_FRND_INVITE:return PC_FRND_INVITE(pk);
	case CMD_PC_FRND_CANCEL:return PC_FRND_CANCEL(pk);
	case CMD_PC_FRND_REFRESH:return PC_FRND_REFRESH(pk);
	case CMD_PC_FRND_REFRESH_INFO:return PC_FRND_REFRESH_INFO(pk);

	case CMD_PC_CHANGE_PERSONINFO:return PC_CHANGE_PERSONINFO(pk);
	case CMD_PC_REGISTER: return PC_REGISTER(pk);
	case CMD_PC_GM_INFO: return PC_GM_INFO(pk);

	case CMD_MC_GUILD_GETNAME:return MC_GUILD_GETNAME(pk);
	case CMD_MC_LISTGUILD:return MC_LISTGUILD(pk);
	case CMD_MC_GUILD_TRYFORCFM:return MC_GUILD_TRYFORCFM(pk);
	case CMD_MC_GUILD_LISTTRYPLAYER:return MC_GUILD_LISTTRYPLAYER(pk);
	case CMD_PC_GUILD:return PC_GUILD(pk);
	case CMD_PC_GUILD_PERM:return PC_GUILD_PERM(pk);
	case CMD_MC_GUILD_MOTTO:return MC_GUILD_MOTTO(pk);
	case CMD_MC_GUILD_LEAVE:	return MC_GUILD_LEAVE(pk);
	case CMD_MC_GUILD_KICK:		return MC_GUILD_KICK(pk);
	case CMD_MC_GUILD_INFO: return MC_GUILD_INFO(pk);
	case CMD_MC_GUILD_LISTCHALL: return MC_GUILD_LISTCHALL(pk);
	case CMD_PC_TEAM_INVITE:return PC_TEAM_INVITE(pk);
	case CMD_PC_TEAM_CANCEL:return PC_TEAM_CANCEL(pk);
	case CMD_PC_TEAM_REFRESH:return PC_TEAM_REFRESH(pk);
	case CMD_MC_TEAM:		 return SC_SynTeam(pk);
	case CMD_MC_TLEADER_ID:    return SC_SynTLeaderID(pk);

	case CMD_MC_FAILEDACTION:return SC_FailedAction(pk);
	case CMD_MC_MESSAGE:     return SC_Message(pk);
	case CMD_MC_ASTATEBEGINSEE: return SC_AStateBeginSee(pk);
	case CMD_MC_ASTATEENDSEE:return SC_AStateEndSee(pk);
	case CMD_MC_CHA_EMOTION: return SC_Cha_Emotion(pk);

	case CMD_MC_QUERY_CHA: return SC_QueryCha(pk);
	case CMD_MM_QUERY_CHAITEM: return SC_QueryChaItem(pk);
	case CMD_MC_QUERY_CHAPING: return SC_QueryChaPing(pk);
	case CMD_MC_QUERY_RELIVE: return SC_QueryRelive(pk);
	case CMD_MC_SEND_SERVER_PUBLIC_KEY: return SC_SendPublicKey(pk);
	case CMD_MC_SEND_HANDSHAKE: return SC_SendHandshake(pk);
	case CMD_MC_PREMOVE_TIME:	return SC_PreMoveTime(pk);
	case CMD_MC_MAP_MASK:	return SC_MapMask(pk);

	case CMD_MC_UPDATEHAIR_RES: return SC_UpdateHairRes(pk);
	case CMD_MC_OPENHAIR: return SC_OpenHairCut(pk);
	case CMD_MC_STALL_NAME: return SC_SynStallName(pk);
	case CMD_MC_EVENT_INFO: return SC_SynEventInfo(pk);
	case CMD_MC_SIDE_INFO: return SC_SynSideInfo(pk);

	case CMD_MC_TEAM_FIGHT_ASK: return SC_TeamFightAsk(pk);
	case CMD_MC_ITEM_REPAIR_ASK: return SC_ItemRepairAsk(pk);
	case CMD_MC_BEGIN_ITEM_REPAIR: return SC_BeginItemRepair(pk);
	case CMD_MC_APPEND_LOOK: return SC_SynAppendLook(pk);
	case CMD_MC_ITEM_FORGE_ASK: return SC_ItemForgeAsk(pk);
	case CMD_MC_ITEM_FORGE_ASR: return SC_ItemForgeAnswer(pk);

	case CMD_MC_ITEM_USE_SUC: return SC_ItemUseSuc(pk);
	case CMD_MC_KITBAG_CAPACITY: return SC_KitbagCapacity(pk);
	case CMD_MC_ESPE_ITEM: return SC_EspeItem(pk);

    case CMD_MC_KITBAG_CHECK_ASR: return SC_KitbagCheckAnswer(pk);

	case CMD_MC_STORE_OPEN_ASR:	return SC_StoreOpenAnswer(pk);
	case CMD_MC_STORE_LIST_ASR:	return SC_StoreListAnswer(pk);
	case CMD_MC_STORE_BUY_ASR:	return SC_StoreBuyAnswer(pk);
	case CMD_MC_STORE_CHANGE_ASR:	return SC_StoreChangeAnswer(pk);
	case CMD_MC_STORE_QUERY:	return SC_StoreHistory(pk);
	case CMD_MC_STORE_VIP:	return SC_StoreVIP(pk);

	case CMD_MC_BLACKMARKET_EXCHANGEDATA: return SC_BlackMarketExchangeData(pk);
	case CMD_MC_BLACKMARKET_EXCHANGE_ASR: return SC_BlackMarketExchangeAsr(pk);
	case CMD_MC_BLACKMARKET_EXCHANGEUPDATE: return SC_BlackMarketExchangeUpdate(pk);
	case CMD_MC_EXCHANGEDATA: return SC_ExchangeData(pk);

	case CMD_MC_TIGER_ITEM_ID: return SC_TigerItemID(pk);

    case CMD_PC_GARNER2_ORDER: return PC_PKSilver(pk);

    case CMD_MC_LIFESKILL_BGING: return SC_LifeSkillShow(pk);
    case CMD_MC_LIFESKILL_ASK: return SC_LifeSkill(pk);
    case CMD_MC_LIFESKILL_ASR: return SC_LifeSkillAsr(pk);
	case CMD_CM_ITEM_LOCK_ASR:	return	SC_DropLockAsr(pk);
	case CMD_MC_ITEM_UNLOCK_ASR: return SC_UnlockItemAsr(pk);
	case CMD_MC_VOLUNTER_LIST: return SC_VolunteerList(pk);
	case CMD_MC_VOLUNTER_STATE: return SC_VolunteerState(pk);
	case CMD_MC_VOLUNTER_OPEN: return SC_VolunteerOpen(pk);
	case CMD_MC_VOLUNTER_ASK: return SC_VolunteerAsk(pk);

	case CMD_MC_KITBAGTEMP_SYNC: return SC_SyncKitbagTemp(pk);

	case CMD_MC_MASTER_ASK: return SC_MasterAsk(pk);
	case CMD_PC_MASTER_REFRESH: return PC_MasterRefresh(pk);
	case CMD_PC_MASTER_CANCEL: return PC_MasterCancel(pk);
	case CMD_PC_MASTER_REFRESH_INFO: return PC_MasterRefreshInfo(pk);
	case CMD_PC_PRENTICE_REFRESH_INFO: return PC_PrenticeRefreshInfo(pk);
	case CMD_MC_PRENTICE_ASK: return SC_PrenticeAsk(pk);
	case CMD_MC_TIGER_STOP: return SC_SyncTigerString(pk);

	case CMD_MC_CHAPLAYEFFECT: return SC_ChaPlayEffect(pk);

	case CMD_MC_SAY2CAMP: return SC_Say2Camp(pk);

	case CMD_MC_GM_MAIL: return SC_GMMail(pk);

	case CMD_MC_CHEAT_CHECK: return SC_CheatCheck(pk);
	case CMD_MC_LISTAUCTION: return SC_ListAuction(pk);
	case CMD_MC_RecDailyBuffInfo:  return SC_DailyBuffInfo(pk);

	case CMD_MC_REQUEST_DROP_RATE: return SC_RequestDropRate(pk);
	case CMD_MC_REQUEST_EXP_RATE: return SC_RequestExpRate(pk);
	case CMD_TC_DISCONNECT: return SC_Disconnect(pk);
	case CMD_PC_REFRESH_SELECT: return SC_RefreshSelectScreen(pk);
	}

	//LG("net_r", "msgRecv Unknown Packet Msg Value = [%u]!\n", sCmdType);
	return FALSE;
}

//---------------------------------------------------------------------------
// class CLogName
//---------------------------------------------------------------------------
CLogName::CLogName()
{
	Init();
}

void CLogName::Init()
{
	memset( _dwWorldArray, 0, sizeof(_dwWorldArray) );
	memset( _szLogName, 0, sizeof(_szLogName) );
	memset( _szNoFind, 0, sizeof(_szNoFind) );	
}

const char* CLogName::SetLogName( DWORD dwWorlID, const char* szName )
{
	if( !g_pGameApp->GetLGConfig()->bEnableAll )
		return "nolog";

	for( int i=0; i<LOG_MAX; i++ )
	{
		if( _dwWorldArray[i]==0 )
		{
			_dwWorldArray[i] = dwWorlID;
			sprintf( _szLogName[i], "%s+%u", szName, dwWorlID );
			return _szLogName[i];
		}
	}

	sprintf( _szNoFind, "nofind(s)-%u", dwWorlID );
	return _szNoFind;
}

const char* CLogName::GetLogName( DWORD dwWorlID )
{
	if( !g_pGameApp->GetLGConfig()->bEnableAll )
		return "nolog";

	for( int i=0; i<LOG_MAX; i++ )
		if( _dwWorldArray[i]==dwWorlID )
			return _szLogName[i];

	sprintf( _szNoFind, "nofind(g)-%u", dwWorlID );
	return _szNoFind;
}

const char* CLogName::GetMainLogName()
{
	if( !g_pGameApp->GetLGConfig()->bEnableAll )
		return "nolog";

	CCharacter *pMain = CGameScene::GetMainCha();
	if( pMain )
	{
		return GetLogName( pMain->getAttachID() );
	}

	sprintf( _szNoFind, "nofind(m)" );
	return _szNoFind;
}

bool CLogName::IsMainCha( DWORD dwWorlID )
{
	CCharacter *pMain = CGameScene::GetMainCha();
	if( pMain )
	{
		return pMain->getAttachID()==dwWorlID;
	}

	return false;
}

inline int lua_HandleNetMessage( lua_State* L )
{
	BOOL bValid = (lua_gettop(L)==2 && lua_islightuserdata( L, 1 ) && lua_islightuserdata(L, 2));
	if(!bValid)
	{
		return 0;
	}

	dbc::DataSocket* pSocket = (dbc::DataSocket *)lua_touserdata( L, 1 );
	dbc::RPacket* pPacket = (dbc::RPacket*)lua_touserdata( L, 2 );
	if( pSocket && pPacket )
	{
		g_NetIF->HandlePacketMessage( pSocket, *pPacket );
	}
	return 0;
}

//---------------------------------------------------------------------------
// class NetIF
//---------------------------------------------------------------------------
NetIF::NetIF(ThreadPool *comm):TcpClientApp(this,0,comm),RPCMGR(this),PKQueue(false),m_framedelay(40)
,m_maxdelay(0),m_curdelay(0),m_mindelay(0),m_pingid(0),m_connect(this)
,m_ulCurStatistic(0),m_ulPacketCount(1), _enc(false), _comm_enc(0)
{
#define _NIF_TRACE(msg) {FILE*_f=fopen("log\\netif_trace.log","a");if(_f){fprintf(_f,"%s\n",msg);fflush(_f);fclose(_f);}}
	_NIF_TRACE("[NIF] entered ctor");
	TcpCommApp::WSAStartup();
	_NIF_TRACE("[NIF] post WSAStartup");

    memset(_key, 0, sizeof _key);
    _key_len = 0;
	_NIF_TRACE("[NIF] pre init_lua");
    g_rLvm = init_lua();
    g_sLvm = init_lua();
	_NIF_TRACE("[NIF] pre load_luc");
    load_luc(g_rLvm, "scripts/lua/apple.luc");
    load_luc(g_sLvm, "scripts/lua/apple.luc");
    load_luc(g_rLvm, "scripts/lua/pear.luc");
    load_luc(g_sLvm, "scripts/lua/pear.luc");
	_NIF_TRACE("[NIF] post load_luc");

	handshakeDone = false;
	memset(m_ulDelayTime, 0, sizeof(dbc::uLong) * 4);
	_NIF_TRACE("[NIF] pre SetPKParse");
	SetPKParse(0,2,64*1024,100);
	_NIF_TRACE("[NIF] pre BeginWork");
	BeginWork( g_Config.m_nSendHeartbeat );
	_NIF_TRACE("[NIF] post BeginWork");

	cliPrivateKey.resize(16);
	{FILE*_f=fopen("log\\netif_trace.log","a");if(_f){fprintf(_f,"[NIF] rng=%p key=%p size=%zu\n",(void*)&rng,cliPrivateKey.data(),(size_t)cliPrivateKey.size());fflush(_f);fclose(_f);}}
	rng.GenerateBlock(cliPrivateKey.data(), cliPrivateKey.size());
	_NIF_TRACE("[NIF] post rng.GenerateBlock");

	m_pCProCir = new CProCirculateCC( this );
	_NIF_TRACE("[NIF] ctor done");
#undef _NIF_TRACE
}

NetIF::~NetIF()
{
    _enc = false;
    memset(_key, 0, sizeof _key);
    exit_lua(g_sLvm);
    exit_lua(g_rLvm);

    ShutDown(6*1000);

	if( m_pCProCir )
    {
        delete m_pCProCir;
        m_pCProCir = NULL;
    }

   TcpCommApp::WSACleanup();
}

void	NetIF::OnProcessData(dbc::DataSocket *datasock,dbc::RPacket& recvbuf){
	dbc::RPacket rpk =recvbuf;
	unsigned short sCmdType = rpk.ReadCmd();

	if(sCmdType !=CMD_MC_PING)
	{
		LG("FromGateServer","%d\n",sCmdType);
	}
	
	//LG("net_r", "Recv Packet Msg Type = [%d]\n", sCmdType);
	//===============for test
//	if(sCmdType == CMD_MC_NOTIACTION)
//	{
//		uLong l_id = rpk.ReadLong();
//
//		CCharacter *pCMainCha = NULL;
//		CGameScene *pCScen = NULL;
//		if (g_pGameApp && (pCScen = g_pGameApp->GetCurScene()))
//			pCMainCha = pCScen->GetMainCha();
//		if (pCMainCha && (l_id == pCMainCha->getAttachID()))
//		{
//#ifdef defPROTOCOL_HAVE_PACKETID
//			uLong ulPacketID = rpk.ReadLong();
//#endif
//			uLong ulActType = rpk.ReadChar();
//			Char szLogName[1024];
//			sprintf(szLogName, "%s--check", pCMainCha->getLogName());
//			LG(szLogName, "PacketID: %10d\tActType: %3d\tTime: %d\n", ulPacketID, ulActType, GetTickCount());
//		}
//	}
	//================

	if(sCmdType==CMD_PC_PING)
	{
		PC_Ping(rpk);
	}
	else if(sCmdType==CMD_MC_PING)
	{
		SC_Ping(rpk);
	}
	else if(sCmdType==CMD_MC_CHECK_PING)
	{
		SC_CheckPing(rpk);
	}
	else
	{
		AddPK(datasock, recvbuf);
	}
}

void NetIF::SwitchNet(bool isConnected)
{ 
    if( m_pCProCir ) delete m_pCProCir;
    if( isConnected )
    {
        m_pCProCir = new CProCirculateCS(this);
    }
    else
    {
        m_pCProCir = new CProCirculateCC(this);
    }
}

//����ֵ:true-��������,false-����������
bool	NetIF::OnConnect(DataSocket *datasock)
{
    LG("connect","\tOnConnect\n");

	datasock->SetRecvBuf(64*1024);
	datasock->SetSendBuf(64*1024);
	return	true;
}
//reasonֵ:0-���س��������˳���-1-Socket����-3-���类�Է��رգ�-5-�����ȳ������ơ�
void	NetIF::OnDisconnect(DataSocket *datasock,int reason)
{
	{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[DISC] reason=%d tick=%u\n",reason,GetCurrentTick());fflush(_f);fclose(_f);}}
	LG("connect","\tOnDisconnect, Reason:%d, Tick:%u,recvTime:%u  \n", reason, GetCurrentTick(),GetRecvTime(datasock) );

    if( g_pGameApp )
    {
		g_NetIF->handshakeDone = false;
        m_connect.OnDisconnect();
        g_pGameApp->SendMessage(APP_NET_DISCONNECT,reason);



        if (m_pCProCir)
        {
            delete m_pCProCir;
        }
        m_pCProCir = new CProCirculateCC( this );
    }
	return;
}

std::string NetIF::GetDisconnectErrText(int reason) const
{
	return [&]()-> std::string
	{
		switch (reason)
		{
		case -33: return "Offline mode has been successfully established, you may now close the client";
		default: return RES_STRING(CL_LANGUAGE_MATCH_138);
		}
	}();
}

// xuedong 2004.09.01
unsigned long NetIF::GetAveragePing()
{
	unsigned long ulAverage = 0, ulCount = 0;

	m_mutmov.lock();

	for (int i = 0; i < 4; i++)
	{
		if (m_ulDelayTime[i] <= 0)
			break;
		ulAverage += m_ulDelayTime[i];
		ulCount ++;
	}

	m_mutmov.unlock();

	if (ulCount > 0)
		ulAverage = (ulAverage + ulCount - 1) / ulCount;
	if (ulAverage <= 0)
		ulAverage = 1;

	return ulAverage;
}
//
void NetIF::ProcessData(dbc::DataSocket *datasock,dbc::RPacket& recvbuf)
{
	g_NetIF->HandlePacketMessage( datasock, recvbuf );
}

extern char g_szSendKey[4];
extern char g_szRecvKey[4];

bool NetIF::OnEncrypt(dbc::DataSocket *datasock,char *ciphertext, uLong ciphertext_len, const char *text,unsigned long& len)
{
	if (_comm_enc > 0 && g_NetIF->handshakeDone)
	{
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[ENC] pre AES len=%lu\n",len);fflush(_f);fclose(_f);}}
		bool r = EncryptAES(ciphertext, ciphertext_len, text, len, g_NetIF->cliPrivateKey);
		{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[ENC] post AES ok=%d newLen=%lu\n",r,len);fflush(_f);fclose(_f);}}
		return r;
	}

	if (ciphertext == text)
	{
		return true;
	}

	std::memcpy(ciphertext, text, len);
	return true;
}


bool NetIF::OnDecrypt(dbc::DataSocket *datasock,char *ciphertext,unsigned long& len)
 {
	try
	{
		if (_comm_enc > 0 && g_NetIF->handshakeDone)
		{
			{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[DEC] pre AES len=%lu\n",len);fflush(_f);fclose(_f);}}
			bool r = DecryptAES(ciphertext, ciphertext, len, g_NetIF->cliPrivateKey);
			{FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[DEC] post AES ok=%d newLen=%lu\n",r,len);fflush(_f);fclose(_f);}}
			return r;
		}
		else {
			return false;
		}
	}
    catch (...)
    {
        {FILE*_f=fopen("log\\connect.log","a");if(_f){fprintf(_f,"[DEC] exception\n");fflush(_f);fclose(_f);}}
	}
	return false;
}

//------------- Packet���ͺ���    Client -> Server ��Ϣ�����ܿ�----------------------------
void NetIF::SendPacketMessage(LPWPACKET pk)
{
	
	
	//pk.WriteShort(0xE44CF); //WPE CODE
	
	BOOL bUseFakeServer = FALSE;
	if(bUseFakeServer)
	{
		// FakeServer.HandleClientPacket(pk);
		return;
	}

	if(!IsConnected()) return;

	if(!m_connect.GetDatasock())
	{
		if ( !g_pGameApp->GetCurScene() )
		{
			LG("error", "msgg_pGameApp->GetCurScene() Is NULL, Can't Send Socket Message!\n");
			return;
		}
		if (!g_pGameApp->GetCurScene()->GetMainCha())
		{
			LG("error", "msgg_pGameApp->GetCurScene()->GetMainCha() Is NULL, Can't Send Socket Message!\n");
			return;
		}
		LG("error", "msgClientSocket Is NULL, Can't Send Socket Message!\n");
		return;
	}

	if( SendData(m_connect.GetDatasock(), pk)==-1 )
	{
		LG("net_error", "msgSendData Error!\n");
		
	}
}

dbc::RPacket NetIF::SyncSendPacketMessage(LPWPACKET pk,unsigned long timeout)
{
	BOOL bUseFakeServer = FALSE;
	if(bUseFakeServer)
	{
		// FakeServer.HandleClientPacket(pk);
		return 0;
	}

	if(!IsConnected()) return 0;

	if(!m_connect.GetDatasock())
	{
		if ( !g_pGameApp->GetCurScene() )
		{
			LG("error", "msgg_pGameApp->GetCurScene() Is NULL, Can't Send Socket Message!\n");
			return 0;
		}
		if (!g_pGameApp->GetCurScene()->GetMainCha())
		{
			LG("error", "msgg_pGameApp->GetCurScene()->GetMainCha() Is NULL, Can't Send Socket Message!\n");
			return 0;
		}
		LG("error", "msgClientSocket Is NULL, Can't Send Socket Message!\n");
		return 0;
	}

	return SyncCall(m_connect.GetDatasock(), pk,timeout);
}



static const char _b64e[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const unsigned char _b64d[256] = {
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
	 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,  0,255,255,
	255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
	255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

static std::string Base64Encode(const unsigned char* src, size_t len) {
	std::string out;
	out.reserve(((len + 2) / 3) * 4);
	for (size_t i = 0; i < len; i += 3) {
		unsigned int n = (unsigned int)src[i] << 16;
		if (i + 1 < len) n |= (unsigned int)src[i + 1] << 8;
		if (i + 2 < len) n |= (unsigned int)src[i + 2];
		out += _b64e[(n >> 18) & 0x3F];
		out += _b64e[(n >> 12) & 0x3F];
		out += (i + 1 < len) ? _b64e[(n >> 6) & 0x3F] : '=';
		out += (i + 2 < len) ? _b64e[n & 0x3F] : '=';
	}
	return out;
}

static std::vector<unsigned char> Base64Decode(const unsigned char* src, size_t len) {
	std::vector<unsigned char> out;
	out.reserve((len / 4) * 3);
	for (size_t i = 0; i + 3 < len; i += 4) {
		unsigned int a = _b64d[src[i]], b = _b64d[src[i+1]], c = _b64d[src[i+2]], d = _b64d[src[i+3]];
		if (a == 255 || b == 255) break;
		out.push_back((unsigned char)((a << 2) | (b >> 4)));
		if (c != 255 && src[i+2] != '=') { out.push_back((unsigned char)(((b & 0xF) << 4) | (c >> 2))); }
		if (d != 255 && src[i+3] != '=') { out.push_back((unsigned char)(((c & 0x3) << 6) | d)); }
	}
	return out;
}

bool NetIF::EncryptAES(char* ciphertext, uLong ciphertext_len, cChar* plaintext, unsigned long& ciphersize, const std::vector<uint8_t>& key) {
	const ULONG TAG_LEN = 12;
	BYTE iv[16];
	rng.GenerateBlock(iv, 16);

	BCRYPT_ALG_HANDLE hAlg = NULL;
	NTSTATUS st = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
	if (st != 0) return false;

	st = BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
	if (st != 0) { BCryptCloseAlgorithmProvider(hAlg, 0); return false; }

	BCRYPT_KEY_HANDLE hKey = NULL;
	st = BCryptGenerateSymmetricKey(hAlg, &hKey, NULL, 0, (PUCHAR)key.data(), (ULONG)key.size(), 0);
	if (st != 0) { BCryptCloseAlgorithmProvider(hAlg, 0); return false; }

	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
	authInfo.pbNonce = iv;
	authInfo.cbNonce = 16;

	BYTE tag[12];
	authInfo.pbTag = tag;
	authInfo.cbTag = TAG_LEN;

	ULONG cbResult = 0;
	std::vector<BYTE> encrypted(ciphersize + TAG_LEN);
	st = BCryptEncrypt(hKey, (PUCHAR)plaintext, ciphersize, &authInfo, NULL, 0, encrypted.data(), (ULONG)ciphersize, &cbResult, 0);
	BCryptDestroyKey(hKey);
	BCryptCloseAlgorithmProvider(hAlg, 0);
	if (st != 0) return false;

	std::vector<BYTE> combined(cbResult + TAG_LEN);
	memcpy(combined.data(), encrypted.data(), cbResult);
	memcpy(combined.data() + cbResult, tag, TAG_LEN);

	std::string b64 = Base64Encode(combined.data(), cbResult + TAG_LEN);

	if (ciphertext_len < b64.size() + 16 + 1) return false;

	memcpy(ciphertext, b64.c_str(), b64.size());
	ciphersize = (unsigned long)b64.size();
	memcpy(ciphertext + ciphersize + 1, iv, 16);
	ciphersize += 16 + 1;

	return true;
}

bool NetIF::DecryptAES(cChar* ciphertext, char* plaintext, uLong& ciphersize, const std::vector<uint8_t>& key)
{
	const ULONG TAG_LEN = 12;
	if (ciphersize < 17) return false;

	ciphersize -= 17;
	const BYTE* iv = (const BYTE*)ciphertext + ciphersize + 1;

	auto decoded = Base64Decode((const unsigned char*)ciphertext, ciphersize);
	if (decoded.size() < TAG_LEN) return false;

	ULONG dataLen = (ULONG)(decoded.size() - TAG_LEN);
	BYTE tag[12];
	memcpy(tag, decoded.data() + dataLen, TAG_LEN);

	BCRYPT_ALG_HANDLE hAlg = NULL;
	NTSTATUS st = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
	if (st != 0) return false;

	st = BCryptSetProperty(hAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_GCM, sizeof(BCRYPT_CHAIN_MODE_GCM), 0);
	if (st != 0) { BCryptCloseAlgorithmProvider(hAlg, 0); return false; }

	BCRYPT_KEY_HANDLE hKey = NULL;
	st = BCryptGenerateSymmetricKey(hAlg, &hKey, NULL, 0, (PUCHAR)key.data(), (ULONG)key.size(), 0);
	if (st != 0) { BCryptCloseAlgorithmProvider(hAlg, 0); return false; }

	BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO authInfo;
	BCRYPT_INIT_AUTH_MODE_INFO(authInfo);
	authInfo.pbNonce = (PUCHAR)iv;
	authInfo.cbNonce = 16;
	authInfo.pbTag = tag;
	authInfo.cbTag = TAG_LEN;

	ULONG cbResult = 0;
	std::vector<BYTE> decrypted(dataLen);
	st = BCryptDecrypt(hKey, decoded.data(), dataLen, &authInfo, NULL, 0, decrypted.data(), dataLen, &cbResult, 0);
	BCryptDestroyKey(hKey);
	BCryptCloseAlgorithmProvider(hAlg, 0);
	if (st != 0) return false;

	memcpy(plaintext, decrypted.data(), cbResult);
	ciphersize = cbResult;
	return true;
}

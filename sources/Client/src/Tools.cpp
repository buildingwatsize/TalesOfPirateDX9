#include "StdAfx.h"
#include "Tools.h"
#include <strstream> 

using namespace std;

string ConvertNumToChinese( int num )
{
	if( num==0 ) return RES_STRING(CMISS_000421);

	char szNum[255] = { 0 };
	itoa( num, szNum, 10 );

	char szBuf[255] = { 0 };
    std::ostrstream str( szBuf, sizeof(szBuf) );

	const char* pszPos = szNum;
	if( *pszPos=='-' )
	{
		str << RES_STRING(CMISS_000422);
		pszPos++;
	}

	static char szChinese[10][16];// = { RES_STRING(CMISS_000421), RES_STRING(CL_LANGUAGE_MATCH_423), RES_STRING(CL_LANGUAGE_MATCH_424), RES_STRING(CL_LANGUAGE_MATCH_425), RES_STRING(CL_LANGUAGE_MATCH_426), RES_STRING(CL_LANGUAGE_MATCH_427), RES_STRING(CL_LANGUAGE_MATCH_428), RES_STRING(CL_LANGUAGE_MATCH_429), RES_STRING(CL_LANGUAGE_MATCH_430), RES_STRING(CL_LANGUAGE_MATCH_431) };
	strcpy(szChinese[0], RES_STRING(CMISS_000421));
	strcpy(szChinese[1], RES_STRING(CL_LANGUAGE_MATCH_423));
	strcpy(szChinese[2], RES_STRING(CL_LANGUAGE_MATCH_424));
	strcpy(szChinese[3], RES_STRING(CL_LANGUAGE_MATCH_425));
	strcpy(szChinese[4], RES_STRING(CL_LANGUAGE_MATCH_426));
	strcpy(szChinese[5], RES_STRING(CL_LANGUAGE_MATCH_427));
	strcpy(szChinese[6], RES_STRING(CL_LANGUAGE_MATCH_428));
	strcpy(szChinese[7], RES_STRING(CL_LANGUAGE_MATCH_429));
	strcpy(szChinese[8], RES_STRING(CL_LANGUAGE_MATCH_430));
	strcpy(szChinese[9], RES_STRING(CL_LANGUAGE_MATCH_431));

	static char szHigh[8][16];// = { RES_STRING(CL_LANGUAGE_MATCH_432), RES_STRING(CL_LANGUAGE_MATCH_433), RES_STRING(CL_LANGUAGE_MATCH_434), RES_STRING(CMISS_000435), RES_STRING(CL_LANGUAGE_MATCH_432), RES_STRING(CL_LANGUAGE_MATCH_433), RES_STRING(CL_LANGUAGE_MATCH_434), RES_STRING(CMISS_000436) };
	strcpy(szHigh[0], RES_STRING(CL_LANGUAGE_MATCH_432));
	strcpy(szHigh[1], RES_STRING(CL_LANGUAGE_MATCH_433));
	strcpy(szHigh[2], RES_STRING(CL_LANGUAGE_MATCH_434));
	strcpy(szHigh[3], RES_STRING(CMISS_000435));
	strcpy(szHigh[4], RES_STRING(CL_LANGUAGE_MATCH_432));
	strcpy(szHigh[5], RES_STRING(CL_LANGUAGE_MATCH_433));
	strcpy(szHigh[6], RES_STRING(CL_LANGUAGE_MATCH_434));
	strcpy(szHigh[7], RES_STRING(CMISS_000436));
	
	char nChar = 0;
	int nZeroNum = 0;				
	bool IsBigMark = false;		// �������ڵ����⴦������ȫ����ʱ
	int nHigh = 0;
	int nLen = 0;
	while( *pszPos )
	{
		nChar = *pszPos++;
		nLen = (int)strlen(pszPos);

		// �в��Ķ����ϲ�Ϊһ����,��β������Ҳ����ʾ
		if( nChar=='0' )
		{
			nZeroNum++;
			if( IsBigMark && (nLen==4 || nLen==8) )
			{
				str << szHigh[ nLen-1 ];
				IsBigMark = false;
			}
			continue;
		}
		else
		{
			IsBigMark = true;
			if( nZeroNum>0 )    // ���㿪ʼ��,�����ĵ�һ������
			{
				nZeroNum = 0;
				str << RES_STRING(CMISS_000421);
			}
			str << szChinese[ nChar - '0' ];
			if( nLen > 0 )
			{
				if( nLen < 9 )
				{
					nHigh = nLen-1;
				}
				else
				{
					nLen = nLen % 9;
					nHigh = nLen;
				}
				str << szHigh[nHigh];

				// ���ڵ����⴦��
				if( nHigh==3 || nHigh==7 )
				{
					IsBigMark = false;
				}
			}
		}
	}
    str << ends;

	// ͷΪһʮ,ʡ��һ
	string rv = str.str();
	if( rv.length()>=4 && rv.substr( 0, 4 ) == RES_STRING(CMISS_000437) )
		return rv.substr( 2, rv.length() );

	return rv;
}

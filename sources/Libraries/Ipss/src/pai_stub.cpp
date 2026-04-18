#include <windows.h>
#include "../include/PAI.h"
#include "../include/BTI.h"

#pragma warning(disable: 4100)

extern "C" {

VOID   ipPAI_Init(BSTR *Location) {}
LONG   ipPAI_Login(LONG HWnd, LONG Code, LONG Port, BSTR *USER, BSTR *PASS, BSTR *GSVR) { return cPAI_Fail; }
LPCSTR ipPAI_Data()     { return ""; }
LPCSTR ipPAI_Reply()    { return ""; }
LPCSTR ipPAI_Passport() { return ""; }
LONG   ipPAI_Version()  { return 0; }

LONG ipBTI_Initial(LONG HWnd)    { return 0; }
LONG ipBTI_Terminate()            { return 0; }
LONG ipBTI_Open(BSTR *IP_A, BSTR *IP_B) { return 0; }
LONG ipBTI_Close()                { return 0; }
LONG ipBTI_IPLong(BSTR *IPText)  { return 0; }
LPCSTR ipBTI_IPText(LONG IPLong) { return "0.0.0.0"; }
LONG ipBTI_GAME(BSTR *Player, BSTR *Passport) { return 0; }
LONG ipBTI_STOP(BSTR *Player, LONG ErrNo) { return 0; }
LONG ipBTI_BUYS(BSTR *Player, LONG Credit, BSTR *ObjName, LONG ObjCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo) { return 0; }
LONG ipBTI_SALE(BSTR *Player, LONG Credit, BSTR *ObjName, LONG ObjCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo) { return 0; }
LONG ipBTI_DEAL(BSTR *Player_P, BSTR *Player_M, LONG Credit, BSTR *ObjName, LONG ObjCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo) { return 0; }
LONG ipBTI_GIVE(BSTR *Player, LONG Credit, BSTR *SvcName, LONG SvcCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo) { return 0; }
LONG ipBTI_BILL(BSTR *Player, LONG Credit, BSTR *SvcName, LONG SvcCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo) { return 0; }
LONG ipBTI_DATA(BSTR *Player, LONG InfoCode) { return 0; }
LONG ipBTI_TIME(BSTR **PlayerList, LONG Hour, LONG Number) { return 0; }

}

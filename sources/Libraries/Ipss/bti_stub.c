/* BTI stub for x64 -- original BTI.lib is x86 only, no source available */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <oleauto.h>

LONG ipBTI_Initial(LONG HWnd)                                              { return 1; }
LONG ipBTI_Terminate(void)                                                  { return 1; }
LONG ipBTI_Open(BSTR *IP_A, BSTR *IP_B)                                   { return 0; }
LONG ipBTI_Close(void)                                                      { return 0; }
LONG ipBTI_IPLong(BSTR *IPText)                                            { return 0; }
LPCSTR ipBTI_IPText(LONG IPLong)                                           { return ""; }
LONG ipBTI_GAME(BSTR *Player, BSTR *Passport)                             { return 0; }
LONG ipBTI_STOP(BSTR *Player, LONG ErrNo)                                  { return 0; }
LONG ipBTI_BUYS(BSTR *Player, LONG Credit, BSTR *ObjName, LONG ObjCount,
    BSTR *OrdID, BSTR *OrdTime, BSTR *Memo)                               { return 0; }
LONG ipBTI_SALE(BSTR *Player, LONG Credit, BSTR *ObjName, LONG ObjCount,
    BSTR *OrdID, BSTR *OrdTime, BSTR *Memo)                               { return 0; }
LONG ipBTI_DEAL(BSTR *Player_P, BSTR *Player_M, LONG Credit, BSTR *ObjName,
    LONG ObjCount, BSTR *OrdID, BSTR *OrdTime, BSTR *Memo)                { return 0; }
LONG ipBTI_GIVE(BSTR *Player, LONG Credit, BSTR *SvcName, LONG SvcCount,
    BSTR *OrdID, BSTR *OrdTime, BSTR *Memo)                               { return 0; }
LONG ipBTI_BILL(BSTR *Player, LONG Credit, BSTR *SvcName, LONG SvcCount,
    BSTR *OrdID, BSTR *OrdTime, BSTR *Memo)                               { return 0; }
LONG ipBTI_DATA(BSTR *Player, LONG InfoCode)                               { return 0; }
LONG ipBTI_TIME(BSTR **PlayerList, LONG Hour, LONG Number)                 { return 0; }

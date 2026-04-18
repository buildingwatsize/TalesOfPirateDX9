#include "stdafx.h"
#include "caLua.h"
#include "script.h"
#include "scene.h"
#include "GameApp.h"
#include "uiformmgr.h"
#include "PacketCmd.h"
#include "cameractrl.h"
#include "UISystemForm.h"

void CGameApp::LoadScriptScene( eSceneType eType )
{
	switch( eType )
	{
	    case enumLoginScene:        LoadScriptScene("scripts/lua/scene/loginscene.bin");		break;
	    case enumSelectChaScene:    LoadScriptScene("scripts/lua/scene/selectchascene.bin");    break;
	    case enumCreateChaScene:    LoadScriptScene("scripts/lua/scene/createchascene.bin");    break;
		case enumWorldScene:        LoadScriptScene("scripts/lua/scene/mainscene.bin");			break;

	}
}

void CGameApp::LoadScriptScene( const char* script_file )
{
	//try
	//{
		CLU_LoadScript((char*)script_file, 0);
	//}
	//catch(...)
	//{
	//	LG( "script", "msgLoad Scene Script[%s] Error", script_file );
	//}
}

//---------------------------------------------------------------------------
// App_Script
//---------------------------------------------------------------------------
int GP_SetCameraPos( double ex, double ey, double ez, double rx, double ry, double rz )
{
	CCameraCtrl *pCam = g_pGameApp->GetMainCam();
	pCam->m_EyePos.x = (float)ex;
	pCam->m_EyePos.y = (float)ey;
	pCam->m_EyePos.z = (float)ez; 
	pCam->m_RefPos.x = (float)rx;
	pCam->m_RefPos.y = (float)ry;
	pCam->m_RefPos.z = (float)rz;

	return R_OK;
}

int GP_GotoScene( int sceneid )
{
    CGameScene * p = dynamic_cast<CGameScene*>(CScript::GetScriptObj(sceneid));
    if( !p ) return R_FAIL;

    g_pGameApp->GotoScene( p );
    return R_OK;
}

//---------------------------------------------------------------------------
// ScriptRegedit
//---------------------------------------------------------------------------
void MPInitLua_App()
{
	CLU_RegisterFunction("GP_SetCameraPos", "int", "double,double,double,double,double,double", CLU_CDECL, CLU_CAST(GP_SetCameraPos));
    CLU_RegisterFunction("GP_GotoScene", "int", "int", CLU_CDECL, CLU_CAST(GP_GotoScene));
}


#include "stdafx.h"
#include "GameWG.h"
#include "PacketCmd.h"

#include <windows.h>
#include <process.h>
#include <tlhelp32.h>


CGameWG::CGameWG(void)
	: m_hThread(0)
{
}


CGameWG::~CGameWG(void)
{
	SafeTerminateThread();

	m_lstModule.clear();
}


// ˢ�µ�ǰ�������ģ��
bool CGameWG::RefreshModule(void)
{
    bool          bRet        = false;

	try
	{
		MODULEENTRY32 me32        = {0};
		std::string   strModule;

		std::unique_ptr<void, decltype(&CloseHandle)> hModuleSnap(CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ::GetCurrentProcessId()), &CloseHandle);
		if (hModuleSnap.get() == INVALID_HANDLE_VALUE)
			return false;

		me32.dwSize = sizeof(MODULEENTRY32); 

		if(Module32First(hModuleSnap.get(), &me32)) 
		{
			// ������ǰ�����������ģ��
			do
			{
				strModule = me32.szModule;
				m_lstModule.insert(strModule);
			}
			while(Module32Next(hModuleSnap.get(), &me32));

			bRet = true;
		}
		else
		{
			// ö��ʧ��
			bRet = false;
		}
	}
	catch(...)
	{
		
	}

    return bRet;
}


// �Ƿ�ʹ���ˡ�������ʹ�����
bool CGameWG::IsUseHdts(void)
{
	if (m_lstModule.contains("hookit.dll"))
	{
		return true;
	}

	return false;
}


// �����߳�
void CGameWG::BeginThread(void)
{
    m_hThread = (HANDLE)_beginthreadex(0, 0, Run, this, 0, 0);
}


// ��ȫ��ֹ�߳�
void CGameWG::SafeTerminateThread()
{
	if(m_hThread)
	{
		TerminateThread(m_hThread, 0);
		CloseHandle(m_hThread);

		m_hThread = 0;
	}
}


// �̻߳ص�
UINT CALLBACK CGameWG::Run(void* param)
{
	CGameWG* pGameWG = (CGameWG*)(param);

	for(;;)
	{
		Sleep(60 * 1000);	// һ����ˢһ��

		if(! g_NetIF || ! g_NetIF->IsConnected())
		{
			// ����δ����
			continue;
		}

		if(! pGameWG->RefreshModule())
		{
			// ˢ��ģ���б�
			continue;
		}

		if(pGameWG->IsUseHdts())
		{
			// ʹ������ҡ�������ʹ��

			CS_ReportWG(RES_STRING(CL_LANGUAGE_MATCH_143));
			break;
		}

	}

	return 0;
}


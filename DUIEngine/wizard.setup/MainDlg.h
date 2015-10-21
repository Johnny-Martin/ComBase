// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>
{
	struct VSENVCFG
	{
		CString strName;
		CString strVsDir;
		CString strDataTarget;
		CString strEntrySrc;
		CString strEntryTarget;
		CString strScriptSrc;
	};
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP_EX(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_BROWSE2, OnBrowseDuiEngineDir)
		COMMAND_ID_HANDLER(IDC_INSTALL, OnInstall)
		COMMAND_ID_HANDLER(IDC_UNINSTALL, OnUninstall)
		COMMAND_ID_HANDLER(IDC_SETOUTSOFT, OnSetoutsoft)
	END_MSG_MAP()

	CString m_strWizardDir;//����Ŀ¼

	CString GetVSDir(LPCTSTR pszEnvName)
	{
		CString strRet;
		strRet.GetEnvironmentVariable(pszEnvName);
		if(!strRet.IsEmpty()) strRet=strRet.Left(strRet.GetLength()-14);//14=length("Common7\Tools\")
		return strRet;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CListViewCtrl vslist;
		vslist.Attach(GetDlgItem(IDC_VSLIST));
		for(int i=0;i<vslist.GetItemCount();i++)
		{
			delete (VSENVCFG*)vslist.GetItemData(i);
		}
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

		CListViewCtrl vslist;
		vslist.Attach(GetDlgItem(IDC_VSLIST));
		ListView_SetExtendedListViewStyleEx(vslist.m_hWnd,LVS_EX_CHECKBOXES,LVS_EX_CHECKBOXES);
		
		TCHAR szDir[1000];
		GetCurrentDirectory(1000,szDir);
		m_strWizardDir=szDir;

		CString szVsList=m_strWizardDir+=_T("\\vslist.ini");
		int i=0;
		for(;;)
		{
			CString entry;
			entry.Format(_T("vs_%d"),++i);
			TCHAR szBuf[1000];
			if(0==GetPrivateProfileString(entry,_T("name"),NULL,szBuf,1000,szVsList))
				break;

			VSENVCFG *pEnvCfg=new VSENVCFG;
			pEnvCfg->strName=szBuf;
			GetPrivateProfileString(entry,_T("envname"),NULL,szBuf,1000,szVsList);
			pEnvCfg->strVsDir=GetVSDir(szBuf);
			if(pEnvCfg->strVsDir.IsEmpty())
			{
				delete pEnvCfg;
				continue;
			}
			GetPrivateProfileString(entry,_T("entryfilesrc"),NULL,szBuf,1000,szVsList);
			pEnvCfg->strEntrySrc=szBuf;
			GetPrivateProfileString(entry,_T("entryfiletarget"),NULL,szBuf,1000,szVsList);
			pEnvCfg->strEntryTarget=szBuf;
			GetPrivateProfileString(entry,_T("wizarddatatarget"),NULL,szBuf,1000,szVsList);
			pEnvCfg->strDataTarget=szBuf;
			
			GetPrivateProfileString(entry,_T("scriptsrc"),NULL,szBuf,1000,szVsList);
			pEnvCfg->strScriptSrc=szBuf;

			int iItem=vslist.InsertItem(vslist.GetItemCount(),pEnvCfg->strName);
			vslist.SetItemData(iItem,(DWORD_PTR)pEnvCfg);
		}

		TCHAR szPath[MAX_PATH];
		if(GetEnvironmentVariable(_T("DUIENGINEPATH"),szPath,MAX_PATH))
		{
			SetDlgItemText(IDC_DUIENGINEDIR,szPath);
		}else
		{
			GetCurrentDirectory(MAX_PATH,szPath);
			TCHAR *pUp=_tcsrchr(szPath,_T('\\'));
			if(pUp)
			{
				_tcscpy(pUp,_T("\\DuiEngine"));
				if(GetFileAttributes(szPath)!=INVALID_FILE_ATTRIBUTES)
				{
					*pUp=0;
					SetDlgItemText(IDC_DUIENGINEDIR,szPath);
				}
			}
		}

		return TRUE;
	}

	LRESULT OnSetoutsoft(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShellExecute(0,_T("open"),_T("http://www.setoutsoft.cn"),NULL,NULL,SW_SHOWNORMAL);
		return 0;
	}

	LRESULT OnBrowseDuiEngineDir(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CFolderDialog folderDlg;
		if(folderDlg.DoModal()==IDOK)
		{
			SetDlgItemText(IDC_DUIENGINEDIR,folderDlg.GetFolderPath());
		}
		return 0;
	}

	LRESULT OnInstall(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		SetCurrentDirectory(m_strWizardDir);

		CListViewCtrl vslist;
		vslist.Attach(GetDlgItem(IDC_VSLIST));

		if(GetFileAttributes(_T("DuiEngineWizard"))==INVALID_FILE_ATTRIBUTES)
		{
			MessageBox(_T("��ǰĿ¼��û���ҵ�DuiEngine��������"),_T("����"),MB_OK|MB_ICONSTOP);
			return 0;
		}
		TCHAR szDuiEngineDir[MAX_PATH]={0};
		int nLen=GetDlgItemText(IDC_DUIENGINEDIR,szDuiEngineDir,MAX_PATH);
		if(szDuiEngineDir[nLen-1]==_T('\\')) szDuiEngineDir[--nLen]=0;

		_tcscat(szDuiEngineDir,_T("\\DuiEngine"));
		if(GetFileAttributes(szDuiEngineDir)==INVALID_FILE_ATTRIBUTES)
		{
			MessageBox(_T("��ǰĿ¼��û���ҵ�DuiEngine��Դ����"),_T("����"),MB_OK|MB_ICONSTOP);
			return 0;
		}
		szDuiEngineDir[nLen]=0;
		//���û�������

		CRegKey reg;
		if(ERROR_SUCCESS==reg.Open(HKEY_LOCAL_MACHINE,_T("System\\CurrentControlSet\\Control\\Session Manager\\Environment"),KEY_SET_VALUE))
		{
			reg.SetStringValue(_T("DUIENGINEPATH"),szDuiEngineDir);
			reg.Close();
			::PostMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,(LPARAM)_T("Environment"));//�㲥���������޸���Ϣ��
		}else
		{
			MessageBox(_T("��ӻ�������ʧ��"),_T("����"),MB_OK|MB_ICONSTOP);
			return 0;
		}

		//׼�������ļ�
		TCHAR szFrom[1024]={0};
		TCHAR szTo[1024]={0};
		SHFILEOPSTRUCT shfo;
		shfo.pFrom=szFrom;
		shfo.pTo=szTo;

		for(int i=0;i<vslist.GetItemCount();i++)
		{
			if(!vslist.GetCheckState(i)) continue;

			VSENVCFG *pCfg=(VSENVCFG*)vslist.GetItemData(i);
			//����������
			BOOL bOK = TRUE;
			if(bOK)
			{
				shfo.wFunc=FO_COPY;
				shfo.fFlags=FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION;
				memset(szFrom,0,sizeof(szFrom));
				memset(szTo,0,sizeof(szTo));
				_tcscpy(szFrom,_T("DuiEngineWizard"));
				_tcscpy(szTo,pCfg->strVsDir);
				_tcscat(szTo,pCfg->strDataTarget);
				bOK = 0==SHFileOperation(&shfo);
			}
			//�����������
			if(bOK)
			{
				shfo.wFunc=FO_COPY;
				shfo.fFlags=FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION;
				memset(szFrom,0,sizeof(szFrom));
				memset(szTo,0,sizeof(szTo));
				_tcscpy(szFrom,pCfg->strEntrySrc);
				_tcscat(szFrom,_T("\\*.*"));
				_tcscpy(szTo,pCfg->strVsDir);
				_tcscat(szTo,pCfg->strEntryTarget);
				bOK = 0==SHFileOperation(&shfo);
			}
			//���ƽű�
			if(bOK)
			{
				shfo.wFunc=FO_COPY;
				memset(szFrom,0,sizeof(szFrom));
				memset(szTo,0,sizeof(szTo));
				_tcscpy(szFrom,pCfg->strScriptSrc);
				CString strTo=pCfg->strVsDir+pCfg->strDataTarget+_T("\\DuiEngineWizard\\Scripts\\2052");
				_tcscpy(szTo,strTo);
				bOK= 0==SHFileOperation(&shfo);
			}
			CString strMsg;
			strMsg.Format(_T("Ϊ%s��װDuiEngine��:%s"),pCfg->strName,bOK?_T("�ɹ�"):_T("ʧ��"));
			::SendMessage(GetDlgItem(IDC_LOG),LB_ADDSTRING,0,(LPARAM)(LPCTSTR)strMsg);
		}

		return 0;
	}

	LRESULT OnUninstall(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CListViewCtrl vslist;
		vslist.Attach(GetDlgItem(IDC_VSLIST));

		SHFILEOPSTRUCT shfo={0};
		shfo.pTo=NULL;
		shfo.wFunc=FO_DELETE;
		shfo.fFlags=FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION|FOF_SILENT;

		for(int i=0;i<vslist.GetItemCount();i++)
		{
			if(!vslist.GetCheckState(i)) continue;

			VSENVCFG *pCfg=(VSENVCFG*)vslist.GetItemData(i);
			BOOL bOK=TRUE;
			//remove data files
			if(bOK)
			{
				TCHAR szBuf[1000]={0};
				CString strFrom=pCfg->strVsDir+pCfg->strDataTarget+_T("\\DuiEngineWizard");
				_tcscpy(szBuf,strFrom);//ע����Ҫ������0��β
				shfo.pFrom=szBuf;
				bOK= 0==SHFileOperation(&shfo);
			}
			//remove entry files
			if(bOK)
			{
				CString strSource=pCfg->strVsDir+pCfg->strEntryTarget+_T("\\DuiEngineWizard.ico");
				DeleteFile(strSource);
				strSource=pCfg->strVsDir+pCfg->strEntryTarget+_T("\\DuiEngineWizard.vsdir");
				bOK = DeleteFile(strSource);
				strSource=pCfg->strVsDir+pCfg->strEntryTarget+_T("\\DuiEngineWizard.vsz");
				bOK = DeleteFile(strSource);
			}
			
			CString strMsg;
			strMsg.Format(_T("��%s��ж��DuiEngine��%s"),pCfg->strName,bOK?_T("�ɹ�"):_T("ʧ��"));
			::SendMessage(GetDlgItem(IDC_LOG),LB_ADDSTRING,0,(LPARAM)(LPCTSTR)strMsg);

		}
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}
};

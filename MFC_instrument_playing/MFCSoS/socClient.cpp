// socClient.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCSoS.h"
#include "socClient.h"
#include "afxdialogex.h"

#define PORT 9999

// socClient 대화 상자

IMPLEMENT_DYNAMIC(socClient, CDialog)

socClient::socClient(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG2, pParent)
	, connectip(_T(""))
{

}

socClient::~socClient()
{
}

void socClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, connectip);
	DDX_Control(pDX, IDC_LIST1, m_list_msg);
	DDX_Control(pDX, IDC_EDIT1, m_edit_send);


}


BEGIN_MESSAGE_MAP(socClient, CDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &socClient::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &socClient::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &socClient::OnBnClickedButton2)
END_MESSAGE_MAP()


// socClient 메시지 처리기


void socClient::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnSysCommand(nID, lParam);
}


void socClient::OnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	LPCTSTR lpctstr;
	UpdateData(TRUE);
	m_edit_send.GetWindowTextW(str);
	lpctstr = (LPCTSTR)str;
	//m_ClientSocket.Send((LPVOID)(LPCTSTR)str,str.GetLength() *2);
	m_ClientSocket.Send(lpctstr, lstrlen(lpctstr) * 2);
	m_edit_send.SetWindowTextW(_T(""));
	UpdateData(FALSE);
}

afx_msg LRESULT socClient::OnClientRecv(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(_T("선택하세요."));
	LPCTSTR lpszStr = (LPCTSTR)lParam;
	m_list_msg.InsertString(-1, lpszStr);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
	return 0;
}

void socClient::OnReceive(LPARAM lParam) {
	LPCTSTR lpszStr = (LPCTSTR)lParam;
	m_list_msg.InsertString(-1, lpszStr);
	m_list_msg.SetCurSel(m_list_msg.GetCount() - 1);
}

void socClient::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_ClientSocket.Connect(connectip, PORT) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
	}
	UpdateData(FALSE);
}


void socClient::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTmp, strMsg;
	int listmsg = m_list_msg.GetCurSel();
	if (listmsg != -1)
	{
		m_list_msg.GetText(listmsg, strTmp);
		strMsg.Append(strTmp + " ");
		//char *pChar = (LPSTR)(LPCTSTR)strMsg;
		int strLen = strMsg.GetLength();
		char *pChar = new char[strLen + 1];
		WideCharToMultiByte(CP_ACP, 0, strMsg, -1, pChar, strLen + 1, NULL, NULL);
		//CString msg;


		char* ptr = strtok(pChar, "/");
		ptr = strtok(NULL, "/");
		int save[900];
		int i = 0;
		// " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
		while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
		{
			save[i] = atoi(ptr);
			ptr = strtok(NULL, "/");      // 다음 문자열을 잘라서 포인터를 반환
			//msg.Format(_T("%S"), ptr);
			//MessageBox(msg);
			i++;
		}
		// " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
		int p = 0;
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 24; j++) {
				datasave[i][j] = save[p];
				p++;
			}
		}
		delete[] pChar;
	}
	else {
		AfxMessageBox(_T("선택하세요."));
	}
	OnOK();
}


BOOL socClient::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_edit_send.SetWindowTextW(savedata);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ClientSocket.SetParent(this);
	m_ClientSocket.SetWnd(this->m_hWnd);
	m_ClientSocket.Create();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

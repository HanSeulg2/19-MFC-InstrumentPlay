#pragma once
#include "CClientSocket.h"

// socClient 대화 상자

class socClient : public CDialog
{
	DECLARE_DYNAMIC(socClient)

public:
	socClient(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~socClient();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif
private:
	CClientSocket m_ClientSocket;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int datasave[100][100];
	CString connectip;
	CListBox m_list_msg;
	CEdit m_edit_send;
	CString savedata;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClickedButton1();
	afx_msg void OnBnClickedButton3();
	void socClient::OnReceive(LPARAM lParam);
public:
	afx_msg void OnBnClickedButton2();
	afx_msg LRESULT OnClientRecv(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
};

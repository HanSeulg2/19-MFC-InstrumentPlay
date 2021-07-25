#pragma once

#include "resource.h"
#include "MFCSoSDlg.h"
#include "socClient.h"
// Melody 대화 상자

class Melody : public CDialog
{
	DECLARE_DYNAMIC(Melody)

public:
	Melody(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Melody();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CRect arr[100][100];
	bool arrcheck[100][100];
	int milli;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedOk();
};

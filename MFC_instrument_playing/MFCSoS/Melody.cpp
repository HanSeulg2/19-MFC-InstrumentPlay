// Melody.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCSoS.h"
#include "Melody.h"
#include "afxdialogex.h"
//#include "MFCSoSDlg.cpp"


// Melody 대화 상자

IMPLEMENT_DYNAMIC(Melody, CDialog)



Melody::Melody(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, milli(100)
{
}

Melody::~Melody()
{
}

void Melody::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, milli);
}


BEGIN_MESSAGE_MAP(Melody, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON1, &Melody::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Melody::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CLEAR, &Melody::OnBnClickedClear)
	ON_BN_CLICKED(IDOK, &Melody::OnBnClickedOk)
END_MESSAGE_MAP()


// Melody 메시지 처리기


void Melody::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_BITMAP3);
	CBitmap *oldBitmap = MemDC.SelectObject(&bitmap);
	dc.BitBlt(0, 0, 2000, 2000, &MemDC, 0, 0, SRCCOPY);
	dc.SelectObject(oldBitmap);

	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			arr[i][j].SetRect(i * 40 + 50, j * 20 + 50, i * 40 + 40 + 50, j * 20 + 20 + 50);
		}
	}
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			if (arrcheck[i][j]) {
				CClientDC dc(this);
				CBrush NewBrush(RGB(0, 255, 0));
				CBrush* pOldBrush = dc.SelectObject(&NewBrush);
				dc.Rectangle(arr[i][j]);
			}
			else if (arrcheck[i][j] == false) {
				CClientDC dc(this);
				dc.Rectangle(arr[i][j]);
			}
		}
	}
	//CClientDC dc(this);
	dc.TextOut(20, 50 + 0 * 20, _T("시"));
	dc.TextOut(20, 50 + 1 * 20, _T("라#"));
	dc.TextOut(20, 50 + 2 * 20, _T("라"));
	dc.TextOut(20, 50 + 3 * 20, _T("솔#"));
	dc.TextOut(20, 50 + 4 * 20, _T("솔"));
	dc.TextOut(20, 50 + 5 * 20, _T("파#"));
	dc.TextOut(20, 50 + 6 * 20, _T("파"));
	dc.TextOut(20, 50 + 7 * 20, _T("미"));
	dc.TextOut(20, 50 + 8 * 20, _T("레#"));
	dc.TextOut(20, 50 + 9 * 20, _T("레"));
	dc.TextOut(20, 50 + 10 * 20, _T("도#"));
	dc.TextOut(20, 50 + 11 * 20, _T("도"));
	dc.TextOut(20, 50 + 12 * 20, _T("시"));
	dc.TextOut(20, 50 + 13* 20, _T("라#"));
	dc.TextOut(20, 50 + 14* 20, _T("라"));
	dc.TextOut(20, 50 + 15 * 20, _T("솔#"));
	dc.TextOut(20, 50 + 16 * 20, _T("솔"));
	dc.TextOut(20, 50 + 17 * 20, _T("파#"));
	dc.TextOut(20, 50 + 18 * 20, _T("파"));
	dc.TextOut(20, 50 + 19 * 20, _T("미"));
	dc.TextOut(20, 50 + 20 * 20, _T("레#"));
	dc.TextOut(20, 50 + 21 * 20, _T("레"));
	dc.TextOut(20, 50 + 22 * 20, _T("도#"));
	dc.TextOut(20, 50 + 23 * 20, _T("도"));
}


int prei, prej;
void Melody::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			if (arr[i][j].PtInRect(point) && arrcheck[i][j] == false) {
				CClientDC dc(this);
				CBrush NewBrush(RGB(0, 255, 0));
				CBrush* pOldBrush = dc.SelectObject(&NewBrush);
				dc.Rectangle(arr[i][j]);
				arrcheck[i][j] = true;
				prei = i;
				prej = j;
			}
			else if (arr[i][j].PtInRect(point) && arrcheck[i][j] == true) {
				CClientDC dc(this);
				dc.Rectangle(arr[i][j]);
				arrcheck[i][j] = false;
				prei = i;
				prej = j;
			}
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void Melody::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nFlags == MK_LBUTTON) {
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 24; j++) {
				if (arr[i][j].PtInRect(point) && arrcheck[i][j] == false) {
					if (prei != i || prej != j) {
						CClientDC dc(this);
						CBrush NewBrush(RGB(0, 255, 0));
						CBrush* pOldBrush = dc.SelectObject(&NewBrush);
						dc.Rectangle(arr[i][j]);
						arrcheck[i][j] = true;
						prei = i;
						prej = j;
					}
				}
				else if (arr[i][j].PtInRect(point) && arrcheck[i][j] == true) {
					if (prei != i || prej != j) {
						CClientDC dc(this);
						dc.Rectangle(arr[i][j]);
						arrcheck[i][j] = false;
						prei = i;
						prej = j;
					}
				}
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}


void Melody::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	socClient popup;
	CString str;
	str.Format(_T(""));
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			CString str2;
			if (arrcheck[i][j]) {
				str.Append(_T("1/"));
			}
			else if (arrcheck[i][j] == false) {
				str.Append(_T("0/"));
			}
			
			//str.Append(str2);
			//str.Append(_T("/"));
		}
	}
	popup.savedata = str;
	
	UpdateData(FALSE);
	if (popup.DoModal() == IDOK) {
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 24; j++) {
				if (popup.datasave[i][j]) {
					arrcheck[i][j] = true;
				}
				else if (popup.datasave[i][j] == false) {
					arrcheck[i][j] = false;
				}
				if (arrcheck[i][j]) {
					CClientDC dc(this);
					CBrush NewBrush(RGB(0, 255, 0));
					CBrush* pOldBrush = dc.SelectObject(&NewBrush);
					dc.Rectangle(arr[i][j]);
				}
				else if (arrcheck[i][j] == false) {
					CClientDC dc(this);
					dc.Rectangle(arr[i][j]);
				}
			}
		}
	}
}


void Melody::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}


void Melody::OnBnClickedClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			arrcheck[i][j] = false;
		}
	}
	Invalidate(true);
}


void Melody::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (milli == 0) {
		MessageBox(_T("시간단위를 입력하세요. (단위 : millisec)"));
		return;
	}
	CDialog::OnOK();
}

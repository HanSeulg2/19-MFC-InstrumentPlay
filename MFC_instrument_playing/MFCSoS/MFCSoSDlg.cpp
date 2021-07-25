
// MFCSoSDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCSoS.h"
#include "MFCSoSDlg.h"
#include "afxdialogex.h"
#include "Melody.h"





#define NINSTRUMENT 128
#define NNOTE 128
#define NVOLUME 128

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "stdio.h"
#include "conio.h"
#include "windows.h"
#include "mmsystem.h"

#pragma comment(lib,"winmm.lib")
#pragma pack(push,1)

typedef struct {
	BYTE byteState;
	BYTE byteNote;
	BYTE byteVelocity;
	BYTE byteNULL;
}MJGRShortMidiMSG_ST;

typedef union
{
	DWORD                dwMidi_Data;
	MJGRShortMidiMSG_ST  stMidi_Data;
}MJGRShortMidiMSG;

void MIDIOutputError(MMRESULT mmResult) {
}

#pragma pack(pop)

HMIDIOUT     m_DevHandle;
MMRESULT     m_MMErrCode;
MIDIOUTCAPS  m_NowMidiOutCaps;

void CALLBACK MidiOutProc(HMIDIOUT hMidiDevice, UINT uMsg, DWORD Instance, DWORD Param1, DWORD Param2) {
}


void Midi_OutputError(MMRESULT mmrErrCode) {
	char szErrMsg[128]; // 에러 문자열을 저장할 배열
	midiInGetErrorText(mmrErrCode, (LPWSTR)szErrMsg, sizeof(szErrMsg)); // 에러 코드를 문자열로 반환
	CString err;
	err.Format(_T("%s"), szErrMsg);
	AfxMessageBox(err);
}

long Midi_GetDevID(HMIDIOUT m_DevOutHandle) {

	UINT     uDeviceID;
	MMRESULT Result = midiOutGetID(m_DevOutHandle, &uDeviceID);
	if (Result != MMSYSERR_NOERROR) {
		Midi_OutputError(Result);
		return -1;
	}
	return (long)uDeviceID;
}

void Midi_GetDevCaps(HMIDIOUT m_DevOutHandle, MIDIOUTCAPS* Caps) {
	long     lDeviceID;
	MMRESULT Result;

	lDeviceID = Midi_GetDevID(m_DevOutHandle);
	if (lDeviceID < 0) return;
	Result = midiOutGetDevCaps((UINT)lDeviceID, Caps, sizeof(MIDIOUTCAPS));
	if (Result != MMSYSERR_NOERROR) {
		Midi_OutputError(Result);
	}
}

HMIDIOUT Midi_Open(WORD wDevsNum) {
	WORD      wMaxDevNum = 0;
	MMRESULT  uiMidiOpen = 0;
	HMIDIOUT  m_DevHandle = NULL;
	wMaxDevNum = midiInGetNumDevs();
	if (wDevsNum >= wMaxDevNum) {
		wDevsNum = 0;
	}
	uiMidiOpen = midiOutOpen(&m_DevHandle, wDevsNum, (DWORD)(MidiOutProc), (DWORD)NULL, CALLBACK_FUNCTION);
	if (uiMidiOpen != MMSYSERR_NOERROR) {
		Midi_OutputError(uiMidiOpen);
		return NULL;
	}
	return m_DevHandle;
}

LRESULT Midi_Close(HMIDIOUT m_DevOutHandle)
{
	MMRESULT Result;
	Result = midiOutClose(m_DevOutHandle);
	if (Result != MMSYSERR_NOERROR) {
		Midi_OutputError(Result);
		return FALSE;
	}
	else {
		m_DevOutHandle = NULL;
	}
	return TRUE;
}

void Midi_SendShortMsg(HMIDIOUT m_DevOutHandle, BYTE byteState, BYTE byteNote, BYTE byteValo) {
	MJGRShortMidiMSG  sMsg;
	sMsg.stMidi_Data.byteVelocity = byteValo;
	sMsg.stMidi_Data.byteNote = byteNote;
	sMsg.stMidi_Data.byteState = byteState;
	sMsg.stMidi_Data.byteNULL = 0;
	midiOutShortMsg(m_DevOutHandle, sMsg.dwMidi_Data);
}

void Midi_SendLongMsg(HMIDIOUT m_DevOutHandle, BYTE* lpMsg, DWORD dwMsgLength) {
	MIDIHDR  m_MIDIHdr;
	MMRESULT Result;
	ZeroMemory(&m_MIDIHdr, sizeof(MIDIHDR));
	m_MIDIHdr.lpData = (char*)lpMsg;
	m_MIDIHdr.dwBufferLength = dwMsgLength;
	m_MIDIHdr.dwFlags = 0;
	Result = midiOutPrepareHeader(m_DevOutHandle, &m_MIDIHdr, sizeof(MIDIHDR));
	if (Result != MMSYSERR_NOERROR) {
		Midi_OutputError(Result);
		return;
	}
	Result = midiOutLongMsg(m_DevOutHandle, &m_MIDIHdr, sizeof(MIDIHDR));
	if (Result != MMSYSERR_NOERROR) {
		Midi_OutputError(Result);
		return;
	}
	midiOutUnprepareHeader(m_DevOutHandle, &m_MIDIHdr, sizeof(MIDIHDR));
}

void Midi_AllChannelSoundOff(HMIDIOUT m_DevOutHandle) {
	BYTE channel;
	for (channel = 0; channel < 16; channel++) {
		Midi_SendShortMsg(m_DevOutHandle, (BYTE)(0xB0 + channel), 0x78, 0);
	}
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCSoSDlg 대화 상자



CMFCSoSDlg::CMFCSoSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSOS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSoSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editB);
}

BEGIN_MESSAGE_MAP(CMFCSoSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCSoSDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCSoSDlg 메시지 처리기
bool melodycheck[100][100];

BOOL CMFCSoSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_editB.SetWindowTextW(_T("Grand Piano"));
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 24; j++) {
			melodycheck[i][j] = false;
		}
	}
	if (m_pThread == NULL)
	{
		m_pThread = AfxBeginThread(RunThread, this);
		hThread = m_pThread->m_hThread;
		m_bStop = false;
		m_bRun = true;
		if (!m_pThread)
		{
			AfxMessageBox(_T("Thread could not be created,"));
			return TRUE;
		}
	}
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCSoSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCSoSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		CClientDC dc(this);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap bitmap;
		bitmap.LoadBitmapW(IDB_BITMAP1);
		CBitmap *oldBitmap = MemDC.SelectObject(&bitmap);
		dc.BitBlt(0, 0, 1390, 790, &MemDC, 0, 0, SRCCOPY);
		dc.SelectObject(oldBitmap);
		
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCSoSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BYTE instrument = 0;
BYTE octave = 48;
BYTE volume = 127;

char myKey;

UINT CMFCSoSDlg::RunThread(LPVOID pParam)
{
	CMFCSoSDlg* g_pDlg = (CMFCSoSDlg*)pParam;
	
	while (1)
	{
		if (g_pDlg->m_bRun == true)
		{
			//작동 함수
			int  i, loop;

			BYTE szMusKey[18] = { 0x41,0x57,0x53,0x45,0x44,
								0x46,0x54,0x47,0x59,0x48,
								0x55,0x4A,0x4B,0x4F,0x4C,
								0x50,0xBA,0xDE };

			int szMusKey2[18] = { 1,2,3,4,5,
								6,7,8,9,10,
								11,12,13,14,15,
								16,17,18 };
			BYTE szMusKeyOnOff[18] = { 0 };
			m_DevHandle = Midi_Open(0);
			Midi_AllChannelSoundOff(m_DevHandle);
			Midi_SendShortMsg(m_DevHandle, 0xB0, 7, 127);
			loop = 1;
			while (loop)
			{
				//if (kbhit())
				//{
				if (GetKeyState(VK_ESCAPE) & 0xFF00) loop = 0;  //ESC 종료
				else {
					for (i = 0; i < 18; i++)
					{
						//if (GetKeyState(szMusKey[i]) & 0xFF00)
						if (GetKeyState(szMusKey[i]) & 0xFF00 ||szMusKey2[i] == myKey)
						{
							if (szMusKeyOnOff[i] == 0)
							{
								szMusKeyOnOff[i] = 1;
								Midi_SendShortMsg(m_DevHandle, 0x90, (BYTE)(octave + i), 120);
							}
						} /*else if (GetKeyState(VK_RIGHT) < 0) {
							if (instrument < (NINSTRUMENT - 1))
								instrument += 1;
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						} else if (GetKeyState(VK_LEFT) < 0) {
							if(instrument != 0)
								instrument -= 1;
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						} else if (GetKeyState(0x31) < 0) {
								instrument = 1;
								Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
								Sleep(100);
						} else if (GetKeyState(0x32) < 0) {
							instrument = 72;  //클라리넷
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						} else if (GetKeyState(0x33) < 0) {
							instrument = 43;  //첼로
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						} else if (GetKeyState(0x34) < 0) {
								instrument = 57;  //트럼펫
								Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
								Sleep(100);
						}
						else if (GetKeyState(0x35) < 0) {
							instrument = 32;     // Harmonics Guitar
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						}
						else if (GetKeyState(0x36) < 0) {
							instrument = 30;     // Electric Guitar
							Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
							Sleep(100);
						}*/

						else if (GetKeyState(VK_UP) < 0) {
							if (octave < (NNOTE - 18))
								octave += 12;
							Sleep(100);
						} else if (GetKeyState(VK_DOWN) < 0) {
							if (octave != 0)
								octave -= 12;
							Sleep(100);
						} /*else if (GetKeyState(VK_OEM_PERIOD) < 0) {
							if (volume < (NVOLUME - 1))
								volume += 10;
							Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
							//Sleep(60);
						}
						else if (GetKeyState(VK_OEM_COMMA) < 0) {
							if (volume != 0)
								volume -= 10;
							Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
							//Sleep(60);
						}*/
					}
				}
				//}
				for (i = 0; i < 18; i++)
				{
					if (!(GetKeyState(szMusKey[i]) & 0xFF00 || szMusKey2[i] == myKey))
					{
						if (szMusKeyOnOff[i] != 0)
						{
							szMusKeyOnOff[i] = 0;
							Midi_SendShortMsg(m_DevHandle, 0x80, (BYTE)(octave + i), 120);
						}
					}
				}
			}
			for (i = 0; i < 18; i++)
			{
				if (szMusKeyOnOff[i] != 0)
				{
					Midi_SendShortMsg(m_DevHandle, 0x80, (BYTE)(octave + i), 120);
				}
			}
			Midi_AllChannelSoundOff(m_DevHandle);
			Midi_Close(m_DevHandle);
		}
		if (g_pDlg->m_bStop == true)
		{
			return 0;
		}
	}
	return 0;
}



void CMFCSoSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pThread != NULL)
	{
		m_bStop = true;
		m_pThread = NULL;
	}
}


void CMFCSoSDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*
	char szMusKey[18] = { 0x41,0x57,0x53,0x45,0x44,
								0x46,0x54,0x47,0x59,0x48,
								0x55,0x4A,0x4B,0x4F,0x4C,
								0x50,0xBA,0xDE };*/
	switch (nChar) {
	case '1':
		m_editB.SetWindowTextW(_T("Grand Piano"));
		instrument = 1;
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		//Sleep(100);
		break;
	case '2':
		m_editB.SetWindowTextW(_T("Clarinet"));
		instrument = 72;  //클라리넷
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		break;
	case '3':
		m_editB.SetWindowTextW(_T("Cello"));
		instrument = 43;  //첼로
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		break;
	case '4':
		m_editB.SetWindowTextW(_T("Trumpet"));
		instrument = 57;  //트럼펫
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		break;
	case '5':
		m_editB.SetWindowTextW(_T("Harmonic Guitar"));
		instrument = 32;     // Harmonics Guitar
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		break;
	case '6':
		m_editB.SetWindowTextW(_T("Electric Guitar"));
		instrument = 30;     // Electric Guitar
		Midi_SendShortMsg(m_DevHandle, 0xC0, instrument, 0);
		break;
	}

	/*
	switch (nChar) {
	case 0x41:  //a
		myKey = 1;
		break;
	case 0x57: 
		myKey = 2;
		break;
	case 0x53:
		myKey = 3;
		break;
	case 0x45:
		myKey = 4;
		break;
	case 0x44:
		myKey = 5;
		break;
	case 0x46:
		myKey = 6;
		break;
	case 0x54:
		myKey = 7;
		break;
	case 0x47:
		myKey = 8;
		break;
	case 0x59:
		myKey = 9;
		break;
	case 0x48:
		myKey = 10;
		break;
	case 0x55:
		myKey = 11;
		break;
	case  0x4A:
		myKey = 12;
		break;
	case 0x4B:
		myKey = 13;
		break;
	case 0x4F:
		myKey = 14;
		break;
	case 0x4C:
		myKey = 15;
		break;
	case 0x50:
		myKey = 16;
		break;
	case 0xBA:
		myKey = 17;
		break;
	case 0xDE:
		myKey = 18;
		break;
	
	}
	*/
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMFCSoSDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//myKey = NULL;
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

int timecount = 0;
BYTE szMusKeyOnOff[24] = { 0 };
int sec = 0;

void CMFCSoSDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	xyP=point;
	CString str;
	str.Format(_T("%d,%d"), xyP.x, xyP.y);
	//MessageBox(str);
	
	/*
	Volcheck = true;
	//AfxMessageBox(xyP.x,xyP.y);
	if (Volcheck == true) {
		if (volume != 0)
		volume -= 20;
		Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
		
		//MessageBox(_T(xyP));
	}
	Volcheck = false;
	*/
	

	CRect down;
	CClientDC dc(this);
	CDC MemDC;

	down.SetRect(1241-100, 713-50, 1241+100, 713+50);
	if (down.PtInRect(point)) {
		timecount = 0;
		KillTimer(0);
		Midi_AllChannelSoundOff(m_DevHandle);
		for (int j = 0; j < 24; j++) {
			szMusKeyOnOff[j] = 0;
		}
		Melody Popup;
		for (int i = 0; i < 36; i++) {
			for (int j = 0; j < 24; j++) {
				Popup.arrcheck[i][j] = melodycheck[i][j];
			}
		}
		if (Popup.DoModal() == IDOK) {
			UpdateData(TRUE);
			for (int i = 0; i < 36; i++) {
				for (int j = 0; j < 24; j++) {
					melodycheck[i][j] = Popup.arrcheck[i][j];
				}
			}
			sec = Popup.milli;
			SetTimer(0, sec, NULL);
		}
	}

	down.SetRect(1203 ,214 ,1296 ,295);
	if (down.PtInRect(point)) {
		  //옥타브 다운 
		if (octave != 0)
			octave -= 12;
			Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
	}
	down.SetRect(1198 ,91 ,1298 ,171);
	if (down.PtInRect(point)) {
		//옥타브 업
		octave += 12;
		Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
	}
	down.SetRect(56, 308, 135, 434);
	if (down.PtInRect(point)) {
		//도 지정
		CString str;
		str.Format(_T("도"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(109,63,151,302);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("도#"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(139, 312, 213, 436);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("레"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(200 ,60 ,240 ,301);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("레#"));
		myKey = octavcheck(str);
	}
	down.SetRect(216 ,308 ,293 ,435);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("미"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(298 ,307 ,374 ,433);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("파"));
		myKey = octavcheck(str);
	}
	down.SetRect(345 ,65, 384, 305);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("파#"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(374,312 ,451 ,435);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("솔"));
		myKey = octavcheck(str);
	}
	down.SetRect(433 ,62 ,472 ,303);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("솔#"));
		myKey = octavcheck(str);
	}
	down.SetRect(457 ,309 ,530 ,437);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("라"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(522, 65 ,560 ,304);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("라#"));
		myKey = octavcheck(str);
	}
	down.SetRect(535 ,311, 607, 434);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("시"));
		myKey = octavcheck(str);
	}
	down.SetRect(614,309,690,436);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("도2"));
		myKey = octavcheck(str);
	}
	
	down.SetRect(666 ,64 ,703 ,306);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("도2#"));
		myKey = octavcheck(str);
	}
	down.SetRect(695 ,312 ,768 ,433);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("레2"));
		myKey = octavcheck(str);
	}
	down.SetRect(753 ,59 ,796 ,304);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("레2#"));
		myKey = octavcheck(str);
	}
	down.SetRect(772 ,309 ,864 ,437);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("미2"));
		myKey = octavcheck(str);
	}
	down.SetRect(849 ,304 ,926 ,433);
	if (down.PtInRect(point)) {
		// 지정
		CString str;
		str.Format(_T("파2"));
		myKey = octavcheck(str);
	}
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_BITMAP2);
	CBitmap *oldBitmap = MemDC.SelectObject(&bitmap);
	dc.SelectObject(oldBitmap);
	dc.BitBlt(point.x - 20, point.y - 15, 1390, 790, &MemDC, 0, 0, SRCAND);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCSoSDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*
	Volcheck = false;
	if (Volcheck == false) {
		volume += 20;
		Midi_SendShortMsg(m_DevHandle, 0xB0, 7, volume);
		//AfxMessageBox(volume);
	}
	Volcheck = true;
	*/
	CDialogEx::OnRButtonDown(nFlags, point);
}


void CMFCSoSDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Invalidate(true);
	myKey = NULL;
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CMFCSoSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	for (int i = 0; i < 24; i++) {
		if (melodycheck[timecount][i]) {
			if (szMusKeyOnOff[i] == 0) {
				szMusKeyOnOff[i] = 1;
				Midi_SendShortMsg(m_DevHandle, 0x90, (BYTE)(72 - i), 120); // 키는거 
			}
		}
		else if (melodycheck[timecount][i] == false) {
			if (szMusKeyOnOff[i] == 1) {
				szMusKeyOnOff[i] = 0;
				Midi_SendShortMsg(m_DevHandle, 0x80, (BYTE)(72 - i), 120); // 끄는거
			}
		}
	}
	timecount++;
	if (timecount > 35) {
		timecount = 0;
		KillTimer(0);
		Midi_AllChannelSoundOff(m_DevHandle);
		for (int j = 0; j < 24; j++) {
			szMusKeyOnOff[j] = 0;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
	/*timecount += 1;
	if (timecount > 34) {
		timecount = 0;
		KillTimer(0);
		Midi_AllChannelSoundOff(m_DevHandle);
		for (int j=0; j < 24; j++) {
			szMusKeyOnOff[j] = 0;
		}
	}*/
}


void CMFCSoSDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

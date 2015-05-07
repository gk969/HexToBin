
// HexToBinDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "HexToBin.h"
#include "HexToBinDlg.h"
#include "afxdialogex.h"

#include "dispPara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexToBinDlg �Ի���




CHexToBinDlg::CHexToBinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHexToBinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexToBinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IN, editIn);
	DDX_Control(pDX, IDC_EDIT_OUT, editOut);
	DDX_Control(pDX, IDC_COMBO_IN_BASE, selIn);
	DDX_Control(pDX, IDC_COMBO_OUT_BASE, selOut);
	DDX_Control(pDX, IDC_COMBO_OPERAT, selOperat);
	DDX_Control(pDX, IDC_CHECK_MONI_CLIPBOARD, moniClipboard);
	DDX_Control(pDX, IDC_CHECK_WRITE_CLIPBOARD, writeClipboard);
}

BEGIN_MESSAGE_MAP(CHexToBinDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TRANS, &CHexToBinDlg::OnBnClickedButtonTrans)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CHexToBinDlg::OnBnClickedButtonExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CHexToBinDlg ��Ϣ�������

BOOL CHexToBinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
    // ��ֹ��󻯰�ť ���ɸı��С
    ModifyStyle(WS_MAXIMIZEBOX, NULL);
    ModifyStyle(WS_SIZEBOX, NULL);

	USES_CONVERSION;
	int i;
	for(i=0; i<(sizeof(inDesc)/sizeof(char*)); i++)
	{
		selIn.AddString(A2W(inDesc[i]));
	}
	selIn.SetCurSel(BASE_16);

	
	for(i=0; i<(sizeof(inDesc)/sizeof(char*)); i++)
	{
		selOut.AddString(A2W(outDesc[i]));
	}
	selOut.SetCurSel(BASE_2);

	
	for(i=0; i<(sizeof(inDesc)/sizeof(char*)); i++)
	{
		selOperat.AddString(A2W(operatDesc[i]));
	}
	selOperat.SetCurSel(OPERAT_NONE);

	
	SetTimer(TIM_CLIPBOARD_CHK, CLIPBOARD_CHK_TIME, NULL);
	clipboardBuff[0]=0;

	ToolTipsInit();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHexToBinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHexToBinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHexToBinDlg::OnOk(){}

bool nextInputIsValid(char ch, u8 base)
{
	switch(base)
	{
	case BASE_2:
		return ch=='0'||ch=='1';
	case BASE_10:
		return ch>='0'&&ch<='9';
	case BASE_16:
		return (ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F');
	case STRING:
		return true;
	default:
		return false;
	}
}

void CHexToBinDlg::OnBnClickedButtonTrans()
{
	USES_CONVERSION;


	u8 inBaseVal=selIn.GetCurSel();
	u8 outBaseVal=selOut.GetCurSel();
	u8 operat=selOperat.GetCurSel();

	u32 sumVal=0;
	u8 xorVal=0;

	char *inStr=W2A(GetWndText(&editIn));
	u32 strEnd=((u32)inStr)+strlen(inStr);

	u32 sPtr=0;

	char *next=inStr;
	while(!nextInputIsValid(*next, inBaseVal)&&(u32)next<strEnd)next++;
	while((u32)next<strEnd)
	{
		u8 val;
		switch(inBaseVal)
		{
		case BASE_2:
			val=strtol(next, &next, 2);
			break;
			
		case BASE_10:
			val=strtol(next, &next, 10);
			break;

		case BASE_16:
			val=strtol(next, &next, 16);
			break;
			
		case STRING:
			val=*next;
			next++;
			break;
		}

		switch(operat)
		{
		case OPERAT_NONE:
			switch(outBaseVal)
			{
			case BASE_2:
				u8 i;
				for(i=8; i>0; i--)
				{
					sPtr+=sprintf_s(outBuff+sPtr, OUT_BUFF_SIZE-sPtr, "%d", (val&(1<<(i-1)))?1:0);
				}
				sPtr+=sprintf_s(outBuff+sPtr, OUT_BUFF_SIZE-sPtr, " ");
				break;
			case BASE_10:
				sPtr+=sprintf_s(outBuff+sPtr, OUT_BUFF_SIZE-sPtr, "%03d ", val);
				break;
			case BASE_16:
				sPtr+=sprintf_s(outBuff+sPtr, OUT_BUFF_SIZE-sPtr, "%02X ", val);
				break;
			case STRING:
				sPtr+=sprintf_s(outBuff+sPtr, OUT_BUFF_SIZE-sPtr, "%c", val);
				break;
			}
			break;
		case OPERAT_SUM:
			sumVal+=val;
			break;
		case OPERAT_XOR:
			xorVal^=val;
			break;
		}
		while(!nextInputIsValid(*next, inBaseVal)&&(u32)next<strEnd)next++;
	}

	editOut.SetWindowText(L"");
	switch(operat)
	{
	case OPERAT_NONE:
		outBuff[sPtr]=0;
		editOut.SetWindowText(A2W(outBuff));
		break;
	case OPERAT_SUM:
		CEditPrintf(&editOut, "%08X", sumVal);
		break;
	case OPERAT_XOR:
		CEditPrintf(&editOut, "%02X", xorVal);
		break;
	}
}


void CHexToBinDlg::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}



void CHexToBinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TIM_CLIPBOARD_CHK)
	{
		if(moniClipboard.GetCheck())
		{
			if(OpenClipboard())
			{
				if(IsClipboardFormatAvailable(CF_TEXT))
				{
					HANDLE hClip=GetClipboardData(CF_TEXT);
					char *clip=(char *)GlobalLock(hClip);
					GlobalUnlock(hClip);

					USES_CONVERSION;

					if(strlen(clip)<CLIPBOARD_BUFF_SIZE&&strcmp(clip, clipboardBuff))
					{
						editIn.SetWindowText(A2W(clip));
						OnBnClickedButtonTrans();

						if(writeClipboard.GetCheck())
						{
							strcpy(clipboardBuff, W2A(GetWndText(&editOut)));

							EmptyClipboard();
							hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(clipboardBuff)+1);
							clip=(char *)GlobalLock(hClip);
							strcpy(clip,clipboardBuff);
							GlobalUnlock(hClip);
							SetClipboardData(CF_TEXT,hClip);
						}
						else
						{
							strcpy(clipboardBuff, clip);
						}
					}
				}
				CloseClipboard();
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CHexToBinDlg::ToolTipsInit()
{
	EnableToolTips(TRUE);
	ToolTips.Create(this, TTS_ALWAYSTIP | TTS_CLOSE | TTS_NOFADE);
	ToolTips.Activate(TRUE);
	ToolTips.SetTipTextColor(RGB(0,0,255));
	ToolTips.SetDelayTime(TTDT_INITIAL ,150);
	ToolTips.SetDelayTime(TTDT_AUTOPOP,5000);
	/*
	ToolTips.AddTool(GetDlgItem(IDC_BUTTON_CLIPBOARD),
		L"��ʼ���Զ�ת�����а��е��ַ���������ת�����������а塣");
		*/
}

BOOL CHexToBinDlg::PreTranslateMessage(MSG* pMsg)
{
   if(pMsg->message== WM_LBUTTONDOWN ||
      pMsg->message== WM_LBUTTONUP ||
      pMsg->message== WM_MOUSEMOVE)
   {
      ToolTips.RelayEvent(pMsg);
   }

   return CDialog::PreTranslateMessage(pMsg);
}


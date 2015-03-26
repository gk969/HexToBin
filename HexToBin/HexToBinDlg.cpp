
// HexToBinDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HexToBin.h"
#include "HexToBinDlg.h"
#include "afxdialogex.h"

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
	DDX_Control(pDX, IDC_COMBO_IN_BASE, inBase);
	DDX_Control(pDX, IDC_COMBO_OUT_BASE, outBase);
}

BEGIN_MESSAGE_MAP(CHexToBinDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TRANS, &CHexToBinDlg::OnBnClickedButtonTrans)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CHexToBinDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CLIPBOARD, &CHexToBinDlg::OnBnClickedButtonClipboard)
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

	inBase.AddString(L"2����");
	inBase.AddString(L"10����");
	inBase.AddString(L"16����");
	inBase.SetCurSel(2);

	
	outBase.AddString(L"2����");
	outBase.AddString(L"10����");
	outBase.AddString(L"16����");
	outBase.SetCurSel(0);

	
	SetTimer(TIM_CLIPBOARD_CHK, CLIPBOARD_CHK_TIME, NULL);
	clipboardBuff[0]=0;
	needToMoniClip=false;

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

bool isHex(char ch, u8 base)
{
	switch(base)
	{
	case 2:
		return ch=='0'||ch=='1';
	case 10:
		return ch>='0'&&ch<='9';
	case 16:
		return (ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F');
	default:
		return false;
	}
}

void CHexToBinDlg::OnBnClickedButtonTrans()
{
	USES_CONVERSION;

	editOut.SetWindowText(L"");

	u8 baseTab[]={2, 10, 16};
	u8 inBaseVal=baseTab[inBase.GetCurSel()];
	u8 outBaseVal=baseTab[outBase.GetCurSel()];


	char *inStr=W2A(GetWndText(&editIn));
	u32 strEnd=((u32)inStr)+strlen(inStr);

	char *next=inStr;
	while(!isHex(*next, inBaseVal)&&(u32)next<strEnd)next++;
	while((u32)next<strEnd)
	{
		u8 val=strtol(next, &next, inBaseVal);
		
		switch(outBaseVal)
		{
		case 2:
			u8 i;
			for(i=8; i>0; i--)
			{
				CEditPrintf(&editOut, "%d", (val&(1<<(i-1)))?1:0);
			}
			break;
		case 10:
			CEditPrintf(&editOut, "%03d", val);
			break;
		case 16:
			CEditPrintf(&editOut, "%02X", val);
			break;
		}
		CEditPrintf(&editOut, " ");
		while(!isHex(*next, inBaseVal)&&(u32)next<strEnd)next++;
	}

}


void CHexToBinDlg::OnBnClickedButtonExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}


void CHexToBinDlg::OnBnClickedButtonClipboard()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(needToMoniClip)
	{
		needToMoniClip=false;
		GetDlgItem(IDC_BUTTON_CLIPBOARD)->SetWindowText(L"��ʼ��ؼ��а�");
	}
	else
	{
		needToMoniClip=true;
		GetDlgItem(IDC_BUTTON_CLIPBOARD)->SetWindowText(L"ֹͣ��ؼ��а�");
	}
}


void CHexToBinDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==TIM_CLIPBOARD_CHK)
	{
		if(needToMoniClip)
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
						strcpy(clipboardBuff, W2A(GetWndText(&editOut)));

						EmptyClipboard();
						hClip=GlobalAlloc(GMEM_MOVEABLE,strlen(clipboardBuff)+1);
						clip=(char *)GlobalLock(hClip);
						strcpy(clip,clipboardBuff);
						GlobalUnlock(hClip);
						SetClipboardData(CF_TEXT,hClip);
					}
					CloseClipboard();
				}
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
	
	ToolTips.AddTool(GetDlgItem(IDC_BUTTON_CLIPBOARD),
		L"��ʼ���Զ�ת�����а��е��ַ���������ת�����������а塣");
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


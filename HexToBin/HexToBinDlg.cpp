
// HexToBinDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HexToBin.h"
#include "HexToBinDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexToBinDlg 对话框




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


// CHexToBinDlg 消息处理程序

BOOL CHexToBinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
    // 禁止最大化按钮 不可改变大小
    ModifyStyle(WS_MAXIMIZEBOX, NULL);
    ModifyStyle(WS_SIZEBOX, NULL);

	inBase.AddString(L"2进制");
	inBase.AddString(L"10进制");
	inBase.AddString(L"16进制");
	inBase.SetCurSel(2);

	
	outBase.AddString(L"2进制");
	outBase.AddString(L"10进制");
	outBase.AddString(L"16进制");
	outBase.SetCurSel(0);

	
	SetTimer(TIM_CLIPBOARD_CHK, CLIPBOARD_CHK_TIME, NULL);
	clipboardBuff[0]=0;
	needToMoniClip=false;

	ToolTipsInit();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHexToBinDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	// TODO: 在此添加控件通知处理程序代码
	DestroyWindow();
}


void CHexToBinDlg::OnBnClickedButtonClipboard()
{
	// TODO: 在此添加控件通知处理程序代码
	if(needToMoniClip)
	{
		needToMoniClip=false;
		GetDlgItem(IDC_BUTTON_CLIPBOARD)->SetWindowText(L"开始监控剪切板");
	}
	else
	{
		needToMoniClip=true;
		GetDlgItem(IDC_BUTTON_CLIPBOARD)->SetWindowText(L"停止监控剪切板");
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
		L"开始后，自动转换剪切板中的字符串，并把转换结果存入剪切板。");
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


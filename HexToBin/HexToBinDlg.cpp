
// HexToBinDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "HexToBin.h"
#include "HexToBinDlg.h"
#include "afxdialogex.h"

#include "afxwin.h"

#include "proc.h"

const char* BASE_NAME[] = { "2进制", "10进制", "16进制", "ANSI字符串", "UTF8字符串"};
const char* OPERAT_NAME[] = { "无", "求和", "异或", "取反", "CRC16" };

enum {
    BASE_2 = 0,
    BASE_10,
    BASE_16,
    STRING_ANSI,
    STRING_UTF8,
};


enum {
    OPERAT_NONE = 0,
    OPERAT_SUM,
    OPERAT_XOR,
    OPERAT_NOT,
    OPERAT_CRC16
};

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif


// CHexToBinDlg 对话框




CHexToBinDlg::CHexToBinDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CHexToBinDlg::IDD, pParent) {
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHexToBinDlg::DoDataExchange(CDataExchange* pDX) {
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_IN, editIn);
    DDX_Control(pDX, IDC_EDIT_OUT, editOut);
    DDX_Control(pDX, IDC_COMBO_IN_BASE, selIn);
    DDX_Control(pDX, IDC_COMBO_OUT_BASE, selOut);
    DDX_Control(pDX, IDC_COMBO_OPERAT, selOperat);
    DDX_Control(pDX, IDC_CHECK_MONI_CLIPBOARD, moniClipboard);
    DDX_Control(pDX, IDC_CHECK_WRITE_CLIPBOARD, writeClipboard);
    DDX_Control(pDX, IDC_CHECK_KEEP_RN, keepWarp);
    DDX_Control(pDX, IDC_STATIC_INPUT, frameTextInput);
}

BEGIN_MESSAGE_MAP(CHexToBinDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_TRANS, &CHexToBinDlg::OnBnClickedButtonTrans)
    ON_BN_CLICKED(IDC_BUTTON_EXIT, &CHexToBinDlg::OnBnClickedButtonExit)
    ON_WM_TIMER()
END_MESSAGE_MAP()

unsigned short crc16(const void* data, unsigned short sizeInByte) {
    unsigned short crc = 0;
    unsigned char* p = (unsigned char*)data;
    
    while (sizeInByte--) {
        unsigned char i;
        crc ^= *p++;
        for (i = 0; i < 8; i++) {
            crc = (crc >> 1) ^ ((crc & 0x01) ? 0xA001 : 0);
        }
    }
    return crc;
}

// CHexToBinDlg 消息处理程序

BOOL CHexToBinDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();
    
    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标
    
    // TODO: 在此添加额外的初始化代码
    
    
    USES_CONVERSION;
    int i;
    for (i = 0; i < (sizeof(BASE_NAME) / sizeof(char*)); i++) {
        selIn.AddString(A2W(BASE_NAME[i]));
    }
    selIn.SetCurSel(BASE_16);
    
    
    for (i = 0; i < (sizeof(BASE_NAME) / sizeof(char*)); i++) {
        selOut.AddString(A2W(BASE_NAME[i]));
    }
    selOut.SetCurSel(BASE_2);
    
    
    for (i = 0; i < (sizeof(OPERAT_NAME) / sizeof(char*)); i++) {
        selOperat.AddString(A2W(OPERAT_NAME[i]));
    }
    selOperat.SetCurSel(OPERAT_NONE);
    
    clipboardBuff = (char*)malloc(CLIPBOARD_BUFF_SIZE);
    outBuff = (char*)malloc(OUT_BUFF_SIZE);
    if (clipboardBuff == NULL || outBuff == NULL) {
        MessageBox(L"内存不足！");
        DestroyWindow();
        return TRUE;
    }
    
    SetTimer(TIM_CLIPBOARD_CHK, CLIPBOARD_CHK_TIME, NULL);
    clipboardBuff[0] = 0;
    
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHexToBinDlg::OnPaint() {
    if (IsIconic()) {
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
    } else {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHexToBinDlg::OnQueryDragIcon() {
    return static_cast<HCURSOR>(m_hIcon);
}


void CHexToBinDlg::OnOk() {}

bool nextInputIsValid(char ch, u8 base, bool needKeepWarp) {
    static char lastChar = 0;
    bool keepSpace = ch == ' ' && lastChar == ' ';
    lastChar = ch;
    
    if (needKeepWarp && (ch == '\r' || ch == '\n' || keepSpace)) {
        return true;
    }
    
    switch (base) {
        case BASE_2:
            return ch == '0' || ch == '1';
        case BASE_10:
            return ch >= '0' && ch <= '9';
        case BASE_16:
            return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
        case STRING_ANSI:
        case STRING_UTF8:
            return true;
        default:
            return false;
    }
}

void outputToCharBuf(char* outBuff, u32 val, u8 outBaseVal, u32* sPtr) {
    u8 binLen = (val > 255) ? 32 : 8;
    switch (outBaseVal) {
        case BASE_2:
            for (u8 i = binLen; i > 0; i--) {
                *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, "%d", (val & (1 << (i - 1))) ? 1 : 0);
                if ((i - 1) % 8 == 0 && i != 1) {
                    *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, " ");
                }
            }
            *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, " ");
            break;
        case BASE_10:
            *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, "%03d ", val);
            break;
        case BASE_16:
            *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, "%02X ", val);
            break;
        case STRING_ANSI:
        case STRING_UTF8:
            *sPtr += sprintf_s(outBuff + *sPtr, OUT_BUFF_SIZE - *sPtr, "%c", val);
            break;
    }
}

void CHexToBinDlg::OnBnClickedButtonTrans() {
    USES_CONVERSION;
    
    int needKeepWarp = keepWarp.GetCheck();
    
    u8 inBaseVal = selIn.GetCurSel();
    u8 outBaseVal = selOut.GetCurSel();
    u8 operat = selOperat.GetCurSel();
    
    u32 calVal = 0;
    u16 crc16Val = 0;
    
    CString inStrW = GetWndText(&editIn);
    
    char* inStr;
    if (inBaseVal == STRING_UTF8) {
        int len = WideCharToMultiByte(CP_UTF8, NULL, inStrW, -1, NULL, NULL, NULL, NULL);
        inStr = new CHAR[len + 1];
        len = WideCharToMultiByte(CP_UTF8, NULL, inStrW, -1, inStr, len, NULL, NULL);
        inStr[len] = 0;
    } else 
    {
        inStr = W2A(inStrW);
    }
    
    u32 strEnd = ((u32)inStr) + strlen(inStr);
    
    u32 sPtr = 0;
    
    u32 transCnt;
    
    char* next = inStr;
    while (!nextInputIsValid(*next, inBaseVal, false) && (u32)next < strEnd)
        next++;
    for (transCnt = 0; (u32)next < strEnd; transCnt++) {
        u8 val;
        bool shouldWarp = needKeepWarp && (*next == '\r' || *next == '\n' || *next == ' ');
        if (shouldWarp) {
            val = *next;
            next++;
        } else {
            switch (inBaseVal) {
                case BASE_2:
                    val = strtol(next, &next, 2);
                    break;
                    
                case BASE_10:
                    val = strtol(next, &next, 10);
                    break;
                    
                case BASE_16:
                    val = strtol(next, &next, 16);
                    break;
                    
                case STRING_ANSI:
                case STRING_UTF8:
                    val = *next;
                    next++;
                    break;
            }
        }
        
        if (shouldWarp) {
            sPtr += sprintf_s(outBuff + sPtr, OUT_BUFF_SIZE - sPtr, "%c", val);
        } else {
            switch (operat) {
                case OPERAT_NONE:
                case OPERAT_NOT:
                    if (operat == OPERAT_NOT) {
                        val = ~val;
                    }
                    outputToCharBuf(outBuff, val, outBaseVal, &sPtr);
                    break;
                case OPERAT_SUM:
                    calVal += val;
                    break;
                case OPERAT_XOR:
                    calVal ^= val;
                    break;
                case OPERAT_CRC16:
                    crc16Val ^= val;
                    for (int i = 0; i < 8; i++) {
                        crc16Val = (crc16Val >> 1) ^ ((crc16Val & 0x01) ? 0xA001 : 0);
                    }
                    break;
            }
        }
        while (!nextInputIsValid(*next, inBaseVal, needKeepWarp) && (u32)next < strEnd)
            next++;
    }
    
    editOut.SetWindowText(L"");
    
    if (operat == OPERAT_SUM || operat == OPERAT_XOR) {
        outputToCharBuf(outBuff, calVal, outBaseVal, &sPtr);
    } else if (operat == OPERAT_CRC16) {
        outputToCharBuf(outBuff, crc16Val, outBaseVal, &sPtr);
    }
    outBuff[sPtr] = 0;
    
    WCHAR* outStr;
    if (outBaseVal == STRING_UTF8) {
        int len = MultiByteToWideChar(CP_UTF8, NULL, outBuff, -1, NULL, NULL);
        outStr = new WCHAR[len + 1];
        len = MultiByteToWideChar(CP_UTF8, NULL, outBuff, -1, outStr, len);
        outStr[len] = 0;
    } else {
        outStr = A2W(outBuff);
    }
    
    editOut.SetWindowText(outStr);
    
    CString inputCntStr;
    inputCntStr.Format(L"输入 %d字节", transCnt);
    frameTextInput.SetWindowText(inputCntStr);
}


void CHexToBinDlg::OnBnClickedButtonExit() {
    // TODO: 在此添加控件通知处理程序代码
    DestroyWindow();
}



void CHexToBinDlg::OnTimer(UINT_PTR nIDEvent) {
    if (nIDEvent == TIM_CLIPBOARD_CHK) {
        if (moniClipboard.GetCheck()) {
            if (OpenClipboard()) {
                if (IsClipboardFormatAvailable(CF_TEXT)) {
                    HANDLE hClip = GetClipboardData(CF_TEXT);
                    char* clip = (char*)GlobalLock(hClip);
                    GlobalUnlock(hClip);
                    
                    USES_CONVERSION;
                    
                    if (strlen(clip) < CLIPBOARD_BUFF_SIZE && strcmp(clip, clipboardBuff)) {
                        editIn.SetWindowText(A2W(clip));
                        OnBnClickedButtonTrans();
                        
                        if (writeClipboard.GetCheck()) {
                            strcpy(clipboardBuff, W2A(GetWndText(&editOut)));
                            
                            EmptyClipboard();
                            hClip = GlobalAlloc(GMEM_MOVEABLE, strlen(clipboardBuff) + 1);
                            clip = (char*)GlobalLock(hClip);
                            strcpy(clip, clipboardBuff);
                            GlobalUnlock(hClip);
                            SetClipboardData(CF_TEXT, hClip);
                        } else {
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


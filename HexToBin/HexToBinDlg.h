
// HexToBinDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "proc.h"




#define TIM_CLIPBOARD_CHK	0
#define CLIPBOARD_CHK_TIME	200


#define CLIPBOARD_BUFF_SIZE	512000


#define OUT_BUFF_SIZE	512000

// CHexToBinDlg 对话框
class CHexToBinDlg : public CDialogEx
{
// 构造
public:
	CHexToBinDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HEXTOBIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void OnOk();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit editIn;
	CEdit editOut;
	afx_msg void OnBnClickedButtonTrans();
	afx_msg void OnBnClickedButtonExit();
	CComboBox selIn;
	CComboBox selOut;
	afx_msg void OnBnClickedButtonClipboard();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CToolTipCtrl  ToolTips;
	void ToolTipsInit();
	BOOL PreTranslateMessage(MSG* pMsg);

	char *clipboardBuff;
	CComboBox selOperat;

	char *outBuff;
	CButton moniClipboard;
	CButton writeClipboard;
	CButton keepWarp;
};

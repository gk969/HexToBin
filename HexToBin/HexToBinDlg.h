
// HexToBinDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "proc.h"




#define TIM_CLIPBOARD_CHK	0
#define CLIPBOARD_CHK_TIME	200


#define CLIPBOARD_BUFF_SIZE	512000


#define OUT_BUFF_SIZE	512000

// CHexToBinDlg �Ի���
class CHexToBinDlg : public CDialogEx
{
// ����
public:
	CHexToBinDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HEXTOBIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void OnOk();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

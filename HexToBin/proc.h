#pragma once

#include "stdafx.h"


typedef unsigned    char    u8;
typedef unsigned    short   u16;
typedef unsigned    int     u32;



u32 atoiHex(const char *astr);
CString ArrayToCString(const unsigned char *data, int len);
void Swap(u8 *dst, u8 *src);
unsigned short CheckSum(unsigned char *data, int size);

CString GetWndText(CWnd *TarText);
void CEditAppendText(CEdit *TarText,  CString *text);
void CEditAppendText(CEdit *TarText,  wchar_t *text);
void CEditAppendText(CEdit *TarText, const char *text);
void CEditPrintf(CEdit *TarText, const char *fmt, ...);


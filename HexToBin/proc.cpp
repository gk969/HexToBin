#include "stdafx.h"
#include "proc.h"

u32 atoiHex(const char *astr)
{
	const char *str=astr;
	u32 num=0;
	if(strlen(str)<=8)
	{
		while(*str)
		{
			num<<=4;
			if((*str>='0')&&(*str<='9'))
			{
				num+=*str-'0';
			}
			else if((*str>='A')&&(*str<='F'))
			{
				num+=*str-'A'+10;
			}
			else if((*str>='a')&&(*str<='f'))
			{
				num+=*str-'a'+10;
			}
			else
			{
				break;
			}
			str++;
		}
	}
	return num;
}


CString ArrayToCString(const unsigned char *data, int len)
{
    int i;
    CString buf;
    CString cstr;
    
    cstr = _T("");
    
    for(i = 0; i < len; i++)
    {
        buf.Format(_T("%.2X "), data[i]);
        cstr = cstr + buf;
    }
    
    return cstr;
}


void Swap(u8 *dst, u8 *src)
{
	u8 temp=*src;
	*src=*dst;
	*dst=temp;
}


unsigned short CheckSum(unsigned char *data, int size)
{
    unsigned short sum = 0;
    
    for(int i = 0; i < size; i++)
    {
        sum += data[i];
    }
    
    return sum;
}

CString GetWndText(CWnd *TarText)
{
    CString cstr;
    
    TarText->GetWindowText(cstr);
    return cstr;
}

void CEditAppendText(CEdit *TarText,  CString *text)
{
    int nLength = TarText->GetWindowTextLength();
    
    if((nLength + text->GetLength()) > 30000)
    {
        TarText->SetSel(0, nLength / 2);
        TarText->ReplaceSel(_T(""));
        nLength = TarText->GetWindowTextLength();
    }
    
    TarText->SetSel(nLength, nLength);
    TarText->ReplaceSel(*text);
}
void CEditAppendText(CEdit *TarText,  wchar_t *text)
{
    CString cstr = text;
    
    CEditAppendText(TarText,  &cstr);
}


#define PRINTF_BUFSIZE  25600

void CEditAppendText(CEdit *TarText, const char *text)
{
    wchar_t wcstring[PRINTF_BUFSIZE];
    MultiByteToWideChar(CP_ACP, 0, text, -1, wcstring, PRINTF_BUFSIZE);
    CString cstr = wcstring;
    
    CEditAppendText(TarText,  &cstr);
}

void CEditPrintf(CEdit *TarText, const char *fmt, ...)
{
    char buffer[PRINTF_BUFSIZE];
    
    va_list arg_ptr;
    va_start(arg_ptr, fmt);
    vsnprintf_s(buffer, PRINTF_BUFSIZE, fmt, arg_ptr);
    CEditAppendText(TarText, buffer);
    va_end(arg_ptr);
}



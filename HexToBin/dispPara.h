#pragma once
#include "afxwin.h"

#include "proc.h"

const char *inDesc[]={"2����", "10����", "16����", "�ַ���"};
const char *outDesc[]={"2����", "10����", "16����", "�ַ���"};
const char *operatDesc[]={"��", "���", "���", "ȡ��"};

enum
{
	BASE_2=0,
	BASE_10,
	BASE_16,
	STRING
};


enum
{
	OPERAT_NONE=0,
	OPERAT_SUM,
	OPERAT_XOR,
	OPERAT_NOT
};
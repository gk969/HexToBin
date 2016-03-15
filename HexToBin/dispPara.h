#pragma once
#include "afxwin.h"

#include "proc.h"

const char *inDesc[]={"2进制", "10进制", "16进制", "字符串"};
const char *outDesc[]={"2进制", "10进制", "16进制", "字符串"};
const char *operatDesc[]={"无", "求和", "异或", "取反"};

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
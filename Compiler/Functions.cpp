#include "stdafx.h"
using namespace std;
int wordclassify(string s)
{
	if (s == "break")
		return BREAK;
	else if (s == "char")
		return CHAR;
	else if (s == "continue")
		return CONTINUE;
	else if (s == "else")
		return ELSE;
	else if (s == "for")
		return FOR;
	else if (s == "if")
		return IF;
	else if (s == "int")
		return INT;
	else if (s == "return")
		return RETURN;
	else if (s == "while")
		return WHILE;
	else
		return ID;
}

bool isescape(char c)
{
	if (c == 0 || (c > 6 && c < 14) || c == 27)
		return true;
	else return false;
}

char toescape(char c)
{
	char tmp;
	switch (c)
	{
	case '0':
		tmp = 0;
		break;
	case 'a':
		tmp = '\a';
		break;
	case 'b':
		tmp = '\b';
		break;
	case 't':
		tmp = '\t';
		break;
	case 'n':
		tmp = '\n';
		break;
	case 'v':
		tmp = '\v';
		break;
	case 'f':
		tmp = '\f';
		break;
	case 'r':
		tmp = '\r';
		break;
	case 'e':
		tmp = '\e';
		break;
	case '\'':
		tmp = '\'';
		break;
	case '\\':
		tmp = '\\';
		break;
	}
	return tmp;
}
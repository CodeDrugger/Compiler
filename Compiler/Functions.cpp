#include "stdafx.h"
using namespace std;
bool iskeyword(string s)
{
	if (s == "break" || s == "char" || s == "continue" || s == "else" ||
		s == "for" || s == "if" || s == "int" || s == "return" || s == "while")
		return true;
	else return false;
}
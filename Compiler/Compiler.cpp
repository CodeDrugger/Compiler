// Compiler.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
using namespace std;

int main()
{
	/*token_scanner ts;
	ts.excute();*/
	syntax_analyser sa;
	sa.production_reader();
	sa.getfirst();
	sa.getcollection();
    return 0;
}


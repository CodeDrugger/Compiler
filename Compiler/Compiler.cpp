// Compiler.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
using namespace std;

int main()
{
	token_scanner ts;
	ts.prepare();
	ts.buffer_scanner();
	ts.show();
    return 0;
}


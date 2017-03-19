// Compiler.cpp : 定义控制台应用程序的入口点。
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


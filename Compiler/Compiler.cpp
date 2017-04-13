// Compiler.cpp : 定义控制台应用程序的入口点。
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
	for (vector<Ep_Closure*>::iterator it = sa.lrc->epset.begin(); it != sa.lrc->epset.end(); it++)
	{
		for (vector<LR1_Item*>::iterator itr = (*it)->LR1_items.begin(); itr != (*it)->LR1_items.end(); itr++)
		{
			for (vector<int>::iterator i = (*itr)->production.begin(); i != (*itr)->production.end(); i++)
			{
				if (i - (*itr)->production.begin() == (*itr)->pos + 1)
					cout << ".";
				if (*i > 0 && *i < 46)
					cout << *(sa.nsmap[*i]);
				else if (*i == 46)
					cout << "S'";
				else
					cout << "->";
			}
			if ((*itr)->symbol > 0)
				cout << "  " << *(sa.nsmap[(*itr)->symbol]) << "  ,  ";
			else
				cout << "  " << "#  ,  ";
		}
		cout << "\n-----------------------------------------------------\n";
	}
    return 0;
}


// Compiler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

int main()
{
	int a = clock();
	token_scanner ts;
	ts.excute();
	syntax_analyser sa;
	sa.excute(ts.token_stream);
	/*sa.getfirst();
	sa.getcollection();
	sa.makelist();
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
	}*/
	/*ofstream os("action0.txt");
	for (vector<int*>::iterator it = sa.action.begin(); it != sa.action.end(); it++)
	{
		for (int i = 0; i < 44; i++)
		{
			os << (*it)[i] << '\t';
		}
		os << "\n";
	}
	os.close();
	ofstream os1("goto0.txt");
	for (vector<int*>::iterator it = sa.moveto.begin(); it != sa.moveto.end(); it++)
	{
		for (int i = 0; i < sa.snmap.size()-MACRONUM; i++)
		{
				os1 << (*it)[i] << '\t';
		}
		os1 << "\n";
	}
	os.close();*/
	int b = clock();
	cout << 1.0 * (b - a) / CLOCKS_PER_SEC;
	return 0;
}


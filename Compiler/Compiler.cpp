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
	sa.excute(ts.token_stream,ts.token_list);
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
	/*ofstream os("action1.txt");
	for (vector<int*>::iterator it = sa.action.begin(); it != sa.action.end(); ++it)
	{
		for (int i = 0; i < 44; i++)
		{
			os << (*it)[i] << '\t';
		}
		os << '\n';
	}
	os.close();
	ofstream os1("goto1.txt");
	for (vector<int*>::iterator its = sa.moveto.begin(); its != sa.moveto.end(); ++its)
	{
		for (int i = 0; i < sa.snmap.size()-MACRONUM; i++)
		{
			os1 << (*its)[i] << '\t';
		}
		os1 << '\n';
	}
	os1.close();*/
	int b = clock();
	cout << 1.0 * (b - a) / CLOCKS_PER_SEC;
	/*for (vector<Production*>::iterator it = sa.grammar_p.begin()+1; it != sa.grammar_p.end(); ++it)
	{
		cout << (*it)->num << "  ";
		for (vector<int>::iterator i = (*it)->production.begin(); i != (*it)->production.end(); ++i)
		{
			if ((*i)>0)
				cout << *(sa.nsmap[*i]);
			else
				cout << "->";
		}
		cout << '\n';
	}*/
	return 0;
}


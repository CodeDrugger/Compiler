#include "stdafx.h"

using namespace std;

syntax_analyser::syntax_analyser()
{
	ifs = ifstream("grammar.txt");
	nsmap.assign(80, 0);
	pair<string, int> p[] = {
		pair<string, int>("BREAK",0),pair<string, int>("B1",39),pair<string, int>("B4",38),pair<string, int>("CHAR",7),pair<string, int>("CONTINUE",0),
		pair<string, int>("ELSE",2),pair<string, int>("FOR",3),pair<string, int>("IF",4),pair<string, int>("INT",8),pair<string, int>("RETURN",5),
		pair<string, int>("WHILE",6),pair<string, int>("ID",9),pair<string, int>("+",10),pair<string, int>("-",11),pair<string, int>("*",12),
		pair<string, int>("/",13),pair<string, int>("%",14),pair<string, int>("++",15),pair<string, int>("--",16),pair<string, int>("&&",17),
		pair<string, int>("||",18),pair<string, int>("!",19),pair<string, int>("==",20),pair<string, int>("<",21),pair<string, int>(">",22),
		pair<string, int>("<=",23),pair<string, int>(">=",24),pair<string, int>("!=",25),pair<string, int>("<>",25),pair<string, int>("(",26),
		pair<string, int>(")",27),pair<string, int>("[",28),pair<string, int>("]",29),pair<string, int>("{",30),pair<string, int>("}",31),
		pair<string, int>("=",32),pair<string, int>(";",33),pair<string, int>("'",34),pair<string, int>("\"",35),pair<string, int>("\\",36), 
		pair<string, int>(",",37),pair<string, int>("STR",40),pair<string, int>("MAIN",41),pair<string, int>("PRINTF",42) };
	static string s[] = {
		"BREAK","CONTINUE","ELSE","FOR","IF","RETURN","WHILE","CHAR","INT","ID","+","-","*","/","%","++","--","&&","||","!","==","<",">",
		"<=",">=","!=","(",")","[","]","{","}","=",";","'","\"","\\",",","B4","B1","STR","MAIN","PRINTF"};
	for (int i = 0; i < 44; i++)
		snmap.insert(p[i]);
	for (int i = 0; i < 43; i++)
		nsmap.at(i) = &s[i];
}

void syntax_analyser::production_reader()
{
	string s, end;
	Production *p = NULL;
	int vstart = MACRONUM, value, hash = 0;
	while (true)
	{
		ifs >> s;
		if (end == s && end == "@") break;
		if (s == "^")
			p = new Production;
		else if (s == "@")
			production_list.push_back(p);
		else if (s == "->")
			p->production.push_back(-1);
		else if (s == "|")
			p->production.push_back(-2);
		else if (isalpha(s[0]))
		{
			map<string, int>::iterator it;
			it = snmap.find(s);
			if (it == snmap.end())
			{
				snmap.insert(pair<string, int>(s, vstart));
				p->production.push_back(vstart);
				string *tmp = new string(s);
				nsmap[vstart] = tmp;
				vstart++;
			}
			else
				p->production.push_back(it->second);	
			for (int i = 0; i < s.length(); i++)
				hash += s[i];
			p->hash += hash;
			hash = 0;
		}
		else if (s[0] == '$')
		{
			s = s.substr(1, s.length() - 1);
			value = snmap.find(s)->second;
			p->production.push_back(value);
			for (int i = 0; i < s.length(); i++)
				hash += s[i];
			p->hash += hash;
			hash = 0;
		}
		else
		{
			value = snmap.find(s)->second;
			p->production.push_back(value);
			for (int i = 0; i < s.length(); i++)
				hash += s[i];
			p->hash += hash;
			hash = 0;
		}
		end = s;
	}
}

void syntax_analyser::getfirst()
{
	/*bool changed = 1;
	vector<int> * p = NULL, *q = NULL;
	int k = 0;
	for (int i = 0; i < MACRONUM; i++)
	{
		p = new vector<int>;
		p->push_back(i);
		first_set.insert(pair<int, vector<int>*>(i, p));
	}
	while (changed)
	{
		q = &(production_list[k]->production);
		for (int j = 0; j < q->size(); j++)
		{
			if ((*q)[j] < 0)
				;
		}
	}*/
}
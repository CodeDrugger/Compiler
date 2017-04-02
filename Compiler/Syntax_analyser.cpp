#include "stdafx.h"

using namespace std;

syntax_analyser::syntax_analyser()
{
	ifs = ifstream("grammar.txt");
	nsmap.assign(80, 0);
	pair<string, int> p[] = {
		pair<string, int>("BREAK",0),pair<string, int>("B1",39),pair<string, int>("B4",38),pair<string, int>("CHAR",7),pair<string, int>("CONTINUE",1),
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
		"<=",">=","!=","(",")","[","]","{","}","=",";","'","\"","\\",",","B4","B1","STR","MAIN","PRINTF" };
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
	for (int i = 0; i < production_list.size(); i++)
	{
		int pos = (production_list[i])->production[0];
		if (pos != i + MACRONUM)
			swap(production_list[i], production_list[pos - MACRONUM]);
	}
}

void syntax_analyser::getfirst()
{
	bool changed = 0;
	vector<int> * q = NULL;
	set<int> * p = NULL, *t = NULL;
	int h = 0;
	pair<std::set<int>::iterator, bool> ret;
	/*for (int i = 0; i < MACRONUM; i++)
	{
		p = new set<int>;
		p->insert(i);
		first_set.insert(pair<int, set<int>*>(i, p));
	}*/
	for (int k = 0; k < production_list.size(); k++)
	{
		q = &(production_list[k]->production);
		p = new set<int>;
		for (int j = 0; j < q->size(); j++)
		{
			if ((*q)[j] < 0 && (*q)[j + 1] < MACRONUM)
				p->insert((*q)[j + 1]);
		}
		first_set.insert(pair<int, set<int>*>(k + MACRONUM, p));
	}
	while (true)
	{
		q = &(production_list[h]->production);
		p = first_set.find(h + MACRONUM)->second;
		for (int j = 0; j < q->size(); j++)
		{
			if ((*q)[j] < 0 && (*q)[j + 1] >= MACRONUM)
			{
				t = first_set.find((*q)[j + 1])->second;
				if (t == p) continue;
				for (set<int>::iterator it = t->begin(); it != t->end(); it++)
				{
					ret = p->insert(*it);
					if (ret.second == true)
						changed = 1;
				}
			}
		}
		h++;
		if (h == production_list.size())
		{
			h = 0;
			if (!changed) break;
			else changed = 0;
		}
	}
}

Ep_Closure* syntax_analyser::getclosure(Ep_Closure* ep)
{
	/*ep->LR1_items.push_back(item);
	ep->hash_set.push_back(item->hash);
	ep->item_num++;
	ep->hash_set_s.push_back(item->hash_s);*/
	int itnum = ep->item_num;
	vector<LR1_Item*> * p = &(ep->LR1_items);
	vector<int> * q = NULL, *t = NULL;
	vector<int>::iterator it;
	do
	{	
		for (int i = 0; i < p->size(); i++)
		{
			q = &(p->at(i)->production);
			t = &(p->at(i)->symbol);
			it = p->at(i)->pos;
			if (it == q->end() - 1) continue;
			it++;
			if (*it >= MACRONUM)
			{
				Production * s = NULL;
				for (int j = 0; j < production_list.size(); j++)
				{
					if (production_list[j]->production[0] == *it)
					{
						s = production_list[j];
						break;
					}
				}
				set<int>* first = first_set.find(*it - MACRONUM)->second;
				LR1_Item * lr = NULL;
				vector<int>::iterator start, end;
				for (set<int>::iterator i = first->begin(); i != first->end(); i++)
				{
					start = s->production.begin();
					end = start;
					while (true)
					{
						start++;
						if (*start == -1)
						{
							start++;
							end = start;
						}
						else if (*start == -2 && start == s->production.end())
						{
							lr = new LR1_Item;
							lr->production.push_back(*it);
							lr->production.push_back(-1);
							lr->pos = lr->production.begin() + 1;
							lr->hash += (*it - 1);
							while (end != start)
							{
								lr->production.push_back(*end);
								lr->hash += *end;
								end++;
							}
							lr->symbol.push_back(*i);
							lr->hash_s += (*i);
							int exist = ep->have_item(*lr);
							if (exist == -1)//不存在
								ep->add_item(lr);
							else if(exist == 0)//存在
								delete lr;
							else//心存在，展望符不存在
							{
								LR1_Item* r = ep->LR1_items[exist];
								r->symbol.push_back(*i);
								r->hash_s += (*i);
								delete lr;
							}
							if (end == s->production.end()) break;
						}
					}
				}
			}
		}
	} while (itnum != ep->item_num);
	return ep;
}

Ep_Closure* syntax_analyser::go(Ep_Closure* ep, int x)
{
	Ep_Closure * epc = new Ep_Closure;//J
	vector<LR1_Item*>* lr = &(ep->LR1_items);
	vector<int>* p = NULL;
	for (vector<LR1_Item*>::iterator it = lr->begin(); it != lr->end(); it++)
	{
		p = &((*it)->production);
		for (vector<int>::iterator i = p->begin();i != p->end();i++)
		{
			if (*i == x)
			{
				LR1_Item* q = NULL;
				q->copy(*it);
				q->pos++;
				epc->add_item(q);
			}
		}
	}
	return epc;
}
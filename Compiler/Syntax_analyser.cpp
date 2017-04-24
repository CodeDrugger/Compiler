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
		pair<string, int>("<=",23),pair<string, int>(">=",24),pair<string, int>("!=",25),pair<string, int>("(",26),pair<string, int>(")",27),
		pair<string, int>("[",28),pair<string, int>("]",29),pair<string, int>("{",30),pair<string, int>("}",31),pair<string, int>("=",32),
		pair<string, int>(";",33),pair<string, int>("'",34),pair<string, int>("\"",35),pair<string, int>("\\",36),pair<string, int>(",",37),
		pair<string, int>("STR",40),pair<string, int>("MAIN",41),pair<string, int>("PRINTF",42) };
	static string s[] = {
		"BREAK","CONTINUE","ELSE","FOR","IF","RETURN","WHILE","CHAR","INT","ID","+","-","*","/","%","++","--","&&","||","!","==","<",">",
		"<=",">=","!=","(",")","[","]","{","}","=",";","'","\"","\\",",","B4","B1","STR","MAIN","PRINTF" };
	for (int i = 0; i < 43; i++)
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
	int pos;
	for (int i = 0; i < production_list.size(); i++)
	{
		pos = (production_list[i])->production[0];
		//cout << pos << "  " << i << "  " << pos - MACRONUM << '\n';
		if (pos != i + MACRONUM)
			swap(production_list[i], production_list[pos - MACRONUM]);
	}
	int num = 1;
	vector<int>* q = NULL;
	vector<int>::iterator pt;
	pair<map<int, Production*>::iterator, bool> ret;
	for (vector<Production*>::iterator it = production_list.begin(); it != production_list.end(); it++)
	{
		q = &((*it)->production);
		for (vector<int>::iterator itr = q->begin(); itr != q->end(); itr++)
		{
			if (*itr < 0)
			{
				p = new Production;
				p->grammar_init((*q)[0]);
				pt = itr + 1;
				while (pt != q->end() && *pt > 0)
				{
					p->add_item(*pt);
					++pt;
				}
				p->num = num;
				num++;
				ret = grammar.insert(pair<int, Production*>(p->hash, p));
				if (ret.second == false)
				{
					cout << "GG了!!!";
				}
				itr = pt - 1;
			}
		}
	}
	//S'
	p = new Production;
	p->grammar_init(snmap.size());
	p->add_item(snmap.size() - 1);
	p->num = grammar.size() + 1;
	grammar.insert(pair<int, Production*>(p->hash, p));
	grammar_p.assign(grammar.size() + 1, 0);
	for (map<int, Production*>::iterator it = grammar.begin(); it != grammar.end(); ++it)
		grammar_p[(*it).second->num] = (*it).second;
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
				for (set<int>::iterator it = t->begin(); it != t->end(); ++it)
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
	int itnum, t, exist;
	vector<LR1_Item*> * p = &(ep->LR1_items);//I的LR1项目集
	//map<pair<int, int>, LR1_Item*>::iterator p = ep->LR1_items.begin();
	vector<int> * q = NULL;
	vector<int>::iterator it;
	LR1_Item * lr = NULL;
	set<int>* first = NULL;
	do
	{
		itnum = ep->item_num;
		for (int i = 0; i < p->size(); i++)//对C的循环
		//for (p;p != ep->LR1_items.end();p++)
		{
			q = &(p->at(i)->production);//LR1项目的生成式
			//q = &((*p).second->production);
			t = p->at(i)->symbol;//展望符
			//t = (*p).second->symbol;
			it = q->begin() + p->at(i)->pos;//点的位置
			//it = q->begin() + (*p).second->pos;
			if (it == q->end() - 1) continue;
			it++;
			if (*it >= MACRONUM)
			{
				for (map<int, Production*>::iterator itr = grammar.begin(); itr != grammar.end(); ++itr)//从拓展文法里找
				{
					if (itr->second->production.at(0) == *it)//B开头的生成式
					{
						if (it + 1 == q->end())//点在末尾，beta是空集
						{
							lr = new LR1_Item;
							lr->production = itr->second->production;
							lr->hash = itr->second->hash;
							lr->pos = 1;
							lr->symbol = t;
							lr->hash += 7;
							exist = ep->have_item(*lr);
							if (exist < 0)
								ep->add_item(lr);
							else
								delete lr;
						}
						else//点不在末尾，beta不是空集
						{
							if (*(it + 1) < MACRONUM)//如果beta是终结符
							{
								lr = new LR1_Item;
								lr->production = itr->second->production;
								lr->hash = itr->second->hash;
								lr->pos = 1;
								lr->symbol = *(it + 1);
								lr->hash += 7;
								exist = ep->have_item(*lr);
								if (exist < 0)
									ep->add_item(lr);
								else
									delete lr;
							}
							else//如果beta不是终结符
							{
								first = first_set.find(*(it + 1))->second;
								for (set<int>::iterator ir = first->begin(); ir != first->end(); ++ir)
								{
									lr = new LR1_Item;
									lr->production = itr->second->production;
									lr->hash = itr->second->hash;
									lr->pos = 1;
									lr->symbol = *ir;
									lr->hash += 7;
									exist = ep->have_item(*lr);
									if (exist < 0)
										ep->add_item(lr);
									else
										delete lr;
								}
							}
						}
					}
				}
				/*Production * s = NULL;
				for (int j = 0; j < production_list.size(); j++)
				{
					if (production_list[j]->production[0] == *it)
					{
						s = production_list[j];
						break;
					}
				}
				set<int>* first = first_set.find(*it)->second;//*it-macronum
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
							lr->hash += (*it - 1) * (*it - 1);
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
							else if (exist == 0)//存在
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
		}*/
			}
		}
	} while (itnum != ep->item_num);
	return ep;
}

Ep_Closure* syntax_analyser::go(Ep_Closure* ep, int x)
{
	Ep_Closure * epc = new Ep_Closure;//J
	vector<LR1_Item*>* lr = &(ep->LR1_items);
	//map<pair<int, int>, LR1_Item*>* lr = &(ep->LR1_items);
	vector<int>* p = NULL;
	LR1_Item* q = NULL;
	for (vector<LR1_Item*>::iterator it = lr->begin(); it != lr->end(); ++it)
	//for (map<pair<int, int>, LR1_Item*>::iterator it = lr->begin(); it != lr->end(); ++it)
	{
		p = &((*it)->production);
		//p = &((*it).second->production);
		for (vector<int>::iterator i = p->begin() + 2; i != p->end(); ++i)
		{
			if (*i != x) continue;
			if (i - p->begin() == (*it)->pos + 1)
			//if (i - p->begin() == (*it).second->pos + 1)
			{
				q = new LR1_Item;
				q->copy(*it);
				//q->copy((*it).second);
				q->pos++;
				q->hash += 7;
				epc->add_item(q);
			}
		}
	}
	if (epc->item_num == 0)
	{
		delete epc;
		return NULL;
	}
	return this->getclosure(epc);
}

void syntax_analyser::getcollection()
{
	lrc = new LR1_Collection;
	LR1_Item * sitem = new LR1_Item;
	vector<int> * p = &(sitem->production);
	int vtnum = snmap.size();//S'
	p->push_back(vtnum);
	p->push_back(-1);
	p->push_back(vtnum - 1);
	sitem->pos = 1;
	sitem->symbol = -3;//#
	sitem->hash += vtnum * vtnum + vtnum - 2;
	sitem->hash += 7;
	Ep_Closure* epc = new Ep_Closure;
	epc->add_item(sitem);
	lrc->add_item(getclosure(epc));
	int itnum, k = 0;
	vector<Ep_Closure*> * ep = NULL;
	vector<Ep_Closure*>::iterator it;
	Ep_Closure * q = NULL;
	do
	{
		itnum = lrc->item_num;
		ep = &(lrc->epset);
		//for (vector<Ep_Closure*>::iterator it = ep->begin(); it != ep->end(); k++)		
		while (true)
		{
			it = ep->begin() + k;
			if (it == ep->end()) break;
			for (int i = 0; i < vtnum; i++)
			{
				it = ep->begin() + k;
				q = go(*it, i);
				if (q == NULL) continue;
				if (!(q->isempty()) && !(lrc->have_item(q)))
					lrc->add_item(getclosure(q));
			}
			k++;
		}
	} while (itnum != lrc->item_num);
}

void syntax_analyser::makelist()
{
	/*
	action表的Sj为+，rj为-，acc=10000，其余为0
	*/
	int * listline = NULL;
	for (int i = 0; i < lrc->item_num; i++)
	{
		listline = new int[MACRONUM + 1];//vtnum+#(-3)
		memset(listline, 0, (MACRONUM + 1) * sizeof(int));
		action.push_back(listline);
		listline = new int[snmap.size() - MACRONUM];//vnum
		memset(listline, 0, (snmap.size() - MACRONUM) * sizeof(int));
		moveto.push_back(listline);
	}
	Ep_Closure * ec = NULL, *ne = NULL;
	vector<int> * pd = NULL;
	int next, sy;
	vector<int>::iterator itr;
	map<int, Production*>::iterator git;
	int fhash = snmap.size()*snmap.size() + snmap.size() - 2 + 7 * 2;
	int phash;
	for (int i = 0; i < lrc->item_num; i++)//i-->k
	{
		ec = lrc->epset[i];//Ik
		for (vector<LR1_Item*>::iterator it = ec->LR1_items.begin(); it != ec->LR1_items.end(); ++it)
		//for (map<pair<int, int>, LR1_Item*>::iterator it = ec->LR1_items.begin(); it != ec->LR1_items.end(); ++it)
		{
			if ((*it)->hash == fhash && (*it)->symbol == -3)//forth if
			//if ((*it).second->hash == fhash && (*it).second->symbol == -3)//forth if
			{
				*(action[i] + MACRONUM) = ACC;
				continue;
			}
			pd = &((*it)->production);
			//pd = &((*it).second->production);
			itr = pd->begin() + (*it)->pos;
			//itr = pd->begin() + (*it).second->pos;
			for (int j = MACRONUM; j < snmap.size(); j++)
			{
				ne = go(ec, j);
				next = lrc->contain(ne);
				if (next >= 0)
					*(moveto[i] + (j - MACRONUM)) = next;
			}
			if (itr != pd->end() - 1)
			{
				itr++;
				ne = go(ec, *itr);
				next = lrc->contain(ne);
				/*if (next >= 0)
				{
					if (*itr < MACRONUM)//first if
						*(action[i] + *itr) = next;//Sj
					else if (*itr >= MACRONUM)//second if
						*(moveto[i] + (*itr - MACRONUM)) = next;//j
				}*/
				if (next >= 0 && *itr < MACRONUM)//first if
					*(action[i] + *itr) = next;//Sj
			}
			else
			{
				phash = (*it)->production[0];
				//phash = (*it).second->production[0];
				phash *= phash;
				phash--;
				for (vector<int>::iterator ir = (*it)->production.begin() + 2; ir != (*it)->production.end(); ++ir)
				//for (vector<int>::iterator ir = (*it).second->production.begin() + 2; ir != (*it).second->production.end(); ++ir)
					phash += *ir;
				git = grammar.find(phash);
				if (git != grammar.end())//third if
				{
					sy = (*it)->symbol;
					//sy = (*it).second->symbol;
					if (sy == -3)
						*(action[i] + MACRONUM) = -1 * (git->second->num);//rj
					else
						*(action[i] + sy) = -1 * (git->second->num);//rj
				}
			}
		}
	}
	for (vector<Ep_Closure*>::iterator it = lrc->epset.begin(); it != lrc->epset.end(); ++it)
	{
		for (vector<LR1_Item*>::iterator i = (*it)->LR1_items.begin(); i != (*it)->LR1_items.end(); ++i)
			delete (*i);
		delete (*it);
	}
	delete lrc;
}

void syntax_analyser::analyser(deque<Token_Stream*>& token_stream)
{
	stack<pair<int, int>> stk;
	stk.push(pair<int, int>(0, -3));
	Token_Stream * t = new Token_Stream;
	t->macrocode = -3;
	t->macrocode = 0;
	token_stream.push_back(t);
	deque<Token_Stream*>::iterator ip = token_stream.begin();
	int ac_item, macro, ps;
	vector<int> * pro = NULL;
	while (true)
	{
		macro = (*ip)->macrocode;
		if (macro > 0)
			ac_item = *(action[stk.top().first] + macro);
		else
			ac_item = *(action[stk.top().first] + MACRONUM);
		if (ac_item == ACC)
			break;
		else if (ac_item > 0)//Si
		{
			stk.push(pair<int, int>(ac_item, macro));
			++ip;
		}
		else if (ac_item < 0)//ri
		{
			pro = &(grammar_p[-1 * ac_item]->production);
			for (int i = 0; i < pro->size() - 2; i++)
				stk.pop();
			ps = pro->at(0);//A
			stk.push(pair<int, int>(*(moveto[stk.top().first] + ps - MACRONUM), ps));
			for (vector<int>::iterator it = pro->begin(); it != pro->end(); ++it)
			{
				if (*it > 0)
					cout << *(nsmap[*it]);
				else
					cout << "->";
			}
			cout << '\n';
		}
		else
		{
			cout << stk.top().first << " " << macro << "  ERROR!";
			break;
		}
	}
}

void syntax_analyser::readlist()
{
	ifstream is("action1.txt");
	int * listline = NULL;
	string s;
	for (int j = 0; j < 255; j++)
	{
		listline = new int[MACRONUM + 1];//vtnum+#(-3)
		memset(listline, 0, (MACRONUM + 1) * sizeof(int));
		for (int i = 0; i <= MACRONUM; i++)
		{
			is >> s;
			*(listline + i) = atoi(s.c_str());
		}
		action.push_back(listline);
	}
	is.close();
	ifstream iss("goto1.txt");
	for (int j = 0; j < 255; j++)
	{
		listline = new int[snmap.size() - MACRONUM];//vnum
		memset(listline, 0, (snmap.size() - MACRONUM) * sizeof(int));
		for (int i = 0; i < snmap.size() - MACRONUM; i++)
		{
			iss >> s;
			*(listline + i) = atoi(s.c_str());
		}
		moveto.push_back(listline);
	}
	iss.close();
}

void syntax_analyser::excute(deque<Token_Stream*>& token_stream)
{
	production_reader();
	getfirst();
	//getcollection();
	readlist();
	//makelist();
	analyser(token_stream);
}

void syntax_analyser::semeatic(int ac_item)
{
	if (ac_item == 1)
	{

	}
}

syntax_analyser::~syntax_analyser()
{
	ifs.close();
}
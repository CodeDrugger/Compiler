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
	semeatic_error = 0;
	offset = 4;
	decdone = 0;
	toffset = TEMPOFFSET;
	floopstart = 0;
	wloopstart = 0;
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

void syntax_analyser::analyser(deque<Token_Stream*>& token_stream, map<std::string, Token_List*>&token_list)
{
	//unchecked_id = token_list.size();
	stack<pair<int, Attribute_Table*>> stk;
	Attribute_Table * at = new Attribute_Table(-3);
	stk.push(pair<int, Attribute_Table*>(0, at));
	Token_Stream * t = new Token_Stream;
	t->macrocode = -3;
	t->macrocode = 0;
	token_stream.push_back(t);
	deque<Token_Stream*>::iterator ip = token_stream.begin(), id_tmp = ip;
	int ac_item, macro, ps;
	vector<int> * pro = NULL;
	while (true)
	{
		if (semeatic_error) break;
		macro = (*ip)->macrocode;
		if (macro == ID)
			id_tmp = ip;
		if (macro > 0)
			ac_item = *(action[stk.top().first] + macro);
		else
			ac_item = *(action[stk.top().first] + MACRONUM);
		if (ac_item == ACC)
			break;
		else if (ac_item > 0)//Si
		{
			Attribute_Table * t = new Attribute_Table(macro);
			stk.push(pair<int, Attribute_Table*>(ac_item, t));
			++ip;
		}
		else if (ac_item < 0)//ri
		{
			vector<Attribute_Table*> * atpoped = new vector<Attribute_Table*>;
			pro = &(grammar_p[-1 * ac_item]->production);
			for (int i = 0; i < pro->size() - 2; i++)
			{
				Attribute_Table* tmp = stk.top().second;
				atpoped->push_back(tmp);
				stk.pop();
			}
			ps = pro->at(0);//A
			Attribute_Table * v = new Attribute_Table(ps);
			stk.push(pair<int, Attribute_Table*>(*(moveto[stk.top().first] + ps - MACRONUM), v));
			semeatic(ac_item, v, ip, atpoped);
			for (vector<Attribute_Table*>::iterator is = atpoped->begin(); is != atpoped->end(); ++is)
			{
				if ((*is)->others != 0)
					delete (*is)->others;
				delete *is;
			}
			delete atpoped;
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
	while (!stk.empty())
	{
		delete stk.top().second;
		stk.pop();
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

void syntax_analyser::excute(deque<Token_Stream*>& token_stream, map<string, Token_List*>& token_list)
{
	production_reader();
	getfirst();
	//getcollection();
	readlist();
	//makelist();
	analyser(token_stream, token_list);
	threecodeprinter();
}

void syntax_analyser::threecodeprinter()
{
	cout << "三地址码:\n";
	for (vector<Quadruple*>::iterator it = threecode.begin(); it != threecode.end(); ++it)
	{
		cout << (*it)->num << "  " << (*it)->op << "  " << (*it)->arg1 << "  " << (*it)->arg2 << "  " << (*it)->result << '\n';
	}
}

/*
ac_item表明规约的是第几个生成式
var即V，生成式的左端，包含属性，规约完成后压入栈中
ts是发生规约前输入的最后一个字符
grammar_p[ac_item].at(0) = var->x
*/
void syntax_analyser::semeatic(int ac_item, Attribute_Table * var, std::deque<Token_Stream*>::iterator &tsit, std::vector<Attribute_Table*> * atpoped)
{
	Token_List * tl = NULL;
	Quadruple * q = NULL;
	deque<Token_Stream*>::iterator tst;
	map<int, int>* mp = NULL;
	map<int, int>::iterator it;
	int fst, scd = 1;
	string * str = NULL;
	FalseList * flist = NULL, *ftmp = NULL;
	ac_item *= -1;
	if ((*tsit)->macrocode == FOR)
		floopstart = 1;
	if ((*tsit)->macrocode == WHILE)
		wloopstart = 1;
	if (wloopstart)
	{
		wloopjumpback.push(threecode.size());
		wloopstart = 0;
	}
	switch (ac_item)
	{
	case 1://Pre->B4
		//var->type = 1;
		var->value = (*(tsit - 1))->attribute;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), INASS, var->value, 0, var->offset);
		threecode.push_back(q);
		break;
	case 2://Pre->B1
		//var->type = 0;
		var->value = (*(tsit - 1))->attribute;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), INASS, var->value, 0, var->offset);
		threecode.push_back(q);
		break;
	case 3://Pre->ID
		tl = (Token_List *)(*(tsit - 1))->attribute;
		//var->type = 2;
		var->addr = (int)tl;
		//if (!decdone)
		//{
			//tl->checked = 1;
			//tl->off = offset;//
			//var->offset = offset;
			//offset += 4;//
		//}
		//else
		//{
		if (!tl->checked)
		{
			semeatic_error = 1;
			break;
		}
		var->offset = toffset;
		toffset += 4;
		var->isarray = tl->isarray;
		//var->value = tl->value;
		//var->type = tl->type;
		q = new Quadruple(threecode.size(), ASSIGN, tl->off, 0, var->offset);
		threecode.push_back(q);
		//}
		break;
	case 4://Poe->Pre
		var->copy(*atpoped->at(0));
		break;
	case 5://Poe->Poe++
		var->copy(*atpoped->at(1));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value++;
		tl = (Token_List*)var->addr;
		//tl->value++;//
		q = new Quadruple(threecode.size(), INC, tl->off, 0, 0);
		threecode.push_back(q);
		break;
	case 6://Poe->Poe--
		var->copy(*atpoped->at(1));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value--;
		tl = (Token_List*)var->addr;
		//tl->value--;//
		q = new Quadruple(threecode.size(), DEC, tl->off, 0, 0);
		threecode.push_back(q);
		break;
	case 7://Mue->Poe
		var->copy(*atpoped->at(0));
		break;
	case 8://Mue->Mue*Poe
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value *= atpoped->at(2)->value;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), MULTI, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 9://Mue->Mue/Poe
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value /= atpoped->at(2)->value;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), RDIV, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 10://Mue->Mue%Poe
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value %= atpoped->at(2)->value;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), MOD, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 11://Ade->Mue
		var->copy(*atpoped->at(0));
		break;
	case 12://Ade->Ade+Mue
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value += atpoped->at(2)->value;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), PLUS, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 13://Ade->Ade-Mue
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value -= atpoped->at(2)->value;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), MINUS, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 14://Ree->Pre
		var->copy(*atpoped->at(0));
		break;
	case 15://Ree->Ree<Pre
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value < atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), LT, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 16://Ree->Ree>Pre
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value > atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), GT, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 17://Ree->Ree<=Pre
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value <= atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), LE, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 18://Ree->Ree>=Pre
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value >= atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), GE, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 19://Eqe->Ree
		var->copy(*atpoped->at(0));
		break;
	case 20://Eqe->Eqe==Ree
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value == atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), EQ, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 21://Eqe->Eqe!=Ree
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value != atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), NE, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 22://Ane->Eqe
		var->copy(*atpoped->at(0));
		break;
	case 23://Ane->Ane&&Eqe
		var->copy(*atpoped->at(2));
		if (var->isarray)
		{
			semeatic_error = 1;
			break;
		}
		//var->value = atpoped->at(2)->value;
		//if (var->value && atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), AND, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		break;
	case 24://Ore->Ane
		var->copy(*atpoped->at(0));
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(2, threecode.size())); //2:Ore自己所在的三地址码序号
		q = new Quadruple(threecode.size(), IFISGO, var->offset, 0, -1);
		threecode.push_back(q);
		mp = var->others;
		it = mp->find(3); //3:falselist
		if (it == mp->end())
		{
			flist = new FalseList;
			flist->insaddr = &(q->result);
			var->others->insert(pair<int, int>(3, (int)flist));
		}
		else
		{
			flist = (FalseList*)it->second;
			ftmp = new FalseList;
			ftmp->insaddr = &(q->result);
			ftmp->num = flist->num + 1;
			ftmp->next = flist;
			it->second = (int)ftmp;
		}
		break;
	case 25://Ore->Ore||Ane
		var->copy(*atpoped->at(2));
		//var->value = atpoped->at(2)->value;
		//if (var->value || atpoped->at(2)->value)
		//	var->value = 1;
		//else
		//	var->value = 0;
		var->offset = toffset;
		toffset += 4;
		q = new Quadruple(threecode.size(), OR, atpoped->at(2)->offset, atpoped->at(0)->offset, var->offset);
		threecode.push_back(q);
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(2, threecode.size()));//2:Ore自己所在的三地址码序号
		q = new Quadruple(threecode.size(), IFISGO, var->offset, 0, -1);
		threecode.push_back(q);
		mp = var->others;
		it = mp->find(3);//3:falselist
		if (it == mp->end())
		{
			flist = new FalseList;
			flist->insaddr = &(q->result);
			var->others->insert(pair<int, int>(3, (int)flist));
		}
		else
		{
			flist = (FalseList*)it->second;
			ftmp = new FalseList;
			ftmp->insaddr = &(q->result);
			ftmp->num = flist->num + 1;
			ftmp->next = flist;
			it->second = (int)ftmp;
		}
		break;
	case 26://Dec->TypID;
		tst = tsit - 1 - 1;
		tl = (Token_List *)(*tst)->attribute;
		//var->addr = (int)tl;
		tl->isarray = 0;
		tl->checked = 1;
		tl->off = offset;
		offset += 4;
		//if (atpoped->at(1)->value == B4)
		//	tl->type = 1;
		//else
		//	tl->type = 0;
		break;
	case 27://Dec->TypIDArr;
		tst = tsit - 5;
		if (!(*tst)->attribute)
			tst = tsit - 8;
		tl = (Token_List*)(*tst)->attribute;
		tl->isarray = 1;
		tl->checked = 1;
		mp = atpoped->at(1)->others;
		it = mp->find(0);//0:数组第一维的维数
		fst = it->second;
		scd = 1;
		it = mp->find(1);//1:数组第二维的维数
		if (it != mp->end())
			scd = it->second;
		tl->off = offset;
		offset += 4 * fst * scd;
		tl->attr = new int[3];
		*(tl->attr + 1) = fst;
		*(tl->attr + 2) = scd;
		if (scd != 1)
			*(tl->attr) = 2;
		else
			*(tl->attr) = 1;
		//if (atpoped->at(3)->value == B4)
			//tl->type = 1;
		//else
			//tl->type = 0;
		break;
	case 28://Typ->INT
		//var->value = ts->macrocode;
		break;
	case 29://Typ->CHAR
		//var->value = ts->macrocode;
		break;
	case 30://Arr->[Pre]
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(0, atpoped->at(1)->value));//应该要有
		var->isarray = 1;
		break;
	case 31://Arr->[Pre][Pre]
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(0, atpoped->at(4)->value));
		var->others->insert(pair<int, int>(1, atpoped->at(1)->value));
		var->isarray = 1;
		break;
	case 32://Ras->Ra1

		break;
	case 33://Ras->Ra2

		break;
	case 34://Ra1->Idn=Ade
		q = new Quadruple(threecode.size(), ASSIGN, atpoped->at(0)->offset, 0, atpoped->at(2)->offset);
		threecode.push_back(q);
		if (floopstart)
		{
			floopjumpback.push(threecode.size());
			floopstart = 0;
		}
		break;
	case 35://Ra2->IdnArr=Ade
		mp = atpoped->at(2)->others;
		it = mp->find(0);
		fst = it->second;
		scd = 1;
		it = mp->find(1);
		if (it != mp->end())
			scd = it->second;
		q = new Quadruple(threecode.size(), ASSIGN, atpoped->at(0)->offset, 0, atpoped->at(3)->offset + 4*fst*scd);
		threecode.push_back(q);
		break;
	case 36://Ra2->Idn=IdnArr
		mp = atpoped->at(0)->others;
		it = mp->find(0);
		fst = it->second;
		scd = 1;
		it = mp->find(1);
		if (it != mp->end())
			scd = it->second;
		q = new Quadruple(threecode.size(), ASSIGN, atpoped->at(1)->offset + 4 * fst*scd, 0, atpoped->at(3)->offset);
		threecode.push_back(q);
		break;
	case 37://Idn->ID
		tl = (Token_List*)(*(tsit - 1))->attribute;
		//tl->off = offset;
		//offset += 4;
		var->offset = tl->off;
		break;
	case 38://Sta->Ras;

		break;
	case 39://Sta->Poe;

		break;
	case 40://Sta->Ifb
		toffset = TEMPOFFSET;
		break;
	case 41://Sta->Fob
		toffset = TEMPOFFSET;
		break;
	case 42://Sta->Whb	
		toffset = TEMPOFFSET;
		break;
	case 43://Sta->Ret
		toffset = TEMPOFFSET;
		break;
	case 44://Sta->;
		toffset = TEMPOFFSET;
		break;
	case 45://Sta->Re
		toffset = TEMPOFFSET;
		break;
	case 46://Ifb->IF(Ore){Blk}
		mp = atpoped->at(4)->others;
		it = mp->find(3);
		if (it == mp->end())
		{
			semeatic_error = 1;
			break;
		}
		flist = (FalseList*)it->second;
		while (flist != NULL)
		{
			*(flist->insaddr) = threecode.size();
			flist = flist->next;
		}
		break;
	case 47://Ifb->IF(Ore){Blk}ELSE{Blk}
		mp = atpoped->at(5)->others;
		it = mp->find(4);
		q = new Quadruple(threecode.size(), GOTO, 0, 0, threecode.size());
		threecode.push_back(q);
		threecode.insert(threecode.begin() + it->second, q);
		mp = atpoped->at(8)->others;
		it = mp->find(3);
		if (it == mp->end())
		{
			semeatic_error = 1;
			break;
		}
		flist = (FalseList*)it->second;
		while (flist != NULL)
		{
			*(flist->insaddr) = threecode.size();
			flist = flist->next;
		}
		break;
	case 48://Fob->FOR(Ra1;Ore;Ade){Blk}
		mp = atpoped->at(6)->others;
		q = new Quadruple(threecode.size(), GOTO, 0, 0, floopjumpback.top());//往ore跳	
		floopjumpback.pop();
		threecode.push_back(q);
		it = mp->find(3);
		if (it == mp->end())
		{
			semeatic_error = 1;
			break;
		}
		flist = (FalseList*)it->second;
		while (flist != NULL)
		{
			*(flist->insaddr) = threecode.size();
			flist = flist->next;
		}
		break;
	case 49://Whb->WHILE(Ore){Blk}
		mp = atpoped->at(4)->others;
		q = new Quadruple(threecode.size(), GOTO, 0, 0, wloopjumpback.top());//往ore跳
		wloopjumpback.pop();
		threecode.push_back(q);
		it = mp->find(3);
		if (it == mp->end())
		{
			semeatic_error = 1;
			break;
		}
		flist = (FalseList*)it->second;
		while (flist != NULL)
		{
			*(flist->insaddr) = threecode.size();
			flist = flist->next;
		}
		break;
	case 50://Ret->PRINTF(STR,Ids);
		tst = tsit - 4 - 1;
		str = (string*)(*tst)->attribute;
		q = new Quadruple(threecode.size(), PRT, (int)str, atpoped->at(2)->offset, 0);
		threecode.push_back(q);
		break;
	case 51://Re->RETURNAde;
		q = new Quadruple(threecode.size(), RETURN, 0, 0, 0);
		threecode.push_back(q);
		break;
	case 52://Des->Dec
		break;
	case 53://Des->DesDec
		break;
	case 54://Blk->Sta
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(4, threecode.size()));
		break;
	case 55://Blk->BlkSta
		//toffset = TEMPOFFSET;
		if (!var->others)
			var->others = new map<int, int>;
		var->others->insert(pair<int, int>(4, threecode.size()));
		break;
	case 56:
		break;
	case 57:
		break;
	case 58:
		break;
	default:
		break;
	}
	if (semeatic_error)
		cout << "SEMEATIC ERROR!";
}
//realsize dreamsize

syntax_analyser::~syntax_analyser()
{
	ifs.close();
}
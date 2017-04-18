//这里是要使用的数据结构
#pragma once
//词法分析器的输出（种别，属性）
struct Token_Stream
{
	int macrocode;
	int attribute;
};

//符号表项
struct Token_List
{
	std::string name;
	void * value;
};
//文法生成式
class Production
{
public:
	std::vector<int> production;
	int hash;
	int num;

	Production();
	void add_item(int);
	void grammar_init(int);
};
//LR1项目，形如[A->B.S,a]
class LR1_Item
{
public:
	int hash;
	//int hash_s;//展望符的hash
	int symbol;//展望符
	std::vector<int> production;//应该是单个的
	//std::vector<int> symbol;
	//std::vector<int>::iterator pos;//表示点在当前字符的后一位
	int pos;

	LR1_Item();
	void copy(LR1_Item*);
	bool pro_equal(int phash);
};
//闭包集，即DFA的状态I
class Ep_Closure
{
public:
	int hash;//自己的hash
	std::set<std::pair<int,int>> hash_set;//所包含的LR1项目的hash，便于判断某LR1项目是否在其中
	std::set<int> hash_set_s;//即symbols
	int item_num;
	std::vector<LR1_Item*> LR1_items;

	Ep_Closure();
	void add_item(LR1_Item* item);
	int have_item(LR1_Item &item);
	bool equal(Ep_Closure &ec);
	bool isempty();
};

//集族
class LR1_Collection
{
public:
	std::vector<Ep_Closure*> epset;
	std::set<int> hash_set;
	std::vector<int>hash_s;
	int item_num;

	LR1_Collection();
	void add_item(Ep_Closure* item);
	bool have_item(Ep_Closure* item);
	int contain(Ep_Closure* item);
};
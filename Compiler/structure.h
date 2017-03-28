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
	Production() { hash = 0; }
};
//LR1项目，形如[A->B.S,a]
struct LR1_Item
{
	int hash;
	std::vector<int> production;
	std::vector<int> symbol;
	int * pos;
};
//闭包集，即DFA的状态I
class Ep_Closure
{
public:
	int hash;
	int * hash_set;
	int item_num;
	std::vector<LR1_Item*> LR1_items;

	bool have_item(LR1_Item &item);
	bool equal(Ep_Closure &ec);
};

//集族
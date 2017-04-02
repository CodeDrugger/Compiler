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
	Production();
};
//LR1项目，形如[A->B.S,a]
class LR1_Item
{
public:
	int hash;
	int hash_s;//展望符的hash
	std::vector<int> production;//应该是单个的
	std::vector<int> symbol;
	std::vector<int>::iterator pos;//表示点在当前字符的后一位

	LR1_Item();
	LR1_Item* copy(LR1_Item*);
};
//闭包集，即DFA的状态I
class Ep_Closure
{
public:
	int hash;//自己的hash
	std::vector<int> hash_set;//所包含的LR1项目的hash，便于判断某LR1项目是否在其中
	std::vector<int> hash_set_s;
	int item_num;
	std::vector<LR1_Item*> LR1_items;

	Ep_Closure();
	void add_item(LR1_Item* item);
	int have_item(LR1_Item &item);
	bool equal(Ep_Closure &ec);
};

//集族
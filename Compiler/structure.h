//这里是要使用的数据结构
#pragma once
//词法分析器的输出（种别，属性）
struct Token_Stream
{
	int macrocode;
	int attribute;
};

//符号表项，只有ID
class Token_List
{
public:
	std::string name;
	bool checked;
	int isarray;
	//int value;//貌似没必要
	int type;//-1:init 0:char 1:int
	int off;//偏移
	int * attr;//维数，各维长度

	Token_List();
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
	//std::set<int> hash_set_s;//即symbols
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
//属性表
class Attribute_Table
{
public:
	int x;//v,t的内部序号
	int isarray;
	int offset;
	int addr;//当type是2时指向(符号表的地址)
	int value;//变量的值也要维护
	//int type;//0:char 1:int
	std::map<int, int> * others;//属性编号,属性值
	/*
	0:数组第一维的维数
	1:数组第二维的维数
	//2:Ore自己所在的三地址码序号
	3:Ore的falselist
	4:规约到blk时的threecode.size(),就是goto的插入点
	*/
	Attribute_Table(int x);
	void copy(const Attribute_Table& at);
};

//四元式
class Quadruple
{
public:
	int num;//序号
	int op;
	int arg1;
	int arg2;
	int result;

	Quadruple(int num, int op, int arg1, int arg2, int result);
};

//falselist
class FalseList
{
public:
	int num;
	int * insaddr;
	FalseList * next;

	FalseList();
};

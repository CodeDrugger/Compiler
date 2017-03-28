//������Ҫʹ�õ����ݽṹ
#pragma once
//�ʷ���������������ֱ����ԣ�
struct Token_Stream
{
	int macrocode;
	int attribute;
};

//���ű���
struct Token_List
{
	std::string name;
	void * value;
};
//�ķ�����ʽ
class Production
{
public:
	std::vector<int> production;
	int hash;
	Production() { hash = 0; }
};
//LR1��Ŀ������[A->B.S,a]
struct LR1_Item
{
	int hash;
	std::vector<int> production;
	std::vector<int> symbol;
	int * pos;
};
//�հ�������DFA��״̬I
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

//����
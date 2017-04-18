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
	int num;

	Production();
	void add_item(int);
	void grammar_init(int);
};
//LR1��Ŀ������[A->B.S,a]
class LR1_Item
{
public:
	int hash;
	//int hash_s;//չ������hash
	int symbol;//չ����
	std::vector<int> production;//Ӧ���ǵ�����
	//std::vector<int> symbol;
	//std::vector<int>::iterator pos;//��ʾ���ڵ�ǰ�ַ��ĺ�һλ
	int pos;

	LR1_Item();
	void copy(LR1_Item*);
	bool pro_equal(int phash);
};
//�հ�������DFA��״̬I
class Ep_Closure
{
public:
	int hash;//�Լ���hash
	std::set<std::pair<int,int>> hash_set;//��������LR1��Ŀ��hash�������ж�ĳLR1��Ŀ�Ƿ�������
	std::set<int> hash_set_s;//��symbols
	int item_num;
	std::vector<LR1_Item*> LR1_items;

	Ep_Closure();
	void add_item(LR1_Item* item);
	int have_item(LR1_Item &item);
	bool equal(Ep_Closure &ec);
	bool isempty();
};

//����
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
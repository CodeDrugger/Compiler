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
	Production();
};
//LR1��Ŀ������[A->B.S,a]
class LR1_Item
{
public:
	int hash;
	int hash_s;//չ������hash
	std::vector<int> production;//Ӧ���ǵ�����
	std::vector<int> symbol;
	std::vector<int>::iterator pos;//��ʾ���ڵ�ǰ�ַ��ĺ�һλ

	LR1_Item();
	LR1_Item* copy(LR1_Item*);
};
//�հ�������DFA��״̬I
class Ep_Closure
{
public:
	int hash;//�Լ���hash
	std::vector<int> hash_set;//��������LR1��Ŀ��hash�������ж�ĳLR1��Ŀ�Ƿ�������
	std::vector<int> hash_set_s;
	int item_num;
	std::vector<LR1_Item*> LR1_items;

	Ep_Closure();
	void add_item(LR1_Item* item);
	int have_item(LR1_Item &item);
	bool equal(Ep_Closure &ec);
};

//����
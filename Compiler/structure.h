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
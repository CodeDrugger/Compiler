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
	char name[32];
	void * value;
};
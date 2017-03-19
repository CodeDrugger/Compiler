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
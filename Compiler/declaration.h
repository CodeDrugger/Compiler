//�����Ǵ��༰���ú���������
#pragma once
class token_scanner
{
public:
	char dbuffer[1024];
	char *begin, *forward,*buffer_writer;
	char ch;
	char token[32];
	bool which_emtpy;
	std::ifstream ifs;
	std::deque<Token_Stream*> token_stream;
	std::deque<Token_List*> token_list;

	token_scanner();
	void prepare();
	void buffer_scanner();
};

bool iskeyword(std::string s);
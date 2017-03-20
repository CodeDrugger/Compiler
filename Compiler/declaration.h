//这里是大类及常用函数的申明
#pragma once
class token_scanner
{
public:
	char dbuffer[1024];
	char *begin, *forward,*buffer_writer;
	char ch;
	char token[32];
	bool which_emtpy;
	bool error;
	bool dq_mark;
	std::ifstream ifs;
	std::deque<Token_Stream*> token_stream;
	std::deque<Token_List*> token_list;

	token_scanner();
	void prepare();
	void buffer_scanner();
	void token_install(int macrocode, int value, std::string &s, int mode);
	std::string gettoken();
	void error_handler(std::string &msg);
	void goahead(bool &isend);
	void movebegin();
	char getfchar(int dis);
	int getbfdis();
	std::string getstring();
	void show();
};

int wordclassify(std::string s);
bool isescape(char c);
char toescape(char c);
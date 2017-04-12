//这里是大类及常用函数、全局变量的申明
#pragma once
class token_scanner
{
public:
	char dbuffer[1024];
	char *begin, *forward, *buffer_writer;
	//char ch;
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
	void excute();
	~token_scanner();
};

class syntax_analyser
{
public:
	std::ifstream ifs;
	std::vector<Production*> production_list;
	std::map<int, Production*> grammar;
	std::vector<std::string*> nsmap;
	std::map<std::string, int> snmap;
	std::map<int, std::set<int>*> first_set;
	LR1_Collection * lrc;
	std::vector<int*> action;
	std::vector<int*> moveto;

	syntax_analyser();
	void production_reader();
	void getfirst();
	Ep_Closure* getclosure(Ep_Closure* ep);
	Ep_Closure* go(Ep_Closure* ep,int x);
	void getcollection();
	void makelist();
	void excute();
	//~syntax_analyser();
};

int wordclassify(std::string s);
bool isescape(char c);
char toescape(char c);

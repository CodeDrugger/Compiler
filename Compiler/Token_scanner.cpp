#include "stdafx.h"

using namespace std;

token_scanner::token_scanner()
{
	begin = NULL;
	forward = NULL;
	buffer_writer = NULL;
	which_emtpy = 0;
	error = 0;
	dq_mark = 0;
	ifs = ifstream("code.c");
}

void token_scanner::prepare()
{
	char c;
	int i = 0;
	if (!which_emtpy)
		buffer_writer = dbuffer;
	else buffer_writer = dbuffer + 512;
	which_emtpy = !which_emtpy;
	while (i < 511)
	{
		ifs.get(*buffer_writer);
		if (*buffer_writer == 0)
		{
			*buffer_writer = 0xff;
			break;
		}
		if (*buffer_writer == '"' && *(buffer_writer - 1) != 92)
			dq_mark = !dq_mark;
		if (!dq_mark)
		{
			if (*buffer_writer == ' ' || *buffer_writer == '\n' || *buffer_writer == '\t')
				*buffer_writer = ' ';
			while (*buffer_writer == ' ' && ifs.peek() == ' ')
				ifs.get(c);
		}
		if (*buffer_writer == '/')
		{
			if (ifs.peek() == '/')
			{
				do
				{
					ifs.get(c);
				} while (c != '\n');
			}
			else if (ifs.peek() == '*')
			{
				do
				{
					ifs.get(c);
					if (c == -1)
					{
						string s = "ע�Ͳ���գ������������´��븽��:";
						error_handler(s);
						return;
					}
				} while (!(c == '*' && ifs.peek() == '/'));
			}
			buffer_writer--;
			i--;
		}
		i++;
		buffer_writer++;
	}
	*(dbuffer + 511) = 0xfd;
	*(dbuffer + 1023) = 0xfe;
}

void token_scanner::buffer_scanner()
{
	begin = dbuffer;
	forward = dbuffer;
	bool isend = 0;
	bool q_mark = 1;
	//bool bbq_mark = 1;
	bool isinstr = 0;
	string useless;
	string msg;
	stack<bool> s_brac, m_brac, l_brac;
	while (!isend)
	{
		if (!isinstr)
		{
			if (isspace(*forward))
			{
				goahead(isend);
				movebegin();
			}
			else if (isalpha(*forward))
			{
				if (getfchar(1) == '\\')
				{
					goahead(isend);
					continue;
				}
				goahead(isend);
				while (isalnum(*forward))
				{
					if (getbfdis() > 31)
					{
						error = 1;
						break;
					}
					goahead(isend);
				}
				if (error)
				{
					msg = "��ʶ�����ȹ����������������´��븽��:";
					error_handler(msg);
					return;
				}
				string s = gettoken();
				movebegin();
				int macrocode = wordclassify(s);
				if (macrocode == ID)
					token_install(macrocode, 0, s, 0);
				else
					token_install(macrocode, 0, useless, 1);
			}
			else if (isdigit(*forward))
			{
				if (*forward == '0')
				{
					goahead(isend);
					if (*forward == 'x')
					{
						goahead(isend);
						while (isxdigit(*forward))
						{
							if (getbfdis() > 9)
							{
								error = 1;
								break;
							}
							goahead(isend);
						}
						if (error)
						{
							msg = "�������ȹ����������������´��븽��:";
							error_handler(msg);
							return;
						}
						string s = gettoken();
						movebegin();
						token_install(B4, strtol(s.c_str(), 0, 16), useless, 2);
					}
					else
					{
						movebegin();
						token_install(B4, 0, useless, 2);
					}
				}
				else
				{
					goahead(isend);
					while (isdigit(*forward))
					{
						if (getbfdis() > 9)
						{
							error = 1;
							break;
						}
						goahead(isend);
					}
					if (error)
					{
						msg = "�������ȹ����������������´��븽��:";
						error_handler(msg);
						return;
					}
					string s = gettoken();
					movebegin();
					token_install(B4, atoi(s.c_str()), useless, 2);
				}
			}
			else
			{
				switch (*forward)
				{
				case '+':
					goahead(isend);
					if (*forward == '+')
					{
						goahead(isend);
						token_install(INC, 0, useless, 2);
					}
					else token_install(PLUS, 0, useless, 2);
					movebegin();
					break;
				case '-':
					goahead(isend);
					if (*forward == '-')
					{
						goahead(isend);
						token_install(DEC, 0, useless, 2);
					}
					else token_install(MINUS, 0, useless, 2);
					break;
				case '*':
					goahead(isend);
					token_install(MULTI, 0, useless, 2);
					movebegin();
					break;
				case '/':
					goahead(isend);
					token_install(RDIV, 0, useless, 2);
					movebegin();
					break;
				case '&':
					goahead(isend);
					if (*forward == '&')
					{
						goahead(isend);
						token_install(AND, 0, useless, 2);
					}
					//else token_install(ADDRESS, 0, useless, 2);
					movebegin();
					break;
				case '|':
					goahead(isend);
					if (*forward == '|')
					{
						goahead(isend);
						token_install(OR, 0, useless, 2);
					}
					else
					{
						msg = "�Ƿ�����'|'�������������´��븽��:";
						error_handler(msg);
						return;
					}
					movebegin();
					break;
				case '!':
					goahead(isend);
					if (*forward == '=')
					{
						goahead(isend);
						token_install(NE, 0, useless, 2);
					}
					else token_install(NOT, 0, useless, 2);
					movebegin();
					break;
				case '=':
					goahead(isend);
					if (*forward == '=')
					{
						goahead(isend);
						token_install(EQ, 0, useless, 2);
					}
					else token_install(ASSIGN, 0, useless, 2);
					movebegin();
					break;
				case '<':
					goahead(isend);
					if (*forward == '=')
					{
						goahead(isend);
						token_install(LE, 0, useless, 2);
					}
					else if (*forward == '>')
					{
						goahead(isend);
						token_install(NE, 0, useless, 2);
					}
					/*else if (isalpha(*forward))
					{
						goahead(isend);
						while (isalpha(*forward) || *forward == '.')
						{
							goahead(isend);
							if (*forward == '>')
								break;
						}
						string s = getstring();
						s = s.substr(1);
						goahead(isend);
						token_install(HEADER, 0, s, 0);
					}*/
					else token_install(LT, 0, useless, 2);
					movebegin();
					break;
				case '>':
					goahead(isend);
					if (*forward == '=')
					{
						goahead(isend);
						token_install(GE, 0, useless, 2);
					}
					else token_install(GT, 0, useless, 2);
					movebegin();
					break;
				case '(':
					goahead(isend);
					token_install(LS_BRAC, 0, useless, 2);
					s_brac.push(0);
					movebegin();
					break;
				case ')':
					goahead(isend);
					token_install(RS_BRAC, 0, useless, 2);
					s_brac.pop();
					movebegin();
					break;
				case '[':
					goahead(isend);
					token_install(LM_BRAC, 0, useless, 2);
					m_brac.push(0);
					movebegin();
					break;
				case ']':
					goahead(isend);
					token_install(RM_BRAC, 0, useless, 2);
					m_brac.pop();
					movebegin();
					break;
				case '{':
					goahead(isend);
					token_install(LL_BRAC, 0, useless, 2);
					l_brac.push(0);
					movebegin();
					break;
				case '}':
					goahead(isend);
					token_install(RL_BRAC, 0, useless, 2);
					l_brac.pop();
					movebegin();
					break;
				case ';':
					goahead(isend);
					token_install(SEMIC, 0, useless, 2);
					movebegin();
					break;
				case '\'':
					goahead(isend);
					//token_install(Q_MARK, 0, useless, 2);
					q_mark = !q_mark;
					if (q_mark)//��β��ʼ
					{
						//�κ���ȷ�ĵ����ţ���-2�ַ����������Ż�б�ܣ��������
						if (getfchar(3) != '\'' && getfchar(3) != '\\')//'a'+'b'+'\''+'\\'
						{
							msg = "�����Ų�ƥ�䣬�����������´��븽��:";
							error_handler(msg);
							return;
						}
						//�����ַ����ţ���-1�ַ��Ƿ�б�ܣ�-2�ַ�������
						if (getfchar(2) == '\\' && getfchar(3) == '\'')//'a'+'b'+'\(')'+'\\'
						{
							q_mark = !q_mark;
							//�κ��ַ����ţ���+1�ַ������ţ��������
							if (*(forward) != '\'')
							{
								msg = "�����Ų�ƥ�䣬�����������´��븽��:";
								error_handler(msg);
								return;
							}
						}
						//����ת���ַ�ĩ�����ţ���-2�ַ��Ƿ�б��
						else if (getfchar(3) == '\\')//'a'+'b'+'\''+'\\(')+'\n(')+'\'(')
						{
							//�κ���ȷ��ת���ַ�ĩ�����ţ���-1�ַ����������š���б�ܻ�ת������������
							if (getfchar(2) != '\'' && getfchar(2) != '\\' && !isescape(getfchar(2)))
							{
								msg = "�����Ų�ƥ�䣬�����������´��븽��:";
								error_handler(msg);
								return;
							}
						}
						//�ַ�ĩ������
						if (q_mark)//'a'+'b'+'\''+'\\'
						{
							if (getfchar(3) == '\'')//����������
								token_install(B1, (int)getfchar(2), useless, 2);
							else if (getfchar(3) == '\\')
							{
								if (isescape(getfchar(2)) || getfchar(2) == '\\' || getfchar(2) == '\'')
									token_install(B1, toescape(getfchar(2)), useless, 2);
							}
							else
							{
								msg = "�޷�ʶ���ת���ַ��������������´��븽��:";
								error_handler(msg);
								return;
							}
						}

					}
					else
						goahead(isend);
					movebegin();
					break;
				case '"':
					goahead(isend);
					//token_install(DQ_MARK, 0, useless, 2);
					/*bbq_mark = !bbq_mark;
					if (bbq_mark)//��wei��ʼ
					{
						if (getfchar(1) == '\\')
							bbq_mark = !bbq_mark;
						else
						{
							string s = getstring();
							token_install(STR, 0, s, 0);
							movebegin();
						}
					}
					else
					{
						isinstr = !isinstr;
						movebegin();
					}*/
					isinstr = !isinstr;
					movebegin();
					break;
				case '\\':
					goahead(isend);
					break;
				case ',':
					goahead(isend);
					token_install(COMMA, 0, useless, 2);
					movebegin();
					break;
					/*case '#':
						goahead(isend);
						token_install(POUND, 0, useless, 2);
						movebegin();
						break;
					case '.':
						goahead(isend);
						token_install(FSTOP, 0, useless, 2);
						movebegin();
						break;*/
				}
			}
		}
		else
		{
			if (getbfdis() > 511)
			{
				msg = "�ַ������ȹ����������������´��븽��:";
				error_handler(msg);
				return;
			}
			if (*forward == '"' && getfchar(1) != '\\')
			{
				isinstr = !isinstr;
				string s = getstring();
				token_install(STR, 0, s, 0);
			}
			goahead(isend);
		}
	}
	if (!s_brac.empty())
		cout << "С���Ų�ƥ��" << endl;
	if (!m_brac.empty())
		cout << "�����Ų�ƥ��" << endl;
	if (!l_brac.empty())
		cout << "�����Ų�ƥ��" << endl;
}

string token_scanner::getstring()
{
	char * str = new char[getbfdis() + 1];
	memset(str, 0, getbfdis() + 1);
	char * p = str;
	for (char * scpy = begin; scpy != forward; scpy++)
	{
		if (scpy > dbuffer + 1023)
			scpy = dbuffer;
		if (*scpy > 0)
		{
			*p = *scpy;
			p++;
		}
	}
	string s = str;
	delete[] str;
	return s;
}

string token_scanner::gettoken()
{
	memset(token, 0, 32);
	int i = 0;
	for (char * tokencpy = begin; tokencpy != forward; tokencpy++)
	{
		if (tokencpy > dbuffer + 1023)
			tokencpy = dbuffer;
		if (*tokencpy > 0)
		{
			token[i] = *tokencpy;
			i++;
		}
	}
	string s = token;
	return s;
}

void token_scanner::token_install(int macrocode, int value, string &s, int mode)
{
	Token_Stream * ts = new Token_Stream;
	ts->macrocode = macrocode;
	pair<map<string, Token_List*>::iterator, bool> ret;
	if (mode == 0)//��ʶ�� �ַ���
	{
		Token_List * tl = new Token_List;
		tl->name = string(s);
		ret = token_list.insert(pair<string, Token_List*>(s, tl));
		ts->attribute = (int)(*ret.first).second;
	}
	else if (mode == 1)//�����ؼ���
	{
		ts->attribute = 0;
	}
	else if (mode == 2)//int ����
	{
		ts->attribute = value;
	}
	token_stream.push_back(ts);
}

void token_scanner::error_handler(string &msg)
{
	char * ep = forward;
	string tmp;
	int times = 0;
	while (ep >= dbuffer)
	{
		if (*ep < 0) break;
		tmp += *ep;
		if (isspace(*ep))
		{
			times++;
			if (times == 3) break;
		}
		ep--;
	}
	for (int i = tmp.size() - 2; i >= 0; i--)
	{
		msg += tmp[i];
	}
	cout << msg << endl;
	msg = "";
}

void token_scanner::goahead(bool &isend)
{
	if (forward < dbuffer + 1024)
		forward++;
	if (*forward == -1)
	{
		isend = 1;
		return;
	}
	else if (*forward == -2)
	{
		forward = dbuffer;
		this->prepare();
	}
	else if (*forward == -3)
	{
		forward++;
		this->prepare();
	}

}

void token_scanner::movebegin()
{
	//char * oldpos = begin;
	begin = forward;
	/*if (oldpos < dbuffer + 511 && begin > dbuffer + 511)
		this->prepare();
	else if (begin < oldpos)
		this->prepare();*/
}

char token_scanner::getfchar(int dis)
{
	if (forward - dis < dbuffer)
	{
		if (forward + 1024 - dis == dbuffer + 1023)
			return *(forward + 1024 - dis - 1);
		else return *(forward + 1024 - dis);
	}
	/*else if (forward - dis < begin)
		return 0xfc;*/
	else if (forward - dis == dbuffer + 511)
		return *(forward - dis - 1);
	else return *(forward - dis);
}

int token_scanner::getbfdis()
{
	int tmp = forward - begin;
	if (tmp < 0)
		tmp += 1024;
	return tmp;
}

void token_scanner::show()
{
	cout << "��Ԫ��" << endl;
	for (int i = 0; i < token_stream.size(); i++)
	{
		cout << token_stream.at(i)->macrocode << "  " << token_stream.at(i)->attribute << endl;
	}
	cout << "���ű�" << endl;
	for (map<string, Token_List*>::iterator it = token_list.begin(); it != token_list.end(); ++it)
	{
		cout << &((*it).second) << "  " << (*it).first << '\n';
	}
}

void token_scanner::excute()
{
	this->prepare();
	this->buffer_scanner();
	this->show();
}

token_scanner::~token_scanner()
{
	ifs.close();
}
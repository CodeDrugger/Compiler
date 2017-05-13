#include "stdafx.h"

using namespace std;

code_generator::code_generator()
{
	strnum = 0;
}

void code_generator::analyser(vector<Quadruple*>& threecode, map<string, Token_List*>& token_list, int maxoffset, std::vector<std::string*>& stringlist)
{
	ofstream os("asm.asm");
	os << ".586\n.model flat, stdcall\n.stack 4096\noption casemap : none\n"
		<< "includelib \"e:\\msvcrt.lib\"\nprintf PROTO C : ptr byte, : vararg\n"
		<< "ExitProcess	PROTO, dwExitCode : DWORD\n"
		<< "exit equ <INVOKE ExitProcess, 0>\nchr$ MACRO any_text : VARARG\n"
		<< "LOCAL txtname\n.data\nIFDEF __UNICODE__\nWSTR txtname, any_text\n"
		<< "align 4\n.code\nEXITM <OFFSET txtname>\nENDIF\ntxtname db any_text, 0\n"
		<< "align 4\n.code\nEXITM <OFFSET txtname>\nENDM\n.data\n";
	Token_List* tl = NULL;
	int * attr = NULL;
	vector<Token_List*> orderd;
	orderd.assign(token_list.size(), 0);
	for (map<string, Token_List*>::iterator it = token_list.begin(); it != token_list.end(); ++it)
	{
		tl = (*it).second;
		adname.insert(pair<int, string>((tl)->off, (*it).first));
		orderd[tl->off / 4] = tl;
	}
	for (vector<Token_List*>::iterator it = orderd.begin(); it != orderd.end(); ++it)
	{
		if ((*it)->isarray)
		{
			attr = (*it)->attr;
			os << (*it)->name << " dword " << (*(attr + 1)) * (*(attr + 2)) << " dup(0)\n";
		}
		else
		{
			os << (*it)->name << " dword 0\n";
		}
	}
	os << "buff dword " << maxoffset - 1000 << " dup(0)\n.code\nmain proc\n";
	for (vector<Quadruple*>::iterator it = threecode.begin(); it != threecode.end(); ++it)
	{
		switch ((*it)->op)
		{
		case 15://++
			os << 'L' << (*it)->num << ": inc " << conv((*it)->arg1) << '\n';
			break;
		case 16://--
			os << 'L' << (*it)->num << ": dec " << conv((*it)->arg1) << '\n';
			break;
		case 12://*
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov edx,0\n";
			os << "mul " << conv((*it)->arg2) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 13:///
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov edx,0\n";
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "div ebx\n";
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 14://%
			os << 'L' << (*it)->num << ": ";
			os << "mov eax " << conv((*it)->arg1) << '\n';
			os << "mov edx,0\n";
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "div ebx\n";
			os << "mov " << conv((*it)->result) << " edx\n";
			break;
		case 10://+
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "add eax," << conv((*it)->arg2) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 11://-
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "sub eax," << conv((*it)->arg2) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 21://LT
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "jnl C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 22://GT
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "jng C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 23://LE
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "jnle C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 24://GE
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "jnge C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 20://EQ
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "jne C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 25://NE
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov ebx," << conv((*it)->arg2) << '\n';
			os << "cmp eax,ebx\n";
			os << "je C" << (*it)->num << '\n';
			os << "mov " << conv((*it)->result) << ",1\n";
			os << "jmp L" << (*it)->num + 1 << '\n';
			os << "C" << (*it)->num << ": ";
			os << "mov " << conv((*it)->result) << ",0\n";
			break;
		case 17://AND
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "and eax," << conv((*it)->arg2) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 18://OR
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "or eax," << conv((*it)->arg2) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 32://ASSIGN
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 54://INASS
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << (*it)->arg1 << '\n';
			os << "mov " << conv((*it)->result) << ",eax\n";
			break;
		case 51://IFISGO
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg1) << '\n';
			os << "cmp eax,0\n";
			os << "je L" << (*it)->result << '\n';
			break;
		case 52://GOTO
			os << 'L' << (*it)->num << ": ";
			os << "jmp L" << (*it)->result << '\n';
			break;
		case 53://PRT
			os << 'L' << (*it)->num << ": ";
			os << "invoke printf, chr$(\"" << *stringlist[strnum] << "\", 0DH, 0AH)," << conv((*it)->arg2) << '\n';
			strnum++;
			break;
		case 55://ATI
			os << 'L' << (*it)->num << ": ";
			os << "mov eax," << conv((*it)->arg2) << '\n';
			os << "mov ebx,[" << conv((*it)->arg1) << "+eax*4]" << '\n';
			os << "mov " << conv((*it)->result) << ",ebx\n";
			break;
		case 56://ITA
			os << 'L' << (*it)->num << ": ";
			os << "mov ebx," << conv((*it)->result) << '\n';
			os << "mov eax," << conv((*it)->arg2) << '\n';
			os << "mov [" << conv((*it)->arg1) << "+eax*4],ebx" << '\n';
			break;
		default:
			break;
		}
	}
	os << "exit\nmain ENDP\nEND	main";
}

string code_generator::conv(int addr)
{
	if (addr >= 1000)
	{
		string s1 = "[buff]";
		return to_string((addr - 1000)) + s1;
	}
	else
	{
		map<int, string>::iterator it = adname.find(addr);
		if (it == adname.end())
		{
			int smallest = 1000;
			int off = 0;
			string var;
			for (map<int, string>::iterator i = adname.begin(); i != adname.end(); ++i)
			{
				off = addr - (*i).first;
				if (off > 0 && off < smallest)
				{
					smallest = off;
					var = (*i).second;
				}
			}
			string s1 = "[", s2 = "]";
			return to_string(smallest) + s1 + var + s2;
		}
		else
			return (*it).second;
	}
}

code_generator::~code_generator()
{
	os.close();
}
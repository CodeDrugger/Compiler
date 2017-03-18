#include "stdafx.h"

using namespace std;

token_scanner::token_scanner() 
{
	begin = NULL;
	forward = NULL;
	buffer_writer = NULL;
	which_emtpy = 0;
	ifs = ifstream("code.c");
}

void token_scanner::prepare()
{
	char c;
	int i = 0;
	if (!which_emtpy)
		buffer_writer = dbuffer;
	else buffer_writer = dbuffer + 512;
	while (i < 511)
	{
		ifs.get(*buffer_writer);
		if (*buffer_writer == ' ' || *buffer_writer == '\n' || *buffer_writer == '\t')
			*buffer_writer = ' ';
		if (ifs.peek() == EOF) break;
		while (*buffer_writer == ' ' && ifs.peek() == ' ')
		{
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
				} while (!(c == '*' && ifs.peek() == '/'));
			}
			buffer_writer--;
		}
		i++;
		buffer_writer++;
	}
	*(dbuffer + 511) = 0xff;
	*(dbuffer + 1023) = 0xff;
}

void token_scanner::buffer_scanner()
{
	begin = dbuffer;
	forward = dbuffer;
	if (isalpha(*forward))
	{

	}
}
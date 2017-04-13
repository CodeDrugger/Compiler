#include "stdafx.h"

using namespace std;

Production::Production()
{
	hash = 0;
}

void Production::add_item(int x)
{
	this->production.push_back(x);
	this->hash += x;
}

void Production::grammer_init(int x)
{
	this->production.push_back(x);
	this->production.push_back(-1);
	this->hash += x * x - 1;
}

LR1_Item::LR1_Item()
{
	hash = 0;
}

void LR1_Item::copy(LR1_Item* item)
{
	this->hash = item->hash;
	this->pos = item->pos;
	this->production = item->production;
	this->symbol = item->symbol;
}

Ep_Closure::Ep_Closure()
{
	hash = 0;
	item_num = 0;
}

int Ep_Closure::have_item(LR1_Item& item)
{
	int hash = item.hash;//包含了pos，生成式
	int symbol = item.symbol;//包含展望符
	for (int j = 0; j < this->hash_set.size(); j++)
	{
		if (this->hash_set[j] == hash)
		{
			for (vector<int>::iterator i = this->hash_set_s.begin(); i != this->hash_set_s.end(); i++)
				if (*i == symbol && i - hash_set_s.begin() == j) return 0;//存在
			//return j;//心存在，展望符不存在
		}
	}
	return -1;//不存在
	/*
	for (int j = 0; j < hash_set.size(); j++)
	{
		if (hash_set[j] == hash)
		{
			for (vector<int>::iterator i = hash_set_s.begin(); i != hash_set_s.end(); i++)
			{
				if (*i == hash_s) return 0;//存在
			}
			return j;//心存在，展望符不存在
		}
	}
	return -1;//不存在
	*/
}

void Ep_Closure::add_item(LR1_Item* item)
{
	hash += item->hash + item->symbol;
	LR1_items.push_back(item);
	hash_set.push_back(item->hash);
	hash_set_s.push_back(item->symbol);
	item_num++;
}

bool Ep_Closure::isempty()
{
	if (this->item_num == 0) 
		return true;
	else return false;
}

LR1_Collection::LR1_Collection()
{
	item_num = 0;
}

void LR1_Collection::add_item(Ep_Closure* item)
{
	this->epset.push_back(item);
	this->item_num++;
	this->hash_set.push_back(item->hash);
}

bool LR1_Collection::have_item(Ep_Closure * item)
{
	int hash = item->hash;
	vector<int> * hash_set = &(this->hash_set);
	for (vector<int>::iterator it = hash_set->begin(); it != hash_set->end(); it++)
	{
		if (*it == hash) return true;
	}
	return false;
}

int LR1_Collection::contain(Ep_Closure* item)
{
	int hash = item->hash;
	for (int i = 0; i < this->hash_set.size(); i++)
	{
		if (hash_set[i] == hash)
			return i;
	}
	return -1;
}
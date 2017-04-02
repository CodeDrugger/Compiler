#include "stdafx.h"

using namespace std;

Production::Production()
{
	hash = 0;
}

LR1_Item::LR1_Item()
{
	hash = 0;
	hash_s = 0;
}

LR1_Item* LR1_Item::copy(LR1_Item* item)
{
	this->hash = item->hash;
	this->hash_s = item->hash_s;
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
	int hash = item.hash;
	int hash_s = item.hash_s;
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
}

void Ep_Closure::add_item(LR1_Item* item)
{
	LR1_items.push_back(item);
	hash_set.push_back(item->hash);
	hash_set_s.push_back(item->hash_s);
	item_num++;
}
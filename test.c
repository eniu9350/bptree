#include "bptree.h"

#include <stdio.h>
int main()
{
	pagelist* pl;
	bptree* t;
	bptree_inode* in;
	bptree_fnode* fn;
	int k,v;
	int i;
	//void* p;
	pl = pagelist_create();
	t = bptree_create(300);
	
	for(i=0;i<1000000;i++)	{
		k = i+1;
		v = 42666;
		bptree_insert(pl, t, &k, &v);
	}
	bptree_show(pl, t);
}

int main2()
{
	pagelist* pl;
	bptree* t;
	bptree_inode* in;
	bptree_fnode* fn;
	int k,v;
	int i;
	//void* p;
	pl = pagelist_create();
	t = bptree_create(4);
	k = 3;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 6;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 19;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 7;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 4;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 5;
	v = 42666;
	//bptree_insert(pl, t, &k, &v);
	bptree_show(pl, t);
}
int main1()
{
	pagelist* pl;
	bptree* t;
	bptree_inode* in;
	bptree_fnode* fn;
	int k,v;
	//void* p;
	pl = pagelist_create();
	t = bptree_create(4);
	k = 30;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 100;
	v = 55666;
	bptree_insert(pl, t, &k, &v);
	k = 60;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 190;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 72;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 40;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 50;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 35;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 45;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 31;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	k = 32;
	v = 42666;
	bptree_insert(pl, t, &k, &v);
	bptree_show(pl, t);
	//fn = bptree_fnode_create(pl, t);
	//p = pagelist_get_free_page(pl);
	//in = bptree_inode_create(pl, 5);	
	//while(1)
	{
		printf("\n...\n");
	}
}

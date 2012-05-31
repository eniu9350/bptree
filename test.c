#include "bptree.h"

#include <stdio.h>

int main()
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
	bptree_show(t);
	//fn = bptree_fnode_create(pl, t);
	//p = pagelist_get_free_page(pl);
	//in = bptree_inode_create(pl, 5);	
	//while(1)
	{
		printf("\n...\n");
	}
}

#include "restore.h"

#include <stdio.h>
#include <stdlib.h>


//-----util method------------
static void backup_write_value(FILE* f, value* v)
{
	fwrite((void*)v, 1, sizeof(value), f);	
	//mmmmm: int* -> char* ok?
}

static void backup_write_key(FILE* f, key* k)
{
	fwrite((void*)k, 1, sizeof(key), f);	
	//mmmmm: int* -> char* ok?
}

static void backup_write_index_splitter(FILE* f)
{
	char s = RESTORE_INDEX_SPLITTER; 
	fwrite(&s, 1, sizeof(s), f);
	fwrite(&s, 1, sizeof(s), f);
	fwrite(&s, 1, sizeof(s), f);
	fwrite(&s, 1, sizeof(s), f);
}

//-----snappy------------
void backup_snappy(pagelist* pl, bptree* t, char* fnindex, char* fndata)
{
	FILE* f;
	int i;
	void* p;	
	bptree_fnode* fnode;

	printf("backup_snappy start\n");

	//write index file
	f = fopen(fnindex, "wb");
	p = (void*)t->root;
	for(i=0;i<t->height-1;i++)	{
		p = ((bptree_inode*)p)->children[0];
	}

	if(BPTREE_NODE_ISINNER(pl, p))	{
		perror("backup_snappy: must be fnode!");
	}

	fnode = (bptree_fnode*)p;
	while(fnode)	{
		backup_write_key(f, &fnode->keys[fnode->len-1]);
		printf("key written: %d\n", fnode->keys[fnode->len-1]);
		if(fnode->next)	{
			backup_write_index_splitter(f);	
		}
		fnode = fnode->next;
	}
	fclose(f);
	//write data file

}

bptree* restore_snappy(pagelist* pl, int order, char* fnindex, char* fndata)
{
	FILE* f;
	int k;
	int i;
	char c;
	bptree* t;
	bptree_fnode* fn;
	bptree_fnode* prevfn = NULL;
	int nnode, nnode_prev;	//count of node of this height,last height
	int tempv = 0;
	//	key* klist;
	void** nlist;
	void** nlist_prev;
	int nkey;
	int nkey_prev;
	int g;
	bptree_inode* in;
	void** children;

	t = bptree_create(order);

	f = fopen(fnindex, "rb");
	//create fnode without value
	nkey = 0;
	nlist = (void**)malloc(1000000*sizeof(void*));
	nlist_prev = (void**)malloc(1000000*sizeof(void*));
	while(!feof(f))	{
		fread((void*)&k, 1, sizeof(key), f);
		if(feof(f))	{
			break;
		}
		fn = bptree_fnode_create(pl, t);	
		nlist_prev[nkey] = fn;
		nkey++;
		if(prevfn)	{
			prevfn->next = fn;
		}
		prevfn = fn;
		bptree_fnode_insert(t, fn, &k, &tempv);
		printf("key=%d\n", k);
		fseek(f, RESTORE_INDEX_SPLITTER_REPCOUNT*sizeof(char), SEEK_CUR);
	}
	fclose(f);

	//create inner node
	//klist = (key*)malloc(nkey*sizeof(key));
	g = 0;	//inverse height
	nnode_prev = nkey;

	do	{
		nnode = 0;
		i=0;
		in = NULL;
		while(i<nnode_prev-1)	{
			if(in==NULL || in->len==t->order)	{
				in = bptree_inode_create(pl, t);
				nlist[nnode] = in;
				nnode++;
			}
			if(g==0)	{
				children = bptree_inode_insert(t, in, &((bptree_fnode*)nlist_prev[i])->keys[((bptree_fnode*)nlist_prev[i])->len-1]);
				*(children-1) = nlist_prev[i];
			}	else	{
				children = bptree_inode_insert(t, in, &((bptree_inode*)nlist_prev[i])->keys[((bptree_inode*)nlist_prev[i])->len-1-1]);
				*(children-1) = nlist_prev[i];
			}
			i++;
		}

		in->children[in->len-1] = nlist_prev[i];

		for(i=0;i<nnode;i++)	{
			nlist_prev[i] = nlist[i];
		}


		nnode_prev = nnode;
		g++;
	}	while(nnode>1);

	if(nnode!=1)	{
		perror("snappy_restore: nnode!=1");
	}
	t->root = in;
	t->height = g;
}

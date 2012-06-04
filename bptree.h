#ifndef _BPTREE_BPTREE_
#define _BPTREE_BPTREE_

#include "common.h"
#include "page.h"

typedef int key;
typedef int value;

#define BPTREE_MAX_ORDER 20

/* ----- key op macros ---------*/
#define KEY_COMPARE(k1, k2) ((k1)-(k2))

/* ----- node op macros ---------*/
#define BPTREE_NODE_GETTYPE(pl, n) ( PL_GET_PAGETYPE(pl, ((void*)n-sizeof(page_head)) ) )
#define BPTREE_NODE_ISINNER(pl, n) ( BPTREE_NODE_GETTYPE(pl, n)==PAGE_TYPE_BPTREE_INNER )
#define BPTREE_NODE_ISLEAF(pl, n) ( BPTREE_NODE_GETTYPE(pl, n)==PAGE_TYPE_BPTREE_LEAF )
#define BPTREE_INODE_CAPACITY(t, in) ((t)->order-(in)->len)	
#define BPTREE_FNODE_CAPACITY(t, fn) ((t)->order-(fn)->len)


/* ----- btree def ---------*/
typedef struct _bptree	{
	int order;
	int height;
	void* root;
}bptree;

//intermediate node
typedef struct _bptree_inode	{
	key* keys;
	void** children;
	int len;	//count of children
}bptree_inode;

//leaf node
typedef struct _bptree_fnode	{
	key* keys;
	value* values;
	int len;
	struct _bptree_fnode* next;
}bptree_fnode;


/* ----- bptree ops ---------*/
bptree* bptree_create(int order);
int bptree_insert(pagelist* pl, bptree* t, key* k, value* v);

/* ----- bptree inode ops ---------*/
//return children pointer
bptree_inode* bptree_inode_create(pagelist* pl, bptree* t);
void* bptree_inode_search(bptree* t, bptree_inode* in, key* k, int* ikey);
void** bptree_inode_insert(bptree* t, bptree_inode* in, key* k);
key* bptree_inode_get_middle(bptree* t, bptree_inode* in, key* k);
bptree_inode* bptree_inode_split(pagelist* pl, bptree* t, bptree_inode* in);
bptree_inode* bptree_inode_insert_remove_split(pagelist* pl, bptree* t, bptree_inode* in, key* ins, key* del, void*** newchildren);
/*
bptree_inode* bptree_inode_split_at(pagelist* pl, bptree* t, bptree_inode* inparent, bptree_inode* inchildren);
*/

/* ----- bptree fnode ops ---------*/
bptree_fnode* bptree_fnode_create(pagelist* pl, bptree* t);
int bptree_fnode_insert(bptree* t, bptree_fnode* fn, key* k, value* v);
bptree_fnode* bptree_fnode_split(pagelist* pl, bptree* t, bptree_fnode* fn, key* k, value* v);
key* bptree_fnode_get_middle(bptree* t, bptree_fnode* fn, key* k);

/* ----- bptree utils ---------*/
void bptree_show(pagelist* pl, bptree* t);

#endif

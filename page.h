#ifndef _BPTREE_PAGE_
#define _BPTREE_PAGE_

#define PAGE_SIZE 32*1024	//byte
#define INIT_PAGELIST_SIZE	10240*5//pages


/*------------- page list ------------------------------*/
typedef struct _pagelist	{
	int size;
	void* head;
}pagelist;

#define PAGE_UNUSED 0
#define PAGE_USED 1
#define PAGE_TYPE_BPTREE_INNER 0
#define PAGE_TYPE_BPTREE_LEAF 1
typedef struct _page_head	{
	unsigned type:2;
	unsigned used:2;
	unsigned size:4;
}page_head;

//get ith(from 0 to list size-1) page
#define PL_GET_PAGE_AT(pl, i) (pl->head+i*PAGE_SIZE)
#define PL_IS_PAGE_UNUSED(p) (((page_head*)p)->used==PAGE_UNUSED)
#define PL_IS_PAGE_USED(p) (((page_head*)p)->used==PAGE_USED)
#define PL_SET_PAGE_USED(p) ((page_head*)p)->used=PAGE_USED
#define PL_GET_PAGETYPE(pl, p) ( (int)(((page_head*)p)->type) )

//init pagelist
pagelist* pagelist_create();

//get free page from pagelist
void* pagelist_get_free_page(pagelist* pl);
//larger than
void* pagelist_get_used_page_lt(pagelist* pl, int min);

#endif

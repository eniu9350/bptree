

#include <stdio.h>
#include <stdlib.h>

#include "page.h"

pagelist* pagelist_create()
{
	pagelist* pl;
	pl = (pagelist*)malloc(sizeof(pagelist));
	pl->size = INIT_PAGELIST_SIZE;
	pl->head = malloc(pl->size*PAGE_SIZE);
	return pl;
}


void* pagelist_get_free_page(pagelist* pl)
{
	int i;
	void* p;
	
	for(i=0;i<pl->size;i++)	{
		
		p = PL_GET_PAGE_AT(pl, i);
		if(PL_IS_PAGE_UNUSED(p))	{
			PL_SET_PAGE_USED(p);
			return p;
		}
	}

	return NULL;
}


void* pagelist_get_used_page_lt(pagelist* pl, int min)
{
	int i;
	void* p;
	page_head* phead;
	page_head* result;

	result = NULL;
	for(i=0;i<pl->size;i++)	{
		p = PL_GET_PAGE_AT(pl, i);
		if(PL_IS_PAGE_USED(p))	{
			phead = (page_head*)p;
			if(phead->size>min)	{
				if(!result)	{
					result = (page_head*)p;
				}
				else	{
					if(phead->size>result->size)	{
						result = phead;
					}
				}
			}
		}
	}

	return (void*)result;

}

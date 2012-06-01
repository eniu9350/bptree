#include "bptree.h"

#include <stdio.h>
#include <stdlib.h>

/* ----- bptree ops ---------*/
bptree* bptree_create(int order)
{
	bptree* t = (bptree*)malloc(sizeof(bptree));
	t->order = order;
	t->height = 0;
	t->root = NULL;
	return t;
}


/*
 * return: 0 if ok, -1 if exist
 */
int bptree_insert(pagelist* pl, bptree* t, key* k, value* v)
{
	bptree_fnode* root;	
	bptree_fnode* pleaf;
	bptree_inode* pinner;
	void** pleft;
	void** pright;
	void** ptrace;

	void** pnewchildren;
	void** tmp;
	bptree_inode* pnewsibling;

	key* toins;
	key* todel;

	bptree_inode* newroot;

//	int itop;

	void* children;
	int ikey;
	//void* _pnode;
	int i,j,l,m;
	int h;	//trace of height traveled
	if(!t->root)	{
		root = bptree_fnode_create(pl, t);
		t->root = root;
		t->height = 1;
		bptree_fnode_insert(t, root, k, v);
	} else	{
		h = 0;
		pleaf = NULL;

		//_pnode = t->root;
		ikey = 0;
		ptrace = (void**)malloc(t->height*sizeof(void*));
		h = 0;
		ptrace[0] = t->root;
		while(h<t->height-1)	{
			//SEARCH THROUGH NODE
			//if(BPTREE_NODE_ISINNER(pl, _pnode)==0)	{
			//pinner = ptrace[h];

			children = bptree_inode_search(t, ptrace[h], k, &ikey);
			if(children==NULL)	{	//key exist
				break;
			}	else	{
				//_pnode = children;
				h++;
				ptrace[h] = children;
			}

			//} else if(BPTREE_NODE_ISLEAF(pl, _pnode)==0)	{
			//}
		}

		if(h!=t->height-1)	{	//key exist already
			return -1;
		}	else	{
			pleaf = (bptree_fnode*)ptrace[t->height-1];
			if(BPTREE_FNODE_CAPACITY(t, pleaf)!=0)	{	//not full
				printf("\ninsert not full: %d\n", *k);
				bptree_fnode_insert(t, pleaf, k, v);
			} else	{	//full
				printf("\ninsert full: %d\n", *k);
				//itop = -1;
				for(i=0;i<t->height-1;i++)	{
					if(((bptree_inode*)ptrace[i])->len==t->order)	{	//the highest that is full
						break;
					}
				}

				//mmm:
				//mmmmmmmmm: wrong?
				toins = (key*)malloc(t->height*sizeof(key));
				todel = (key*)malloc(t->height*sizeof(key));

				//scan from bottom to top
				//mmmmmm
				todel[t->height-1] = *bptree_fnode_get_middle(t, ptrace[t->height-1], k);
				for(j=t->height-1-1;j>=i;j--)	{
					toins[j] = todel[j+1];	//mmm: should copy!
					todel[j] = *bptree_inode_get_middle(t, ptrace[j], &toins[j]);//mmm: should copy!
				}

				if(i==0)	{	//root is full
					//bptree_inode_inser
					newroot = bptree_inode_create(pl, t);
					newroot->keys[0] = todel[0];
					newroot->children[0] = t->root;
					newroot->len = 2;

					//mmm: if fnode!!!
					//					newroot->children[1] = bptree_fnode_split(pl, t, t->root, 

					pnewchildren = &newroot->children[1];

					t->root = newroot;
					//mmm: set height at last!!!
					//	t->height = t->height+1;
				}
				else	{
					//					pnewchildren = ptrace[i-1]->xxxxxx;	//this is an exception
					pnewchildren = bptree_inode_insert(t, (bptree_inode*)ptrace[i-1], &todel[i]);
				}

				//split from top to bottom
				for(j=i;j<t->height-1;j++)	{
					//mmm: not impl
					if(j==i)	{
					}	else	{
					}
					tmp = pnewchildren;
					*pnewchildren = (void*)bptree_inode_insert_remove_split(pl, t, ptrace[j], &toins[j], &todel[j], &tmp);
					pnewchildren = tmp;
					for(m=0;m<((bptree_inode*)ptrace[j])->len;m++)	{
						if(((bptree_inode*)ptrace[j])->children[m]==(void*)ptrace[j+1])	{
							break;
						}
					}
					for(l=0;l<((bptree_inode*)*pnewchildren)->len;l++)	{
						if(((bptree_inode*)*pnewchildren)->children[m]==(void*)ptrace[j+1])	{
							break;
						}
					}
					if(m!=((bptree_inode*)ptrace[j])->len)	{
						pnewchildren = &(((bptree_inode*)ptrace[j])->children[m]);
					} else if(l!=((bptree_inode*)*pnewchildren)->len)	{
						pnewchildren = &(((bptree_inode*)*pnewchildren)->children[m]);
					} else {	//not found
						perror("can not find new children pointer to be modified\n");
					}
				}

				//split leaf
				*pnewchildren = bptree_fnode_split(pl, t, pleaf, k, v);

				//increase height if root full
				if(i==0)	{
					t->height = t->height+1;
				}

			}	//full

		}	//key not exist
	}
	//	bptree_show(t);
}

/* ----- bptree inode ops ---------*/
bptree_inode* bptree_inode_create(pagelist* pl, bptree* t)
{
	void* p;
	void* _pnode;
	void* _pdata;
	int order = t->order;
	page_head* phead;
	bptree_inode* pnode;
	p = pagelist_get_free_page(pl);
	if(p)	{
		phead = (page_head*)p;
		phead->size = sizeof(bptree_inode) + (order-1)*sizeof(key) + order*sizeof(void*);
		phead->type = PAGE_TYPE_BPTREE_INNER;

		_pnode = (void*)phead+sizeof(page_head);
		_pdata = _pnode+sizeof(bptree_inode);

		pnode = (bptree_inode*)_pnode;

		pnode->keys = (key*)_pdata;
		pnode->children = (void**)(_pdata+(order-1)*sizeof(key));
		pnode->len = 0;

		return pnode;
	}
	else	{
		perror("get page failed in inode creation\n");
		return 0;
	}
}

void* bptree_inode_search(bptree* t, bptree_inode* in, key* k, int* ikey){
	int i;
	for(i=0;i<in->len-1;i++)	{	//key count = children count - 1
		if(KEY_COMPARE(*k, in->keys[i])==0)	{
			*ikey = i;
			return NULL;
		}	else if(KEY_COMPARE(*k, in->keys[i])<0)	{
			break;
		}
	}

	return in->children[i];
}

//ass. inode not full!
//return: new children
void** bptree_inode_insert(bptree* t, bptree_inode* in, key* k)
{
	int i,j;
	if(in->len==t->order)	{
		return NULL;
	}

	for(i=0;i<in->len-1;i++)	{
		if(KEY_COMPARE(*k, in->keys[i])<0)	{
			break;
		}
	}

	for(j=in->len-1-1;j>=i;j--)	{
		in->keys[j+1] = in->keys[j];
		in->children[j+1+1] = in->children[j+1];
	}

	in->keys[i] = *k;
	in->len = in->len+1;

	return &in->children[i+1];
}

bptree_inode* bptree_inode_split(pagelist* pl, bptree* t, bptree_inode* in)
{
	int med;	//0~med: original node, med+1~: new node
	int i;
	bptree_inode* innew = bptree_inode_create(pl, t);

	//	med = (t->order+1)/2-1;	//mmm:wrong?
	med = (in->len-1)/2;
	i=0;

	//copy some slots from orig to new node
	for(i=med+1;i<in->len;i++)	{
		innew->keys[i-med] = in->keys[i];
		innew->children[i-med] = in->children[i];
	}
	innew->len = (in->len-1)-(med+1)+1;

	//remove slots in original node
	in->len = med+1;

	return innew;
}
/*
	 bptree_inode* bptree_inode_split_at(pagelist* pl, bptree* t, bptree_inode* inparent, bptree_inode* inchildren)
	 {
	 int i;
	 bptree_inode* innew = bptree_inode_create(pl, t);

	 for(i=0;i<inparent->len;i++)	{
	 if(inparent->children[i] == inchildren)	{
	 break;
	 }
	 }

	 }
	 */
/*
 * return: 0 if ok, -1 if full, -2 if other.
 */
int bptree_fnode_insert(bptree* t, bptree_fnode* fn, key* k, value* v)
{
	int i,j;
	if(fn->len==t->order)	{
		return -1;
	}

	for(i=0;i<fn->len;i++)	{
		if(KEY_COMPARE(*k, fn->keys[i])<0)	{
			break;
		}
	}

	if(fn->len!=0&&i==fn->len&&t->root!=fn)	{
		return -2;
	}

	for(j=fn->len-1;j>=i;j--)	{
		fn->keys[j+1] = fn->keys[j];
		fn->values[j+1] = fn->values[j];
	}

	fn->keys[i] = *k;
	fn->values[i] = *v;
	fn->len = fn->len+1;

	return 0;
}

key* bptree_inode_get_middle(bptree* t, bptree_inode* in, key* k)
{
	int i;
	int mid;
	for(i=0;i<in->len-1;i++)	{
		if(KEY_COMPARE(*k, in->keys[i])<0)	{
			break;
		}
	}

	mid = (in->len-1)/2;

	if((in->len-1)%2)	{
		if(i<=mid)	{
			return in->keys+mid;
		} else	{
			return k;
		}
	} else	{
		if(i<=mid)	{
			return k;
		} else	{
			return in->keys+mid;
		}
	}
}

key* bptree_fnode_get_middle(bptree* t, bptree_fnode* fn, key* k)
{
	int i;
	int mid;
	for(i=0;i<fn->len;i++)	{
		if(KEY_COMPARE(*k, fn->keys[i])<0)	{
			break;
		}
	}

	mid = (fn->len)/2;

	if(fn->len%2)	{
		if(i<=mid)	{
			return fn->keys+mid;
		} else	{
			return k;
		}
	} else	{
		if(i<=mid)	{
			return k;
		} else	{
			return fn->keys+mid;
		}
	}
}

bptree_inode* bptree_inode_insert_remove_split(pagelist* pl, bptree* t, bptree_inode* in, key* ins, key* del, void*** newchildren)
{
	int inspos;		//index of the key which is at the left of the key to be inserted, -1 if the most left

	//left:1, right:2

	int delpos;
	int i,j;

	bptree_inode* innew;

	innew = bptree_inode_create(pl, t);
	//return the new inserted
	if(*ins==*del)	{
		//fill the right one
		innew->len = in->len-1-1-(delpos+1)+1+1;
		for(i=delpos+1;i<in->len-1;i++)	{
			innew->keys[i-(delpos+1)] = in->keys[i];
			innew->children[i-(delpos+1)] = in->children[i];
		}
		innew->children[innew->len-1] = in->children[in->len-1];

		//fill the left one(decrease length only)
		in->len = delpos-1+1+1;
	} else	{
		//get ins position
		for(i=0;i<in->len-1;i++)	{
			if(KEY_COMPARE(*ins, in->keys[i])<0)	{
				inspos = i-1;
				break;
			}
		}
		//get del position
		if(*del==*ins)	{
			i=inspos;
			delpos = inspos;
		} else if(*del<*ins)	{
			for(i=0;i<inspos;i++)	{
				if(KEY_COMPARE(in->keys[i], *del)==0)	{
					delpos = i;
					break;
				}
			}
		}	else	{
			for(i=inspos;i<in->len-1;i++)	{
				if(KEY_COMPARE(in->keys[i], *del)==0)	{
					delpos = i+1;
					break;
				}
			}
		}

		if(i==in->len-1)	{
			perror("bptree_inode_insert_remove_failure: del key not found!\n");
		}	

		if(inspos<delpos)	{	//inserted at left
			//fill the right one
			innew->len = in->len-1-1-(delpos+1)+1+1;
			for(i=delpos+1;i<in->len-1;i++)	{
				innew->keys[i-(delpos+1)] = in->keys[i];
				innew->children[i-(delpos+1)] = in->children[i];
			}
			innew->children[innew->len-1] = in->children[in->len-1];

			//fill the left one(insert only)
			in->len = delpos-1+1+1+1;
			for(i=0;i<in->len-1;i++)	{
				if(KEY_COMPARE(*ins, in->keys[i])<0)	{
					break;
				}
			}
			for(j=in->len-1-1;j>=i;j++)	{
				in->keys[j+1] = in->keys[j];
				in->children[j+1] = in->children[j];
			}
			in->keys[i] = *ins;

			*newchildren = &in->children[i];
		}	else	{
			//fill the right one(copy and insert)
			innew->len = in->len-1-1-(delpos+1)+1+1+1;
			for(i=delpos+1;i<in->len-1;i++)	{
				innew->keys[i-(delpos+1)] = in->keys[i];
				innew->children[i-(delpos+1)] = in->children[i];
			}
			innew->children[innew->len-1] = in->children[in->len-1];

			for(i=0;i<innew->len-1;i++)	{
				if(KEY_COMPARE(*ins, innew->keys[i])<0)	{
					break;
				}
			}
			for(j=innew->len-1-1;j>=i;j++)	{
				innew->keys[j+1] = innew->keys[j];
				innew->children[j+1] = innew->children[j];
			}
			innew->keys[i] = *ins;

			*newchildren = &innew->children[i];

			//fill the left one(decrease length only)
			in->len = delpos-1+1+1;
		}
	}

	return innew;
}

/* ----- bptree fnode ops ---------*/
bptree_fnode* bptree_fnode_create(pagelist* pl, bptree* t)
{
	void* p;
	void* _pnode;
	void* _pdata;
	int order = t->order;
	page_head* phead;
	bptree_fnode* pnode;
	p = pagelist_get_free_page(pl);
	if(p)	{
		phead = (page_head*)p;
		phead->size = sizeof(bptree_fnode) + (order-1)*sizeof(key) + order*sizeof(value);
		phead->type = PAGE_TYPE_BPTREE_LEAF;

		_pnode = (void*)phead+sizeof(page_head);
		_pdata = _pnode+sizeof(bptree_fnode);

		pnode = (bptree_fnode*)_pnode;

		pnode->keys = (key*)_pdata;
		pnode->values = (value*)(_pdata+(order-1)*sizeof(key));
		pnode->len = 0;

		return pnode;
	}
	else	{
		perror("get page failed in fnode creation\n");
		return 0;
	}
}

bptree_fnode* bptree_fnode_split(pagelist* pl, bptree* t, bptree_fnode* fn, key* k, value* v)
{
	int med;	//0~med: original node, med+1~: new node
	int i,j;
	bptree_fnode* newfn;

	newfn = bptree_fnode_create(pl, t);

	for(i=0;i<fn->len;i++)	{
		if(KEY_COMPARE(*k, fn->keys[i])<0)	{
			break;
		}
	}

	med = fn->len/2;
	if(i<=med)	{	//insert before med and let the right be [mid,~
		for(j=med;j<fn->len;j++)	{
			newfn->keys[j-med] = fn->keys[j];
			newfn->values[j-med] = fn->values[j];
		}
		newfn->len = fn->len-1-med+1;

		for(j=med-1;j>=i;j--)	{
			fn->keys[j+1] = fn->keys[j];
			fn->values[j+1] = fn->values[j];
		}
		fn->keys[i] = *k;
		fn->values[i] = *v;
		fn->len = med+1;
	} else	{//insert after mid and let the right be [mid+1(or k), ~
		for(j=med+1;j<=i-1;j++)	{
			newfn->keys[j-med-1] = fn->keys[j-med-1];
			newfn->values[j-med-1] = fn->values[j-med-1];
		}
		for(j=fn->len-1;j>=i;j--)	{
			newfn->keys[j-med] = fn->keys[j];
			newfn->values[j-med] = fn->values[j];
		}
		newfn->keys[i-med-1] = *k;
		newfn->values[i-med-1] = *v;

		newfn->len = fn->len-1-(med+1)+1+1;

		fn->len = med+1;
	}


	//remove slots in original node
	return newfn;
}

/* ----- bptree utils ---------*/
void bptree_show(bptree* t)
{
	int i,j;
	bptree_inode* in = (bptree_inode*)t->root;
	bptree_fnode* fn;
	printf("---start---\n");
	for(i=0;i<in->len-1;i++)	{
		printf("%d ", in->keys[i]);
	}
	printf("\n");
	for(i=0;i<in->len;i++)	{
		fn = (bptree_fnode*)in->children[i];
		for(j=0;j<fn->len;j++)	{
			printf("(l)%d ", fn->keys[j]);
		}
		printf("|");
	}
	printf("\n---end---\n\n");
	/*
		 int h;
		 h = 0;

		 while(h<t->height)	{

		 h++;
		 }
		 */
}
/*
	 static void bptree_show_inode(bptree* t, bptree_inode* in, int h)	{
	 int i;
	 void* children;
	 for(i=0;i<in->len;i++)	{
	 children = in->children[i];



	 }
	 }

	 static void bptree_show_fnode(bptree* t, bptree_fnode* in)	{
	 int i;
	 printf("[ ");
	 for(i=0;i<fn->len;i++)	{
	 printf("%d ", fn->keys[i]);
	 }
	 printf("] ");
	 }

*/

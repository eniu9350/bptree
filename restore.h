#ifndef _BPTREE_RESTORE_
#define _BPTREE_RESTORE_

#include "bptree.h"
#include "page.h"

#define RESTORE_INDEX_SPLITTER ' '
#define RESTORE_INDEX_SPLITTER_REPCOUNT 4 

//-----snappy------------
void backup_snappy(pagelist* pl, bptree* t, char* fnindex, char* fndata);
bptree* restore_snappy(int order, char* fnindex, char* fndata);

#endif

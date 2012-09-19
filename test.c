#include <stdio.h>
#include <gsl_rng.h>
#include <gsl_randist.h>
#include <time.h>


#include "bptree.h"
#include "restore.h"


int main()
{

	key min = 1;
	key max;// = 1000;
	key keys[10] = {2,3,5,8,16,23,26,35,48,50};
	int i;
	
	bptree* t;
	pagelist* pl;
	pl = pagelist_create();



	int nuser = 1*1024-1;	//1024*1024*64-1;
	double mean_ia = 205;

	
	gsl_rng *r;
		const gsl_rng_type *T;
		int n=5;
		double u;
			
		T=gsl_rng_ranlxs0;		//设随机数生成器类型是 ranlxs0
		
		
		//gen arrival
		gsl_rng_default_seed = ((unsigned long)(time(NULL))); 	 //设seed值为当前时间
		r=gsl_rng_alloc(T); 	 //生成实例
		
		double* exp_sample_ir = MALLOC(nuser, double);
		double abstemp = 0;
		for(i=0;i<nuser;i++)	{
				exp_sample_ir[i] = gsl_ran_exponential(r, mean_ia);
				//exp_sample_ir[i] =  2+(i%10000)*0.3;
#ifdef LOGISTIC				
				abstemp = gsl_ran_logistic(r, 1);
				if(abstemp<0)	{
					abstemp=0-abstemp;
				}
				exp_sample_ir[i] = abstemp;				
#endif	
				//exp_sample_ir[i] = 5*gsl_ran_beta(r, 5, 1);
				//exp_sample_ir[i] = 5*gsl_ran_lognormal(r, 5, 0.25);
				//printf("exp: %f\n", exp_sample_ir[i]);
		}
		
		
		double* arrival_real	= MALLOC(nuser, double);
		arrival_real[0] = 1.0;
		for(i=1;i<nuser;i++)	{
			arrival_real[i] = arrival_real[i-1]+exp_sample_ir[i-1];
			//printf("arrival_real: %f\n", arrival_real[i]);
		}

		key* arrival	= MALLOC(nuser, key);
		for(i=0;i<nuser;i++)	{
					arrival[i] = (key)arrival_real[i];
					//printf("arrival: %ld\n", arrival[i]);
		}


		max = 0;
	for(i=0;i<nuser;i++)	{
		
		if(arrival[i]>max)	{
			max = arrival[i];
		}
		
	}

	printf("---max=%ld\n", max);

	t = bptree_create(30);
	int v = 0;
	for(i=0;i<nuser;i++)	{
		bptree_insert(pl, t, arrival+i, &v);
		//printf("insert %ld, height=%d\n", arrival[i], t->h);
	}
	printf("height=%d\n", t->height);


}

int main3()
{
	pagelist* pl;
	bptree* t;
	bptree* newt;
	bptree_inode* in;
	bptree_fnode* fn;
	int k,v;
	int i;
	char c;
	//void* p;
	pl = pagelist_create();
	t = bptree_create(30);
	
	//printf("sizeof IS %d\n", sizeof(RESTORE_INDEX_SPLITTER));
	//c = ' ';
	//printf("sizeof IS %d\n", sizeof(c));

	for(i=0;i<100000;i++)	{
	//for(i=1000;i>=0;i--)	{
		k = i+1;
		v = 42666;
		bptree_insert(pl, t, &k, &v);
	}
	//bptree_show(pl, t);


	backup_snappy(pl, t, "./index", "./data");
	newt = restore_snappy(pl, 30, "./index", "./data");
	bptree_show(pl, newt);
	
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

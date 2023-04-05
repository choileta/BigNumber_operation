#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__ )

#include "test.h"
#include "Altair_Calculation.h"
#include "Altair_Basic_Operations.h"
#include "Altair_ECC.h"

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int main()
{
	//srand(time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 101;

	//ECC Test
#if 1
	bigint* p = NULL;
	bi_new(&p, 8);
	p->a[0] = 0xffffffff;	p->a[1] = 0xffffffff, p->a[2] = 0xffffffff, p->a[3] = 0x00000000;
	p->a[4] = 0x00000000;	p->a[5] = 0x00000000, p->a[6] = 0x00000001, p->a[7] = 0xffffffff;

	ECC_bigint* P = NULL;
	ECC_new(&P, SIZE);
	P->x->a[7] = 0x6b17d1f2; P->x->a[6] = 0xe12c4247; P->x->a[5] = 0xf8bce6e5; P->x->a[4] = 0x63a440f2;
	P->x->a[3] = 0x77037d81; P->x->a[2] = 0x2deb33a0; P->x->a[1] = 0xf4a13945; P->x->a[0] = 0xd898c296;

	P->y->a[7] = 0x4fe342e2; P->y->a[6] = 0xfe1a7f9b; P->y->a[5] = 0x8ee7eb4a; P->y->a[4] = 0x7c0f9e16;
	P->y->a[3] = 0x2bce3357; P->y->a[2] = 0x6b315ece; P->y->a[1] = 0xcbb64068; P->y->a[0] = 0x37bf51f5;
	P->flag = NOT_INFINITY_POINT;

	//addFpTest(p);
	//subFpTest(p);
	//fastReductionTest(p);
	//inversionTest(p);
	//ECCL2RTest(p, P);
	//ECCJacobianL2R(p, P);
	ECCwNAFSMTest(p, P);

	bi_delete(&p);
#endif

	//Big Number Test
#if 0
	//addTest();							
	//subTest();
	//mulTest();
	//mulKaratsubaTest();			//테스트 케이스가 512bit, word 8bit에서 false 1개 생김
	//squTest();
	//squKaratsubaTest();
	//divBinaryTest();
	//divTest();
#endif

#if 0
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}

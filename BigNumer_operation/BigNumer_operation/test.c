#include "test.h"
#include "Altair_Calculation.h"
#include "Altair_Basic_Operations.h"
#include "Altair_ECC.h"

void addTest()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&B, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, (TEST_BITLEN / WORD_BITLEN) + 1);

		bi_gen_rand(&A, 0, A->wordLen);
		bi_gen_rand(&B, 0, B->wordLen);
		bi_ADD(A, B, &C);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);
		printf("C = 0x");
		bi_show_hex(C);
		printf("print(A+B == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void subTest()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&B, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, TEST_BITLEN / WORD_BITLEN);
		
		bi_gen_rand(&A, 0, A->wordLen);
		bi_gen_rand(&B, 0, B->wordLen);
		bi_SUB(A, B, &C);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);
		if(C->sign == NEGATIVE)
			printf("C = -0x");
		else
			printf("C = 0x");
		bi_show_hex(C);
		printf("print(A-B == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void mulTest()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&B, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, 2 * (TEST_BITLEN / WORD_BITLEN));

		bi_gen_rand(&A, 0, A->wordLen);
		bi_gen_rand(&B, 0, B->wordLen);
		bi_MUL(A, B, &C);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);
		printf("C = 0x");
		bi_show_hex(C);
		printf("print(A*B == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void mulKaratsubaTest()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&B, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, 2 * (TEST_BITLEN / WORD_BITLEN));

		bi_gen_rand(&A, 0, A->wordLen);
		bi_gen_rand(&B, 0, B->wordLen);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);

		bi_MUL_Karatsuba(A, B, &C);

		printf("C = 0x");
		bi_show_hex(C);
		printf("print(A*B == C)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void mulKaratsubaSimpleTest()
{
	//A = 0x5C7FA0815BF2DA465810DCE5751D7494B0BFFAAF4ED95EB88CA8718D821E72EA5A7DDB21B1366B174B027680FC406A5ED590A4B58DAF749231946D3D0ECAF0BCDC0EF98B2E2C2B98181519832143D6166B50BE011FD384D81D7D7E51E2732FB8BBEC14B62B0DB2429802DEE53C604E3449B95F895D7F2803271CBDBF5950C757
	//B = 0xB1EEBF7DB0818E85363CFC219FFA4E5CA07AD5EDB837F82BD984804E55ED26EA0DCC59A8D72953452FD65DCF20CA601C73BBAE8D18011104D4A914212AD68FC38D5F77BCC6A4E6D5A2B26804DF9A87EB570D98936C3AC569B5EB5D77E9DC5AE50A5D32B2D52AE1AD688835B933A35B402326A9F70B1AACD255027449E9361DCB
}
void squTest()
{
	bigint* A = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, (2 * (TEST_BITLEN / WORD_BITLEN) + 1));

		bi_gen_rand(&A, 0, A->wordLen);
		bi_SQU(A, &C);

		printf("A = 0x");
		bi_show_hex(A);
		printf("C = 0x");
		bi_show_hex(C);
		printf("print(A^2 == C)\n");
	}
	bi_delete(&A);
	bi_delete(&C);
}
void squKaratsubaTest()
{
	bigint* A = NULL;
	bigint* C = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, TEST_BITLEN / WORD_BITLEN);
		bi_new(&C, (2 * (TEST_BITLEN / WORD_BITLEN)) + 1);

		bi_gen_rand(&A, 0, A->wordLen);
		bi_SQU_Karatsuba(A, &C);
		
		printf("A = 0x");
		bi_show_hex(A);
		printf("C = 0x");
		bi_show_hex(C);
		printf("print(A^2 == C)\n");
	}
	bi_delete(&A);
	bi_delete(&C);
}
void divBinaryTest()
{
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;
	int i;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, (TEST_BITLEN / WORD_BITLEN) * A_BIG_THAN_B);
		bi_new(&B, (TEST_BITLEN / WORD_BITLEN));
		bi_new(&Q, (TEST_BITLEN / WORD_BITLEN) * A_BIG_THAN_B);
		bi_new(&R, (TEST_BITLEN / WORD_BITLEN));

		bi_gen_rand(&A, NON_NEGATIVE, A->wordLen);
		bi_gen_rand(&B, NON_NEGATIVE, B->wordLen);

		bi_DIV_BINARY(A, B, &Q, &R);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);
		printf("Q = 0x");
		bi_show_hex(Q);
		printf("R = 0x");
		bi_show_hex(R);
		printf("print(A==B*Q+R)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&Q);
	bi_delete(&R);
}


word bi_2word_LDA(word A1, word A0, word B) {
	word Q = 0;
	word R = A1;
	for (int j = WORD_BITLEN - 1; j >= 0; j--) {
		if (R > ((long long)1 << (WORD_BITLEN - 1)) || R == ((long long)1 << (WORD_BITLEN - 1))) {
			Q = Q + ((long long)1 << j);
			R = (R << 1) + ((A0 >> j) & 0x1) - B;
		}
		else
		{
			R = (R << 1) + ((A0 >> j) & 0x1);
			if (R > B || R == B) {
				Q = Q + ((long long)1 << j);
				R = R - B;
			}
		}
	}
	return Q;
}
void wordDivEZTest()
{
	/*
	A = 0x3ec
	B = 0x99
	Q = 0x67
	*/
	word A[2] = { 0xec,0x3 }, B = 0x99, Q = 0;
	bi_2WORD_DIV_1WORD(A, B, &Q);
	printf("A = 0x%x%x\n", A[1], A[0]);
	printf("B = 0x%x\n", B);
	printf("Q = 0x%x\n", Q);
}

void wordDivTest()
{
	int i;
	word A[2] = { 0, }, B = 0, Q = 0;
	for (i = 0; i < TEST_NUM; i++)
	{
		array_rand(A, 2);
		array_rand(&B, 1);
		while ((!(A[1] < B))||(!(B>>(WORD_BITLEN-1))))
		{
			array_rand(A, 2);
			array_rand(&B, 1);
		}
		Q = 0;
		bi_2WORD_DIV_1WORD(A, B, &Q);
#if WORD_BITLEN == 64
		printf("A = 0x%llx%016llx\n", A[1], A[0]);
		printf("B = 0x%llx\n", B);
		printf("Q = 0x%llx\n", Q);
#elif WORD_BITLEN == 32
		printf("A = 0x%x%08x\n", A[1], A[0]);
		printf("B = 0x%x\n", B);
		printf("Q = 0x%x\n", Q);
#else
		printf("A = 0x%x%02x\n", A[1], A[0]);
		printf("B = 0x%x\n", B);
		printf("Q = 0x%x\n", Q);
#endif
		printf("print(A//B == Q)\n");
	}
}
void divEZTest()
{
	/*
	A = 0x83F6306994EC129402ECC5C0AD516808F2F83FD11419C9BFED6704FAD469FEA1
	B = 0xCAFDCE38314FDA58A1DDBDE3419563AF
	*/
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;
	int i;
	bi_new(&A, 32);
	bi_new(&B, 16);
	bi_new(&Q, 33);
	bi_new(&R, 17);
	A->a[31] = 0x83; A->a[30] = 0xF6; A->a[29] = 0x30; A->a[28] = 0x69; A->a[27] = 0x94; A->a[26] = 0xEC;
	A->a[25] = 0x12; A->a[24] = 0x94; A->a[23] = 0x02; A->a[22] = 0xEC; A->a[21] = 0xC5; A->a[20] = 0xC0;
	A->a[19] = 0xAD; A->a[18] = 0x51; A->a[17] = 0x68; A->a[16] = 0x08; A->a[15] = 0xF2; A->a[14] = 0xF8;
	A->a[13] = 0x3F; A->a[12] = 0xD1; A->a[11] = 0x14; A->a[10] = 0x19; A->a[9] = 0xC9; A->a[8] = 0xBF;
	A->a[7] = 0xED; A->a[6] = 0x67; A->a[5] = 0x04; A->a[4] = 0xFA; A->a[3] = 0xD4; A->a[2] = 0x69;
	A->a[1] = 0xFE; A->a[0] = 0xA1;

	B->a[15] = 0xCA; B->a[14] = 0xFD; B->a[13] = 0xCE; B->a[12] = 0x38; B->a[11] = 0x31; B->a[10] = 0x4F;
	B->a[9] = 0xDA; B->a[8] = 0x58; B->a[7] = 0xA1; B->a[6] = 0xDD; B->a[5] = 0xBD; B->a[4] = 0xE3;
	B->a[3] = 0x41; B->a[2] = 0x95; B->a[1] = 0x63; B->a[0] = 0xAF;;

	bi_DIV(A, B, &Q, &R);

	printf("A = 0x");
	bi_show_hex(A);
	printf("B = 0x");
	bi_show_hex(B);
	printf("Q = 0x");
	bi_show_hex(Q);
	printf("R = 0x");
	bi_show_hex(R);
	printf("print(A == B*Q+R)\n");

	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&Q);
	bi_delete(&R);
}
void divTest()
{
	int i;
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;

	for (i = 0; i < TEST_NUM; i++)
	{
		bi_new(&A, 2 * (TEST_BITLEN / WORD_BITLEN));
		bi_new(&B, TEST_BITLEN / WORD_BITLEN);
		bi_new(&Q, 2 * (TEST_BITLEN / WORD_BITLEN));
		bi_new(&R, (2 * (TEST_BITLEN / WORD_BITLEN)));

		bi_gen_rand(&A, NON_NEGATIVE, A->wordLen);
		bi_gen_rand(&B, NON_NEGATIVE, B->wordLen);

		printf("A = 0x");
		bi_show_hex(A);
		printf("B = 0x");
		bi_show_hex(B);

		bi_DIV(A, B, &Q, &R);

		printf("Q = 0x");
		bi_show_hex(Q);
		printf("R = 0x");
		bi_show_hex(R);
		printf("print(A == B*Q+R)\n");
	}
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&Q);
	bi_delete(&R);
}

////////////////
void addFpTest(bigint* p)
{
	FILE* fp1, * fp2, * fp3;
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_opA.txt", "r");
	fp2 = fopen("TV_opB.txt", "r");
	fp3 = fopen("ret_PFADD.txt", "w");

	printf("addFpTest StarT\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&A, ECC_WORD_NUM);
		bi_new(&B, ECC_WORD_NUM);
		bi_new(&C, ECC_WORD_NUM + 1);

		readData(A->a, fp1);
		readData(B->a, fp2);

		ADD_FP(A, B, p, &C);
		
		for (i = ECC_WORD_NUM - 1; i >= 0; i--)
#if   WORD_BITLEN == 64
			fprintf(fp3, "%016llX", C->a[i]);
#elif WORD_BITLEN == 32
			fprintf(fp3, "%08X", C->a[i]);
#else
			fprintf(fp3, "%02X", C->a[i]);
#endif
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
		fgets(buf, sizeof(buf), fp2);
	}
	printf("addFpTest End\n");
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void subFpTest(bigint* p)
{
	FILE* fp1, * fp2, * fp3;
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_opA.txt", "r");
	fp2 = fopen("TV_opB.txt", "r");
	fp3 = fopen("ret_PFSUB.txt", "w");

	printf("subFpTest StarT\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&A, ECC_WORD_NUM);
		bi_new(&B, ECC_WORD_NUM);
		bi_new(&C, ECC_WORD_NUM + 1);

		readData(A->a, fp1);
		readData(B->a, fp2);

		SUB_FP(A, B, p, &C);

		for (i = ECC_WORD_NUM - 1; i >= 0; i--)
#if   WORD_BITLEN == 64
			fprintf(fp3, "%016llX", C->a[i]);
#elif WORD_BITLEN == 32
			fprintf(fp3, "%08X", C->a[i]);
#else
			fprintf(fp3, "%02X", C->a[i]);
#endif
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
		fgets(buf, sizeof(buf), fp2);
	}
	printf("subFpTest End\n");
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
}
void fastReductionTest(bigint* p)
{
	FILE* fp1, * fp2, * fp3, * fp4;
	bigint* A = NULL;
	bigint* B = NULL;
	bigint* C = NULL;
	bigint* C2 = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_opA_PF.txt", "r");
	fp2 = fopen("TV_opB_PF.txt", "r");
	fp3 = fopen("ret_PFMUL.txt", "w");

	printf("fastReductionTest Start\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&A, ECC_WORD_NUM);
		bi_new(&B, ECC_WORD_NUM);
		bi_new(&C, (2 * ECC_WORD_NUM));
		bi_new(&C2, ECC_WORD_NUM);

		readData(A->a, fp1);
		readData(B->a, fp2);

		MUL_OS(A, B, &C);
		FastReduction(C, p, &C2);

		for (i = ECC_WORD_NUM - 1; i >= 0; i--)
#if   WORD_BITLEN == 64
			fprintf(fp3, "%016llX", C2->a[i]);
#elif WORD_BITLEN == 32
			fprintf(fp3, "%08X", C2->a[i]);
#else
			fprintf(fp3, "%02X", C2->a[i]);
#endif
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
		fgets(buf, sizeof(buf), fp2);
	}
	printf("fastReductionTest End\n");
	bi_delete(&A);
	bi_delete(&B);
	bi_delete(&C);
	bi_delete(&C2);
}
void inversionTest(bigint* p)
{
	FILE* fp1, * fp3;
	bigint* A = NULL;
	bigint* invA = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_opA_inv.txt", "r");
	fp3 = fopen("ret_PFINV.txt", "w");

	printf("inversionFermatLT Test Start\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&A, ECC_WORD_NUM);
		bi_new(&invA, ECC_WORD_NUM);
		
		readData(A->a, fp1);

		//INVERSION_FermatLT(A, p, &invA);
		INVERSION_Binary(A, p, &invA);

		for (i = ECC_WORD_NUM - 1; i >= 0; i--)
#if   WORD_BITLEN == 64
			fprintf(fp3, "%016llX", invA->a[i]);
#elif WORD_BITLEN == 32
			fprintf(fp3, "%08X", invA->a[i]);
#else
			fprintf(fp3, "%02X", invA->a[i]);
#endif
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
	}
	printf("inversionFermatLT Test End\n");
	bi_delete(&A);
	bi_delete(&invA);
}
void ECCL2RTest(bigint* p, ECC_bigint* P)
{
	FILE* fp1, * fp3;
	bigint* k = NULL;
	ECC_bigint* kP = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_Scalar.txt", "r");
	fp3 = fopen("ret_SM.txt", "w");

	printf("ECC SM Test Start\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&k, SIZE);
		ECC_new(&kP, SIZE);

		readData(k->a, fp1);

		//ECC_MOD_EXP_L2R(k, P, p, &kP);
		ECC_MOD_EXP_R2L(k, P, p, &kP);

		for (i = SIZE - 1; i >= 0; i--)
			fprintf(fp3, "%08X", kP->x->a[i]);
		fprintf(fp3, "\n");
		for (i = SIZE - 1; i >= 0; i--)
			fprintf(fp3, "%08X", kP->y->a[i]);
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
	}
	printf("ECC SM Test End\n");
	bi_delete(&k);
	ECC_delete(&kP);
}
void ECCJacobianL2RTest(bigint* p, ECC_bigint* P)
{
	FILE* fp1, * fp3;
	bigint* k = NULL;
	ECC_bigint* kP = NULL;
	u8 buf[100];
	int i, j;
	fp1 = fopen("TV_Scalar.txt", "r");
	fp3 = fopen("ret_SM.txt", "w");

	printf("ECC Jacobian L2R Test Start\n");
	for (j = 0; j < 10000; j++)
	{
		bi_new(&k, SIZE);
		ECC_new(&kP, SIZE);

		readData(k->a, fp1);

		ECC_Jacobian_MOD_EXP_L2R(k, P, p, &kP);

		for (i = SIZE - 1; i >= 0; i--)
			fprintf(fp3, "%08X", kP->x->a[i]);
		fprintf(fp3, "\n");
		for (i = SIZE - 1; i >= 0; i--)
			fprintf(fp3, "%08X", kP->y->a[i]);
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
	}
	printf("ECC Jacobian L2R Test End\n");
	bi_delete(&k);
	ECC_delete(&kP);
}
void ECCwNAFSMTest(bigint* p, ECC_bigint* P)
{
	char wNAFk[257] = { 0, };						//wNAFk
	FILE* fp1, * fp3;
	bigint* k = NULL;
	ECC_bigint* kP = NULL;
	ECC_bigint** table = NULL;
	u8 buf[100];
	int i, j;
	int w = 0, wNum = 0;

	fp1 = fopen("TV_Scalar.txt", "r");
	fp3 = fopen("ret_wNAFSM.txt", "w");

	printf("원하는 w를 입력하시오 : ");
	scanf("%d", &w);
	
	wNum = (int)pow(2, (double)w - 2);
	table = (ECC_bigint**)malloc(sizeof(ECC_bigint*) * wNum);
	if (table == NULL)
		return;
	for (i = 0; i < wNum; i++)
	{
		ECC_new(&table[i], SIZE);
	}

	ECC_wNAF_makeTable(P, w, p, table);

	for (i = 0; i < wNum; i++)
	{
		printf("%08X\n", table[i]->x->a[0]);
		printf("%08X\n", table[i]->y->a[0]);
		printf("\n");
	}

	for (i = 0; i < 100; i++)
	{
		bi_new(&k, SIZE);
		ECC_new(&kP, SIZE);

		readData(k, fp1);

		ECC_wNAF(k, w, wNAFk);
		ECC_wNAF_SCALAR_MUL(table, wNAFk, p, &kP);

		for (j = SIZE - 1; j >= 0; j--)
		{
			fprintf(fp3, "%08X", kP->x->a[j]);
		}
		fprintf(fp3, "\n");
		for (j = SIZE - 1; j >= 0; j--)
		{
			fprintf(fp3, "%08X", kP->y->a[j]);
		}
		fprintf(fp3, "\n\n");

		fgets(buf, sizeof(buf), fp1);
	}
	for (i = 0; i < wNum; i++)
		ECC_delete(&table[i]);
	free(table);
	bi_delete(&k);
	ECC_delete(&kP);
}
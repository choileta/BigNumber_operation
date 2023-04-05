#include "Altair_ECC.h"
#include "Altair_Basic_Operations.h"
#include "Altair_Calculation.h"

int div2(IN bigint** A)
{
	int i;
	for (i = 0; i < SIZE - 1; i++)
		(*A)->a[i] = ((((*A)->a[i + 1] & 0x1) << 31) | ((*A)->a[i] >> 1));
	(*A)->a[i] >>= 1;
}
int divN(IN bigint** A, IN int num)
{
	int i, cnt = (1 << num) - 1;
	for (i = 0; i < SIZE - 1; i++)
		(*A)->a[i] = ((((*A)->a[i + 1] & ((1 << num) - 1)) << (32 - num)) | ((*A)->a[i] >> num));
	(*A)->a[i] >>= num;
}
int cmpData(word* A, word* B)
{
	int i;
	for (i = SIZE - 1; i >= 0; i--)
	{
		if (A[i] > B[i])
			return 1;
		else if (A[i] < B[i])
			return 0;
	}
	return 1;
}
void ECC_Copy(ECC_bigint* A, ECC_bigint* B)
{
	bi_copy(A->x, B->x);
	bi_copy(A->y, B->y);
	bi_copy(A->z, B->z);
	A->flag = B->flag;
}
void ECC_new(ECC_bigint** A, int wordLen)
{
	*A = (ECC_bigint*)malloc(sizeof(ECC_bigint));
	if ((*A) == NULL)
		return;
	(*A)->flag = INFINITY_POINT;
	(*A)->x = NULL;
	(*A)->y = NULL;
	(*A)->z = NULL;
	bi_new(&(*A)->x, wordLen);
	bi_new(&(*A)->y, wordLen);
	bi_new(&(*A)->z, wordLen);
}
void ECC_delete(ECC_bigint** A)
{
	if (*A == NULL)
		return;
	bi_delete(&(*A)->x);
	bi_delete(&(*A)->y);
	bi_delete(&(*A)->z);
	free(*A);
	*A = NULL;
}

int ADD(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	int i, carry = 0;
	for (i = 0; i < SIZE; i++)
	{
		(*C)->a[i] = A->a[i] + B->a[i] + carry;

		if ((carry) && ((*C)->a[i] <= (A)->a[i]))
			carry = 1;
		else if ((*C)->a[i] < A->a[i])
			carry = 1;
		else
			carry = 0;
	}
	return carry;
}
int ADD_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C)
{
	if (ADD(A, B, C) || cmpData((*C)->a, p->a))
	{
		SUB(*C, p, C);
	}
}
//int SUB(IN bigint * A, IN bigint * B, OUT bigint * *C)
//{
//	int i, borrow = 0;
//	for (i = 0; i < SIZE; i++)
//	{
//		(*C)->a[i] = A->a[i] - B->a[i] - borrow;
//		if ((borrow) && (A->a[i] <= B->a[i]))
//			borrow = 1;
//		else if (A->a[i] < B->a[i])
//			borrow = 1;
//		else
//			borrow = 0;
//	}
//	return borrow;
//}
int SUB(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	int borrow = 0;   //borrow
	int i = 0;      //loop data
	word value = 0;

	for (i = 0; i < SIZE; i++) {
		(*C)->a[i] = A->a[i] - B->a[i] - borrow;
		if (A->a[i] < B->a[i] || ((A->a[i] == B->a[i]) && borrow == 1))
			borrow = 1;
		else
			borrow = 0;
	}
	return borrow;
}
int SUB_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C)
{
	if (SUB(A, B, C) || cmpData((*C)->a, p->a))
		ADD(p, *C, C);
}
int MUL_OS(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	u64 UV = 0;
	word U = 0;
	memset((*C)->a, 0x00000000, sizeof(word) * (2 * SIZE));
	int i, j;	// i : A의 index, j : B의 index
	for (i = 0; i < SIZE; i++)
	{
		U = 0x00000000;
		for (j = 0; j < SIZE; j++)
		{
			UV = (u64)(*C)->a[i + j] + ((u64)A->a[i] * (u64)B->a[j]) + (u64)U;

			(*C)->a[i + j] = (UV & 0xffffffff);
			U = (word)((UV >> 32));
		}
		(*C)->a[i + SIZE] = U;
	}
}
int MUL_PS(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	word R0 = 0, R1 = 0, R2 = 0, U = 0, V = 0, temp = 0;
	u64 UV = 0;
	int i, j, k, carry = 0;
	for (k = 0; k < (2 * SIZE - 1); k++)
	{
		i = 0; j = k;
		while (j > (SIZE - 1))
		{
			i++; j--;
		}
		while ((j >= 0) && (j <= (SIZE - 1)) && (i >= 0) && (i <= (SIZE - 1)))
		{
			carry = 0;

			UV = (u64)A->a[i] * (u64)B->a[j];
			V = (word)((UV & 0xffffffff));
			U = (word)((UV >> 32));

			R0 += V;
			if (R0 < V)
				carry = 1;
			else
				carry = 0;

			temp = R1 + U;

			if (temp < R1)
			{
				R1 = temp + carry;
				carry = 1;
			}
			else if ((temp == 0xffffffff) && (carry))
			{
				R1 = temp + carry;
				carry = 1;
			}
			else
			{
				R1 = temp + carry;
				carry = 0;
			}

			R2 = R2 + carry;

			i++;
			j--;
		}
		(*C)->a[k] = R0; R0 = R1; R1 = R2; R2 = 0;
	}
	(*C)->a[2 * SIZE - 1] = R0;
}
int FastReduction(IN bigint* A, IN bigint* p, OUT bigint** C)
{
	bigint* s1 = NULL;
	bigint* s2 = NULL;
	bigint* s3 = NULL;
	bigint* s4 = NULL;
	bigint* s5 = NULL;
	bigint* s6 = NULL;
	bigint* s7 = NULL;
	bigint* s8 = NULL;
	bigint* s9 = NULL;
	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* out = NULL;

	bi_new(&s1, SIZE);
	bi_new(&s2, SIZE);
	bi_new(&s3, SIZE);
	bi_new(&s4, SIZE);
	bi_new(&s5, SIZE);
	bi_new(&s6, SIZE);
	bi_new(&s7, SIZE);
	bi_new(&s8, SIZE);
	bi_new(&s9, SIZE);
	bi_new(&temp1, SIZE);
	bi_new(&temp2, SIZE);
	bi_new(&out, SIZE);
	s1->a[0] = A->a[0]; s1->a[1] = A->a[1]; s1->a[2] = A->a[2]; s1->a[3] = A->a[3];
	s1->a[4] = A->a[4]; s1->a[5] = A->a[5]; s1->a[6] = A->a[6]; s1->a[7] = A->a[7];

	s2->a[3] = A->a[11]; s2->a[4] = A->a[12]; s2->a[5] = A->a[13];
	s2->a[6] = A->a[14]; s2->a[7] = A->a[15];

	s3->a[3] = A->a[12]; s3->a[4] = A->a[13];
	s3->a[5] = A->a[14]; s3->a[6] = A->a[15];

	s4->a[0] = A->a[8]; s4->a[1] = A->a[9]; s4->a[2] = A->a[10];
	s4->a[6] = A->a[14]; s4->a[7] = A->a[15];

	s5->a[0] = A->a[9]; s5->a[1] = A->a[10]; s5->a[2] = A->a[11]; s5->a[3] = A->a[13];
	s5->a[4] = A->a[14]; s5->a[5] = A->a[15]; s5->a[6] = A->a[13]; s5->a[7] = A->a[8];

	s6->a[0] = A->a[11]; s6->a[1] = A->a[12]; s6->a[2] = A->a[13];
	s6->a[6] = A->a[8]; s6->a[7] = A->a[10];

	s7->a[0] = A->a[12]; s7->a[1] = A->a[13]; s7->a[2] = A->a[14];
	s7->a[3] = A->a[15]; s7->a[6] = A->a[9]; s7->a[7] = A->a[11];

	s8->a[0] = A->a[13]; s8->a[1] = A->a[14]; s8->a[2] = A->a[15]; s8->a[3] = A->a[8];
	s8->a[4] = A->a[9]; s8->a[5] = A->a[10]; s8->a[7] = A->a[12];

	s9->a[0] = A->a[14]; s9->a[1] = A->a[15]; s9->a[3] = A->a[9];
	s9->a[4] = A->a[10]; s9->a[5] = A->a[11]; s9->a[7] = A->a[13];

	ADD_FP(s2, s2, p, &temp1);
	ADD_FP(s3, s3, p, &temp2);
	ADD_FP(s1, temp1, p, &out);
	ADD_FP(out, temp2, p, &s1);
	ADD_FP(s1, s4, p, &s2);
	ADD_FP(s2, s5, p, &s3);
	SUB_FP(s3, s6, p, &s4);
	SUB_FP(s4, s7, p, &s5);
	SUB_FP(s5, s8, p, &s6);
	SUB_FP(s6, s9, p, C);

	bi_delete(&s1);
	bi_delete(&s2);
	bi_delete(&s3);
	bi_delete(&s4);
	bi_delete(&s5);
	bi_delete(&s6);
	bi_delete(&s7);
	bi_delete(&s8);
	bi_delete(&s9);
	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&out);
}
int INVERSION_FermatLT(IN bigint* A, IN bigint* p, OUT bigint* invA)
{
	bigint* z3 = NULL;
	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* t2 = NULL;
	bigint* t3 = NULL;
	bigint* temp = NULL;
	bigint* temp2 = NULL;
	int i;

	bi_new(&z3, 2 * SIZE);
	bi_new(&t0, 2 * SIZE);
	bi_new(&t1, 2 * SIZE);
	bi_new(&t2, 2 * SIZE);
	bi_new(&t3, 2 * SIZE);
	bi_new(&temp, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);

	MUL_PS(A, A, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, A, &temp);
	FastReduction(temp, p, &z3);

	MUL_PS(z3,z3, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2,temp2, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, z3, &temp);
	FastReduction(temp, p, &temp2);

	MUL_PS(temp2, temp2,&temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2,temp2, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, z3, &temp);
	FastReduction(temp, p, &t0);

	MUL_PS(t0,t0, &temp);
	FastReduction(temp, p, &temp2);
	for (i = 0; i < 5; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t0, &temp);
	FastReduction(temp, p, &temp2);
	memcpy(t1->a, temp2->a, sizeof(word) * SIZE);

	for (i = 0; i < 12; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t1, &temp);
	FastReduction(temp, p, &temp2);
	for (i = 0; i < 6; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t0, &temp);
	FastReduction(temp, p, &t2);

	MUL_PS(t2, t2,&temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, temp2,&temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, z3, &temp);
	FastReduction(temp, p, &temp2);
	memcpy(t3->a, temp2->a, sizeof(word) * SIZE);

	for (i = 0; i < 32; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, A, &temp);
	FastReduction(temp, p, &temp2);
	for (i = 0; i < 96; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}

	for (i = 0; i < 32; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t3, &temp);
	FastReduction(temp, p, &temp2);
	for (i = 0; i < 32; i++)
	{
		MUL_PS(temp2,temp2, &temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t3, &temp);
	FastReduction(temp, p, &temp2);

	for (i = 0; i < 30; i++)
	{
		MUL_PS(temp2, temp2,&temp);
		FastReduction(temp, p, &temp2);
	}
	MUL_PS(temp2, t2, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2,temp2, &temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, temp2,&temp);
	FastReduction(temp, p, &temp2);
	MUL_PS(temp2, A, &temp);
	FastReduction(temp, p, invA);

	bi_delete(&z3);
	bi_delete(&t0);
	bi_delete(&t1);
	bi_delete(&t2);
	bi_delete(&t3);
	bi_delete(&temp);
	bi_delete(&temp2);

	return operationSuccess;
}
int INVERSION_Binary(IN bigint* A, IN bigint* p, OUT bigint** invA)
{
	bigint* u = NULL;
	bigint* v = NULL;
	bigint* x1 = NULL;
	bigint* x2 = NULL;
	bigint* cmp = NULL;
	bigint* temp = NULL;
	bi_new(&u, SIZE);
	bi_new(&v, SIZE);
	bi_new(&x1, SIZE);
	bi_new(&x2, SIZE);
	bi_new(&cmp, SIZE);
	bi_new(&temp, SIZE);

	if (!(memcmp(A->a, u->a, sizeof(word) * SIZE)))
	{
		memset((*invA)->a, 0, sizeof(word) * SIZE);
		return operationSuccess;
	}
	int carry = 0;
	memcpy(u->a, A->a, sizeof(word) * SIZE);
	memcpy(v->a, p->a, sizeof(word) * SIZE);
	x1->a[0] = 1; cmp->a[0] = 1;

	while (memcmp(u->a, cmp->a, sizeof(word) * SIZE) && memcmp(v->a, cmp->a, sizeof(word) * SIZE))
	{
		while (!(u->a[0] & 0x1))
		{
			div2(&u);
			if (!(x1->a[0] & 0x1))
				div2(&x1);
			else
			{
				carry = ADD(x1, p, &temp);
				div2(&temp);
				memcpy(x1->a, temp->a, sizeof(word) * SIZE);
				x1->a[SIZE - 1] += (carry << 31);
			}
		}
		while (!(v->a[0] & 0x1))
		{
			div2(&v);
			if (!(x2->a[0] & 0x1))
				div2(&x2);
			else
			{
				carry = ADD(x2, p, &temp);
				div2(&temp);
				memcpy(x2->a, temp->a, sizeof(word) * SIZE);
				x2->a[SIZE - 1] += (carry << 31);
			}
		}
		if (cmpData(u->a, v->a))
		{
			SUB_FP(u, v, p, &temp);
			memcpy(u->a, temp->a, sizeof(word) * SIZE);
			SUB_FP(x1, x2, p, &temp);
			memcpy(x1->a, temp->a, sizeof(word) * SIZE);
		}
		else
		{
			SUB_FP(v, u, p, &temp);
			memcpy(v->a, temp->a, sizeof(word) * SIZE);
			SUB_FP(x2, x1, p, &temp);
			memcpy(x2->a, temp->a, sizeof(word) * SIZE);
		}
	}
	if (!memcmp(u->a, cmp->a, sizeof(word) * SIZE))
		memcpy((*invA)->a, x1->a, sizeof(word) * SIZE);
	else
		memcpy((*invA)->a, x2->a, sizeof(word) * SIZE);

	bi_delete(&u);
	bi_delete(&v);
	bi_delete(&x1);
	bi_delete(&x2);
	bi_delete(&cmp);
	bi_delete(&temp);

	return operationSuccess;
}
int ECC_ADD(IN ECC_bigint* P, IN ECC_bigint* Q, IN bigint* p, OUT ECC_bigint** R)
{
	if ((P->flag == 1) && (Q->flag == 0))
	{
		memcpy((*R)->x->a, Q->x->a, sizeof(word) * Q->x->wordLen);
		memcpy((*R)->y->a, Q->y->a, sizeof(word) * Q->y->wordLen);
		(*R)->flag = 0;
		return NOT_INFINITY_POINT;
	}
	else if ((P->flag == 0) && (Q->flag == 1))
	{
		memcpy((*R)->x->a, P->x->a, sizeof(word) * P->x->wordLen);
		memcpy((*R)->y->a, P->y->a, sizeof(word) * P->y->wordLen);
		(*R)->flag = 0;
		return NOT_INFINITY_POINT;
	}
	else if ((P->flag == 1) && (Q->flag == 1))
		return INFINITY_POINT;

	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* tempI = NULL;
	bigint* tempG = NULL;
	bigint* tempM = NULL;
	bigint* Rx = NULL;
	bigint* Ry = NULL;
	bi_new(&temp1, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);
	bi_new(&tempI, 2 * SIZE);
	bi_new(&tempG, 2 * SIZE);
	bi_new(&tempM, 2 * SIZE);
	bi_new(&Rx, SIZE);
	bi_new(&Ry, SIZE);

	SUB_FP(Q->y, P->y, p, &temp1);			//temp1 = y2-y1
	SUB_FP(Q->x, P->x, p, &temp2);			//temp2 = x2-x1
	INVERSION_Binary(temp2, p, &tempI);		//tempI = 1/(x2-x1)
	MUL_PS(temp1, tempI, &temp2);			//temp2 = (y2-y1)/(x2-x1)
	FastReduction(temp2, p, &tempG);		//tempG = (y2-y1)/(x2-x1)의 감산 후 결과
	MUL_PS(tempG, tempG, &tempM);			//tempM = ((y2-y1)/(x2-x1))^2
	FastReduction(tempM, p, &temp1);		//temp1 = ((y2-y1)/(x2-x1))^2 의 감산 후 결과
	SUB_FP(temp1, P->x, p, &temp2);
	SUB_FP(temp2, Q->x, p, &Rx);

	SUB_FP(P->x, Rx, p, &temp1);			//temp1 = x1-x3
	MUL_PS(tempG, temp1, &temp2);			//temp2 = tempG*(x1-x3)
	FastReduction(temp2, p, &temp1);		//temp1 = temp2의 감산
	SUB_FP(temp1, P->y, p, &(*R)->y);

	//memcpy((*R)->x->a, Rx->a, sizeof(word) * SIZE);
	bi_copy((*R)->x, Rx);

	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&tempI);
	bi_delete(&tempG);
	bi_delete(&tempM);
	bi_delete(&Rx);
	bi_delete(&Ry);

	return NOT_INFINITY_POINT;
}
int ECC_DBL(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R)
{
	if (P->flag == 1)
	{
		return INFINITY_POINT;
	}
	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* temp3 = NULL;
	bigint* tempG = NULL;
	bigint* a = NULL;
	bigint* Rx = NULL;
	bigint* Ry = NULL;
	bi_new(&temp1, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);
	bi_new(&temp3, 2 * SIZE);
	bi_new(&tempG, 2 * SIZE);
	bi_new(&a, 2 * SIZE);
	bi_new(&Rx, SIZE);
	bi_new(&Ry, SIZE);

	MUL_PS(P->x, P->x, &temp1);	//temp1 = x1^2
	FastReduction(temp1, p, &temp2);			//temp2 = x1^2의 감산
	ADD_FP(temp2, temp2, p, &temp3);		//temp3 = 2*x1^2
	ADD_FP(temp3, temp2, p, &temp1);		//temp1 = 3*x1^2
	memset(temp2->a, 0, sizeof(word) * SIZE);
	temp2->a[0] = 3;
	SUB_FP(p, temp2, p, &a);			//a = -3 = p-3
	ADD_FP(temp1, a, p, &temp2);			//temp2 = 3*x1^2+a
	ADD_FP(P->y, P->y, p, &temp1);		//temp1 = 2*y1
	INVERSION_Binary(temp1, p, &temp3);		//temp3 = 1/(2*y1)
	MUL_PS(temp2, temp3, &temp1);	//temp1 = ((3*x1^2+a)/(2*y1))
	FastReduction(temp1, p, &tempG);
	MUL_PS(tempG, tempG, &temp1);
	FastReduction(temp1, p, &temp2);			//temp2 = ((3*x1^2+a)/(2*y1))^2의 감산
	ADD_FP(P->x, P->x, p, &temp1);
	SUB_FP(temp2, temp1, p, &Rx);

	SUB_FP(P->x, Rx, p, &temp1);
	MUL_PS(tempG, temp1, &temp2);
	FastReduction(temp2, p, &temp1);
	SUB_FP(temp1, P->y, p, &(*R)->y);

	//memcpy((*R)->x->a, Rx->a, sizeof(word) * SIZE);
	bi_copy((*R)->x, Rx);
	
	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&temp3);
	bi_delete(&tempG);
	bi_delete(&a);
	bi_delete(&Rx);
	bi_delete(&Ry);

	return NOT_INFINITY_POINT;
}
int ECC_MOD_EXP_L2R(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP)
{
	ECC_bigint* Q = NULL;
	ECC_new(&Q, SIZE);
	//Q->flag = 1;
	int i, j;
	for (i = SIZE - 1; i >= 0; i--)
	{
		for (j = 31; j >= 0; j--)
		{
			Q->flag = ECC_DBL(Q, p, &Q);
			if ((k->a[i] >> j) & 0x1)
			{
				Q->flag = ECC_ADD(Q, P, p, &Q);
			}
		}
	}
	ECC_Copy(*kP, Q);
	ECC_delete(&Q);
	return operationSuccess;
}
int ECC_MOD_EXP_R2L(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP)
{
	ECC_bigint* Q = NULL;
	ECC_bigint* temp = NULL;
	ECC_new(&Q, 8);
	ECC_new(&temp, 8);
	ECC_Copy(temp, P);
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < 32; j++)
		{
			if ((k->a[i] >> j) & 0x1)
				Q->flag = ECC_ADD(Q, temp, p, &Q);
			temp->flag = ECC_DBL(temp, p, &temp);
		}
	}
	ECC_Copy(*kP, Q);
	ECC_delete(&Q);
	ECC_delete(&temp);
	return operationSuccess;
}
int ECC_Jacobian_ADD(IN ECC_bigint* P, IN ECC_bigint* Q, IN bigint* p, OUT ECC_bigint** R)
{
	if (Q->flag == INFINITY_POINT)
	{
		ECC_Copy(*R, P);
		return NOT_INFINITY_POINT;
	}
	else if (P->flag == INFINITY_POINT)
	{
		ECC_Copy(*R, Q);
		memset((*R)->z->a, 0, sizeof(word) * ((*R)->z->wordLen));
		(*R)->z->a[0] = 1;
		return NOT_INFINITY_POINT;
	}
	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* temp3 = NULL;
	bigint* temp4 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* T3 = NULL;
	bigint* T4 = NULL;
	bigint* Rx = NULL;
	bigint* Ry = NULL;
	bigint* Rz = NULL;

	bi_new(&temp1, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);
	bi_new(&temp3, 2 * SIZE);
	bi_new(&temp4, 2 * SIZE);
	bi_new(&T1, 2 * SIZE);
	bi_new(&T2, 2 * SIZE);
	bi_new(&T3, 2 * SIZE);
	bi_new(&T4, 2 * SIZE);
	bi_new(&Rx, SIZE);
	bi_new(&Ry, SIZE);
	bi_new(&Rz, SIZE);

	MUL_PS(P->z, P->z, &temp1);	//3
	FastReduction(temp1, p, &T1);
	MUL_PS(T1, P->z, &temp2);		//4
	FastReduction(temp2, p, &T2);
	MUL_PS(T1, Q->x, &temp1);		//5
	FastReduction(temp1, p, &T1);
	MUL_PS(T2, Q->y, &temp1);		//6
	FastReduction(temp1, p, &T2);
	SUB_FP(T1, P->x, p, &temp1);		//7
	memcpy(T1->a, temp1->a, sizeof(word) * (2 * SIZE));
	SUB_FP(T2, P->y, p, &temp1);		//8
	memcpy(T2->a, temp1->a, sizeof(word) * (2 * SIZE));
	if (!(memcmp(T1->a, temp3->a, sizeof(word) * SIZE)))	//9
	{
		if (!(memcmp(T2->a, temp3->a, sizeof(word) * SIZE)))
		{
			memset(Q->z->a, 0, sizeof(Q->z));
			Q->z->a[0] = 1;
			ECC_Jacobian_DBL(Q, p, R);
			return NOT_INFINITY_POINT;
		}
		else
			return INFINITY_POINT;
	}
	MUL_PS(P->z, T1, &temp1);		//10
	FastReduction(temp1, p, &(*R)->z);
	MUL_PS(T1, T1, &temp1);		//11
	FastReduction(temp1, p, &T3);
	MUL_PS(T3, T1, &temp1);		//12
	FastReduction(temp1, p, &T4);
	MUL_PS(T3, P->x, &temp1);		//13
	FastReduction(temp1, p, &T3);
	ADD_FP(T3, T3, p, &T1);				//14
	MUL_PS(T2, T2, &temp1);		//15
	FastReduction(temp1, p, &Rx);
	SUB_FP(Rx, T1, p, &temp1);		//16
	SUB_FP(temp1, T4, p, &Rx);		//17
	SUB_FP(T3, Rx, p, &temp1);		//18
	MUL_PS(temp1, T2, &temp2);		//19
	FastReduction(temp2, p, &T3);
	MUL_PS(T4, P->y, &temp1);		//20
	FastReduction(temp1, p, &T4);
	SUB_FP(T3, T4, p, &(*R)->y);			//21

	memcpy((*R)->x->a, Rx->a, sizeof(word) * SIZE);

	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&temp3);
	bi_delete(&temp4);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&T3);
	bi_delete(&T4);
	bi_delete(&Rx);
	bi_delete(&Ry);
	bi_delete(&Rz);

	return NOT_INFINITY_POINT;
}
int ECC_Jacobian_DBL(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R)
{
	if (P->flag == INFINITY_POINT)							//1
		return INFINITY_POINT;

	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* temp3 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* T3 = NULL;
	bigint* Rx = NULL;
	bigint* Ry = NULL;
	bigint* Rz = NULL;

	bi_new(&temp1, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);
	bi_new(&temp3, 2 * SIZE);
	bi_new(&T1, 2 * SIZE);
	bi_new(&T2, 2 * SIZE);
	bi_new(&T3, 2 * SIZE);
	bi_new(&Rx, SIZE);
	bi_new(&Ry, SIZE);
	bi_new(&Rz, SIZE);

	int carry = 0;

	MUL_PS(P->z, P->z, &temp1);	//2
	FastReduction(temp1, p, &T1);	//T1 = Z1^2

	SUB_FP(P->x, T1, p, &T2);			//3

	ADD_FP(P->x, T1, p, &temp1);			//4
	memcpy(T1->a, temp1->a, sizeof(word) * SIZE);

	MUL_PS(T1, T2, &temp1);		//5
	FastReduction(temp1, p, &T2);

	ADD_FP(T2, T2, p, &temp1);			//6
	ADD_FP(temp1, T2, p, &temp2);
	memcpy(T2->a, temp2->a, sizeof(word) * SIZE);

	ADD_FP(P->y, P->y, p, &temp1);		//7, temp1 = Y3

	MUL_PS(temp1, P->z, &temp2);	//8
	FastReduction(temp2, p, &(*R)->z);

	MUL_PS(temp1, temp1, &temp2);	//9
	FastReduction(temp2, p, &temp1);

	MUL_PS(temp1, P->x, &temp2);	//10
	FastReduction(temp2, p, &T3);

	MUL_PS(temp1, temp1, &temp2);	//11
	FastReduction(temp2, p, &temp1);

	if (!(temp1->a[0] & 0x1))						//12
		div2(&temp1);
	else
	{
		carry = ADD(temp1, p, &temp2);
		div2(&temp2);
		memcpy(temp1->a, temp2->a, sizeof(word) * (2 * SIZE));
		temp1->a[SIZE - 1] += (carry << 31);
	}

	MUL_PS(T2, T2, &temp2);		//13
	FastReduction(temp2, p, &temp3);	//temp3 = X3

	ADD_FP(T3, T3, p, &temp2);			//14

	SUB_FP(temp3, temp2, p, &(*R)->x);	//15

	SUB_FP(T3, (*R)->x, p, &temp3);		//16

	MUL_PS(temp3, T2, &temp2);		//17
	FastReduction(temp2, p, &T1);

	SUB_FP(T1, temp1, p, &(*R)->y);		//18

	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&temp3);
	bi_delete(&T1);
	bi_delete(&T2);
	bi_delete(&T3);
	bi_delete(&Rx);
	bi_delete(&Ry);
	bi_delete(&Rz);

	return NOT_INFINITY_POINT;
}
int ECC_Jac2Aff(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R)
{
	bigint* temp1 = NULL;
	bigint* temp2 = NULL;
	bigint* tempI = NULL;
	bi_new(&temp1, 2 * SIZE);
	bi_new(&temp2, 2 * SIZE);
	bi_new(&tempI, 2 * SIZE);

	MUL_PS(P->z, P->z, &temp1);
	FastReduction(temp1, p, &temp2);
	MUL_PS(temp2, P->z, &temp1);
	FastReduction(temp1, p, &temp2);
	INVERSION_Binary(temp2, p, &tempI);
	MUL_PS(P->y, tempI, &temp1);
	FastReduction(temp1, p, &(*R)->y);
	MUL_PS(P->z, tempI, &temp1);
	FastReduction(temp1, p, &temp2);
	MUL_PS(P->x, temp2, &temp1);
	FastReduction(temp1, p, &(*R)->x);

	bi_delete(&temp1);
	bi_delete(&temp2);
	bi_delete(&tempI);

	return operationSuccess;
}
int ECC_Jacobian_MOD_EXP_L2R(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP)
{
	ECC_bigint* Q = NULL;
	ECC_new(&Q, 8);
	int i, j;
	for (i = SIZE - 1; i >= 0; i--)
	{
		for (j = 31; j >= 0; j--)
		{
			Q->flag = ECC_Jacobian_DBL(Q, p, &Q);
			if ((k->a[i] >> j) & 0x1)
			{
				Q->flag = ECC_Jacobian_ADD(Q, P, p, &Q);
			}
		}
	}
	ECC_Jac2Aff(Q, p, kP);
	ECC_delete(&Q);
	return operationSuccess;
}
int ECC_SUB_CHAR(IN bigint* A, IN char B, OUT bigint** C)
{
	int i = 0, borrow = 0;
	bigint* temp = NULL;
	bi_new(&temp, SIZE);
	temp->a[0] = A->a[0] - B;
	for (i = 1; i < SIZE; i++)
		temp->a[i] = A->a[i];
	memcpy((*C)->a, temp->a, sizeof(word) * SIZE);
	bi_delete(&temp);
	return operationSuccess;
}
int ECC_ADD_WORD(IN bigint* A, IN word B, OUT bigint** C)
{
	int i = 0, carry = 0;
	bigint* temp = NULL;
	bi_new(&temp, SIZE);

	temp->a[i] = A->a[i] + B;
	if (temp->a[i] < A->a[i])
		carry = 1;
	for (i = 1; i < SIZE; i++)
	{
		temp->a[i] = A->a[i] + carry;
		if (A->a[i] != 0xffffffff)
			carry = 0;
	}
	memcpy((*C)->a, temp->a, sizeof(word) * SIZE);
	bi_delete(&temp);
	return operationSuccess;
}
int ECC_wNAF(IN bigint* k, IN int w, OUT char* wNAFk)
{
	int i = 0;
	word modAnd = (word)pow(2, w) - 1;		//mod 2^w를 하기 위한 변수
	word modValue = ((modAnd + 1) >> 1);	//wNAFk값이 +인지 -인지를 결정해주기 위한 2^(w-1)를 저장하는 변수
	word modSub = modValue << 1;			//2^w가 저장되어 있는 변수
	while (k->a[0] != 0)
	{
		if (k->a[0] & 0x1)
		{
			wNAFk[i] = (k->a[0] & modAnd);
			if (wNAFk[i] > modValue)
			{
				wNAFk[i] -= modSub;
			}
			ECC_SUB_CHAR(k, wNAFk[i], k);
		}
		else
			wNAFk[i] = 0;
		div2(&k);
		i++;
	}
	for (; i < 257; i++)
		wNAFk[i] = 0;
}
int ECC_wNAF_makeTable(IN ECC_bigint* P, IN int w, IN bigint* p, OUT ECC_bigint* table[])
{
	int i;
	int num = (int)pow(2, (double)w - 2);	//총 2*(w-2)개를 저장해야한다.
	ECC_bigint* P2 = NULL;
	ECC_bigint* temp = NULL;
	ECC_new(&P2, SIZE);
	ECC_new(&temp, SIZE);
	P2->flag = ECC_DBL(P, p, &P2);		//2P값을 P2에 저장한다.
	ECC_Copy(table[0], P);				//table[0]에 P를 저장한다.

	for (i = 1; i < num; i++)
	{
		table[i]->flag = ECC_ADD(table[i - 1], P2, p, &table[i]);
	}
	ECC_delete(&P2);
	ECC_delete(&temp);
}
int ECC_wNAF_SCALAR_MUL(IN ECC_bigint* table[], IN char* wNAFk, IN bigint* p, OUT ECC_bigint** kP)
{
	ECC_bigint* Q = NULL;
	ECC_bigint* temp = NULL;
	ECC_new(&Q, SIZE);
	ECC_new(&temp, SIZE);
	int i;

	for (i = 256; i >= 0; i--)
	{
		Q->flag = ECC_Jacobian_DBL(Q, p, &Q);
		if (wNAFk[i] != 0)
		{
			if (wNAFk[i] > 0)
			{
				Q->flag = ECC_Jacobian_ADD(Q, table[((wNAFk[i] - 1) / 2)], p, &Q);
			}
			else //wNAFk[i] < 0인 경우
			{
				SUB_FP(p, table[(((-wNAFk[i]) - 1) / 2)]->y, p, &temp->y);
				memcpy(temp->x, table[(((-wNAFk[i]) - 1) / 2)]->x, sizeof(word) * SIZE);
				//Q.flag = ECADD(&Q, &temp, p, &Q);
				Q->flag = ECC_Jacobian_ADD(Q, temp, p, &Q);
			}
		}
	}
	ECC_Jac2Aff(Q, p, kP);
	ECC_delete(&Q);
	ECC_delete(&temp);
}
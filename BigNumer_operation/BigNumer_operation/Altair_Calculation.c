#include "Altair_Calculation.h"
#include "Altair_Basic_Operations.h"

/**
 * @brief      single-precision addition 함수
 * @param      word		A			word형 데이터 A
 * @param      word		B			word형 데이터 B
 * @param      int		c			carry 값
 * @param      word*	C			A+B+c를 저장할 word형 배열
 * @return     A+B+c를 계산하면서 발생한 carry값
*/
int bi_ADDS(IN word A, IN word B, IN int c, OUT word* C)
{
	int c2 = 0;
	*C = A + B;
	if (*C < A)
		c2 = 1;
	*C += c;
	if (*C < c)
		c2++;
	return c2;
}

/**
 * @brief		multi-precision addition 함수
 * @param		bigint*		A			덧셈할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @param		wordLen(A) >= wordLen(B)
 * @param		sign(A) = sign(B)
 * @param		bigint**	C			덧셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_ADDC(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	int i;

	int c = 0;
	for (i = 0; i < B->wordLen; i++)
		c = bi_ADDS(A->a[i], B->a[i], c, &(*C)->a[i]);
	for (i = B->wordLen; i < A->wordLen; i++)
	{
		if (!c)
		{
			array_copy(&(*C)->a[i], &A->a[i], A->wordLen - i);
			//bi_refine(*C);
			return operationSuccess;
		}
		(*C)->a[i] = A->a[i] + c;
		if ((*C)->a[i] < c)
			c = 1;
		else
			c = 0;
	}
	(*C)->a[i] = c;
	/*if (!c)
		bi_refine(*C);*/

	return operationSuccess;
}
/**
 * @brief		addition 함수
 * @param		bigint*		A			덧셈할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @param		bigint**	C			덧셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	if (bi_is_zero(A))
	{
		bi_copy(*C, B);
	}
	else if (bi_is_zero(B))
	{
		bi_copy(*C, A);
	}
	else if ((!(A->sign) && (B->sign)))//A>0, B<0
	{
		B->sign = 0;
		bi_SUB(A, B, C);
		B->sign = 1;
	}
	else if ((A->sign) && (!(B->sign)))//A<0, B>0
	{
		A->sign = 0;
		bi_SUB(B, A, C);
		A->sign = 1;
	}
	else if ((A->wordLen < B->wordLen))
	{
		bi_ADDC(B, A, C);
	}
	else
	{
		bi_ADDC(A, B, C);
	}
	bi_refine(*C);
	return operationSuccess;
}
/**
 * @brief		addition 함수, A = A + B,	wordLen은 복사하지않는다.
 * @param		bigint**	A			덧셈할 구조체 및 결과를 저장할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD_AAB(IN bigint** A, IN bigint* B)
{
	bigint* C = NULL;
	bi_new(&C, (*A)->wordLen + B->wordLen);
	if (C == NULL)
		return memoryAllocationFailure;
	bi_ADD(*A, B, &C);
	memset((*A)->a, 0, (*A)->wordLen); //**
	memcpy((*A)->a, C->a, sizeof(word) * C->wordLen);
	(*A)->sign = C->sign;
	bi_delete(&C);
	return operationSuccess;
}
/**
 * @brief		addition 함수, A = A + B,	wordLen까지 복사
 * @param		bigint**	A			덧셈할 구조체 및 결과를 저장할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD_AAB2(IN bigint** A, IN bigint* B)
{
	bigint* C = NULL;
	bi_new(&C, (*A)->wordLen + B->wordLen);
	if (C == NULL)
		return memoryAllocationFailure;
	bi_ADD(*A, B, &C);
	memset((*A)->a, 0, (*A)->wordLen); //**
	bi_copy(*A, C);
	bi_delete(&C);
	return operationSuccess;
}
/**
 * @brief      single-precision subtraction 함수
 * @param      word		A			word형 데이터 A
 * @param      word		B			word형 데이터 B
 * @param      int		b			borrow 값
 * @param      word*	C			A-B-c를 저장할 word형 배열
 * @return     A-B-c를 계산하면서 발생한 borrow값
*/
int bi_SUBS(IN word A, IN word B, IN int b, OUT word* C)
{
	*C = A - b;
	if (A < b)
		b = 1;
	else
		b = 0;
	if (*C < B)
		b++;
	*C = *C - B;
	return b;
}
/**
 * @brief		multi-precision subtraction 함수
 * @param		bigint*		A			뺄셈할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @param		A>=B>0
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SUBC(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	int i;

	int bValueIndex = 0;	//Karatsuba에서의 예외처리를 위해 추가한 내용, B의 0이 아닌 값 중 가장 큰 index를 bValueIndex에 저장한다.
	for (i = B->wordLen - 1; i >= 0; i--)
		if (B->a[i] != 0)
		{
			bValueIndex = i;
			break;
		}
	bValueIndex++;

	int aValueIndex = 0;
	for (i = A->wordLen - 1; i >= 0; i--)
		if (A->a[i] != 0)
		{
			aValueIndex = i;
			break;
		}
	aValueIndex++;


	int b = 0;
	//for (i = 0; i < B->wordLen; i++)
	for (i = 0; i < bValueIndex; i++)
		b = bi_SUBS(A->a[i], B->a[i], b, &(*C)->a[i]);

	//for (i = B->wordLen; i < A->wordLen; i++)
	for (i = bValueIndex; i < aValueIndex; i++)
	{
		if (!b)
		{
			array_copy(&(*C)->a[i], &A->a[i], A->wordLen - i);
			return operationSuccess;
		}
		(*C)->a[i] = A->a[i] - b;
		if (A->a[i] < b)
			b = 1;
		else
			b = 0;
	}
	/*bi_refine(*C);*/
	return operationSuccess;
}
/**
 * @brief		subtraction 함수
 * @param		bigint*		A			뺄셈할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SUB(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	if (bi_is_zero(A))
	{
		bi_copy(*C, B);
		bi_flip_sign(C);
	}
	else if (bi_is_zero(B))
	{
		bi_copy(*C, A);
	}
	else if (bi_compare(A, B) == 0)
	{
		bi_set_zero(C);
	}
	else if ((bi_compare(A, B) != -1) && (!((B->sign))))
	{
		bi_SUBC(A, B, C);
	}
	else if ((bi_compare(A, B) == -1) && (!((A->sign))))
	{
		bi_SUBC(B, A, C);
		bi_flip_sign(C);
	}
	else if ((bi_compare(A, B) != -1) && ((A->sign)))
	{
		A->sign = 0;
		B->sign = 0;
		bi_SUBC(B, A, C);
		A->sign = 1;
		B->sign = 1;
	}
	else if ((bi_compare(A, B) == -1) && ((B->sign)))
	{
		A->sign = 0;
		B->sign = 0;
		bi_SUBC(A, B, C);
		A->sign = 1;
		B->sign = 1;
		bi_flip_sign(C);
	}
	else if ((!(A->sign)) && (B->sign))
	{
		B->sign = 0;
		bi_ADD(A, B, C);
		B->sign = 1;
	}
	else
	{
		B->sign = 0;
		bi_ADD(A, B, C);
		B->sign = 1;
		bi_flip_sign(C);
	}
	bi_refine(*C);
	return operationSuccess;
}
int bi_SUB_noRefine(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	if (bi_is_zero(A))
	{
		bi_copy(*C, B);
		bi_flip_sign(C);
	}
	else if (bi_is_zero(B))
	{
		bi_copy(*C, A);
	}
	else if (bi_compare(A, B) == 0)
	{
		bi_set_zero(C);
	}
	else if ((bi_compare(A, B) != -1) && (!((B->sign))))
	{
		bi_SUBC(A, B, C);
	}
	else if ((bi_compare(A, B) == -1) && (!((A->sign))))
	{
		bi_SUBC(B, A, C);
		bi_flip_sign(C);
	}
	else if ((bi_compare(A, B) != -1) && ((A->sign)))
	{
		A->sign = 0;
		B->sign = 0;
		bi_SUBC(B, A, C);
		A->sign = 1;
		B->sign = 1;
	}
	else if ((bi_compare(A, B) == -1) && ((B->sign)))
	{
		A->sign = 0;
		B->sign = 0;
		bi_SUBC(A, B, C);
		A->sign = 1;
		B->sign = 1;
		bi_flip_sign(C);
	}
	else if ((!(A->sign)) && (B->sign))
	{
		B->sign = 0;
		bi_ADD(A, B, C);
		B->sign = 1;
	}
	else
	{
		B->sign = 0;
		bi_ADD(A, B, C);
		B->sign = 1;
		bi_flip_sign(C);
	}
	return operationSuccess;
}
int bi_SUB_AAB(IN bigint** A, IN bigint* B)
{
	bigint* C = NULL;
	bi_new(&C, (*A)->wordLen + B->wordLen);
	if (C == NULL)
		return memoryAllocationFailure;
	bi_SUB(*A, B, &C);
	memset((*A)->a, 0, (*A)->wordLen * sizeof(word));
	memcpy((*A)->a, C->a, sizeof(word) * C->wordLen);
	(*A)->sign = C->sign;
	bi_delete(&C);
	return operationSuccess;
}
/**
 * @brief		single-precision multiplication 함수
 * @param		word		A			word형 데이터 A
 * @param		word		B			word형 데이터 B
 * @param		A,B in [0,W)
 * @param		word*		C			A*B를 저장할 word형 배열
 * @return		함수 실행 결과
*/
int bi_MULS(IN word A, IN word B, OUT word* C)
{
	if ((A < 0) || (B < 0))		//A와 B의 자료형이 unsigned이기 때문에 이 조건이 발생할 일은 없다.
		return parameterError;
	else if ((A == 0) || (B == 0))
	{
		memset(C, 0, sizeof(word) * 2);
		return operationSuccess;
	}
	else if (A == 1)
	{
		C[0] = B;
		C[1] = 0;
		//memcpy(C, &B, sizeof(word));
		return operationSuccess;
	}
	else if (B == 1)
	{
		C[0] = A;
		C[1] = 0;
		//memcpy(C, &A, sizeof(word));
		return operationSuccess;
	}
	word A1, A0, B1, B0, T1, T0, T;
	A1 = A >> (WORD_BITLEN / 2);
	B1 = B >> (WORD_BITLEN / 2);
	A0 = A & (((u64)1 << (WORD_BITLEN / 2)) - 1);
	B0 = B & (((u64)1 << (WORD_BITLEN / 2)) - 1);

	T1 = A1 * B0;
	T0 = A0 * B1;
	T0 = T1 + T0;
	T1 = T0 < T1;
	C[1] = A1 * B1;
	C[0] = A0 * B0;
	T = C[0];
	C[0] += (word)(T0 << (WORD_BITLEN / 2));
	C[1] += (word)(T1 << (WORD_BITLEN / 2)) + (word)(T0 >> (WORD_BITLEN / 2)) + (C[0] < T);
	return operationSuccess;
}
/**
 * @brief		multi-precision schoolbook multiplication 함수
 * @param		bigint*		A			곱셈할 구조체, A in [W^(n-1), W^n)
 * @param		bigint*		B			곱셈할 구조체, B in [W^(m-1), W^m)
 * @param		bigint**	C			곱셈 결과를 저장할 구조체, 메모리 할당 후 입력
 * @return		함수 실행 결과
*/
int bi_MULC(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	int i, j;
	bigint* T = NULL;
	bi_new(&T, A->wordLen + B->wordLen);
	memset(T->a, 0, sizeof(word) * (T->wordLen));
	for (i = 0; i < A->wordLen; i++)
	{
		for (j = 0; j < B->wordLen; j++)
		{
			bi_MULS(A->a[i], B->a[j], &T->a[i + j]);
			bi_ADD_AAB(C, T);
			T->a[i + j] = 0;
		}
		T->a[i + j] = 0;
	}
	bi_delete(&T);
	return operationSuccess;
}
/**
 * @brief		multiplication 함수
 * @param		bigint*		A			곱셈할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @param		bigint**	C			곱셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	memset((*C)->a, 0, sizeof(word) * (*C)->wordLen);	//ECC하던 중 추가
	if ((bi_is_zero(A) == TRUE) || (bi_is_zero(B) == TRUE))
	{
		bi_set_zero(C);
		return operationSuccess;
	}
	else if (bi_is_one(A) == TRUE)
	{
		bi_copy(*C, B);
		return operationSuccess;
	}
	else if ((bi_get_word_length(A) == 1) && (A->a[0] == 1) && (bi_get_sign(A)))
	{
		bi_copy(*C, B);
		bi_flip_sign(C);
		return operationSuccess;
	}
	else if (bi_is_one(B) == TRUE)
	{
		bi_copy(*C, A);
		return operationSuccess;
	}
	else if ((bi_get_word_length(B) == 1) && (B->a[0] == 1) && (bi_get_sign(B)))
	{
		bi_copy(*C, A);
		bi_flip_sign(C);
		return operationSuccess;
	}
	else
	{
		int Asign = A->sign;
		int Bsign = B->sign;
		A->sign = 0;
		B->sign = 0;
		int ret = bi_MULC(A, B, C);
		A->sign = Asign;
		B->sign = Bsign;
		(*C)->sign = Asign ^ Bsign;
		if (ret == operationFailure)
			return operationFailure;
		else
			return operationSuccess;
	}
}
/**
 * @brief		multiplication 함수, A = A*B
 * @param		bigint**	A			곱셈 및 저장할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL_AAB(IN bigint** A, IN bigint* B)
{
	bigint* C = NULL;
	bi_new(&C, (*A)->wordLen + B->wordLen);
	if (C == NULL)
		return memoryAllocationFailure;
	if (bi_MUL(*A, B, &C) == operationSuccess)
	{
		bi_refine(C);
		
		array_copy((*A)->a, C->a, C->wordLen);
		//bi_copy(*A, C);
		bi_delete(&C);
		return operationSuccess;
	}
	bi_delete(&C);
	return operationFailure;
}
/**
 * @brief		multiplication Karatsuba 함수
 * @param		bigint*		A			곱셈할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @param		bigint**	C			곱셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL_Karatsuba(IN bigint* A, IN bigint* B, OUT bigint** C)
{
	if ((A->wordLen < 4) && (B->wordLen < 4))	//flag 조정해가면서 속도비교
	{
		bi_MUL(A, B, C);
		return operationSuccess;
	}
	int l = (((A->wordLen > B->wordLen) * A->wordLen + (!(A->wordLen > B->wordLen)) * B->wordLen) + 1) >> 1;
	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* B1 = NULL;
	bigint* B0 = NULL;
	bigint* T1 = NULL;
	bigint* T0 = NULL;
	bigint* R = NULL;
	bigint* S1 = NULL;
	bigint* S0 = NULL;
	bigint* S = NULL;

	bi_new(&A0, A->wordLen);
	bi_new(&A1, A->wordLen);
	bi_new(&B0, B->wordLen);
	bi_new(&B1, B->wordLen);
	memcpy(A0->a, A->a, sizeof(word) * A->wordLen);
	memcpy(A1->a, A->a, sizeof(word) * A->wordLen);
	memcpy(B0->a, B->a, sizeof(word) * B->wordLen);
	memcpy(B1->a, B->a, sizeof(word) * B->wordLen);
	bi_right_shift(&A1, l * WORD_BITLEN);
	bi_refine(A1);	//
	bi_reduction(&A0, l * WORD_BITLEN);
	bi_right_shift(&B1, l * WORD_BITLEN);
	bi_refine(B1);	//
	bi_reduction(&B0, l * WORD_BITLEN);

	bi_new(&T1, A1->wordLen + B1->wordLen);
	bi_new(&T0, A0->wordLen + B0->wordLen);
	bi_MUL_Karatsuba(A1, B1, &T1);
	bi_MUL_Karatsuba(A0, B0, &T0);

	bi_new(&R, 4 * l);
	bi_ADD_AAB(&R, T1);
	bi_left_shift(&R, 2 * l * WORD_BITLEN);
	bi_ADD_AAB(&R, T0);

	bi_new(&S1, l);
	bi_new(&S0, l);
	bi_SUB(A0, A1, &S1);
	bi_SUB(B1, B0, &S0);

	bi_new(&S, 4 * l);	//S 할당하는 부분
	bi_MUL_Karatsuba(S0, S1, &S);
	S->sign = S1->sign != S0->sign;
	bi_ADD_AAB(&S, T1);
	bi_ADD_AAB(&S, T0);
	bi_left_shift(&S, l * WORD_BITLEN);

	bi_ADD_AAB(&R, S);
	bi_refine(R);
	memcpy((*C)->a, R->a, sizeof(word) * R->wordLen);

	bi_delete(&A0);
	bi_delete(&A1);
	bi_delete(&B0);
	bi_delete(&B1);
	bi_delete(&T0);
	bi_delete(&T1);
	bi_delete(&R);
	bi_delete(&S0);
	bi_delete(&S1);
	bi_delete(&S);			//Heap 오류 발생

	return operationSuccess;
}
/**
 * @brief		single-precision squaring 함수
 * @param		word		A			word형 데이터 A
 * @param		A in (-W,W)
 * @param		word*		C			A^2를 저장할 word형 배열
 * @return		함수 실행 결과
*/
int bi_SQUS(IN word A, OUT word* C)
{
	if (A == 0)
	{
		memset(C, 0, sizeof(word) * 2);
		return operationSuccess;
	}
	else if (A == 1)
	{
		C[0] = 1;
		C[1] = 0;
	}
	word A1, A0, T, T1, T0;
	int c = 0;
	A1 = A >> (WORD_BITLEN / 2);
	A0 = A & (((u64)1 << (WORD_BITLEN / 2)) - 1);
	C[1] = A1 * A1;
	C[0] = A0 * A0;
	T = A0 * A1;

	T0 = T << ((WORD_BITLEN / 2) + 1);
	C[0] += T0;
	if (C[0] < T0)
		c = 1;

	T1 = T >> ((WORD_BITLEN / 2) - 1);
	C[1] += T1 + c;

	return operationSuccess;
}
/**
 * @brief		multi-precision schoolbook squaring 함수
 * @param		bigint*		A			제곱할 구조체, A in [W^(n-1), W^n)
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQUC(IN bigint* A, OUT bigint** C)
{
	int i, j, c = 0, c2 = 0, cnt = 0;
	word T1[2] = { 0, };
	word T2[2] = { 0, };
	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* tempC = NULL;	//
#if TESTING == 0

	int AValidWordLen = 0;
	i = A->wordLen - 1;
	while (A->a[i] == 0)
	{
		i--;
	}
	AValidWordLen = i;

	bi_new(&C1, (2 * AValidWordLen) + 1);
	bi_new(&C2, (2 * AValidWordLen) + 1);
	if ((C1 == NULL) || (C2 == NULL))
		return memoryAllocationFailure;

	for (i = 0; i < AValidWordLen; i++)
	{
		bi_SQUS(A->a[i], T1);
		array_copy(&C1->a[2 * i], T1, 2);
		c = 0;
		for (j = i + 1; j < AValidWordLen; j++)
		{
			bi_MULS(A->a[i], A->a[j], T2);

			c2 = 0;
			C2->a[i + j] += T2[0];
			if (C2->a[i + j] < T2[0])
				c2 = 1;
			c = c2;

			c2 = 0;
			C2->a[i + j + 1] += T2[1];
			if (C2->a[i + j + 1] < T2[1])
				c2 = 1;
			C2->a[i + j + 1] += c;
			if (C2->a[i + j + 1] < c)
				c2++;
			c = c2;

			cnt = 0;
			while (1)
			{
				C2->a[i + j + 2 + cnt] += c;
				if (C2->a[i + j + 2 + cnt] < c)
					c = 1;
				else
					break;
				cnt++;
			}
		}
	}
#else
	bi_new(&C1, 2 * A->wordLen);
	bi_new(&C2, 2 * A->wordLen);
	bi_new(&tempC, (*C)->wordLen);	//
	if ((C1 == NULL) || (C2 == NULL))
		return memoryAllocationFailure;

	for (i = 0; i < A->wordLen; i++)
	{
		bi_SQUS(A->a[i], T1);
		array_copy(&C1->a[2 * i], T1, 2);
		c = 0;
		for (j = i + 1; j < A->wordLen; j++)
		{
			bi_MULS(A->a[i], A->a[j], T2);

			c2 = 0;
			C2->a[i + j] += T2[0];
			if (C2->a[i + j] < T2[0])
				c2 = 1;
			c = c2;

			c2 = 0;
			C2->a[i + j + 1] += T2[1];
			if (C2->a[i + j + 1] < T2[1])
				c2 = 1;
			C2->a[i + j + 1] += c;
			if (C2->a[i + j + 1] < c)
				c2++;
			c = c2;

			cnt = 0;
			while (1)
			{
				C2->a[i + j + 2 + cnt] += c;
				if (C2->a[i + j + 2 + cnt] < c)
					c = 1;
				else
					break;
				cnt++;
			}
		}
	}
#endif
	bi_left_shift(&C2, 1);
#if TESTING 1
	bi_refine(C1);
	bi_refine(C2);
#endif
	//bi_ADD(C1,C2,C);
	bi_ADD(C1, C2, &tempC);								//
	array_copy((*C)->a, tempC->a, tempC->wordLen);		//
	bi_delete(&C1);
	bi_delete(&C2);
	bi_delete(&tempC);									//
	return operationSuccess;
}
/**
 * @brief		squaring 함수
 * @param		bigint*		A			제곱할 구조체
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU(IN bigint* A, OUT bigint** C)
{
	memset((*C)->a, 0, sizeof(word) * (*C)->wordLen);
	if ((bi_is_zero(A) == TRUE) || (bi_is_one(A) == TRUE))
	{
		bi_copy(*C, A);
		(*C)->sign = NON_NEGATIVE;
		return operationSuccess;
	}
	bi_SQUC(A, C);
	return operationSuccess;
}
/**
 * @brief		squaring 함수, A = A^2
 * @param		bigint*		A			연산에 사용할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU_AA(IN OUT bigint** A)
{
	bigint* B = NULL;
	bi_new(&B, (*A)->wordLen * 2);
	if (B == NULL)
		return memoryAllocationFailure;
	if (bi_SQU(*A, &B) == operationSuccess)
	{
		array_copy((*A)->a, B->a, B->wordLen);
		bi_delete(&B);
		return operationSuccess;
	}
	bi_delete(&B);
	return operationFailure;
}
/**
 * @brief		squaring Karatsuba 함수
 * @param		bigint*		A			제곱할 구조체
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU_Karatsuba(IN bigint* A, OUT bigint** C)
{
	if (A->wordLen < 4)	//flag 조정해가면서 속도비교
	{
		bi_SQU(A, C);
		return operationSuccess;
	}
	int l = (A->wordLen + 1) >> 1;
	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* T1 = NULL;
	bigint* T0 = NULL;
	bigint* R = NULL;
	bigint* S = NULL;

	bi_new(&A0, A->wordLen);
	bi_new(&A1, A->wordLen);
	array_copy(A0->a, A->a, A->wordLen);
	array_copy(A1->a, A->a, A->wordLen);

	bi_right_shift(&A1, l * WORD_BITLEN);
	bi_refine(A1);
	bi_reduction(&A0, l * WORD_BITLEN);
	A1->sign = NON_NEGATIVE;
	A0->sign = NON_NEGATIVE;

	bi_new(&T1, 4 * l);
	bi_new(&T0, 4 * l);
	/*bi_new(&T1, 2 * A1->wordLen);
	bi_new(&T0, 2 * A0->wordLen);*/
	bi_SQU_Karatsuba(A1, &T1);
	bi_SQU_Karatsuba(A0, &T0);

	bi_new(&R, 4 * l);
	bi_ADD_AAB(&R, T1);
	bi_left_shift(&R, 2 * l * WORD_BITLEN);
	bi_ADD_AAB(&R, T0);

	bi_new(&S, 4 * l);
	bi_MUL_Karatsuba(A1, A0, &S);
	bi_left_shift(&S, (l * WORD_BITLEN) + 1);

	bi_ADD_AAB(&R, S);

	bi_refine(R);	//이거 넣어서 해결, R 크기와 C의 크기를 맞춤
	bi_copy(*C, R);

	bi_delete(&A0);
	bi_delete(&A1);
	bi_delete(&T1);
	bi_delete(&T0);
	bi_delete(&R);
	bi_delete(&S);

	return operationSuccess;
}

/**
 * @brief		binary division 함수,	A = B*Q + R
 * @param		bigint*		A			나눠지는 수
 * @param		bigint*		B			A를 나눌 수
 * @param		bigint**	Q			몫, 함수입력전에 크기를 할당해주어한다.
 * @param		bigint**	R			나머지, 함수입력전에 크기를 할당해주어한다.
 * @return		함수 실행 결과
*/
int bi_DIV_BINARY(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R)
{
	if ((B->sign == NEGATIVE) || (bi_is_zero(B) == TRUE))
		return parameterError;
	if (bi_compare(A, B) == -1)
	{
		bi_set_zero(Q);
		bi_copy(*R, A);
		return operationSuccess;
	}
	int i, j;
	for (i = A->wordLen - 1; i >= 0; i--)
	{
		for (j = WORD_BITLEN - 1; j >= 0; j--)
		{
			bi_left_shift(R, 1);
			(*R)->a[0] += (((A->a[i]) >> j) & 0x1);
			if (bi_compare(*R, B) > -1)
			{
				(*Q)->a[i] += ((long long)1 << j);
				bi_SUB_AAB(R, B);
			}
		}
	}
	bi_refine(*Q);
	bi_refine(*R);
	return operationSuccess;
}
/**
 * @brief		C = floor(A/B)
 * @param		word*		A			나눠지는 수, word 2개
 * @param		word		B			A를 나눌 수, word 1개
 * @param		word		C			A/B의 몫을 저장
 * @return		함수 실행 결과
*/
int bi_2WORD_DIV_1WORD(IN word* A, IN word B, OUT word* C)
{
	word R = A[1];
	int i;
	for (i = WORD_BITLEN - 1; i >= 0; i--)
	{
		if (R >> (WORD_BITLEN - 1))
		{
			*C += ((long long)1 << i);
			R = (R << 1) + ((A[0] >> i) & 0x1) - B;
		}
		else
		{
			R = (R << 1) + ((A[0] >> i) & 0x1);
			if (!(R < B))
			{
				*C += ((long long)1 << i);
				R -= B;
			}
		}
	}
}

/**
 * @brief		DIV core 속의 core
 * @param		bigint*		A			나눠지는 수
 * @param		bigint*		B			나누는 수
 * @param		bigint**	Q			몫
 * @param		bigint**	R			나머지
 * @return		함수 실행 결과
*/
int bi_DIVCC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R)
{
	bigint* temp = NULL;
	bigint* tempR = NULL;
	bi_new(&temp, B->wordLen + 1);
	bi_new(&tempR, A->wordLen);
	if (A->wordLen == B->wordLen)
	{
		(*Q)->a[0] = A->a[A->wordLen - 1] / B->a[A->wordLen - 1];
	}
	if (A->wordLen == B->wordLen + 1)
	{
		if (A->a[A->wordLen - 1] == B->a[B->wordLen - 1])
		{
			(*Q)->a[0] = WORD_MASK;
		}
		else
		{
			bi_2WORD_DIV_1WORD(&A->a[B->wordLen - 1], B->a[B->wordLen - 1], &(*Q)->a[0]);
		}
	}
	bi_MUL(B, *Q, &temp);
	//추가
	bi_SUB_noRefine(A, temp, &tempR);		//여기서 tempR의 크기가 refine(A-temp)로 설정된다.
	while (tempR->sign)
	{
		(*Q)->a[0]--;
		bi_ADD_AAB2(&tempR, B);
	}
	array_copy((*R)->a, tempR->a, tempR->wordLen);
	//
	bi_delete(&temp);
	bi_delete(&tempR);
	return operationSuccess;
}
/**
* @brief		DIV core
* @param		bigint* A			나눠지는 수
* @param		bigint* B			나누는 수
* @param		bigint** Q			몫
* @param		bigint** R			나머지
* @return		함수 실행 결과
*/
int bi_DIVC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R)
{
	bigint* tmpA = NULL;
	bigint* tmpB = NULL;
	/*bi_assign(&tmpA, A);
	bi_assign(&tmpB, B);*/
	bi_new(&tmpA, A->wordLen+1);
	bi_new(&tmpB, B->wordLen+1);
	array_copy(tmpA->a, A->a, A->wordLen);
	array_copy(tmpB->a, B->a, B->wordLen);
	/*bi_copy(tmpA, A);
	bi_copy(tmpB, B);*/
	if (bi_compare(A, B) == -1)
	{
		bi_set_zero(Q);
		array_copy((*R)->a, A->a, A->wordLen);
		bi_delete(&tmpA);
		bi_delete(&tmpB);
		return operationSuccess;
	}
	int k = 0;
	while (!(B->a[B->wordLen - 1] >> (WORD_BITLEN - 1 - k)))
		k++;

	bi_left_shift(&tmpA, k);
	bi_left_shift(&tmpB, k);

	bi_refine(tmpA);
	bi_refine(tmpB);

	bi_DIVCC(tmpA, tmpB, Q, R);
	bi_right_shift(R, k);

	bi_delete(&tmpA);
	bi_delete(&tmpB);

	return operationSuccess;
}
/**
* @brief		DIV의 내부함수, R<-RW+Ai를 처리
* @param		bigint** R			연산되고 저장할 bigint
* @param		word A				RW의 가장 최하위 index word에 더해질 값
* @return		함수 실행 결과
*/
int bi_DIV_func(IN bigint** R, word A)
{
	int i;
	for (i = (*R)->wordLen - 2; i >= 0; i--)
	{
		(*R)->a[i + 1] = (*R)->a[i];
	}
	(*R)->a[0] = A;
	return operationSuccess;
}
/**
* @brief		DIV
* @param		bigint* A			나눠지는 수
* @param		bigint* B			나누는 수
* @param		bigint** Q			몫
* @param		bigint** R			나머지
* @return		함수 실행 결과
*/
int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R)
{
	int i;
	bigint* tmpQ = NULL;
	bigint* tmpR = NULL;
	bi_new(&tmpQ, 1);
	bi_new(&tmpR, A->wordLen);

	if ((bi_get_sign(B) == NEGATIVE) || (bi_is_zero(B) == TRUE))
		return parameterError;
	if (bi_compare(A, B) == -1)
	{
		bi_set_zero(Q);
		bi_assign(R, A);
		return operationSuccess;
	}
	memset((*Q)->a, 0, sizeof(word) * (*Q)->wordLen);
	memset((*R)->a, 0, sizeof(word) * (*R)->wordLen);
	for (i = A->wordLen - 1; i >= 0; i--)
	{
		bi_DIV_func(&tmpR, A->a[i]);
		memset((*R)->a, 0, (*R)->wordLen);
		bi_set_zero(&tmpQ);							//wordLen = 1짜리 bigint tmpQ를 사용해서 Qi를 입력받는다.
		bi_DIVC(tmpR, B, &tmpQ, R);
		(*Q)->a[i] = tmpQ->a[0];
		array_copy(tmpR->a, (*R)->a, (*R)->wordLen);
	}
	//
	bi_refine(*Q);
	bi_refine(*R);
	//
	bi_delete(&tmpQ);
	bi_delete(&tmpR);	// HEAP
}

/**
* @brief		bi_L2R 안에서 사용하는 내부 함수
* @detail		한 word의 총 bit를 계산해주는 내부 함수
* @param		word A		bit를 계산할 워드
* @return		word의 총 bit 수
*/
word bi_L2R_get_bit_func(word A) {

	for (word i = 0; i < WORD_BITLEN; i++) {
		if ((A >> i) == 0)
			return i;
	}
	return WORD_BITLEN;
}

/**
* @brief		Left to Right 방법의 지수승 연산 
* @param		bigint* A		지수승 연산의 밑(x^n 의 x)
* @param		bitint* N		지수승 연산의 지수(x^n 의 n)
* @param		bigint* out		결과 값을 저장할 구조체
* @return		함수 실행 결과
*/
word bi_L2R_BINARY_allocation(IN bigint* A, IN bigint* N, OUT bigint** out) {

	if (TRUE == bi_is_zero(N)) {
		bi_set_one(out);
		return operationSuccess;
	}

	word len = 0;			//지수승할 최상위 워드 데이터 비트 길이를 저장할 변수
	word* new_temp = NULL;	//중간 값 Temp 저장해놓을 배열

	bigint* temp = NULL;

	int k = 0;
	len = bi_L2R_get_bit_func(N->a[N->wordLen - 1]);
	
	bi_new(&temp, 4 * A->wordLen);
	temp->a[0] = 1;
	
	/*
		Left to Right 방식은 N의 비트별로 계산하기 때문에 N의 최상위 워드와 나머지 워드를 나눠서 연산
		N의 최상위 워드는 WORD_BITLEN으로 다 차있지 않는 경우도 존재하기 때문에 나눠서 구현
	*/
	for (int i = len - 1; i >= 0; i--) {
		bi_SQU_AA(&temp);
		if ((N->a[N->wordLen - 1] >> i) & 0x1) {
			bi_MUL_AAB(&temp, A);
		}
		/*
			Altair의 내부 함수는 C의 크기를 정해주고 들어가는 함수로 구성되어 있어서 
			연산을 하기 위해 크기를 재설정해주고 값을 옮겨주는 부분이 필요함.
			해당 부분을 통해 값을 재설정
		*/
		bi_refine(temp);
		k = temp->wordLen + A->wordLen + 1;
		new_temp = (word*)calloc(temp->wordLen, sizeof(word) * temp->wordLen);
		if (new_temp == NULL)
			return operationFailure;
		memcpy(new_temp, temp->a, (temp->wordLen) * sizeof(word));
		bi_delete(&temp);
		bi_new(&temp, 3 * k);
		array_copy(temp->a, new_temp, (k - A->wordLen - 1));
		free(new_temp);
	}

	/*
		최상위 워드를 제외한 나머지 부분의 연산 부분
	*/
	for (int i = N->wordLen - 2; i >= 0; i--) {

		for (int j = WORD_BITLEN - 1; j >= 0; j--) {

			bi_SQU_AA(&temp);
			if ((N->a[i] >> j) & 0x1)
				bi_MUL_AAB(&temp, A);
			bi_refine(temp);
			k = temp->wordLen + A->wordLen + 1;
			new_temp = (word*)calloc(temp->wordLen, sizeof(word) * temp->wordLen);
			if (new_temp == NULL)
				return operationFailure;
			memcpy(new_temp, temp->a, (temp->wordLen) * sizeof(word));
			bi_delete(&temp);
			bi_new(&temp, 4 * k);
			array_copy(temp->a, new_temp, (k - A->wordLen - 1));
			free(new_temp);
		}
	}
	bi_refine(temp);
	bi_new(out, temp->wordLen);
	bi_copy(*out, temp);
	bi_delete(&temp);
}

/**
* @brief		Left to Right 방법의 Modular Exponentitation 연산
* @param		bigint* A		지수승 연산의 밑(x^n 의 x)
* @param		bitint* N		지수승 연산의 지수(x^n 의 n)
* @param		bigint* M		Modular 연산을 할 M 값
* @param		bigint* out		결과 값을 저장할 구조체
* @return		함수 실행 결과
*/

word bi_L2R_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out) {

	if (bi_is_zero(M) == TRUE) {
		return operationFailure;
	}
	if (bi_is_zero(N) == TRUE) {
		bi_set_one(out);
		return operationSuccess;
	}

	bigint* temp = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* Q1 = NULL;
	bigint* R1 = NULL;

	int len = bi_L2R_get_bit_func(N->a[N->wordLen - 1]);
	u32 k = 0;

	bi_new(&temp, 4 * A->wordLen);
	temp->a[0] = 1;
	for (int i = len - 1; i >= 0; i--) {
		bi_SQU_AA(&temp);
		bi_new(&Q1, temp->wordLen);
		bi_new(&R1, temp->wordLen);
		bi_DIV(temp, M, &Q1, &R1);
		k = temp->wordLen + A->wordLen + 1;
		bi_delete(&temp);
		bi_new(&temp, k);
		array_copy(temp->a, R1->a, R1->wordLen);
		bi_delete(&Q1, &R1);

		if ((N->a[N->wordLen - 1] >> i) & 0x1) 
			bi_MUL_AAB(&temp, A);
		

		bi_refine(temp);
		bi_new(&Q, temp->wordLen);
		bi_new(&R, temp->wordLen);
		bi_DIV(temp, M, &Q, &R);
		k = temp->wordLen;
		bi_delete(&temp);
		bi_new(&temp, 2 * k);
		array_copy(temp->a, R->a, R->wordLen);
		bi_delete(&Q);
		bi_delete(&R);
	}

	for (int i = N->wordLen - 2; i >= 0; i--) {

		for (int j = WORD_BITLEN - 1; j >= 0; j--) {

			bi_SQU_AA(&temp);
			bi_new(&Q1, temp->wordLen);
			bi_new(&R1, temp->wordLen);
			bi_DIV(temp, M, &Q1, &R1);
			k = temp->wordLen + A->wordLen + 1;
			bi_delete(&temp);
			bi_new(&temp, k);
			array_copy(temp->a, R1->a, R1->wordLen);
			bi_delete(&Q1, &R1);

			if ((N->a[i] >> j) & 0x1)
				bi_MUL_AAB(&temp, A);

			bi_refine(temp);
			bi_new(&Q, temp->wordLen);
			bi_new(&R, temp->wordLen);
			bi_DIV(temp, M, &Q, &R);
			k = temp->wordLen;
			bi_delete(&temp);
			bi_new(&temp, 2 * k);
			array_copy(temp->a, R->a, R->wordLen);
			bi_delete(&Q);
			bi_delete(&R);
		}
	}
	bi_refine(temp);
	bi_new(out, temp->wordLen);
	bi_copy(*out, temp);
	bi_delete(&temp);
}

word bi_Mont_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out) {

	if (bi_is_zero(M) == TRUE) {
		return parameterError;
	}
	if (bi_is_zero(N) == TRUE) {
		bi_set_one(out);
		return operationSuccess;
	}

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* Q = NULL;
	bigint* R = NULL;
	bigint* temp = NULL;
	bigint* temp2 = NULL;
	int len = bi_L2R_get_bit_func(N->a[N->wordLen - 1]);


	bi_new(&t0, A->wordLen);
	bi_new(&t1, A->wordLen);
	t0->a[0] = 1;
	bi_copy(t1, A);
	for (int i = len - 1; i >= 0; i--) {

		bi_new(&temp, t0->wordLen + t1->wordLen + 1);

		if (((N->a[N->wordLen - 1] >> i) & 0x1) == 1) {

			//printf("MUL 전\n");
			//bi_show_hex(t0);
			//bi_show_hex(t1);
			bi_MUL(t0, t1, &temp);
			bi_refine(temp);
			//printf("MUL 후\n");
			//bi_show_hex(temp);

			bi_new(&Q, temp->wordLen);
			bi_new(&R, temp->wordLen);
			
			//printf("DIV 전 \n");
			//bi_show_hex(temp);
			bi_DIV(temp, M, &Q, &R);
			//printf("DIV 후\n");
			bi_refine(R);
			//bi_show_hex(R);
	
			bi_delete(&t0);
			bi_new(&t0, R->wordLen);
			array_copy(t0->a, R->a, R->wordLen);
			
			bi_delete(&Q);
			bi_delete(&R);
			bi_delete(&temp);

			bi_new(&temp, t1->wordLen * 2);
			array_copy(temp->a, t1->a, t1->wordLen);

			//printf("제곱 전\n");
			//bi_show_hex(temp);
			bi_SQU_AA(&temp);
			bi_refine(temp);
			//printf("제곱 후\n");
			//bi_show_hex(temp);

			bi_new(&Q, temp->wordLen);
			bi_new(&R, temp->wordLen);
			//printf("DIV 전 \n");
			//bi_show_hex(temp);
			bi_DIV(temp, M, &Q, &R);
			//printf("DIV 후\n");
			bi_refine(R);
			//bi_show_hex(R);
			
			bi_delete(&t1);
			bi_new(&t1, R->wordLen);
			array_copy(t1->a, R->a, R->wordLen);
		}
		else {

			//printf("MUL 전\n");
			//bi_show_hex(t0);
			//bi_show_hex(t1);
			bi_MUL(t0, t1, &temp);
			bi_refine(temp);
			//printf("MUL 후\n");
			//bi_show_hex(temp);

			bi_new(&Q, temp->wordLen);
			bi_new(&R, temp->wordLen);
	;
			//printf("DIV 전 \n");
			//bi_show_hex(temp);
			bi_DIV(temp, M, &Q, &R);
			//printf("DIV 후\n");
			bi_refine(R);
			//bi_show_hex(R);

			bi_delete(&t1);
			bi_new(&t1, R->wordLen);
			array_copy(t1->a, R->a, R->wordLen);
			
			bi_delete(&Q);
			bi_delete(&R);
			bi_delete(&temp);

			bi_new(&temp, t0->wordLen * 2);
			array_copy(temp->a, t0->a, t0->wordLen);

			//printf("제곱 전\n");
			//bi_show_hex(temp);
			bi_SQU_AA(&temp);
			bi_refine(temp);	
			//printf("제곱 후\n");
			//bi_show_hex(temp);

			bi_new(&Q, temp->wordLen);
			bi_new(&R, temp->wordLen);

			//printf("DIV 전 \n");
			//bi_show_hex(temp);
			bi_DIV(temp, M, &Q, &R);
			//printf("DIV 후\n");
			bi_refine(R);
			//bi_show_hex(R);

			bi_delete(&t0);
			bi_new(&t0, R->wordLen);
			array_copy(t0->a, R->a, R->wordLen);

		}
		bi_delete(&Q);
		bi_delete(&R);
		bi_delete(&temp);

	}

	for (int i = N->wordLen - 2; i >= 0; i--) {

		for (int j = WORD_BITLEN - 1; j >= 0; j--) {
			bi_new(&temp, t0->wordLen + t1->wordLen + 1);

			if ((N->a[i] >> j) & 0x1) {

				bi_MUL(t0, t1, &temp);
				bi_refine(temp);

				bi_new(&Q, temp->wordLen);
				bi_new(&R, temp->wordLen);

				bi_DIV(temp, M, &Q, &R);
				bi_refine(R);

				bi_delete(&t0);
				bi_new(&t0, R->wordLen);
				array_copy(t0->a, R->a, R->wordLen);

				bi_delete(&Q);
				bi_delete(&R);
				bi_delete(&temp);

				bi_new(&temp, t1->wordLen * 2);
				array_copy(temp->a, t1->a, t1->wordLen);

				bi_SQU_AA(&temp);
				bi_refine(temp);

				bi_new(&Q, temp->wordLen);
				bi_new(&R, temp->wordLen);
				bi_DIV(temp, M, &Q, &R);
				bi_refine(R);

				bi_delete(&t1);
				bi_new(&t1, R->wordLen);
				array_copy(t1->a, R->a, R->wordLen);
			}
			else {

				bi_MUL(t0, t1, &temp);
				bi_refine(temp);

				bi_new(&Q, temp->wordLen);
				bi_new(&R, temp->wordLen);
				;
				bi_DIV(temp, M, &Q, &R);
				bi_refine(R);

				bi_delete(&t1);
				bi_new(&t1, R->wordLen);
				array_copy(t1->a, R->a, R->wordLen);

				bi_delete(&Q);
				bi_delete(&R);
				bi_delete(&temp);

				bi_new(&temp, t0->wordLen * 2);
				array_copy(temp->a, t0->a, t0->wordLen);

				bi_SQU_AA(&temp);
				bi_refine(temp);

				bi_new(&Q, temp->wordLen);
				bi_new(&R, temp->wordLen);
				bi_DIV(temp, M, &Q, &R);
				bi_refine(R);

				bi_delete(&t0);
				bi_new(&t0, temp->wordLen);
				array_copy(t0->a, R->a, R->wordLen);

			}
			bi_delete(&Q);
			bi_delete(&R);
			bi_delete(&temp);

		}
	}
	bi_refine(t0);
	bi_new(out, t0->wordLen);
	bi_copy(*out, t0);
	bi_delete(&t0);
	bi_delete(&t1);
}
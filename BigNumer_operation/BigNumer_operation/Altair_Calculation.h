#pragma once
#include "type.h"

/**
 * @brief      single-precision addition �Լ�
 * @param      word		A			word�� ������ A
 * @param      word		B			word�� ������ B
 * @param      int		c			carry ��
 * @param      word*	C			A+B+c�� ������ word�� �迭
 * @return     A+B+c�� ����ϸ鼭 �߻��� carry��
*/
int bi_ADDS(IN word A, IN word B, IN int c, OUT word* C);
/**
 * @brief		multi-precision addition �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		wordLen(A) >= wordLen(B)
 * @param		sign(A) = sign(B)
 * @param		bigint**	C			���� ����� ������ ����ü, array
 * @return		�Լ� ���� ���
*/
int bi_ADDC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_ADD(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition �Լ�, A = A + B,	wordLen�� ���������ʴ´�.
 * @param		bigint**	A			������ ����ü �� ����� ������ ����ü
 * @param		bigint*		B			������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_ADD_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		addition �Լ�, A = A + B,	wordLen���� ����
 * @param		bigint**	A			������ ����ü �� ����� ������ ����ü
 * @param		bigint*		B			������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_ADD_AAB2(IN bigint** A, IN bigint* B);
/**
 * @brief      single-precision subtraction �Լ�
 * @param      word		A			word�� ������ A
 * @param      word		B			word�� ������ B
 * @param      int		b			borrow ��
 * @param      word*	C			A-B-c�� ������ word�� �迭
 * @return     A-B-c�� ����ϸ鼭 �߻��� borrow��
*/
int bi_SUBS(IN word A, IN word B, IN int b, OUT word* C);
/**
 * @brief		multi-precision subtraction �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		A>=B>0
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SUBC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SUB(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction �Լ�,	refine�� ������ �ʴ´�.
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SUB_noRefine(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction �Լ�, A = A - B,		wordLen�� ���������ʴ´�.
 * @param		bigint**	A			�E���� ����ü �� ����� ������ ����ü
 * @param		bigint*		B			������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SUB_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		single-precision multiplication �Լ�
 * @param		word		A			word�� ������ A
 * @param		word		B			word�� ������ B
 * @param		A,B in [0,W)
 * @param		word*		C			A*B�� ������ word�� �迭
 * @return		�Լ� ���� ���
*/
int bi_MULS(IN word A, IN word B, OUT word* C);
/**
 * @brief		multi-precision schoolbook multiplication �Լ�
 * @param		bigint*		A			������ ����ü, A in [W^(n-1), W^n)
 * @param		bigint*		B			������ ����ü, B in [W^(m-1), W^m)
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_MULC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		multiplication �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_MUL(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		multiplication �Լ�, A = A*B
 * @param		bigint**	A			���� �� ������ ����ü
 * @param		bigint*		B			������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_MUL_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		multiplication Karatsuba �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_MUL_Karatsuba(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		single-precision squaring �Լ�
 * @param		word		A			word�� ������ A
 * @param		A in (-W,W)
 * @param		word*		C			A^2�� ������ word�� �迭
 * @return		�Լ� ���� ���
*/
int bi_SQUS(IN word A, OUT word* C);
/**
 * @brief		multi-precision schoolbook squaring �Լ�
 * @param		bigint*		A			������ ����ü, A in [W^(n-1), W^n)
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SQUC(IN bigint* A, OUT bigint** C);
/**
 * @brief		squaring �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SQU(IN bigint* A, OUT bigint** C);
/**
 * @brief		squaring �Լ�, A = A^2
 * @param		bigint**		A		���꿡 ����� ����ü
 * @return		�Լ� ���� ���
*/
int bi_SQU_AA(IN OUT bigint** A);
/**
 * @brief		squaring Karatsuba �Լ�
 * @param		bigint*		A			������ ����ü
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int bi_SQU_Karatsuba(IN bigint* A, OUT bigint** C);

int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);

/**
 * @brief		binary division �Լ�,	A = B*Q + R
 * @param		bigint*		A			�������� ��
 * @param		bigint*		B			A�� ���� ��
 * @param		bigint**	Q			��, �Լ��Է����� ũ�⸦ �Ҵ����־��Ѵ�.
 * @param		bigint**	R			������, �Լ��Է����� ũ�⸦ �Ҵ����־��Ѵ�.
 * @return		�Լ� ���� ���
*/
int bi_DIV_BINARY(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
 * @brief		C = floor(A/B)
 * @param		word*		A			�������� ��
 * @param		word		B			A�� ���� ��
 * @param		word*		C			A/B�� ���� ����
 * @return		�Լ� ���� ���
*/
int bi_2WORD_DIV_1WORD(IN word* A, IN word B, OUT word* C);
/**
 * @brief		DIV core ���� core
 * @param		bigint*		A			�������� ��
 * @param		bigint*		B			������ ��
 * @param		bigint**	Q			��
 * @param		bigint**	R			������
 * @return		�Լ� ���� ���
*/
int bi_DIVCC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
* @brief		DIV core
* @param		bigint* A			�������� ��
* @param		bigint* B			������ ��
* @param		bigint** Q			��
* @param		bigint** R			������
* @return		�Լ� ���� ���
*/
int bi_DIVC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
* @brief		DIV�� �����Լ�, R<-RW+Ai�� ó��
* @param		bigint** R			�������� ��
* @param		word A				RW�� ���� ������ index word�� ������ ��
* @return		�Լ� ���� ���
*/
int bi_DIV_func(IN bigint** R, word A);
/**
* @brief		DIV
* @param		bigint* A			�������� ��
* @param		bigint* B			������ ��
* @param		bigint** Q			��
* @param		bigint** R			������
* @return		�Լ� ���� ���
*/
int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
//	int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);		//ȣ���� divison
/**
 * @brief		Left to Right ������ �Լ�, C = A^n
 * @param		bigint*		A			�������� ����ü
 * @param		bigint*		n			�������� ��ŭ ����
 * @param		bigint**	C			������ ����� ������ ����ü
 * @return		�Լ� ���� ���
*/

/**
* @brief		Left to Right ����� Modular Exponentitation ����
* @param		bigint* A		������ ������ ��(x^n �� x)
* @param		bitint* N		������ ������ ����(x^n �� n)
* @param		bigint* M		Modular ������ �� M ��
* @param		bigint* out		��� ���� ������ ����ü
* @return		�Լ� ���� ���
*/
word bi_L2R_BINARY_allocation(IN bigint* A, IN bigint* N, OUT bigint** out);
word bi_L2R_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out);
word bi_Mont_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out);
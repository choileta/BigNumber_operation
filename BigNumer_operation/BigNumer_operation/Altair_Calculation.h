#pragma once
#include "type.h"

/**
 * @brief      single-precision addition 함수
 * @param      word		A			word형 데이터 A
 * @param      word		B			word형 데이터 B
 * @param      int		c			carry 값
 * @param      word*	C			A+B+c를 저장할 word형 배열
 * @return     A+B+c를 계산하면서 발생한 carry값
*/
int bi_ADDS(IN word A, IN word B, IN int c, OUT word* C);
/**
 * @brief		multi-precision addition 함수
 * @param		bigint*		A			덧셈할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @param		wordLen(A) >= wordLen(B)
 * @param		sign(A) = sign(B)
 * @param		bigint**	C			덧셈 결과를 저장할 구조체, array
 * @return		함수 실행 결과
*/
int bi_ADDC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition 함수
 * @param		bigint*		A			덧셈할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @param		bigint**	C			덧셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition 함수, A = A + B,	wordLen은 복사하지않는다.
 * @param		bigint**	A			덧셈할 구조체 및 결과를 저장할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		addition 함수, A = A + B,	wordLen까지 복사
 * @param		bigint**	A			덧셈할 구조체 및 결과를 저장할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @return		함수 실행 결과
*/
int bi_ADD_AAB2(IN bigint** A, IN bigint* B);
/**
 * @brief      single-precision subtraction 함수
 * @param      word		A			word형 데이터 A
 * @param      word		B			word형 데이터 B
 * @param      int		b			borrow 값
 * @param      word*	C			A-B-c를 저장할 word형 배열
 * @return     A-B-c를 계산하면서 발생한 borrow값
*/
int bi_SUBS(IN word A, IN word B, IN int b, OUT word* C);
/**
 * @brief		multi-precision subtraction 함수
 * @param		bigint*		A			뺄셈할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @param		A>=B>0
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SUBC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction 함수
 * @param		bigint*		A			뺄셈할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SUB(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction 함수,	refine을 해주지 않는다.
 * @param		bigint*		A			뺄셈할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SUB_noRefine(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction 함수, A = A - B,		wordLen은 복사하지않는다.
 * @param		bigint**	A			뺼셈할 구조체 및 결과를 저장할 구조체
 * @param		bigint*		B			뺄셈할 구조체
 * @return		함수 실행 결과
*/
int bi_SUB_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		single-precision multiplication 함수
 * @param		word		A			word형 데이터 A
 * @param		word		B			word형 데이터 B
 * @param		A,B in [0,W)
 * @param		word*		C			A*B를 저장할 word형 배열
 * @return		함수 실행 결과
*/
int bi_MULS(IN word A, IN word B, OUT word* C);
/**
 * @brief		multi-precision schoolbook multiplication 함수
 * @param		bigint*		A			곱셈할 구조체, A in [W^(n-1), W^n)
 * @param		bigint*		B			곱셈할 구조체, B in [W^(m-1), W^m)
 * @param		bigint**	C			곱셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_MULC(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		multiplication 함수
 * @param		bigint*		A			곱셈할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @param		bigint**	C			곱셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		multiplication 함수, A = A*B
 * @param		bigint**	A			곱셈 및 저장할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL_AAB(IN bigint** A, IN bigint* B);
/**
 * @brief		multiplication Karatsuba 함수
 * @param		bigint*		A			곱셈할 구조체
 * @param		bigint*		B			곱셈할 구조체
 * @param		bigint**	C			곱셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_MUL_Karatsuba(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		single-precision squaring 함수
 * @param		word		A			word형 데이터 A
 * @param		A in (-W,W)
 * @param		word*		C			A^2를 저장할 word형 배열
 * @return		함수 실행 결과
*/
int bi_SQUS(IN word A, OUT word* C);
/**
 * @brief		multi-precision schoolbook squaring 함수
 * @param		bigint*		A			제곱할 구조체, A in [W^(n-1), W^n)
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQUC(IN bigint* A, OUT bigint** C);
/**
 * @brief		squaring 함수
 * @param		bigint*		A			제곱할 구조체
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU(IN bigint* A, OUT bigint** C);
/**
 * @brief		squaring 함수, A = A^2
 * @param		bigint**		A		연산에 사용할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU_AA(IN OUT bigint** A);
/**
 * @brief		squaring Karatsuba 함수
 * @param		bigint*		A			제곱할 구조체
 * @param		bigint**	C			제곱 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int bi_SQU_Karatsuba(IN bigint* A, OUT bigint** C);

int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);

/**
 * @brief		binary division 함수,	A = B*Q + R
 * @param		bigint*		A			나눠지는 수
 * @param		bigint*		B			A를 나눌 수
 * @param		bigint**	Q			몫, 함수입력전에 크기를 할당해주어한다.
 * @param		bigint**	R			나머지, 함수입력전에 크기를 할당해주어한다.
 * @return		함수 실행 결과
*/
int bi_DIV_BINARY(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
 * @brief		C = floor(A/B)
 * @param		word*		A			나눠지는 수
 * @param		word		B			A를 나눌 수
 * @param		word*		C			A/B의 몫을 저장
 * @return		함수 실행 결과
*/
int bi_2WORD_DIV_1WORD(IN word* A, IN word B, OUT word* C);
/**
 * @brief		DIV core 속의 core
 * @param		bigint*		A			나눠지는 수
 * @param		bigint*		B			나누는 수
 * @param		bigint**	Q			몫
 * @param		bigint**	R			나머지
 * @return		함수 실행 결과
*/
int bi_DIVCC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
* @brief		DIV core
* @param		bigint* A			나눠지는 수
* @param		bigint* B			나누는 수
* @param		bigint** Q			몫
* @param		bigint** R			나머지
* @return		함수 실행 결과
*/
int bi_DIVC(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
/**
* @brief		DIV의 내부함수, R<-RW+Ai를 처리
* @param		bigint** R			나눠지는 수
* @param		word A				RW의 가장 최하위 index word에 더해질 값
* @return		함수 실행 결과
*/
int bi_DIV_func(IN bigint** R, word A);
/**
* @brief		DIV
* @param		bigint* A			나눠지는 수
* @param		bigint* B			나누는 수
* @param		bigint** Q			몫
* @param		bigint** R			나머지
* @return		함수 실행 결과
*/
int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);
//	int bi_DIV(IN bigint* A, IN bigint* B, OUT bigint** Q, OUT bigint** R);		//호진이 divison
/**
 * @brief		Left to Right 지수승 함수, C = A^n
 * @param		bigint*		A			지수승할 구조체
 * @param		bigint*		n			지수승을 얼만큼 할지
 * @param		bigint**	C			지수승 결과를 저장할 구조체
 * @return		함수 실행 결과
*/

/**
* @brief		Left to Right 방법의 Modular Exponentitation 연산
* @param		bigint* A		지수승 연산의 밑(x^n 의 x)
* @param		bitint* N		지수승 연산의 지수(x^n 의 n)
* @param		bigint* M		Modular 연산을 할 M 값
* @param		bigint* out		결과 값을 저장할 구조체
* @return		함수 실행 결과
*/
word bi_L2R_BINARY_allocation(IN bigint* A, IN bigint* N, OUT bigint** out);
word bi_L2R_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out);
word bi_Mont_EXP(IN bigint* A, IN bigint* N, IN bigint* M, OUT bigint** out);
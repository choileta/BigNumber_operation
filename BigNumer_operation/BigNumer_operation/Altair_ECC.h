#pragma once
#include "type.h"

int div2(IN bigint** A);
int divN(IN bigint** A, IN int num);
int cmpData(word* A, word* B);
//A에 B를 복사
void ECC_Copy(ECC_bigint* A, ECC_bigint* B);
void ECC_new(ECC_bigint** A, int wordLen);
void ECC_delete(ECC_bigint** A);

//큰 정수 연산
int ADD(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition in field with NIST prime p256
 * @param		bigint*		A			덧셈할 구조체
 * @param		bigint*		B			덧셈할 구조체
 * @param		bigint*		p			감산을 수행할 소수
 * @param		bigint**	C			덧셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int ADD_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C);
int SUB(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction in field with NIST prime p256
 * @param		bigint*		A			뺄셈할 구조체, 빼지는 값
 * @param		bigint*		B			뺄셈할 구조체, 빼는 값
 * @param		bigint*		p			감산을 수행할 소수
 * @param		bigint**	C			뺄셈 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int SUB_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C);
int MUL_OS(IN bigint* A, IN bigint* B, OUT bigint** C);
int MUL_PS(IN bigint* A, IN bigint* B, OUT bigint** C);
int FastReduction(IN bigint* A, IN bigint* p, OUT bigint** C);

//역원
/**
 * @brief		fastReduction modulo p256	//	32bit만 지원
 * @param		bigint*		A			감산을 실행할 값을 갖는 구조체
 * @param		bigint*		p			감산을 수행할 소수
 * @param		bigint**	C			감산 결과를 저장할 구조체
 * @return		함수 실행 결과
*/
int INVERSION_FermatLT(IN bigint* A, IN bigint* p, OUT bigint* invA);
/**
 * @brief		Inversion 함수, 페르마의 작은 정리를 이용한
 * @param		bigint*		A			역원을 구할 값
 * @param		bigint*		p			감산에 사용할 소수
 * @param		bigint**	invA		A의 역원을 저장할 구조체
 * @return		함수 실행 결과
*/
int INVERSION_Binary(IN bigint* A, IN bigint* p, OUT bigint** invA);

//타원곡선연산
int ECC_ADD(IN ECC_bigint* P, IN ECC_bigint* Q, IN bigint* p, OUT ECC_bigint** R);
int ECC_DBL(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R);
int ECC_MOD_EXP_L2R(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP);
int ECC_MOD_EXP_R2L(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP);

//자코비안
int ECC_Jacobian_ADD(IN ECC_bigint* P, IN ECC_bigint* Q, IN bigint* p, OUT ECC_bigint** R);
int ECC_Jacobian_DBL(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R);
int ECC_Jac2Aff(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R);
int ECC_Jacobian_MOD_EXP_L2R(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP);

//wNAF
int ECC_SUB_CHAR(IN bigint* A, IN char B, OUT bigint** C);
int ECC_ADD_WORD(IN bigint* A, IN word B, OUT bigint** C);
int ECC_wNAF(IN bigint* k, IN int w, OUT char* wNAFk);
int ECC_wNAF_makeTable(IN ECC_bigint* P, IN int w, IN bigint* p, OUT ECC_bigint* table[]);
int ECC_wNAF_SCALAR_MUL(IN ECC_bigint* table[], IN char* wNAFk, IN bigint* p, OUT ECC_bigint** kP);

//fixedWindow
int ECC_Recode_Signed(IN bigint* k, IN int w, OUT char* k2);
int ECC_Precomputation_SignedWindow(IN bigint* P, IN int w, IN bigint* p, OUT bigint** table);
int ECC_FixedWindow_SCALAR_MUL(IN bigint** table, IN char* k2, IN int w, IN bigint* p, OUT bigint** kP);

//comb method
int ECC_Precomputation_Comb(IN bigint* P, IN int w, IN int num, IN bigint* p, OUT bigint** precomputationP);
int ECC_combPadding(IN bigint* k, IN int w, OUT char* combK[]);
int ECC_comb_SCALAR_MUL(IN bigint* k, IN bigint* precomputationP, IN bigint* p, IN char* combK[], IN int w, OUT bigint** kP);
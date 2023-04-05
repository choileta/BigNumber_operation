#pragma once
#include "type.h"

int div2(IN bigint** A);
int divN(IN bigint** A, IN int num);
int cmpData(word* A, word* B);
//A�� B�� ����
void ECC_Copy(ECC_bigint* A, ECC_bigint* B);
void ECC_new(ECC_bigint** A, int wordLen);
void ECC_delete(ECC_bigint** A);

//ū ���� ����
int ADD(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		addition in field with NIST prime p256
 * @param		bigint*		A			������ ����ü
 * @param		bigint*		B			������ ����ü
 * @param		bigint*		p			������ ������ �Ҽ�
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int ADD_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C);
int SUB(IN bigint* A, IN bigint* B, OUT bigint** C);
/**
 * @brief		subtraction in field with NIST prime p256
 * @param		bigint*		A			������ ����ü, ������ ��
 * @param		bigint*		B			������ ����ü, ���� ��
 * @param		bigint*		p			������ ������ �Ҽ�
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int SUB_FP(IN bigint* A, IN bigint* B, IN bigint* p, OUT bigint** C);
int MUL_OS(IN bigint* A, IN bigint* B, OUT bigint** C);
int MUL_PS(IN bigint* A, IN bigint* B, OUT bigint** C);
int FastReduction(IN bigint* A, IN bigint* p, OUT bigint** C);

//����
/**
 * @brief		fastReduction modulo p256	//	32bit�� ����
 * @param		bigint*		A			������ ������ ���� ���� ����ü
 * @param		bigint*		p			������ ������ �Ҽ�
 * @param		bigint**	C			���� ����� ������ ����ü
 * @return		�Լ� ���� ���
*/
int INVERSION_FermatLT(IN bigint* A, IN bigint* p, OUT bigint* invA);
/**
 * @brief		Inversion �Լ�, �丣���� ���� ������ �̿���
 * @param		bigint*		A			������ ���� ��
 * @param		bigint*		p			���꿡 ����� �Ҽ�
 * @param		bigint**	invA		A�� ������ ������ ����ü
 * @return		�Լ� ���� ���
*/
int INVERSION_Binary(IN bigint* A, IN bigint* p, OUT bigint** invA);

//Ÿ�������
int ECC_ADD(IN ECC_bigint* P, IN ECC_bigint* Q, IN bigint* p, OUT ECC_bigint** R);
int ECC_DBL(IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** R);
int ECC_MOD_EXP_L2R(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP);
int ECC_MOD_EXP_R2L(IN bigint* k, IN ECC_bigint* P, IN bigint* p, OUT ECC_bigint** kP);

//���ں��
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
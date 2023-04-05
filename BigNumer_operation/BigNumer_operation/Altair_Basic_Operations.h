#pragma once
#include "type.h"

u8 getHex(u8 ch);
/**
 * @brief		string�� word�� �����ͷ� ����
 * @detail		from�� ���� index���� to�� ���� ���� index���� �ǵ��� ��ȯ
 * @param		u8* from			��ȯ�� string �迭�� �ּ�
 * @param		int size			��ȯ�� string�� ũ��, ������ ����
 * @param		word* to			��ȯ�� �����͸� ������ word�� �迭�� �ּ�
*/
void convertStr2Word(char* from, int size, word* to);
/**
 * @brief		���� �����Ϳ��� ���� ���� �о�� word�� �迭�� ����
 * @param		word* opA			�о�� �����͸� ��ȯ�Ͽ� ������ �迭
 * @param		FILE* fp			���� �о�� ���� ������
*/
void readData(word* opA, FILE* fp);
/**
 * @brief      ����ü �޸� �ݳ�
 * @param      bigint** x      �ݳ��� ����ü�� �ּ�
*/
void bi_delete(bigint** x);
/**
 * @brief      ����ü ����
 * @param      bigint** x      ������ ����ü�� �ּ�
 * @param      int wordLen      ����ü ��� ��, ������ Data�� Array �迭 ũ��
*/
void bi_new(bigint** x, int wordLen);
/**
 * @brief      ����ü �迭�� �Է� ������ ������ ����
 * @detail      �Է� �������� �ڷ����� ����ü�� �ڷ����� ������ ���(�Է� �����Ͱ� ����ü�� �ڷ����� ������ ���)
 * @param      bigint** x      ����ü�� �ּ�
 * @param      int sign      Data�� ��ȣ
 * @param      word* a         �Է� ������ ��
 * @param      int wordLen      �Է� �������� ����
*/
void bi_set_by_array(bigint** x, int sign, word* a, int wordLen);
/**
 * @brief      ����ü �迭�� �Է� ������ ������ ����
 * @detail      �Է� �������� �ڷ����� string�� ���
 * @param      bigint** x      ����ü�� �ּ�
 * @param      int sign      Data�� ��ȣ
 * @param      char* str      �Է� ������ ��
 * @param      word base      �Է� �������� ǥ����(2����, 16����)
*/
void bi_set_by_string(bigint** x, int sign, char* str, word base);
/**
 * @brief      ����ü�� �迭 ���� 16������ ���
 * @param      bigint* x      ����� �������� ����ü
*/
void bi_show_hex(bigint* x);

//�̿ϼ�
void bi_show_dec(bigint* x);
/**
 * @brief      ����ü�� �迭 ���� 2������ ���
 * @param      bigint* x      ����� �������� ����ü
*/
void bi_show_bin(bigint* x);
/**
 * @brief      ����ü�� �迭 ���� �������� ���� 0�� ��� �ش� �޸𸮸� �ݳ��ϰ� �缳��
 * @param      bigint* x      �缳���� ����ü
*/
void bi_refine(bigint* x);
/**
 * @brief      �迭 ������ copy
 * @param      word* dst      copy�Ͽ� ������ �迭
 * @param      word* src      copy�� ������
 * @param      int len         copy�� �������� ����
*/
void array_copy(word* dst, word* src, int len);
/**
 * @brief      bigint ����ü copy
 * @param      bigint* dst      copy�Ͽ� ������ �迭
 * @param      bigint* src      copy�� ������
*/
void bi_copy(bigint* dst, bigint* src);
/**
 * @brief      ����ü ������ copy
 * @param      bigint** y      copy�Ͽ� ���� ������ ����ü �ּ�
 * @param      bigint** x      copy�� ���� ����� ����ü
*/
void bi_assign(bigint** y, bigint* x);
/**
 * @brief      ���� ������ ����
 * @param      word* dst      ���� ���� ������ Array
 * @param      int wordLen      ������ Array�� ����
*/
void array_rand(word* dst, int wordLen);
/**
 * @brief      ���� ���� ������ ����ü ����
 * @param      bigint** x      ���� ������ ����ü
 * @param      int sign      ���� ��ȣ
 * @param      int wordLen      ������ ���� ����
*/
void bi_gen_rand(bigint** x, int sign, int wordLen);
/**
 * @brief      ����ü Array�� wordLen���� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @return      x->wordLen      Array�� ����
*/
int bi_get_word_length(bigint* x);
/**
 * @brief      ����ü Array�� bit���� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @return      Array�� bit ����
*/
int bi_get_bit_length(bigint* x);
/**
 * @brief      �ִ� 32bit�� ���� ������ n�� bit ���̸� ����ϴ� �Լ�
 * @param      u32 n		��Ʈ ���̰� �ñ��� n, unsigned int ��
 * @return     n�� ��Ʈ ����
*/
int get_bit_length(u32 n);
/**
 * @brief      ����ü Array�� j��° bit �� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @param      int j         return�� Array�� ����ü
 * @return      Array�� j��° bit ����
*/
int bi_get_jth_bit(bigint* x, int j);
/**
 * @brief      ����ü Data�� ��ȣ return �Լ�
 * @param      bigint*   x      return�� sign�� ����ü
 * @return      x->sign         ����ü�� sign ��, ����� 0 ������ 1
*/
int bi_get_sign(bigint* x);
/**
 * @brief      ����ü Data�� ��ȣ�� flip�ϴ� �Լ�
 * @param      bigint*   x      flip�� sign�� ����ü
*/
void bi_flip_sign(bigint** x);
/**
 * @brief      ����ü Data�� 1�� �����ϴ� �Լ�
 * @param      bigint*   x      ������ ����ü�� �ּ�
*/
void bi_set_one(bigint** x);
/**
 * @brief      ����ü Data�� 0���� �����ϴ� �Լ�
 * @param      bigint*   x      ������ ����ü�� �ּ�
*/
void bi_set_zero(bigint** x);
/**
 * @brief      ����ü Data�� 0���� Ȯ���ϴ� �Լ�
 * @param      bigint*   x      Ȯ���� ����ü
 * @return      TRUE         ����ü�� Data�� 0�� ���
 * @return      FALSE         ����ü�� Data�� 0�� �ƴ� ���
 */
int bi_is_zero(bigint* x);
/**
 * @brief      ����ü Data�� 1���� Ȯ���ϴ� �Լ�
 * @param      bigint*   x      Ȯ���� ����ü
 * @return      TRUE         ����ü�� Data�� 1�� ���
 * @return      FALSE         ����ü�� Data�� 1�� �ƴ� ���
 */
int bi_is_one(bigint* x);
/**
 * @brief      �� ���� ����ü Data�� ABS�� ���ϴ� �Լ�
 * @param      bigint*   A      Ȯ���� ����ü
 * @param      bigint*   B      Ȯ���� ����ü
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1            A<B
 */
int bi_compareABS(bigint* A, bigint* B);
/**
 * @brief      �� ���� ����ü Data�� ũ�⸦ ���ϴ� �Լ�
 * @param      bigint*   A      Ȯ���� ����ü
 * @param      bigint*   B      Ȯ���� ����ü
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1            A<B
 */
int bi_compare(bigint* A, bigint* B);
/**
 * @brief      ����ü �����͸� r-bit ��ŭ left_shift�ϴ� �Լ�
 * @param      bigint** A      Data�� ���� ����ü�� �ּ�
 * @param      int r         left_shifting�� ũ��
 */
void bi_left_shift(bigint** A, int r);
/**
 * @brief      ����ü �����͸� r-bit ��ŭ right_shift�ϴ� �Լ�
 * @param      bigint** A      Data�� ���� ����ü�� �ּ�
 * @param      int r         right_shifting�� ũ��
 */
void bi_right_shift(bigint** A, int r);
/**
 * @brief      ����ü �����͸� r-bit ��ŭ reduction �ϴ� �Լ�
 * @param      bigint** A      Data�� ���� ����ü�� �ּ�
 * @param      int r         reduction �� ũ��
 */
void bi_reduction(bigint** A, int r);
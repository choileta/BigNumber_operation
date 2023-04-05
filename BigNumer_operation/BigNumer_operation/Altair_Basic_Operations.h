#pragma once
#include "type.h"

u8 getHex(u8 ch);
/**
 * @brief		string을 word형 데이터로 변형
 * @detail		from의 낮은 index값이 to의 가장 높은 index값이 되도록 변환
 * @param		u8* from			변환할 string 배열의 주소
 * @param		int size			변환할 string의 크기, 문자의 개수
 * @param		word* to			변환한 데이터를 저장할 word형 배열의 주소
*/
void convertStr2Word(char* from, int size, word* to);
/**
 * @brief		파일 포인터에서 값을 한줄 읽어와 word형 배열에 저장
 * @param		word* opA			읽어온 데이터를 변환하여 저장할 배열
 * @param		FILE* fp			값을 읽어올 파일 포인터
*/
void readData(word* opA, FILE* fp);
/**
 * @brief      구조체 메모리 반납
 * @param      bigint** x      반납할 구조체의 주소
*/
void bi_delete(bigint** x);
/**
 * @brief      구조체 생성
 * @param      bigint** x      생성할 구조체의 주소
 * @param      int wordLen      구조체 멤버 중, 저장할 Data의 Array 배열 크기
*/
void bi_new(bigint** x, int wordLen);
/**
 * @brief      구조체 배열을 입력 데이터 값으로 설정
 * @detail      입력 데이터의 자료형이 구조체의 자료형과 동일한 경우(입력 데이터가 구조체의 자료형과 동일한 경우)
 * @param      bigint** x      구조체의 주소
 * @param      int sign      Data의 부호
 * @param      word* a         입력 데이터 값
 * @param      int wordLen      입력 데이터의 길이
*/
void bi_set_by_array(bigint** x, int sign, word* a, int wordLen);
/**
 * @brief      구조체 배열을 입력 데이터 값으로 설정
 * @detail      입력 데이터의 자료형이 string인 경우
 * @param      bigint** x      구조체의 주소
 * @param      int sign      Data의 부호
 * @param      char* str      입력 데이터 값
 * @param      word base      입력 데이터의 표현법(2진수, 16진수)
*/
void bi_set_by_string(bigint** x, int sign, char* str, word base);
/**
 * @brief      구조체의 배열 값을 16진수로 출력
 * @param      bigint* x      출력할 데이터의 구조체
*/
void bi_show_hex(bigint* x);

//미완성
void bi_show_dec(bigint* x);
/**
 * @brief      구조체의 배열 값을 2진수로 출력
 * @param      bigint* x      출력할 데이터의 구조체
*/
void bi_show_bin(bigint* x);
/**
 * @brief      구조체의 배열 상위 데이터의 값이 0인 경우 해당 메모리를 반납하고 재설정
 * @param      bigint* x      재설정할 구조체
*/
void bi_refine(bigint* x);
/**
 * @brief      배열 데이터 copy
 * @param      word* dst      copy하여 저장할 배열
 * @param      word* src      copy할 데이터
 * @param      int len         copy할 데이터의 길이
*/
void array_copy(word* dst, word* src, int len);
/**
 * @brief      bigint 구조체 copy
 * @param      bigint* dst      copy하여 저장할 배열
 * @param      bigint* src      copy할 데이터
*/
void bi_copy(bigint* dst, bigint* src);
/**
 * @brief      구조체 데이터 copy
 * @param      bigint** y      copy하여 값을 저장할 구조체 주소
 * @param      bigint** x      copy할 값이 저장된 구조체
*/
void bi_assign(bigint** y, bigint* x);
/**
 * @brief      난수 데이터 설정
 * @param      word* dst      난수 값을 저장할 Array
 * @param      int wordLen      저장할 Array의 길이
*/
void array_rand(word* dst, int wordLen);
/**
 * @brief      난수 값을 가지는 구조체 생성
 * @param      bigint** x      값을 저장할 구조체
 * @param      int sign      값의 부호
 * @param      int wordLen      생성할 난수 길이
*/
void bi_gen_rand(bigint** x, int sign, int wordLen);
/**
 * @brief      구조체 Array의 wordLen길이 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @return      x->wordLen      Array의 길이
*/
int bi_get_word_length(bigint* x);
/**
 * @brief      구조체 Array의 bit길이 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @return      Array의 bit 길이
*/
int bi_get_bit_length(bigint* x);
/**
 * @brief      최대 32bit를 갖는 데이터 n의 bit 길이를 출력하는 함수
 * @param      u32 n		비트 길이가 궁금한 n, unsigned int 형
 * @return     n의 비트 길이
*/
int get_bit_length(u32 n);
/**
 * @brief      구조체 Array의 j번째 bit 값 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @param      int j         return할 Array의 구조체
 * @return      Array의 j번째 bit 길이
*/
int bi_get_jth_bit(bigint* x, int j);
/**
 * @brief      구조체 Data의 부호 return 함수
 * @param      bigint*   x      return할 sign의 구조체
 * @return      x->sign         구조체의 sign 값, 양수면 0 음수면 1
*/
int bi_get_sign(bigint* x);
/**
 * @brief      구조체 Data의 부호를 flip하는 함수
 * @param      bigint*   x      flip할 sign의 구조체
*/
void bi_flip_sign(bigint** x);
/**
 * @brief      구조체 Data를 1로 설정하는 함수
 * @param      bigint*   x      설정할 구조체의 주소
*/
void bi_set_one(bigint** x);
/**
 * @brief      구조체 Data를 0으로 설정하는 함수
 * @param      bigint*   x      설정할 구조체의 주소
*/
void bi_set_zero(bigint** x);
/**
 * @brief      구조체 Data가 0인지 확인하는 함수
 * @param      bigint*   x      확인할 구조체
 * @return      TRUE         구조체의 Data가 0인 경우
 * @return      FALSE         구조체의 Data가 0이 아닌 경우
 */
int bi_is_zero(bigint* x);
/**
 * @brief      구조체 Data가 1인지 확인하는 함수
 * @param      bigint*   x      확인할 구조체
 * @return      TRUE         구조체의 Data가 1인 경우
 * @return      FALSE         구조체의 Data가 1이 아닌 경우
 */
int bi_is_one(bigint* x);
/**
 * @brief      두 개의 구조체 Data의 ABS를 비교하는 함수
 * @param      bigint*   A      확인할 구조체
 * @param      bigint*   B      확인할 구조체
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1            A<B
 */
int bi_compareABS(bigint* A, bigint* B);
/**
 * @brief      두 개의 구조체 Data의 크기를 비교하는 함수
 * @param      bigint*   A      확인할 구조체
 * @param      bigint*   B      확인할 구조체
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1            A<B
 */
int bi_compare(bigint* A, bigint* B);
/**
 * @brief      구조체 데이터를 r-bit 만큼 left_shift하는 함수
 * @param      bigint** A      Data가 속한 구조체의 주소
 * @param      int r         left_shifting할 크기
 */
void bi_left_shift(bigint** A, int r);
/**
 * @brief      구조체 데이터를 r-bit 만큼 right_shift하는 함수
 * @param      bigint** A      Data가 속한 구조체의 주소
 * @param      int r         right_shifting할 크기
 */
void bi_right_shift(bigint** A, int r);
/**
 * @brief      구조체 데이터를 r-bit 만큼 reduction 하는 함수
 * @param      bigint** A      Data가 속한 구조체의 주소
 * @param      int r         reduction 할 크기
 */
void bi_reduction(bigint** A, int r);
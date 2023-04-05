#include "Altair_Basic_Operations.h"

u8 getHex(u8 ch)
{
    u8 hex = 0;

    if (ch >= '0' && ch <= '9')
    {
        hex = ch - '0';
    }
    else if (ch >= 'a' && ch <= 'f')
    {
        hex = ch - 'a' + 10;
    }
    else if (ch >= 'A' && ch <= 'F')
    {
        hex = ch - 'A' + 10;
    }

    return hex;
}

#if WORD_BITLEN == 8
void convertStr2Word(char* from, int size, word* to)
{
    int cnt_i = 0;
    int cnt_j = (int)ceil((double)size / 2) - 1;
    int ch = 0;
    
    if (size & 0x1)
    {
        ch = from[cnt_i++];
        to[cnt_j--] = getHex(ch);
    }

    for (; cnt_i < size; cnt_i += 2, cnt_j--)
    {
        ch = from[cnt_i];

        to[cnt_j] = getHex(ch);
        to[cnt_j] <<= 4;

        ch = from[cnt_i + 1];
        to[cnt_j] |= getHex(ch);
    }
}
#elif WORD_BITLEN == 32
void convertStr2Word(char* from, int size, word* to)
{
    int cnt_i = 0;
    int cnt_j = (int)ceil((double)size / 8) - 1;
    int ch = 0;
    int num = size & 0x7;

    if (num)
    {
        while (!(cnt_i == num))
        {
            ch = from[cnt_i++];

            to[cnt_j] <<= 4;
            to[cnt_j] |= getHex(ch);
        }
        cnt_j--;
    }

    for (; cnt_i < size; cnt_i += 8, cnt_j--)
    {
        ch = from[cnt_i];
        to[cnt_j] = getHex(ch);

        ch = from[cnt_i + 1];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 2];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 3];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 4];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 5];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 6];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 7];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);
    }
}
#else
void convertStr2Word(char* from, int size, word* to)
{
    int cnt_i = 0;
    int cnt_j = ceil((double)size / 16) - 1;
    int ch = 0;
    int num = size & 0xf;

    if (num)
    {
        while (!(cnt_i == num))
        {
            ch = from[cnt_i++];

            to[cnt_j] <<= 4;
            to[cnt_j] |= getHex(ch);
        }
        cnt_j--;
    }

    for (; cnt_i < size; cnt_i += 16, cnt_j--)
    {
        ch = from[cnt_i];
        to[cnt_j] = getHex(ch);

        ch = from[cnt_i + 1];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 2];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 3];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 4];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 5];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 6];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 7];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 8];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 9];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 10];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 11];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 12];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 13];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 14];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);

        ch = from[cnt_i + 15];
        to[cnt_j] <<= 4;
        to[cnt_j] |= getHex(ch);
    }
}
#endif
void readData(word* opA, FILE* fp)
{
    u8 buf[100];
    int i;

    fgets(buf, sizeof(buf), fp);
    convertStr2Word(buf, 64, opA);
}

/**
 * @brief      구조체 메모리 반납
 * @param      bigint** x      반납할 구조체의 주소
*/
void bi_delete(bigint** x)
{
    if (*x == NULL)
        return;
#ifdef ZERORIZE
    array_init((*x)->a, (*x)->wordLen);
#endif
    free((*x)->a);
    free(*x);
    *x = NULL;
}

/**
 * @brief      구조체 생성
 * @param      bigint** x      생성할 구조체의 주소
 * @param      int wordLen      구조체 멤버 중, 저장할 Data의 Array 배열 크기
*/
void bi_new(bigint** x, int wordLen)
{
    if (*x != NULL)
        bi_delete(x);
    *x = (bigint*)malloc(sizeof(bigint));
    if ((*x) == NULL)
        return;
    (*x)->sign = NON_NEGATIVE;
    (*x)->wordLen = wordLen;
    (*x)->a = (word*)calloc(wordLen, sizeof(word));
}

/**
 * @brief      구조체 배열을 입력 데이터 값으로 설정
 * @detail      입력 데이터의 자료형이 구조체의 자료형과 동일한 경우(입력 데이터가 구조체의 자료형과 동일한 경우)
 * @param      bigint** x      구조체의 주소
 * @param      int sign         Data의 부호
 * @param      word* a         입력 데이터 값
 * @param      int wordLen      입력 데이터의 길이
*/
void bi_set_by_array(bigint** x, int sign, word* a, int wordLen)
{
    if (*x != NULL)
        bi_delete(x);
    *x = (bigint*)malloc(sizeof(bigint));
    if ((*x) == NULL)
        return;

    (*x)->sign = sign;
    (*x)->wordLen = wordLen;
    (*x)->a = (word*)calloc(wordLen, sizeof(word));

    if ((*x)->a == NULL)
        return;

    int i;
    for (i = 0; i < wordLen; i++)
        (*x)->a[wordLen - i - 1] = a[i];
}


/**
 * @brief      구조체 배열을 입력 데이터 값으로 설정
 * @detail      입력 데이터의 자료형이 string인 경우
 * @param      bigint** x      구조체의 주소
 * @param      int sign      Data의 부호
 * @param      char* str      입력 데이터 값
 * @param      word base      입력 데이터의 표현법(2진수, 16진수)
*/
void bi_set_by_string(bigint** x, int sign, char* str, word base)
{
    if (*x == NULL)
        bi_delete(x);
    *x = (bigint*)malloc(sizeof(bigint));
    if ((*x) == NULL)
        return;
    if (base != 2 && base != 16)
        return;


    (*x)->sign = sign;

    int size = 0;
    int i = 0;
    int j = 0;
    int index = 1;
    int dst_index = 0;

    while (str[i++] != '\0') {
        size++;
    }

    int Q = size / WORD_BITLEN;
    int R = size % WORD_BITLEN;

    word temp = 0;


    if (base == 2)
    {
        (*x)->a = (word*)calloc((int)ceil((double)size / WORD_BITLEN), sizeof(word));
        (*x)->wordLen = (int)ceil((double)size / WORD_BITLEN);

        for (i = 0; i < Q; i++) {
            for (j = 0; j < WORD_BITLEN; j++)
                temp = temp + ((word)(str[size - index++] - 48) << j);

            (*x)->a[dst_index++] = temp;
            temp = 0;
        }

        if (R) {
            for (j = 0; j < R; j++) {
                temp = temp + ((word)(str[size - index++] - 48) << j);
            }
            (*x)->a[dst_index++] = temp;
        }

    }
    else if (base == 16)
    {
        (*x)->a = (word*)calloc((int)ceil(((double)size) / WORD_BITLEN), sizeof(word));
        if ((*x)->a == NULL)
            return;
        (*x)->wordLen = (int)ceil(((double)size * 4) / WORD_BITLEN);

        char* buffer = (char*)calloc(size, sizeof(char));
        if (buffer == NULL)
            return;

        memcpy(buffer, str, size);

        for (i = 0; i < size; i++) {
            if (islower(buffer[i]))
                buffer[i] = buffer[i] - 87;
            else if (isupper(buffer[i]))
                buffer[i] = buffer[i] - 55;
            else
                buffer[i] = buffer[i] - 48;
        }

        Q = size / (WORD_BITLEN / 4);
        R = size % (WORD_BITLEN / 4);

        for (i = 0; i < Q; i++) {
            for (j = 0; j < WORD_BITLEN / 4; j++) {
                temp = temp + ((word)(buffer[size - index++]) << (4 * j));
            }
            (*x)->a[dst_index++] = temp;
            temp = 0;
        }

        if (R) {
            for (j = 0; j < R; j++)
                temp = temp + ((word)(buffer[size - index++]) << (4 * j));
            (*x)->a[dst_index++] = temp;
        }

        free(buffer);
    }
}

/**
 * @brief      구조체의 배열 값을 16진수로 출력
 * @param      bigint* x      출력할 데이터의 구조체
*/
void bi_show_hex(bigint* x)
{
    int i;
    for (i = x->wordLen - 1; i >= 0; i--)
    {
#if   WORD_BITLEN == 64
        printf("%016llX", x->a[i]);
#elif WORD_BITLEN == 32
        printf("%08X", x->a[i]);
#else
        printf("%02X", x->a[i]);
#endif
    }
    printf("\n");
}

void bi_show_dec(bigint* x)   //미완성
{
    int i;
    for (i = x->wordLen - 1; i >= 0; i--)
    {
#if   WORD_BITLEN == 64
        printf("%016lld ", x->a[i]);
#elif WORD_BITLEN == 32
        printf("%08d ", x->a[i]);
#else
        printf("%02d ", x->a[i]);
#endif
    }
}

/**
 * @brief      구조체의 배열 값을 2진수로 출력
 * @param      bigint* x      출력할 데이터의 구조체
*/
void bi_show_bin(bigint* x)
{
    int i, j;
    for (i = x->wordLen - 1; i >= 0; i--)
    {
        for (j = WORD_BITLEN - 1; j >= 0; j--)
            printf("%d", ((int)((x->a[i]) >> j) & 0x1));
        printf(" ");
    }
    printf("\n");
}


/**
 * @brief      구조체의 배열 상위 데이터의 값이 0인 경우 해당 메모리를 반납하고 재설정
 * @param      bigint* x      재설정할 구조체
*/
void bi_refine(bigint* x)
{
    if (x == NULL)
        return;
    int new_wordLen = x->wordLen;
    while (new_wordLen > 1) // at least one word needed
    {
        if (x->a[new_wordLen - 1] != 0)
            break;
        new_wordLen--;
    }
    if (x->wordLen != new_wordLen)
    {
        x->wordLen = new_wordLen;
        if (x->a == NULL)
            return;
        word* tmp = x->a;
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordLen);
        if (x->a == NULL)
        {
            free(tmp);
            return;
            //x->a = tmp;
        }
    }
    if ((x->wordLen == 1) && (x->a[0] == 0x0))
        x->sign = NON_NEGATIVE;
}

/**
 * @brief      배열 데이터 copy
 * @param      word* dst      copy하여 저장할 배열
 * @param      word* src      copy할 데이터
 * @param      int len        copy할 데이터의 길이
*/
void array_copy(word* dst, word* src, int len)
{
    int i;
    for (i = 0; i < len; i++)
        dst[i] = src[i];
}
/**
 * @brief      bigint 구조체 copy
 * @param      bigint* dst      copy하여 저장할 배열
 * @param      bigint* src      copy할 데이터
*/
void bi_copy(bigint* dst, bigint* src)
{
    array_copy(dst->a, src->a, src->wordLen);
    dst->sign = src->sign;
    dst->wordLen = src->wordLen;
}

/**
 * @brief      구조체 데이터 copy
 * @param      bigint** y      copy하여 값을 저장할 구조체 주소
 * @param      bigint** x      copy할 값이 저장된 구조체
*/
void bi_assign(bigint** y, bigint* x)
{
    if (*y != NULL)
        bi_delete(y);
    bi_new(y, x->wordLen);
    (*y)->sign = x->sign;
    array_copy((*y)->a, x->a, x->wordLen);
}

/**
 * @brief      난수 데이터 설정
 * @param      word* dst      난수 값을 저장할 Array
 * @param      int wordLen      저장할 Array의 길이
*/
void array_rand(word* dst, int wordLen)
{
    u8* p = (u8*)dst;
    int cnt = wordLen * sizeof(word);
    while (cnt > 0)
    {
        *p = rand() & 0xff;
        p++;
        cnt--;
    }
}

/**
 * @brief      난수 값을 가지는 구조체 생성
 * @param      bigint** x      값을 저장할 구조체
 * @param      int sign      값의 부호
 * @param      int wordLen      생성할 난수 길이
*/
void bi_gen_rand(bigint** x, int sign, int wordLen)
{
    bi_new(x, wordLen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordLen);
    bi_refine(*x);
}

/**
 * @brief      구조체 Array의 wordLen길이 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @return      x->wordLen      Array의 길이
*/
int bi_get_word_length(bigint* x)
{
    return x->wordLen;
}

/**
 * @brief      구조체 Array의 bit길이 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @return      Array의 bit 길이
*/
int bi_get_bit_length(bigint* x)
{
    int cnt = 0;
    while (!((x->a[x->wordLen - 1] >> (WORD_BITLEN - 1 - cnt)) & 0x1))
    {
        cnt++;
    }
    return ((x->wordLen * WORD_BITLEN) - cnt);
}

/**
 * @brief      최대 32bit를 갖는 데이터 n의 bit 길이를 출력하는 함수
 * @param      u32 n		비트 길이가 궁금한 n, unsigned int 형
 * @return     n의 비트 길이
*/
int get_bit_length(u32 n)
{
    int cnt = 0;
    while ((n >> ++cnt));
    return cnt;
}

/**
 * @brief      구조체 Array의 j번째 bit 값 return 함수
 * @param      bigint*   x      return할 Array의 구조체
 * @param      int j         return할 Array의 구조체
 * @return      Array의 j번째 bit 길이
*/
int bi_get_jth_bit(bigint* x, int j)
{
    int q, r;
    q = j / WORD_BITLEN;
    r = j % WORD_BITLEN;
    return (((x->a[q]) >> r) & 0x1);
}

/**
 * @brief      구조체 Data의 부호 return 함수
 * @param      bigint*   x      return할 sign의 구조체
 * @return      x->sign         구조체의 sign 값, 양수면 0 음수면 1
*/
int bi_get_sign(bigint* x)
{
    return x->sign;
}

/**
 * @brief      구조체 Data의 부호를 flip하는 함수
 * @param      bigint*   x      flip할 sign의 구조체
*/
void bi_flip_sign(bigint** x)
{
    (*x)->sign = ((*x)->sign) ^ 0x1;
}

/**
 * @brief      구조체 Data를 1로 설정하는 함수
 * @param      bigint*   x      설정할 구조체의 주소
*/
void bi_set_one(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

/**
 * @brief      구조체 Data를 0으로 설정하는 함수
 * @param      bigint*   x      설정할 구조체의 주소
*/
void bi_set_zero(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

/**
 * @brief      구조체 Data가 0인지 확인하는 함수
 * @param      bigint*   x      확인할 구조체
 * @return      TRUE         구조체의 Data가 0인 경우
 * @return      FALSE         구조체의 Data가 0이 아닌 경우
 */
int bi_is_zero(bigint* x)
{
    if ((x->sign == NEGATIVE) | (x->a[0] != 0))
        return FALSE;
    for (int i = x->wordLen - 1; i >= 1; i--)
        if (x->a[i] != 0)
            return FALSE;
    return TRUE;
}

/**
 * @brief      구조체 Data가 1인지 확인하는 함수
 * @param      bigint*   x      확인할 구조체
 * @return      TRUE         구조체의 Data가 1인 경우
 * @return      FALSE         구조체의 Data가 1이 아닌 경우
 */
int bi_is_one(bigint* x)
{
    if ((x->sign == NEGATIVE) | (x->a[0] != 1))
        return FALSE;
    for (int i = x->wordLen - 1; i >= 1; i--)
        if (x->a[i] != 0)
            return FALSE;
    return TRUE;
}

/**
 * @brief      두 개의 구조체 Data의 ABS를 비교하는 함수
 * @param      bigint*   A      확인할 구조체
 * @param      bigint*   B      확인할 구조체
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1           A<B
 */
int bi_compareABS(bigint* A, bigint* B)
{
    int n = 0, m = 0;
    int i;
    for (i = A->wordLen - 1; i >= 0; i--)
        if (A->a[i] != 0)
        {
            n = i + 1;
            break;
        }
    for (i = B->wordLen - 1; i >= 0; i--)
        if (B->a[i] != 0)
        {
            m = i + 1;
            break;
        }

    if (n > m)
        return 1;
    else if (n < m)
        return -1;
    for (int j = n - 1; j >= 0; j--)
    {
        if (A->a[j] > B->a[j])
            return 1;
        else if (A->a[j] < B->a[j])
            return -1;
    }
    return 0;
}

/**
 * @brief      두 개의 구조체 Data의 크기를 비교하는 함수
 * @param      bigint*   A      확인할 구조체
 * @param      bigint*   B      확인할 구조체
 * @return      1            A>B
 * @return      0            A=B
 * @return      -1            A<B
 */
int bi_compare(bigint* A, bigint* B)
{
    int ret = 0;

    if ((A->sign == NON_NEGATIVE) && (B->sign == NEGATIVE))
        return 1;
    if ((A->sign == NEGATIVE) && (B->sign == NON_NEGATIVE))
        return -1;

    ret = bi_compareABS(A, B);
    if (A->sign == NON_NEGATIVE)
        return ret;
    if (B->sign == NEGATIVE)
        return ret * (-1);
    return 0;
}

/**
 * @brief       구조체 데이터를 r-bit 만큼 left_shift하는 함수
 * @detail      입력으로 들어온 구조체 A의 유효한 데이터(all zero가 아닌 word)에 대해서 left_shift를 실행
 * @detail      따로 refine을 해주는 과정은 포함되어 있지 않으니 필요시 함수 호출 후 따로 bi_refine함수 호출필요
 * @param       bigint** A      Data가 속한 구조체의 주소
 * @param       int r         left_shifting할 크기
 */
void bi_left_shift(bigint** A, int r)
{
    if (bi_is_zero(*A) == TRUE)
        return;
    if (r == 0)
        return;
    int i, Q, R, newWordLen;
    Q = r / WORD_BITLEN;
    R = r % WORD_BITLEN;
    word* tmp = NULL;

    newWordLen = (*A)->wordLen + Q;

    if (R)
    {
        newWordLen++;   //이렇게 하면 DIV_BINARY는 맞는다.

        tmp = (*A)->a;
        (*A)->a = (word*)realloc((*A)->a, sizeof(word) * newWordLen);
        if ((*A)->a == NULL)
            (*A)->a = tmp;
        memset(&(*A)->a[(*A)->wordLen], 0, sizeof(word) * (size_t)((double)newWordLen - (*A)->wordLen));

        for (i = (*A)->wordLen - 1; i >= 0; i--)
        {
            (*A)->a[i + Q] = (*A)->a[i];
        }
        for (i = Q - 1; i >= 0; i--)
        {
            (*A)->a[i] = 0;
        }

        (*A)->a[newWordLen - 1] = (*A)->a[newWordLen - 2] >> (WORD_BITLEN - R);
        for (i = newWordLen - 2; i >= Q + 1; i--)
        {
            (*A)->a[i] = ((*A)->a[i] << R) | ((*A)->a[i - 1] >> (WORD_BITLEN - R));
        }
        (*A)->a[i] = ((*A)->a[i] << R);
    }
    else
    {
        tmp = (*A)->a;
        (*A)->a = (word*)realloc((*A)->a, sizeof(word) * newWordLen);
        if ((*A)->a == NULL)
            (*A)->a = tmp;

        memset(&(*A)->a[(*A)->wordLen], 0, sizeof(word) * (size_t)((double)newWordLen - (*A)->wordLen));
        for (i = (*A)->wordLen - 1; i >= 0; i--)
        {
            (*A)->a[i + Q] = (*A)->a[i];
            (*A)->a[i] = 0;
        }
    }
    (*A)->wordLen = newWordLen;
    //bi_refine(*A);
}

/**
 * @brief      구조체 데이터를 r-bit 만큼 right_shift하는 함수
 * @param      bigint** A      Data가 속한 구조체의 주소
 * @param      int r         right_shifting할 크기
 */
void bi_right_shift(bigint** A, int r)
{
    word* tempArray;
    int temp = (*A)->wordLen * WORD_BITLEN;
    int Q = r / WORD_BITLEN;
    int newWordLen = (*A)->wordLen - Q;
    int R = r % WORD_BITLEN;
    int i;
    if ((temp < r) || (temp == r)) {
        bi_set_zero(A);
        return;
    }
    else if (!R) {
        for (i = 0; i < (*A)->wordLen - Q; i++) {
            (*A)->a[i] = (*A)->a[i + Q];
        }
        for (i = (*A)->wordLen - 1; i >= (*A)->wordLen - Q; i--)
            (*A)->a[i] = 0;
        //bi_refine(*A);        //DIV에서 필요해서 수정
    }
    else
    {
        tempArray = (word*)malloc(sizeof(word) * ((unsigned long long)(*A)->wordLen - Q));
        memset(tempArray, 0, (sizeof(word) * ((unsigned long long)(*A)->wordLen - Q)));
        for (i = 0; i < (*A)->wordLen - Q; i++)
        {
            (*A)->a[i] = (*A)->a[i + Q];
        }
        for (i = (*A)->wordLen - 1; i >= (*A)->wordLen - Q; i--)
            (*A)->a[i] = 0;

        for (i = 0; i < (*A)->wordLen - Q - 1; i++)
            tempArray[i] = (((*A)->a[i + 1] << (WORD_BITLEN - R)) | ((*A)->a[i] >> R));

        tempArray[(*A)->wordLen - Q - 1] = (*A)->a[(*A)->wordLen - Q - 1] >> R;
        array_copy((*A)->a, tempArray, newWordLen);
        //bi_refine(*A);        //DIV때문에 수정
        free(tempArray);
    }
}

/**
 * @brief      구조체 데이터를 r-bit 만큼 reduction 하는 함수
 * @param      bigint** A      Data가 속한 구조체의 주소
 * @param      int r         reduction 할 크기
 */
void bi_reduction(bigint** A, int r)
{
    int temp = (*A)->wordLen * WORD_BITLEN;
    int Q = r / WORD_BITLEN;
    int R = r % WORD_BITLEN;
    int i;

    if ((temp == r) || (temp < r))
        return;
    else if (R)
    {
        for (i = (*A)->wordLen - 1; i > Q; i--)
        {
            (*A)->a[i] = 0;
        }
        (*A)->a[i] &= (word)(((long long)1 << R) - 1);
        bi_refine(*A);
    }
    else
    {
        for (i = (*A)->wordLen - 1; i >= Q; i--)
        {
            (*A)->a[i] = 0;
        }
        bi_refine(*A);
    }
}

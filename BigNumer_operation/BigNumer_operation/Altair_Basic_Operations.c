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
 * @brief      ����ü �޸� �ݳ�
 * @param      bigint** x      �ݳ��� ����ü�� �ּ�
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
 * @brief      ����ü ����
 * @param      bigint** x      ������ ����ü�� �ּ�
 * @param      int wordLen      ����ü ��� ��, ������ Data�� Array �迭 ũ��
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
 * @brief      ����ü �迭�� �Է� ������ ������ ����
 * @detail      �Է� �������� �ڷ����� ����ü�� �ڷ����� ������ ���(�Է� �����Ͱ� ����ü�� �ڷ����� ������ ���)
 * @param      bigint** x      ����ü�� �ּ�
 * @param      int sign         Data�� ��ȣ
 * @param      word* a         �Է� ������ ��
 * @param      int wordLen      �Է� �������� ����
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
 * @brief      ����ü �迭�� �Է� ������ ������ ����
 * @detail      �Է� �������� �ڷ����� string�� ���
 * @param      bigint** x      ����ü�� �ּ�
 * @param      int sign      Data�� ��ȣ
 * @param      char* str      �Է� ������ ��
 * @param      word base      �Է� �������� ǥ����(2����, 16����)
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
 * @brief      ����ü�� �迭 ���� 16������ ���
 * @param      bigint* x      ����� �������� ����ü
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

void bi_show_dec(bigint* x)   //�̿ϼ�
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
 * @brief      ����ü�� �迭 ���� 2������ ���
 * @param      bigint* x      ����� �������� ����ü
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
 * @brief      ����ü�� �迭 ���� �������� ���� 0�� ��� �ش� �޸𸮸� �ݳ��ϰ� �缳��
 * @param      bigint* x      �缳���� ����ü
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
 * @brief      �迭 ������ copy
 * @param      word* dst      copy�Ͽ� ������ �迭
 * @param      word* src      copy�� ������
 * @param      int len        copy�� �������� ����
*/
void array_copy(word* dst, word* src, int len)
{
    int i;
    for (i = 0; i < len; i++)
        dst[i] = src[i];
}
/**
 * @brief      bigint ����ü copy
 * @param      bigint* dst      copy�Ͽ� ������ �迭
 * @param      bigint* src      copy�� ������
*/
void bi_copy(bigint* dst, bigint* src)
{
    array_copy(dst->a, src->a, src->wordLen);
    dst->sign = src->sign;
    dst->wordLen = src->wordLen;
}

/**
 * @brief      ����ü ������ copy
 * @param      bigint** y      copy�Ͽ� ���� ������ ����ü �ּ�
 * @param      bigint** x      copy�� ���� ����� ����ü
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
 * @brief      ���� ������ ����
 * @param      word* dst      ���� ���� ������ Array
 * @param      int wordLen      ������ Array�� ����
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
 * @brief      ���� ���� ������ ����ü ����
 * @param      bigint** x      ���� ������ ����ü
 * @param      int sign      ���� ��ȣ
 * @param      int wordLen      ������ ���� ����
*/
void bi_gen_rand(bigint** x, int sign, int wordLen)
{
    bi_new(x, wordLen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordLen);
    bi_refine(*x);
}

/**
 * @brief      ����ü Array�� wordLen���� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @return      x->wordLen      Array�� ����
*/
int bi_get_word_length(bigint* x)
{
    return x->wordLen;
}

/**
 * @brief      ����ü Array�� bit���� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @return      Array�� bit ����
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
 * @brief      �ִ� 32bit�� ���� ������ n�� bit ���̸� ����ϴ� �Լ�
 * @param      u32 n		��Ʈ ���̰� �ñ��� n, unsigned int ��
 * @return     n�� ��Ʈ ����
*/
int get_bit_length(u32 n)
{
    int cnt = 0;
    while ((n >> ++cnt));
    return cnt;
}

/**
 * @brief      ����ü Array�� j��° bit �� return �Լ�
 * @param      bigint*   x      return�� Array�� ����ü
 * @param      int j         return�� Array�� ����ü
 * @return      Array�� j��° bit ����
*/
int bi_get_jth_bit(bigint* x, int j)
{
    int q, r;
    q = j / WORD_BITLEN;
    r = j % WORD_BITLEN;
    return (((x->a[q]) >> r) & 0x1);
}

/**
 * @brief      ����ü Data�� ��ȣ return �Լ�
 * @param      bigint*   x      return�� sign�� ����ü
 * @return      x->sign         ����ü�� sign ��, ����� 0 ������ 1
*/
int bi_get_sign(bigint* x)
{
    return x->sign;
}

/**
 * @brief      ����ü Data�� ��ȣ�� flip�ϴ� �Լ�
 * @param      bigint*   x      flip�� sign�� ����ü
*/
void bi_flip_sign(bigint** x)
{
    (*x)->sign = ((*x)->sign) ^ 0x1;
}

/**
 * @brief      ����ü Data�� 1�� �����ϴ� �Լ�
 * @param      bigint*   x      ������ ����ü�� �ּ�
*/
void bi_set_one(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

/**
 * @brief      ����ü Data�� 0���� �����ϴ� �Լ�
 * @param      bigint*   x      ������ ����ü�� �ּ�
*/
void bi_set_zero(bigint** x)
{
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

/**
 * @brief      ����ü Data�� 0���� Ȯ���ϴ� �Լ�
 * @param      bigint*   x      Ȯ���� ����ü
 * @return      TRUE         ����ü�� Data�� 0�� ���
 * @return      FALSE         ����ü�� Data�� 0�� �ƴ� ���
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
 * @brief      ����ü Data�� 1���� Ȯ���ϴ� �Լ�
 * @param      bigint*   x      Ȯ���� ����ü
 * @return      TRUE         ����ü�� Data�� 1�� ���
 * @return      FALSE         ����ü�� Data�� 1�� �ƴ� ���
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
 * @brief      �� ���� ����ü Data�� ABS�� ���ϴ� �Լ�
 * @param      bigint*   A      Ȯ���� ����ü
 * @param      bigint*   B      Ȯ���� ����ü
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
 * @brief      �� ���� ����ü Data�� ũ�⸦ ���ϴ� �Լ�
 * @param      bigint*   A      Ȯ���� ����ü
 * @param      bigint*   B      Ȯ���� ����ü
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
 * @brief       ����ü �����͸� r-bit ��ŭ left_shift�ϴ� �Լ�
 * @detail      �Է����� ���� ����ü A�� ��ȿ�� ������(all zero�� �ƴ� word)�� ���ؼ� left_shift�� ����
 * @detail      ���� refine�� ���ִ� ������ ���ԵǾ� ���� ������ �ʿ�� �Լ� ȣ�� �� ���� bi_refine�Լ� ȣ���ʿ�
 * @param       bigint** A      Data�� ���� ����ü�� �ּ�
 * @param       int r         left_shifting�� ũ��
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
        newWordLen++;   //�̷��� �ϸ� DIV_BINARY�� �´´�.

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
 * @brief      ����ü �����͸� r-bit ��ŭ right_shift�ϴ� �Լ�
 * @param      bigint** A      Data�� ���� ����ü�� �ּ�
 * @param      int r         right_shifting�� ũ��
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
        //bi_refine(*A);        //DIV���� �ʿ��ؼ� ����
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
        //bi_refine(*A);        //DIV������ ����
        free(tempArray);
    }
}

/**
 * @brief      ����ü �����͸� r-bit ��ŭ reduction �ϴ� �Լ�
 * @param      bigint** A      Data�� ���� ����ü�� �ּ�
 * @param      int r         reduction �� ũ��
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

#include "Altair_Basic_Operations.h"
#include "Altair_Calculation.h"
#include "test.h"
// N이 하나의 WORD로 표현될 때, 여러 개의 WORD로 표현될 때 생각을 해보자
// N이 하나의 WORD로 표현되면 WORD_length로 표현해서 그냥 for문 하나,
// N이 여러개의 WORD로 표현되면 Basic Operation에서 함수 하나 호출
// bi_get_bit_length 함수 호출.

word bi_L2R_get_bit_func(word A) {
	
	for (word i = 0; i < WORD_BITLEN; i++) {
		if ((A >> i) == 0)
			return i;
	}
	return WORD_BITLEN;
}

word bi_L2R_BINARY(IN bigint* A, IN bigint* N, OUT bigint** out) {
	

	//A의 최상위 워드만 따로 꺼내서 구현
	/*최상위 워드는 몇 비트인지 알아야하고, 남은 워드들은 최상위 비트가
	0이어도 상관 없음
	*/
	//int i A =->wordLen-1;
	if (TRUE == bi_is_zero(N)) {
		bi_set_one(out);
		return operationSuccess;
	}
	word len = 0;
	len = bi_L2R_get_bit_func(N->a[N->wordLen - 1]);
	bigint* temp = NULL;
	int a = bi_get_bit_length(N);
	bi_new(&temp, (100000));
	temp->a[0] = 1;


	for (int i = len - 1; i >= 0; i--) {
		bi_SQU_AA(&temp);
		if ((N->a[N->wordLen - 1] >> i) & 0x1) {
			bi_MUL_AAB(&temp, A);
		}
	}

	for (int i = N->wordLen - 2; i >= 0; i--) {

		for (int j = WORD_BITLEN - 1; j >= 0; j--) {

			bi_SQU_AA(&temp);
			if ((N->a[i] >> j) & 0x1)
				bi_MUL_AAB(&temp, A);
		}
	}

	bi_refine(temp);
	bi_copy(*out, temp);
	bi_delete(&temp);
}

word bi_L2R_BINARY_allocation(IN bigint* A, IN bigint* N, OUT bigint** out) {

	if (TRUE == bi_is_zero(N)) {
		bi_set_one(out);
		return operationSuccess;
	}

	word len = 0;
	len = bi_L2R_get_bit_func(N->a[N->wordLen - 1]);
	bigint* temp = NULL;
	word* new_temp = NULL;
	int k = 0;
	bi_new(&temp, 4 * A->wordLen);
	temp->a[0] = 1;



	for (int i = len - 1; i >= 0; i--) {
		bi_SQU_AA(&temp);
		if ((N->a[N->wordLen - 1] >> i) & 0x1) {
			bi_MUL_AAB(&temp, A);
		}
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
			array_copy(temp->a, new_temp, (k - A->wordLen -1));
			free(new_temp);
		}
	}
	bi_refine(temp);
	bi_new(out, temp->wordLen);
	bi_copy(*out, temp);
	bi_delete(&temp);
}

void L2R_EZ_test() {
	bigint* A = NULL;
	bigint* N = NULL;
	bigint* out = NULL;
	bi_new(&A, 1);
	bi_new(&N, 1);
	A->a[0] = 0xff;
	N->a[0] = 2;
	int len = bi_get_bit_length(N);
	bi_L2R_BINARY_allocation(A, N, &out);
	bi_show_hex(out);
	bi_delete(&A);
	bi_delete(&N);
	bi_delete(&out);
}


void L2RTest()
{
	bigint* A = NULL;
	bigint* N = NULL;
	bigint* out = NULL;
	bi_new(&out, (1));

	for (int i = 0; i < 100; i++)
	{
		
		bi_gen_rand(&A, NON_NEGATIVE, 3);
		bi_gen_rand(&N, NON_NEGATIVE, 1);
		(N->a[0] = N->a[0] >> 59);
		printf("A = 0x");
		bi_show_hex(A);
		printf("N = 0x");
		bi_show_hex(N);
		//bi_L2R_BINARY(A, N, &out);

		bi_L2R_BINARY_allocation(A, N, &out);

		printf("out = 0x");
		bi_show_hex(out);
		printf("print((A^N) == out)\n");
	}
}

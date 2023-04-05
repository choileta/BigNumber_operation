#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define IN
#define OUT

#define WORD_BITLEN			32
#define ECC_WORD_NUM		(256/(WORD_BITLEN))
#define TEST_BITLEN			512
#define A_BIG_THAN_B		4				//divison���� A�� B���� ��� �� ū��
#define TEST_NUM			50
#define SIZE				8				//ECC���� ����� �迭�� ��

#define NEGATIVE			1
#define NON_NEGATIVE		0
#define TRUE				1
#define FALSE				0
#define INFINITY_POINT		1
#define NOT_INFINITY_POINT	0

#define TESTING				1

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;

enum returnData {
	operationSuccess = 10000,
	operationFailure,
	memoryAllocationFailure,
	parameterError,
	MultiPrecisionDivisionFailure,
};

#if WORD_BITLEN == 64
typedef u64 word;
#define WORD_MASK 0xffffffffffffffff
#elif WORD_BITLEN == 32
typedef u32 word;
#define WORD_MASK 0xffffffff
#else
typedef u8 word;
#define WORD_MASK 0xff
#endif

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

typedef struct {
	int sign;
	int wordLen;
	word* a;
	int flag;
}bigint;

typedef struct {
	bigint* x;
	bigint* y;
	bigint* z;
	int flag;			//���ѿ����̸� 1, �ƴϸ� 0
}ECC_bigint;
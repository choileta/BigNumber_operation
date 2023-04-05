#pragma once

#include "type.h"

void addTest();
void subTest();
void mulTest();
void mulKaratsubaTest();
void mulKaratsubaSimpleTest();
void squTest();
void squKaratsubaTest();
void divBinaryTest();
void wordDivEZTest();
void wordDivTest();
void divEZTest();
void divTest();

//ECC
void addFpTest(bigint* p);
void subFpTest(bigint* p);
void fastReductionTest(bigint* p);
void inversionTest(bigint* p);
void ECCL2RTest(bigint* p, ECC_bigint* P);
void ECCJacobianL2RTest(bigint* p, ECC_bigint* P);
void ECCwNAFSMTest(bigint* p, ECC_bigint* P);
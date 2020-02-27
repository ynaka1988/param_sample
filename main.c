/*! @file
 *
 *  コンソールから関数に引数を与えるサンプル
 *
 */

#include "readint.h"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

typedef int (*TEST_FUNC_T)(void const * const);

/*!
 * 標準入力から数値の入力を求め、その数値を標準出力に出す
 *
 * @retval EXIT_SUCCESS 成功
 * @retval EXIT_FAILURE 失敗
 */
static int test0(
	void const * const pParam //!< 使用しない
)
{
	(void)pParam;
	int32_t num = 0;

	puts("test0 引数を使用しないパターン");

	printf("Input :");
	int result = readInt32(&num);
	if (result == EXIT_FAILURE) {
		puts("エラー");
		return EXIT_FAILURE;
	}

	printf("Output:%" PRId32 "\n", num);

	return EXIT_SUCCESS;
}


/*!
 * 引数を32bit符号あり数値の配列として解釈し、標準出力に出す
 *
 * @retval EXIT_SUCCESS 成功
 */
static int test1(
	void const * const pParam //!< 32bit符号あり数値の配列（と関数内で解釈する)
)
{
	puts("test1 キャストするパターン");

	for(int i =0; i < 4; ++i)
	{
		printf("param%d:%d\n", i, ((int*)pParam)[i]);
	}

	return EXIT_SUCCESS;
}

/*! 
 * 近場にコメント書けば反映されるので
 * @なんとかを覚える必要は、実はない
 */
typedef struct {
	int32_t paramA; //!< とくに
	int32_t paramB; //!< 意味は
	int32_t paramC; //!< ないよ
	int32_t paramD; //!< !!!
} data0_t;

/*!
 * 引数を構造体として解釈して、標準出力に出力する
 *
 * @retval EXIT_SUCCESS 成功
 */
static int test2(
	data0_t const * const pParam //!< 32bitでアライメントされた構造体
	)
{
	puts("test2 アライメントされた構造体に入れるパターン");

	printf("paramA:%d\n", pParam->paramA);
	printf("paramB:%d\n", pParam->paramB);
	printf("paramC:%d\n", pParam->paramC);
	printf("paramD:%d\n", pParam->paramD);

	return EXIT_SUCCESS;
}

typedef struct {
	int8_t  paramE;
	int16_t paramF;
	int32_t paramG;
	int64_t paramH;
} data1_t;

/*!
 * 引数を構造体として解釈して、標準出力に出力する
 * @note 引数の構造体はパディングあり
 *
 * @retval EXIT_SUCCESS 成功
 */
static int test3(
		data1_t const * const pParam //!< 適当なパディングありの構造体
	)
{
	puts("test3 パディングありの構造体に入れるパターン(これはうまく行かない)");

	printf("paramA:%d\n", pParam->paramA);
	printf("paramB:%d\n", pParam->paramB);
	printf("paramC:%d\n", pParam->paramC);
	printf("paramD:%ld\n", pParam->paramD);

	return EXIT_SUCCESS;
}


static int inputTestNum(uint32_t * const pTestNum, const uint32_t test_num_max)
{
	int32_t testNum = 0;
	int result = EXIT_FAILURE;

	do {
		printf("test num(0-%u):", test_num_max);
		result = readInt32(&testNum);
		if (result == EXIT_FAILURE) {
			puts("test num is invalid");
			continue;
		}

		if (testNum > test_num_max) {
			puts("test num is out of range");
			continue;
		}
		else {
			result = EXIT_SUCCESS;
		}

	} while (result == EXIT_FAILURE);

	*pTestNum = testNum;

	return EXIT_SUCCESS;
}

int inputParam(int32_t * const paParam, const uint32_t param_num_max)
{
	printf("param num(0-%u):", param_num_max);

	int32_t paramNum = 0;
	int result = readInt32(&paramNum);
	if (result == EXIT_FAILURE) {
		puts("param num is invalid");
		return EXIT_FAILURE;
	}
	else if ( (paramNum < 0) || (paramNum > param_num_max) ) {
		puts("param num is out of range");
		return EXIT_FAILURE;
	}

	for (int_fast32_t i = 0u; i < paramNum; ++i) {
		printf("param%" PRIdFAST32 ":", i);
		result = readInt32(&paParam[i]);
		if (result == EXIT_FAILURE) {
			printf("param%" PRIdFAST32 " is invalid\n", i);
			--i;
			continue;
		}
	}

	return EXIT_SUCCESS;
}

static int execTest(const uint32_t test_number, int32_t const * const pParam, TEST_FUNC_T const * const pTestTable, const uint32_t table_max)
{
	if (test_number >= table_max) {
		return EXIT_FAILURE;
	}

	pTestTable[test_number](pParam);

	return EXIT_SUCCESS;
}


int main(void)
{
	enum {table_max = 4};
	static TEST_FUNC_T s_func_table[table_max] = {
		(TEST_FUNC_T)test0,
		(TEST_FUNC_T)test1,
		(TEST_FUNC_T)test2,
		(TEST_FUNC_T)test3,
	};

	uint32_t testNumber = 0;
	int result = inputTestNum(&testNumber, table_max -1);
	if (result == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	enum {param_max = 4};
	int32_t aParam[param_max] = {};

	result = inputParam(aParam, param_max);
	if (result == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	result = execTest(testNumber, aParam, s_func_table, table_max);
	if (result == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}


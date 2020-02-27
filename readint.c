#include "readint.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h> /* ERANGEを使用するために必要 */
#include <limits.h>

#include <stdint.h>
#include <inttypes.h>

#include <assert.h>


static int readStr(char * const pBuf, const size_t buf_size)
{
	if (fgets(pBuf, buf_size, stdin) == NULL) {
		return EXIT_FAILURE;
	}
	else if (pBuf[0] == '\n') {
		return EXIT_FAILURE;
	}

	// 改行を削除
	char * const pNewLine = strchr(pBuf, '\n');
	if (pNewLine != NULL) {
		*pNewLine = '\0';
	}

	return EXIT_SUCCESS;
}


static int readLong(long * pVal)
{
	enum {BUF_SIZE = 13}; // 符号 + 10桁 + '\n' + '\0' で13
	char aBuf[BUF_SIZE] = "";

	if (readStr(aBuf, sizeof(aBuf)) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	char * pEnd = NULL;
	long num = strtol(aBuf, &pEnd, 10);
	if (errno == ERANGE) {
		return EXIT_FAILURE;
	}
	else if (*pEnd != '\0') {
		return EXIT_FAILURE;
	}

	*pVal = num;
	return EXIT_SUCCESS;
}


static int readLongLong(long long * pVal)
{
	enum {BUF_SIZE = 22}; // 符号 + 19桁 + '\n' + '\0' で22
	char aBuf[BUF_SIZE] = "";

	if (readStr(aBuf, sizeof(aBuf)) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	char * pEnd = NULL;
	long long num = strtoll(aBuf, &pEnd, 10);
	if (errno == ERANGE) {
		return EXIT_FAILURE;
	}
	else if (*pEnd != '\0') {
		return EXIT_FAILURE;
	}

	*pVal = num;
	return EXIT_SUCCESS;
}


// 規格上、longは32bit以上であることが保証されている
static_assert(LONG_MAX >= INT32_MAX, "");
static_assert(LONG_MIN <= INT32_MIN, "");

int readInt32(int32_t* pVal)
{
	long val = 0;

	int result = readLong(&val);
	if (result == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	else if (val > INT32_MAX) {
		return EXIT_FAILURE;
	}
	else if (val < INT32_MIN) {
		return EXIT_FAILURE;
	}
	else {
		*pVal = val;
		return EXIT_SUCCESS;
	}
}

// 規格上、long longは64bit以上であることが保証されている
static_assert(LLONG_MAX >= INT64_MAX, "");
static_assert(LLONG_MIN <= INT64_MIN, "");

int readInt64(int64_t* pVal)
{
	long long val = 0;
	
	int result = readLongLong(&val);
	if (result == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}
	else if (val > INT64_MAX) {
		return EXIT_FAILURE;
	}
	else if (val < INT64_MIN) {
		return EXIT_FAILURE;
	}
	else {
		*pVal = val;
		return EXIT_SUCCESS;
	}
}


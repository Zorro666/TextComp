#ifndef BINARY_UTIL_HH
#define BINARY_UTIL_HH

#include <stdio.h>

typedef struct BitFile
{
	FILE* pFile;
	int currentByte;
	int currentBitPos;
} BitFile;

void binaryFilePrint(FILE* pFile, unsigned int code, unsigned int bits);

#endif /* #ifndef BINARY_UTIL_HH */


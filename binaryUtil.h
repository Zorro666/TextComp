#ifndef BINARY_UTIL_HH
#define BINARY_UTIL_HH

#include <stdio.h>

typedef struct BitFile
{
	FILE* pFile;
	int currentBits;
	unsigned int mode;
	unsigned char currentMask;
} BitFile;

BitFile* binaryOpenInputFile(const char* const name);
BitFile* binaryOpenOutputFile(const char* const name);

void binaryOutputBits(BitFile* const pOutput, unsigned long code, unsigned int count);

void binaryFilePrint(FILE* pFile, unsigned int code, unsigned int bits);

#endif /* #ifndef BINARY_UTIL_HH */


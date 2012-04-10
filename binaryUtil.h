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
void binaryCloseFile(BitFile* const pBitFile);

void binaryFilePrint(FILE* pFile, unsigned int value, unsigned int numBits);

void binaryOutputBits(BitFile* const pOutput, unsigned long value, unsigned int numBits);
unsigned long binaryInputBits(BitFile* const pOutput, unsigned int numBits);

#endif /* #ifndef BINARY_UTIL_HH */


#include <malloc.h>
#include <string.h>

#include "binaryUtil.h"

#define BINARY_MODE_UNKNOWN (0)
#define BINARY_MODE_INPUT (1)
#define BINARY_MODE_OUTPUT (2)

/* Internal static functions */
static BitFile* binaryOpenFile(const char* const name, const char* const mode)
{
	BitFile* const pBitFile = (BitFile*const)malloc(sizeof(BitFile));
	memset(pBitFile, 0, sizeof(BitFile));
	pBitFile->pFile = fopen(name, mode);
	pBitFile->currentBits = 0x0;
	pBitFile->currentMask = 0x80;
	pBitFile->mode = BINARY_MODE_UNKNOWN;
	return pBitFile;
}

/* Public API functions */
BitFile* binaryOpenInputFile(const char* const name)
{
	BitFile* const pBitFile = binaryOpenFile(name, "rb");
	pBitFile->mode = BINARY_MODE_INPUT;
	return pBitFile;
}

BitFile* binaryOpenOutputFile(const char* const name)
{
	BitFile* const pBitFile = binaryOpenFile(name, "wb");
	pBitFile->mode = BINARY_MODE_OUTPUT;
	return pBitFile;
}

void binaryCloseFile(BitFile* const pBitFile)
{
	if (pBitFile->mode == BINARY_MODE_OUTPUT)
	{
		if (pBitFile->currentMask != 0x80)
		{
			putc(pBitFile->currentBits, pBitFile->pFile);
		}
	}
	fclose(pBitFile->pFile);
	memset(pBitFile, 0, sizeof(BitFile));
	free(pBitFile);
}

void binaryFilePrint(FILE* pFile, unsigned int value, unsigned int numBits)
{
	unsigned int mask = 0x0;
	mask = (unsigned int)(1<<(numBits-1));
	while (mask != 0)
	{
		if (value & mask)
		{
			fputc('1', pFile);
		}
		else
		{
			fputc('0', pFile);
		}
		mask = mask>>1;
	}
}

void binaryOutputBits(BitFile* const pOutput, unsigned long value, unsigned int numBits)
{
	unsigned long mask;
	mask = (unsigned long)(1L<<(numBits-1));
	while (mask != 0)
	{
		if (mask & value)
		{
			pOutput->currentBits |= pOutput->currentMask;
		}
		pOutput->currentMask = pOutput->currentMask >> 1;
		if (pOutput->currentMask == 0)
		{
			putc(pOutput->currentBits, pOutput->pFile);
			pOutput->currentBits = 0x0;
			pOutput->currentMask = 0x80;
		}
		mask = mask >> 1L;
	}
}

unsigned long binaryInputBits(BitFile* const pOutput, unsigned int numBits)
{
	unsigned long value = 0;
	unsigned long mask;

	mask = (unsigned long)(1L<<(numBits-1));
	while (mask != 0)
	{
		if (pOutput->currentMask == 0x80)
		{
			pOutput->currentBits = getc(pOutput->pFile);
		}
		if (pOutput->currentMask & pOutput->currentBits)
		{
			value |= mask;
		}
		mask = mask >> 1;

		pOutput->currentMask = pOutput->currentMask >> 1;
		if (pOutput->currentMask == 0)
		{
			pOutput->currentMask = 0x80;
		}
	}

	return value;
}


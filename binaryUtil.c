#include "binaryUtil.h"

void binaryFilePrint(FILE* pFile, unsigned int code, unsigned int bits)
{
	unsigned int mask = 0x0;
	mask = (unsigned int)(1<<(bits-1));
	while (mask != 0)
	{
		if (code & mask)
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

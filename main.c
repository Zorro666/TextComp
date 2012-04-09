#include <string.h>

#include "huffman.h"

int main(int argc, char** argv)
{
	char* pInput = NULL;
	int numBytes = 0;
	BitFile* pOutput = NULL;
	int debugOutputModel = 0;
	int i;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			debugOutputModel = 1;
		}
	}

	pInput = "AAAAA";
	numBytes = 5;
	pOutput = binaryOpenOutputFile("jake.bin");
	compressInput(pInput, numBytes, pOutput, debugOutputModel);

	return -1;
}

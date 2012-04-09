#include <string.h>

#include "huffman.h"

static size_t getFileSize(const char* fileName)
{
	size_t fileSize = 0;
	FILE* const pFile = fopen(fileName, "rb");
	fseek(pFile, 0, SEEK_END);
	fileSize = (size_t)ftell(pFile);
	fclose(pFile);

	return fileSize;
}

char* loadDiaFile(const char* const inputFileName, size_t* const pInputSize)
{
	const size_t fileSize = getFileSize(inputFileName);
	char* const inputBuffer = (char* const)malloc(fileSize);
	size_t inputSize = 0;
	size_t i;
	FILE* const pFile = fopen(inputFileName, "rb");

	for (i = 0; i < fileSize; i++)
	{
		char data;
		if (fread(&data, sizeof(char), 1, pFile) != 1)
		{
			inputSize = 0;
			break;
		}
		if (data != '\0')
		{
			inputBuffer[inputSize] = data;
			inputSize++;
		}
	}
	fclose(pFile);

	*pInputSize = inputSize;
	return inputBuffer;
}

int main(int argc, char** argv)
{
	char* pInput = NULL;
	BitFile* pOutput = NULL;
	int debugOutputModel = 0;
	int i;
	const char* const inputFileName = "jake.dia";
	const char* const outputFileName = "jake.bin";
	size_t inputSize = 0;
	size_t outputSize = 0;
	float ratio = 0.0f;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			debugOutputModel = 1;
		}
	}

	pInput = loadDiaFile(inputFileName, &inputSize);

	pOutput = binaryOpenOutputFile(outputFileName);
	compressInput(pInput, inputSize, pOutput, debugOutputModel);
	binaryCloseFile(pOutput);

	outputSize = getFileSize(outputFileName);

	ratio = (100.0f*(float)outputSize/(float)inputSize);
	printf("InputSize %d OutputSize %d Ratio %.2f Predict Size %.2fMB Saving %.2fMB\n", inputSize, outputSize, ratio, 
					(ratio*4.0f/100.0f), (4.0f-ratio*4.0f/100.0f));

	return 1;
}

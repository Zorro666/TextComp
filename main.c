#include <string.h>
#include <malloc.h>

#include "huffman.h"

static int outputFile(const char* const fileName, const char* const pData, const size_t numBytes)
{
	FILE* const pFile = fopen(fileName, "wb");
	if (pFile == NULL)
	{
		return 0;
	}
	fwrite(pData, sizeof(char), numBytes, pFile);
	fclose(pFile);
	return 1;
}

static size_t getFileSize(const char* const fileName)
{
	size_t fileSize = 0;
	FILE* const pFile = fopen(fileName, "rb");
	if (pFile == NULL)
	{
		return 0;
	}
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
	int i;
	int compressFile = 0;
	int uncompressFile = 0;
	unsigned int debugFlag = 0;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-c") == 0)
		{
			compressFile = 1;
		}
		if (strcmp(argv[i], "-u") == 0)
		{
			uncompressFile = 1;
		}
		if (strcmp(argv[i], "-d") == 0)
		{
			debugFlag = 1;
		}
	}

	if (compressFile == 1)
	{
		const char* const inputFileName = "jake.dia";
		const char* const outputFileName = "jake.bin";
		size_t inputSize = 0;
		size_t outputSize = 0;
		float ratio = 0.0f;
		char* pInput = NULL;
		BitFile* pOutput = NULL;

		pInput = loadDiaFile(inputFileName, &inputSize);
		pOutput = binaryOpenOutputFile(outputFileName);
		compressInput(pInput, inputSize, pOutput, debugFlag);
		binaryCloseFile(pOutput);

		outputSize = getFileSize(outputFileName);

		ratio = (100.0f*(float)outputSize/(float)inputSize);
		printf("InputSize %d OutputSize %d Ratio %.2f Predict Size %.2fMB Saving %.2fMB\n", inputSize, outputSize, ratio, 
						(ratio*4.0f/100.0f), (4.0f-ratio*4.0f/100.0f));
	}
	if (uncompressFile == 1)
	{
		const char* const inputFileName = "jake.bin";
		const char* const outputFileName = "jakeNew.dia";
		size_t inputSize = 0;
		size_t outputSize = 0;
		float ratio = 0.0f;
		BitFile* pInput = NULL;
		char* pOutput = NULL;

		pOutput = (char*)malloc(16*1024*124);
		pInput = binaryOpenInputFile(inputFileName);
		inputSize = getFileSize(inputFileName);
		outputSize = uncompressInput(pInput, inputSize, pOutput, debugFlag);

		binaryCloseFile(pInput);
		
		outputFile(outputFileName, pOutput, outputSize);

		free(pOutput);
		inputSize = getFileSize(inputFileName);
		outputSize = getFileSize(outputFileName);

		ratio = (100.0f*(float)inputSize/(float)outputSize);
		printf("InputSize %d OutputSize %d Ratio %.2f Predict Size %.2fMB Saving %.2fMB\n", inputSize, outputSize, ratio, 
						(ratio*4.0f/100.0f), (4.0f-ratio*4.0f/100.0f));
	}

	return 1;
}

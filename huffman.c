#include <string.h>
#include <malloc.h>

#include "binaryUtil.h"

typedef struct TreeNode
{
	unsigned int count;
	unsigned int savedCount;
	int child0;
	int child1;
} TreeNode;

typedef struct SymbolCode
{
	unsigned int value;
	unsigned int numBits;
} SymbolCode;

#define MAX_SYMBOL_VALUE (255)
#define MAX_NUM_SYMBOLS (MAX_SYMBOL_VALUE+1)
#define END_OF_STREAM (MAX_NUM_SYMBOLS)
#define MAX_NUM_CODES (MAX_NUM_SYMBOLS+1)
#define MAX_NUM_NODES (MAX_NUM_CODES*2)
#define	MAX_NODE (MAX_NUM_NODES-1)

/* Internal private static functions */

static void makeCounts(const char* const pSource, const size_t numBytes, unsigned long* const pCounts)
{
	size_t i;
	for (i = 0; i < numBytes; i++)
	{
		const int symbol = pSource[i];
		pCounts[symbol]++;
	}
}

static void scaleCounts(unsigned long* const pCounts, TreeNode* const pNodes)
{
	unsigned long maxCount = 0;
	int i;

	for (i = 0; i < MAX_NUM_SYMBOLS; i++)
	{
		const unsigned long count = pCounts[i];
		if (count > maxCount)
		{
			maxCount = count;
		}
	}
	if (maxCount == 0)
	{
		pCounts[0] = 1;
		maxCount = 1;
	}
	maxCount = maxCount/MAX_NUM_SYMBOLS;
	maxCount = maxCount+1;
	for (i = 0; i < MAX_NUM_SYMBOLS; i++)
	{
		const unsigned long count = pCounts[i];
		unsigned int scaledCount = (unsigned int)(count/maxCount);
		if ((scaledCount == 0) && (count != 0))
		{
			scaledCount = 1;
		}
		pNodes[i].count = scaledCount;
		pNodes[i].child0 = END_OF_STREAM;
		pNodes[i].child1 = END_OF_STREAM;
	}
	pNodes[END_OF_STREAM].count = 1;
	pNodes[END_OF_STREAM].child0 = END_OF_STREAM;
	pNodes[END_OF_STREAM].child1 = END_OF_STREAM;
}

/* Format is: startSymbol, stopSymbol, count0, count1, count2, ... countN, ..., 0 */
/* When finding the start, stop symbols only break out if find more than 3 0's in the counts */
static void outputCounts(FILE* const pFile, const TreeNode* const pNodes)
{
	int first = 0;
	int last;
	int next;

	while ((first < MAX_NUM_SYMBOLS) && (pNodes[first].count == 0))
	{
		first++;
	}
	last = first;
	next = first;
	for (; first < MAX_NUM_SYMBOLS; first = next)
	{
		int i;
		last = first+1;
		while (1)
		{
			for (; last < MAX_NUM_SYMBOLS; last++)
			{
				if (pNodes[last].count == 0)
				{
					break;
				}
			}
			last--;
			for (next = last+1; next < MAX_NUM_SYMBOLS; next++)
			{
				if (pNodes[next].count != 0)
				{
					break;
				}
			}
			if (next == MAX_NUM_SYMBOLS)
			{
				break;
			}
			if ((next-last) > 3)
			{
				break;
			}
			last = next;
		}
		putc(first, pFile);
		putc(last, pFile);
		for (i = first; i <= last; i++)
		{
			const unsigned int count = pNodes[i].count;
			putc((int)count, pFile);
		}
	}
	putc(0xFF, pFile);
	putc(0xFF, pFile);
	putc((int)(pNodes[0xFF].count), pFile);
}

static void inputCounts(FILE* const pFile, TreeNode* const pNodes)
{
	while (1)
	{
		int i;
		const int first = getc(pFile);
		const int last = getc(pFile);
		for (i = first; i <= last; i++)
		{
			const int count = getc(pFile);
			pNodes[i].count = (size_t)count;
			pNodes[i].child0 = END_OF_STREAM;
			pNodes[i].child1 = END_OF_STREAM;
		}
		if ((first == last) && (first == 0xFF))
		{
			break;
		}
	}
	pNodes[END_OF_STREAM].count = 1;
	pNodes[END_OF_STREAM].child0 = END_OF_STREAM;
	pNodes[END_OF_STREAM].child1 = END_OF_STREAM;
}

static int buildTree(TreeNode* const pNodes)
{
	int min1;
	int min2;
	int nextFree;

	pNodes[MAX_NODE].count = 0xFFFFFFF;
	for (nextFree = END_OF_STREAM+1; ; nextFree++)
	{
		int i;
		min1 = MAX_NODE;
		min2 = MAX_NODE;
		for (i = 0; i < nextFree; i++)
		{
			const unsigned int count = pNodes[i].count;
			if (count != 0)
			{
				if (count < pNodes[min1].count)
				{
					min2 = min1;
					min1 = i;
				}
				else if (count < pNodes[min2].count)
				{
					min2 = i;
				}
			}
		}
		if (min2 == MAX_NODE)
		{
			break;
		}
		pNodes[nextFree].count = pNodes[min1].count+pNodes[min2].count;

		pNodes[min1].savedCount = pNodes[min1].count;
		pNodes[min1].count = 0;

		pNodes[min2].savedCount = pNodes[min2].count;
		pNodes[min2].count = 0;

		pNodes[nextFree].child0 = min1;
		pNodes[nextFree].child1 = min2;
		pNodes[nextFree].savedCount = 0;
	}

	nextFree--;
	pNodes[nextFree].savedCount = pNodes[nextFree].count;

	return nextFree;
}

static void convertTreeToCode(const TreeNode* const pNodes, SymbolCode* const pCodes, 
											 				const unsigned int value, const unsigned int numBits, const int node)
{
	unsigned int nextValue;
	unsigned int nextNumBits;
	if (node <= END_OF_STREAM)
	{
		pCodes[node].value = value;
		pCodes[node].numBits = numBits;
		return;
	}
	nextValue = value<<1;
	nextNumBits = numBits+1;
	convertTreeToCode(pNodes, pCodes, nextValue, nextNumBits, pNodes[node].child0);
	nextValue = nextValue | 0x1;
	convertTreeToCode(pNodes, pCodes, nextValue, nextNumBits, pNodes[node].child1);
}

static void printChar(const int c)
{
	if (c >= ' ' && c < 127)
	{
		printf("'%c'", c);
	}
	else
	{
		printf("0x%03X", c);
	}
}

static void printModel(const TreeNode* const pNodes, const SymbolCode* const pCodes)
{
	int i;
	for (i = 0; i < MAX_NODE; i++)
	{
		const unsigned int count = pNodes[i].savedCount;
		if (count != 0)
		{
			printf("node=");
			printChar(i);
			printf(" count=%3d", count);
			printf(" child0=");
			printChar(pNodes[i].child0);
			printf(" child1=");
			printChar(pNodes[i].child1);
			if (pCodes && (i <= END_OF_STREAM))
			{
				printf(" Huffman code=");
				binaryFilePrint(stdout, pCodes[i].value, pCodes[i].numBits);
			}
			printf("\n");
		}
	}
}

static void compressData(const char* const pInput, const size_t numBytes, BitFile* const pOutput, const SymbolCode* const pCodes)
{
	size_t i;
	for (i = 0; i < numBytes; i++)
	{
		const int symbol = pInput[i];
		const unsigned value = pCodes[symbol].value;
		const unsigned numBits = pCodes[symbol].numBits;
		binaryOutputBits(pOutput, value, numBits);
	}
	binaryOutputBits(pOutput, pCodes[END_OF_STREAM].value, pCodes[END_OF_STREAM].numBits);
}

static size_t	uncompressData(BitFile* const pInput, char* const pOutput, const size_t maxOutputSize, 
														const int rootNode, const TreeNode* const pNodes, const size_t debugFlag)
{
	size_t numOutputBytes = 0;

	while (1)
	{
		int code;
		int node = rootNode;
		do
		{
			unsigned long bitValue = binaryInputBits(pInput, 1);
#if 0
			printf("bit=%ld\n", bitValue);
#endif

			if (bitValue == 0)
			{
				node = pNodes[node].child0;
			}
			else
			{
				node = pNodes[node].child1;
			}
		} while (node > END_OF_STREAM);

		if (node == END_OF_STREAM)
		{
			break;
		}
		code = node;
#if 1
		if (debugFlag > 0)
		{
			printf("%c", code);
			if (code == '\0')
			{
				printf("\n");
			}
		}
#endif
		pOutput[numOutputBytes] = (char)code;
		numOutputBytes++;
		if (numOutputBytes >= maxOutputSize)
		{
			break;
		}
	}
	
	return numOutputBytes;
}

/* Public API functions */
void compressInput(const char* const pInput, const size_t numBytes, BitFile* const pOutput, const unsigned int debugFlag)
{
	unsigned long* pCounts = NULL;
	TreeNode* pNodes = NULL;
	SymbolCode* pCodes = NULL;
	int rootNode = -1;

	pCounts = (unsigned long*)malloc(MAX_NUM_SYMBOLS*sizeof(unsigned long));
	memset(pCounts, 0, sizeof(unsigned long)*MAX_NUM_SYMBOLS);
	pNodes = (TreeNode*)malloc(MAX_NUM_NODES*sizeof(TreeNode));
	memset(pNodes, 0, sizeof(TreeNode)*MAX_NUM_NODES);
	pCodes = (SymbolCode*)malloc(MAX_NUM_CODES*sizeof(SymbolCode));
	memset(pCodes, 0, sizeof(SymbolCode)*MAX_NUM_CODES);
	
	makeCounts(pInput, numBytes, pCounts);
	scaleCounts(pCounts, pNodes);
	outputCounts(pOutput->pFile, pNodes);
	rootNode = buildTree(pNodes);
	convertTreeToCode(pNodes, pCodes, 0, 0, rootNode);
	if (debugFlag > 0)
	{
		printf("RootNode 0x%03X\n", rootNode);
		printModel(pNodes, pCodes);
	}

	compressData(pInput, numBytes, pOutput, pCodes);

	free(pCounts);
	free(pNodes);
	free(pCodes);
}

size_t uncompressInput(BitFile* const pInput, char* const pOutput, const size_t maxOutputSize, const unsigned int debugFlag)
{
	TreeNode* pNodes = NULL;
	SymbolCode* pCodes = NULL;
	int rootNode = -1;
	size_t outputSize = 0;

	pNodes = (TreeNode*)malloc(MAX_NUM_NODES*sizeof(TreeNode));
	memset(pNodes, 0, sizeof(TreeNode)*MAX_NUM_NODES);
	if (debugFlag > 0)
	{
		pCodes = (SymbolCode*)malloc(MAX_NUM_CODES*sizeof(SymbolCode));
		memset(pCodes, 0, sizeof(SymbolCode)*MAX_NUM_CODES);
	}

	inputCounts(pInput->pFile, pNodes);
	rootNode = buildTree(pNodes);
	if (debugFlag > 0)
	{
		convertTreeToCode(pNodes, pCodes, 0, 0, rootNode);
		printf("RootNode 0x%03X\n", rootNode);
		printModel(pNodes, pCodes);
	}

	outputSize = uncompressData(pInput, pOutput, maxOutputSize, rootNode, pNodes, debugFlag);

	free(pNodes);
	free(pCodes);
	return outputSize;
}

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
	unsigned int code;
	unsigned int bits;
} SymbolCode;

#define MAX_SYMBOL_VALUE (255)
#define MAX_NUM_SYMBOLS (MAX_SYMBOL_VALUE+1)
#define END_OF_STREAM (MAX_NUM_SYMBOLS)
#define MAX_NUM_CODES (MAX_NUM_SYMBOLS+1)
#define MAX_NUM_NODES (MAX_NUM_CODES*2)
#define	MAX_NODE (MAX_NUM_NODES-1)

/* Internal private static functions */

static void makeCounts(const char* const pSource, const int numBytes, unsigned long* const pCounts)
{
	int i;
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
	}
	pNodes[END_OF_STREAM].count = 1;
}

/* Format is: startSymbol, stopSymbol, count0, count1, count2, ... countN, ..., 0 */
/* When finding the start, stop symbols only break out if find more than 3 0's in the counts */
static void outputCounts(BitFile* const pOutput, const TreeNode* const pNodes)
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
		for (; ;)
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
		putc(first, pOutput->pFile);
		putc(last, pOutput->pFile);
		for (i = first; i <= last; i++)
		{
			const unsigned int count = pNodes[i].count;
			putc((int)count, pOutput->pFile);
		}
	}
	putc(0, pOutput->pFile);
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
	}

	nextFree--;
	pNodes[nextFree].savedCount = pNodes[nextFree].count;

	return nextFree;
}

static void convertTreeToCode(const TreeNode* const pNodes, SymbolCode* const pCodes, 
											 				const unsigned int code, const unsigned int bits, const int node)
{
	unsigned int nextCode;
	unsigned int nextBits;
	if (node <= END_OF_STREAM)
	{
		pCodes[node].code = code;
		pCodes[node].bits = bits;
		return;
	}
	nextCode = code<<1;
	nextBits = bits+1;
	convertTreeToCode(pNodes, pCodes, nextCode, nextBits, pNodes[node].child0);
	nextCode = nextCode | 0x1;
	convertTreeToCode(pNodes, pCodes, nextCode, nextBits, pNodes[node].child1);
}

static void printChar(const int c)
{
	if (c >= ' ' && c < 127)
	{
		printf("'%c'", c);
	}
	else
	{
		printf("0x%02X", c);
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
				binaryFilePrint(stdout, pCodes[i].code, pCodes[i].bits);
			}
			printf("\n");
		}
	}
}

static void compressData(const char* const pInput, const int numBytes, BitFile* const pOutput, const SymbolCode* const pCodes)
{
	int i;
	for (i = 0; i < numBytes; i++)
	{
		const int symbol = pInput[i];
		const unsigned code = pCodes[symbol].code;
		const unsigned bits = pCodes[symbol].bits;
		binaryOutputBits(pOutput, code, bits);
	}
	binaryOutputBits(pOutput, pCodes[END_OF_STREAM].code, pCodes[END_OF_STREAM].bits);
}

#if 0
static void inputCounts(BitFile* const pOutput, TreeNode* const pNodes);
static void uncompressData(BitFile* const pInput, void* const pOutput, int* const pNumBytes, const TreeNode* const pNodes, const int rootNode);

#endif /* #if 0 */

/* Public API functions */
void compressInput(const char* const pInput, const int numBytes, BitFile* const pOutput, const int debugOutputModel)
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
	printf("outputCounts\n");
	outputCounts(pOutput, pNodes);
	rootNode = buildTree(pNodes);
	convertTreeToCode(pNodes, pCodes, 0, 0, rootNode);
	if (debugOutputModel)
	{
		printModel(pNodes, pCodes);
	}
	compressData(pInput, numBytes, pOutput, pCodes);
	free(pCounts);
	free(pNodes);
	free(pCounts);
}


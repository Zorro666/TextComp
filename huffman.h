#ifndef HUFFMAN_HH
#define HUFFMAN_HH

#include "binaryUtil.h"

typedef unsigned char byte;

void compressInput(const byte* const pInput, const size_t numBytes, BitFile* const pOutput, const unsigned int debugFlag);
size_t uncompressInput(BitFile* const pInput, byte* const pOutput, const size_t maxOutputSize, const unsigned int debugFlag);

#endif /* #ifndef HUFFMAN_HH */

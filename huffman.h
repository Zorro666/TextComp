#ifndef HUFFMAN_HH
#define HUFFMAN_HH

#include "binaryUtil.h"

void compressInput(const char* const pInput, const size_t numBytes, BitFile* const pOutput, const unsigned int debugFlag);
size_t uncompressInput(BitFile* const pInput, char* const pOutput, const size_t maxOutputSize, const unsigned int debugFlag);

#endif /* #ifndef HUFFMAN_HH */

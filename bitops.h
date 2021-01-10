#ifndef HUFFMAN_BITOPS_H
#define HUFFMAN_BITOPS_H

#include <iostream>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class BitWriter {
private:
    unsigned char byte;
    int sizeOfByte;
    std::ostream *out;

public:
    BitWriter(std::ostream& os);
    std::vector<int> bitsWritten;
    void writeBit(char bit);
    void writeByte(char c);
    void flush();
};

class BitReader {
private:
    char byte;
    int bitsRemainingInByte;
    std::istream *in;
    int bits_read = 0;
    int bytes_read = 0;
    char input_byte[10];
    int bytes_remaining = 0;

public:
    BitReader(std::istream& is);
    
    // returns either a 0 or 1, coming from `BitReader::byte`
    bool readBit();
    char readByte();
    // void segfault_sigaction(int signal, siginfo_t *si, void *arg);
};

#endif // HUFFMAN_BITOPS_H
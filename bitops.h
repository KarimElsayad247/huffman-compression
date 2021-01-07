#ifndef HUFFMAN_BITOPS_H
#define HUFFMAN_BITOPS_H

#include <iostream>
#include <vector>

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

public:
    BitReader(std::istream& is);
    
    // returns either a 0 or 1, coming from `BitReader::byte`
    bool readBit();
    char readByte();
};

#endif // HUFFMAN_BITOPS_H
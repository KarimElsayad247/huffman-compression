#ifndef HUFFMAN_BITOPS_H
#define HUFFMAN_BITOPS_H

#include <iostream>

class BitWriter {
    private:
    unsigned char byte = 0;
    int sizeOfByte = 0;
    std::ostream *out;

    public:
    BitWriter(std::ostream& os);
    void writeBit(bool bit);
    void flush();
};

#endif // HUFFMAN_BITOPS_H
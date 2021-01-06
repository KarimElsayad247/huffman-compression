// iostream already included
#include "bitops.h"

using namespace std;

BitWriter::BitWriter(ostream& os) {
    byte = 0;
    sizeOfByte = 0;
    out = &os;  
}

void BitWriter::writeBit(bool bit) {
 
    // OR with new value
    // shift once to the left
    // since it starts with all 0s, first shift is negligible 
    byte <<= 1; 
    byte |= bit;    
    sizeOfByte++;
    
    if (sizeOfByte > 7) {
       
        // write it out
        *out << byte;

        // reset byte
        byte = 0;
        sizeOfByte = 0;
    }
}


/*
 * Last byte might not be full so it needs to be zero padded
 * 
*/
void BitWriter::flush() {
    if (sizeOfByte < 8 && sizeOfByte != 0) {
        // zero pad until last byte is 8 bits wide
        byte <<= (8 - sizeOfByte);
        *out << byte;
    }
}
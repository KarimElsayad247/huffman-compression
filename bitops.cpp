// iostream already included
#include "bitops.h"
#include <iostream>
#include <iomanip>
#include <exception>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

BitWriter::BitWriter(ostream& os) {
    byte = 0;
    sizeOfByte = 0;
    out = &os;  
}

void BitWriter::writeBit(char bit) {
 
    // OR with new value
    // shift once to the left
    // since it starts with all 0s, first shift is negligible 
    byte <<= 1; 
    byte |= bit;   
    bitsWritten.push_back(bit); 
    sizeOfByte++;
    
    if (sizeOfByte > 7) {
       
        // write it out
        *out << byte;

        // reset byte
        byte = 0;
        sizeOfByte = 0;
    }
}


// Last byte might not be full so it needs to be zero padded
void BitWriter::flush() {
    if (sizeOfByte < 8 && sizeOfByte != 0) {
        // zero pad until last byte is 8 bits wide
        byte <<= (8 - sizeOfByte);
        *out << byte;
    }
}

void BitWriter::writeByte(char c)
{
    bool bit; 
    for (int i = 0; i < 8; ++i) {
        bit = c & 0b10000000;
        writeBit(bit);
        // cout << bit;
        c <<= 1; // last shift is negligible 
    }
}

BitReader::BitReader(istream& is) {
    // assign argument istream to in
    // this is the stream from which we read bits
    byte = 0;
    bitsRemainingInByte = 0;
    in = &is;
}


bool BitReader::readBit() {

try
{
    if (bytes_remaining < 1) {
        in->read(input_byte, 10);
        bytes_remaining = 10;
        bytes_read++;
    }

    bits_read++;
    if (bitsRemainingInByte < 1) {
        byte = input_byte[10 - bytes_remaining];
        bitsRemainingInByte = 8;
        bytes_remaining--;
        // cout << bits_read << endl;
    }
}
catch(const std::exception& e)
{
    std::cerr << e.what() << '\n';
    exit(0);
}


    // // read a byte when we run out of bits to read
    // // also reads at the very start
    // if (bitsRemainingInByte < 1) {
    //     in->get(byte);
    //     bitsRemainingInByte = 8;
    // }

    // ANDing with rightmost bit gets either 0 or 1
    bool bit = byte & 0b10000000;
    
    // shift the byte once to the left to get next bit
    byte <<= 1;

    // with that, the amount of remaining non-read bits decreases by one
    bitsRemainingInByte--;

    return bit;
}

char BitReader::readByte() 
{
    // basically just read 8 bits
    char result = 0;
    for (int i = 0; i < 8; i++) {
        result <<= 1;
        bool bit = readBit(); 
        result |= bit;
    }

    return result;
}

// void BitReader::segfault_sigaction(int signal, siginfo_t *si, void *arg)
// {

// }
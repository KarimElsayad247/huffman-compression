#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include <stdlib.h>

using namespace std;

/*
 * THINGS TO DO:
 *  
 * - Read a file
 *  
 * - Create a huffman map from file
 *  
 * - after getting code for each character, return a map 
 *  
 * - now we must translate each string in file into a series of 0s an 1s
 *   and put that somehow into an array
 * 
 * - It's then easy to squash that array of bits into small output,
 *   BUT output must contain the compression codes
 * 
 * - now comes decoding phase, read that file, extract metadata and actual data
 *   You need to somehow read bits of a character sequentially and translate them using 
 *   codes in metadata
 * 
*/

void to_binary_string(unsigned char c, char* buffer)
{
    unsigned char mask = 0b00000001;
    for (int i = 7; i >= 0; --i) {
        buffer[i] = c & mask;
        c >>= 1;
    }
}

int main(int argc, char** argv) 
{
    /* TODO: READ FROM FILE 
     *
    */

    map<char, char> codes;

    codes['a'] = 0b0;
    codes['b'] = 0b00000010;

    string sample = "aaaaabbbaabbaabbaabbaaba";

    if (argc != 2) {
        cout << "NO INPUT FILE ARGUMENT" << endl;
        exit(1);
    }

    // string input;
    // ifstream file;
    // file.open("sample.txt");
    // file >> input;
    // file.close();
    // cout << input << endl;

    vector<int> output;

    for (unsigned char c : sample) {
        
        // first you need to get the bytes of the code
        int x = codes[c];

        output.push_back(x);
    }

    for (int i : output) {
        cout << i << " ";
    }

    string outputString;
    unsigned char byte;
    int sizeOfByte;

    size_t i = 0;
    while (i < output.size()) {

        byte = 0;
        sizeOfByte = 0;

        byte |= output[i++];
        sizeOfByte++;
        while (sizeOfByte < 8 && i < output.size()) {
            byte <<= 1;             // shift once to the left
            byte |= output[i++];    // OR with new value
            sizeOfByte++;
        }

        // if byte is not full, zero pad until full
        if (sizeOfByte < 8) {
            byte <<= (8 - sizeOfByte);
        }

        outputString.push_back(byte);
    }


    cout << endl;

    cout << "Compressed result is: " << outputString << endl;
    cout << "It's size = " << outputString.size() << endl;

    char buffer[8];
    to_binary_string(outputString[0], buffer);
    
    cout << buffer << endl;
    for (int i : buffer)
        cout << i;
    cout << endl;

    /*
     * DECODING
     * 
     * - read a character
     * 
     * - convert to binary form (itoa(char, buffer, base))
     *   SOLUTION: use my  homebrew brand new function(tm)
     * 
     * - read leftmost bit 
     * 
     * - while reading, try to decode
     * -- if it exists, put it on decoded output
     * -- if if doesn't, fetch another bit and check again
     * 
     */

    int total_characters_to_decode = sample.size();
    cout << total_characters_to_decode << endl;

    // as we go through output, single characters might span more than one byte
    int currentByteFromOutputThatShiftsIntoDecoding = 0;

    // the thing you want to decode. we wanna decode all
    char input_cipher = outputString[currentByteFromOutputThatShiftsIntoDecoding]; 
    
    // last cipher would have a string of 0s with no meaning
    // keep track of all characters we have decoded and stop when we are done
    int decoded_characters = 0;

    // we store decoded_output here
    vector<char> decoded_output;

    map<int, char> decoding_codes;
    decoding_codes[0b0] = 'a';
    decoding_codes[0b00000010] = 'b';

    // cout << decoding_codes[0] << endl;
    // cout << decoding_codes[1] << endl;
    // cout << (decoding_codes.find(0) == decoding_codes.end()) << endl;

    // we gonna put left-most bit into some input
    // not gonna be a simple char because one letter might be coded into
    // more than 8 bit. bad times I tell ya
    // it starts off at 0, i.e. all bits are 0
    int thing_to_check_against_codes = 0;

    // get the bits of the onput cipher. we gonna fill it again only when we use all its bits
    to_binary_string(input_cipher, buffer); // now we have some zeros and ones in that buffer

    // remember that buffer? we gonna read more of it if we read all of it
    int bufferBitsShiftetIntoVariable = 0;

    while (decoded_characters < total_characters_to_decode) {

        // we gonna read those zeros and ones one by one, put them in thing_to_check_against_codes 
        // and check them to see if they have a corresponding code. for that we use a map we got from 
        // file header which contained the codes. we ain't stoppin' tell we get a character
        bool found_character = false;
        
        // if have found a character, it means we should start reading codes from scratch
        // so this will be 0 becuase no input was shifted in
        thing_to_check_against_codes = 0;

        while (!found_character) {

            // this starts at 0, then incremented as we go through buffer.
            // 0 0 0 0 0 1 1 1
            // 0 1 2 3 4 5 6 7
            // also we know for sure we have a buffer full of bits to shift in,
            // and we still haven't found a character, we shift in the actual bits
            thing_to_check_against_codes |= buffer[bufferBitsShiftetIntoVariable++];

            // check if the current cipher has a correspoinding character
            auto character_iter = decoding_codes.find(thing_to_check_against_codes);
            
            // we compare with end to see if it exists
            if (character_iter != decoding_codes.end()) {
                
                // BINGO! We found a character -- CODE CRACKED
                found_character = true;
                decoded_output.push_back((*character_iter).second);
                decoded_characters++;
            }
            else {
                // if it doesn't exist, then we shift more bits into thing to check
                // first, shift it by one to make room
                thing_to_check_against_codes <<= 1;
            }
            
            // if we already read 8 bits from buffer, fetch more input
            // it goes from 0 to 7, so higher means we done
            if (bufferBitsShiftetIntoVariable > 7) {
                currentByteFromOutputThatShiftsIntoDecoding++;
                input_cipher = outputString[currentByteFromOutputThatShiftsIntoDecoding];
                to_binary_string(input_cipher, buffer);

                // reset some values
                bufferBitsShiftetIntoVariable = 0;
            }
        }
    }

    cout << "The original string was *drum rolls*:" << endl;
    cout << string(decoded_output.begin(), decoded_output.end()) << endl;
    
    to_binary_string(outputString[0], buffer);
    cout << "input 0: "; 
    for (int i : buffer)
        cout << i;
    cout << endl;

    to_binary_string(outputString[1], buffer);
    cout << "input 1: ";
    for (int i : buffer)
        cout << i;
    cout << endl;    
    
    to_binary_string(outputString[2], buffer);
    cout << "input 2: ";
    for (int i : buffer)
        cout << i;
    cout << endl;
    return 0;
}
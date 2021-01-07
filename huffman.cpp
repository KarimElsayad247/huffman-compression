#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include <stdlib.h>

#include "bitops.h"
#include "huffmanTree.h"

using namespace std;

/* THINGS TO DO:
 *  
 * - Read a file    _/
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
        cout << (c & mask);
        c >>= 1;
    }
}

// so far programme only takes one argument that is the name 
// of the file to be compressed 
int main(int argc, char** argv) 
{
    if (argc != 2) {
        cout << "NO INPUT FILE ARGUMENT" << endl;
        exit(1);
    }  

    char* filename = argv[1];
 
    // read the input file requiring compression into ~~a string~~ no longer reading into a string
    // we create an ifstream and iterate through
    ifstream file;
    file.open(filename);
    if (!file.is_open() ) {
        cout << "Error opening file" << endl;
    }

    // build the frequency vector
    map<char, int> frequency;
    char c;
    while (file.get(c)) {
        frequency[c] += 1;
    }
    file.close();

    // using the frequency vector, build the haffman tree and get the codes
    HuffmanTree tree(frequency);
    map<char, vector<bool>> huffmanBuiltCodes = tree.huffmanCodes();

    char buffer[8];

    // // // checking to see if the codes make sense
    cout << "Initial codes" << endl;
    cout << "Byte" << "\t" << "Code" << "\t" << "New code" << endl; 
    for (auto i = huffmanBuiltCodes.begin(); i != huffmanBuiltCodes.end(); ++i) {
        cout << (i->first) << ":\t";
        to_binary_string(i->first, buffer);
        // string s = buffer;
        cout <<"\t";
        for (bool n : i->second) {
            cout << n;
        }
        cout << endl;
    }
    
    // vector to store the bits after compression
    vector<int> output;
    unsigned int numEncodedBytes = 0;
    file.open(filename);
    while (file.get(c)) {
        for (auto i = huffmanBuiltCodes[c].begin(); i != huffmanBuiltCodes[c].end(); ++i)
            output.push_back(*i);
        numEncodedBytes++;
    }
    file.close();

    ofstream myfile;
    myfile.open("res.huff", ios::out);
    BitWriter fileWriter(myfile);
    
    // the number of characters to expect
    myfile << numEncodedBytes;

    // now, construct a header
    tree.buildHeader(fileWriter);


    // fileWriter.flush();
    // myfile.close();
    // return -1;

    // write compressed data into a file (BODY)
    for (bool i : output) {
        fileWriter.writeBit(i);
    }
    fileWriter.flush();
    myfile.close();

    // open the file to decompress and create a BitReader class
    // assiciated with said file
    ifstream fileToDecode;
    fileToDecode.open("res.huff", ios::in);

    if (!fileToDecode.is_open()) {
        cout << "error opening file: res.huff" << endl;
    }

    BitReader newReader(fileToDecode);
    
    // The tree which will be constructed from file header
    HuffmanTree newTree;

    // the first 4 bytes (unsigned int) 
    // represent number of characters to expect
    unsigned int numCharactersExpected;
    fileToDecode >> numCharactersExpected;

    // read the header and construct the huffman tree again
    // we will travers the tree to decode characters
    newTree.constructTreeFromHeader(newReader);

    // // view reconstructed code
    // cout << "reconstructed codes" << endl;
    // map<char, vector<bool>> reconstructed_codes = newTree.huffmanCodes();
    // for (auto i = reconstructed_codes.begin(); i != reconstructed_codes.end(); ++i) {
    //     cout << (i->first) << ": ";
    //     for (bool n : i->second) {
    //         cout << n;
    //     }
    //     cout << endl;
    // }

    // for (int i = 0; i < 1000; ++i) {
    //     cout << newReader.readBit();
    // }
    // cout << endl;

    // decode the file
    cout << "decoding now: decoded output is: " << endl;
    unsigned int numDecodedCharacters = 0;
    int bitsRead = 0;
    while (numDecodedCharacters < numCharactersExpected) {
        if (tree.traversWithBitUntilLeafReached(newReader.readBit())){
            cout << tree.decodedCharacter;
            numDecodedCharacters++;
        }
        bitsRead++;
    }
    fileToDecode.close();
    // cout << endl << bitsRead << endl;
    return 0;
}
#ifndef HUFFMAN_HUFFMANTREE_H
#define HUFFMAN_HUFFMANTREE_H


#include <map>
#include <vector>
#include <list>

#include "bitops.h"

class Node {
public:
    int val;
    unsigned char c;
    Node *left;
    Node *right;
    Node(int v); 
    Node(int v, unsigned char letter);
    Node(int v, Node *left, Node *right);
    bool isLeaf();
};

class HuffmanTree {
private:
    Node *root;
    Node *currentNodeWhileTraversing;
    std::map<char, std::vector<bool>> codes;
    std::list<bool> single_code;
    void buildCodeMap(Node *root, int depth);
    void encodeHuffmanNode(Node *node, BitWriter& writer);
    Node *readHuffmanNode(BitReader& reader);
public:
    char decodedCharacter;
    HuffmanTree();
    HuffmanTree(std::map<char, int> frequency);
    std::map<char, std::vector<bool>> huffmanCodes();
    void buildHeader(BitWriter& writer);
    void constructTreeFromHeader(BitReader& reader);
    bool traversWithBitUntilLeafReached(bool bit);
};


#endif // HUFFMAN_HUFFMANTREE_H
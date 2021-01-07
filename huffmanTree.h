#ifndef HUFFMAN_HUFFMANTREE_H
#define HUFFMAN_HUFFMANTREE_H


#include <map>
#include <vector>
#include <list>

class Node {
public:
    int val;
    unsigned char c;
    Node *left;
    Node *right;
    Node(int v); 
    Node(int v, unsigned char letter);
    bool isLeaf();
};

class HuffmanTree {
private:
    Node *root;
    std::map<char, std::vector<bool>> codes;
    std::list<bool> single_code;
    void buildCodeMap(Node *root, int depth);
public:
    HuffmanTree(std::map<char, int> frequency);
    std::map<char, std::vector<bool>> huffmanCodes();
    std::string buildHeader();
};


#endif // HUFFMAN_HUFFMANTREE_H
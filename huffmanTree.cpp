#include <vector>
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>
#include <list>
#include <queue>

// included <map>
#include "huffmanTree.h"

using namespace std;

void HuffmanTree::buildCodeMap(Node *root, int depth)
{
    if (!(root->isLeaf())) {
        single_code.push_back(0);
        buildCodeMap(root->left, depth + 1);
        single_code.pop_back();

        single_code.push_back(1);
        buildCodeMap(root->right, depth + 1);
        single_code.pop_back();
    }
    else {
        codes[root->c] = vector<bool>(single_code.begin(), single_code.end());
    }
}

Node::Node(int v) {
    val = v;
    left = nullptr;
    right = nullptr;
}

Node::Node(int v, unsigned char letter) {
    val = v;
    c = letter;
    left = nullptr;
    right = nullptr;
}

Node::Node(int v, Node *left, Node *right) 
{
    val = v;
    this->left = left;
    this->right = right;
}

bool Node::isLeaf() 
{
    return (this->left == nullptr && this->right == nullptr);
}

class sortingPredicat {
    public:
    bool operator()(Node *p1, Node *p2) {
        return (p1->val > p2->val);
    }
};

HuffmanTree::HuffmanTree()
{
    root = nullptr;
}

HuffmanTree::HuffmanTree(map<char, int> frequency) {
    
    root = nullptr;

    // build nodes for all character input
    priority_queue<Node*, vector<Node*>, sortingPredicat > nodes;
    map<char, int>::iterator p;
    for (p = frequency.begin(); p != frequency.end(); ++p) {
        nodes.push(new Node(p->second, p->first));
    }

    // proceed to build the tree
    while (nodes.size() > 1) {

        Node *first = nodes.top();
        nodes.pop();
        Node *second = nodes.top();
        nodes.pop();

        // children of this new node will be the popped children
        Node *newNode = new Node(first->val + second->val);
        newNode->left = first;
        newNode->right = second;

        nodes.push(newNode);

    }

    root = nodes.top();
    currentNodeWhileTraversing = root;
    nodes.pop();
}

map<char, vector<bool> > HuffmanTree::huffmanCodes()
{
    this->buildCodeMap(root, 0);
    return codes;
}

// used to construct the header
void HuffmanTree::encodeHuffmanNode(Node *node, BitWriter& writer)
{
    if (node->isLeaf()) {
        writer.writeBit(1);
        // cout << 1;
        writer.writeByte(node->c);
        // cout << node->c << endl;
    }
    else {
        writer.writeBit(0);
        // cout << 0 << endl;
        encodeHuffmanNode(node->left, writer);
        encodeHuffmanNode(node->right, writer);
    }
}

Node *HuffmanTree::readHuffmanNode(BitReader& reader)
{
    // 1 means the following 8 bits represent a character
    // bool bit = reader.readBit();
    if (reader.readBit() == 1) {
        return new Node(0, reader.readByte());
    }
    else {
        Node *leftchild = readHuffmanNode(reader);
        Node *rightchild = readHuffmanNode(reader);
        return new Node(0, leftchild, rightchild);
    }
}

void HuffmanTree::buildHeader(BitWriter& writer)
{
    encodeHuffmanNode(root, writer);
    cout << endl;
}

void HuffmanTree::constructTreeFromHeader(BitReader& reader)
{
    root = readHuffmanNode(reader);
    currentNodeWhileTraversing = root;
}

// basically, if you get 0, go left, if you get 1, go right
// if you find a character, return true
bool HuffmanTree::traversWithBitUntilLeafReached(bool bit)
{
    if (bit == 0) {
        currentNodeWhileTraversing = currentNodeWhileTraversing->left;
        // cout << endl << "left" << endl;
    }
    else {
        currentNodeWhileTraversing = currentNodeWhileTraversing->right;
        // cout << endl << "right" << endl;
    }
    
    if (currentNodeWhileTraversing->isLeaf()) {
        decodedCharacter = currentNodeWhileTraversing->c;
        currentNodeWhileTraversing = root;
        return true;
    }
    else {
        return false;
    }
}
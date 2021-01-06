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
    if (root->left != nullptr) {
        single_code.push_back(0);
        buildCodeMap(root->left, depth + 1);
        single_code.pop_back();
    }

    if(root->right != nullptr) {
        single_code.push_back(1);
        buildCodeMap(root->right, depth + 1);
        single_code.pop_back();
    }

    if (root->left == nullptr && root->right == nullptr) {
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

class sortingPredicat {
    public:
    bool operator()(Node *p1, Node *p2) {
        return (p1->val > p2->val);
    }
};

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
    nodes.pop();
    // sort(nodes.begin(), nodes.end(), soritngPredicat);
    // int originalSize = nodes.size();
    // cout << originalSize << endl;
    // for (int i = 0; i < originalSize ; ++i) {
    //     cout << nodes.top()->c << ": " << nodes.top()->val << endl;
    //     nodes.pop();
    // }
    // cout << endl;
}

map<char, vector<bool> > HuffmanTree::huffmanCodes()
{
    this->buildCodeMap(root, 0);
    return codes;
}


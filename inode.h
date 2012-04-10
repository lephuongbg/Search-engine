#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <vector>
#include "document.h"

class INode_data
{
public:
    string word;
    vector<Document> docs;

    vector<string> conjunt(INode_data * node_data);
    vector<string> compose(INode_data * node_data);
};

class INode
{
private:
    INode_data data_;
    INode * left_;
    INode * right_;
    int height_;
    bool initialized;

public:
    INode();
    INode(string w);
    INode * getLeft();
    INode * getRight();
    int getHeight();
    INode_data getData();
    void fixStats();
    void setLeft(INode * node);
    void setRight(INode * node);
    string getWord();
    void setWord(string w);
    vector<Document> getDocs();
};

#endif // INDEXER_H

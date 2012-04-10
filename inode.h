#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>
#include "document.h"


class INodeData
{
private:
    string word_;
    vector<Document> docs_;

public:
    INodeData();
    string word();
    void word(string name);
    vector<Document> docs();
    void docs(Document doc);
    vector<Document> conjunt(INodeData * node_data);
    vector<Document> compose(INodeData * node_data);
};

class INode
{
private:
    INodeData data_;
    INode * left_;
    INode * right_;
    int height_;

public:
    INode(string w);
    const INode * left();
    const INode * right();
    int height();
    INodeData data();
    void fixStats();
    void setLeft(INode * node);
    void setRight(INode * node);
};

#endif // INODE_H

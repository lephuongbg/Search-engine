#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>
#include "inodedata.h"

class INode
{
private:
    INodeData * data_;
    INode * left_;
    INode * right_;
    int height_;

public:
    INode();
    INode(const string &w);
    ~INode();
    INode * left();
    INode *right();
    int height();
    static int getHeight(INode * node);
    INodeData * data();
    void fixStats();
    void setLeft(INode * node);
    void setRight(INode * node);
};

#endif // INODE_H

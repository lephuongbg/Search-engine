#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>
#include "document.h"
#include "inodedata.h"

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

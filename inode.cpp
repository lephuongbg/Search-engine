#include "inode.h"

INode::INode()
{
    initialized = false;
}

INode::INode(string w)
{
    this->data_.word = w;
    initialized = true;
}

INode *INode::getLeft()
{
    return this->left_;
}

INode *INode::getRight()
{
    return this->right_;
}

int INode::getHeight()
{
    return this->height_;
}

INode_data INode::getData()
{
    return this->data_;
}

void INode::fixStats()
{
    this->height_ = max(this->left_->getHeight(), this->right_->getHeight()) + 1;
}

void INode::setLeft(INode *node)
{
    this->left_ = node;
    fixStats();
}

void INode::setRight(INode *node)
{
    this->right_ = node;
    fixStats();
}

string INode::getWord()
{
    return this->data_.word;
}

void INode::setWord(string w)
{
    if (initialized)
        return;
    else
        this->data_.word = w;
}

vector<Document> INode::getDocs()
{
    return this->data_.docs;
}

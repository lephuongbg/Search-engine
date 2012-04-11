#include "inode.h"

/*******************************
 *     CLASS CONSTRUCTOR       *
 *******************************/
INode::INode()
{
    data_ = new INodeData;
    left_ = NULL;
    right_ = NULL;
    height_ = 1;
}

INode::INode(const string &w)
{
    data_ = new INodeData(w);
    left_ = NULL;
    right_ = NULL;
    height_ = 1;
}

INode::~INode()
{
    if (left_)
        delete left_;
    if (right_)
        delete right_;
    delete data_;
}

/************************************************************
 *  GET POINTER OF LEFT CHILD AND RIGHT CHILD OF THE INODE  *
 ************************************************************/
INode *INode::left()
{
    return left_;
}

INode *INode::right()
{
    return right_;
}

/*************************************
 *    GET THE HEIGHT OF THE NODE     *
 *************************************/
int INode::height()
{
    return height_;
}

int INode::getHeight(INode *node)
{
    if (node == NULL)
        return 0;
    return node->height();
}

/*************************************
 *    GET THE DATA OF THE NODE       *
 *************************************/
INodeData * INode::data()
{
    return data_;
}

/*************************************
 *  RECALCULATE HEIGHT OF THE NODE   *
 *************************************/
void INode::fixStats()
{
    height_ = max(getHeight(left_), getHeight(this->right_)) + 1;
}

/**************************************************************
 *   POINT THE LEFT AND RIGHT CHILD POINTER TO OTHER INODE    *
 **************************************************************/
void INode::setLeft(INode *node)
{
    left_ = node;
    fixStats();
}

void INode::setRight(INode *node)
{
    right_ = node;
    fixStats();
}

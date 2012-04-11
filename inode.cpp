#include "inode.h"

/*******************************
 *     CLASS CONSTRUCTOR       *
 *******************************/
INode::INode()
{
    data_ = new INodeData;
    this->left_ = NULL;
    this->right_ = NULL;
    this->height_ = 1;
}

INode::INode(const string &w)
{
    this->data_ = new INodeData(w);
    this->left_ = NULL;
    this->right_ = NULL;
    this->height_ = 1;
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
    return this->left_;
}

INode *INode::right()
{
    return this->right_;
}

/*************************************
 *    GET THE HEIGHT OF THE NODE     *
 *************************************/
int INode::height()
{
    return this->height_;
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
    this->height_ = max(getHeight(left_), getHeight(this->right_)) + 1;
}

/**************************************************************
 *   POINT THE LEFT AND RIGHT CHILD POINTER TO OTHER INODE    *
 **************************************************************/
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

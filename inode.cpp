#include "inode.h"

/*******************************
 *     CLASS CONSTRUCTOR       *
 *******************************/
INode::INode(string w)
{
    this->data_.word(w);
    this->left_ = NULL;
    this->right_ = NULL;
    this->height_ = 1;
}

/************************************************************
 *  GET POINTER OF LEFT CHILD AND RIGHT CHILD OF THE INODE  *
 ************************************************************/
const INode *INode::left()
{
    return this->left_;
}

const INode *INode::right()
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

/*************************************
 *    GET THE DATA OF THE NODE       *
 *************************************/
INodeData INode::data()
{
    return this->data_;
}

/*************************************
 *  RECALCULATE HEIGHT OF THE NODE   *
 *************************************/
void INode::fixStats()
{
    this->height_ = max(this->left_->height(), this->right_->height()) + 1;
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

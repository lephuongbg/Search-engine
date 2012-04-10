#include "indexer.h"
#include <cstdlib>

Indexer::Indexer()
{
    indexer_ = NULL;
}

Indexer::~Indexer()
{
    if (indexer_)
        delete indexer_;
}

void Indexer::insertKey(const string & keyword)
{
    indexer_ = insertKey(indexer_, keyword);
}

INode * Indexer::insertKey(INode * node, const string &keyword)
{
    if (node == NULL)
    {
        INode * new_node = new INode(keyword);
        return new_node;
    }
    if (keyword <= node->data().word())
    {
        node->setLeft(insertKey(node->left(), keyword));
        return reBalance(node);
    }
    else
    {
        node->setRight(insertKey(node->right(), keyword));
        return reBalance(node);
    }
}

INode * Indexer::reBalance(INode *node)
{
    if (node == NULL)
        return node;

    int leftHeight = node->left()->height();
    int rightHeight = node->right()->height();
    if (abs(leftHeight - rightHeight) < 2)
        return node;

    // Check if we need to rotate the node right or left
    bool rotateRight = leftHeight < rightHeight;
    // We rotate the node right when left height is larger than right height
    if (rotateRight)
    {
        INode * left = node->left();
        INode * lChild = left->left();
        INode * rChild = left->right();
        //Left-left case
        if (lChild->height() > rChild->height())
        {
            node->setLeft(rChild);
            left->setRight(node);
            node = left;
        }
        //Left-right case
        else
        {
            node->setLeft(rChild->right());
            left->setRight(rChild->left());
            rChild->setLeft(left);
            rChild->setRight(node);
            node = rChild;
        }
    }
    // Rotate the node left when left height is smaller than right height
    else
    {
        INode * right = node->right();
        INode * lChild = right->left();
        INode * rChild = right->right();
        //Right-right case
        if (rChild->height() > lChild->height())
        {
            node->setRight(rChild->left());
            rChild->setLeft(node);
            node = right;
        }
        //Right-left case
        else
        {
            node->setRight(lChild->left());
            right->setLeft(lChild->right());
            lChild->setLeft(node);
            lChild->setRight(right);
            node = lChild;
        }
    }

    return node;
}

INode *Indexer::find(const string &keyword)
{
    INode * ptr = indexer_;
    while (ptr != NULL && keyword != ptr->data().word())
    {
        if (keyword < ptr->data().word())
            ptr = ptr->left();
        else
            ptr = ptr->right();
    }

    return ptr;
}

void Indexer::setQuery(const string &query)
{
}

void Indexer::excute()
{
    stack<string> s_operator;
    stack<vector<Document>> s_operand;
    Document result;
    string buffer;
    
    while (!this->query_.empty())
    {
        buffer = this->query_.pop();
        if (buffer == "AND")
        {
            S_operator.push(buffer);
        }
        else if (buffer == "OR")
        {
            if (this->query_.top() == "AND")
            {
                s_operator.pop();
                result = Document::conjunct(s_operand.pop(), s_operand.pop();
                s_operand.push( result );
            }
            s_operator.push(buffer);
        }
        else
        {
            s_operand.push(indexer[buffer].docs());
        }
    }
    
    while (!s_operator.empty())
    {
        if (s_operator.pop() == "AND")
        {
            result = Document::conjunct(s_operand.pop(), s_operand.pop();
            s_operand.push( result );
        }
        else
        {
            result = Document::disjunct(s_operand.pop(), s_operand.pop();
            s_operand.push( result );
        }
    }
    
    this->result_ = result;
}

void Indexer::addDocument(const string &docname)
{
}

vector<Document> Indexer::operator[](const string &keyword)
{
    vector<Document> result;
    INode * keynode = this->find(keyword);
    if (keynode == NULL)
    {
        this->insertKey(keyword);
    }
    else
    {
        result = keynode->data().docs();
    }

    return result;
}

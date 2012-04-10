#include "indexer.h"
#include <cstdlib>
#include <iostream>

// CONSTRUCTOR
Indexer::Indexer()
{
    indexer_ = NULL;
}

// DESTRUCTOR
Indexer::~Indexer()
{
    if (indexer_)
        delete indexer_;
}

// INSERT A KEYWORD INTO THE INDEXER
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

// REBALANCE THE INDEXER'S STRUCTURE
INode * Indexer::reBalance(INode *node)
{
    if (node == NULL)
        return node;

    int leftHeight = INode::getHeight(node->left());
    int rightHeight = INode::getHeight(node->right());
    if (abs(leftHeight - rightHeight) < 2)
        return node;

    // Check if we need to rotate the node right or left
    bool rotateRight = leftHeight > rightHeight;
    // We rotate the node right when left height is larger than right height
    if (rotateRight)
    {
        INode * left = node->left();
        INode * lChild = left->left();
        INode * rChild = left->right();
        //Left-left case
        if (INode::getHeight(lChild) > INode::getHeight(rChild))
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
        if (INode::getHeight(rChild) > INode::getHeight(lChild))
        {
            node->setRight(right->left());
            right->setLeft(node);
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

// FIND A NODE IN THE INDEXER CONTAIN PROVIDED KEYWORD
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

// SET A QUERY INTO THE INDEXER
void Indexer::setQuery(const string &query)
{
}

// EXECUTE THE QUERY
void Indexer::excute()
{
}

// INDEX A DOCUMENT
void Indexer::addDocument(const string &docname)
{
}

// OVERLOADING OPERATOR [] FOR FAST ACCESS NODE VALUE BY KEY WORD
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

// PRINT ALL KEYWORDS IN INDEXER
void Indexer::traverse(INode *node)
{
    if (node != NULL)
    {
        traverse(node->left());
        std::cout << node->data().word() << '\n';
        traverse(node->right());
    }
}

// PROVIDE ACCESS TO INDEXER DATA
INode *Indexer::indexer()
{
    return indexer_;
}

void Indexer::setQuery(string queryStr)
{
    string buffer;
    bool flag = false;

    while (!query_.empty())
        query_.pop();
    break;

    while ((pos == queryStr.find_first_not_of(' ')) !=  string::npos)
    {
        queryStr = queryStr.substr(pos);
        pos = queryStr.find_first_of(' ');
        buffer = substr(0, pos);
        queryStr = queryStr.substr( pos );

        if ((buffer == "AND" || buffer == "OR") && flag)
        {
            this->query_.push_back( buffer );
            flag = false;
        }
        else if ((buffer != "AND" && buffer != "OR") && !flag)
        {
            this->query_.push_back( buffer );
            flag = true;
        }
        else
        {
            while (!query_.empty())
                query_.pop();
            break;
        }
    }

    if (!flag)
        query_.pop();
}

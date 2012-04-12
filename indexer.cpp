#include "indexer.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

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
    //If the tree is empty, create a new node for it
    if (node == NULL)
    {
        INode * new_node = new INode(keyword);
        return new_node;
    }
    //If keyword is smaller than current node key, set it to the left
    if (keyword < node->data()->word())
    {
        node->setLeft(insertKey(node->left(), keyword));
        return reBalance(node);
    }
    //If keyword is greater than current node key, set it to the right
    else if (keyword > node->data()->word())
    {
        node->setRight(insertKey(node->right(), keyword));
        return reBalance(node);
    }
    //If keyword has already exist, return the node
    return node;
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
INode *Indexer::at(const string &keyword)
{
    INode * ptr = indexer_;
    while (ptr != NULL && keyword != ptr->data()->word())
    {
        if (keyword < ptr->data()->word())
            ptr = ptr->left();
        else
            ptr = ptr->right();
    }

    return ptr;
}

// SET A QUERY INTO THE INDEXER
void Indexer::setQuery(const string &query)
{
    stringstream strm;
    strm.str(query);
    string buffer;
    bool flag = false;
    
    while (!query_.empty())
        query_.pop();
    
    while (!strm.eof())
    {
        strm >> buffer;
        
        if ((buffer == "AND" || buffer == "OR") && flag)
        {
            query_.push(buffer);
            flag = false;
        }
        else if ((buffer != "AND" && buffer != "OR") && !flag)
        {
            query_.push(buffer);
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

// EXECUTE THE QUERY
void Indexer::execute()
{
    stack<string> s_operator;
    stack< vector<Document> > s_operand;
    vector<Document> o1, o2;
    string buffer;
    
    while (!query_.empty())
    {
        buffer = query_.top();
        query_.pop();
        if (buffer == "AND")
        {
            s_operator.push(buffer);
        }
        else if (buffer == "OR")
        {
            if (query_.top() == "AND")
            {
                s_operator.pop();
                o1 = s_operand.top();
                s_operand.pop();
                o2 = s_operand.top();
                s_operand.pop();
                result_ = Document::conjunct(o1, o2);
                s_operand.push(result_);
            }
            s_operator.push(buffer);
        }
        else
        {
            s_operand.push((*this)[buffer]);
        }
    }
    
    while (!s_operator.empty())
    {
        if (s_operator.top() == "AND")
        {
            s_operator.pop();
            o1 = s_operand.top();
            s_operand.pop();
            o2 = s_operand.top();
            s_operand.pop();
            result_ = Document::conjunct(o1, o2);
            s_operand.push(result_);
        }
        else
        {
            s_operator.pop();
            o1 = s_operand.top();
            s_operand.pop();
            o2 = s_operand.top();
            s_operand.pop();
            result_ = Document::disjunct(o1, o2);
            s_operand.push(result_);
        }
    }
    
    if (!s_operand.empty())
        result_ = s_operand.top();
}

vector<Document> Indexer::result()
{
    // Sort result by score before return it out
    sort(result_.begin(), result_.end(), Document::docFreqComp);
    return result_;
}

// INDEX A DOCUMENT
void Indexer::addDocument(const string &docname)
{
    // Open document
    ifstream docfile;
    docfile.open(docname.c_str());

    //Association value for keyword
    Document doc;
    doc.name(docname);

    string keyword;
    if (docfile.is_open())
    {
        // Check if the file is text file or not
        int c;
        while ( (c = docfile.get()) != EOF && c <= 127 );
        if (c == EOF)
        {
            docfile.clear();
            docfile.seekg(0, ios::beg);
            while (docfile.good())
            {
                // Read each word one by one in the document
                docfile >> keyword;
                // Remove unnecessary character from keyword
                filter(keyword);
                // If keyword is insignificantly important, ignoreit
                if (isIgnore(keyword))
                    continue;
                this->insertKey(keyword);
                this->at(keyword)->data()->docs(doc);
            }
        }
    }
}

// OVERLOADING OPERATOR [] FOR FAST ACCESS NODE VALUE BY KEY WORD
vector<Document> Indexer::operator[](const string &keyword)
{
    vector<Document> result;
    INode * keynode = this->at(keyword);
    // If the keyword doesn't exist, add it to the indexer
    if (keynode == NULL)
    {
        this->insertKey(keyword);
    }
    // otherwise, return all documents containing it
    else
    {
        result = keynode->data()->docs();
    }

    return result;
}

bool Indexer::isIgnore(const string &keyword)
{
    if (keyword.size() == 0)
        return true;
    switch (keyword.at(0))
    {
    // Ignore SGML tags
    case '<':
        return true;
    // Ignore punctuation, comma
    case ',':
    case '.':
        if (keyword.size() == 1)
            return true;
        break;
    default:
        break;
    }

    // If the keyword exists in stop words list, also ignore it
    if (stopwords_.find(keyword) != stopwords_.end())
        return true;
    return false;
}

// REMOVE UNNECESSARY CHARACTER FROM KEYWORD
bool Indexer::isGarbage(char c)
{
    return c == '(' || c == ')' || c == '/';
}

void Indexer::filter(string &keyword)
{
    // Remove all characters defined in isGarbage method
    keyword.resize(std::remove_if(keyword.begin(), keyword.end(), isGarbage) - keyword.begin());

    // Remove comma and punctuation at the end of the word
    if (keyword.size() != 0 && keyword.rfind(',') == keyword.length() - 1)
        keyword.erase(keyword.end()-1);
    if (keyword.size() != 0 && keyword.rfind('.') == keyword.length() - 1)
        keyword.erase(keyword.end()-1);
}

// RETRIEVE STOP WORDS FROM FILE
void Indexer::indexStopWords(const string &wordfile)
{
    ifstream file;
    file.open(wordfile.c_str());
    string stopword;
    if (file.is_open())
    {
        while (file.good())
        {
            // Read words one by one then insert them to the set
            file >> stopword;
            stopwords_.insert(stopword);
        }
    }
}

// PRINT ALL KEYWORDS IN INDEXER
void Indexer::traverse(INode *node)
{
    if (node != NULL)
    {
        traverse(node->left());
        std::cout << node->data()->word() << '\n';
        traverse(node->right());
    }
}

// PROVIDE ACCESS TO INDEXER DATA
INode *Indexer::indexer()
{
    return indexer_;
}

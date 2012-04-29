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
    strm.str(query);  // Init the string stream with the query string
    string buffer;
    bool operatorPhase = false;  // A flag to determine an unit should be operator or operand in a specific phase. Operand phase always come first
    bool error = false;  // A flag to determine that if the input query has error or not
    
    while (! this->query_.empty()) this->query_.pop();  // Fllush out the last query
    this->status_ = SUCCESS;  // Change error code to default SUCCESS value
    
    while (! strm.eof())  // Parse through the string stream
    {
        strm >> buffer;  // Flow into buffer memory
        
        if (buffer == "AND" || buffer == "OR")  // If a operator is stored in buffer memory
        {
            if (operatorPhase)  // and we are in operator phase
        	{
		        this->query_.push(buffer);  // everything is normal and we just push buffer into query stack
		        operatorPhase = false;  // turn the flag to indicate that this phase is ended
			}
			else  // otherwise, we have some user error in query string
			{
				error = true;  // turn the flag
				break;  // and break the whole process
			}
        }
        else  // If a keyword is stored in buffer memory
        {
        	if (operatorPhase)  // and we are in operator phase
        		this->query_.push("AND");  // automatically push in the AND operator and then the key word. Eg: net work ->> net AND work
        	
        	if (isIgnore(buffer))  // If buffer store a stopword
        	{
                this->status_ = STOPWORD_WARNING;
        		if (! this->query_.empty() && this->query_.top() == "AND")  // replace preceding operator with OR
        		{
    				this->query_.pop();
    				this->query_.push("OR");
        		}
        		
        		this->query_.push(buffer);  // push in the stop word
    			this->query_.push("OR");  // push OR right after it
    			strm >> buffer;  // ignore the next operator
        	}
        	else  // otherwise, just a normal keyword
        	{
	        	this->query_.push(buffer);  // push it right into query stack
	        	operatorPhase = true;  // turn the flag to indicate that this phase is ended
	        }
        }           
    }
    
    if (! operatorPhase)  // If operator at the end of query stack
        error = true;  // raise the error flag
        
	if (error)  // If error flag was raised
	{
        this->status_ = SYNTAX_ERROR;  // change the status code
		while (! this->query_.empty()) this->query_.pop();  // fllush out the query stack
		while (! this->result_.empty()) this->result_.pop_back();  // and fllush out the result
	}
}

// EXECUTE THE QUERY
void Indexer::execute()
{
    stack<string> s_operator;  // A stack used to store operator(s)
    stack< vector<Document> > s_operand;  // A stack used to store operand(s)
    vector<Document> d1, d2;
    string buffer;
    
    // Parse through the query stack
    while (! this->query_.empty())
    {
        // Cache the top element
        buffer = this->query_.top();
        this->query_.pop();

        // If buffer contain operator AND - highest priority
        if (buffer == "AND")
        {
            // push it into operator stack right away
            s_operator.push(buffer);
        }
        // else, it is operator OR
        else if (buffer == "OR")
        {        	
            // loop while last operator has higher priority (AND operator)
            while (! s_operator.empty() && s_operator.top() == "AND")
            {
                // we pop the AND operator out
                s_operator.pop();
                
                d1 = s_operand.top();
                s_operand.pop();
                d2 = s_operand.top();
                s_operand.pop();
                
                // and do the math
                result_ = Document::conjunct(d1, d2);
                // push the result into the operand stack as a normal operand
                s_operand.push(result_);
            }
            // push OR into operator stack
            s_operator.push(buffer);
        }
        // else, it is an operand
        else
        {
            // If the keyword is a wildcard
            if (buffer.find("*") != string::npos)
                // push the wildcard filter result into operand stack
                s_operand.push(Indexer::match(this->indexer_, buffer));
            // otherwise,
            else
                // just push it into operand stack for later process
                s_operand.push((*this)[buffer]);
        }
    }
    
    // Parse through operator stack
    while (!s_operator.empty())
    {
        // If the operator is AND
        if (s_operator.top() == "AND")
        {
            // take out the 2 next operands in operand stack
            s_operator.pop();
            d1 = s_operand.top();
            s_operand.pop();
            d2 = s_operand.top();
            s_operand.pop();
            
            // do the conjunct operation
            result_ = Document::conjunct(d1, d2);
            // and then push the result back into operand stack
            s_operand.push(result_);
        }
        // otherwise
        else
        {
            s_operator.pop();
            d1 = s_operand.top();
            s_operand.pop();
            d2 = s_operand.top();
            s_operand.pop();
            
            // do the disjunct operation
            result_ = Document::disjunct(d1, d2);
            // and then push the result back into operand stack
            s_operand.push(result_);
        }
    }
	
    // The last result is the last element left in operand stack
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
    docfile.close();
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

// IGNORE CERTAIN WORDS
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

// HANDLE THE REGEX STRING TO TURN IT INTO A SPECIFIC PATTERN OBJECT. MAKE THE TASK MORE COMFORTABLE.
vector<Document> Indexer::match(INode *node, string regex)
{
    int pos;  // An anchor to determine the position of '*' character
	vector<string> pattern;  // A minified object used to store regex string in an easy-to-handle format
	
	while ((pos = regex.find("**")) != string::npos)  // Standardize the wildcard string
	{
		regex.replace(pos, 2, "*");
	}
	
	while ((pos = regex.find('*')) != string::npos)  // Parse through regex string and use '*' as delimeter
	{
		pattern.push_back(regex.substr(0, pos));  // to cut regex string into small parts used in pattern object
		regex = regex.substr(pos + 1);
	}
	
	pattern.push_back(regex);  // Push the last of regex string into the object to complete the pattern
	return Indexer::match(node, pattern); 
}

// FIND ALL INDEX WITH RIGHT FORMAT
vector<Document> Indexer::match(INode *node, vector<string> pattern)
{
	vector<Document> result;
	if (node == NULL)  // If the node is empty
		return result;  // return an empty result
	
	vector<string> temp;
	string word = node->data()->word(); // Cache out the INode word data value
    unsigned int i;
    int pos;
	string first, last;
	bool suitable = true; // A flag determine if the pattern match the word or not
	first = pattern.at(0), last = pattern.at(pattern.size() - 1);  // Cache the first and last pattern
	
	if ((pos = word.find(first)) == string::npos || pos != 0)  // If the first pattern is not at the start of the word
	{
		suitable = false;  // turn the flag
	}
	else  // else, the first pattern match the word
	{
		word = word.substr(first.size());  // cut the first matching part out of the word
	}
	
    if ((pos = word.rfind(last)) == string::npos || (unsigned) pos != word.size() - last.size())  // If the last pattern is not at the end of the word
	{
		suitable = false;  // turn the flag
	}
	else  // else, the last pattern match the word
	{
		word = word.substr(0, pos);  // cut the last matching part out of the word
	}
	
	// Else, we move on and test the pattern in the middle of the word	
	for (i = 1; i < pattern.size() - 1; i++)
	{
		if ((pos = word.find(pattern.at(i))) != string::npos)  // If we find the pattern
		{
			word = word.substr(pos + 1);  // cut the matching part out of the word
		}
		else  // else,
		{
			suitable = false;  // turn the flag
			break; // and break the testing loop
		}
	}
	
	if (suitable)  // If the word is suitable for the pattern
	{
		result = node->data()->docs();  // append the documents containing it into the result
	}
	
	result = Document::disjunct(result, Indexer::match(node->left(), pattern));  // Do the whole process again with the left tree node and combine the 2 results
	
	result = Document::disjunct(result, Indexer::match(node->right(), pattern));  // Do the whole process again with the right tree node and combine the 2 results
	
    return result;
}

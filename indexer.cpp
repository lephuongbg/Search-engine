#include "indexer.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

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
void Indexer::insertKey(const string & keyword, Document & docname)
{
    indexer_ = insertKey(indexer_, keyword, docname);
}

INode * Indexer::insertKey(INode * node, const string &keyword, Document & docname)
{
    //If the tree is empty, create a new node for it
    if (node == NULL)
    {
        INode * new_node = new INode(keyword);
        new_node->data()->docs(docname);
        return new_node;
    }
    //If keyword is smaller than current node key, set it to the left
    if (std::strcmp(keyword.c_str(), node->data()->word_.c_str()) < 0)
    {
        node->setLeft(insertKey(node->left(), keyword, docname));
        return reBalance(node);
    }
    //If keyword is greater than current node key, set it to the right
    else if (std::strcmp(keyword.c_str(), node->data()->word_.c_str()) > 0)
    {
        node->setRight(insertKey(node->right(), keyword, docname));
        return reBalance(node);
    }
    //If keyword has already exist, return the node
    node->data()->docs(docname);
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
void Indexer::setQuery(string &query)
{
	// Delete all trailing white spaces
	string::iterator it = query.end() - 1;
	for (;it != query.begin() && *it == ' '; it--);
	query.resize(it + 1 - query.begin());
	
	stringstream strm;
    strm.str(query);  // Init the string stream with the query string
    string temp;
	unsigned int i = 0;
	
	// Flush out the last query
	query_.clear();
    
    // Change status code to default SUCCESS value
    status_ = SUCCESS;
    
    // Parse through the string stream
    while (! strm.eof())
    {
    	strm >> temp;  // flow it into buffer memory
    	query_.push_back(temp);
    	if (i % 2 == 0 && (query_.at(i) == "AND" || query_.at(i) == "OR"))  // if it is not operand as it should be
    	{
			status_ = SYNTAX_ERROR;  // raise an error
			break;  // break the process
		}
		else if (i % 2 == 1 && (query_.at(i) != "AND" && query_.at(i) != "OR"))  // if it is not operator as it should be
		{
			query_.push_back(query_.at(i));
			query_.at(i++) = "AND";
		}
		i++;
    }
    
    if (i % 2 == 0)  // If the last unit is an operand
	{
    	status_ = SYNTAX_ERROR;  // raise an error
    	return;
    }
    
    for (i = 0; i < query_.size(); i = i + 2)
    {
        if (!filter(query_.at(i), true))
    	{
    		status_ = STOPWORD_WARNING;  // raise an warning
    		if (query_.size() == 1)
    		{
    			query_.erase(query_.begin());
    			break;
    		}
    		else if (i == 0)
    		{
    			query_.erase(query_.begin());
    			query_.erase(query_.begin());
    			i = i - 2;
    		}
    		else if (i == query_.size() - 1)
    		{
    			query_.pop_back();
    			query_.pop_back();
    			break;
    		}
    		else
    		{
    			query_.erase(query_.begin() + i);
    			if (query_.at(i-1) == "AND" && query_.at(i) == "OR")
    				query_.erase(query_.begin() + i - 1);
    			else
    				query_.erase(query_.begin() + i);
    			i = i - 2;
    		}
    	}
    }
}

// EXECUTE THE QUERY
void Indexer::execute()
{
	if (status_ != SYNTAX_ERROR && query_.size() > 0)
	{
		stack<string> s_operator;  // A stack used to store operator(s)
		stack< vector<Document> > s_operand;  // A stack used to store operand(s)
		vector<Document> d1, d2;
		string buffer;
		
		// Parse through the query stack
		while (query_.size() > 0)
		{
		    // Cache the top element
		    buffer = query_.back();
		    query_.pop_back();

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
		        while (!s_operator.empty() && s_operator.top() == "AND")
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
		            s_operand.push(Indexer::match(indexer_, buffer));
		        // otherwise,
		        else
		            // just push it into operand stack for later process
		            s_operand.push((*this)[buffer]);
		    }
		}
		
		// Evaluation
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
	else
		result_.clear();
}

vector<Document> Indexer::result()
{
    // Sort result by score before return it out
    sort(result_.begin(), result_.end(), Document::docFreqComp);
    return result_;
}

// INDEX A DOCUMENT
bool Indexer::addDocument(const string &docname)
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
                // Filter the keyword and if it's still valid, we continue indexing
                if (filter(keyword, false))
                    this->insertKey(keyword, doc);
            }
            std::cout << "Indexed " + docname + ".\n";
        }
        else
        {
            std::cout << "Ignored " + docname + ".\n";
            return false; // Indicate the file is not read
        }
    }
    docfile.close();

    return true; // Indicate successfully reading
}

// OVERLOADING OPERATOR [] FOR FAST ACCESS NODE VALUE BY KEY WORD
vector<Document> Indexer::operator[](const string &keyword)
{
    vector<Document> result;
    INode * keynode = this->at(keyword);
    // If the keyword exists, return all documents containing it
    if (keynode != NULL)
    {
        keynode->data()->sortDocs();
        result = keynode->data()->docs();
    }

    // return empty result by default
    return result;
}

// REMOVE UNNECESSARY CHARACTER FROM THE KEYWORD AND CHECK ITS VALIDITY
bool Indexer::isGarbage(char c)
{
    return !( (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') );
}

bool Indexer::filter(string &keyword, bool isQuery)
{
    // Lower all characters and remove unnecessary one
    string::iterator it = keyword.begin();
    while (!keyword.empty() && it < keyword.end())
    {
        *it = tolower(*it);
        if (isQuery)
            if (*it == '*')
            {
                it++;
                continue;
            }
        if (isGarbage(*it))
        {
            keyword.erase(it);
            continue;
        }
        it++;
    }

    if (keyword.size() == 0 || stopwords_.find(keyword) != stopwords_.end())
        return false;

    return true;
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
        std::cout << node->data()->word_ << '\n';
        traverse(node->right());
    }
}

// PROVIDE ACCESS TO INDEXER DATA
INode *Indexer::indexer()
{
    return indexer_;
}

Indexer::Status Indexer::status()
{
    return status_;
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
		
	string word = node->data()->word(); // Cache out the INode word data value
    int pos;
	
	// Post-order traverse
	result = Document::disjunct(Indexer::match(node->left(), pattern), Indexer::match(node->right(), pattern));
	
	string temp(pattern.at(0));
	if ((pos = word.find(temp)) == string::npos || pos != 0)  // If the first pattern is not at the start of the word
	{
		return result;  // return
	}
	else  // else, the first pattern match the word
	{
		word = word.substr(temp.size());  // cut the first matching part out of the word
	}
	
	temp = pattern.back();
    if ((pos = word.rfind(temp)) == string::npos || (unsigned) pos != word.size() - temp.size())  // If the last pattern is not at the end of the word
	{
		return result;  // return
	}
	else  // else, the last pattern match the word
	{
		word = word.substr(0, pos);  // cut the last matching part out of the word
	}
	
	// Else, we move on and test the pattern in the middle of the word	
	for (unsigned int i = 1; i < pattern.size() - 1; i++)
	{
		if ((pos = word.find(pattern.at(i))) != string::npos)  // If we find the pattern
			word = word.substr(pos + 1);  // cut the matching part out of the word
		else  // else,
			return result;  // return
	}
	
	node->data()->sortDocs();
	result = Document::disjunct(result, node->data()->docs());  // append the documents containing it into the result
	
    return result;
}

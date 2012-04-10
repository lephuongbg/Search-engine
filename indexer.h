#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"
#include <stack>

class Indexer
{
private:
    INode * indexer_;
    stack<string> query_;
    vector<string> result_;

public:
    Indexer();
    ~Indexer();
    void insertKey(const string & keyword);
    static INode * insertKey(INode * node, const string & keyword);
    static INode * reBalance(INode * node);
    //void deleteKey(string keyword);
    void setQuery(const string &query);
    void excute();
    void addDocument(const string &docname);
};

#endif // INDEXER_H

#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"
#include <stack>
#include <vector>

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
    INode * find(const string & keyword);
    //void deleteKey(string keyword);
    void setQuery(const string &query);
    void excute();
    void addDocument(const string &docname);
    vector<Document> operator[](const string &keyword);
};

#endif // INDEXER_H

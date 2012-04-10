#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"

class Indexer
{
private:
    INode * indexer_;
    stack<string> query_;
    vector<string> result_;

public:
    Indexer();
    ~Indexer();
    void insertINode(INode * node);
    void deleteINode(INode * node);
    void setQuery(string);
    void excute();
    void addDocument(string);
};

#endif // INDEXER_H

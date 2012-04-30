#ifndef INODEDATA_H
#define INODEDATA_H

#include <vector>
#include "document.h"

class INode;

class INodeData
{
private:
    vector<Document> docs_;
    bool sorted_;
    string word_;

public:
    INodeData();
    INodeData(const string &word);
    string word();
    vector<Document> docs();
    void docs(Document doc);
    void sortDocs();

//friend class INode;
friend class Indexer; // For frequently calling private variables from Indexer
};

#endif // INODEDATA_H

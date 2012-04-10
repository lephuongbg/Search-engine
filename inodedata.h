#ifndef INODEDATA_H
#define INODEDATA_H

#include <vector>
#include "document.h"

class INodeData
{
private:
    string word_;
    vector<Document> docs_;
    bool sorted_;

public:
    INodeData();
    INodeData(string word);
    string word();
    vector<Document> docs();
    void docs(Document doc);
    void sortDocs();
    vector<Document> conjunct(INodeData * node_data);
    vector<Document> disjunct(INodeData * node_data);
};

#endif // INODEDATA_H

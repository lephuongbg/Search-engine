#ifndef INODEDATA_H
#define INODEDATA_H

#include <vector>
#include "document.h"

class INodeData
{
private:
    string word_;
    vector<Document> docs_;
    bool sorted;

public:
    INodeData();
    INodeData(string w);
    string word();
    vector<Document> docs();
    void docs(Document doc);
    void sortDocs();
    vector<Document> conjunt(INodeData * node_data);
    vector<Document> compose(INodeData * node_data);
};

#endif // INODEDATA_H

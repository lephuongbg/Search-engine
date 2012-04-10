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
    static vector<Document> _conjunct(INodeData *node1, INodeData *node2);
    static vector<Document> _disjunct(INodeData *node1, INodeData *node2);
};

#endif // INODEDATA_H

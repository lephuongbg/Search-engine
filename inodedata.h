#ifndef INODEDATA_H
#define INODEDATA_H

class INodeData
{
private:
    string word_;
    vector<Document> docs_;

public:
    INodeData();
    INodeData(string word);
    string word();
    void word(string name);
    vector<Document> docs();
    void docs(Document doc);
    vector<Document> conjunt(INodeData * node_data);
    vector<Document> compose(INodeData * node_data);
    void sortDocs();
};

#endif // INODEDATA_H

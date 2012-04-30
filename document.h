#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
using namespace std;

class Document
{
private:
    string name_;
    int occurrence_;
public:
    Document();
    Document(const string &docname);
    string name();
    void name(const string &docname);
    int occurrence();
    void occurrence(int occurrence);
    void increaseOccurrence(int n);
    static bool docNameComp(Document doc1, Document doc2);
    static bool docFreqComp(Document doc1, Document doc2);
    static vector<Document> conjunct(vector<Document> docs1, vector<Document> docs2);
    static vector<Document> disjunct(vector<Document> docs1, vector<Document> docs2);

friend class INodeData;
};

#endif // DOCUMENT_H

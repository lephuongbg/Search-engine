#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

using namespace std;

class Document
{
private:
    string name_;
    int occurence_;
public:
    Document();
    Document(string docname);
    string name();
    void name(string docname);
    int occurrence();
    void occurrence(int n);
    void increaseOccurrence(int n);
};

#endif // DOCUMENT_H

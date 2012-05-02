#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"
#include <unordered_set>
#include <set>
#include <stack>
#include <vector>

class Indexer
{
public:
    Indexer();
    ~Indexer();

    enum Status {SYNTAX_ERROR, STOPWORD_WARNING, SUCCESS};
    // Functional methods
    void insertKey(const string & keyword, Document &docname);
    static INode * insertKey(INode * node, const string & keyword, Document &docname);
    static INode * reBalance(INode * node);
    void setQuery(const string &query);
    void execute();
    bool addDocument(const string &docname);
    static bool isGarbage(char c);
    bool filter(string &keyword, bool isQuery);
    void indexStopWords(const string &wordfile);
    static vector<Document> match(INode * node, string regex);
    static vector<Document> match(INode * node, vector<string> q_regex);
    // Access methods
    INode * at(const string & keyword);
    vector<Document> result();
    vector<Document> operator[](const string &keyword);
    INode * indexer();
    Status status();
    // Debug methods
    static void traverse(INode * node);

protected:
    INode * indexer_;
    unordered_set<string> stopwords_;
    vector<string> query_;
    vector<Document> result_;
    Status status_;
};

#endif // INDEXER_H

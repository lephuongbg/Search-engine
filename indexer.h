#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"
#include <set>
#include <stack>
#include <vector>

class Indexer
{
protected:
    INode * indexer_;
    set<string> stopwords_;
    stack<string> query_;
    vector<Document> result_;
    enum Status {SYNTAX_ERROR, STOPWORD_WARNING, SUCCESS} status_;

public:
    Indexer();
    ~Indexer();

    // Functional methods
    void insertKey(const string & keyword);
    static INode * insertKey(INode * node, const string & keyword);
    static INode * reBalance(INode * node);
    void setQuery(const string &query);
    void execute();
    void addDocument(const string &docname);
    bool isIgnore(const string &keyword);
    static bool isGarbage(char c);
    static void filter (string &keyword);
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
};

#endif // INDEXER_H

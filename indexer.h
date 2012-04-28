#ifndef INDEXER_H
#define INDEXER_H
#include "inode.h"
#include <set>
#include <stack>
#include <vector>

enum Status {ERROR_SYNTAX, WARNING_STOPWORD, SUCCESS};

class Indexer
{
protected:
    INode * indexer_;
    set<string> stopwords_;
    stack<string> query_;
    vector<Document> result_;
    Status status_;

public:
    Indexer();
    ~Indexer();
    void insertKey(const string & keyword);
    static INode * insertKey(INode * node, const string & keyword);
    static INode * reBalance(INode * node);
    INode * at(const string & keyword);
    void setQuery(const string &query);
    void execute();
    vector<Document> result();
    void addDocument(const string &docname);
    vector<Document> operator[](const string &keyword);
    bool isIgnore(const string &keyword);
    static bool isGarbage(char c);
    static void filter (string &keyword);
    void indexStopWords(const string &wordfile);
    // For debugging
    static void traverse(INode * node);
    INode * indexer();
    
    Status status();
    static vector<Document> match(INode * node, string regex);
    static vector<Document> match(INode * node, vector<string> q_regex);
};

#endif // INDEXER_H
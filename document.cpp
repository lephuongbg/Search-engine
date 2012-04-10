#include "document.h"

/************************************
 *          CONSTRUCTOR             *
 ************************************/
Document::Document()
{
    this->name_ = "";
    this->occurrence_ = 1;
}

Document::Document(string name)
{
    this->name_ = name;
    this->occurrence_ = 1;
}

/************************************
 *       GET THE DOCUMENT NAME      *
 ************************************/
string Document::name()
{
    return this->name_;
}

/************************************
 *       SET THE DOCUMENT NAME      *
 ************************************/
void Document::name(string name)
{
    this->name_ = name;
}

/***************************************************
 *     GET THE KEYWORD FREQUENCY IN THE DOCUMENT   *
 ***************************************************/
int Document::occurrence()
{
    return this->occurrence_;
}

/***************************************************
 *     SET THE KEYWORD FREQUENCY IN THE DOCUMENT   *
 ***************************************************/
void Document::occurrence(int occurrence)
{
    this->occurrence_ = occurrence;
}

void Document::increaseOccurrence(int n)
{
    this->occurrence_ += n;
}

/*****************************************************
 *    COMPARE TWO DOCUMENTS' NAME: use for sorting   *
 *****************************************************/
bool Document::docNameComp(Document doc1, Document doc2)
{
    return (doc1.name().compare(doc2.name()) <= 0);
}

/**************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ONLY COMMON ONES    *
 **************************************************************/
vector<Document> Document::conjunct(vector<Document> docs1, vector<Document> docs2)
{
}

/**************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ALL DIFFERNCES      *
 **************************************************************/
vector<Document> Document::disjunct(vector<Document> docs1, vector<Document> docs2)
{
}

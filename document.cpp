#include "document.h"

/************************************
 *          CONSTRUCTOR             *
 ************************************/
Document::Document()
{
    occurrence_ = 1;
}

Document::Document(const string &docname)
{
    name_ = docname;
    occurrence_ = 1;
}

/************************************
 *       GET THE DOCUMENT NAME      *
 ************************************/
string Document::name()
{
    return name_;
}

/************************************
 *       SET THE DOCUMENT NAME      *
 ************************************/
void Document::name(const string &docname)
{
    name_ = docname;
}

/***************************************************
 *     GET THE KEYWORD FREQUENCY IN THE DOCUMENT   *
 ***************************************************/
int Document::occurrence()
{
    return occurrence_;
}

/***************************************************
 *     SET THE KEYWORD FREQUENCY IN THE DOCUMENT   *
 ***************************************************/
void Document::occurrence(int occurrence)
{
    occurrence_ = occurrence;
}

void Document::increaseOccurrence(int n)
{
    occurrence_ += n;
}

/*****************************************************
 *    COMPARE TWO DOCUMENTS' NAME: use for sorting   *
 *****************************************************/
bool Document::docNameComp(Document doc1, Document doc2)
{
    return (doc1.name().compare(doc2.name()) <= 0);
}

bool Document::docFreqComp(Document doc1, Document doc2)
{
    return (doc1.occurrence() > doc2.occurrence());
}

/*****************************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ONLY COMMON ONES                   *
 *****************************************************************************/
vector<Document> Document::conjunct(vector<Document> docs1, vector<Document> docs2)
{
    unsigned int o1, o2;
    vector<Document>::iterator it1end = docs1.end(), it2end = docs2.end();
    vector<Document>::iterator it1 = docs1.begin(), it2 = docs2.begin();
    Document temp;
    vector<Document> result;
    
    while (it1 < it1end && it2 < it2end)
    {
        if (it1->name_ == it2->name_)
        {
            temp.name(it1->name_);
            o1 = it1->occurrence_;
            o2 = it2->occurrence_;
            temp.occurrence( (o1 < o2) ? o1 : o2 );
            result.push_back( temp );
            it1++;
            it2++;
        }
        else if (it1->name_ < it2->name_)
        {
            it1++;
        }
        else
        {
            it2++;
        }
    }
    
    return result;
}

/*****************************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ALL DIFFERENCES AND COMMON ONES    *
 *****************************************************************************/
vector<Document> Document::disjunct(vector<Document> docs1, vector<Document> docs2)
{
    unsigned int o1, o2;
    vector<Document>::iterator it1end = docs1.end(), it2end = docs2.end();
    vector<Document>::iterator it1 = docs1.begin(), it2 = docs2.begin();
    Document temp;
    vector<Document> result;

    while (it1 != it1end && it2 != it2end)
    {
        if (it1->name_ == it2->name_)
        {
            temp.name( it1->name_ );
            o1 = it1->occurrence();
            o2 = it2->occurrence();
            temp.occurrence( (o1 > o2) ? o1 : o2 );
            result.push_back( temp );
            it1++;
            it2++;
        }
        else if (it1->name_ < it2->name_)
        {
            result.push_back( *it1 );
            it1++;
        }
        else
        {
            result.push_back( *it2 );
            it2++;
        }
    }
    
    while (it1 != it1end)
    {
        result.push_back( *it1 );
        it1++;
    }
    
    while (it2 != it2end)
    {
        result.push_back( *it2 );
        it2++;
    }
    
    return result;
}

#include "document.h"

/************************************
 *          CONSTRUCTOR             *
 ************************************/
Document::Document()
{
    this->name_ = "";
    this->occurrence_ = 1;
}

Document::Document(const string &docname)
{
    this->name_ = docname;
    this->occurrence_ = 1;
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
    this->name_ = docname;
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

/*****************************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ONLY COMMON ONES                   *
 *****************************************************************************/
vector<Document> Document::conjunct(vector<Document> docs1, vector<Document> docs2)
{
    unsigned int i = 0, j = 0, o1, o2;
    string name1, name2;
    Document temp;
    vector<Document> result;
    
    while (i < docs1.size() && j < docs2.size()) 
    {
        name1 = docs1.at(i).name();
        name2 = docs2.at(j).name();
        if (name1 == name2)
        {
            temp.name( name1 );
            o1 = docs1.at(i).occurrence();
            o2 = docs2.at(j).occurrence();
            temp.occurrence( (o1 < o2) ? o1 : o2 );
            result.push_back( temp );
            i++;
            j++;
        }
        else if (name1 < name2)
        {
            i++;
        }
        else
        {
            j++;
        }
    }
    
    return result;
}

/*****************************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ALL DIFFERNCES AND COMMON ONES     *
 *****************************************************************************/
vector<Document> Document::disjunct(vector<Document> docs1, vector<Document> docs2)
{
    unsigned int i = 0, j = 0, o1, o2;
    string name1, name2;
    Document temp;
    vector<Document> result;
    
    while (i < docs1.size() && j < docs2.size()) 
    {
        name1 = docs1.at(i).name();
        name2 = docs2.at(j).name();
        if (name1 == name2)
        {
            temp.name( name1 );
            o1 = docs1.at(i).occurrence();
            o2 = docs2.at(j).occurrence();
            temp.occurrence( (o1 > o2) ? o1 : o2 );
            result.push_back( temp );
            i++;
            j++;
        }
        else if (name1 < name2)
        {
            temp.name( name1 );
            temp.occurrence( docs1.at(i).occurrence() );
            result.push_back( temp );
            i++;
        }
        else
        {
            temp.name( name2 );
            temp.occurrence( docs2.at(j).occurrence() );
            result.push_back( temp );
            j++;
        }
    }
    
    return result;
}

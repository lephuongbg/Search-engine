#include "inodedata.h"

/*******************************
 *        CONSTRUCTOR          *
 *******************************/
INodeData::INodeData()
{
    this->sorted_ = false;
}

INodeData::INodeData(string w)
{
    this->word_ = w;
}

/*******************************
 *      GET THE KEYWORD        *
 *******************************/
string INodeData::word()
{
    return this->word_;
}

/********************************************
 * LIST OF DOCUMENTS CONTAINING THE KEYWORD *
 ********************************************/
vector<Document> INodeData::docs()
{
    return this->docs_;
}

/*****************************************
 *       ADD DOCUMENT TO THE LIST        *
 *****************************************/
void INodeData::docs(Document doc)
{
    unsigned int i;
    //Check for existance of the document in the list
    for (i = 0; i < this->docs_.size(); i++)
    {
        if (this->docs_.at(i).name() == doc.name())     //If yes,
        {
            this->docs_.at(i).increaseOccurrence(doc.occurrence()); //Increase the word occurrence counter for the document
            return;
        }
    }
    //If the list doesn't have the document, add it to the list
    this->docs_.insert(this->docs_.end(), doc);
}

/*******************************************
 *      SORT THE LIST OF DOCUMENTS         *
 *******************************************/
void INodeData::sortDocs()
{

}

/**************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ONLY COMMON ONES    *
 **************************************************************/
vector<Document> INodeData::_conjunct(INodeData *node1, INodeData *node2)
{
}

/**************************************************************
 *      MERGE TWO LIST OF DOCUMENTS, KEEP ALL DIFFERNCES      *
 **************************************************************/
vector<Document> INodeData::_disjunct(INodeData *node1, INodeData *node2)
{
}

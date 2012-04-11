#include <algorithm>
#include "inodedata.h"

/*******************************
 *        CONSTRUCTOR          *
 *******************************/
INodeData::INodeData()
{
    sorted_ = false;
}

INodeData::INodeData(const string &w)
{
    word_ = w;
    sorted_ = false;
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
    for (i = 0; i < docs_.size(); i++)
    {
        //If yes,
        if (docs_.at(i).name() == doc.name())
        {
            //Increase the word occurrence counter for the document
            docs_.at(i).increaseOccurrence(doc.occurrence());
            return;
        }
    }
    //If the list doesn't have the document, add it to the list
    docs_.insert(docs_.end(), doc);
}

/*******************************************
 *      SORT THE LIST OF DOCUMENTS         *
 *******************************************/
void INodeData::sortDocs()
{
    if (!sorted_)
        sort(this->docs_.begin(), this->docs().end(), Document::docNameComp);
}

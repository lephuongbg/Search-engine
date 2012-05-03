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
    return word_;
}

/********************************************
 * LIST OF DOCUMENTS CONTAINING THE KEYWORD *
 ********************************************/
vector<Document> INodeData::docs()
{
    return docs_;
}

/*****************************************
 *       ADD DOCUMENT TO THE LIST        *
 *****************************************/
void INodeData::docs(Document doc)
{
    //Check for existance of the document in the list
    for (vector<Document>::reverse_iterator rit = docs_.rbegin(), ritend = docs_.rend(); rit != ritend; rit++)
    {
        //If yes,
        if (rit->name_ == doc.name_)
        {
            //Increase the word occurrence counter for the document
            rit->occurrence_ += doc.occurrence_;
            return;
        }
    }
    //If the list doesn't have the document, add it to the list
    docs_.insert(docs_.end(), doc);
    sorted_ = false;
}

/*******************************************
 *      SORT THE LIST OF DOCUMENTS         *
 *******************************************/
void INodeData::sortDocs()
{
    if (!sorted_)
        sort(docs_.begin(), this->docs().end(), Document::docNameComp);
    sorted_ = true;
}

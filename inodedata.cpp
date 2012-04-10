#include "inodedata.h"

INodeData::INodeData()
{
    //Do nothing
}

string INodeData::word()
{
    return this->word_;
}

void INodeData::word(string name)
{
    this->word_ = name;
}

vector<Document> INodeData::docs()
{
    return this->docs_;
}

void INodeData::docs(Document doc)
{
    unsigned int i;
    for (i = 0; i < this->docs_.size(); i++)
    {
        if (this->docs_.at(i).name() == doc.name())
            this->docs_.at(i).increaseOccurrence(doc.occurrence());
    }
}

vector<Document> INodeData::conjunt(INodeData *node_data)
{

}

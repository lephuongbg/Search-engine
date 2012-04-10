#include "document.h"

Document::Document()
{
    this->name_ = "";
    this->occurence_ = 0;
}

Document::Document(string name)
{
    this->name_ = name;
    this->occurence_ = 1;
}

string Document::name()
{
    return this->name_;
}

void Document::name(string name)
{
    this->name_ = name;
}

int Document::occurrence()
{
    return this->occurrence_;
}

void Document::occurrence(int occurence)
{
    this->occurrence_ = occurrence;
}

void Document::increaseOccurrence(int n)
{
    this->occurrence_ += n;
}

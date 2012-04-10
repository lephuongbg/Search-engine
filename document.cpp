#include "document.h"

Document::Document()
{
    name_ = "";
    occurence_ = 0;
}

Document::Document(string name)
{
    name_ = name;
    occurence_ = 1;
}

string Document::name()
{
    return name_;
}

void Document::name(string name)
{
    name_ = name;
}

int Document::occurrence()
{
    return occurrence_;
}

void Document::occurrence(int occurence)
{
    occurrence_ = occurrence;
}

void Document::increaseOccurrence(int n)
{
    occurrence_ += n;
}

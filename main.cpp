#include <iostream>
#include <string>
#include "indexer.h"

using namespace std;

int main()
{
    Indexer I;
    I.insertKey("I");
    I.insertKey("love");
    I.insertKey("you");
    I.insertKey("very");
    I.insertKey("much");
    I.insertKey("more");
    I.insertKey("than");
    I.insertKey("I");
    I.insertKey("say");
    Indexer::traverse(I.indexer());
    return 0;
}


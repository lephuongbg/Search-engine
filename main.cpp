#include <iostream>
#include <string>
#include "document.h"

using namespace std;

int main()
{
    Document A("file1");
    Document B("file2");
    if (Document::docNameComp(A, B))
        cout << A.name() << B.name();
    else
        cout << B.name() << A.name();
    return 0;
}


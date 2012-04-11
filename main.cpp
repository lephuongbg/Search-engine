#include <iostream>
#include <string>
#include "indexer.h"

using namespace std;

int main(int argc, char *argv[])
{
    Indexer I;
    cout << "Indexing..." << '\n';
    for (int i = 1; i < argc; i++)
    {
        I.addDocument(argv[i]);
    }
    while (1)
    {
        cout << "Query: ";
        string q;
        getline(cin, q);
        if (q == ".")
            break;
        I.setQuery(q);
        I.execute();
        vector<Document> result = I.result();
        for (vector<Document>::iterator it = result.begin(); it != result.end(); it++)
        {
            cout << it->name() << " " << it->occurrence() << "\n";
        }
    }
    return 0;
}


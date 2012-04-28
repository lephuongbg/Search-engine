#include <iostream>
#include <string>
#include "indexer.h"
#include "time.h"

using namespace std;

int main(int argc, char *argv[])
{
    Indexer I;

    // Store command line arguments
    vector<string> args;
    for (int i = 1; i < argc; i++)
        args.insert(args.end(), string(argv[i]));
    vector<string>::iterator it;

    // If using stop words list:
    for (it = args.begin(); it != args.end() && *it != "--stop-words-file"; it++);
    if (it != args.end())
    {
        // Consume option
        args.erase(it);
        // Index stop words list
        I.indexStopWords(*it);
        // Consume argument
        args.erase(it);
    }

    // Index all documents from command line args
    time_t start = clock();
    for (it = args.begin(); it != args.end(); it++)
    {
        cout << "Indexing " << *it << "...\n";
        I.addDocument(*it);
    }
    cout << "Complete in " << (float) (clock() - start)/CLOCKS_PER_SEC << " second(s).\n";
//    Indexer::traverse(I.indexer());
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


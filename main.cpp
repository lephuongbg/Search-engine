#include <iostream>
#include <string>
#include "indexer.h"
#include "time.h"

using namespace std;

void showHelp();

int main(int argc, char *argv[])
{
    Indexer I;

    // Store command line arguments
    vector<string> args;
    for (int i = 1; i < argc; i++)
        args.insert(args.end(), string(argv[i]));
    vector<string>::iterator it;

    // If user needs help, display help then exit
    for (it = args.begin(); it != args.end() && *it != "--help"; it++);
    if (it != args.end())
    {
        // Display help
        showHelp();
        // Exit
        return 0;
    }

    // If user specifies stop words list:
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
    // else, load default stopwords file
    else
    {
        I.indexStopWords("stopwords");
    }

    // If there's no file to index, display help then exit
    if (args.size() == 0)
    {
        showHelp();
        return 0;
    }

    // Index all documents from command line args
    time_t start = clock();
    for (it = args.begin(); it != args.end(); it++)
    {
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

void showHelp()
{
    cout << "CLI Manual (can apply for GUI version):\n"
         << "Running:\n"
         << "    search-engine [--option [<argument>]] <files_to_index>\n"
         << "Provide all file names as commandline arguments for the program\n\n"
         << "Program's option:\n"
         << "--stop-words-file <path/to/file>\n"
         << "    specify stop words list file. Default file: \"./stopwords\"\n"
         << "--no-gui (for GUI version)\n"
         << "    run without GUI\n"
         << "--help"
         << "    show this manual\n";

}

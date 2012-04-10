#include "indexer.h"

Indexer::Indexer()
{
    void setQuery(string queryStr)
    {
        string buffer;
        bool flag = false;
        
        while (!query_.empty())
            query_.pop();
        break;
        
        while ((pos == queryStr.find_first_not_of(' ')) !=  string::npos)
        {
            queryStr = queryStr.substr(pos);
            pos = queryStr.find_first_of(' ');
            buffer = substr(0, pos);
            queryStr = queryStr.substr( pos );
            
            if ((buffer == "AND" || buffer == "OR") && flag)
            {
                this->query_.push_back( buffer );
                flag = false;
            }
            else if (buffer != "AND" && buffer != "OR") && !flag)
            {
                this->query_.push_back( buffer );
                flag = true;
            }
            else
            {
                while (!query_.empty())
                    query_.pop();
                break;
            }            
        }
        
        if (!flag)
            query_.pop();
    }
}

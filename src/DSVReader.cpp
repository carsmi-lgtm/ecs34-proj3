#include "DSVReader.h"

struct CDSVReader::SImplementation{
    std::shared_ptr<CDataSource> DSource;
    char DDelimiter;
    bool DEnd;
};


CDSVReader::CDSVReader(std::shared_ptr< CDataSource > src, char delimiter) : DImplementation(std::make_unique<SImplementation>()){
    DImplementation->DSource = src;
    DImplementation->DDelimiter = delimiter;
    DImplementation->DEnd = false;
    if (DImplementation->DDelimiter == '"') {
        DImplementation->DDelimiter = ',';   
    }
}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::End() const{
    return DImplementation->DEnd; // returns boolean value DEnd: Is this the end of the line
}

bool CDSVReader::ReadRow(std::vector<std::string> &row){
    row.clear(); // start with empty vector

    if (DImplementation->DEnd) return false; // if end of row cannot read row 

    char c;
    char nextchar;
    bool WithinQuotes = false;
    std::string Str;

    while (DImplementation->DSource->Get(c)){  // read next character while chars left in data source
        
        // flip boolean quotes if quotes are read, also convert two double quotes to one
        if (c == '"'){
            if (WithinQuotes == true){
                if (DImplementation->DSource->Peek(nextchar) && nextchar == '"'){
                    DImplementation->DSource->Get(nextchar);
                    Str += '\"';
                }
                else{
                    WithinQuotes = false;
                }
            }
            else{
                WithinQuotes = true;
            }
        }

        // delimeter is read, add string to vector and clear Str
        else if (WithinQuotes == false && DImplementation->DDelimiter == c){
            row.push_back(Str);
            Str.clear();
            char next;
            if (!DImplementation->DSource->Peek(next)){
                row.push_back(Str);
                DImplementation->DEnd = true;
                return true;
            }
        }
        
        // handle carriage return
        else if (c == '\r') continue;

        // if in quotes and newline is read, add string to vector and clear Str, return true (full row read)
        else if (WithinQuotes == false && c == '\n'){
            if (!Str.empty() || !row.empty()) row.push_back(Str);
            return true;
        }

        // otherwise add character to Str for current cell
        else{ 
            Str += c;
        }
    }

    // end of source, add to vector any leftover characters in Str
    if (!Str.empty()){
        row.push_back(Str);
    }

    DImplementation->DEnd = true;

    // if row vector is empty, nothing was read, return false. otherwise true
    return !row.empty();
}
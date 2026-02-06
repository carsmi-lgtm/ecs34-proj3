#include "DSVWriter.h"

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> DSink;
    char DDelimiter;
    bool DQuoteAll;
};

CDSVWriter::CDSVWriter(std::shared_ptr< CDataSink > sink, char delimiter, bool quoteall) : DImplementation(std::make_unique<SImplementation>()){
    DImplementation->DSink = sink;
    DImplementation->DDelimiter = delimiter;
    DImplementation->DQuoteAll = quoteall;
    if (DImplementation->DDelimiter == '"') {
        DImplementation->DDelimiter = ',';   
    }
}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string> &row){

    char delim = DImplementation->DDelimiter;

    for (size_t i = 0; i < row.size(); ++i){
        std::string Str = row[i];
        
        
        bool UseQuotes = false;
        if (!DImplementation->DQuoteAll) {
            bool UseQuotes = true;
        }

        // flip UseQuotes to true if encounter delimeter or quote or newline
        for (char c : Str){
            if (c == delim || c == '"' || c == '\n'){
                UseQuotes = true;
                break;
            }
        }
        
        // start quotes
        if (UseQuotes == true){
            if (!DImplementation->DSink->Put('"')) return false;
        }

        // write characters to cell
        for (char c : Str){
            if (c == '"'){
                if (!DImplementation->DSink->Put('"')) return false; // escape quotes by adding extra quote
            }
            if (!DImplementation->DSink->Put(c)) return false;
        }

        // end quotes
        if (UseQuotes == true){
            if (!DImplementation->DSink->Put('"')) return false;
        }

        // delimiter between cells
        if (i+1 < row.size()){
            if (!DImplementation->DSink->Put(delim)) return false;
        }
    }

    if (!DImplementation->DSink->Put('\n')) return false; // newline after entire row
    return true;
}

#include "XMLWriter.h"
#include <string>
#include <vector>

struct CXMLWriter::SImplementation{
    std::shared_ptr<CDataSink> Sink;
    std::vector<std::string> Stack;

    bool WriteString(const std::string &word){ // writes string to sink
        if(!Sink){
            return false;
        }
        std::vector<char> Temp(word.begin(), word.end());
        return Sink->Write(Temp);
    }
};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(std::make_unique<SImplementation>()){
    DImplementation->Sink = sink;
}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::WriteEntity(const SXMLEntity &entity){
    if(!DImplementation || !DImplementation->Sink){
        return false;
    }

    if(entity.DType == SXMLEntity::EType::StartElement){
        if(entity.DNameData.empty()){
            return false;
        }
        std::string Tag = "<" + entity.DNameData + ">"; // construct XML start tag string
        if(!DImplementation->WriteString(Tag)){
            return false;
        }
        DImplementation->Stack.push_back(entity.DNameData); // push tag onto stack (been opened but not closed)
        return true;
    }

    if(entity.DType == SXMLEntity::EType::EndElement){
        if(entity.DNameData.empty()){
            return false;
        }
        if(DImplementation->Stack.empty()){ // no open tag on stack (end tag has nothing to close)
            return false;
        }
        if(DImplementation->Stack.back() != entity.DNameData){ // if end tag doesn't match most recently opened tag
            return false;
        }
        std::string Tag = "</" + entity.DNameData + ">"; // construct XML end tag
        if(!DImplementation->WriteString(Tag)){
            return false;
        }
        DImplementation->Stack.pop_back(); // remove matching start tag from stack
        return true;
    }

    if(entity.DType == SXMLEntity::EType::CharData){
        return DImplementation->WriteString(entity.DNameData); // writing character data directly to output (no < >)
    }

    if(entity.DType == SXMLEntity::EType::CompleteElement){
        if(entity.DNameData.empty()){
            return false;
        }
        std::string Tag = "<" + entity.DNameData + "/>"; // opens and closes immediately (never goes on the stack)
        return DImplementation->WriteString(Tag);
    }

    return false;
}

bool CXMLWriter::Flush(){ // close tags that were started but never ended
    if(!DImplementation || !DImplementation->Sink){
        return false;
    }
    while(!DImplementation->Stack.empty()){ // while there are still open tags
        std::string Name = DImplementation->Stack.back(); // gets the most recently open tag
        DImplementation->Stack.pop_back(); // removing tag from stack
        std::string Tag = "</" + Name + ">"; // adding the end tag
        if(!DImplementation->WriteString(Tag)){ // writing fails
            return false;
        }
    }
    return true;
}

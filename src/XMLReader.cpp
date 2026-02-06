#include "XMLReader.h"
#include <expat.h>
#include <queue>

// <tag attr="val">CharData</tag>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DSource;
    XML_Parser DParser;
    std::queue<SXMLEntity> DQueue;
    // when start tage like <tag attr ="val"> is found
    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;

        NewEntity.DType = SXMLEntity::EType::StartElement;
        NewEntity.DNameData = name;
        for (int i = 0; atts[i]; i += 2){
            NewEntity.DAttributes.push_back({atts[i],atts[i+1]});
        }
        This->DQueue.push(NewEntity); // save entity for later
    }
    // when end tag like </tag> is found
    static void EndElementHandler(void *userData, const XML_Char *name){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;
        std::queue<SXMLEntity> DQueue;

        NewEntity.DType = SXMLEntity::EType::EndElement;
        NewEntity.DNameData = name;
        This->DQueue.push(NewEntity);
    }
    // when test between tags is found
    static void CharacterDataHandler(void *userData, const XML_Char *s, int len){
        SImplementation *This = (SImplementation *)userData;
        SXMLEntity NewEntity;
        std::queue<SXMLEntity> DQueue;

        NewEntity.DType = SXMLEntity::EType::CharData;
        NewEntity.DNameData = std::string(s,len);
        This->DQueue.push(NewEntity);
    }
    // set up parser and connect the handlers
    SImplementation(std::shared_ptr<CDataSource> src){
        DSource = src;
        DParser = XML_ParserCreate(nullptr);
        XML_SetUserData(DParser,(void *)this); // expat passes this object to the handlers
        XML_SetStartElementHandler(DParser,StartElementHandler);
        XML_SetEndElementHandler(DParser,EndElementHandler);
        XML_SetCharacterDataHandler(DParser,CharacterDataHandler);
    }
    // clean up parser
    ~SImplementation(){
        if (DParser){
            XML_ParserFree(DParser);
            DParser = nullptr;
        }
    }
    // done and nothing left in queue
    bool End() const{
        return DSource->End() && DQueue.empty();
    }
    // read more data and gives to parser
    bool ParseMore() {
        if (DSource->End()) {
            return false;
        }
        std::vector<char> Buffer(512);
        size_t Count = DSource->Read(Buffer, Buffer.size());
        int Len = (int)Count;
        int IsFinal = DSource->End() ? 1 : 0;

        if (XML_Parse(DParser, Buffer.data(), Len, IsFinal) == XML_STATUS_ERROR) {
            return false;
        }
        return true;
    }

    // getting the next parsed
    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        if (DQueue.empty()){
            std::vector<char> Buffer(512);
            DSource->Read(Buffer,Buffer.size());
            XML_Parse(DParser,Buffer.data(),Buffer.size(),DSource->End());
        }
        if (skipcdata) { // skips chardata
            while (true) {
                while (!DQueue.empty() && DQueue.front().DType == SXMLEntity::EType::CharData){
                    DQueue.pop(); 
                }
                if (!DQueue.empty() || DSource->End()){ // if something is finished or ready
                    break;
                }
                // reads more data
                if (!ParseMore()){
                    break;
                }
            }
        }
        // returns next entity if exists
        if (!DQueue.empty()){
            entity = DQueue.front();
            DQueue.pop();
            return true;
        }
        return false;
    }
};

// functions to help use the XML reader
CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
    DImplementation = std::make_unique<SImplementation>(src);
}
CXMLReader::~CXMLReader(){
}
bool CXMLReader::End() const {
    return DImplementation->End();
}
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
}

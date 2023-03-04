/*
    XMLParser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XMLPARSER_HPP
#define INCLUDED_XMLPARSER_HPP

#include <string_view>

class XMLParser {
    private: 

    std::string_view content;
    std::string_view qName;
    std::string_view prefix;
    std::string_view localName;
    std::string_view characters;
    long totalBytes = 0;
    bool doneReading = false;

    public:
        
        //constructor
        XMLParser(std::string_view& content, std::string_view& qName, std::string_view& prefix, 
                  std::string_view& localName, std::string_view& characters, long& totalBytes, bool& doneReading);

        XMLParser(std::string_view& content, long& totalBytes, bool& doneReading);

        XMLParser(std::string_view& content, bool& doneReading);

        XMLParser(std::string_view& content);

        // check if declaration
        bool isXMLDeclaration();
        
        // check if DOCTYPE
        bool isDOCTYPE();
        
        // check if character entity references
        bool isCharacterEntityReferences();
        
        // check if character non-entity references
        bool isCharacterNonEntityReferences();
        
        // check if comment
        bool isXMLComment();
        
        // check if CDATA
        bool isCDATA();
        
        // check if processing instruction
        bool isProcessingInstruction();
        
        // check if end tag
        bool isEndTag();
        
        // check if start tag
        bool isStartTag();
        
        // check if namespace
        bool isXMLNamespace();
        
        // Start tracing document
        void startTracing();
        
        // check for file input
        void checkFIleInput();
        
        // parse XML declaration
        void parseXMLDeclaration();
        
        // parse DOCTYPE
        void parseDOCTYPE();
        
        // refill content preserving unprocessed
        void refillContentUnprocessed();
        
        // parse character entity references
        std::string_view parseCharacterEntityReferences();
        
        // parse character non-entity references
        std::string_view parseCharacterNonEntityReferences();
        
        // parse XML comment
        void parseXMLComment();
        
        // parse CDATA
        std::string_view parseCDATA();
        
        // parse processing instruction
        void parseProcessingInstruction();
        
        // parse end tag
        void parseEndTag();
        
        // parse start tag
        std::string_view parseStartTag();
        
        // parse XML namespace
        void parseXMLNamespace();
        
        // parse attribute
        std::size_t parseAttribute();
        
        // End tracing document
        void EndTracing();

        long getTotalBytes();

        bool getDoneReading();

};

#endif
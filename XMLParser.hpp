/*
    XMLParser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XMLPARSER_HPP
#define INCLUDED_XMLPARSER_HPP

#include <string_view>
#include <functional>
#include <optional>

#include "XMLParserHandler.hpp"

class XMLParser {
    
    private: 

    std::string_view content;
    long totalBytes;
    bool doneReading;
    int depth;
    XMLParserHandler& handler;

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
    void parseCharacterEntityReferences();

    // parse character non-entity references
    void parseCharacterNonEntityReferences();

    // parse XML comment
    void parseXMLComment();

    // parse CDATA
    void parseCDATA();

    // parse processing instruction
    void parseProcessingInstruction();

    // parse end tag
    void parseEndTag();

    // parse start tag
    void parseStartTag();

    // parse XML namespace
    void parseXMLNamespace();

    // parse attribute
    void parseAttribute();

    // End tracing document
    void endTracing();

    public:

    // constructor
    XMLParser(XMLParserHandler& handler);

    virtual ~XMLParser() = default;
    
    long getTotalBytes();

    void parse();

};
#endif

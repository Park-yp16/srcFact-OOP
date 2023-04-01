/*
    XMLParser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XMLPARSER_HPP
#define INCLUDED_XMLPARSER_HPP

#include <string_view>
#include <functional>
#include <optional>

class XMLParser {
    
    private: 
    std::string_view content;
    long totalBytes;
    bool doneReading;
    int depth;

    std::function<void()> startHandler;
    std::function<void(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone)> declarationHandler;
    std::function<void()> DOCTYPEHandler;
    std::function<void(std::string_view qName, std::string_view prefix, std::string_view localName)> startTagHandler;
    std::function<void(std::string_view prefix, std::string_view qName, std::string_view localName)> endTagHandler;
    std::function<void(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value)> attributeHandler;
    std::function<void(std::string_view prefix, std::string_view uri)> namespaceHandler;
    std::function<void(std::string_view comment)> commentHandler;
    std::function<void(std::string_view characters)> CDATAHandler;
    std::function<void(std::string_view target, std::string_view data)> processingInstructionHandler;
    std::function<void(std::string_view characters)> characterEntityReferencesHandler;
    std::function<void(std::string_view characters)> characterNonEntityReferencesHandler;
    std::function<void()> endHandler;

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

    //constructor
    XMLParser(
        std::function<void()> startHandler,
        std::function<void(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone)> declarationHandler,
        std::function<void()> DOCTYPEHandler, 
        std::function<void(std::string_view qName, std::string_view prefix, std::string_view localName)> startTagHandler, 
        std::function<void(std::string_view prefix, std::string_view qName, std::string_view localName)> endTagHandler,
        std::function<void(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value)> attributeHandler,
        std::function<void(std::string_view prefix, std::string_view uri)> namespaceHandler,
        std::function<void(std::string_view comment)> commentHandler,
        std::function<void(std::string_view characters)> CDATAHandler,
        std::function<void(std::string_view target, std::string_view data)> processingInstructionHandler,
        std::function<void(std::string_view characters)> characterEntityReferencesHandler,
        std::function<void(std::string_view characters)> characterNonEntityReferencesHandler,
        std::function<void()> endHandler
    );

    long getTotalBytes();

    void parse();

};
#endif

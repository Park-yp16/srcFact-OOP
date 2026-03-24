/*
    xml_parser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XML_PARSER_HPP
#define INCLUDED_XML_PARSER_HPP

#include <string_view>

namespace xml_parser {

    // check if declaration
    bool isXMLDeclaration(std::string_view& text);

    // check if DOCTYPE
    bool isDOCTYPE(std::string_view& text);

    // check if character entity references
    bool isCharacterEntityReferences(std::string_view& text);

    // check if character non-entity references
    bool isCharacterNonEntityReferences(std::string_view& text);

    // check if comment
    bool isXMLComment(std::string_view& text);

    // check if CDATA
    bool isCDATA(std::string_view& text);

    // check if processing instruction
    bool isProcessingInstruction(std::string_view& text);

    // check if end tag
    bool isEndTag(std::string_view& text);

    // check if start tag
    bool isStartTag(std::string_view& text);

    // check if namespace
    bool isXMLNamespace(std::string_view& text);

    // Start tracing document
    void startTracing();

    // check for file input
    void checkFIleInput(std::string_view& text, long& totalBytes);

    // parse XML declaration
    void parseXMLDeclaration(std::string_view& text);

    // parse DOCTYPE
    void parseDOCTYPE(std::string_view& text);

    // refill content preserving unprocessed
    void refillContentUnprocessed(std::string_view& text, bool& doneReading, long& totalBytes);

    // parse character entity references
    void parseCharacterEntityReferences(std::string_view& text);

    // parse character non-entity references
    void parseCharacterNonEntityReferences(std::string_view& text);

    // parse XML comment
    void parseXMLComment(std::string_view& text, bool& doneReading, long& totalBytes);

    // parse CDATA
    void parseCDATA(std::string_view& text, bool& doneReading, long& totalBytes);

    // parse processing instruction
    void parseProcessingInstruction(std::string_view& text);

    // parse end tag
    void parseEndTag(std::string_view& text);

    // parse start tag
    void parseStartTag(std::string_view& text);

    // parse XML namespace
    void parseXMLNamespace(std::string_view& text);

    // parse attribute
    void parseAttribute(std::string_view& text);

    // End tracing document
    void EndTracing();
}
#endif

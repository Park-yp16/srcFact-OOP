/*
    xml_parser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XML_PARSER_HPP
#define INCLUDED_XML_PARSER_HPP

#include <string_view>

// check if declaration
bool isXMLDeclaration(std::string_view& text);

// check if DOCTYPE
bool isDOCTYPE(std::string_view& text);

// check if character entity references
bool isCharEntityRefs(std::string_view& text);

// check if character non-entity references
bool isCharNonEntityRefs(std::string_view& text);

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

// parse XML declaration
void parseXMLDeclaration(std::string_view& text);

// parse DOCTYPE
void parseDOCTYPE(std::string_view& text);

// refill content preserving unprocessed
int refillContentUnprocessed(std::string_view& text, bool& doneReading);

// parse character entity references
void parseCharEntityRefs(std::string_view& text);

// parse character non-entity references
const std::string_view parseCharNonEntityRefs(std::string_view& text);

// parse XML comment
void parseXMLComment(std::string_view& text, bool& doneReading, long& totalBytes);

// parse CDATA
std::string_view parseCDATA(std::string_view& text, bool& doneReading, long& totalBytes);

// parse processing instruction
void parseProcessingInstruction(std::string_view& text);

// parse end tag
void parseEndTag(std::string_view& text);

// parse XML namespace
void parseXMLNamespace(std::string_view& text);

// parse attribute
void parseAttribute(std::string_view& text, bool& inEscape, std::string& url);
 
#endif

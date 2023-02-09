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

// check if declaration
bool isXMLDeclaration(std::string_view& text);

// check if declaration
bool isXMLDeclaration(std::string_view& text);


// parse XML declaration
void parseXMLDeclaration(std::string_view& text);

// parse DOCTYPE
void parseDOCTYPE(std::string_view& text);

// refill content preserving unprocessed
void refillContentUnprocessed(std::string_view& text, bool& doneReading, long& totalBytes);

// parse character entity references
void parseCharEntityRefs(std::string_view& text, int& textSize);

// parse character non-entity references
void parseCharNonEntityRefs(std::string_view& text, int& textSize, int& loc);

// parse XML comment
void parseXMLComment(std::string_view& text, bool& doneReading, long& totalBytes);

// parse CDATA
void parseCDATA(std::string_view& text, bool& doneReading, long& totalBytes, int& textSize, int& loc);

// parse processing instruction
void parseProcessingInstruction(std::string_view& text);

// parse end tag
void parseEndTag(std::string_view& text);

// parse XML namespace
void parseXMLNamespace(std::string_view& text);
 
#endif

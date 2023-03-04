/*
    XMLParser.cpp

    Implementation file for xml parser functions
*/

#include "XMLParser.hpp"
#include "xml_parser.hpp"
#include "refillContent.hpp"
#include <iostream>
#include <bitset>
#include <optional>
#include <cassert>
#include <algorithm>
#include <iomanip>


// provides literal string operator""sv
using namespace std::literals::string_view_literals;

const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");

constexpr auto WHITESPACE = " \n\t\r"sv;
constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

// trace parsing
#ifdef TRACE
#undef TRACE
#define HEADER(m) std::clog << "\033[1m" << std::setw(10) << std::left << m << "\u001b[0m" << '\t'
#define TRACE0() ""
#define TRACE1(l1, n1)                         "\033[1m" << l1 << "\u001b[0m" << "|" << "\u001b[31;1m" << n1 << "\u001b[0m" << "| "
#define TRACE2(l1, n1, l2, n2)                 TRACE1(l1,n1)             << TRACE1(l2,n2)
#define TRACE3(l1, n1, l2, n2, l3, n3)         TRACE2(l1,n1,l2,n2)       << TRACE1(l3,n3)
#define TRACE4(l1, n1, l2, n2, l3, n3, l4, n4) TRACE3(l1,n1,l2,n2,l3,n3) << TRACE1(l4,n4)
#define GET_TRACE(_2,_3,_4,_5,_6,_7,_8,_9,NAME,...) NAME
#define TRACE(m,...) HEADER(m) << GET_TRACE(__VA_ARGS__, TRACE4, _UNUSED, TRACE3, _UNUSED, TRACE2, _UNUSED, TRACE1, TRACE0, TRACE0)(__VA_ARGS__) << '\n';
#else
#define TRACE(...)
#endif

//constructor
XMLParser::XMLParser(std::string_view& content, std::string_view& qName, std::string_view& prefix, 
                     std::string_view& localName, std::string_view& characters, long& totalBytes, bool& doneReading)
            : content(content), qName(qName), prefix(prefix), localName(localName), characters(characters), totalBytes(totalBytes), doneReading(doneReading) {}

XMLParser::XMLParser(std::string_view& content, long& totalBytes, bool& doneReading)
            : content(content), totalBytes(totalBytes), doneReading(doneReading) {}

XMLParser::XMLParser(std::string_view& content, bool& doneReading)
            : content(content), doneReading(doneReading) {}
            
XMLParser::XMLParser(std::string_view& content)
            : content(content) {}
            
// Start tracing document
void XMLParser::startTracing() {

    return xml_parser::startTracing();
}

// check for file input
void XMLParser::checkFIleInput() {

    auto bytesRead = xml_parser::checkFIleInput(content);
    totalBytes += bytesRead;
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// check if declaration
bool XMLParser::isXMLDeclaration() {

    return xml_parser::isXMLDeclaration(content);
}

// parse XML declaration
void XMLParser::parseXMLDeclaration() {

    return xml_parser::parseXMLDeclaration(content);
}

// check if DOCTYPE
bool XMLParser::isDOCTYPE() {

    return xml_parser::isDOCTYPE(content);
}

// parse DOCTYPE
void XMLParser::parseDOCTYPE() {

    return xml_parser::parseDOCTYPE(content);
}

// refill content preserving unprocessed
void XMLParser::refillContentUnprocessed() {
       
    auto bytesRead = xml_parser::refillContentUnprocessed(content, doneReading);
    totalBytes += bytesRead;
}

// check if character entity references
bool XMLParser::isCharacterEntityReferences() {
    
    return xml_parser::isCharacterEntityReferences(content);
}

// parse character entity references
std::string_view XMLParser::parseCharacterEntityReferences() {
    
    return xml_parser::parseCharacterEntityReferences(content);
}

// check if character non-entity references
bool XMLParser::isCharacterNonEntityReferences() {

    return xml_parser::isCharacterNonEntityReferences(content);
}

// parse character non-entity references
std::string_view XMLParser::parseCharacterNonEntityReferences() {
    
    return xml_parser::parseCharacterNonEntityReferences(content);
}

// check if comment
bool XMLParser::isXMLComment() {

    return xml_parser::isXMLComment(content);
}

// parse XML comment
void XMLParser::parseXMLComment() {

    return xml_parser::parseXMLComment(content, doneReading, totalBytes);
}

// check if CDATA
bool XMLParser::isCDATA() {

    return xml_parser::isCDATA(content);
}

// parse CDATA
std::string_view XMLParser::parseCDATA() {
    
    return xml_parser::parseCDATA(content, doneReading, totalBytes);
}

// check if processing instruction
bool XMLParser::isProcessingInstruction() {

    return xml_parser::isProcessingInstruction(content);
}

// parse processing instruction
void XMLParser::parseProcessingInstruction() {

    return xml_parser::parseProcessingInstruction(content);
}

// check if end tag
bool XMLParser::isEndTag() {

    return xml_parser::isEndTag(content);
}

// parse end tag
void XMLParser::parseEndTag() {

    return xml_parser::parseEndTag(content);
}

// check if start tag
bool XMLParser::isStartTag() {
    return xml_parser::isStartTag(content);
}

// parse start tag
std::string_view XMLParser::parseStartTag() {
    return xml_parser::parseStartTag(content);
}

// check if namespace
bool XMLParser::isXMLNamespace() {

    return xml_parser::isXMLNamespace(content);
}

// parse XML namespace
void XMLParser::parseXMLNamespace() {

    return xml_parser::parseXMLNamespace(content);
}

// parse attribute
std::size_t XMLParser::parseAttribute() {
    
    return xml_parser::parseAttribute(content);
}

// End tracing document
void XMLParser::EndTracing() {

    return xml_parser::EndTracing();
}

// get method for total bytes
long XMLParser::getTotalBytes() {
    return totalBytes;
}

bool XMLParser::getDoneReading() {
    return doneReading;
}
/*
    xml_parser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XML_PARSER_HPP
#define INCLUDED_XML_PARSER_HPP

#include <string_view>

// parse XML declaration
void parseXMLDeclaration(std::string_view& data);

// parse DOCTYPE
void parseDOCTYPE(std::string_view& data);

// refill content preserving unprocessed
void refillContentUnprocessed(std::string_view& data, bool& doneReading, long& totalBytes);

#endif

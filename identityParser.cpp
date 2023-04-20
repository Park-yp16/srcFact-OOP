/*
    identityParser.cpp

    Include file for an identity transformation of XML.
    The output XML is the same (as much as possible) as the input XML.

    There are no CDATA parts, but escape all >, <, and & in Character and CDATA content.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <optional>

#include "identityParser.hpp"

using namespace std::literals::string_view_literals;

identityParser::identityParser() {}

void identityParser::handleStartDocument() {}

void identityParser::handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {

    std::cout << "<?xml version=\"" << version << "\" encoding=\"" << encoding.value() << "\" standalone=\"" << standalone.value() << "\"?>\n";
}

void identityParser::handleDOCTYPE() {}

void identityParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {

    std::cout << "<" << qName << ">";
}

void identityParser::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {

    std::cout << "</" << qName << ">";
}

void identityParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

    std::cout.seekp(-1, std::ios::cur);
    std::cout << " " << qName << "=\"" << value << "\">";
}

void identityParser::handleNamespace(std::string_view prefix, std::string_view uri) {
    
    std::cout.seekp(-1, std::ios::cur);
    if(prefix.empty())
        std::cout << " xmlns=\"" << uri << "\">";
    else
        std::cout << " xmlns:" << prefix << "=\"" << uri << "\">";
}

void identityParser::handleComment(std::string_view comment) {

    std::cout << "<!--" << comment << "-->";
}


void identityParser::handleCDATA(std::string_view characters) {

    if(characters == "<") {
            std::cout << "&lt;";
        } else if(characters == "&") {
            std::cout << "&amp;";
        } else if(characters == ">") {
            std::cout << "&gt;";
        } else {
            std::cout << characters;
        }
}

void identityParser::handleProcessingInstruction(std::string_view target, std::string_view data) {

    std::cout << "<?" << target << " " << data << "?>";
}

void identityParser::handleCharacterEntityReferences(std::string_view characters) {

    if(characters == "<") {
        std::cout << "&lt;";
    } else if(characters == "&") {
        std::cout << "&amp;";
    } else if(characters == ">") {
        std::cout << "&gt;";
    } else {
        std::cout << characters;
    }
}

void identityParser::handleCharacterNonEntityReferences(std::string_view characters) {

    if(characters == "<") {
        std::cout << "&lt;";
    } else if(characters == "&") {
        std::cout << "&amp;";
    } else if(characters == ">") {
        std::cout << "&gt;";
    } else {
        std::cout << characters;
    }
}

void identityParser::handleEndDocument() {

    // std::cout << "\n";
    // std::cout.close();
}
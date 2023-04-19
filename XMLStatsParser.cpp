/*
    XMLStatsParser.cpp

    Markdown report with the number of each part of XML.
    e.g., the number of start tags, end tags, attributes, character sections, etc.
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include "XMLStatsParser.hpp"

void XMLStatsParser::handleStartDocument() {

    ++startDocCount;
}

void XMLStatsParser::handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {

    ++XMLDeclarationCount;
}

void XMLStatsParser::handleDOCTYPE() {

    ++DOCTYPECount;
}

void XMLStatsParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {

    ++startTagCount;
}

void XMLStatsParser::handleEndTag(std::string_view prefix, std::string_view qName, std::string_view localName) {

    ++endTagCount;
}

void XMLStatsParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

    ++attributeCount;
}

void XMLStatsParser::handleNamespace(std::string_view prefix, std::string_view uri) {

    ++namespaceCount;
}

void XMLStatsParser::handleComment(std::string_view comment) {

    ++commentCount;
}

void XMLStatsParser::handleCDATA(std::string_view characters) {

    ++CDATACount;
}

void XMLStatsParser::handleProcessingInstruction(std::string_view target, std::string_view data) {

    ++PICount;
}

void XMLStatsParser::handleCharacterEntityReferences(std::string_view characters) {

    ++CERCount;
}

void XMLStatsParser::handleCharacterNonEntityReferences(std::string_view characters) {

   ++nonCERCount;
}

void XMLStatsParser::handleEndDocument() {

    ++endDocCount;
}

// Get method for startDocCount
int XMLStatsParser::getStartDocCount(){

    return startDocCount;
}

// Get method for XMLDeclarationCount
int XMLStatsParser::getXMLDeclarationCount(){

    return XMLDeclarationCount;
}

// Get method for DOCTYPECount
int XMLStatsParser::getDOCTYPECount(){

    return DOCTYPECount;
}

// Get method for CERCount
int XMLStatsParser::getCERCount(){

    return CERCount;
}

// Get method for nonCERCount
int XMLStatsParser::getNonCERCount(){

    return nonCERCount;
}

// Get method for commentCount
int XMLStatsParser::getCommentCount(){

    return commentCount;
}

// Get method for CDATACount
int XMLStatsParser::getCDATACount(){

    return CDATACount;
}

// Get method for PICount
int XMLStatsParser::getPICount(){

    return PICount;
}

// Get method for endTagCount
int XMLStatsParser::getEndTagCount(){

    return endTagCount;
}

// Get method for startTagCount
int XMLStatsParser::getStartTagCount(){

    return startTagCount;
}

// Get method for namespaceCount
int XMLStatsParser::getNamespaceCount(){

    return namespaceCount;
}

// Get method for attributeCount
int XMLStatsParser::getAttributeCount(){

    return attributeCount;
}

// Get method for endDocCount
int XMLStatsParser::getEndDocCount(){

    return endDocCount;
}

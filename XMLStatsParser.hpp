/*
    XMLStatsParser.hpp

    Markdown report with the number of each part of XML.
    e.g., the number of start tags, end tags, attributes, character sections, etc.
*/

#ifndef INCLUDED_XMLSTATSPARSER_HPP
#define INCLUDED_XMLSTATSPARSER_HPP

#include "XMLParserHandler.hpp"

class XMLStatsParser : public XMLParserHandler {

    private:
    
    int startDocCount = 0;
    int XMLDeclarationCount = 0;
    int DOCTYPECount = 0;
    int CERCount = 0;
    int nonCERCount = 0;
    int commentCount = 0;
    int CDATACount = 0;
    int PICount = 0;
    int endTagCount = 0;
    int startTagCount = 0;
    int namespaceCount = 0;
    int attributeCount = 0;
    int endDocCount = 0;

    // Override function for handlers
    void handleStartDocument() override;

    void handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) override;

    void handleDOCTYPE() override;

    void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;

    void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;

    void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) override;

    void handleNamespace(std::string_view prefix, std::string_view uri) override;

    void handleComment(std::string_view comment) override;

    void handleCDATA(std::string_view characters) override;

    void handleProcessingInstruction(std::string_view target, std::string_view data) override;

    void handleCharacterEntityReferences(std::string_view characters) override;

    void handleCharacterNonEntityReferences(std::string_view characters) override;

    void handleEndDocument() override;

    public:

    XMLStatsParser();

    // Get method for startDocCount
    int getStartDocCount();

    // Get method for XMLDeclarationCount
    int getXMLDeclarationCount();

    // Get method for DOCTYPECount
    int getDOCTYPECount();

    // Get method for CERCount
    int getCERCount();

    // Get method for nonCERCount
    int getNonCERCount();

    // Get method for commentCount
    int getCommentCount();

    // Get method for CDATACount
    int getCDATACount();

    // Get method for PICount
    int getPICount();

    // Get method for endTagCount
    int getEndTagCount();

    // Get method for startTagCount
    int getStartTagCount();

    // Get method for namespaceCount
    int getNamespaceCount();

    // Get method for attributeCount
    int getAttributeCount();

    // Get method for endDocCount
    int getEndDocCount();

};

#endif

/*
    identityParser.hpp

    Include file for an identity transformation of XML.
    The output XML is the same (as much as possible) as the input XML.

    There are no CDATA parts, but escape all >, <, and & in Character and CDATA content.
*/

#ifndef INCLUDED_IDENTITYPARSER_HPP
#define INCLUDED_IDENTITYPARSER_HPP

#include "XMLParser.hpp"

class identityParser : public XMLParserHandler {

    private:

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

    identityParser();

};

#endif

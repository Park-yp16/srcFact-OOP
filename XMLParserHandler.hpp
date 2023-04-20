/*
    XMLParserHandler.hpp

    Include file for XML parsing handler class
*/

#ifndef XMLPARSERHANDLER_HPP
#define XMLPARSERHANDLER_HPP

#include <string_view>
#include <optional>

class XMLParserHandler {
    public:

    virtual void handleStartDocument() {};

    virtual void handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {};

    virtual void handleDOCTYPE() {};

    virtual void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    virtual void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    virtual void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {};

    virtual void handleNamespace(std::string_view prefix, std::string_view uri) {};

    virtual void handleComment(std::string_view comment) {};

    virtual void handleCDATA(std::string_view characters) {};

    virtual void handleProcessingInstruction(std::string_view target, std::string_view data) {};

    virtual void handleCharacterEntityReferences(std::string_view characters) {};

    virtual void handleCharacterNonEntityReferences(std::string_view characters) {};

    virtual void handleEndDocument() {};
};

#endif

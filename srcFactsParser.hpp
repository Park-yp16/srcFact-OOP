/*
    srcFactsParser.hpp

    Include file for srcFacts parsing class
*/

#ifndef INCLUDED_SRCFACTSPARSER_HPP
#define INCLUDED_SRCFACTSPARSER_HPP

#include "XMLParserHandler.hpp"

#include <string>

class srcFactsParser : public XMLParserHandler {

    private:
    
    std::string url;
    int textSize = 0;
    int loc = 0;
    int exprCount = 0;
    int functionCount = 0;
    int classCount = 0;
    int unitCount = 0;
    int declCount = 0;
    int commentCount = 0;
    int returnCount = 0;
    int lineCommentCount = 0;
    int literalCount = 0;

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

    srcFactsParser();

    // Get method for URL
    std::string getURL();

    // Get method for textsize
    int getTextsize();

    // Get method for LOC
    int getLOC();

    // Get method for exprCount
    int getExprCount();

    // Get method for functionCount
    int getFunctionCount();

    // Get method for classCount
    int getClassCount();

    // Get method for unitCount
    int getUnitCount();

    // Get method for declCount
    int getDeclCount();

    // Get method for commentCount
    int getCommentCount();

    // Get method for returnCount
    int getReturnCount();

    // Get method for lineCommentCount
    int getLineCommentCount();

    // Get method for literalCount
    int getLiteralCount();
};

#endif

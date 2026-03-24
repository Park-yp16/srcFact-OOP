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

    // [FIX] 모든 getter에 const 추가: 내부 상태를 변경하지 않는 함수는 const로 선언해야 함
    // Get method for URL
    std::string getURL() const;

    // Get method for textsize
    int getTextsize() const;

    // Get method for LOC
    int getLOC() const;

    // Get method for exprCount
    int getExprCount() const;

    // Get method for functionCount
    int getFunctionCount() const;

    // Get method for classCount
    int getClassCount() const;

    // Get method for unitCount
    int getUnitCount() const;

    // Get method for declCount
    int getDeclCount() const;

    // Get method for commentCount
    int getCommentCount() const;

    // Get method for returnCount
    int getReturnCount() const;

    // Get method for lineCommentCount
    int getLineCommentCount() const;

    // Get method for literalCount
    int getLiteralCount() const;
};

#endif

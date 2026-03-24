/*
    srcFactsParser.cpp

    Implementation file for srcFacts parsing class
*/

#include "srcFactsParser.hpp"
#include <algorithm>

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

srcFactsParser::srcFactsParser() {}

void srcFactsParser::handleStartDocument() {}

void srcFactsParser::handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {}

void srcFactsParser::handleDOCTYPE() {}

void srcFactsParser::handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {

    if (localName == "expr"sv) {
        ++exprCount;
    } else if (localName == "decl"sv) {
        ++declCount;
    } else if (localName == "comment"sv) {
        ++commentCount;
    } else if (localName == "function"sv) {
        ++functionCount;
    } else if (localName == "unit"sv) {
        ++unitCount;
    } else if (localName == "class"sv) {
        ++classCount;
    } else if (localName == "return"sv) {
        ++returnCount;
    }
}

void srcFactsParser::handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {}

void srcFactsParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

    if (localName == "url"sv)
        url = value;
    // [FIX] inEscape 제거: localName이 "escape"이면서 동시에 "char"일 수 없어 조건이 절대 true가 되지 않는 버그였음
    // escape 처리는 qName을 기준으로 부모 태그 상태를 추적해야 하므로 현재 구조에서는 미구현 상태로 둠
    // [FIX] if → else if 로 수정: value는 "line"이면서 동시에 "string"일 수 없으므로 else if가 의미상 올바름
    if (value == "line"sv) {
        ++lineCommentCount;
    } else if (value == "string"sv) {
        ++literalCount;
    }
}

void srcFactsParser::handleNamespace(std::string_view prefix, std::string_view uri) {}

void srcFactsParser::handleComment(std::string_view comment) {}

void srcFactsParser::handleCDATA(std::string_view characters) {

    textSize += static_cast<int>(characters.size());
    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
}

void srcFactsParser::handleProcessingInstruction(std::string_view target, std::string_view data) {}

void srcFactsParser::handleCharacterEntityReferences(std::string_view characters) {

    ++textSize;
}

void srcFactsParser::handleCharacterNonEntityReferences(std::string_view characters) {

    loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    textSize += static_cast<int>(characters.size());
}

void srcFactsParser::handleEndDocument() {}

// [FIX] 모든 getter 구현에 const 추가 (헤더 선언과 일치시킴)
// get method for URL
std::string srcFactsParser::getURL() const {

    return url;
}

//get method for textsize
int srcFactsParser::getTextsize() const {

    return textSize;
}

//get method for loc
int srcFactsParser::getLOC() const {

    return loc;
}

//get method for exprCount
int srcFactsParser::getExprCount() const {

    return exprCount;
}

//get method for functionCount
int srcFactsParser::getFunctionCount() const {

    return functionCount;
}

//get method for classCount
int srcFactsParser::getClassCount() const {

    return classCount;
}

//get method for unitCount
int srcFactsParser::getUnitCount() const {

    return unitCount;
}

//get method for declCount
int srcFactsParser::getDeclCount() const {

    return declCount;
}

//get method for commentCount
int srcFactsParser::getCommentCount() const {

    return commentCount;
}

//get method for returnCount
int srcFactsParser::getReturnCount() const {

    return returnCount;
}

//get method for lineCommentCount
int srcFactsParser::getLineCommentCount() const {

    return lineCommentCount;
}

//get method for literalCount
int srcFactsParser::getLiteralCount() const {

    return literalCount;
}

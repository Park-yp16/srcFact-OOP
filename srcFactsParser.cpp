/*
    srcFactsParser.cpp

    Implementation file for srcFacts parsing class
*/

#include "srcFactsParser.hpp"
#include "XMLParser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

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

void srcFactsParser::handleEndTag(std::string_view prefix, std::string_view qName, std::string_view localName) {}

void srcFactsParser::handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

    bool inEscape = localName == "escape"sv;
    if (localName == "url"sv)
        url = value;
    // convert special srcML escaped element to characters
    if (inEscape && localName == "char"sv /* && inUnit */) {
        // use strtol() instead of atoi() since strtol() understands hex encoding of '0x0?'
        [[maybe_unused]] char escapeValue = (char)strtol(value.data(), NULL, 0);
    }
    if(value == "line"sv) {
        ++lineCommentCount;
    }
    if(value == "string"sv) {
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

// get method for URL
std::string srcFactsParser::getURL() {

    return url;
}

//get method for textsize
int srcFactsParser::getTextsize() {

    return textSize;
}

//get method for loc
int srcFactsParser::getLOC() {

    return loc;
}

//get method for exprCount
int srcFactsParser::getExprCount() {

    return exprCount;
}

//get method for functionCount
int srcFactsParser::getFunctionCount() {

    return functionCount;
}

//get method for classCount
int srcFactsParser::getClassCount() {
    
    return classCount;
}

//get method for unitCount
int srcFactsParser::getUnitCount() {

    return unitCount;
}

//get method for declCount
int srcFactsParser::getDeclCount() {

    return declCount;
}

//get method for commentCount
int srcFactsParser::getCommentCount() {

    return commentCount;
}

//get method for returnCount
int srcFactsParser::getReturnCount() {

    return returnCount;
}

//get method for lineCommentCount
int srcFactsParser::getLineCommentCount() {

    return lineCommentCount;
}

//get method for literalCount
int srcFactsParser::getLiteralCount() {

    return literalCount;
}

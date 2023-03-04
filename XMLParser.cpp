/*
    XMLParser.cpp

    Implementation file for xml parser functions
*/

#include "XMLParser.hpp"
#include "refillContent.hpp"
#include <iostream>
#include <bitset>
#include <optional>
#include <cassert>
#include <algorithm>
#include <iomanip>


// provides literal string operator""sv
using namespace std::literals::string_view_literals;

const std::bitset<128> xmlNameMask("00000111111111111111111111111110100001111111111111111111111111100000001111111111011000000000000000000000000000000000000000000000");

constexpr auto WHITESPACE = " \n\t\r"sv;
constexpr auto NAMEEND = "> /\":=\n\t\r"sv;

// trace parsing
#ifdef TRACE
#undef TRACE
#define HEADER(m) std::clog << "\033[1m" << std::setw(10) << std::left << m << "\u001b[0m" << '\t'
#define TRACE0() ""
#define TRACE1(l1, n1)                         "\033[1m" << l1 << "\u001b[0m" << "|" << "\u001b[31;1m" << n1 << "\u001b[0m" << "| "
#define TRACE2(l1, n1, l2, n2)                 TRACE1(l1,n1)             << TRACE1(l2,n2)
#define TRACE3(l1, n1, l2, n2, l3, n3)         TRACE2(l1,n1,l2,n2)       << TRACE1(l3,n3)
#define TRACE4(l1, n1, l2, n2, l3, n3, l4, n4) TRACE3(l1,n1,l2,n2,l3,n3) << TRACE1(l4,n4)
#define GET_TRACE(_2,_3,_4,_5,_6,_7,_8,_9,NAME,...) NAME
#define TRACE(m,...) HEADER(m) << GET_TRACE(__VA_ARGS__, TRACE4, _UNUSED, TRACE3, _UNUSED, TRACE2, _UNUSED, TRACE1, TRACE0, TRACE0)(__VA_ARGS__) << '\n';
#else
#define TRACE(...)
#endif

//constructor
XMLParser::XMLParser(const std::string_view& content, long& totalBytes, bool& doneReading)
: content(content), totalBytes(totalBytes), doneReading(doneReading) {}

// Start tracing document
void XMLParser::startTracing() {

    TRACE("START DOCUMENT");
}

// check for file input
void XMLParser::checkFIleInput() {

    int bytesRead = refillContent();
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0) {
        std::cerr << "parser error : Empty file\n";
        exit(1);
    }
    totalBytes += bytesRead;
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// check if declaration
bool XMLParser::isXMLDeclaration() {

    return (content[0] == '<' && content[1] == '?' && content[2] == 'x' && content[3] == 'm' && content[4] == 'l' && content[5] == ' ');
}

// parse XML declaration
void XMLParser::parseXMLDeclaration() {

    assert(content.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
    content.remove_prefix("<?xml"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));

    // parse required version
    std::size_t nameEndPosition = content.find_first_of("= ");
    const std::string_view attr(content.substr(0, nameEndPosition));
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    content.remove_prefix("="sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    const auto delimiter = content[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
        exit(1);
    }
    content.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = content.find(delimiter);
    if (valueEndPosition == content.npos) {
        std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
        exit(1);
    }
    if (attr != "version"sv) {
        std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view version(content.substr(0, valueEndPosition));
    content.remove_prefix(valueEndPosition);
    content.remove_prefix("\""sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));

    // parse optional encoding and standalone attributes
    std::optional<std::string_view> encoding;
    std::optional<std::string_view> standalone;
    if (content[0] != '?') {
        std::size_t nameEndPosition = content.find_first_of("= ");
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        assert(content.compare(0, "="sv.size(), "="sv) == 0);
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        auto delimiter2 = content[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = content.find(delimiter2);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (attr2 == "encoding"sv) {
            encoding = content.substr(0, valueEndPosition);
        }
        else if (attr2 == "standalone"sv) {
            standalone = content.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        content.remove_prefix(valueEndPosition + 1);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }
    if (content[0] != '?') {
        std::size_t nameEndPosition = content.find_first_of("= ");
        if (nameEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(content.substr(0, nameEndPosition));
        content.remove_prefix(nameEndPosition);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        content.remove_prefix("="sv.size());
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
        const auto delimiter2 = content[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        content.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = content.find(delimiter2);
        if (valueEndPosition == content.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (!standalone && attr2 == "standalone"sv) {
            standalone = content.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        // assert(content[valueEndPosition + 1] == '"');
        content.remove_prefix(valueEndPosition + 1);
        content.remove_prefix(content.find_first_not_of(WHITESPACE));
    }
    TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
    assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
    content.remove_prefix("?>"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// check if DOCTYPE
bool XMLParser::isDOCTYPE() {

    return (content[1] == '!' && content[0] == '<' && content[2] == 'D' && content[3] == 'O' && content[4] == 'C' && content[5] == 'T' && content[6] == 'Y' && content[7] == 'P' && content[8] == 'E' && content[9] == ' ');
}

// parse DOCTYPE
void XMLParser::parseDOCTYPE() {

    assert(content.compare(0, "<!DOCTYPE "sv.size(), "<!DOCTYPE "sv) == 0);
    content.remove_prefix("<!DOCTYPE"sv.size());
    int depthAngleBrackets = 1;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    bool inComment = false;
    std::size_t p = 0;
    while ((p = content.find_first_of("<>'\"-"sv, p)) != content.npos) {
        if (content.compare(p, "<!--"sv.size(), "<!--"sv) == 0) {
            inComment = true;
            p += "<!--"sv.size();
            continue;
        }
        else if (content.compare(p, "-->"sv.size(), "-->"sv) == 0) {
            inComment = false;
            p += "-->"sv.size();
            continue;
        }
        if (inComment) {
            ++p;
            continue;
        }
        if (content[p] == '<' && !inSingleQuote && !inDoubleQuote) {
            ++depthAngleBrackets;
        }
        else if (content[p] == '>' && !inSingleQuote && !inDoubleQuote) {
            --depthAngleBrackets;
        }
        else if (content[p] == '\'') {
            inSingleQuote = !inSingleQuote;
        }
        else if (content[p] == '"') {
            inDoubleQuote = !inDoubleQuote;
        }
        if (depthAngleBrackets == 0)
            break;
        ++p;
    }
    [[maybe_unused]] const std::string_view contents(content.substr(0, p));
    TRACE("DOCTYPE", "contents", contents);
    content.remove_prefix(p);
    assert(content[0] == '>');
    content.remove_prefix(">"sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// refill content preserving unprocessed
void XMLParser::refillContentUnprocessed() {
       
    int bytesRead = refillContent(content);
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0) {
        doneReading = true;
    }
    totalBytes += bytesRead;
}

// check if character entity references
bool XMLParser::isCharacterEntityReferences() {
    
    return (content[0] == '&');
}

// parse character entity references
std::string_view XMLParser::parseCharacterEntityReferences() {
    
    std::string_view unescapedCharacter;
    std::string_view escapedCharacter;
    if (content[1] == 'l' && content[2] == 't' && content[3] == ';') {
        unescapedCharacter = "<";
        escapedCharacter = "&lt;"sv;
    } else if (content[1] == 'g' && content[2] == 't' && content[3] == ';') {
        unescapedCharacter = ">";
        escapedCharacter = "&gt;"sv;
    } else if (content[1] == 'a' && content[2] == 'm' && content[3] == 'p' && content[4] == ';') {
        unescapedCharacter = "&";
        escapedCharacter = "&amp;"sv;
    } else {
        unescapedCharacter = "&";
        escapedCharacter = "&"sv;
    }
    assert(content.compare(0, escapedCharacter.size(), escapedCharacter) == 0);
    content.remove_prefix(escapedCharacter.size());
    [[maybe_unused]] const std::string_view characters(unescapedCharacter);
    TRACE("CHARACTERS", "characters", characters);
    return characters;
}

// check if character non-entity references
bool XMLParser::isCharacterNonEntityReferences() {

    return (content[0] != '<');
}

// parse character non-entity references
std::string_view XMLParser::parseCharacterNonEntityReferences() {
    
    assert(content[0] != '<' && content[0] != '&');
    std::size_t characterEndPosition = content.find_first_of("<&");
    const std::string_view characters(content.substr(0, characterEndPosition));
    TRACE("CHARACTERS", "characters", characters);
    return characters;
}

// check if comment
bool XMLParser::isXMLComment() {
    
    return (content[1] == '!' /* && content[0] == '<' */ && content[2] == '-' && content[3] == '-');
}

// parse XML comment
void XMLParser::parseXMLComment() {

    assert(content.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
    content.remove_prefix("<!--"sv.size());
    std::size_t tagEndPosition = content.find("-->"sv);
    if (tagEndPosition == content.npos) {
        
        // refill content preserving unprocessed
        refillContentUnprocessed();
        tagEndPosition = content.find("-->"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated XML comment\n";
            exit(1);
        }
    }
    [[maybe_unused]] const std::string_view comment(content.substr(0, tagEndPosition));
    TRACE("COMMENT", "content", comment);
    content.remove_prefix(tagEndPosition);
}

// check if CDATA
bool XMLParser::isCDATA() {

    return (content[1] == '!' /* && content[0] == '<' */ && content[2] == '[' && content[3] == 'C' && content[4] == 'D' &&
            content[5] == 'A' && content[6] == 'T' && content[7] == 'A' && content[8] == '[');
}

// parse CDATA
std::string_view XMLParser::parseCDATA() {
    
    content.remove_prefix("<![Ccontent["sv.size());
    std::size_t tagEndPosition = content.find("]]>"sv);
    if (tagEndPosition == content.npos) {
        
        // refill content preserving unprocessed
        refillContentUnprocessed();
        tagEndPosition = content.find("-->"sv);
        tagEndPosition = content.find("]]>"sv);
        if (tagEndPosition == content.npos) {
            std::cerr << "parser error : Unterminated CDATA\n";
            exit(1);
        }
    }
    const std::string_view characters(content.substr(0, tagEndPosition));
    content.remove_prefix(tagEndPosition);
    content.remove_prefix("]]>"sv.size());

    return characters;
}

// check if processing instruction
bool XMLParser::isProcessingInstruction() {

    return (content[1] == '?' /* && content[0] == '<' */);
}

// parse processing instruction
void XMLParser::parseProcessingInstruction() {

    assert(content.compare(0, "<?"sv.size(), "<?"sv) == 0);
    content.remove_prefix("<?"sv.size());
    std::size_t tagEndPosition = content.find("?>"sv);
    if (tagEndPosition == content.npos) {
        std::cerr << "parser error: Incomplete XML declaration\n";
        exit(1);
    }
    std::size_t nameEndPosition = content.find_first_of(NAMEEND);
    if (nameEndPosition == content.npos) {
        std::cerr << "parser error : Unterminated processing instruction\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view target(content.substr(0, nameEndPosition));
    [[maybe_unused]] const std::string_view data(content.substr(nameEndPosition, tagEndPosition - nameEndPosition));
    TRACE("PI", "target", target, "data", data);
    content.remove_prefix(tagEndPosition);
    assert(content.compare(0, "?>"sv.size(), "?>"sv) == 0);
    content.remove_prefix("?>"sv.size());
}

// check if end tag
bool XMLParser::isEndTag() {

    return (content[1] == '/' /* && content[0] == '<' */);
}

// parse end tag
void XMLParser::parseEndTag() {

    assert(content.compare(0, "</"sv.size(), "</"sv) == 0);
    content.remove_prefix("</"sv.size());
    if (content[0] == ':') {
        std::cerr << "parser error : Invalid end tag name\n";
        exit(1);
    }
    std::size_t nameEndPosition = content.find_first_of(NAMEEND);
    if (nameEndPosition == content.size()) {
        std::cerr << "parser error : Unterminated end tag '" << content.substr(0, nameEndPosition) << "'\n";
        exit(1);
    }
    size_t colonPosition = 0;
    if (content[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    const std::string_view qName(content.substr(0, nameEndPosition));
    if (qName.empty()) {
        std::cerr << "parser error: EndTag: invalid element name\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
    [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
    TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    assert(content.compare(0, ">"sv.size(), ">"sv) == 0);
    content.remove_prefix(">"sv.size());
}

// check if start tag
bool XMLParser::isStartTag() {
    return (content[0] == '<');
}

// parse start tag
std::string_view XMLParser::parseStartTag() {
    assert(content.compare(0, "<"sv.size(), "<"sv) == 0);
    content.remove_prefix("<"sv.size());
    if (content[0] == ':') {
        std::cerr << "parser error : Invalid start tag name\n";
        exit(1);
    }
    std::size_t nameEndPosition = content.find_first_of(NAMEEND);
    if (nameEndPosition == content.size()) {
        std::cerr << "parser error : Unterminated start tag '" << content.substr(0, nameEndPosition) << "'\n";
        exit(1);
    }
    size_t colonPosition = 0;
    if (content[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    const std::string_view qName(content.substr(0, nameEndPosition));
    if (qName.empty()) {
        std::cerr << "parser error: StartTag: invalid element name\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
    const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0, nameEndPosition));
    TRACE("START TAG", "qName", qName, "prefix", prefix, "localName", localName);
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));

    return localName;
}

// check if namespace
bool XMLParser::isXMLNamespace() {

    return (content[0] == 'x' && content[1] == 'm' && content[2] == 'l' && content[3] == 'n' && content[4] == 's' && (content[5] == ':' || content[5] == '='));
}

// parse XML namespace
void XMLParser::parseXMLNamespace() {

    assert(content.compare(0, "xmlns"sv.size(), "xmlns"sv) == 0);
    content.remove_prefix("xmlns"sv.size());
    std::size_t nameEndPosition = content.find('=');
    if (nameEndPosition == content.npos) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    std::size_t prefixSize = 0;
    if (content[0] == ':') {
        content.remove_prefix(":"sv.size());
        --nameEndPosition;
        prefixSize = nameEndPosition;
    }
    [[maybe_unused]] const std::string_view prefix(content.substr(0, prefixSize));
    content.remove_prefix(nameEndPosition);
    content.remove_prefix("="sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    if (content.empty()) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    const auto delimiter = content[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    content.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = content.find(delimiter);
    if (valueEndPosition == content.npos) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view uri(content.substr(0, valueEndPosition));
    TRACE("NAMESPACE", "prefix", prefix, "uri", uri);
    content.remove_prefix(valueEndPosition);
    assert(content.compare(0, "\""sv.size(), "\""sv) == 0);
    content.remove_prefix("\""sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
}

// parse attribute
std::size_t XMLParser::parseAttribute() {
    
    std::size_t nameEndPosition = content.find_first_of(NAMEEND);
    if (nameEndPosition == content.size()) {
        std::cerr << "parser error : Empty attribute name" << '\n';
        exit(1);
    }
    size_t colonPosition = 0;
    if (content[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = content.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    std::string_view qName(content.substr(0, nameEndPosition));
    [[maybe_unused]] std::string_view prefix(qName.substr(0, colonPosition));
    std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
    content.remove_prefix(nameEndPosition);
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    if (content.empty()) {
        std::cerr << "parser error : attribute " << qName << " incomplete attribute\n";
        exit(1);
    }
    if (content[0] != '=') {
        std::cerr << "parser error : attribute " << qName << " missing =\n";
        exit(1);
    }
    content.remove_prefix("="sv.size());
    content.remove_prefix(content.find_first_not_of(WHITESPACE));
    const auto delimiter = content[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
        exit(1);
    }
    content.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = content.find(delimiter);
    if (valueEndPosition == content.npos) {
        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
        exit(1);
    }
    const std::string_view value(content.substr(0, valueEndPosition));
    TRACE("ATTRIBUTE", "qname", qName, "prefix", prefix, "localName", localName, "value", value);
    return valueEndPosition;
}

// End tracing document
void XMLParser::EndTracing() {

    TRACE("END DOCUMENT");
}

// get method for total bytes
long XMLParser::getTotalBytes() {
    return totalBytes;
}

bool XMLParser::getDoneReading() {
    return doneReading;
}
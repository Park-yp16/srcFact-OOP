/*
    xml_parser.cpp

    Implementation file for xml parser functions
*/

#include "xml_parser.hpp"
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

// Start tracing document
void xml_parser::startTracing() {

    TRACE("START DOCUMENT");
}

// check for file input
void xml_parser::checkFIleInput(std::string_view& text, long& totalBytes) {

    int bytesRead = refillContent(text);
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0) {
        std::cerr << "parser error : Empty file\n";
        exit(1);
    }
    totalBytes += bytesRead;
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// check if declaration
bool xml_parser::isXMLDeclaration(std::string_view& text) {

    return (text[0] == '<' && text[1] == '?' && text[2] == 'x' && text[3] == 'm' && text[4] == 'l' && text[5] == ' ');
}

// parse XML declaration
void xml_parser::parseXMLDeclaration(std::string_view& text) {

    assert(text.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
    text.remove_prefix("<?xml"sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));

    // parse required version
    std::size_t nameEndPosition = text.find_first_of("= ");
    const std::string_view attr(text.substr(0, nameEndPosition));
    text.remove_prefix(nameEndPosition);
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    text.remove_prefix("="sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    const auto delimiter = text[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
        exit(1);
    }
    text.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = text.find(delimiter);
    if (valueEndPosition == text.npos) {
        std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
        exit(1);
    }
    if (attr != "version"sv) {
        std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view version(text.substr(0, valueEndPosition));
    text.remove_prefix(valueEndPosition);
    text.remove_prefix("\""sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));

    // parse optional encoding and standalone attributes
    std::optional<std::string_view> encoding;
    std::optional<std::string_view> standalone;
    if (text[0] != '?') {
        std::size_t nameEndPosition = text.find_first_of("= ");
        if (nameEndPosition == text.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(text.substr(0, nameEndPosition));
        text.remove_prefix(nameEndPosition);
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
        assert(text.compare(0, "="sv.size(), "="sv) == 0);
        text.remove_prefix("="sv.size());
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
        auto delimiter2 = text[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        text.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = text.find(delimiter2);
        if (valueEndPosition == text.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (attr2 == "encoding"sv) {
            encoding = text.substr(0, valueEndPosition);
        }
        else if (attr2 == "standalone"sv) {
            standalone = text.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        text.remove_prefix(valueEndPosition + 1);
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
    }
    if (text[0] != '?') {
        std::size_t nameEndPosition = text.find_first_of("= ");
        if (nameEndPosition == text.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(text.substr(0, nameEndPosition));
        text.remove_prefix(nameEndPosition);
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
        text.remove_prefix("="sv.size());
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
        const auto delimiter2 = text[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        text.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = text.find(delimiter2);
        if (valueEndPosition == text.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (!standalone && attr2 == "standalone"sv) {
            standalone = text.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        // assert(content[valueEndPosition + 1] == '"');
        text.remove_prefix(valueEndPosition + 1);
        text.remove_prefix(text.find_first_not_of(WHITESPACE));
    }
    TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
    assert(text.compare(0, "?>"sv.size(), "?>"sv) == 0);
    text.remove_prefix("?>"sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// check if DOCTYPE
bool xml_parser::isDOCTYPE(std::string_view& text) {

    return (text[1] == '!' && text[0] == '<' && text[2] == 'D' && text[3] == 'O' && text[4] == 'C' && text[5] == 'T' && text[6] == 'Y' && text[7] == 'P' && text[8] == 'E' && text[9] == ' ');
}

// parse DOCTYPE
void xml_parser::parseDOCTYPE(std::string_view& text) {

    assert(text.compare(0, "<!DOCTYPE "sv.size(), "<!DOCTYPE "sv) == 0);
    text.remove_prefix("<!DOCTYPE"sv.size());
    int depthAngleBrackets = 1;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    bool inComment = false;
    std::size_t p = 0;
    while ((p = text.find_first_of("<>'\"-"sv, p)) != text.npos) {
        if (text.compare(p, "<!--"sv.size(), "<!--"sv) == 0) {
            inComment = true;
            p += "<!--"sv.size();
            continue;
        }
        else if (text.compare(p, "-->"sv.size(), "-->"sv) == 0) {
            inComment = false;
            p += "-->"sv.size();
            continue;
        }
        if (inComment) {
            ++p;
            continue;
        }
        if (text[p] == '<' && !inSingleQuote && !inDoubleQuote) {
            ++depthAngleBrackets;
        }
        else if (text[p] == '>' && !inSingleQuote && !inDoubleQuote) {
            --depthAngleBrackets;
        }
        else if (text[p] == '\'') {
            inSingleQuote = !inSingleQuote;
        }
        else if (text[p] == '"') {
            inDoubleQuote = !inDoubleQuote;
        }
        if (depthAngleBrackets == 0)
            break;
        ++p;
    }
    [[maybe_unused]] const std::string_view contents(text.substr(0, p));
    TRACE("DOCTYPE", "contents", contents);
    text.remove_prefix(p);
    assert(text[0] == '>');
    text.remove_prefix(">"sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// refill content preserving unprocessed
void xml_parser::refillContentUnprocessed(std::string_view& text, bool& doneReading, long& totalBytes) {
       
    int bytesRead = refillContent(text);
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
bool xml_parser::isCharacterEntityReferences(std::string_view& text) {
    
    return (text[0] == '&');
}

// parse character entity references
void xml_parser::parseCharacterEntityReferences(std::string_view& text) {
    
    std::string_view unescapedCharacter;
    std::string_view escapedCharacter;
    if (text[1] == 'l' && text[2] == 't' && text[3] == ';') {
        unescapedCharacter = "<";
        escapedCharacter = "&lt;"sv;
    } else if (text[1] == 'g' && text[2] == 't' && text[3] == ';') {
        unescapedCharacter = ">";
        escapedCharacter = "&gt;"sv;
    } else if (text[1] == 'a' && text[2] == 'm' && text[3] == 'p' && text[4] == ';') {
        unescapedCharacter = "&";
        escapedCharacter = "&amp;"sv;
    } else {
        unescapedCharacter = "&";
        escapedCharacter = "&"sv;
    }
    assert(text.compare(0, escapedCharacter.size(), escapedCharacter) == 0);
    text.remove_prefix(escapedCharacter.size());
    [[maybe_unused]] const std::string_view characters(unescapedCharacter);
    TRACE("CHARACTERS", "characters", characters);
}

// check if character non-entity references
bool xml_parser::isCharacterNonEntityReferences(std::string_view& text) {

    return (text[0] != '<');
}

// parse character non-entity references
void xml_parser::parseCharacterNonEntityReferences(std::string_view& text) {
    
    assert(text[0] != '<' && text[0] != '&');
    std::size_t characterEndPosition = text.find_first_of("<&");
    const std::string_view characters(text.substr(0, characterEndPosition));
    TRACE("CHARACTERS", "characters", characters);
    text.remove_prefix(characters.size());
}

// check if comment
bool xml_parser::isXMLComment(std::string_view& text) {
    
    return (text[1] == '!' /* && text[0] == '<' */ && text[2] == '-' && text[3] == '-');
}

// parse XML comment
void xml_parser::parseXMLComment(std::string_view& text, bool& doneReading, long& totalBytes) {

    assert(text.compare(0, "<!--"sv.size(), "<!--"sv) == 0);
    text.remove_prefix("<!--"sv.size());
    std::size_t tagEndPosition = text.find("-->"sv);
    if (tagEndPosition == text.npos) {
        
        // refill content preserving unprocessed
        refillContentUnprocessed(text, doneReading, totalBytes);
        tagEndPosition = text.find("-->"sv);
        if (tagEndPosition == text.npos) {
            std::cerr << "parser error : Unterminated XML comment\n";
            exit(1);
        }
    }
    [[maybe_unused]] const std::string_view comment(text.substr(0, tagEndPosition));
    TRACE("COMMENT", "content", comment);
    text.remove_prefix(tagEndPosition);
}

// check if CDATA
bool xml_parser::isCDATA(std::string_view& text) {

    return (text[1] == '!' /* && text[0] == '<' */ && text[2] == '[' && text[3] == 'C' && text[4] == 'D' &&
            text[5] == 'A' && text[6] == 'T' && text[7] == 'A' && text[8] == '[');
}

// parse CDATA
void xml_parser::parseCDATA(std::string_view& text, bool& doneReading, long& totalBytes) {
    
    text.remove_prefix("<![Ctext["sv.size());
    std::size_t tagEndPosition = text.find("]]>"sv);
    if (tagEndPosition == text.npos) {
        
        // refill content preserving unprocessed
        refillContentUnprocessed(text, doneReading, totalBytes);
        tagEndPosition = text.find("-->"sv);
        tagEndPosition = text.find("]]>"sv);
        if (tagEndPosition == text.npos) {
            std::cerr << "parser error : Unterminated CDATA\n";
            exit(1);
        }
    }
    const std::string_view characters(text.substr(0, tagEndPosition));
    TRACE("CDATA", "characters", characters);
    text.remove_prefix(tagEndPosition);
    text.remove_prefix("]]>"sv.size());
}

// check if processing instruction
bool xml_parser::isProcessingInstruction(std::string_view& text) {

    return (text[1] == '?' /* && text[0] == '<' */);
}

// parse processing instruction
void xml_parser::parseProcessingInstruction(std::string_view& text) {

    assert(text.compare(0, "<?"sv.size(), "<?"sv) == 0);
    text.remove_prefix("<?"sv.size());
    std::size_t tagEndPosition = text.find("?>"sv);
    if (tagEndPosition == text.npos) {
        std::cerr << "parser error: Incomplete XML declaration\n";
        exit(1);
    }
    std::size_t nameEndPosition = text.find_first_of(NAMEEND);
    if (nameEndPosition == text.npos) {
        std::cerr << "parser error : Unterminated processing instruction\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view target(text.substr(0, nameEndPosition));
    [[maybe_unused]] const std::string_view data(text.substr(nameEndPosition, tagEndPosition - nameEndPosition));
    TRACE("PI", "target", target, "data", data);
    text.remove_prefix(tagEndPosition);
    assert(text.compare(0, "?>"sv.size(), "?>"sv) == 0);
    text.remove_prefix("?>"sv.size());
}

// check if end tag
bool xml_parser::isEndTag(std::string_view& text) {

    return (text[1] == '/' /* && text[0] == '<' */);
}

// parse end tag
void xml_parser::parseEndTag(std::string_view& text) {

    assert(text.compare(0, "</"sv.size(), "</"sv) == 0);
    text.remove_prefix("</"sv.size());
    if (text[0] == ':') {
        std::cerr << "parser error : Invalid end tag name\n";
        exit(1);
    }
    std::size_t nameEndPosition = text.find_first_of(NAMEEND);
    if (nameEndPosition == text.size()) {
        std::cerr << "parser error : Unterminated end tag '" << text.substr(0, nameEndPosition) << "'\n";
        exit(1);
    }
    size_t colonPosition = 0;
    if (text[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = text.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    const std::string_view qName(text.substr(0, nameEndPosition));
    if (qName.empty()) {
        std::cerr << "parser error: EndTag: invalid element name\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
    [[maybe_unused]] const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
    TRACE("END TAG", "qName", qName, "prefix", prefix, "localName", localName);
    text.remove_prefix(nameEndPosition);
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    assert(text.compare(0, ">"sv.size(), ">"sv) == 0);
    text.remove_prefix(">"sv.size());
}

// check if start tag
bool xml_parser::isStartTag(std::string_view& text) {
    return (text[0] == '<');
}

// parse start tag
void xml_parser::parseStartTag(std::string_view& text) {
    assert(text.compare(0, "<"sv.size(), "<"sv) == 0);
    text.remove_prefix("<"sv.size());
    if (text[0] == ':') {
        std::cerr << "parser error : Invalid start tag name\n";
        exit(1);
    }
    std::size_t nameEndPosition = text.find_first_of(NAMEEND);
    if (nameEndPosition == text.size()) {
        std::cerr << "parser error : Unterminated start tag '" << text.substr(0, nameEndPosition) << "'\n";
        exit(1);
    }
    size_t colonPosition = 0;
    if (text[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = text.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    const std::string_view qName(text.substr(0, nameEndPosition));
    if (qName.empty()) {
        std::cerr << "parser error: StartTag: invalid element name\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view prefix(qName.substr(0, colonPosition));
    const std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0, nameEndPosition));
    TRACE("START TAG", "qName", qName, "prefix", prefix, "localName", localName);
    text.remove_prefix(nameEndPosition);
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// check if namespace
bool xml_parser::isXMLNamespace(std::string_view& text) {

    return (text[0] == 'x' && text[1] == 'm' && text[2] == 'l' && text[3] == 'n' && text[4] == 's' && (text[5] == ':' || text[5] == '='));
}

// parse XML namespace
void xml_parser::parseXMLNamespace(std::string_view& text) {

    assert(text.compare(0, "xmlns"sv.size(), "xmlns"sv) == 0);
    text.remove_prefix("xmlns"sv.size());
    std::size_t nameEndPosition = text.find('=');
    if (nameEndPosition == text.npos) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    std::size_t prefixSize = 0;
    if (text[0] == ':') {
        text.remove_prefix(":"sv.size());
        --nameEndPosition;
        prefixSize = nameEndPosition;
    }
    [[maybe_unused]] const std::string_view prefix(text.substr(0, prefixSize));
    text.remove_prefix(nameEndPosition);
    text.remove_prefix("="sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    if (text.empty()) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    const auto delimiter = text[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    text.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = text.find(delimiter);
    if (valueEndPosition == text.npos) {
        std::cerr << "parser error : incomplete namespace\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view uri(text.substr(0, valueEndPosition));
    TRACE("NAMESPACE", "prefix", prefix, "uri", uri);
    text.remove_prefix(valueEndPosition);
    assert(text.compare(0, "\""sv.size(), "\""sv) == 0);
    text.remove_prefix("\""sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// parse attribute
void xml_parser::parseAttribute(std::string_view& text) {
    
    std::size_t nameEndPosition = text.find_first_of(NAMEEND);
    if (nameEndPosition == text.size()) {
        std::cerr << "parser error : Empty attribute name" << '\n';
        exit(1);
    }
    size_t colonPosition = 0;
    if (text[nameEndPosition] == ':') {
        colonPosition = nameEndPosition;
        nameEndPosition = text.find_first_of(NAMEEND, nameEndPosition + 1);
    }
    std::string_view qName(text.substr(0, nameEndPosition));
    [[maybe_unused]] std::string_view prefix(qName.substr(0, colonPosition));
    std::string_view localName(qName.substr(colonPosition ? colonPosition + 1 : 0));
    text.remove_prefix(nameEndPosition);
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    if (text.empty()) {
        std::cerr << "parser error : attribute " << qName << " incomplete attribute\n";
        exit(1);
    }
    if (text[0] != '=') {
        std::cerr << "parser error : attribute " << qName << " missing =\n";
        exit(1);
    }
    text.remove_prefix("="sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
    const auto delimiter = text[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
        exit(1);
    }
    text.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = text.find(delimiter);
    if (valueEndPosition == text.npos) {
        std::cerr << "parser error : attribute " << qName << " missing delimiter\n";
        exit(1);
    }
    const std::string_view value(text.substr(0, valueEndPosition));
    TRACE("ATTRIBUTE", "qname", qName, "prefix", prefix, "localName", localName, "value", value);
    text.remove_prefix(valueEndPosition);
    text.remove_prefix("\""sv.size());
    text.remove_prefix(text.find_first_not_of(WHITESPACE));
}

// End tracing document
void xml_parser::EndTracing() {

    TRACE("END DOCUMENT");
}


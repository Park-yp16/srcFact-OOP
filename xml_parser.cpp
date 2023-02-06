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

// parse XML declaration
void parseXMLDeclaration(std::string_view& data) {

    assert(data.compare(0, "<?xml "sv.size(), "<?xml "sv) == 0);
    data.remove_prefix("<?xml"sv.size());
    data.remove_prefix(data.find_first_not_of(WHITESPACE));

    // parse required version
    std::size_t nameEndPosition = data.find_first_of("= ");
    const std::string_view attr(data.substr(0, nameEndPosition));
    data.remove_prefix(nameEndPosition);
    data.remove_prefix(data.find_first_not_of(WHITESPACE));
    data.remove_prefix("="sv.size());
    data.remove_prefix(data.find_first_not_of(WHITESPACE));
    const auto delimiter = data[0];
    if (delimiter != '"' && delimiter != '\'') {
        std::cerr << "parser error: Invalid start delimiter for version in XML declaration\n";
        exit(1);
    }
    data.remove_prefix("\""sv.size());
    std::size_t valueEndPosition = data.find(delimiter);
    if (valueEndPosition == data.npos) {
        std::cerr << "parser error: Invalid end delimiter for version in XML declaration\n";
        exit(1);
    }
    if (attr != "version"sv) {
        std::cerr << "parser error: Missing required first attribute version in XML declaration\n";
        exit(1);
    }
    [[maybe_unused]] const std::string_view version(data.substr(0, valueEndPosition));
    data.remove_prefix(valueEndPosition);
    data.remove_prefix("\""sv.size());
    data.remove_prefix(data.find_first_not_of(WHITESPACE));

    // parse optional encoding and standalone attributes
    std::optional<std::string_view> encoding;
    std::optional<std::string_view> standalone;
    if (data[0] != '?') {
        std::size_t nameEndPosition = data.find_first_of("= ");
        if (nameEndPosition == data.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(data.substr(0, nameEndPosition));
        data.remove_prefix(nameEndPosition);
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
        assert(data.compare(0, "="sv.size(), "="sv) == 0);
        data.remove_prefix("="sv.size());
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
        auto delimiter2 = data[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        data.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = data.find(delimiter2);
        if (valueEndPosition == data.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (attr2 == "encoding"sv) {
            encoding = data.substr(0, valueEndPosition);
        }
        else if (attr2 == "standalone"sv) {
            standalone = data.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        data.remove_prefix(valueEndPosition + 1);
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
    }
    if (data[0] != '?') {
        std::size_t nameEndPosition = data.find_first_of("= ");
        if (nameEndPosition == data.npos) {
            std::cerr << "parser error: Incomplete attribute in XML declaration\n";
            exit(1);
        }
        const std::string_view attr2(data.substr(0, nameEndPosition));
        data.remove_prefix(nameEndPosition);
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
        data.remove_prefix("="sv.size());
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
        const auto delimiter2 = data[0];
        if (delimiter2 != '"' && delimiter2 != '\'') {
            std::cerr << "parser error: Invalid end delimiter for attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        data.remove_prefix("\""sv.size());
        std::size_t valueEndPosition = data.find(delimiter2);
        if (valueEndPosition == data.npos) {
            std::cerr << "parser error: Incomplete attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        if (!standalone && attr2 == "standalone"sv) {
            standalone = data.substr(0, valueEndPosition);
        }
        else {
            std::cerr << "parser error: Invalid attribute " << attr2 << " in XML declaration\n";
            exit(1);
        }
        // assert(content[valueEndPosition + 1] == '"');
        data.remove_prefix(valueEndPosition + 1);
        data.remove_prefix(data.find_first_not_of(WHITESPACE));
    }
    TRACE("XML DECLARATION", "version", version, "encoding", (encoding ? *encoding : ""), "standalone", (standalone ? *standalone : ""));
    assert(data.compare(0, "?>"sv.size(), "?>"sv) == 0);
    data.remove_prefix("?>"sv.size());
    data.remove_prefix(data.find_first_not_of(WHITESPACE));
}

// parse DOCTYPE
void parseDOCTYPE(std::string_view& data) {

    assert(data.compare(0, "<!DOCTYPE "sv.size(), "<!DOCTYPE "sv) == 0);
    data.remove_prefix("<!DOCTYPE"sv.size());
    int depthAngleBrackets = 1;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    bool inComment = false;
    std::size_t p = 0;
    while ((p = data.find_first_of("<>'\"-"sv, p)) != data.npos) {
        if (data.compare(p, "<!--"sv.size(), "<!--"sv) == 0) {
            inComment = true;
            p += "<!--"sv.size();
            continue;
        }
        else if (data.compare(p, "-->"sv.size(), "-->"sv) == 0) {
            inComment = false;
            p += "-->"sv.size();
            continue;
        }
        if (inComment) {
            ++p;
            continue;
        }
        if (data[p] == '<' && !inSingleQuote && !inDoubleQuote) {
            ++depthAngleBrackets;
        }
        else if (data[p] == '>' && !inSingleQuote && !inDoubleQuote) {
            --depthAngleBrackets;
        }
        else if (data[p] == '\'') {
            inSingleQuote = !inSingleQuote;
        }
        else if (data[p] == '"') {
            inDoubleQuote = !inDoubleQuote;
        }
        if (depthAngleBrackets == 0)
            break;
        ++p;
    }
    [[maybe_unused]] const std::string_view contents(data.substr(0, p));
    TRACE("DOCTYPE", "contents", contents);
    data.remove_prefix(p);
    assert(data[0] == '>');
    data.remove_prefix(">"sv.size());
    data.remove_prefix(data.find_first_not_of(WHITESPACE));
}

// bool doneReading = false;
// refill content preserving unprocessed
void refillContentUnprocessed(std::string_view& data, bool& doneReading, long& totalBytes){
       
    int bytesRead = refillContent(data);
    if (bytesRead < 0) {
        std::cerr << "parser error : File input error\n";
        exit(1);
    }
    if (bytesRead == 0) {
        doneReading = true;
    }
    totalBytes += bytesRead;
}

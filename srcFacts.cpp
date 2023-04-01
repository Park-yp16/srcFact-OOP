/*
    srcFacts.cpp

    Produces a report with various measures of source code.
    Supports C++, C, Java, and C#. Input is an XML file in the srcML format,
    and output is a markdown table with the measures. Performance statistics
    are output to standard error.
    The code includes a complete XML parser:
    * Characters and content from XML is in UTF-8
    * DTD declarations are allowed, but not fine-grained parsed
    * No checking for well-formedness
*/

#include <iostream>
#include <locale>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <string_view>
#include <optional>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <memory>
#include <stdlib.h>
#include <bitset>
#include <cassert>

#include "refillContent.hpp"
#include "XMLParser.hpp"

// provides literal string operator""sv
using namespace std::literals::string_view_literals;

int main(int argc, char* argv[]) {

    const auto startTime = std::chrono::steady_clock::now();
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

    XMLParser parser(
    []() {
        return;
    },
    [](std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {
        return;
    },
    []() {
        return;
    },
    // parse start tag
    [&](std::string_view qName, std::string_view prefix, std::string_view localName) {

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
    },
    [](std::string_view prefix, std::string_view qName, std::string_view localName) {
        return;
    },
    // parse attribute 
    [&url, &lineCommentCount, &literalCount](std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {
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
    },
    [](std::string_view prefix, std::string_view uri) {
        return;
    },
    [](std::string_view comment) {
        return;
    },
    // CDATA
    [&textSize, &loc](std::string_view characters) {
        textSize += static_cast<int>(characters.size());
        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
    },
    [](std::string_view target, std::string_view data) {
        return;
    },
    // parse character entity references
    [& textSize](std::string_view characters) {
        ++textSize;
    },
    // parse character non-entity references
    [&textSize, &loc](std::string_view characters) {
        loc += static_cast<int>(std::count(characters.cbegin(), characters.cend(), '\n'));
        textSize += static_cast<int>(characters.size());
    },
    []() {
        return;
    });

    parser.parse();


    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const double MLOCPerSecond = loc / elapsedSeconds / 1000000;
    int files = std::max(unitCount - 1, 1);
    std::cout.imbue(std::locale{""});
    int valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));

    // output Report
    std::cout << "# srcFacts: " << url << '\n';
    std::cout << "| Measure       | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:--------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Characters    | " << std::setw(valueWidth) << textSize         << " |\n";
    std::cout << "| LOC           | " << std::setw(valueWidth) << loc              << " |\n";
    std::cout << "| Files         | " << std::setw(valueWidth) << files            << " |\n";
    std::cout << "| Classes       | " << std::setw(valueWidth) << classCount       << " |\n";
    std::cout << "| Functions     | " << std::setw(valueWidth) << functionCount    << " |\n";
    std::cout << "| Declarations  | " << std::setw(valueWidth) << declCount        << " |\n";
    std::cout << "| Expressions   | " << std::setw(valueWidth) << exprCount        << " |\n";
    std::cout << "| Comments      | " << std::setw(valueWidth) << commentCount     << " |\n";
    std::cout << "| Returns       | " << std::setw(valueWidth) << returnCount      << " |\n";
    std::cout << "| Line Comments | " << std::setw(valueWidth) << lineCommentCount << " |\n";
    std::cout << "| Strings       | " << std::setw(valueWidth) << literalCount     << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}

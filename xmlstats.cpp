/*
    xmlstats.cpp
    markdown report with the number of each part of XML.
    E.g., the number of start tags, end tags, attributes, character sections, etc.
    
*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include "XMLParser.hpp"

int main(int argc, char* argv[]) {

    int startDocCount = 0;
    int XMLDeclarationCount = 0;
    int DOCTYPECount = 0;
    int CERCount = 0;
    int nonCERCount = 0;
    int commentCount = 0;
    int CDATACount = 0;
    int PICount = 0;
    int endTagCount = 0;
    int startTagCount = 0;
    int namespaceCount = 0;
    int attributeCount = 0;
    int endDocCount = 0;

    XMLParser parser(
    [&startDocCount]() {

        ++startDocCount;
    },
    [&XMLDeclarationCount](std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {

        ++XMLDeclarationCount;
    },
    [&DOCTYPECount]() {

        ++DOCTYPECount;
    },
    [&startTagCount](std::string_view qName, std::string_view prefix, std::string_view localName) {

        ++startTagCount;
    },
    [&endTagCount](std::string_view prefix, std::string_view qName, std::string_view localName) {

        ++endTagCount;
    },
    [&attributeCount](std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

        ++attributeCount;
    },
    [&namespaceCount](std::string_view prefix, std::string_view uri) {

        ++namespaceCount;
    },
    [&commentCount](std::string_view comment) {

        ++commentCount;
    },
    [&CDATACount](std::string_view characters) {

        ++CDATACount;
    },
    [&PICount](std::string_view target, std::string_view data) {

        ++PICount;
    },
    [&CERCount](std::string_view characters) {

        ++CERCount;
    },
    [&nonCERCount](std::string_view characters) {

       ++nonCERCount;
    },
    [&endDocCount]() {

        ++endDocCount;
    });

    parser.parse();

    int valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));

    // output xmlstats
    std::cout << "# XMLStates: " << '\n';
    std::cout << "| Measure                | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:-----------------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Start Document         | " << std::setw(valueWidth) << startDocCount << " |\n";
    std::cout << "| XML Declaration        | " << std::setw(valueWidth) << XMLDeclarationCount   << " |\n";
    std::cout << "| DOCTYPE                | " << std::setw(valueWidth) << DOCTYPECount          << " |\n";
    std::cout << "| Start Tags             | " << std::setw(valueWidth) << startTagCount         << " |\n";
    std::cout << "| End Tags               | " << std::setw(valueWidth) << endTagCount           << " |\n";
    std::cout << "| Attributes             | " << std::setw(valueWidth) << attributeCount        << " |\n";
    std::cout << "| XML Namespace          | " << std::setw(valueWidth) << namespaceCount        << " |\n";
    std::cout << "| XML Comments           | " << std::setw(valueWidth) << commentCount          << " |\n";
    std::cout << "| CDATA                  | " << std::setw(valueWidth) << CDATACount            << " |\n";
    std::cout << "| Processing Instruction | " << std::setw(valueWidth) << PICount               << " |\n";
    std::cout << "| CER                    | " << std::setw(valueWidth) << CERCount              << " |\n";
    std::cout << "| nonCER                 | " << std::setw(valueWidth) << nonCERCount           << " |\n";
    std::cout << "| End Document           | " << std::setw(valueWidth) << endDocCount           << " |\n";
    std::cout << "\n";
    return 0;
}

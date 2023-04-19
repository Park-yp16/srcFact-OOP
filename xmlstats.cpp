/*
    xmlstats.cpp

    Markdown report with the number of each part of XML.
    e.g., the number of start tags, end tags, attributes, character sections, etc.
*/

#include <iostream>
#include <iomanip>
#include <cmath>

#include "XMLParser.hpp"
#include "XMLStatsParser.hpp"

int main(int argc, char* argv[]) {

    XMLStatsParser parser;

    parser.parse();

    int valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));

    // output xmlstats
    std::cout << "# XMLStates: " << '\n';
    std::cout << "| Measure                | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:-----------------------|-" << std::setw(valueWidth + 3) << std::setfill('-')             << ":|\n" << std::setfill(' ');
    std::cout << "| Start Document         | " << std::setw(valueWidth) << parser.getStartDocCount()         << " |\n";
    std::cout << "| XML Declaration        | " << std::setw(valueWidth) << parser.getXMLDeclarationCount()   << " |\n";
    std::cout << "| DOCTYPE                | " << std::setw(valueWidth) << parser.getDOCTYPECount()          << " |\n";
    std::cout << "| Start Tags             | " << std::setw(valueWidth) << parser.getStartTagCount()         << " |\n";
    std::cout << "| End Tags               | " << std::setw(valueWidth) << parser.getEndTagCount()           << " |\n";
    std::cout << "| Attributes             | " << std::setw(valueWidth) << parser.getAttributeCount()        << " |\n";
    std::cout << "| XML Namespace          | " << std::setw(valueWidth) << parser.getNamespaceCount()        << " |\n";
    std::cout << "| XML Comments           | " << std::setw(valueWidth) << parser.getCommentCount()          << " |\n";
    std::cout << "| CDATA                  | " << std::setw(valueWidth) << parser.getCDATACount()            << " |\n";
    std::cout << "| Processing Instruction | " << std::setw(valueWidth) << parser.getPICount()               << " |\n";
    std::cout << "| CER                    | " << std::setw(valueWidth) << parser.getCERCount()              << " |\n";
    std::cout << "| nonCER                 | " << std::setw(valueWidth) << parser.getNonCERCount()           << " |\n";
    std::cout << "| End Document           | " << std::setw(valueWidth) << parser.getEndDocCount()           << " |\n";
    std::cout << "\n";
    return 0;
}

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
#include "srcFactsParser.hpp"

int main(int argc, char* argv[]) {

    const auto startTime = std::chrono::steady_clock::now();

    srcFactsParser handler;
    XMLParser parser(handler);

    parser.parse();

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const double MLOCPerSecond = handler.getLOC() / elapsedSeconds / 1000000;
    int files = std::max(handler.getUnitCount() - 1, 1);
    std::cout.imbue(std::locale{""});
    int valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));

    // output Report
    std::cout << "# srcFacts: " << handler.getURL() << '\n';
    std::cout << "| Measure       | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:--------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Characters    | " << std::setw(valueWidth) << handler.getTextsize()         << " |\n";
    std::cout << "| LOC           | " << std::setw(valueWidth) << handler.getLOC()              << " |\n";
    std::cout << "| Files         | " << std::setw(valueWidth) << files                         << " |\n";
    std::cout << "| Classes       | " << std::setw(valueWidth) << handler.getClassCount()       << " |\n";
    std::cout << "| Functions     | " << std::setw(valueWidth) << handler.getFunctionCount()    << " |\n";
    std::cout << "| Declarations  | " << std::setw(valueWidth) << handler.getDeclCount()        << " |\n";
    std::cout << "| Expressions   | " << std::setw(valueWidth) << handler.getExprCount()        << " |\n";
    std::cout << "| Comments      | " << std::setw(valueWidth) << handler.getCommentCount()     << " |\n";
    std::cout << "| Returns       | " << std::setw(valueWidth) << handler.getReturnCount()      << " |\n";
    std::cout << "| Line Comments | " << std::setw(valueWidth) << handler.getLineCommentCount() << " |\n";
    std::cout << "| Strings       | " << std::setw(valueWidth) << handler.getLiteralCount()     << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}

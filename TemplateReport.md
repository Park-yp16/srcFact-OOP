# Project 4 template Report: yp16

## XMLParser

Build:

```console
-- The C compiler identification is GNU 11.3.0
-- The CXX compiler identification is GNU 11.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /Build
[  5%] Building CXX object CMakeFiles/srcfacts.dir/srcFacts.cpp.o
[ 11%] Building CXX object CMakeFiles/srcfacts.dir/refillContent.cpp.o
[ 16%] Building CXX object CMakeFiles/srcfacts.dir/XMLParser.cpp.o
/Source/XMLParser.cpp: In member function 'void XMLParser::parseStartTag()':
/Source/XMLParser.cpp:470:10: warning: unused variable 'inEscape' [-Wunused-variable]
  470 |     bool inEscape = localName == "escape"sv;
      |          ^~~~~~~~
[ 22%] Building CXX object CMakeFiles/srcfacts.dir/xml_parser.cpp.o
[ 27%] Building CXX object CMakeFiles/srcfacts.dir/srcFactsParser.cpp.o
```

Run:

```console
[ 14%] Building CXX object CMakeFiles/srcfacts.dir/srcFactsParser.cpp.o
[100%] Linking CXX executable srcfacts
[100%] Built target srcfacts
[114%] Run demo
# srcFacts: 
| Measure       |   Value |
|:--------------|--------:|
| Characters    |   30853 |
| LOC           |     641 |
| Files         |       1 |
| Classes       |       0 |
| Functions     |       2 |
| Declarations  |      96 |
| Expressions   |     939 |
| Comments      |      35 |
| Returns       |      42 |
| Line Comments |      28 |
| Strings       |     224 |
[114%] Built target run
```

XMLParser virtual methods

```C++
virtual void handleStartDocument() {}
virtual void handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {}
virtual void handleDOCTYPE() {}
virtual void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {}
virtual void handleEndTag(std::string_view prefix, std::string_view qName, std::string_view localName) {}
virtual void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {}
virtual void handleNamespace(std::string_view prefix, std::string_view uri) {}
virtual void handleComment(std::string_view comment) {}
virtual void handleCDATA(std::string_view characters) {}
virtual void handleProcessingInstruction(std::string_view target, std::string_view data) {}
virtual void handleCharacterEntityReferences(std::string_view characters) {}
virtual void handleCharacterNonEntityReferences(std::string_view characters) {}
virtual void handleEndDocument() {}
```

srcFacts.cpp main program

```C++
int main(int argc, char* argv[]) {

    const auto startTime = std::chrono::steady_clock::now();

    srcFactsParser parser;

    parser.parse();

    const auto finishTime = std::chrono::steady_clock::now();
    const auto elapsedSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(finishTime - startTime).count();
    const double MLOCPerSecond = parser.getLOC() / elapsedSeconds / 1000000;
    int files = std::max(parser.getUnitCount() - 1, 1);
    std::cout.imbue(std::locale{""});
    int valueWidth = std::max(5, static_cast<int>(log10(parser.getTotalBytes()) * 1.3 + 1));

    // output Report
    std::cout << "# srcFacts: " << parser.getURL() << '\n';
    std::cout << "| Measure       | " << std::setw(valueWidth + 3) << "Value |\n";
    std::cout << "|:--------------|-" << std::setw(valueWidth + 3) << std::setfill('-') << ":|\n" << std::setfill(' ');
    std::cout << "| Characters    | " << std::setw(valueWidth) << parser.getTextsize()         << " |\n";
    std::cout << "| LOC           | " << std::setw(valueWidth) << parser.getLOC()              << " |\n";
    std::cout << "| Files         | " << std::setw(valueWidth) << files            << " |\n";
    std::cout << "| Classes       | " << std::setw(valueWidth) << parser.getClassCount()       << " |\n";
    std::cout << "| Functions     | " << std::setw(valueWidth) << parser.getFunctionCount()    << " |\n";
    std::cout << "| Declarations  | " << std::setw(valueWidth) << parser.getDeclCount()        << " |\n";
    std::cout << "| Expressions   | " << std::setw(valueWidth) << parser.getExprCount()        << " |\n";
    std::cout << "| Comments      | " << std::setw(valueWidth) << parser.getCommentCount()     << " |\n";
    std::cout << "| Returns       | " << std::setw(valueWidth) << parser.getReturnCount()      << " |\n";
    std::cout << "| Line Comments | " << std::setw(valueWidth) << parser.getLineCommentCount() << " |\n";
    std::cout << "| Strings       | " << std::setw(valueWidth) << parser.getLiteralCount()     << " |\n";
    std::clog.imbue(std::locale{""});
    std::clog.precision(3);
    std::clog << '\n';
    std::clog << parser.getTotalBytes()  << " bytes\n";
    std::clog << elapsedSeconds << " sec\n";
    std::clog << MLOCPerSecond << " MLOC/sec\n";

    return 0;
}
```

srcMLParser public methods

```C++
std::string getURL();
int getTextsize();
int getLOC();
int getExprCount();
int getFunctionCount();
int getClassCount();
int getUnitCount();
int getDeclCount();
int getCommentCount();
int getReturnCount();
int getLineCommentCount();
int getLiteralCount();
```

srcMLParser.hpp

```C++
/*
    srcFactsParser.hpp

    Include file for srcFacts parsing class
*/

#ifndef INCLUDED_SRCFACTSPARSER_HPP
#define INCLUDED_SRCFACTSPARSER_HPP

#include "XMLParser.hpp"

#include <string>

class srcFactsParser : public XMLParser {

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

    void handleEndTag(std::string_view prefix, std::string_view qName, std::string_view localName) override;

    void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) override;

    void handleNamespace(std::string_view prefix, std::string_view uri) override;

    void handleComment(std::string_view comment) override;

    void handleCDATA(std::string_view characters) override;

    void handleProcessingInstruction(std::string_view target, std::string_view data) override;

    void handleCharacterEntityReferences(std::string_view characters) override;

    void handleCharacterNonEntityReferences(std::string_view characters) override;

    void handleEndDocument() override;

    public:

    // Get method for URL
    std::string getURL();

    // Get method for textsize
    int getTextsize();

    // Get method for LOC
    int getLOC();

    // Get method for exprCount
    int getExprCount();

    // Get method for functionCount
    int getFunctionCount();

    // Get method for classCount
    int getClassCount();

    // Get method for unitCount
    int getUnitCount();

    // Get method for declCount
    int getDeclCount();

    // Get method for commentCount
    int getCommentCount();

    // Get method for returnCount
    int getReturnCount();

    // Get method for lineCommentCount
    int getLineCommentCount();

    // Get method for literalCount
    int getLiteralCount();
};

#endif

```

## Comment-each-section-of-code-in-main-program-of-template

Code is written in block or sections. Each of the major sections needs a comment describing it.
## Extra-whitespace-at-EOL

Remove the extra whitespace at the ends of lines:

```C++
17:••••
212:••••

```
## Function-declarations-missing-comments

All function declarations need a comment. The following are missing comments:

```C++
void handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) override;
void handleDOCTYPE() override;
void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;
void handleEndTag(std::string_view prefix, std::string_view qName, std::string_view localName) override;
void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) override;
void handleNamespace(std::string_view prefix, std::string_view uri) override;
void handleComment(std::string_view comment) override;
void handleCDATA(std::string_view characters) override;
void handleProcessingInstruction(std::string_view target, std::string_view data) override;
void handleCharacterEntityReferences(std::string_view characters) override;
void handleCharacterNonEntityReferences(std::string_view characters) override;
void handleEndDocument() override;

```
## Non-const-get-methods-for-class-srcFactsParser

The following get methods for class srcFactsParser are not `const`:

```C++
std::string getURL();
int getTextsize();
int getLOC();
int getExprCount();
int getFunctionCount();
int getClassCount();
int getUnitCount();
int getDeclCount();
int getCommentCount();
int getReturnCount();
int getLineCommentCount();
int getLiteralCount();

```

Without being const, they cannot be called on the parameter `parser` in the following function:

```C++
void process(const srcFactsParser& parser) {
    
}
```
## Unnecessary-include-files-in-srcFacts-for-template

The following standard include files are included in srcFacts.cpp. However, they are not needed there for template and are part of the internals of srcML parsing:

```C++
#include <iterator>
#include <string>
#include <sys/types.h>
#include <string_view>
#include <optional>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <bitset>
#include <cassert>

```


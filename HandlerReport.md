# Project 4 handler Report: yp16

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
/Source/XMLParser.cpp:471:10: warning: unused variable 'inEscape' [-Wunused-variable]
  471 |     bool inEscape = localName == "escape"sv;
      |          ^~~~~~~~
[ 22%] Building CXX object CMakeFiles/srcfacts.dir/xml_parser.cpp.o
[ 27%] Building CXX object CMakeFiles/srcfacts.dir/srcFactsParser.cpp.o
[ 33%] Linking CXX executable srcfacts
[ 33%] Built target srcfacts
[ 38%] Building CXX object CMakeFiles/xmlstats.dir/xmlstats.cpp.o
[ 44%] Building CXX object CMakeFiles/xmlstats.dir/refillContent.cpp.o
[ 50%] Building CXX object CMakeFiles/xmlstats.dir/XMLParser.cpp.o
[ 55%] Building CXX object CMakeFiles/xmlstats.dir/xml_parser.cpp.o
[ 61%] Building CXX object CMakeFiles/xmlstats.dir/XMLStatsParser.cpp.o
[ 66%] Linking CXX executable xmlstats
[ 66%] Built target xmlstats
[ 72%] Building CXX object CMakeFiles/identity.dir/identity.cpp.o
[ 77%] Building CXX object CMakeFiles/identity.dir/refillContent.cpp.o
[ 83%] Building CXX object CMakeFiles/identity.dir/XMLParser.cpp.o
[ 88%] Building CXX object CMakeFiles/identity.dir/xml_parser.cpp.o
[ 94%] Building CXX object CMakeFiles/identity.dir/identityParser.cpp.o
[100%] Linking CXX executable identity
[100%] Built target identity
```

Run:

```console
[ 85%] Built target srcfacts
[100%] Run demo
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
[100%] Built target run
```

XMLParserHandler

```C++
/*
    XMLParserHandler.hpp

    Include file for XML parsing handler class
*/

#ifndef XMLPARSERHANDLER_HPP
#define XMLPARSERHANDLER_HPP

#include <string_view>
#include <optional>

class XMLParserHandler {
    public:

    virtual void handleStartDocument() {};

    virtual void handleDeclaration(std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {};

    virtual void handleDOCTYPE() {};

    virtual void handleStartTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    virtual void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) {};

    virtual void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {};

    virtual void handleNamespace(std::string_view prefix, std::string_view uri) {};

    virtual void handleComment(std::string_view comment) {};

    virtual void handleCDATA(std::string_view characters) {};

    virtual void handleProcessingInstruction(std::string_view target, std::string_view data) {};

    virtual void handleCharacterEntityReferences(std::string_view characters) {};

    virtual void handleCharacterNonEntityReferences(std::string_view characters) {};

    virtual void handleEndDocument() {};
};

#endif
```

XMLParser.hpp

```C++
/*
    XMLParser.hpp

    Include file for xml parser functions
*/

#ifndef INCLUDED_XMLPARSER_HPP
#define INCLUDED_XMLPARSER_HPP

#include <string_view>
#include <functional>
#include <optional>

#include "XMLParserHandler.hpp"

class XMLParser {
    
    private: 

    std::string_view content;
    long totalBytes;
    bool doneReading;
    int depth;
    XMLParserHandler& handler;

    // check if declaration
    bool isXMLDeclaration();

    // check if DOCTYPE
    bool isDOCTYPE();

    // check if character entity references
    bool isCharacterEntityReferences();

    // check if character non-entity references
    bool isCharacterNonEntityReferences();

    // check if comment
    bool isXMLComment();

    // check if CDATA
    bool isCDATA();

    // check if processing instruction
    bool isProcessingInstruction();

    // check if end tag
    bool isEndTag();

    // check if start tag
    bool isStartTag();

    // check if namespace
    bool isXMLNamespace();

    // Start tracing document
    void startTracing();

    // check for file input
    void checkFIleInput();

    // parse XML declaration
    void parseXMLDeclaration();

    // parse DOCTYPE
    void parseDOCTYPE();

    // refill content preserving unprocessed
    void refillContentUnprocessed();

    // parse character entity references
    void parseCharacterEntityReferences();

    // parse character non-entity references
    void parseCharacterNonEntityReferences();

    // parse XML comment
    void parseXMLComment();

    // parse CDATA
    void parseCDATA();

    // parse processing instruction
    void parseProcessingInstruction();

    // parse end tag
    void parseEndTag();

    // parse start tag
    void parseStartTag();

    // parse XML namespace
    void parseXMLNamespace();

    // parse attribute
    void parseAttribute();

    // End tracing document
    void endTracing();

    public:

    // constructor
    XMLParser(XMLParserHandler& handler);

    virtual ~XMLParser() = default;
    
    long getTotalBytes();

    void parse();

};
#endif
```

srcFacts.cpp main program

```C++
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
```

srcFacts{Parser,Handler,ParserHandler}.hpp

```C++
/*
    srcFactsParser.hpp

    Include file for srcFacts parsing class
*/

#ifndef INCLUDED_SRCFACTSPARSER_HPP
#define INCLUDED_SRCFACTSPARSER_HPP

#include "XMLParserHandler.hpp"

#include <string>

class srcFactsParser : public XMLParserHandler {

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

    void handleEndTag(std::string_view qName, std::string_view prefix, std::string_view localName) override;

    void handleAttribute(std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) override;

    void handleNamespace(std::string_view prefix, std::string_view uri) override;

    void handleComment(std::string_view comment) override;

    void handleCDATA(std::string_view characters) override;

    void handleProcessingInstruction(std::string_view target, std::string_view data) override;

    void handleCharacterEntityReferences(std::string_view characters) override;

    void handleCharacterNonEntityReferences(std::string_view characters) override;

    void handleEndDocument() override;

    public:

    srcFactsParser();

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

## Function-definitions-missing-comments

All function definitions need a comment. The following are missing comments:

```C++
virtual void handleDeclaration() 
virtual void handleDOCTYPE() 
virtual void handleStartTag() 
virtual void handleEndTag() 
virtual void handleAttribute() 
virtual void handleNamespace() 
virtual void handleComment() 
virtual void handleCDATA() 
virtual void handleProcessingInstruction() 
virtual void handleCharacterEntityReferences() 
virtual void handleCharacterNonEntityReferences() 
virtual void handleEndDocument() 

```


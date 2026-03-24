/*
    identity.cpp

    An identity transformation of XML.
    The output XML is the same (as much as possible) as the input XML.

    There are no CDATA parts, but escape all >, <, and & in Character and CDATA content.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <optional>

#include "XMLParser.hpp"
#include "identityParser.hpp"

// [FIX] Merge conflict 해결: HEAD 브랜치의 빈 main()을 main 브랜치의 완성된 버전으로 교체
int main(int argc, char* argv[]) {

    identityParser handler;
    XMLParser parser(handler);

    parser.parse();

    return 0;
}

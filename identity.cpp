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

int main(int argc, char* argv[]) {

    identityParser handler;
    XMLParser parser(handler);
    
    parser.parse();

    return 0;
}

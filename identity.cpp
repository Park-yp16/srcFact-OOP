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

using namespace std::literals::string_view_literals;

int main() {

    std::ofstream demoCopy;

    XMLParser parser(
    [&]() {

        demoCopy.open("demoCopy.xml");
    },
    [&](std::string_view version, std::optional<std::string_view> encoding, std::optional<std::string_view> standalone) {

        demoCopy << "<?xml version=\"" << version << "\" encoding=\"" << encoding.value() << "\" standalone=\"" << standalone.value() << "\"?>\n";
    },
    [&]() {

        return;
    },
    [&](std::string_view qName, std::string_view prefix, std::string_view localName) {

        demoCopy << "<" << qName << ">";
    },
    [&](std::string_view qName, std::string_view prefix, std::string_view localName) {

        demoCopy << "</" << qName << ">";
    },
    [&](std::string_view qName, std::string_view prefix, std::string_view localName, std::string_view value) {

        demoCopy.seekp(-1, std::ios::cur);
        demoCopy << " " << qName << "=\"" << value << "\">";
    },
    [&](std::string_view prefix, std::string_view uri) {

        demoCopy.seekp(-1, std::ios::cur);
        if(prefix.empty())
            demoCopy << " xmlns=\"" << uri << "\">";
        else
            demoCopy << " xmlns:" << prefix << "=\"" << uri << "\">";
    },
    [&](std::string_view comment) {

        demoCopy << "<!--" << comment << "-->";
        // demoCopy << comment;
    },
    [&](std::string_view characters) {

        if(characters == "<") {
            demoCopy << "&lt;";
        } else if(characters == "&") {
            demoCopy << "&amp;";
        } else if(characters == ">") {
            demoCopy << "&gt;";
        } else {
            demoCopy << characters;
        }
    },
    [&](std::string_view target, std::string_view data) {

        std::cout << "<?" << target << " " << data << "?>";
    },
    [&](std::string_view characters) {

        if(characters == "<") {
            demoCopy << "&lt;";
        } else if(characters == "&") {
            demoCopy << "&amp;";
        } else if(characters == ">") {
            demoCopy << "&gt;";
        } else {
            demoCopy << characters;
        }
    },
    [&](std::string_view characters) {

        if(characters == "<") {
            demoCopy << "&lt;";
        } else if(characters == "&") {
            demoCopy << "&amp;";
        } else if(characters == ">") {
            demoCopy << "&gt;";
        } else {
            demoCopy << characters;
        }
    },
    [&]() {

        demoCopy << "\n";
        demoCopy.close();
    });

    parser.parse();

    return 0;
}

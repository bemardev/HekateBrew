/*
 * SimpleIniParser
 * Copyright (c) 2019 Nichole Mattera
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above 
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <iostream>
#include <fstream>
#include <switch.h>
#include <SimpleIniParser.hpp>

using namespace simpleIniParser;
using namespace std;

void writeOption(IniOption * option, bool withTab) {
    switch (option->type) {
        case IniOptionType::SemicolonComment:
            cout << ((withTab) ? "\t" : "") << "Type: Semicolon Comment, Value: \"" << option->value << "\"\n";
            break;

        case IniOptionType::HashtagComment:
            cout << ((withTab) ? "\t" : "") << "Type: Hashtag Comment, Value: \"" << option->value << "\"\n";
            break;

        default:
            cout << ((withTab) ? "\t" : "") << "Type: Option, Key: \"" << option->key << "\", Value: \"" << option->value << "\"\n";
            break;
    }
}

void writeSection(IniSection * section) {
    switch (section->type) {
        case IniSectionType::SemicolonComment:
            cout << "Type: Semicolon Comment, Value: \"" << section->value << "\"\n";
            break;

        case IniSectionType::HashtagComment:
            cout << "Type: Hashtag Comment, Value: \"" << section->value << "\"\n";
            break;

        case IniSectionType::HekateCaption:
            cout << "Type: Hekate Caption, Value: \"" << section->value << "\"\n";
            break;

        default:
            cout << "Type: Section, Value: \"" << section->value << "\"\n";
            break;
    }

    for (auto const& option : section->options) {
        writeOption(option, true);
    }

    cout << "\n";
}

int main(int argc, char **argv) {
    consoleInit(NULL);

    Result rc = romfsInit();
    if (R_FAILED(rc)) {
        cout << "Unable to initialize romfs.\n";
    }
    else {
        Ini * config = Ini::parseFile("romfs:/config.ini");

        cout << "Reading through an INI file.\n";
        cout << "=====================================================\n\n";


        for (auto const& option : config->options) {
            writeOption(option, false);
        }
        
        if (config->options.size() > 0)
            cout << "\n";

        for (auto const& section : config->sections) {
            writeSection(section);
        }

        cout << "\nGet a specific option from a specific section.\n";
        cout << "=====================================================\n\n";
        IniOption * option = config->findSection("config")->findFirstOption("cUsToMlOgO", false);
        cout << "Key: \"" << option->key << "\" | Value: \"" << option->value << "\"\n";

        IniOption * option2 = config->findSection("CFW", true, IniSectionType::Section)->findFirstOption("option comment test", false, IniOptionType::HashtagComment, IniOptionSearchField::Value);
        cout << "Key: \"" << option2->key << "\" | Value: \"" << option2->value << "\"\n\n";

        delete config;
    }

    cout << "\nPress any key to close.\n";

    while(appletMainLoop())
    {
        hidScanInput();
        if (hidKeysDown(CONTROLLER_P1_AUTO))
            break;

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
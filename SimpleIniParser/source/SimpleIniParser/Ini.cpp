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

#include <algorithm> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <switch.h>

#include "Ini.hpp"
#include "IniOption.hpp"
#include "IniStringHelper.hpp"

using namespace std;

namespace simpleIniParser {
    Ini::~Ini() {
        magic = "";

        for (IniSection * section : sections) {
            if (section != nullptr) {
                delete section;
                section = nullptr;
            }
        }
        sections.clear();
    }

    string Ini::build() {
        string result;

        if (magic != "") {
            result += magic + "\n";
        }

        for (auto const& option : options) {
            result += option->build();
        }

        for (auto const& section : sections) {
            result += section->build();
        }

        return result;
    }

    IniOption * Ini::findFirstOption(string term, bool caseSensitive, IniOptionType type, IniOptionSearchField field) {
        if (!caseSensitive) {
            IniStringHelper::toupper(term);
        }

        auto it = find_if(options.begin(), options.end(), [&term, &caseSensitive, &type, &field](const IniOption * obj) {
            if (type != IniOptionType::Any && type != obj->type) {
                return false;
            }

            string fieldValue = "";
            if (field == IniOptionSearchField::Key) {
                fieldValue = (!caseSensitive) ? IniStringHelper::toupper_copy(obj->key) : obj->key;
            } else {
                fieldValue = (!caseSensitive) ? IniStringHelper::toupper_copy(obj->value) : obj->value;
            }

            return fieldValue == term;
        });

        if (it == options.end())
            return nullptr;

        return (*it);
    }

    IniSection * Ini::findSection(string term, bool caseSensitive, IniSectionType type) {
        if (!caseSensitive) {
            IniStringHelper::toupper(term);
        }

        auto it = find_if(sections.begin(), sections.end(), [&term, &caseSensitive, &type](const IniSection * obj) {
            if (type != IniSectionType::Any && type != obj->type) {
                return false;
            }

            string fieldValue = (!caseSensitive) ? IniStringHelper::toupper_copy(obj->value) : obj->value;
            return fieldValue == term;
        });

        if (it == sections.end())
            return nullptr;

        return (*it);
    }

    bool Ini::writeToFile(string path) {
        ofstream file(path);
        if (!file.is_open())
            return false;

        file << build();

        file.flush();
        file.close();

        fsdevCommitDevice("sdmc");

        return true;
    }

    Ini * Ini::parseFile(string path) {
        ifstream file(path);
        if (!file.is_open())
            return nullptr;

        stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        return _parseContent(&buffer, "");
    }
    
    Ini * Ini::parseFileWithMagic(string path, string magic) {
        ifstream file(path);
        if (!file.is_open())
            return nullptr;

        stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        string line;
        getline(buffer, line);
        IniStringHelper::trim(line);
        if (line != magic) {
            return nullptr;
        }
        
        return _parseContent(&buffer, magic);
    }

    Ini * Ini::_parseContent(stringstream * content, string magic) {
        Ini * ini = new Ini();
        ini->magic = magic;
        string line;
        while (getline(* content, line)) {
            IniStringHelper::trim(line);

            if (line.size() == 0)
                continue;

            bool shouldParseCommentsAsSection = ini->sections.size() != 0 && ini->sections.back()->type != IniSectionType::Section;
            IniSection * section = IniSection::parse(line, shouldParseCommentsAsSection);
            if (section != nullptr) {
                ini->sections.push_back(section);
            } else {
                IniOption * option = IniOption::parse(line);

                if (option != nullptr && ini->sections.size() == 0) {
                    ini->options.push_back(option);
                } else if (option != nullptr && ini->sections.size() != 0 && ini->sections.back()->type == IniSectionType::Section) {
                    ini->sections.back()->options.push_back(option);
                }
            }
        }

        return ini;
    }
}

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
#include <iostream>

#include "IniSection.hpp"
#include "IniStringHelper.hpp"

using namespace std;

namespace simpleIniParser {
    IniSection::IniSection(IniSectionType t, string v) {
        type = t;
        value = v;
    }

    IniSection::~IniSection() {
        for (IniOption * option : options) {
            if (option != nullptr) {
                delete option;
                option = nullptr;
            }
        }
        options.clear();
    }

    IniOption * IniSection::findFirstOption(string term, bool caseSensitive, IniOptionType type, IniOptionSearchField field) {
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

    string IniSection::build() {
        switch (type) {
            case IniSectionType::HekateCaption:
                return "\n{" + value + "}\n";

            case IniSectionType::SemicolonComment:
                return "\n; " + value + "\n";

            case IniSectionType::HashtagComment:
                return "\n# " + value + "\n";

            default:
                string result = "\n[" + value + "]\n";

                for (auto const& option : options) {
                    result += option->build();
                }

                return result;
        }
    }

    IniSection * IniSection::parse(string line, bool parseComments) {
        if (line.at(0) == '{' && line.at(line.size() - 1) == '}') {
            return new IniSection(IniSectionType::HekateCaption, IniStringHelper::trim_copy(line.substr(1, line.size() - 2)));
        } else if (parseComments && line.at(0) == ';') {
            return new IniSection(IniSectionType::SemicolonComment, IniStringHelper::trim_copy(line.substr(1, line.size() - 1)));
        } else if (parseComments && line.at(0) == '#') {
            return new IniSection(IniSectionType::HashtagComment, IniStringHelper::trim_copy(line.substr(1, line.size() - 1)));
        } else if (line.at(0) == '[' && line.at(line.size() - 1) == ']') {
            return new IniSection(IniSectionType::Section, IniStringHelper::trim_copy(line.substr(1, line.size() - 2)));
        } else {
            return nullptr;
        }
    }
}

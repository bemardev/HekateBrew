#pragma once

#include <string>

namespace simpleIniParser {
    class IniStringHelper {
        public:
            static void toupper(std::string &s);
            static std::string toupper_copy(std::string s);

            // Start of Source from https://stackoverflow.com/questions/216823
            static void ltrim(std::string &s);
            static void rtrim(std::string &s);
            static void trim(std::string &s);
            static std::string ltrim_copy(std::string s);
            static std::string rtrim_copy(std::string s);
            static std::string trim_copy(std::string s);
            // End
    };
}

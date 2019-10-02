

#include <algorithm> 
#include <cctype>
#include <locale>

#include "IniStringHelper.hpp"

using namespace std;

namespace simpleIniParser {
    void IniStringHelper::toupper(string &s) {
        for_each(s.begin(), s.end(), [](char & c){
            c = ::toupper(c);
        });
    }

    string IniStringHelper::toupper_copy(string s) {
        toupper(s);
        return s;
    }

    // Start of Source from https://stackoverflow.com/questions/216823
    void IniStringHelper::ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
            return !isspace(ch);
        }));
    }

    void IniStringHelper::rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), s.end());
    }

    void IniStringHelper::trim(string &s) {
        ltrim(s);
        rtrim(s);
    }

    string IniStringHelper::ltrim_copy(string s) {
        ltrim(s);
        return s;
    }

    string IniStringHelper::rtrim_copy(string s) {
        rtrim(s);
        return s;
    }

    string IniStringHelper::trim_copy(string s) {
        trim(s);
        return s;
    }
    // End
}



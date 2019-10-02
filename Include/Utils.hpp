/*
 * Copyright (C) 2019 bemar
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#pragma once
#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <dirent.h>
#include <sys/types.h>

namespace fs = std::filesystem;

// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

// ends with suffix
static inline bool endsWith(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

// begins with suffix
static inline bool startsWith(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

// replace occurence in string
inline void find_and_replace(std::string& file_contents,
        const std::string& before, const std::string& after)
{
    // This searches the file for the first occurence of the morn string.
    auto pos = file_contents.find(before);
    while (pos != std::string::npos) {
        file_contents.replace(pos, before.length() + 1, after);
        // Continue searching from here.
        pos = file_contents.find(before, pos);
    }
}

// remove double slash
inline void removeDblSlash(std::string& file_contents)
{
    file_contents.erase(std::unique(file_contents.begin(), file_contents.end(), [](char a, char b){return a == '/' && b == '/';}), file_contents.end());
}

// shift string
inline void stringShift(std::string& str)
{
    if (str.length() > 0) {
        std::string tmp = str;
        str.erase(0, 1);
        str += tmp[0];
    }
}

// fs is directory
inline bool isDir(const std::string& pathString = std::string())
{
    const fs::path path(pathString);
    std::error_code ec;
    if (fs::is_directory(path, ec)) {
        return true;
    }
    if (ec) // handle error
    {
    }
    return false;
}

// fs is file
inline bool isFile(const std::string& pathString = std::string())
{
    const fs::path path(pathString);
    std::error_code ec;
    if (fs::is_regular_file(path, ec)) {
        return true;
    }
    if (ec) // handle error
    {
    }
    return false;
}

// fs remove file
inline bool removeFile(const std::string& pathString)
{
    std::error_code ec;
    const fs::path path(pathString);
    if (std::remove(pathString.c_str()) == 0)
        return true;
    return false;
}

// fs rename file
inline bool renameFile(const std::string& pathOrigString, const std::string& pathDestString)
{
    if (pathOrigString != "" && pathDestString != "") {
        if (std::rename(pathOrigString.c_str(), pathDestString.c_str()) == 0)
            return true;
    }
    return false;
}

// stream copy file
inline bool copyFile(const std::string& pathOrigString, const std::string& pathDestString)
{
    std::ifstream src(pathOrigString, std::ios::binary);
    std::ofstream dst(pathDestString, std::ios::binary);
    dst << src.rdbuf();
    if (src.tellg() != dst.tellp()) {
        removeFile(pathDestString);
    } else {
        return true;
    }
    return false;
}

// fs create dir
inline bool create_directory(const std::string& pathString)
{
    if (!pathString.empty()) {
        const fs::path path(pathString);
        return fs::create_directory(path);
    } else
        return false;
}

// fs read dir ascii order
inline std::vector<std::string> read_directory(const std::string& pathString = std::string())
{
    std::vector <std::string> result;
    for (const auto & entry : fs::directory_iterator(pathString)) {
        result.push_back(entry.path());
    }
    std::sort(result.begin(), result.end());
    return result;
}

inline std::vector<std::string> listSubDirectories(const std::string& pathString)
{
    std::vector<std::string> result;
    DIR * dir;
    struct dirent* entry;
    dir = opendir(pathString.c_str());
    if(dir != NULL)
    {
        while((entry = readdir(dir)))
        {
            if(entry->d_type == DT_DIR)
                result.push_back(entry->d_name);
        }
    }
    closedir(dir);
    std::sort(result.begin(), result.end());
    return result;
}
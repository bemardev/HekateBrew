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
#include <Utils.hpp>
#include <Types.hpp>
#include <SimpleIniParser.hpp>
#include <vector>
#include <string>

using namespace simpleIniParser;

static inline bool LoadHekateBrewConfig(HekateBrewConfig &config)
{
    std::string hekateBrewDir = "sdmc:/switch/HekateBrew/";
    if (!isDir(hekateBrewDir))
        return create_directory(hekateBrewDir);
    return true;
}

static inline void LoadHekateConfig(HekateConfig &config)
{
    std::string hekateFile = "sdmc:/bootloader/hekate_ipl.ini";
    if (isFile(hekateFile)) {
        Ini *hkIni = Ini::parseFile(hekateFile);
        IniOption * option = hkIni->findSection("config")->findFirstOption("autoboot", false);
        if (option != nullptr)
            config.autoboot = std::atoi(option->value.c_str());
        else
            config.autoboot = -1;
        option = hkIni->findSection("config")->findFirstOption("autoboot", false);
        if (option != nullptr)
            config.autoboot_list = std::atoi(option->value.c_str());
        else
            config.autoboot_list = -1;
    }
}

static inline void LoadIniConfig(std::vector<LauncherItem> &configItems, std::string fileName, int listIndex)
{
    Ini *hkIni = Ini::parseFile(fileName);
    int configIndex = 1;
    for (auto const& section : hkIni->sections) {
        if (section->type != IniSectionType::Section)
            continue;
        if (section->value != "config") {
            std::string image = "";
            auto iconPath = section->findFirstOption("icon", false);
            if (iconPath != nullptr)
                image = "sdmc:/" + iconPath->value;
            configItems.push_back({section->value, image, configIndex, listIndex});
            configIndex += 1;
        }
    }
}

static inline void LoadHekateInfos(std::vector<LauncherItem> &configItems, std::vector<LauncherItem> &moreconfigItems, std::vector<PayloadItem> &payloadItems)
{
    std::string hekateFile = "sdmc:/bootloader/hekate_ipl.ini";
    std::string hekateIniDir = "sdmc:/bootloader/ini/";
    std::string hekatePayloadDir = "sdmc:/bootloader/payloads/";
    if (isFile(hekateFile)) {
        LoadIniConfig(configItems, hekateFile, 0);
    }
    if (isDir(hekateIniDir)) {
        std::vector<std::string> files = read_directory(hekateIniDir);
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
            LoadIniConfig(moreconfigItems, *it, 1);
        }
    }
    if (isDir(hekatePayloadDir)) {
        std::vector<std::string> files = read_directory(hekatePayloadDir);
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
            if (endsWith(*it, ".bin")) {
                std::string image = "";
                std::string currentFile = (*it).substr((*it).find_last_of("\\/") + 1, (*it).length()-(*it).find_last_of("\\/") - 5);
                if (isFile(hekatePayloadDir + currentFile + ".bmp"))
                    image = hekatePayloadDir + currentFile + ".bmp";
                else if (isFile(hekatePayloadDir + currentFile + ".png"))
                    image = hekatePayloadDir + currentFile + ".png";
                payloadItems.push_back({currentFile, (*it), image});
            }
        }
    }
}

static inline void LoadArgonInfos(std::vector<PayloadItem> &payloadItems)
{
    std::string argonDir = "sdmc:/argon/";
    std::string argonPayloadDir = "sdmc:/argon/payloads/";
    std::string argonLogoDir = "sdmc:/argon/logos/";
    if (isDir(argonDir) && isDir(argonPayloadDir)) {
        std::vector<std::string> files = read_directory(argonPayloadDir);
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
            if (endsWith(*it, ".bin")) {
                std::string image = "";
                std::string currentFile = (*it).substr((*it).find_last_of("\\/") + 1, (*it).length()-(*it).find_last_of("\\/") - 5);
                if (isFile(argonLogoDir + currentFile + ".bmp"))
                    image = argonLogoDir + currentFile + ".bmp";
                payloadItems.push_back({currentFile, (*it), image});
            }
        }
    }
}


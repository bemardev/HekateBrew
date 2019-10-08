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

const std::string hekateBrewDir = "sdmc:/switch/HekateBrew/";
const std::string hekateBrewFile = "sdmc:/switch/HekateBrew/settings.ini";
const std::string hekateFile = "sdmc:/bootloader/hekate_ipl.ini";
const std::string hekateIniDir = "sdmc:/bootloader/ini/";
const std::string hekatePayloadDir = "sdmc:/bootloader/payloads/";
const std::string argonDir = "sdmc:/argon/";
const std::string argonPayloadDir = "sdmc:/argon/payloads/";
const std::string argonLogoDir = "sdmc:/argon/logos/";
const std::string rootPayloadDir = "sdmc:/payloads/";

static inline void CreateDefaultHekateBrewConfig()
{
    simpleIniParser::Ini * hbIni = new simpleIniParser::Ini();
    simpleIniParser::IniSection * configSection = new simpleIniParser::IniSection(simpleIniParser::IniSectionType::Section, "config");
    configSection->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "showhekate", "0"));
    configSection->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "showargon", "0"));
    configSection->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "showrootdir", "0"));
    configSection->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "showcustompath", "0"));
    configSection->options.push_back(new simpleIniParser::IniOption(simpleIniParser::IniOptionType::Option, "custompath", ""));
    hbIni->sections.push_back(configSection);
    hbIni->writeToFile(hekateBrewFile);
    delete hbIni;
}

static inline bool SaveHekateBrewConfig(HekateBrewConfig config)
{
    if(isFile(hekateBrewFile))
    {
        simpleIniParser::Ini *hbIni = simpleIniParser::Ini::parseFile(hekateBrewFile);
        hbIni->findSection("config")->findFirstOption("showhekate")->value = config.showHekate;
        hbIni->findSection("config")->findFirstOption("showargon")->value = config.showArgon;
        hbIni->findSection("config")->findFirstOption("showrootdir")->value = config.showRootDir;
        hbIni->findSection("config")->findFirstOption("showcustompath")->value = config.showCustomPath;
        hbIni->findSection("config")->findFirstOption("custompath")->value = config.customPath;
        hbIni->writeToFile(hekateBrewFile);
        delete hbIni;
        return true;
    }
    return false;
}

static inline bool SaveHekateConfig(HekateConfig config)
{
    if (isFile(hekateFile)) {
        simpleIniParser::Ini *hkIni = simpleIniParser::Ini::parseFile(hekateFile);
        hkIni->findSection("config")->findFirstOption("autoboot")->value = config.autoboot;
        hkIni->findSection("config")->findFirstOption("autoboot_list")->value = config.autoboot_list;
        hkIni->findSection("config")->findFirstOption("bootwait")->value = config.bootwait;
        hkIni->findSection("config")->findFirstOption("verification")->value = config.verification;
        hkIni->findSection("config")->findFirstOption("backlight")->value = config.backlight;
        hkIni->findSection("config")->findFirstOption("autohosoff")->value = config.autohosoff;
        hkIni->findSection("config")->findFirstOption("autonogc")->value = config.autonogc;
        hkIni->writeToFile(hekateFile);
        delete hkIni;
        return true;
    }
    else
        return false;
}

static inline void LoadHekateBrewConfig(HekateBrewConfig &config)
{
    bool hasHkbDir = isDir(hekateBrewDir);
    if (!hasHkbDir)
        hasHkbDir = create_directory(hekateBrewDir);
    if(!isFile(hekateBrewFile))
        CreateDefaultHekateBrewConfig();
    
    simpleIniParser::Ini *hbIni = simpleIniParser::Ini::parseFile(hekateBrewFile);
    simpleIniParser::IniOption * option = hbIni->findSection("config")->findFirstOption("showhekate", false);
    if (option != nullptr)
        config.showHekate = option->value.c_str();
    else
        config.showHekate = "1";
    option = hbIni->findSection("config")->findFirstOption("showargon", false);
    if (option != nullptr)
        config.showArgon = option->value.c_str();
    else
        config.showArgon = "1";
    option = hbIni->findSection("config")->findFirstOption("showrootdir", false);
    if (option != nullptr)
        config.showRootDir = option->value.c_str();
    else
        config.showRootDir = "1";
    option = hbIni->findSection("config")->findFirstOption("showcustompath", false);
    if (option != nullptr)
        config.showCustomPath = option->value.c_str();
    else
        config.showCustomPath = "1";
    option = hbIni->findSection("config")->findFirstOption("custompath", false);
    if (option != nullptr)
        config.customPath = option->value.c_str();
    else
        config.customPath = std::string();
    config.hasHekate = isFile(hekateFile);
    config.hasArgon = isDir(argonDir);
    config.path = hekateBrewDir;
    delete hbIni;
}

static inline void LoadHekateConfig(HekateConfig &config)
{
    if (isFile(hekateFile)) {
        simpleIniParser::Ini *hkIni = simpleIniParser::Ini::parseFile(hekateFile);
        simpleIniParser::IniOption * option = hkIni->findSection("config")->findFirstOption("autoboot", false);
        if (option != nullptr)
            config.autoboot = option->value.c_str();
        else
            config.autoboot = "0";
        option = hkIni->findSection("config")->findFirstOption("autoboot", false);
        if (option != nullptr)
            config.autoboot_list = option->value.c_str();
        else
            config.autoboot_list = "0";
        option = hkIni->findSection("config")->findFirstOption("bootwait", false);
        if (option != nullptr)
            config.bootwait = option->value.c_str();
        else
            config.bootwait = "1";
        option = hkIni->findSection("config")->findFirstOption("verification", false);
        if (option != nullptr)
            config.verification = option->value.c_str();
        else
            config.verification = "1";
        option = hkIni->findSection("config")->findFirstOption("backlight", false);
        if (option != nullptr)
            config.backlight = option->value.c_str();
        else
            config.backlight = "100";
        option = hkIni->findSection("config")->findFirstOption("autohosoff", false);
        if (option != nullptr)
            config.autohosoff = option->value.c_str();
        else
            config.autohosoff = "0";
        option = hkIni->findSection("config")->findFirstOption("autonogc", false);
        if (option != nullptr)
            config.autonogc = option->value.c_str();
        else
            config.autonogc = "1";
        delete hkIni;
    }
}

static inline void LoadHekateInfos(std::vector<LauncherItem> &configItems)
{
    if (isFile(hekateFile)) {
        simpleIniParser::Ini *hkIni = simpleIniParser::Ini::parseFile(hekateFile);
        int configIndex = 1;
        for (auto const& section : hkIni->sections) {
            if (section->type != simpleIniParser::IniSectionType::Section)
                continue;
            if (section->value != "config") {
                std::string image = "";
                auto iconPath = section->findFirstOption("icon", false);
                if (iconPath != nullptr)
                    image = "sdmc:/" + iconPath->value;
                configItems.push_back({section->value, image, std::to_string(configIndex), "0"});
                configIndex += 1;
            }
        }
        delete hkIni;
    }
    if (isDir(hekateIniDir)) {
        int configIndex = 1;
        std::vector<std::string> files = read_directory(hekateIniDir);
        for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
            simpleIniParser::Ini *hkIni = simpleIniParser::Ini::parseFile(*it);
            for (auto const& section : hkIni->sections) {
                if (section->type != simpleIniParser::IniSectionType::Section)
                    continue;
                if (section->value != "config") {
                    std::string image = "";
                    auto iconPath = section->findFirstOption("icon", false);
                    if (iconPath != nullptr)
                        image = "sdmc:/" + iconPath->value;
                    configItems.push_back({section->value, image, std::to_string(configIndex), "1"});
                    configIndex += 1;
                }
            }
            delete hkIni;
        }
    }
}

static inline void LoadPayloadsInfos(std::vector<PayloadItem> &payloadItems, std::string payloadPath, std::string logoPath)
{
    if(isDir(payloadPath) && isDir(logoPath))
    {
        std::vector<std::string> files = read_directory(payloadPath);
        for(auto& file : files)
        {
            if(endsWith(file, ".bin"))
            {
                std::string image = "";
                std::string currentFile = (file).substr((file).find_last_of("\\/") + 1, (file).length()-(file).find_last_of("\\/") - 5);
                if (isFile(logoPath + R"(/)" + currentFile + ".bmp"))
                    image = logoPath + R"(/)" + currentFile + ".bmp";
                else if (isFile(logoPath + R"(/)" + currentFile + ".png"))
                    image = logoPath + R"(/)" + currentFile + ".png";
                payloadItems.push_back({currentFile, file, image});
            }
        }
    }
}

static inline void LoadAllPayloads(std::vector<PayloadItem> &payloadItems, HekateBrewConfig config)
{
    if(config.hasHekate && config.showHekate == "1")
        LoadPayloadsInfos(payloadItems, hekatePayloadDir, hekatePayloadDir);
    if(config.hasArgon && config.showArgon == "1")
        LoadPayloadsInfos(payloadItems, argonPayloadDir, argonLogoDir);
    if(config.showRootDir == "1")
        LoadPayloadsInfos(payloadItems, rootPayloadDir, rootPayloadDir);
    if(config.showCustomPath  == "1" && config.customPath != std::string())
        LoadPayloadsInfos(payloadItems, config.customPath, config.customPath);
}
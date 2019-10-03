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
#include <string>
#include <switch.h>
#include <pu/Plutonium>
#include <IconTypes.hpp>

struct FileInfo
{
    std::string name;
    std::string pathName;
    std::string previousPath;
};
struct ini_info
{
    std::string name;
    std::string value;
    std::string link;
    std::string type;
};
struct HekateConfig
{
    int autoboot;
    int autoboot_list;
    int bootwait;
    int verification;
    int autohosoff;
    int autonogc;
    int backlight;
};
struct HekateBrewConfig
{
    std::string path;
    std::string theme;
    bool showHekate;
    bool showArgon;
    bool showRootDir;
    bool showCustomPath;
    std::string customPath;
    bool hasHekate;
    bool hasArgon;
};
struct ColorScheme
{
    pu::ui::Color Background; // background layout color
    pu::ui::Color Base; // background imagebutton color
    pu::ui::Color BaseFocus; // focus color
    pu::ui::Color Text; // menu text color
    pu::ui::Color GridText; // grid text
    pu::ui::Color GridBord; // grid border color
    pu::ui::Color GridAlt; // grid alternate border color
    pu::ui::Color GridInner; // grid inner border color
    pu::ui::Color LineSep;
    std::string configMenuImage;
    std::string moreconfigMenuImage;
    std::string payloadMenuImage;
    std::string defaultImage;
    std::string warnImage;
    std::string loaderImage;
};
struct LauncherItem
{
    std::string entryName;
    std::string entryImage;
    int entryIndex;
    int entryInList;
};
struct PayloadItem
{
    std::string payloadName;
    std::string payloadPath;
    std::string payloadImage;
};
enum class LaunchMode
{
    Unknown,
    Applet,
    Application
};

LaunchMode GetLaunchMode();

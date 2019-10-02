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
#include <switch.h>
#include <Types.hpp>
#include <ui_Utils.hpp>
#include <vector>

namespace set
{
    struct Settings
    {
        ColorScheme CustomScheme;
        HekateConfig hConfig;
        HekateBrewConfig hbConfig;
        bool needSave;
        int blinkDelay;
        std::vector<LauncherItem> configItems;
        std::vector<LauncherItem> moreconfigItems;
        std::vector<PayloadItem> hekatePayloads;
        std::vector<PayloadItem> argonPayloads;
    };

    Settings ProcessSettings();
    
    void SaveSettings(HekateBrewConfig &config, HekateConfig &hconfig);
}

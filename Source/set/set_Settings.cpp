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
#include <set/set_Settings.hpp>
#include <ui/ui_MainApplication.hpp>

namespace set
{

    Settings ProcessSettings()
    {
        Settings gset;

        LoadHekateBrewConfig(gset.hbConfig);
        
        // Blink delay for borders
        gset.blinkDelay = 500;

        // Theme defined
        ColorSetId csid = ColorSetId_Light;
        setsysGetColorSetId(&csid);
        if (csid == ColorSetId_Dark)
            gset.CustomScheme = ui::DefaultDark;
        else
            gset.CustomScheme = ui::DefaultLight;

        LoadHekateConfig(gset.hConfig);
        LoadHekateInfos(gset.hekateItems);
        LoadAllPayloads(gset.payloadItems, gset.hbConfig);

        return gset;
    }
    
    void ReloadList(Settings &gset)
    {
        gset.hekateItems.clear();
        LoadHekateInfos(gset.hekateItems);
        gset.payloadItems.clear();
        LoadAllPayloads(gset.payloadItems, gset.hbConfig);
    }
    
    void SaveSettings(Settings &gset)
    {
        SaveHekateBrewConfig(gset.hbConfig);
        SaveHekateConfig(gset.hConfig);
        
    }
}

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
#include <cfw/cfw_Helper.hpp>

namespace cfw
{

    bool IsAtmosphere()
    {
        u64 tmpc = 0;
        return R_SUCCEEDED(splGetConfig((SplConfigItem) 65000, &tmpc));
    }

    bool IsReiNX()
    {
        Handle tmph = 0;
        Result rc = smRegisterService(&tmph, "rnx", false, 1);
        if (R_FAILED(rc)) return true;
        smUnregisterService("rnx");
        return false;
    }

    bool IsSXOS()
    {
        Handle tmph = 0;
        Result rc = smRegisterService(&tmph, "tx", false, 1);
        if (R_FAILED(rc)) return true;
        smUnregisterService("tx");
        return false;
    }
}
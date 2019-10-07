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

#define PATCHED_RELOC_SZ 0x94
#define IRAM_PAYLOAD_MAX_SIZE 0x2F000
#define IRAM_PAYLOAD_BASE 0x40010000

#define BOOT_CFG_AUTOBOOT_EN (1 << 0)
#define BOOT_CFG_FROM_LAUNCH (1 << 1)
#define BOOT_CFG_SEPT_RUN    (1 << 7)

typedef struct __attribute__ ((__packed__)) _boot_cfg_t
{
    unsigned char boot_cfg;
    unsigned char autoboot;
    unsigned char autoboot_list;
    unsigned char extra_cfg;
    union
    {
        struct
        {
            char id[8];
        };
        unsigned char xt_str[0x80];
    };
}
boot_cfg_t;

namespace PayloadReboot
{

    bool Init(std::string payloadPath);
    void Reboot();
    void RebootRCM();
    void RebootHekate(std::string hbPath);
    void Shutdown();
    bool AlterPayload(std::string autoboot, std::string autobootl, std::string hbPath, bool launch = false, bool nocfg = false);
}
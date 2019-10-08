/*
 * Copyright (c) 2018-2019 Atmosphère-NX
 * Copyright (C) 2019 bemar
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdbool>
#include <switch.h>
#include <unistd.h>
#include <PayloadReboot.hpp>
#include <Utils.hpp>

static u8 g_reboot_payload[IRAM_PAYLOAD_MAX_SIZE] alignas(0x1000);
static u8 g_ff_page[0x1000] alignas(0x1000);
static u8 g_work_page[0x1000] alignas(0x1000);
boot_cfg_t *b_cfg = NULL;

void do_iram_dram_copy(void *buf, uintptr_t iram_addr, size_t size, int option)
{
    std::memcpy(g_work_page, buf, size);

    SecmonArgs args = {0};
    args.X[0] = 0xF0000201; /* smcAmsIramCopy */
    args.X[1] = (uintptr_t) g_work_page; /* DRAM Address */
    args.X[2] = iram_addr; /* IRAM Address */
    args.X[3] = size; /* Copy size */
    args.X[4] = option; /* 0 = Read, 1 = Write */
    svcCallSecureMonitor(&args);

    std::memcpy(buf, g_work_page, size);
}

void copy_to_iram(uintptr_t iram_addr, void *buf, size_t size)
{
    do_iram_dram_copy(buf, iram_addr, size, 1);
}

void copy_from_iram(void *buf, uintptr_t iram_addr, size_t size)
{
    do_iram_dram_copy(buf, iram_addr, size, 0);
}

static void clear_iram(void)
{
    std::memset(g_ff_page, 0xFF, sizeof (g_ff_page));
    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += sizeof (g_ff_page))
    {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, g_ff_page, sizeof (g_ff_page));
    }
}

void PayloadReboot::Reboot()
{

    splInitialize();
    clear_iram();

    for (size_t i = 0; i < IRAM_PAYLOAD_MAX_SIZE; i += 0x1000)
    {
        copy_to_iram(IRAM_PAYLOAD_BASE + i, &g_reboot_payload[i], 0x1000);
    }

    splSetConfig((SplConfigItem) 65001, 2);
    splExit();
}

void PayloadReboot::RebootRCM()
{
    splInitialize();
    clear_iram();

    splSetConfig((SplConfigItem) 65001, 1);
    splExit();
}

void PayloadReboot::RebootHekate(std::string hbPath)
{
    PayloadReboot::AlterPayload("0", "0", hbPath, true, true);
}

void PayloadReboot::Shutdown()
{
    splInitialize();
    clear_iram();

    splSetConfig((SplConfigItem) 65002, 1);
    splExit();
}

bool PayloadReboot::Init(std::string payloadPath)
{

    splInitialize();
    FILE *f = std::fopen(payloadPath.c_str(), "rb");
    if (!f)
        return false;
    std::fread(g_reboot_payload, sizeof (g_reboot_payload), 1, f);
    std::fclose(f);

    return true;
}

bool PayloadReboot::AlterPayload(std::string autoboot, std::string autobootl, std::string hbPath, bool launch, bool nocfg)
{
    std::string binFile = hbPath + "payload.bin";
    std::string bakFile = hbPath + "payload.bak";
    /*if (isFile(bakFile))
    {
        if(isFile(binFile))
            removeFile(binFile);
        //renameFile(bakFile, binFile);
        if(!copyFile(bakFile, binFile))
            return false;
    }*/
    if (isFile(binFile))
    {
        //if (copyFile(binFile, bakFile))
        //{
        b_cfg = new boot_cfg_t;
        b_cfg->boot_cfg = (nocfg) ? 0 : BOOT_CFG_AUTOBOOT_EN;
        b_cfg->autoboot = std::stoi(autoboot);
        b_cfg->autoboot_list = std::stoi(autobootl);
        b_cfg->extra_cfg = 0;
        std::memset(b_cfg->id, 0, sizeof (b_cfg->id));
        std::memset(b_cfg->xt_str, 0, sizeof (b_cfg->xt_str));

        std::FILE* fp = std::fopen(binFile.c_str(), "r+b");
        if (fp != NULL)
        {
            std::fseek(fp, PATCHED_RELOC_SZ, SEEK_SET);
            std::fwrite(b_cfg, sizeof (boot_cfg_t), 1, fp);
            std::fclose(fp);
        }
        else
        {
            std::fclose(fp);
            delete b_cfg;
            return false;
        }
        delete b_cfg;

        usleep(100);

        if (launch)
        {
            if (Init(binFile))
                Reboot();
            else
                return false;
        }
        return true;
        //}
    }
    return false;
}
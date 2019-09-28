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
#include <ui_MainApplication.hpp>
#include <Types.hpp>
#include <sys/stat.h>
#include <cstdlib>
#include <ctime>

extern char *fake_heap_end;
void *new_heap_addr = NULL;

static constexpr u64 HeapSize = 0x10000000;

void Initialize()
{
    if ((GetLaunchMode() == LaunchMode::Applet) && R_SUCCEEDED(svcSetHeapSize(&new_heap_addr, HeapSize)))
        fake_heap_end = (char*) new_heap_addr + HeapSize;
    splInitialize();
    setsysInitialize();
}

void Finalize()
{
    if (GetLaunchMode() == LaunchMode::Applet)
        svcSetHeapSize(&new_heap_addr, ((u8*) envGetHeapOverrideAddr() + envGetHeapOverrideSize()) - (u8*) new_heap_addr);
    splExit();
    setsysExit();
}

namespace ui
{
    MainApplication::Ref mainapp;
}

int main()
{
    Initialize();

    ui::mainapp = ui::MainApplication::New();
    ui::mainapp->ShowWithFadeIn();

    Finalize();
    return 0;
}

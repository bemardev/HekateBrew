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
#include <pu/Plutonium>
#include <elm/elm_TextIcon.hpp>
#include <elm/elm_SimpleGrid.hpp>

namespace ui
{
    class MainPageLayout : public pu::ui::Layout
    {
    public:
        MainPageLayout();
        PU_SMART_CTOR(MainPageLayout)
        void OnInput(u64 Down, u64 Up, u64 Held);
        void Load();
        void Unload();
    private:
        void buttonGrid_OnClick(int btnIndex);
        elm::TextIcon::Ref iconClose;
        elm::TextIcon::Ref iconLaunch;
        elm::TextIcon::Ref iconOptions;
        elm::TextIcon::Ref iconReboot;
        elm::TextIcon::Ref iconHReboot;
        pu::ui::elm::TextBlock::Ref autobootInfos;
        pu::ui::elm::TextBlock::Ref pageName;
        pu::ui::elm::TextBlock::Ref errMessage;
        elm::SimpleGrid::Ref buttonGrid;
    };
}

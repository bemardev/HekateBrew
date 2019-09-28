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
#include <functional>
#include <pu/Plutonium>

namespace elm
{
    class SeparatorLine : public pu::ui::elm::Element
    {
    public:
        SeparatorLine(s32 X1, s32 Y1, s32 X2, s32 Y2, pu::ui::Color LineColor);
        PU_SMART_CTOR(SeparatorLine)

        s32 GetX();
        s32 GetY();
        s32 GetWidth();
        s32 GetHeight();
        void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
        void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
    private:
        s32 x1;
        s32 x2;
        s32 y1;
        s32 y2;
        pu::ui::Color clr;
    };
}
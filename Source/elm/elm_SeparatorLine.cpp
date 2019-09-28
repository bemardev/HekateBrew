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
#include <elm/elm_SeparatorLine.hpp>

namespace elm
{

    SeparatorLine::SeparatorLine(s32 X1, s32 Y1, s32 X2, s32 Y2, pu::ui::Color LineColor) : Element::Element()
    {
        this->x1 = X1;
        this->x2 = X2;
        this->y1 = Y1;
        this->y2 = Y2;
        this->clr = LineColor;
    }

    s32 SeparatorLine::GetX()
    {
        return this->x1;
    }

    s32 SeparatorLine::GetY()
    {
        return this->y1;
    }

    s32 SeparatorLine::GetWidth()
    {
        return this->x2 - this->x1;
    }

    s32 SeparatorLine::GetHeight()
    {
        return this->y2 - this->y1;
    }

    void SeparatorLine::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        Drawer->RenderLine(this->clr, this->x1, this->y1, this->x2, this->y2);
    }

    void SeparatorLine::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
    }
}

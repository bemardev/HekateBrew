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
#include <IconTypes.hpp>

namespace elm
{
    enum class TextIconAlign
    {
        Left, // X is coord of begin
        Right // X is coord of end
    };
    class TextIcon : public pu::ui::elm::Element
    {
    public:
        TextIcon(s32 X, s32 Y, pu::String Text, Icons IconType, pu::ui::Color Color, TextIconAlign Align = TextIconAlign::Left, s32 FontSize = 25);
        PU_SMART_CTOR(TextIcon)
        ~TextIcon();

        s32 GetX();
        void SetX(s32 X);
        s32 GetY();
        void SetY(s32 Y);
        s32 GetWidth();
        s32 GetHeight();
        s32 GetTextWidth();
        s32 GetTextHeight();
        pu::String GetText();
        void SetText(pu::String Text, Icons IconType);
        void SetFont(pu::ui::render::NativeFont Font);
        pu::ui::Color GetColor();
        void SetColor(pu::ui::Color Color);
        void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
        void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
    private:
        pu::String text;
        Icons iconType;
        TextIconAlign align;
        s32 padding;
        s32 x;
        s32 y;
        pu::ui::render::NativeFont fnt;
        pu::ui::render::NativeFont icofnt;
        s32 fsize;
        pu::ui::Color clr;
        pu::ui::render::NativeTexture ntex;
        pu::ui::render::NativeTexture itex;
        void ReloadTextures();
    };
}
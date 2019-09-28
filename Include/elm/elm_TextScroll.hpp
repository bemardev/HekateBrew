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

namespace elm
{
    class TextScroll : public pu::ui::elm::Element
    {
    public:
        TextScroll(s32 X, s32 Y, s32 Width, pu::String Text, s32 FontSize = 30);
        PU_SMART_CTOR(TextScroll)
        ~TextScroll();

        s32 GetX();
        void SetX(s32 X);
        s32 GetY();
        void SetY(s32 Y);
        s32 GetWidth();
        s32 GetHeight();
        s32 GetTextWidth();
        s32 GetTextHeight();
        pu::String GetText();
        void SetText(pu::String Text);
        void SetFont(pu::ui::render::NativeFont Font);
        pu::ui::Color GetColor();
        void SetColor(pu::ui::Color Color);
        void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
        void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
    private:
        int newLength;
        pu::String text;
        std::string currentText;
        s32 x;
        s32 y;
        s32 outputW;
        pu::ui::render::NativeFont fnt;
        s32 fsize;
        pu::ui::Color clr;
        pu::ui::render::NativeTexture ntex;
        std::chrono::high_resolution_clock::time_point start_time;
        std::chrono::high_resolution_clock::time_point current_time;
        std::chrono::duration<double, std::milli> Elapsed;
    };
}
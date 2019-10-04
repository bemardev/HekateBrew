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
#include <vector>
#include <chrono>
#include <functional>
#include <filesystem>

namespace ui
{    
    class SliderDialog
    {
        public:
            SliderDialog(std::string Title, int MinValue, int MaxValue, int Step, int CurrentValue);
            PU_SMART_CTOR(SliderDialog)
            ~SliderDialog();
            
            s32 GetX();
            void SetX(s32 X);
            s32 GetY();
            void SetY(s32 Y);
            void SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor, pu::ui::Color LineColor, pu::ui::Color BaseFocus);
            void SetOnSelectionChanged(std::function<void()> Callback);
            int Show(pu::ui::render::Renderer::Ref &Drawer, void *App);
            bool UserCancelled();
        private:
            std::string title;
            elm::TextIcon::Ref iconLaunch;
            elm::TextIcon::Ref iconMove;
            elm::TextIcon::Ref iconCancel;
            bool dtouch;
            s32 fisel;
            s32 isel;
            s32 x;
            s32 y;
            s32 w;
            s32 h;
            s32 minValue;
            s32 maxValue;
            s32 currentValue;
            s32 step;
            s32 currentXpos;
            bool cancel;
            std::chrono::time_point<std::chrono::steady_clock> blinktime;
            bool _blink;
            std::function<void()> onselch;
            pu::ui::render::NativeFont font;
            pu::ui::render::NativeFont icofont;
            pu::ui::render::NativeTexture vtex;
            pu::ui::Color _txtclr;
            pu::ui::Color _borderclr;
            pu::ui::Color _altclr;
            pu::ui::Color _baseclr;
            pu::ui::Color _innerclr;
            pu::ui::Color _lineclr;
            pu::ui::Color _bfocus;
            
            s32 previsel;
            s32 pselfact;
            s32 selfact;
            
            bool icdown;          
    };
}
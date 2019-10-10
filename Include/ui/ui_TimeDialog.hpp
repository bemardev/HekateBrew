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
#include <vector>
#include <chrono>
#include <functional>
#include <filesystem>

namespace ui
{    
    class TimeDialog
    {
        public:
            TimeDialog(std::string DialogText, s32 Delay=3000);
            PU_SMART_CTOR(TimeDialog)
            ~TimeDialog();
            
            s32 GetX();
            void SetX(s32 X);
            s32 GetY();
            void SetY(s32 Y);
            s32 GetWidth();
            void SetWidth(s32 Width);
            s32 GetHeight();
            void SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BaseColor, pu::ui::Color BaseFocus);
            s32 Show(pu::ui::render::Renderer::Ref &Drawer, void *App);
            bool UserCancelled();
        private:
            std::string dialogText;
            std::string txtcounter;
            s32 initialDelay;
            bool dtouch;
            s32 x;
            s32 y;
            s32 w;
            s32 h;
            bool cancel;
            std::chrono::time_point<std::chrono::steady_clock> delaytime;
            pu::ui::render::NativeFont font;
            pu::ui::render::NativeTexture texdialog;
            pu::ui::Color _txtclr;
            pu::ui::Color _bfocus;
            pu::ui::Color _baseclr;
    };
}
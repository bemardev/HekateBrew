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
    class ListDialog
    {
        public:
            ListDialog(std::string Title, s32 X, s32 Y, s32 Width, s32 Height, pu::ui::Color OptionColor, s32 ItemSize, s32 ItemsToShow);
            PU_SMART_CTOR(ListDialog)

            s32 GetX();
            void SetX(s32 X);
            s32 GetY();
            void SetY(s32 Y);
            s32 GetWidth();
            void SetWidth(s32 Width);
            s32 GetHeight();
            s32 GetItemSize();
            void SetItemSize(s32 ItemSize);
            s32 GetNumberOfItemsToShow();
            void SetNumberOfItemsToShow(s32 ItemsToShow);
            pu::ui::Color GetColor();
            void SetColor(pu::ui::Color Color);
            pu::ui::Color GetOnFocusColor();
            void SetOnFocusColor(pu::ui::Color Color);
            pu::ui::Color GetScrollbarColor();
            void SetScrollbarColor(pu::ui::Color Color);
            void SetOnSelectionChanged(std::function<void()> Callback);
            void AddItem(std::string Item);
            void ClearItems();
            void SetCooldownEnabled(bool Cooldown);
            std::vector<std::string> GetItems();
            s32 GetSelectedIndex();
            void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
            int Show(pu::ui::render::Renderer::Ref &Drawer, void *App);
            void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
            bool UserCancelled();
        private:
            void ReloadItemRenders();
            std::string title;
            elm::TextIcon::Ref iconLaunch;
            elm::TextIcon::Ref iconCancel;
            bool dtouch;
            s32 x;
            s32 y;
            s32 w;
            s32 h;
            s32 isize;
            s32 ishow;
            s32 previsel;
            s32 fisel;
            s32 isel;
            s32 pselfact;
            s32 selfact;
            pu::ui::Color scb;
            pu::ui::Color clr;
            pu::ui::Color fcs;
            bool icdown;
            bool cancel;
            int basestatus;
            std::chrono::time_point<std::chrono::steady_clock> basetime;
            std::function<void()> onselch;
            std::vector<std::string> itms;
            pu::ui::render::NativeFont font;
            pu::ui::render::NativeFont icofont;
            std::vector<pu::ui::render::NativeTexture> loadednames;
    };
}
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
    struct File
    {
        std::string name;
        std::string pathName;
    };
    
    class FileDialog
    {
        public:
            FileDialog(std::string BeginPath);
            PU_SMART_CTOR(FileDialog)
            ~FileDialog();
            
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
            void SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor, pu::ui::Color LineColor, pu::ui::Color BaseFocus);
            void SetOnSelectionChanged(std::function<void()> Callback);
            void AddItem(File Item);
            void ClearItems();
            void SetCooldownEnabled(bool Cooldown);
            std::vector<File> GetItems();
            s32 GetSelectedIndex();
            void SetSelectedIndex(s32 Index);
            void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
            std::string Show(pu::ui::render::Renderer::Ref &Drawer, void *App);
            void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
            bool UserCancelled();
        private:
            void ReloadItemRenders();
            void SetCurrentPath(std::string pathName);
            std::string currentPath;
            elm::TextIcon::Ref iconLaunch;
            elm::TextIcon::Ref iconSelect;
            elm::TextIcon::Ref iconCancel;
            bool dtouch;
            s32 isize;
            s32 ishow;
            s32 fisel;
            s32 isel;
            s32 x;
            s32 y;
            s32 w;
            s32 h;
            bool cancel;
            std::chrono::time_point<std::chrono::steady_clock> blinktime;
            bool _blink;
            std::function<void()> onselch;
            std::vector<File> itms;
            pu::ui::render::NativeFont font;
            pu::ui::render::NativeFont icofont;
            pu::ui::render::NativeTexture istex;
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
            
            int basestatus;
            std::chrono::time_point<std::chrono::steady_clock> basetime;
            std::vector<pu::ui::render::NativeTexture> loadednames;
            std::vector<pu::ui::render::NativeTexture> loadedicons;            
    };
}
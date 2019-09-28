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
#include <map>

namespace elm
{
    class SimpleGridItem
    {
    public:
        SimpleGridItem(pu::String Image, pu::String Name);
        PU_SMART_CTOR(SimpleGridItem)

        pu::String GetName();
        pu::String GetImage();
        void SetNewLength(s32 length);
        s32 GetNewLength();
        void SetName(pu::String Name);
        pu::ui::Color GetColor();
        void SetColor(pu::ui::Color Color);
        bool GetCoolDown();
        void SetCoolDown(bool CoolDown);
        void AddOnClick(std::function<void() > Callback, u64 Key = KEY_A);
        s32 GetCallbackCount();
        std::function<void() > GetCallback(s32 Index);
        u64 GetCallbackKey(s32 Index);
        std::string GetIcon();
        void SetIcon(std::string Icon);
        bool HasIcon();
    private:
        pu::String name;
        pu::String img;
        pu::ui::Color clr;
        bool icdown;
        bool hasicon;
        std::string icon;
        std::vector<std::function<void() >> cbs;
        std::vector<u64> cbipts;
        s32 newLength;
        pu::ui::render::NativeTexture ntex;
    };
    class SimpleGrid : public pu::ui::elm::Element
    {
    public:
        SimpleGrid(s32 X, s32 Y, bool rounded = false);
        PU_SMART_CTOR(SimpleGrid)

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
        void SetHorizontalPadding(s32 HorizontalPadding);
        void SetVerticalPadding(s32 VerticalPadding);
        void SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor);
        pu::ui::Color GetColor();
        void SetColor(pu::ui::Color Color);
        pu::ui::Color GetOnFocusColor();
        void SetOnFocusColor(pu::ui::Color Color);
        pu::ui::Color GetScrollbarColor();
        void SetScrollbarColor(pu::ui::Color Color);
        void SetOnSelectionChanged(std::function<void() > Callback);
        void SetDisplayNameOnSelected(bool txtonsel);
        void SetTextUnderIcon(bool txtunder);
        void SetFontSize(s32 fontsize);
        void AddItem(SimpleGridItem::Ref &Item);
        void ClearItems();
        void SetCooldownEnabled(bool Cooldown);
        SimpleGridItem::Ref &GetSelectedItem();
        std::vector<SimpleGridItem::Ref> &GetItems();
        s32 GetSelectedIndex();
        void SetSelectedIndex(s32 Index);
        void OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y);
        void OnInput(u64 Down, u64 Up, u64 Held, bool Touch);
        pu::ui::render::NativeTexture GetImageContent(std::string imagePath);
        pu::ui::render::NativeTexture GetBorder(pu::ui::Color color);
        pu::ui::render::NativeTexture GetTextContent(std::string text);
    private:
        bool dtouch;
        s32 x;
        s32 y;
        s32 w;
        s32 isize;
        s32 ishow;
        s32 rshow;
        s32 previsel;
        s32 fisel;
        s32 isel;
        s32 hspace;
        s32 vspace;
        s32 _fontsize;
        pu::ui::Color scb;
        pu::ui::Color clr;
        pu::ui::Color fcs;
        bool icdown;
        int basestatus;
        std::chrono::time_point<std::chrono::steady_clock> basetime;
        std::chrono::time_point<std::chrono::steady_clock> blinktime;
        std::function<void() > onselch;
        std::vector<SimpleGridItem::Ref> itms;
        pu::ui::render::NativeFont font;
        std::map<std::string, pu::ui::render::NativeTexture> _contents;
        pu::ui::render::NativeTexture _currentDisplayText;
        bool _blink;
        bool _rounded;
        bool _txtonsel;
        bool _txtunder;
        pu::ui::Color _txtclr;
        pu::ui::Color _borderclr;
        pu::ui::Color _altclr;
        pu::ui::Color _baseclr;
        pu::ui::Color _innerclr;
    };
}
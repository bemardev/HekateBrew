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
#include <ui/ui_MainPageLayout.hpp>
#include <ui/ui_HkConfigLayout.hpp>
#include <ui/ui_PayloadLayout.hpp>
#include <ui/ui_LoadingOverlay.hpp>
#include <ui/ui_OptionsLayout.hpp>
#include <ui/ui_ListDialog.hpp>
#include <set/set_Settings.hpp>
#include <elm/elm_SeparatorLine.hpp>
#include <Types.hpp>
#include <pu/Plutonium>

extern set::Settings gsets;

namespace ui
{
    class MainApplication : public pu::ui::Application
    {
    public:
        MainApplication();
        PU_SMART_CTOR(MainApplication)

        MainPageLayout::Ref &GetMainPageLayout();
        HkConfigLayout::Ref &GetHkConfigLayout();
        PayloadLayout::Ref &GetPayloadLayout();
        OptionsLayout::Ref &GetOptionsLayout();

        void showNotification(std::string text, int delay=1500);
        int CreateShowDialog(pu::String Title, pu::String Content, std::vector<pu::String> Options, bool UseLastOptionAsCancel, std::string Icon = "");
        std::string CreateFileDialog(std::string title, std::string BeginPath);
        int CreateListDialog(std::string title, std::vector<ListDialogItem::Ref> &listItems);
        void endWithErrorMessage(std::string errMessage);
        void ShowLoading(bool close = false);
        void mainClose();
    private:
        elm::SeparatorLine::Ref topSeparator;
        elm::SeparatorLine::Ref bottomSeparator;
        pu::ui::extras::Toast::Ref toast;
        ui::LoadingOverlay::Ref loader;

        MainPageLayout::Ref mainPage;
        HkConfigLayout::Ref configPage;
        PayloadLayout::Ref payloadPage;
        OptionsLayout::Ref optionsPage;

        void OnInput(u64 Down, u64 Up, u64 Held);
    };

    static const ColorScheme DefaultDark = {
        { 45, 45, 45, 1}, // background layout color
        { 70, 70, 70, 255}, // background imagebutton color
        { 115, 223, 235, 255}, // focus color
        { 255, 255, 255, 255}, // menu text color
        { 255, 255, 255, 255}, // grid text
        { 18, 187, 254, 255}, // grid border color
        { 115, 223, 235, 255}, // grid alternate border color
        { 58, 61, 66, 255}, // grid inner border color
        { 104, 110, 108, 255}, // line & disabled text color
        "romfs:/LaunchIcon.png",
        "romfs:/MoreConfigIcon.png",
        "romfs:/PayloadIcon.png",
        "romfs:/config.bmp",
        "romfs:/Warn.png",
        "romfs:/loader.png"
    };

    static const ColorScheme DefaultLight = {
        { 235, 235, 235, 1}, // background layout color
        { 240, 240, 240, 255}, // background imagebutton color
        { 45, 79, 238, 255}, // focus color
        { 15, 15, 15, 255}, // menu text color
        { 15, 15, 15, 255}, // grid text
        { 85, 253, 217, 255}, // grid border color
        { 12, 191, 195, 255}, // grid alternate border color
        { 253, 253, 253, 255}, // grid inner border color
        { 182, 185, 192, 255}, // line & disabled text color
        "romfs:/LaunchIcon.l.png",
        "romfs:/MoreConfigIcon.l.png",
        "romfs:/PayloadIcon.l.png",
        "romfs:/config.l.bmp",
        "romfs:/Warn.l.png",
        "romfs:/loader.png"
    };

    extern MainApplication::Ref mainapp;
}

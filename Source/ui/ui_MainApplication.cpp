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
#include <ui/ui_MainApplication.hpp>
#include <cfw/cfw_Helper.hpp>
#include <set/set_Settings.hpp>
#include <PayloadReboot.hpp>
#include <Utils.hpp>

set::Settings gsets;

namespace ui
{

    MainApplication::MainApplication() : pu::ui::Application()
    {

        gsets = set::ProcessSettings();
        pu::ui::render::SetDefaultFontFromShared(pu::ui::render::SharedFont::Standard);

        this->loader = ui::LoadingOverlay::New(gsets.CustomScheme.loaderImage, pu::ui::Color(40, 40, 40, 255));
        this->toast = pu::ui::extras::Toast::New(":", 20, pu::ui::Color(225, 225, 225, 255), pu::ui::Color(40, 40, 40, 255));
        this->topSeparator = elm::SeparatorLine::New(30, 73, 1250, 73, gsets.CustomScheme.Text);
        this->bottomSeparator = elm::SeparatorLine::New(30, 647, 1250, 647, gsets.CustomScheme.Text);

        this->mainPage = MainPageLayout::New();
        this->mainPage->Add(this->topSeparator);
        this->mainPage->Add(this->bottomSeparator);
        this->SetOnInput(std::bind(&MainApplication::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        this->LoadLayout(this->mainPage);
        this->mainPage->Load();

        this->configPage = HkConfigLayout::New();
        this->configPage->Add(this->topSeparator);
        this->configPage->Add(this->bottomSeparator);

        this->payloadPage = PayloadLayout::New();
        this->payloadPage->Add(this->topSeparator);
        this->payloadPage->Add(this->bottomSeparator);
    }

    MainPageLayout::Ref &MainApplication::GetMainPageLayout()
    {
        return this->mainPage;
    }

    HkConfigLayout::Ref &MainApplication::GetHkConfigLayout()
    {
        return this->configPage;
    }

    PayloadLayout::Ref &MainApplication::GetPayloadLayout()
    {
        return this->payloadPage;
    }

    void MainApplication::OnInput(u64 Down, u64 Up, u64 Held)
    {
    }

    void MainApplication::showNotification(std::string text)
    {
        this->EndOverlay();
        this->toast->SetText(text);
        this->StartOverlayWithTimeout(this->toast, 1500);
    }

    int MainApplication::CreateShowDialog(pu::String Title, pu::String Content, std::vector<pu::String> Options, bool UseLastOptionAsCancel, std::string Icon)
    {
        pu::ui::Dialog dlg(Title, Content);
        dlg.SetColor(gsets.CustomScheme.Base);
        dlg.SetTitleColor(gsets.CustomScheme.Text);
        dlg.SetContentColor(gsets.CustomScheme.Text);
        dlg.SetButtonColor(gsets.CustomScheme.BaseFocus);
        for (s32 i = 0; i < Options.size(); i++)
        {
            if (UseLastOptionAsCancel && (i == Options.size() - 1)) dlg.SetCancelOption(Options[i]);
            else dlg.AddOption(Options[i]);
        }
        if (!Icon.empty()) dlg.SetIcon(Icon);
        int opt = dlg.Show(this->rend, this);
        if (dlg.UserCancelled()) opt = -1;
        else if (!dlg.IsOk()) opt = -2;
        return opt;
    }

    void MainApplication::mainClose()
    {
        int opt = this->CreateShowDialog("Exit", "Exit HekateBrew ?",{"Yes", "No", "Cancel"}, true);
        if (opt == 0)
        {
            this->CloseWithFadeOut();
        }
    }

    void MainApplication::endWithErrorMessage(std::string errMessage)
    {
        int sopt = this->CreateShowDialog("Error", errMessage,{"OK"}, false, gsets.CustomScheme.warnImage);
        if (sopt == 0)
        {
            this->CloseWithFadeOut();
        }
    }

    void MainApplication::ShowLoading(bool close)
    {
        this->EndOverlay();
        if (!close)
            this->StartOverlay(this->loader);
    }
}

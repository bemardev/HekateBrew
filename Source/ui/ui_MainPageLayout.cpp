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
#include <ui/ui_MainPageLayout.hpp>
#include <ui/ui_MainApplication.hpp>
#include <PayloadReboot.hpp>

namespace ui
{

    MainPageLayout::MainPageLayout() : pu::ui::Layout()
    {
        this->SetBackgroundColor(gsets.CustomScheme.Background);

        //ActionIcons
        this->iconLaunch = elm::TextIcon::New(1220, 670, "Launch", Icons::Icon_A, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconLaunch);
        this->iconClose = elm::TextIcon::New(this->iconLaunch->GetX() - 40, 670, "Exit", Icons::Icon_B, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconClose);
        this->iconOptions = elm::TextIcon::New(1220, 20, "Options", Icons::Icon_Plus, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconOptions);
        this->iconReboot = elm::TextIcon::New(this->iconOptions->GetX() - 40, 20, "Reboot to RCM", Icons::Icon_ZR, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconReboot);
        this->iconHReboot = elm::TextIcon::New(this->iconReboot->GetX() - 40, 20, "Reboot to Hekate", Icons::Icon_ZL, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconHReboot);
        this->pageName = pu::ui::elm::TextBlock::New(60, 20, "HekateBrew", 25);
        this->pageName->SetColor(gsets.CustomScheme.Text);
        this->Add(this->pageName);
        this->errMessage = pu::ui::elm::TextBlock::New(0,0, "No payloads found and no Hekate config found.\nGo to Options to select payloads directory.");
        this->errMessage->SetColor(gsets.CustomScheme.Text);
        this->errMessage->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Center);
        this->errMessage->SetVerticalAlign(pu::ui::elm::VerticalAlign::Center);
        this->errMessage->SetVisible(false);
        this->Add(this->errMessage);
        this->autobootInfos = pu::ui::elm::TextBlock::New(0, 612, "", 25);
        this->autobootInfos->SetColor(gsets.CustomScheme.BaseFocus);
        this->autobootInfos->SetVisible(false);
        this->Add(this->autobootInfos);
        
        this->buttonGrid = elm::SimpleGrid::New(58, 232, true);
        this->buttonGrid->SetColorScheme(gsets.CustomScheme.Text, gsets.CustomScheme.GridBord, gsets.CustomScheme.GridAlt, gsets.CustomScheme.GridInner, gsets.CustomScheme.Base);
        this->Add(this->buttonGrid);

        this->SetOnInput(std::bind(&MainPageLayout::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    void MainPageLayout::Load()
    {
        int buttonIndex = 1;
        if (gsets.hbConfig.hasHekate)
        {
            elm::SimpleGridItem::Ref launchItem = elm::SimpleGridItem::New(gsets.CustomScheme.hekateMenuImage, "Hekate Config");
            launchItem->AddOnClick([this, buttonIndex]
            {
                this->buttonGrid_OnClick(buttonIndex);
            });
            this->buttonGrid->AddItem(launchItem);
        }
        buttonIndex += 1;
        if (gsets.payloadItems.size() > 0)
        {
            elm::SimpleGridItem::Ref configItem = elm::SimpleGridItem::New(gsets.CustomScheme.defaultImage, "Payloads");
            configItem->AddOnClick([this, buttonIndex]
            {
                this->buttonGrid_OnClick(buttonIndex);
            });
            this->buttonGrid->AddItem(configItem);
        }
        if (this->buttonGrid->GetItems().size() > 0)
        {
            this->buttonGrid->SetSelectedIndex(0);
            this->errMessage->SetVisible(false);
            if(gsets.hbConfig.autoboot == "1" && gsets.hbConfig.autoboot_payload != "" && stoi(gsets.hbConfig.autoboot_payload) < gsets.payloadItems.size())
            {
                this->autobootInfos->SetText("Autoboot payload: " + gsets.payloadItems[stoi(gsets.hbConfig.autoboot_payload)].payloadPath);
                this->autobootInfos->SetX(1220 - this->autobootInfos->GetTextWidth());
                this->autobootInfos->SetVisible(true);
            }
            else if(gsets.hbConfig.autoboot == "2" && gsets.hbConfig.autoboot_config != "" && stoi(gsets.hbConfig.autoboot_config) < gsets.hekateItems.size())
            {
                this->autobootInfos->SetText("Autoboot config: " + gsets.hekateItems[stoi(gsets.hbConfig.autoboot_config)].entryName);
                this->autobootInfos->SetX(1220 - this->autobootInfos->GetTextWidth());
                this->autobootInfos->SetVisible(true);
            }
            else
            {
                this->autobootInfos->SetVisible(false);
            }
        }else
            this->errMessage->SetVisible(true);
    }

    void MainPageLayout::Unload()
    {
        this->buttonGrid->ClearItems();
    }

    void MainPageLayout::buttonGrid_OnClick(int btnIndex)
    {
        if (btnIndex == 1)
        {
            this->Unload();
            mainapp->LoadLayout(mainapp->GetHkConfigLayout());
            mainapp->GetHkConfigLayout()->Load();
        }
        else if (btnIndex == 2)
        {
            this->Unload();
            mainapp->LoadLayout(mainapp->GetPayloadLayout());
            mainapp->GetPayloadLayout()->Load();
        }
    }

    void MainPageLayout::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if ((Down & KEY_B))
        {
            mainapp->mainClose();
        }
        else if ((Down & KEY_ZR))
        {
            PayloadReboot::RebootRCM();
        }
        else if ((Down & KEY_ZL))
        {
            PayloadReboot::RebootHekate(gsets.hbConfig.path);
        }
        else if ((Down & KEY_PLUS))
        {
            this->Unload();
            mainapp->LoadLayout(mainapp->GetOptionsLayout());
            mainapp->GetOptionsLayout()->Load();
        }
    }
}

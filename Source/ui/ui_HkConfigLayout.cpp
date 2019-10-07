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
#include <ui/ui_HkConfigLayout.hpp>
#include <ui/ui_MainApplication.hpp>
#include <PayloadReboot.hpp>

namespace ui
{

    HkConfigLayout::HkConfigLayout() : pu::ui::Layout()
    {
        this->SetBackgroundColor(gsets.CustomScheme.Background);
        //ActionIcons
        this->iconLaunch = elm::TextIcon::New(1220, 670, "Launch", Icons::Icon_A, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconLaunch);
        this->iconClose = elm::TextIcon::New(this->iconLaunch->GetX() - 40, 670, "Back", Icons::Icon_B, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconClose);
        this->pageName = pu::ui::elm::TextBlock::New(60, 20, "", 25);
        this->pageName->SetColor(gsets.CustomScheme.Text);
        this->Add(this->pageName);
        
        this->buttonGrid = elm::SimpleGrid::New(60, 112, true);
        this->buttonGrid->SetItemSize(192);
        this->buttonGrid->SetVerticalPadding(65);
        this->buttonGrid->SetHorizontalPadding(128);
        this->buttonGrid->SetTextUnderIcon(true);
        this->buttonGrid->SetFontSize(20);
        this->buttonGrid->SetColorScheme(gsets.CustomScheme.Text, gsets.CustomScheme.GridBord, gsets.CustomScheme.GridAlt, gsets.CustomScheme.GridInner, gsets.CustomScheme.Base);
        this->Add(this->buttonGrid);

        this->SetOnInput(std::bind(&HkConfigLayout::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    void HkConfigLayout::Load()
    {
        this->pageName->SetText("Launch");
        for (auto const& item : gsets.hekateItems)
        {
            std::string image = item.entryImage.empty() ? gsets.CustomScheme.defaultImage : item.entryImage;
            elm::SimpleGridItem::Ref launchItem = elm::SimpleGridItem::New(image, item.entryName);
            launchItem->AddOnClick([this, item]
            {
                this->buttonGrid_OnClick(item.entryIndex, item.entryInList);
            });
            this->buttonGrid->AddItem(launchItem);
        }
        if (this->buttonGrid->GetItems().size() > 0)
            this->buttonGrid->SetSelectedIndex(0);
    }

    void HkConfigLayout::Unload()
    {
        this->buttonGrid->ClearItems();
    }

    void HkConfigLayout::buttonGrid_OnClick(std::string autoboot, std::string autobootl)
    {
        if (!PayloadReboot::AlterPayload(autoboot, autobootl, gsets.hbConfig.path, true))
        {
            mainapp->showNotification("Error sending config to hekate payload");
        }
    }

    void HkConfigLayout::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if ((Down & KEY_B))
        {
            this->Unload();
            mainapp->LoadLayout(mainapp->GetMainPageLayout());
            mainapp->GetMainPageLayout()->Load();
        }
    }
}
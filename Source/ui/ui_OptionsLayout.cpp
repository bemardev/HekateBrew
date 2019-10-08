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
#include <ui/ui_OptionsLayout.hpp>
#include <ui/ui_MainApplication.hpp>
#include <set/set_Settings.hpp>
#include <Utils.hpp>
#include <vector>

namespace ui
{

    OptionsLayout::OptionsLayout() : pu::ui::Layout()
    {
        this->SetBackgroundColor(gsets.CustomScheme.Background);
        this->optionSeparator = elm::SeparatorLine::New(320, 112, 320, 608, gsets.CustomScheme.Text);
        this->Add(this->optionSeparator);
        
        //ActionIcons
        this->iconLaunch = elm::TextIcon::New(1220, 670, "Ok", Icons::Icon_A, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconLaunch);
        this->iconSave = elm::TextIcon::New(this->iconLaunch->GetX() - 40, 670, "Save", Icons::Icon_Y, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconSave);
        this->iconHint = elm::TextIcon::New(this->iconSave->GetX() - 40, 670, "Infos", Icons::Icon_Minus, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconHint);
        this->iconClose = elm::TextIcon::New(this->iconHint->GetX() - 40, 670, "Back", Icons::Icon_B, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->Add(this->iconClose);
        
        this->pageName = pu::ui::elm::TextBlock::New(60, 20, "", 25);
        this->pageName->SetColor(gsets.CustomScheme.Text);
        this->Add(this->pageName);
        
        this->optionsMenu = elm::NinMenu::New(30, 112, 280, gsets.CustomScheme.Background, 65, 6);
        this->optionsMenu->SetColorScheme(gsets.CustomScheme.Text, gsets.CustomScheme.GridBord, gsets.CustomScheme.GridAlt, gsets.CustomScheme.GridInner, gsets.CustomScheme.Base, gsets.CustomScheme.BaseFocus);
        this->optionsMenu->SetOnSelectionChanged([this]
        {
           this->contentsMenu->ClearItems(); 
        });
        this->optionsMenu->SetIsFocused(true);
        this->Add(this->optionsMenu);
        
        this->contentsMenu = elm::NinContentMenu::New(330, 112, 920, gsets.CustomScheme.Background, 65, 6);
        this->contentsMenu->SetColorScheme(gsets.CustomScheme.Text, gsets.CustomScheme.GridBord, gsets.CustomScheme.GridAlt, gsets.CustomScheme.GridInner, gsets.CustomScheme.Base, gsets.CustomScheme.BaseFocus);
        this->contentsMenu->SetIsFocused(false);
        this->Add(this->contentsMenu);

        this->SetOnInput(std::bind(&OptionsLayout::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    void OptionsLayout::Load()
    {
        this->pageName->SetText("Options");
        elm::NinMenuItem::Ref payloadOptions = elm::NinMenuItem::New("Payloads");
        payloadOptions->SetColor(gsets.CustomScheme.Text);
        payloadOptions->AddOnClick([this]
        {
            this->LoadPayloadOptionsItems(true);
        });
        payloadOptions->AddOnClick([this]
        {
            this->LoadPayloadOptionsItems(true);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        }, KEY_RIGHT);
        this->optionsMenu->AddItem(payloadOptions);
        
        if(gsets.hbConfig.hasHekate)
        {
            elm::NinMenuItem::Ref hekateOptions = elm::NinMenuItem::New("Hekate config");
            hekateOptions->SetColor(gsets.CustomScheme.Text);
            hekateOptions->AddOnClick([this]
            {
                this->LoadHekateOptionsItems(true);
            });
            hekateOptions->AddOnClick([this]
            {
                this->LoadHekateOptionsItems(true);
                this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
            }, KEY_RIGHT);
            this->optionsMenu->AddItem(hekateOptions);
        }
        
        this->optionsMenu->SetIsFocused(true);
        this->contentsMenu->SetIsFocused(false);
        this->optionsMenu->SetSelectedIndex(0);
    }
    
    void OptionsLayout::LoadPayloadOptionsItems(bool isFocused)
    {
        this->contentsMenu->ClearItems();
        
        // Show Hekate payloads
        elm::NinContentMenuItem::Ref showHekate = elm::NinContentMenuItem::New("Show Hekate payloads", gsets.hbConfig.showHekate=="1" ? "Yes" : "No");
        showHekate->SetColor(gsets.CustomScheme.Text);
        showHekate->SetValueColor(gsets.hbConfig.showHekate=="1" ? gsets.CustomScheme.BaseFocus : gsets.CustomScheme.LineSep);
        showHekate->AddOnClick([this, isFocused]
        {
            gsets.hbConfig.showHekate = (gsets.hbConfig.showHekate=="1") ? "0" : "1";
            this->LoadPayloadOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        showHekate->AddOnClick([this]
            {
                this->contentsMenu->SetIsFocused(false);
                this->optionsMenu->SetIsFocused(true);
            }, KEY_LEFT);
        showHekate->AddOnClick([]
            {
            mainapp->showNotification("Show existing payloads from sdmc:/bootloader/payloads/", 3000);
            }, KEY_MINUS);
        this->contentsMenu->AddItem(showHekate);
        
        // Show Argon payloads
        elm::NinContentMenuItem::Ref showArgon = elm::NinContentMenuItem::New("Show Argon payloads", gsets.hbConfig.showArgon=="1" ? "Yes" : "No");
        showArgon->SetColor(gsets.CustomScheme.Text);
        showArgon->SetValueColor(gsets.hbConfig.showArgon=="1" ? gsets.CustomScheme.BaseFocus : gsets.CustomScheme.LineSep);
        showArgon->AddOnClick([this, isFocused]
        {
            gsets.hbConfig.showArgon = (gsets.hbConfig.showArgon=="1") ? "0" : "1";
            this->LoadPayloadOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        showArgon->AddOnClick([this]
            {
                this->contentsMenu->SetIsFocused(false);
                this->optionsMenu->SetIsFocused(true);
            }, KEY_LEFT);
        showArgon->AddOnClick([]
            {
            mainapp->showNotification("Show existing payloads from sdmc:/argon/payloads/", 3000);
            }, KEY_MINUS);
        this->contentsMenu->AddItem(showArgon);
        
        // Show root payload dir
        elm::NinContentMenuItem::Ref showRootDir = elm::NinContentMenuItem::New("Show payloads from sdmc:/payloads", gsets.hbConfig.showRootDir=="1" ? "Yes" : "No");
        showRootDir->SetColor(gsets.CustomScheme.Text);
        showRootDir->SetValueColor(gsets.hbConfig.showRootDir=="1" ? gsets.CustomScheme.BaseFocus : gsets.CustomScheme.LineSep);
        showRootDir->AddOnClick([this, isFocused]
        {
            gsets.hbConfig.showRootDir = (gsets.hbConfig.showRootDir=="1") ? "0" : "1";
            this->LoadPayloadOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        showRootDir->AddOnClick([this]
            {
                this->contentsMenu->SetIsFocused(false);
                this->optionsMenu->SetIsFocused(true);
            }, KEY_LEFT);
        showRootDir->AddOnClick([]
            {
            mainapp->showNotification("Show existing payloads from sdmc:/payloads/", 3000);
            }, KEY_MINUS);
        this->contentsMenu->AddItem(showRootDir);
        
        // Show Custom path payloads
        elm::NinContentMenuItem::Ref showCustomPath = elm::NinContentMenuItem::New("Show custom path payloads", gsets.hbConfig.showCustomPath=="1" ? "Yes" : "No");
        showCustomPath->SetColor(gsets.CustomScheme.Text);
        showCustomPath->SetValueColor(gsets.hbConfig.showCustomPath=="1" ? gsets.CustomScheme.BaseFocus : gsets.CustomScheme.LineSep);
        showCustomPath->AddOnClick([this, isFocused]
        {
            gsets.hbConfig.showCustomPath = (gsets.hbConfig.showCustomPath=="1") ? "0" : "1";
            this->LoadPayloadOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        showCustomPath->AddOnClick([this]
            {
                this->contentsMenu->SetIsFocused(false);
                this->optionsMenu->SetIsFocused(true);
            }, KEY_LEFT);
        showCustomPath->AddOnClick([]
            {
            mainapp->showNotification("Show existing payloads from defined custom path", 3000);
            }, KEY_MINUS);
        this->contentsMenu->AddItem(showCustomPath);
        
        // Custom payload path
        if(gsets.hbConfig.showCustomPath == "1")
        {
            elm::NinContentMenuItem::Ref customPath = elm::NinContentMenuItem::New("Custom path", gsets.hbConfig.customPath);
            customPath->SetColor(gsets.CustomScheme.Text);
            customPath->SetValueColor(gsets.CustomScheme.BaseFocus);
            customPath->AddOnClick([this, isFocused]
            {
                std::string returnVal = mainapp->CreateFileDialog("Custom path", "sdmc:/");
                if(returnVal != std::string())
                {
                    removeDblSlash(returnVal);
                    gsets.hbConfig.customPath = returnVal;
                }
                this->LoadPayloadOptionsItems(isFocused);
                this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
            });
            customPath->AddOnClick([this]
                {
                    this->contentsMenu->SetIsFocused(false);
                    this->optionsMenu->SetIsFocused(true);
                }, KEY_LEFT);
            customPath->AddOnClick([]
                {
                mainapp->showNotification("Custom path definition", 3000);
                }, KEY_MINUS);
            this->contentsMenu->AddItem(customPath);
        }
        this->optionsMenu->SetIsFocused(!isFocused);
        this->contentsMenu->SetIsFocused(isFocused);
    }

    void OptionsLayout::LoadHekateOptionsItems(bool isFocused)
    {
        this->contentsMenu->ClearItems();
        
        // autoboot/autoboot_List
        elm::NinContentMenuItem::Ref autoboot = elm::NinContentMenuItem::New("autoboot / autoboot_list", gsets.hConfig.autoboot + " / " + gsets.hConfig.autoboot_list);
        autoboot->SetColor(gsets.CustomScheme.Text);
        autoboot->SetValueColor(gsets.CustomScheme.BaseFocus);
        autoboot->AddOnClick([this, isFocused]
        {
            std::vector<ListDialogItem::Ref> listItems;
            ListDialogItem::Ref disabledItem = ListDialogItem::New("Disabled");
            if(gsets.hConfig.autoboot == "0" && gsets.hConfig.autoboot_list == "0")
                disabledItem->SetSelected(true);
            listItems.push_back(disabledItem);
            for(auto &config : gsets.hekateItems)
            {
                ListDialogItem::Ref configItem = ListDialogItem::New(config.entryName);
                configItem->SetIcon(config.entryImage);
                if(gsets.hConfig.autoboot == config.entryIndex && gsets.hConfig.autoboot_list == config.entryInList)
                    configItem->SetSelected(true);
                listItems.push_back(configItem);
            }
            int returnVal = mainapp->CreateListDialog("autoboot", listItems);
            if(returnVal != -1)
            {
                if(returnVal == 0)
                {
                    gsets.hConfig.autoboot = std::to_string(returnVal);
                    gsets.hConfig.autoboot_list = std::to_string(returnVal);
                }
                else
                {
                    gsets.hConfig.autoboot = gsets.hekateItems[returnVal-1].entryIndex;
                    gsets.hConfig.autoboot_list = gsets.hekateItems[returnVal-1].entryInList;
                }
            }
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        autoboot->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
        autoboot->AddOnClick([]
        {
            mainapp->showNotification("0: Disable, #: Boot entry number to auto boot / \n0: Read autoboot boot entry from hekate_ipl.ini, 1: Read from ini folder (ini files are ASCII ordered)", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(autoboot);
        
        // bootwait
        elm::NinContentMenuItem::Ref bootwait = elm::NinContentMenuItem::New("bootwait", gsets.hConfig.bootwait);
        bootwait->SetColor(gsets.CustomScheme.Text);
        bootwait->SetValueColor(gsets.hConfig.bootwait == "0" ? gsets.CustomScheme.LineSep : gsets.CustomScheme.BaseFocus);
        bootwait->AddOnClick([this, isFocused]
        {
            int returnVal = mainapp->CreateSliderDialog("Bootwait", 0, 10, 1, std::stoi(gsets.hConfig.bootwait));
            if(returnVal != -1)
                gsets.hConfig.bootwait = std::to_string(returnVal);
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        bootwait->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
        bootwait->AddOnClick([]
        {
            mainapp->showNotification("0: Disable (It also disables bootlogo. Having VOL- pressed since injection goes to menu.)\n#: Time to wait for VOL- to enter menu", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(bootwait);
        
        // verification
        elm::NinContentMenuItem::Ref verification = elm::NinContentMenuItem::New("verification", gsets.hConfig.verification);
        verification->SetColor(gsets.CustomScheme.Text);
        verification->SetValueColor(gsets.hConfig.verification == "0" ? gsets.CustomScheme.LineSep : gsets.CustomScheme.BaseFocus);
        verification->AddOnClick([this, isFocused]
        {
            std::vector<ListDialogItem::Ref> listItems;
            for(auto &verif : {"0","1","2"})
            {
                ListDialogItem::Ref valueItem = ListDialogItem::New(verif);
                if(gsets.hConfig.verification == verif)
                    valueItem->SetSelected(true);
                listItems.push_back(valueItem);
            }
            int returnVal = mainapp->CreateListDialog("verification", listItems);
            if(returnVal != -1)
                gsets.hConfig.verification = std::to_string(returnVal);
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        verification->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
        verification->AddOnClick([]
        {
            mainapp->showNotification("0: Disable Backup/Restore verification\n1: Sparse (block based, fast and not 100% reliable)\n2: Full (sha256 based, slow and 100% reliable).", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(verification);
        
        // backlight
        elm::NinContentMenuItem::Ref backlight = elm::NinContentMenuItem::New("backlight", gsets.hConfig.backlight);
        backlight->SetColor(gsets.CustomScheme.Text);
        backlight->SetValueColor(gsets.hConfig.backlight == "0" ? gsets.CustomScheme.LineSep : gsets.CustomScheme.BaseFocus);
        backlight->AddOnClick([this, isFocused]
        {
            int returnVal = mainapp->CreateSliderDialog("Backlight", 0, 255, 5, std::stoi(gsets.hConfig.backlight));
            if(returnVal != -1)
                gsets.hConfig.backlight = std::to_string(returnVal);
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        backlight->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
        backlight->AddOnClick([]
        {
            mainapp->showNotification("Screen backlight level from 0 to 255", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(backlight);
        
        // autohosoff
        elm::NinContentMenuItem::Ref autohosoff = elm::NinContentMenuItem::New("autohosoff", gsets.hConfig.autohosoff);
        autohosoff->SetColor(gsets.CustomScheme.Text);
        autohosoff->SetValueColor(gsets.hConfig.autohosoff == "0" ? gsets.CustomScheme.LineSep : gsets.CustomScheme.BaseFocus);
        autohosoff->AddOnClick([this, isFocused]
        {
            std::vector<ListDialogItem::Ref> listItems;
            for(auto &autohosoff : {"0","1","2"})
            {
                ListDialogItem::Ref valueItem = ListDialogItem::New(autohosoff);
                if(gsets.hConfig.autohosoff == autohosoff)
                    valueItem->SetSelected(true);
                listItems.push_back(valueItem);
            }
            int returnVal = mainapp->CreateListDialog("autohosoff", listItems);
            if(returnVal != -1)
                gsets.hConfig.autohosoff = std::to_string(returnVal);
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        autohosoff->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
            autohosoff->AddOnClick([]
        {
            mainapp->showNotification("0: Disable\n1: If woke up from HOS via an RTC alarm, shows logo, then powers off completely\n2: No logo, immediately powers off", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(autohosoff);
        
        // autonogc
        elm::NinContentMenuItem::Ref autonogc = elm::NinContentMenuItem::New("autonogc", gsets.hConfig.autonogc);
        autonogc->SetColor(gsets.CustomScheme.Text);
        autonogc->SetValueColor(gsets.hConfig.autonogc == "0" ? gsets.CustomScheme.LineSep : gsets.CustomScheme.BaseFocus);
        autonogc->AddOnClick([this, isFocused]
        {
            //Load popup or ui element to modify
            gsets.hConfig.autonogc = (gsets.hConfig.autonogc == "0") ? "1" : "0";
            this->LoadHekateOptionsItems(isFocused);
            this->contentsMenu->SetSelectedIndex(this->contentsMenu->GetSelectedIndex());
        });
        autonogc->AddOnClick([this]
        {
            this->contentsMenu->SetIsFocused(false);
            this->optionsMenu->SetIsFocused(true);
        }, KEY_LEFT);
        autonogc->AddOnClick([]
        {
            mainapp->showNotification("0: Disable\n1: Automatically applies nogc patch if unburnt fuses found and a >= 4.0.0 HOS is booted", 3000);
        }, KEY_MINUS);
        this->contentsMenu->AddItem(autonogc);
        
        this->optionsMenu->SetIsFocused(!isFocused);
        this->contentsMenu->SetIsFocused(isFocused);
    }
    
    void OptionsLayout::Unload()
    {
        this->optionsMenu->ClearItems();
        this->contentsMenu->ClearItems();
    }

    void OptionsLayout::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if ((Down & KEY_B))
        {
            this->Unload();
            gsets = set::ProcessSettings();
            mainapp->LoadLayout(mainapp->GetMainPageLayout());
            mainapp->GetMainPageLayout()->Load();
        }
        else if ((Down & KEY_Y))
        {
            mainapp->ShowLoading();
            set::SaveSettings(gsets);
            this->Unload();
            set::ReloadList(gsets);
            mainapp->ShowLoading(true);
            mainapp->LoadLayout(mainapp->GetMainPageLayout());
            mainapp->GetMainPageLayout()->Load();
            mainapp->showNotification("Settings saved");
        }
    }
}

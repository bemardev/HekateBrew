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
#include <ui/ui_FileDialog.hpp>
#include <IconTypes.hpp>
#include <Utils.hpp>

namespace ui
{

    FileDialog::FileDialog(std::string BeginPath)
    {
        this->currentPath = BeginPath;
        this->blinktime = std::chrono::steady_clock::now();
        this->_blink = true;
        this->_txtclr = {255, 255, 255, 255};
        this->_borderclr = {18, 187, 254, 255};
        this->_altclr = {115, 223, 235, 255};
        this->_baseclr = {70, 70, 70, 255};
        this->_lineclr = {106, 106, 106, 255};
        this->_innerclr = {58, 61, 66, 255};

        this->x = 0;
        this->y = 129;
        this->w = 1280;
        this->h = 0;
        this->isize = 60; //Hardcoded as in Nintendo style //old : ItemSize;
        this->ishow = 7; //Hardcoded as in Nintendo style //old : ItemsToShow;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->onselch = [&]()
        {
        };
        this->icdown = false;
        this->dtouch = false;
        this->basestatus = 0;
        this->font = pu::ui::render::LoadDefaultFont(25);
        this->icofont = pu::ui::render::LoadSharedIconFont(pu::ui::render::SharedFont::NintendoExtended, 25);
        this->cancel = false;
        this->istex = pu::ui::render::RenderIcon(this->icofont, Icons::Icon_Checked, this->_bfocus);
        this->iconLaunch = elm::TextIcon::New(1220, 670, "Ok", Icons::Icon_A, this->_txtclr, elm::TextIconAlign::Right);
        this->iconSelect = elm::TextIcon::New(this->iconLaunch->GetX() - 40, 670, "Select", Icons::Icon_X, this->_txtclr, elm::TextIconAlign::Right);
        this->iconCancel = elm::TextIcon::New(this->iconSelect->GetX() - 40, 670, "Cancel", Icons::Icon_B, this->_txtclr, elm::TextIconAlign::Right);
    }

    FileDialog::~FileDialog()
    {
        if (this->istex != NULL)
        {
            pu::ui::render::DeleteTexture(this->istex);
            this->istex = NULL;
        }
        for (u32 i = 0; i < this->loadednames.size(); i++) pu::ui::render::DeleteTexture(this->loadednames[i]);
        for (u32 i = 0; i < this->loadedicons.size(); i++) pu::ui::render::DeleteTexture(this->loadedicons[i]);
        this->loadednames.clear();
        this->loadedicons.clear();
    }

    s32 FileDialog::GetX()
    {
        return this->x;
    }

    void FileDialog::SetX(s32 X)
    {
        this->x = X;
    }

    s32 FileDialog::GetY()
    {
        return this->y;
    }

    void FileDialog::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 FileDialog::GetWidth()
    {
        return this->w;
    }

    void FileDialog::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 FileDialog::GetHeight()
    {
        return this->h;
    }

    s32 FileDialog::GetItemSize()
    {
        return this->isize;
    }

    void FileDialog::SetItemSize(s32 ItemSize)
    {
        this->isize = ItemSize;
    }

    s32 FileDialog::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void FileDialog::SetNumberOfItemsToShow(s32 ItemsToShow)
    {
        this->ishow = ItemsToShow;
    }

    void FileDialog::SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor, pu::ui::Color LineColor, pu::ui::Color BaseFocus)
    {
        this->_txtclr = TextColor;
        this->_borderclr = BorderColor;
        this->_altclr = AltBorderColor;
        this->_innerclr = InnerBorderColor;
        this->_baseclr = BaseColor;
        this->_lineclr = LineColor;
        this->_bfocus = BaseFocus;
        pu::ui::render::DeleteTexture(this->istex);
        this->istex = pu::ui::render::RenderIcon(this->icofont, Icons::Icon_Checked, this->_bfocus);
        this->iconLaunch->SetColor(this->_txtclr);
        this->iconSelect->SetColor(this->_txtclr);
        this->iconCancel->SetColor(this->_txtclr);
    }

    void FileDialog::SetOnSelectionChanged(std::function<void() > Callback)
    {
        this->onselch = Callback;
    }

    void FileDialog::AddItem(File Item)
    {
        this->itms.push_back(Item);
    }

    void FileDialog::ClearItems()
    {
        this->itms.clear();
        this->loadednames.clear();
    }

    void FileDialog::SetCooldownEnabled(bool Cooldown)
    {
        this->icdown = Cooldown;
    }

    std::vector<File> FileDialog::GetItems()
    {
        return this->itms;
    }

    s32 FileDialog::GetSelectedIndex()
    {
        return this->isel;
    }

    void FileDialog::SetSelectedIndex(s32 Index)
    {
        if (this->itms.size() > Index)
        {
            this->isel = Index;
            this->previsel = Index;
            this->fisel = 0;
            if (this->isel >= (this->itms.size() - this->ishow)) this->fisel = this->itms.size() - this->ishow;
            else if (this->isel < this->ishow) this->fisel = 0;
            else this->fisel = this->isel;

            ReloadItemRenders();
        }
    }

    void FileDialog::SetCurrentPath(std::string pathName)
    {
        this->itms.clear();
        this->currentPath = pathName;
        for (const auto& entry : listSubDirectories(pathName))
        {
            this->itms.push_back({entry, (pathName + R"(/)" + entry)});
        }
        if (std::count(pathName.begin(), pathName.end(), '/') > 1)
            pathName.erase(pathName.begin() + pathName.find_last_of("/"), pathName.end());

        this->itms.insert(this->itms.begin(),
        {
            "..", pathName
        });
    }

    std::string FileDialog::Show(pu::ui::render::Renderer::Ref &Drawer, void *App)
    {
        this->SetCurrentPath(this->currentPath);
        pu::ui::Color ovclr({this->_baseclr.R, this->_baseclr.G, this->_baseclr.B, 140});
        pu::ui::render::NativeTexture pathtex = NULL;
        bool end = false;
        while (true)
        {
            bool ok = ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref & Drawer) -> bool
            {
                //Global overlay
                Drawer->RenderRectangleFill(ovclr, 0, 0, 1280, 720);
                //Popup back color
                Drawer->RenderRectangleFill(this->_baseclr, this->GetX(), this->GetY(), this->GetWidth(), 720 - this->GetY());
                //Top Separator Line
                Drawer->RenderLine(this->_txtclr, 30, this->GetY() + 71, 1250, this->GetY() + 71);
                //Bottom Separator Line
                Drawer->RenderLine(this->_txtclr, 30, 647, 1250, 647);
                //CurrentPath Text
                if(pathtex != NULL) pu::ui::render::DeleteTexture(pathtex);
                pathtex = pu::ui::render::RenderText(this->font, this->currentPath, this->_bfocus);
                s32 tth = pu::ui::render::GetTextHeight(this->font, this->currentPath);
                s32 telmy = this->GetY() + 11 + ((60 - tth) / 2);
                Drawer->RenderTexture(pathtex, 72, telmy);
                // Top element separator
                Drawer->RenderLine(this->_lineclr, 280, this->GetY() + 82, 1000, this->GetY() + 82);
                u64 d = hidKeysDown(CONTROLLER_P1_AUTO);
                if ((d & KEY_DDOWN) || (d & KEY_LSTICK_DOWN))
                {
                    this->blinktime = std::chrono::steady_clock::now();
                    this->_blink = true;
                    if (this->isel < (this->itms.size() - 1))
                    {
                        if ((this->isel - this->fisel) == (this->ishow - 1))
                        {
                            this->fisel++;
                            this->isel++;
                            (this->onselch)();
                            ReloadItemRenders();
                        }
                        else
                        {
                            this->previsel = this->isel;
                            this->isel++;
                            (this->onselch)();
                            if (!this->itms.empty()) for (s32 i = 0; i < this->itms.size(); i++)
                            {
                                if (i == this->isel) this->selfact = 0;
                                else if (i == this->previsel) this->pselfact = 255;
                            }
                        }
                    }
                    else
                    {
                        this->isel = 0;
                        this->fisel = 0;
                        if (this->itms.size() >= this->ishow)
                        {
                            ReloadItemRenders();
                        }
                    }
                }
                else if ((d & KEY_DUP) || (d & KEY_LSTICK_UP))
                {
                    this->blinktime = std::chrono::steady_clock::now();
                    this->_blink = true;
                    if (this->isel > 0)
                    {
                        if (this->isel == this->fisel)
                        {
                            this->fisel--;
                            this->isel--;
                            (this->onselch)();
                            ReloadItemRenders();
                        }
                        else
                        {
                            this->previsel = this->isel;
                            this->isel--;
                            (this->onselch)();
                            if (!this->itms.empty()) for (s32 i = 0; i < this->itms.size(); i++)
                            {
                                if (i == this->isel) this->selfact = 0;
                                else if (i == this->previsel) this->pselfact = 255;
                            }
                        }
                    }
                    else
                    {
                        this->isel = this->itms.size() - 1;
                        this->fisel = 0;
                        if (this->itms.size() >= this->ishow)
                        {
                            this->fisel = this->itms.size() - this->ishow;
                            ReloadItemRenders();
                        }
                    }
                }
                else if (d & KEY_X)
                {
                    end = true;
                    this->cancel = false;
                }
                else if (d & KEY_A)
                {
                    end = false;
                    this->cancel = false;
                    this->SetCurrentPath(this->itms[this->GetSelectedIndex()].pathName);
                    this->isel = 0;
                    this->fisel = 0;
                    this->ReloadItemRenders();
                }
                else if (d & KEY_B)
                {
                    end = true;
                    this->cancel = true;
                }
                u64 th = hidKeysDown(CONTROLLER_HANDHELD);
                this->OnRender(Drawer, 280, this->GetY() + 83);
                this->iconLaunch->OnRender(Drawer, this->iconLaunch->GetX(), this->iconLaunch->GetY());
                this->iconSelect->OnRender(Drawer, this->iconSelect->GetX(), this->iconSelect->GetY());
                this->iconCancel->OnRender(Drawer, this->iconCancel->GetX(), this->iconCancel->GetY());
                if (end)
                    return false;
                return true;
            });
            if (!ok)
            {
                ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref & Drawer) -> bool
                {
                });
                break;
            }
        }
        return this->itms[this->GetSelectedIndex()].pathName;
    }

    void FileDialog::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if (!this->itms.empty())
        {
            s32 cx = X;
            s32 cy = Y;
            s32 cw = 1000 - 280;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if (its > this->itms.size()) its = this->itms.size();
            if ((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if (this->loadednames.empty()) ReloadItemRenders();
            for (s32 i = this->fisel; i < (its + this->fisel); i++)
            {
                auto loadedidx = i - this->fisel;
                auto curname = this->loadednames[loadedidx];
                auto curicon = this->loadedicons[loadedidx];
                if (this->isel == i)
                {
                    auto curtime = std::chrono::steady_clock::now();
                    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - this->blinktime).count();
                    if (diff >= 500)
                    {
                        this->_blink = !this->_blink;
                        this->blinktime = std::chrono::steady_clock::now();
                    }
                    if (this->_blink)
                    {
                        Drawer->RenderRoundedRectangleFill(this->_borderclr, cx - 5, cy - 5, cw + 10, ch + 10, 4);
                    }
                    else
                    {
                        Drawer->RenderRoundedRectangleFill(this->_altclr, cx - 5, cy - 5, cw + 10, ch + 10, 4);
                    }
                    Drawer->RenderRectangleFill(this->_innerclr, cx, cy, cw, ch);
                }
                s32 xh = pu::ui::render::GetTextHeight(this->font, this->itms[i].name);
                s32 tx = (cx + 15);
                s32 ty = ((ch - xh) / 2) + cy;
                Drawer->RenderTexture(curname, tx, ty);
                Drawer->RenderLine(this->_lineclr, 280, cy + 60, 1000, cy + 60);
                cy += ch + 1;
            }
            if (this->ishow < this->itms.size())
            {
                s32 sccr = this->_lineclr.R;
                s32 sccg = this->_lineclr.G;
                s32 sccb = this->_lineclr.B;
                s32 snr = sccr - 30;
                if (snr < 0) snr = 0;
                s32 sng = sccg - 30;
                if (sng < 0) sng = 0;
                s32 snb = sccb - 30;
                if (snb < 0) snb = 0;
                pu::ui::Color sclr(snr, sng, snb, this->_lineclr.A);
                s32 scx = X + (this->w - 20);
                s32 scy = Y;
                s32 scw = 20;
                s32 sch = (this->ishow * this->isize);
                Drawer->RenderRectangleFill(this->_lineclr, scx, scy, scw, sch);
                s32 fch = ((this->ishow * sch) / this->itms.size());
                s32 fcy = scy + (this->fisel * (sch / this->itms.size()));
                Drawer->RenderRectangleFill(sclr, scx, fcy, scw, fch);
            }
        }
    }

    void FileDialog::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
        if (itms.empty()) return;
        if (basestatus == 1)
        {
            auto curtime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - basetime).count();
            if (diff >= 150)
            {
                basestatus = 2;
            }
        }
        if (Touch)
        {
            touchPosition tch;
            hidTouchRead(&tch, 0);
            s32 cx = this->GetX();
            s32 cy = this->GetY();
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if (its > this->itms.size()) its = this->itms.size();
            if ((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            for (s32 i = this->fisel; i < (this->fisel + its); i++)
            {
                if (((cx + cw) > tch.px) && (tch.px > cx) && ((cy + ch) > tch.py) && (tch.py > cy))
                {
                    this->dtouch = true;
                    this->previsel = this->isel;
                    this->isel = i;
                    (this->onselch)();
                    if (i == this->isel) this->selfact = 255;
                    else if (i == this->previsel) this->pselfact = 0;
                    break;
                }
                cy += this->isize;
            }
        }
        else
        {


        }
    }

    bool FileDialog::UserCancelled()
    {
        return this->cancel;
    }

    void FileDialog::ReloadItemRenders()
    {
        for (u32 i = 0; i < this->loadednames.size(); i++) pu::ui::render::DeleteTexture(this->loadednames[i]);
        for (u32 i = 0; i < this->loadedicons.size(); i++) pu::ui::render::DeleteTexture(this->loadedicons[i]);
        this->loadednames.clear();
        this->loadedicons.clear();
        s32 its = this->ishow;
        if (its > this->itms.size()) its = this->itms.size();
        if ((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
        for (s32 i = this->fisel; i < (its + this->fisel); i++)
        {
            auto strname = this->itms[i].name;
            auto tex = pu::ui::render::RenderText(this->font, strname, this->_txtclr);
            this->loadednames.push_back(tex);
        }
    }
}
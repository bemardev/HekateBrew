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
#include <ui/ui_MainApplication.hpp>
#include <IconTypes.hpp>
#include <Utils.hpp>

namespace ui
{
    bool sortByName(const File &lfile, const File &rfile) { return lfile.name < rfile.name; }
    
    FileDialog::FileDialog(std::string Title, std::string BeginPath, s32 X, s32 Y, s32 Width, s32 Height, pu::ui::Color OptionColor, s32 ItemSize, s32 ItemsToShow)
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->h = Height;
        this->clr = OptionColor;
        this->scb = { 110, 110, 110, 255 };
        this->isize = ItemSize;
        this->ishow = ItemsToShow;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->onselch = [&](){};
        this->icdown = false;
        this->dtouch = false;
        this->fcs = { 40, 40, 40, 255 };
        this->basestatus = 0;
        this->font = pu::ui::render::LoadDefaultFont(25);
        this->icofont = pu::ui::render::LoadSharedIconFont(pu::ui::render::SharedFont::NintendoExtended, 25);
        this->title = Title;
        this->currentPath = BeginPath;
        this->cancel = false;
        this->iconLaunch = elm::TextIcon::New(1060, this->GetY() + 530, "Ok", Icons::Icon_A, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->iconSelect = elm::TextIcon::New(this->iconLaunch->GetX() - 40, this->GetY() + 530, "Select", Icons::Icon_X, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
        this->iconCancel = elm::TextIcon::New(this->iconSelect->GetX() - 40, this->GetY() + 530, "Cancel", Icons::Icon_B, gsets.CustomScheme.Text, elm::TextIconAlign::Right);
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

    pu::ui::Color FileDialog::GetColor()
    {
        return this->clr;
    }

    void FileDialog::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
    }
    
    pu::ui::Color FileDialog::GetOnFocusColor()
    {
        return this->fcs;
    }

    void FileDialog::SetOnFocusColor(pu::ui::Color Color)
    {
        this->fcs = Color;
    }

    pu::ui::Color FileDialog::GetScrollbarColor()
    {
        return this->scb;
    }

    void FileDialog::SetScrollbarColor(pu::ui::Color Color)
    {
        this->scb = Color;
    }

    void FileDialog::SetOnSelectionChanged(std::function<void()> Callback)
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
        
    void FileDialog::SetCurrentPath(std::string pathName)
    {
        this->itms.clear();
        this->currentPath = pathName;
        for (const auto& entry : listSubDirectories(pathName))
        {
            this->itms.push_back({entry, (pathName + R"(/)" + entry)});
        }
        if(std::count(pathName.begin(), pathName.end(), '/') > 1)
            pathName.erase(pathName.begin()+pathName.find_last_of("/"), pathName.end());
        
        this->itms.insert(this->itms.begin(), {"..", pathName});
    }

    std::string FileDialog::Show(pu::ui::render::Renderer::Ref &Drawer, void *App)
    {
        this->SetCurrentPath(this->currentPath);
        pu::ui::Color ovclr({gsets.CustomScheme.Base.R, gsets.CustomScheme.Base.G, gsets.CustomScheme.Base.B, 140});
        auto titletex = pu::ui::render::RenderText(this->font, this->title, gsets.CustomScheme.Text);
        auto pathtex = pu::ui::render::RenderText(this->font, this->currentPath, gsets.CustomScheme.BaseFocus);
        s32 ttw = pu::ui::render::GetTextWidth(this->font, this->title);
        s32 tth = pu::ui::render::GetTextHeight(this->font, this->title);
        s32 telmx = this->GetX() + ((this->GetWidth() - ttw) / 2);
        s32 telmy = this->GetY() + ((60 - tth) / 2);
        bool end = false;
        while(true)
        {
            bool ok = ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref &Drawer) -> bool
            {
                Drawer->RenderRectangleFill(ovclr, 0, 0, 1280, 720);
                Drawer->RenderRoundedRectangleFill(gsets.CustomScheme.Base, this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight(), 20);
                Drawer->RenderTexture(titletex, telmx, telmy);
                Drawer->RenderShadowSimple(this->GetX(), this->GetY()+60, this->GetWidth(), 5, 160);
                Drawer->RenderShadowSimple(this->GetX(), this->GetY()+120, this->GetWidth(), 5, 160);
                s32 cth = pu::ui::render::GetTextHeight(this->font, this->currentPath);
                s32 celmy = this->GetY() + 60 + ((60 - cth)/2);
                pu::ui::render::DeleteTexture(pathtex);
                pathtex = pu::ui::render::RenderText(this->font, this->currentPath, gsets.CustomScheme.BaseFocus);
                Drawer->RenderTexture(pathtex, this->GetX()+25, celmy);
                u64 d = hidKeysDown(CONTROLLER_P1_AUTO);
                if((d & KEY_DDOWN) || (d & KEY_LSTICK_DOWN))
                {
                    if(this->isel < (this->itms.size() - 1))
                    {
                        if((this->isel - this->fisel) == (this->ishow - 1))
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
                            if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                            {
                                if(i == this->isel) this->selfact = 0;
                                else if(i == this->previsel) this->pselfact = 255;
                            }
                        }
                    }
                    else
                    {
                        this->isel = 0;
                        this->fisel = 0;
                        if(this->itms.size() >= this->ishow)
                        {
                            ReloadItemRenders();
                        }
                    }
                }
                else if((d & KEY_DUP) || (d & KEY_LSTICK_UP))
                {
                    if(this->isel > 0)
                    {
                        if(this->isel == this->fisel)
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
                            if(!this->itms.empty()) for(s32 i = 0; i < this->itms.size(); i++)
                            {
                                if(i == this->isel) this->selfact = 0;
                                else if(i == this->previsel) this->pselfact = 255;
                            }
                        }
                    }
                    else
                    {
                        this->isel = this->itms.size() - 1;
                        this->fisel = 0;
                        if(this->itms.size() >= this->ishow) {
                            this->fisel = this->itms.size() - this->ishow;
                            ReloadItemRenders();
                        }
                    }
                }
                else if(d & KEY_X)
                {
                    end = true;
                    this->cancel = false;
                }
                else if(d & KEY_A)
                {
                    end = false;
                    this->cancel = false;
                    this->SetCurrentPath(this->itms[this->GetSelectedIndex()].pathName);
                    this->isel = 0;
                    this->fisel = 0;
                    this->ReloadItemRenders();
                }
                else if(d & KEY_B)
                {
                    end = true;
                    this->cancel = true;
                }
                else if(d & KEY_Y)
                {
                    mainapp->showNotification(this->itms[this->GetSelectedIndex()].pathName);
                }
                u64 th = hidKeysDown(CONTROLLER_HANDHELD);
                //this->OnInput(d, u, h, th);
                this->OnRender(Drawer, this->GetX(), this->GetY()+125);
                this->iconLaunch->OnRender(Drawer, this->iconLaunch->GetX(), this->iconLaunch->GetY());
                this->iconSelect->OnRender(Drawer, this->iconSelect->GetX(), this->iconSelect->GetY());
                this->iconCancel->OnRender(Drawer, this->iconCancel->GetX(), this->iconCancel->GetY());
                if(end)
                    return false;
                return true;
            });
            if(!ok)
            {
                ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref &Drawer) -> bool {});
                break;
            }
        }
        return this->itms[this->GetSelectedIndex()].pathName;
    }
    
    void FileDialog::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if(!this->itms.empty())
        {
            s32 cx = X;
            s32 cy = Y;
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if(this->loadednames.empty()) ReloadItemRenders();
            for(s32 i = this->fisel; i < (its + this->fisel); i++)
            {
                s32 clrr = this->clr.R;
                s32 clrg = this->clr.G;
                s32 clrb = this->clr.B;
                s32 nr = clrr - 70;
                if(nr < 0) nr = 0;
                s32 ng = clrg - 70;
                if(ng < 0) ng = 0;
                s32 nb = clrb - 70;
                if(nb < 0) nb = 0;
                pu::ui::Color nclr(nr, ng, nb, this->clr.A);
                auto loadedidx = i - this->fisel;
                auto curname = this->loadednames[loadedidx];
                if(this->isel == i)
                {
                    Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->selfact < 255)
                    {
                        Drawer->RenderRectangleFill(pu::ui::Color(this->fcs.R, this->fcs.G, this->fcs.B, this->selfact), cx, cy, cw, ch);
                        this->selfact += 48;
                    }
                    else Drawer->RenderRectangleFill(this->fcs, cx, cy, cw, ch);
                }
                else if(this->previsel == i)
                {
                    Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                    if(this->pselfact > 0)
                    {
                        Drawer->RenderRectangleFill(pu::ui::Color(this->fcs.R, this->fcs.G, this->fcs.B, this->pselfact), cx, cy, cw, ch);
                        this->pselfact -= 48;
                    }
                    else Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                }
                else Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                auto itm = this->itms[i];
                s32 xh = pu::ui::render::GetTextHeight(this->font, itm.name);
                s32 tx = (cx + 25);
                s32 ty = ((ch - xh) / 2) + cy;
                Drawer->RenderTexture(curname, tx, ty);
                cy += ch;
            }
            if(this->ishow < this->itms.size())
            {
                s32 sccr = this->scb.R;
                s32 sccg = this->scb.G;
                s32 sccb = this->scb.B;
                s32 snr = sccr - 30;
                if(snr < 0) snr = 0;
                s32 sng = sccg - 30;
                if(sng < 0) sng = 0;
                s32 snb = sccb - 30;
                if(snb < 0) snb = 0;
                pu::ui::Color sclr(snr, sng, snb, this->scb.A);
                s32 scx = X + (this->w - 20);
                s32 scy = Y;
                s32 scw = 20;
                s32 sch = (this->ishow * this->isize);
                Drawer->RenderRectangleFill(this->scb, scx, scy, scw, sch);
                s32 fch = ((this->ishow * sch) / this->itms.size());
                s32 fcy = scy + (this->fisel * (sch / this->itms.size()));
                Drawer->RenderRectangleFill(sclr, scx, fcy, scw, fch);
            }
            Drawer->RenderShadowSimple(cx, cy, cw, 5, 160);
        }
    }

    void FileDialog::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
        if(itms.empty()) return;
        if(basestatus == 1)
        {
            auto curtime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - basetime).count();
            if(diff >= 150)
            {
                basestatus = 2;
            }
        }
        if(Touch)
        {
            touchPosition tch;
            hidTouchRead(&tch, 0);
            s32 cx = this->GetX();
            s32 cy = this->GetY();
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if(its > this->itms.size()) its = this->itms.size();
            if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            for(s32 i = this->fisel; i < (this->fisel + its); i++)
            {
                if(((cx + cw) > tch.px) && (tch.px > cx) && ((cy + ch) > tch.py) && (tch.py > cy))
                {
                    this->dtouch = true;
                    this->previsel = this->isel;
                    this->isel = i;
                    (this->onselch)();
                    if(i == this->isel) this->selfact = 255;
                    else if(i == this->previsel) this->pselfact = 0;
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
        for(u32 i = 0; i < this->loadednames.size(); i++) pu::ui::render::DeleteTexture(this->loadednames[i]);
        this->loadednames.clear();
        s32 its = this->ishow;
        if(its > this->itms.size()) its = this->itms.size();
        if((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
        for(s32 i = this->fisel; i < (its + this->fisel); i++)
        {
            auto strname = this->itms[i].name;
            auto tex = pu::ui::render::RenderText(this->font, strname, gsets.CustomScheme.Text);
            this->loadednames.push_back(tex);
        }
    }
}
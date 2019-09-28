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
#include <elm/elm_SimpleGrid.hpp>
#include <ui/ui_MainApplication.hpp>
#include <Utils.hpp>

namespace elm
{

    SimpleGridItem::SimpleGridItem(pu::String Image, pu::String Name)
    {
        this->clr = {58, 168, 182, 255};
        this->name = Name;
        this->img = Image;
        this->hasicon = true;
        this->icdown = true;
    }

    pu::String SimpleGridItem::GetName()
    {
        return this->name;
    }

    pu::String SimpleGridItem::GetImage()
    {
        return this->img;
    }

    void SimpleGridItem::SetName(pu::String Name)
    {
        this->name = Name;
    }

    pu::ui::Color SimpleGridItem::GetColor()
    {
        return this->clr;
    }

    void SimpleGridItem::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
    }

    bool SimpleGridItem::GetCoolDown()
    {
        return this->icdown;
    }

    void SimpleGridItem::SetCoolDown(bool CoolDown)
    {
        this->icdown = CoolDown;
    }

    void SimpleGridItem::AddOnClick(std::function<void() > Callback, u64 Key)
    {
        this->cbs.push_back(Callback);
        this->cbipts.push_back(Key);
    }

    s32 SimpleGridItem::GetCallbackCount()
    {
        return this->cbs.size();
    }

    std::function<void() > SimpleGridItem::GetCallback(s32 Index)
    {
        if (this->cbs.empty()) return [&]()
            {
            };
        return this->cbs[Index];
    }

    u64 SimpleGridItem::GetCallbackKey(s32 Index)
    {
        return this->cbipts[Index];
    }

    std::string SimpleGridItem::GetIcon()
    {
        return this->icon;
    }

    void SimpleGridItem::SetIcon(std::string Icon)
    {
        std::ifstream ifs(Icon);
        if (ifs.good())
        {
            this->icon = Icon;
            this->hasicon = true;
        }
        ifs.close();
    }

    bool SimpleGridItem::HasIcon()
    {
        return this->hasicon;
    }

    SimpleGrid::SimpleGrid(s32 X, s32 Y, bool rounded) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = 1280;
        this->clr = pu::ui::Color::FromHex("#ffffff");
        this->scb = {110, 110, 110, 255};
        this->isize = 256; // icon size
        this->ishow = 4; // number per rows
        this->rshow = 2;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->hspace = 15;
        this->vspace = 15;
        this->onselch = [&]()
        {
        };
        this->icdown = true;
        this->dtouch = false;
        this->fcs = {40, 40, 40, 255};
        this->basestatus = 0;
        this->_fontsize = 25;
        this->font = pu::ui::render::LoadDefaultFont(this->_fontsize);
        this->blinktime = std::chrono::steady_clock::now();
        this->_blink = true;
        this->_rounded = rounded;
        this->_txtonsel = false;
        this->_txtunder = false;
        this->_txtclr = {255, 255, 255, 255};
        this->_borderclr = {18, 187, 254, 255};
        this->_altclr = {115, 223, 235, 255};
        this->_baseclr = {61, 61, 61, 255};
        this->_innerclr = {28, 33, 37, 255};
    }

    s32 SimpleGrid::GetX()
    {
        return this->x;
    }

    void SimpleGrid::SetX(s32 X)
    {
        this->x = X;
    }

    s32 SimpleGrid::GetY()
    {
        return this->y;
    }

    void SimpleGrid::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 SimpleGrid::GetWidth()
    {
        return this->w;
    }

    void SimpleGrid::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 SimpleGrid::GetHeight()
    {
        return (this->isize * this->ishow);
    }

    s32 SimpleGrid::GetItemSize()
    {
        return this->isize;
    }

    void SimpleGrid::SetItemSize(s32 ItemSize)
    {
        this->isize = ItemSize;
    }

    s32 SimpleGrid::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void SimpleGrid::SetNumberOfItemsToShow(s32 ItemsToShow)
    {
        this->ishow = ItemsToShow;
    }

    void SimpleGrid::SetHorizontalPadding(s32 HorizontalPadding)
    {
        this->hspace = HorizontalPadding;
    }

    void SimpleGrid::SetVerticalPadding(s32 VerticalPadding)
    {
        this->vspace = VerticalPadding;
    }

    void SimpleGrid::SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor)
    {
        this->_txtclr = TextColor;
        this->_borderclr = BorderColor;
        this->_altclr = AltBorderColor;
        this->_innerclr = InnerBorderColor;
        this->_baseclr = BaseColor;
    }

    pu::ui::Color SimpleGrid::GetColor()
    {
        return this->clr;
    }

    void SimpleGrid::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
    }

    pu::ui::Color SimpleGrid::GetOnFocusColor()
    {
        return this->fcs;
    }

    void SimpleGrid::SetOnFocusColor(pu::ui::Color Color)
    {
        this->fcs = Color;
    }

    pu::ui::Color SimpleGrid::GetScrollbarColor()
    {
        return this->scb;
    }

    void SimpleGrid::SetScrollbarColor(pu::ui::Color Color)
    {
        this->scb = Color;
    }

    void SimpleGrid::SetOnSelectionChanged(std::function<void() > Callback)
    {
        this->onselch = Callback;
    }

    void SimpleGrid::SetDisplayNameOnSelected(bool txtonsel)
    {
        this->_txtonsel = txtonsel;
    }

    void SimpleGrid::SetTextUnderIcon(bool txtunder)
    {
        this->_txtunder = txtunder;
    }

    void SimpleGrid::SetFontSize(s32 fontsize)
    {
        this->_fontsize = fontsize;
        this->font = pu::ui::render::LoadDefaultFont(this->_fontsize);
    }

    void SimpleGrid::AddItem(SimpleGridItem::Ref &Item)
    {
        this->itms.push_back(Item);
    }

    void SimpleGrid::ClearItems()
    {
        this->itms.clear();
    }

    void SimpleGrid::SetCooldownEnabled(bool Cooldown)
    {
        this->icdown = Cooldown;
    }

    SimpleGridItem::Ref &SimpleGrid::GetSelectedItem()
    {
        return this->itms[this->isel];
    }

    std::vector<SimpleGridItem::Ref> &SimpleGrid::GetItems()
    {
        return this->itms;
    }

    s32 SimpleGrid::GetSelectedIndex()
    {
        return this->isel;
    }

    void SimpleGrid::SetSelectedIndex(s32 Index)
    {
        if (this->itms.size() > Index)
        {
            this->itms[this->previsel]->SetCoolDown(true);
            this->isel = Index;
            this->itms[this->isel]->SetCoolDown(false);
            this->previsel = Index;
            this->fisel = 0;
            if (this->isel >= (this->itms.size() - this->ishow)) this->fisel = this->itms.size() - this->ishow;
            else if (this->isel < this->ishow) this->fisel = 0;
            else this->fisel = this->isel;
        }
    }

    void SimpleGrid::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if (!this->itms.empty())
        {
            s32 cx = X;
            s32 cy = Y;
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 th = 40;
            s32 finInc = (this->fisel + (this->rshow * this->ishow)) > this->itms.size() ? this->itms.size() : this->fisel + (this->rshow * this->ishow);

            for (s32 i = this->fisel; i < finInc; i++)
            {
                s32 xcoeff = i - this->fisel;
                while (xcoeff >= this->ishow)
                    xcoeff -= this->ishow;
                s32 ycoeff = (i - this->fisel) / this->ishow;
                cx = X + (this->isize + this->hspace) * (xcoeff);
                cy = Y + (this->isize + this->vspace) * (ycoeff);
                if (cx >= 1280)
                    break;
                std::string strDisplay = "";
                s32 currentTextLength = pu::ui::render::GetTextWidth(this->font, this->itms[i]->GetName());
                if (currentTextLength > this->isize)
                {
                    s32 newLength = floor(((float) this->isize / currentTextLength) * this->itms[i]->GetName().length());
                    strDisplay = this->itms[i]->GetName().AsUTF8().substr(0, newLength);
                }
                else
                {
                    strDisplay = this->itms[i]->GetName().AsUTF8();
                }
                s32 rdx = cx;
                s32 rdy = cy;
                rdx = rdx + ((this->isize - pu::ui::render::GetTextWidth(this->font, strDisplay)) / 2);
                if (this->_txtunder)
                    rdy += this->isize + 20;
                else
                    rdy += this->isize - 50 + ((th - pu::ui::render::GetTextHeight(this->font, strDisplay)) / 2);
                if (this->isel == i)
                {
                    auto curtime = std::chrono::steady_clock::now();
                    //cy = cy-20;
                    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - this->blinktime).count();
                    if (diff >= 500)
                    {
                        this->_blink = !this->_blink;
                        this->blinktime = std::chrono::steady_clock::now();
                    }
                    if (this->_rounded)
                    {
                        if (this->_blink)
                        {
                            Drawer->RenderRoundedRectangleFill(this->_borderclr, cx - 10, cy - 10, ch + 20, ch + 20, 4);
                        }
                        else
                        {
                            Drawer->RenderRoundedRectangleFill(this->_altclr, cx - 10, cy - 10, ch + 20, ch + 20, 4);
                        }
                        Drawer->RenderRoundedRectangleFill(this->_innerclr, cx - 5, cy - 5, ch + 10, ch + 10, 4);
                    }
                    else
                    {
                        if (this->_blink)
                            Drawer->RenderTexture(GetBorder(this->_borderclr), cx - 10, cy - 10, ch + 20, ch + 20);
                        else
                            Drawer->RenderTexture(GetBorder(this->_altclr), cx - 10, cy - 10, ch + 20, ch + 20);
                        Drawer->RenderTexture(GetBorder(this->_innerclr), cx - 5, cy - 5, ch + 10, ch + 10);
                    }

                    Drawer->RenderTexture(GetBorder(this->_baseclr), cx, cy, ch, ch);
                    Drawer->RenderTexture(GetImageContent(this->itms[i]->GetImage().AsUTF8()), cx, cy, ch, ch);
                    if (!this->_txtunder)
                        Drawer->RenderTexture(GetBorder(this->_baseclr), cx, cy + this->isize - 50, this->isize, th);
                    Drawer->RenderTexture(GetTextContent(strDisplay), rdx, rdy);
                }
                else
                {
                    Drawer->RenderTexture(GetBorder(this->_baseclr), cx, cy, ch, ch);
                    Drawer->RenderTextureScaled(GetImageContent(this->itms[i]->GetImage().AsUTF8()), cx, cy, ch, ch);
                    if (!this->_txtunder)
                        Drawer->RenderTexture(GetBorder(this->_baseclr), cx, rdy, this->isize, th);
                    if (!this->_txtonsel)
                        Drawer->RenderTexture(GetTextContent(strDisplay), rdx, rdy);
                }
            }
            if ((this->rshow * this->ishow) < this->itms.size())
            {
                s32 scx = this->w - 30;
                s32 scy = Y;
                s32 scw = 5;
                s32 sch = (this->isize + this->vspace) / this->rshow;
                s32 ycoeff = this->isel / this->ishow;
                s32 fcy = scy + (ycoeff * sch);
                Drawer->RenderRectangleFill(this->scb, scx, fcy, scw, sch);
            }
        }
    }

    void SimpleGrid::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
        if (itms.empty()) return;
        if (Touch)
        {
            touchPosition tch;
            hidTouchRead(&tch, 0);
            s32 cx = this->GetProcessedX();
            s32 cy = this->GetProcessedY();
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->rshow * this->ishow;
            if (its > this->itms.size()) its = this->itms.size();
            if ((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            for (s32 i = this->fisel; i < (this->fisel + its); i++)
            {
                s32 xcoeff = i - this->fisel;
                while (xcoeff >= this->ishow)
                    xcoeff -= this->ishow;
                s32 ycoeff = (i - this->fisel) / this->ishow;
                s32 rdx = cx + (this->isize + this->hspace) * xcoeff;
                s32 rdy = cy + (this->isize + this->vspace) * ycoeff;
                if ((tch.px >= rdx) && (tch.py >= rdy) && (tch.px < (rdx + this->isize)) && (tch.py < (rdy + this->isize)))
                {
                    this->previsel = this->isel;
                    this->dtouch = true;
                    this->isel = i;
                    (this->onselch)();
                    break;
                }
            }
        }
        else if (this->dtouch)
        {
            if (this->itms[this->isel]->GetCoolDown())
            {
                this->itms[this->previsel]->SetCoolDown(true);
                this->itms[this->isel]->SetCoolDown(false);
            }
            else (this->itms[this->isel]->GetCallback(0))();
            this->dtouch = false;
        }
        else
        {
            if ((Down & KEY_DUP) || (Down & KEY_LSTICK_UP) || (Held & KEY_RSTICK_UP))
            {
                this->blinktime = std::chrono::steady_clock::now();
                this->_blink = true;
                this->previsel = this->isel;
                if (this->isel >= this->ishow)
                {
                    if ((this->isel - this->ishow) < (this->fisel))
                    {
                        this->fisel -= (this->rshow * this->ishow);
                    }
                    this->isel -= this->ishow;
                    (this->onselch)();
                    this->itms[this->previsel]->SetCoolDown(true);
                    this->itms[this->isel]->SetCoolDown(false);
                }
            }
            else if ((Down & KEY_DDOWN) || (Down & KEY_LSTICK_DOWN) || (Held & KEY_RSTICK_DOWN))
            {
                this->blinktime = std::chrono::steady_clock::now();
                this->_blink = true;
                this->previsel = this->isel;
                if ((this->isel + this->ishow) < this->itms.size())
                {
                    if ((this->isel + this->ishow) >= (this->fisel + (this->rshow * this->ishow)))
                    {
                        this->fisel += (this->rshow * this->ishow);
                    }
                    this->isel += this->ishow;
                    (this->onselch)();
                    this->itms[this->previsel]->SetCoolDown(true);
                    this->itms[this->isel]->SetCoolDown(false);
                }
            }
            else if ((Down & KEY_DRIGHT) || (Down & KEY_LSTICK_RIGHT) || (Held & KEY_RSTICK_RIGHT))
            {
                this->blinktime = std::chrono::steady_clock::now();
                this->_blink = true;
                this->previsel = this->isel;
                if (((this->isel + 1) % (this->ishow) != 0) && (this->isel < this->itms.size() - 1))
                {
                    this->isel++;
                    (this->onselch)();
                    this->itms[this->previsel]->SetCoolDown(true);
                    this->itms[this->isel]->SetCoolDown(false);
                }
            }
            else if ((Down & KEY_DLEFT) || (Down & KEY_LSTICK_LEFT) || (Held & KEY_RSTICK_LEFT))
            {
                this->blinktime = std::chrono::steady_clock::now();
                this->_blink = true;
                this->previsel = this->isel;
                if ((this->isel % this->ishow != 0) && (this->isel < (this->fisel + (this->rshow * this->ishow))))
                {
                    this->isel--;
                    (this->onselch)();
                    this->itms[this->previsel]->SetCoolDown(true);
                    this->itms[this->isel]->SetCoolDown(false);
                }
            }
            else
            {
                s32 ipc = this->itms[this->isel]->GetCallbackCount();
                if (ipc > 0) for (s32 i = 0; i < ipc; i++)
                    {
                        if (Down & this->itms[this->isel]->GetCallbackKey(i))
                        {
                            if (this->itms[this->isel]->GetCoolDown())
                            {
                                this->itms[this->previsel]->SetCoolDown(true);
                                this->itms[this->isel]->SetCoolDown(false);
                            }
                            else (this->itms[this->isel]->GetCallback(i))();
                        }
                    }
            }
        }
    }

    pu::ui::render::NativeTexture SimpleGrid::GetTextContent(std::string text)
    {
        if (_contents.count(text) == 0 || _contents[text] == NULL)
            _contents[text] = pu::ui::render::RenderText(this->font, text, this->_txtclr);
        return _contents[text];
    }

    pu::ui::render::NativeTexture SimpleGrid::GetImageContent(std::string imagePath)
    {
        if (_contents.count(imagePath) == 0 || _contents[imagePath] == NULL)
            _contents[imagePath] = pu::ui::render::LoadImage(imagePath);
        return _contents[imagePath];
    }

    pu::ui::render::NativeTexture SimpleGrid::GetBorder(pu::ui::Color color)
    {
        std::string currentKey = std::to_string(color.R) + "_" + std::to_string(color.G) + "_" + std::to_string(color.B) + "_" + std::to_string(color.A);
        if (_contents.count(currentKey) == 0 || _contents[currentKey] == NULL)
        {
            _contents[currentKey] = pu::ui::render::GetColorTexture(color);
        }
        return _contents[currentKey];
    }
}
#include <elm/elm_NinMenu.hpp>

namespace elm
{

    NinMenuItem::NinMenuItem(pu::String Name)
    {
        this->clr = {10, 10, 10, 255};
        this->name = Name;
        this->hasicon = false;
    }

    pu::String NinMenuItem::GetName()
    {
        return this->name;
    }

    void NinMenuItem::SetName(pu::String Name)
    {
        this->name = Name;
    }

    pu::ui::Color NinMenuItem::GetColor()
    {
        return this->clr;
    }

    void NinMenuItem::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
    }

    void NinMenuItem::AddOnClick(std::function<void() > Callback, u64 Key)
    {
        this->cbs.push_back(Callback);
        this->cbipts.push_back(Key);
    }

    s32 NinMenuItem::GetCallbackCount()
    {
        return this->cbs.size();
    }

    std::function<void() > NinMenuItem::GetCallback(s32 Index)
    {
        if (this->cbs.empty()) return [&]()
            {
            };
        return this->cbs[Index];
    }

    u64 NinMenuItem::GetCallbackKey(s32 Index)
    {
        return this->cbipts[Index];
    }

    std::string NinMenuItem::GetIcon()
    {
        return this->icon;
    }

    void NinMenuItem::SetIcon(std::string Icon)
    {
        std::ifstream ifs(Icon);
        if (ifs.good())
        {
            this->icon = Icon;
            this->hasicon = true;
        }
        ifs.close();
    }

    bool NinMenuItem::HasIcon()
    {
        return this->hasicon;
    }

    NinMenu::NinMenu(s32 X, s32 Y, s32 Width, pu::ui::Color OptionColor, s32 ItemSize, s32 ItemsToShow) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->w = Width;
        this->clr = OptionColor;
        this->scb = {110, 110, 110, 255};
        this->isize = ItemSize;
        this->ishow = ItemsToShow;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->onselch = [&]()
        {
        };
        this->icdown = false;
        this->dtouch = false;
        this->basestatus = 0;
        this->font = pu::ui::render::LoadDefaultFont(25);

        this->blinktime = std::chrono::steady_clock::now();
        this->_blink = true;
        this->_txtclr = {255, 255, 255, 255};
        this->_borderclr = {18, 187, 254, 255};
        this->_altclr = {115, 223, 235, 255};
        this->_baseclr = {61, 61, 61, 255};
        this->_innerclr = {28, 33, 37, 255};
        this->_lineclr = {45, 79, 238, 255};
    }

    s32 NinMenu::GetX()
    {
        return this->x;
    }

    void NinMenu::SetX(s32 X)
    {
        this->x = X;
    }

    s32 NinMenu::GetY()
    {
        return this->y;
    }

    void NinMenu::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 NinMenu::GetWidth()
    {
        return this->w;
    }

    void NinMenu::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 NinMenu::GetHeight()
    {
        return (this->isize * this->ishow);
    }

    s32 NinMenu::GetItemSize()
    {
        return this->isize;
    }

    void NinMenu::SetItemSize(s32 ItemSize)
    {
        this->isize = ItemSize;
    }

    s32 NinMenu::GetNumberOfItemsToShow()
    {
        return this->ishow;
    }

    void NinMenu::SetNumberOfItemsToShow(s32 ItemsToShow)
    {
        this->ishow = ItemsToShow;
    }

    pu::ui::Color NinMenu::GetColor()
    {
        return this->clr;
    }

    void NinMenu::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
    }

    void NinMenu::SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor, pu::ui::Color LineColor)
    {
        this->_txtclr = TextColor;
        this->_borderclr = BorderColor;
        this->_altclr = AltBorderColor;
        this->_innerclr = InnerBorderColor;
        this->_baseclr = BaseColor;
        this->_lineclr = LineColor;
    }
    
    void NinMenu::SetIsFocused(bool isFocus)
    {
        this->_isfocus = isFocus;
    }

    pu::ui::Color NinMenu::GetScrollbarColor()
    {
        return this->scb;
    }

    void NinMenu::SetScrollbarColor(pu::ui::Color Color)
    {
        this->scb = Color;
    }

    void NinMenu::SetOnSelectionChanged(std::function<void() > Callback)
    {
        this->onselch = Callback;
    }

    void NinMenu::AddItem(NinMenuItem::Ref &Item)
    {
        this->itms.push_back(Item);
    }

    void NinMenu::ClearItems()
    {
        this->itms.clear();
        for(auto &name : this->loadednames)
            pu::ui::render::DeleteTexture(name);
        this->loadednames.clear();
        for(auto &icon : this->loadedicons)
            if(icon != NULL)
                pu::ui::render::DeleteTexture(icon);
        this->loadedicons.clear();
    }

    void NinMenu::SetCooldownEnabled(bool Cooldown)
    {
        this->icdown = Cooldown;
    }

    NinMenuItem::Ref &NinMenu::GetSelectedItem()
    {
        return this->itms[this->isel];
    }

    std::vector<NinMenuItem::Ref> &NinMenu::GetItems()
    {
        return this->itms;
    }

    s32 NinMenu::GetSelectedIndex()
    {
        return this->isel;
    }

    void NinMenu::SetSelectedIndex(s32 Index)
    {
        if (this->itms.size() > Index)
        {
            this->isel = Index;
            this->fisel = 0;
            if (this->isel >= (this->itms.size() - this->ishow)) this->fisel = this->itms.size() - this->ishow;
            else if (this->isel < this->ishow) this->fisel = 0;
            else this->fisel = this->isel;

            ReloadItemRenders();
        }
    }

    void NinMenu::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        if (!this->itms.empty())
        {
            s32 cx = X;
            s32 cy = Y;
            s32 cw = this->w;
            s32 ch = this->isize;
            s32 its = this->ishow;
            if (its > this->itms.size()) its = this->itms.size();
            if ((its + this->fisel) > this->itms.size()) its = this->itms.size() - this->fisel;
            if (this->loadednames.empty()) ReloadItemRenders();
            for (s32 i = this->fisel; i < (its + this->fisel); i++)
            {
                s32 clrr = this->clr.R;
                s32 clrg = this->clr.G;
                s32 clrb = this->clr.B;
                s32 nr = clrr - 70;
                if (nr < 0) nr = 0;
                s32 ng = clrg - 70;
                if (ng < 0) ng = 0;
                s32 nb = clrb - 70;
                if (nb < 0) nb = 0;
                pu::ui::Color nclr(nr, ng, nb, this->clr.A);
                auto loadedidx = i - this->fisel;
                auto curname = GetTextContent(this->itms[i]->GetName().AsUTF8(), (this->isel == i));
                auto curicon = this->loadedicons[loadedidx];
                if (this->isel == i)
                {
                    if(this->_isfocus)
                    {
                        auto curtime = std::chrono::steady_clock::now();
                        //cy = cy-20;
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
                    Drawer->RenderRectangleFill(this->_lineclr, cx+8, cy+10, 3, ch-20);
                }
                //else Drawer->RenderRectangleFill(this->clr, cx, cy, cw, ch);
                auto itm = this->itms[i];
                s32 xh = pu::ui::render::GetTextHeight(this->font, itm->GetName());
                s32 tx = (cx + 25);
                s32 ty = ((ch - xh) / 2) + cy;
                if (itm->HasIcon())
                {
                    float factor = (float) pu::ui::render::GetTextureHeight(curicon) / (float) pu::ui::render::GetTextureWidth(curicon);
                    s32 icw = (this->isize - 10);
                    s32 ich = icw;
                    s32 icx = (cx + 25);
                    s32 icy = (cy + 5);
                    tx = (icx + icw + 25);
                    if (factor < 1)
                    {
                        ich = ich*factor;
                        icy = icy + ((this->isize - ich) / 2);
                    }
                    else
                    {
                        icw = icw / factor;
                        icx = icx + ((this->isize - icw) / 2);
                    }
                    Drawer->RenderTextureScaled(curicon, icx, icy, icw, ich);
                }
                Drawer->RenderTexture(curname, tx, ty);
                cy += ch;
            }
            if (this->ishow < this->itms.size())
            {
                s32 sccr = this->scb.R;
                s32 sccg = this->scb.G;
                s32 sccb = this->scb.B;
                s32 snr = sccr - 30;
                if (snr < 0) snr = 0;
                s32 sng = sccg - 30;
                if (sng < 0) sng = 0;
                s32 snb = sccb - 30;
                if (snb < 0) snb = 0;
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

    void NinMenu::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
        if (itms.empty()  || !this->_isfocus) return;
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
            s32 cx = this->GetProcessedX();
            s32 cy = this->GetProcessedY();
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
                    break;
                }
                cy += this->isize;
            }
        }
        else if (this->dtouch)
        {
            if (this->icdown) this->icdown = false;
            else (this->itms[this->isel]->GetCallback(0))();
            this->dtouch = false;
        }
        else
        {
            if ((Down & KEY_DDOWN) || (Down & KEY_LSTICK_DOWN) || (Held & KEY_RSTICK_DOWN))
            {
                bool move = true;
                if (Held & KEY_RSTICK_DOWN)
                {
                    move = false;
                    if (basestatus == 0)
                    {
                        basetime = std::chrono::steady_clock::now();
                        basestatus = 1;
                    }
                    else if (basestatus == 2)
                    {
                        basestatus = 0;
                        move = true;
                    }
                }
                if (move)
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
            }
            else if ((Down & KEY_DUP) || (Down & KEY_LSTICK_UP) || (Held & KEY_RSTICK_UP))
            {
                bool move = true;
                if (Held & KEY_RSTICK_UP)
                {
                    move = false;
                    if (basestatus == 0)
                    {
                        basetime = std::chrono::steady_clock::now();
                        basestatus = 1;
                    }
                    else if (basestatus == 2)
                    {
                        basestatus = 0;
                        move = true;
                    }
                }
                if (move)
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
            }
            else
            {
                s32 ipc = this->itms[this->isel]->GetCallbackCount();
                if (ipc > 0) for (s32 i = 0; i < ipc; i++)
                    {
                        if (Down & this->itms[this->isel]->GetCallbackKey(i))
                        {
                            if (this->icdown) this->icdown = false;
                            else (this->itms[this->isel]->GetCallback(i))();
                        }
                    }
            }
        }
    }
    
    pu::ui::render::NativeTexture NinMenu::GetTextContent(std::string text, bool selected)
    {
        std::string indexText = (selected) ? text + "_select" : text;
        if (_contents.count(indexText) == 0 || _contents[indexText] == NULL)
        {
            if(selected)
            {
                _contents[indexText] = pu::ui::render::RenderText(this->font, text, this->_lineclr);
            }
            else
                _contents[indexText] = pu::ui::render::RenderText(this->font, text, this->_txtclr);
        }
            
        return _contents[indexText];
    }

    void NinMenu::ReloadItemRenders()
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
            auto strname = this->itms[i]->GetName();
            auto tex = pu::ui::render::RenderText(this->font, strname, this->itms[i]->GetColor());
            this->loadednames.push_back(tex);
            if (this->itms[i]->HasIcon())
            {
                auto stricon = this->itms[i]->GetIcon();
                auto icontex = pu::ui::render::LoadImage(stricon);
                this->loadedicons.push_back(icontex);
            }
            else this->loadedicons.push_back(NULL);
        }
    }
}
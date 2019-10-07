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
#include <ui/ui_SliderDialog.hpp>
#include <IconTypes.hpp>
#include <Utils.hpp>

namespace ui
{   
    SliderDialog::SliderDialog(std::string Title, int MinValue, int MaxValue, int Step, int CurrentValue)
    {
        // Slider width : 600px
        // Slider height : 6px
        // Diametre : 42px
        // Border : 5px
        this->blinktime = std::chrono::steady_clock::now();
        this->_blink = true;
        this->_txtclr = {255, 255, 255, 255};
        this->_borderclr = {18, 187, 254, 255};
        this->_altclr = {115, 223, 235, 255};
        this->_baseclr = {70, 70, 70, 255};
        this->_lineclr = {106, 106, 106, 255};
        this->_innerclr = {58, 61, 66, 255};
        
        this->x = 340;
        this->y = 0;
        this->w = 600;
        this->h = 21;
        this->minValue = MinValue;
        this->maxValue = MaxValue;
        this->currentValue = CurrentValue;
        this->step = Step;
        this->currentXpos = 0;
        this->previsel = 0;
        this->isel = 0;
        this->fisel = 0;
        this->selfact = 255;
        this->pselfact = 0;
        this->onselch = [&](){};
        this->icdown = false;
        this->dtouch = false;
        this->font = pu::ui::render::LoadDefaultFont(25);
        this->icofont = pu::ui::render::LoadSharedIconFont(pu::ui::render::SharedFont::NintendoExtended, 25);
        this->title = Title;
        this->cancel = false;
        this->vtex = pu::ui::render::RenderText(this->font, std::to_string(this->currentValue), this->_bfocus);
        this->iconLaunch = elm::TextIcon::New(1220, 670, "Ok", Icons::Icon_A, this->_txtclr, elm::TextIconAlign::Right);
        this->iconMove = elm::TextIcon::New(this->iconLaunch->GetX() - 40, 670, "Move", Icons::Icon_JoystickR, this->_txtclr, elm::TextIconAlign::Right);
        this->iconCancel = elm::TextIcon::New(this->iconMove->GetX() - 40, 670, "Cancel", Icons::Icon_B, this->_txtclr, elm::TextIconAlign::Right);
    }
    
    SliderDialog::~SliderDialog()
    {
        pu::ui::render::DeleteTexture(this->vtex);
    }

    s32 SliderDialog::GetX()
    {
        return this->x;
    }

    void SliderDialog::SetX(s32 X)
    {
        this->x = X;
    }

    s32 SliderDialog::GetY()
    {
        return this->y;
    }

    void SliderDialog::SetY(s32 Y)
    {
        this->y = Y;
    }
    
    void SliderDialog::SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BorderColor, pu::ui::Color AltBorderColor, pu::ui::Color InnerBorderColor, pu::ui::Color BaseColor, pu::ui::Color LineColor, pu::ui::Color BaseFocus)
    {
        this->_txtclr = TextColor;
        this->_borderclr = BorderColor;
        this->_altclr = AltBorderColor;
        this->_innerclr = InnerBorderColor;
        this->_baseclr = BaseColor;
        this->_lineclr = LineColor;
        this->_bfocus = BaseFocus;
        this->iconLaunch->SetColor(this->_txtclr);
        this->iconMove->SetColor(this->_txtclr);
        this->iconCancel->SetColor(this->_txtclr);
        pu::ui::render::DeleteTexture(this->vtex);
        this->vtex = pu::ui::render::RenderText(this->font, std::to_string(this->currentValue), this->_bfocus);
    }

    void SliderDialog::SetOnSelectionChanged(std::function<void()> Callback)
    {
        this->onselch = Callback;
    }

    int SliderDialog::Show(pu::ui::render::Renderer::Ref &Drawer, void *App)
    {
        pu::ui::Color ovclr({this->_baseclr.R, this->_baseclr.G, this->_baseclr.B, 140});
        this->SetY(720 - (73 /*bottom buttons*/ + 10*2 /*padding before/after elements*/ + 72 /*top text title*/ + 60));
        auto titletex = pu::ui::render::RenderText(this->font, this->title, this->_txtclr);
        s32 tth = pu::ui::render::GetTextHeight(this->font, this->title);
        s32 telmy = this->GetY()+11 + ((60 - tth) / 2);
        bool end = false;
        while(true)
        {
            bool ok = ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref &Drawer) -> bool
            {
                pu::ui::render::DeleteTexture(this->vtex);
                this->currentXpos = ((float)this->w/this->maxValue)*this->currentValue;
                this->vtex = pu::ui::render::RenderText(this->font, std::to_string(this->currentValue), this->_bfocus);
                s32 ytex = this->GetY() + 72 + ((60 - pu::ui::render::GetTextHeight(this->font, std::to_string(this->currentValue))) / 2);
                //Global overlay
                Drawer->RenderRectangleFill(ovclr, 0, 0, 1280, 720);
                //Popup back color
                Drawer->RenderRectangleFill(this->_baseclr, 0, this->GetY(), 1280, 720-this->GetY());
                //Top Separator Line
                Drawer->RenderLine(this->_txtclr, 30, this->GetY() + 71, 1250, this->GetY() + 71);
                //Bottom Separator Line
                Drawer->RenderLine(this->_txtclr, 30, 647, 1250, 647);
                //Title Text
                Drawer->RenderTexture(titletex, 72, telmy);
                // Top element separator
                Drawer->RenderRectangleFill(this->_bfocus, this->GetX(), 604, this->currentXpos, 6);
                Drawer->RenderRectangleFill(this->_lineclr, this->GetX() + this->currentXpos, 604, 600 - this->currentXpos, 6);
                auto curtime = std::chrono::steady_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - this->blinktime).count();
                if (diff >= 500)
                {
                    this->_blink = !this->_blink;
                    this->blinktime = std::chrono::steady_clock::now();
                }
                if (this->_blink)
                {
                    Drawer->RenderCircleFill(this->_borderclr, this->GetX() + this->currentXpos, 607, 26);
                }
                else
                {
                    Drawer->RenderCircleFill(this->_altclr, this->GetX() + this->currentXpos, 607, 26);
                }
                Drawer->RenderCircleFill(this->_innerclr, this->GetX() + this->currentXpos, 607, 21);
                Drawer->RenderTexture(this->vtex, this->GetX()+625, ytex);
                u64 d = hidKeysDown(CONTROLLER_P1_AUTO);
                u64 h = hidKeysHeld(CONTROLLER_P1_AUTO);
                if((d & KEY_DRIGHT) || (h & KEY_RSTICK_RIGHT))
                {
                    end = false;
                    this->cancel = false;
                    if(this->currentValue<this->maxValue)
                        this->currentValue+=this->step;
                }
                else if((d & KEY_DLEFT) || (h & KEY_RSTICK_LEFT))
                {
                    end = false;
                    this->cancel = false;
                    if(this->currentValue>this->minValue)
                        this->currentValue-=this->step;
                }
                else if(d & KEY_A)
                {
                    end = true;
                    this->cancel = false;
                }
                else if(d & KEY_B)
                {
                    end = true;
                    this->cancel = true;
                }
                u64 th = hidKeysDown(CONTROLLER_HANDHELD);
                this->iconLaunch->OnRender(Drawer, this->iconLaunch->GetX(), this->iconLaunch->GetY());
                this->iconMove->OnRender(Drawer, this->iconMove->GetX(), this->iconMove->GetY());
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
        return this->currentValue;
    }
    
    bool SliderDialog::UserCancelled()
    {
        return this->cancel;
    }
}
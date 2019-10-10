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
#include <ui/ui_TimeDialog.hpp>
#include <IconTypes.hpp>
#include <Utils.hpp>

namespace ui
{

    TimeDialog::TimeDialog(std::string DialogText, s32 Delay)
    {
        this->dialogText = DialogText;
        this->initialDelay = Delay;
        this->txtcounter = std::to_string(this->initialDelay / 1000);
        this->delaytime = std::chrono::steady_clock::now();
        this->_txtclr = {255, 255, 255, 255};

        this->x = 0;
        this->y = 129;
        this->w = 1280;
        this->h = 0;
        
        this->dtouch = false;
        this->font = pu::ui::render::LoadDefaultFont(25);
        this->cancel = false;
    }

    TimeDialog::~TimeDialog()
    {
        if(this->texdialog != NULL)
            pu::ui::render::DeleteTexture(this->texdialog);
    }

    s32 TimeDialog::GetX()
    {
        return this->x;
    }

    void TimeDialog::SetX(s32 X)
    {
        this->x = X;
    }

    s32 TimeDialog::GetY()
    {
        return this->y;
    }

    void TimeDialog::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 TimeDialog::GetWidth()
    {
        return this->w;
    }

    void TimeDialog::SetWidth(s32 Width)
    {
        this->w = Width;
    }

    s32 TimeDialog::GetHeight()
    {
        return this->h;
    }

    void TimeDialog::SetColorScheme(pu::ui::Color TextColor, pu::ui::Color BaseColor, pu::ui::Color BaseFocus)
    {
        this->_txtclr = TextColor;
        this->_bfocus = BaseFocus;
        this->_baseclr = BaseColor;
        if(this->texdialog != NULL) pu::ui::render::DeleteTexture(this->texdialog);
        this->texdialog = pu::ui::render::RenderText(this->font, this->dialogText, this->_bfocus);
    }

    s32 TimeDialog::Show(pu::ui::render::Renderer::Ref &Drawer, void *App)
    {
        pu::ui::Color ovclr({this->_baseclr.R, this->_baseclr.G, this->_baseclr.B, 210});
        std::string preText = "Autobooting:";
        std::string infoText = "Press any key or touch screen to cancel.";
        pu::ui::render::NativeTexture pretex = NULL;
        pretex = pu::ui::render::RenderText(this->font, preText, this->_txtclr);
        s32 ptx = (1280 - pu::ui::render::GetTextWidth(this->font, preText)) /2;
        pu::ui::render::NativeTexture infotex = NULL;
        infotex = pu::ui::render::RenderText(this->font, infoText, pu::ui::Color(218, 11, 11,255));
        s32 itx = (1280 - pu::ui::render::GetTextWidth(this->font, infoText)) /2;
        pu::ui::render::NativeTexture delaytex = NULL;
        s32 dtx = (1280 - pu::ui::render::GetTextWidth(this->font, this->dialogText)) /2;
        s32 dty = (720 - pu::ui::render::GetTextHeight(this->font, this->dialogText)) /2;
        bool end = false;
        while (true)
        {
            bool ok = ((pu::ui::Application*)App)->CallForRenderWithRenderOver([&](pu::ui::render::Renderer::Ref & Drawer) -> bool
            {
                //Global overlay
                Drawer->RenderRectangleFill(ovclr, 0, 0, 1280, 720);
                
                Drawer->RenderTexture(pretex, ptx, dty - 40);
                Drawer->RenderTexture(texdialog, dtx, dty);
                Drawer->RenderTexture(infotex, itx, dty + 40);
                if(delaytex != NULL) pu::ui::render::DeleteTexture(delaytex);
                delaytex = pu::ui::render::RenderText(this->font, this->txtcounter, this->_bfocus);
                s32 ctx = (1280 - pu::ui::render::GetTextWidth(this->font, this->txtcounter)) /2;
                s32 cty = dty + 80;
                Drawer->RenderTexture(delaytex, ctx, cty);
                
                u64 d = hidKeysDown(CONTROLLER_P1_AUTO);
                u64 u = hidKeysUp(CONTROLLER_P1_AUTO);
                u64 h = hidKeysHeld(CONTROLLER_P1_AUTO);
                u64 th = hidKeysDown(CONTROLLER_HANDHELD);
                auto curtime = std::chrono::steady_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - this->delaytime).count();
                if ((d) || (u) || (h) || (th))
                {
                    end = true;
                    this->cancel = true;
                }
                else if(diff >= this->initialDelay)
                {
                    end = true;
                    this->cancel = false;
                }
                else
                {
                    this->txtcounter = std::to_string((this->initialDelay - diff) / 1000);
                }
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
        return 0;
    }

    bool TimeDialog::UserCancelled()
    {
        return this->cancel;
    }
}
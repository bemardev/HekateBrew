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
#include <elm/elm_TextScroll.hpp>
#include <Utils.hpp>

namespace elm
{

    TextScroll::TextScroll(s32 X, s32 Y, s32 Width, pu::String Text, s32 FontSize) : Element::Element()
    {
        this->outputW = Width;
        this->x = X;
        this->y = Y;
        this->text = Text;
        this->currentText = this->text.AsUTF8();
        this->clr = {0, 0, 0, 255};
        this->fnt = pu::ui::render::LoadDefaultFont(FontSize);
        this->fsize = FontSize;
        if (pu::ui::render::GetTextWidth(this->fnt, this->currentText) > Width)
            this->currentText += "\t";
        this->ntex = pu::ui::render::RenderText(this->fnt, currentText, this->clr);
        this->start_time = std::chrono::high_resolution_clock::now();
        this->newLength = floor(((float) Width / pu::ui::render::GetTextWidth(this->fnt, this->currentText)) * Text.length());
    }

    TextScroll::~TextScroll()
    {
        if (this->ntex != NULL)
        {
            pu::ui::render::DeleteTexture(this->ntex);
            this->ntex = NULL;
        }
    }

    s32 TextScroll::GetX()
    {
        return this->x;
    }

    void TextScroll::SetX(s32 X)
    {
        this->x = X;
    }

    s32 TextScroll::GetY()
    {
        return this->y;
    }

    void TextScroll::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 TextScroll::GetWidth()
    {
        return this->GetTextWidth();
    }

    s32 TextScroll::GetHeight()
    {
        return this->GetTextHeight();
    }

    s32 TextScroll::GetTextWidth()
    {
        return pu::ui::render::GetTextWidth(this->fnt, this->text);
    }

    s32 TextScroll::GetTextHeight()
    {
        return pu::ui::render::GetTextHeight(this->fnt, this->text);
    }

    pu::String TextScroll::GetText()
    {
        return this->text;
    }

    void TextScroll::SetText(pu::String Text)
    {
        this->text = Text;
        pu::ui::render::DeleteTexture(this->ntex);
        this->ntex = pu::ui::render::RenderText(this->fnt, Text, this->clr);
    }

    void TextScroll::SetFont(pu::ui::render::NativeFont Font)
    {
        this->fnt = Font;
        pu::ui::render::DeleteTexture(this->ntex);
        this->ntex = pu::ui::render::RenderText(Font, this->text, this->clr);
    }

    pu::ui::Color TextScroll::GetColor()
    {
        return this->clr;
    }

    void TextScroll::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
        pu::ui::render::DeleteTexture(this->ntex);
        this->ntex = pu::ui::render::RenderText(this->fnt, this->text, Color);
    }

    void TextScroll::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        s32 rdx = X;
        s32 rdy = Y;
        this->current_time = std::chrono::high_resolution_clock::now();
        this->Elapsed = current_time - start_time;

        if (pu::ui::render::GetTextWidth(this->fnt, this->currentText) > this->outputW)
        {
            pu::ui::render::DeleteTexture(this->ntex);
            this->ntex = pu::ui::render::RenderText(this->fnt, this->currentText.substr(0, this->newLength), this->clr);
            if (this->Elapsed.count() >= 150)
            {
                stringShift(this->currentText);
                this->start_time = std::chrono::high_resolution_clock::now();
            }
        }
        rdx = rdx + ((this->outputW - pu::ui::render::GetTextWidth(this->fnt, this->currentText.substr(0, this->newLength))) / 2);
        Drawer->RenderTexture(this->ntex, rdx, rdy);
    }

    void TextScroll::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
    }
}
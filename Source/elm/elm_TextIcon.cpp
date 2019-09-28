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
#include <elm/elm_TextIcon.hpp>

namespace elm
{

    TextIcon::TextIcon(s32 X, s32 Y, pu::String Text, Icons IconType, pu::ui::Color Color, TextIconAlign Align, s32 FontSize) : Element::Element()
    {
        this->x = X;
        this->y = Y;
        this->text = Text;
        this->iconType = IconType;
        this->align = Align;
        this->clr = Color;
        this->fsize = FontSize;
        this->padding = 12;
        this->fnt = pu::ui::render::LoadDefaultFont(FontSize);
        this->icofnt = pu::ui::render::LoadSharedIconFont(pu::ui::render::SharedFont::NintendoExtended, 25);
        this->ntex = NULL;
        this->itex = NULL;
        this->ReloadTextures();
    }

    TextIcon::~TextIcon()
    {
        if (this->ntex != NULL)
        {
            pu::ui::render::DeleteTexture(this->ntex);
            this->ntex = NULL;
        }
        if (this->itex != NULL)
        {
            pu::ui::render::DeleteTexture(this->itex);
            this->itex = NULL;
        }
    }

    s32 TextIcon::GetX()
    {
        return this->x;
    }

    void TextIcon::SetX(s32 X)
    {
        this->x = X;
    }

    s32 TextIcon::GetY()
    {
        return this->y;
    }

    void TextIcon::SetY(s32 Y)
    {
        this->y = Y;
    }

    s32 TextIcon::GetWidth()
    {
        return this->GetTextWidth();
    }

    s32 TextIcon::GetHeight()
    {
        return this->GetTextHeight();
    }

    s32 TextIcon::GetTextWidth()
    {
        return pu::ui::render::GetTextWidth(this->fnt, this->text) + 30 /*harcoded*/ + this->padding;
    }

    s32 TextIcon::GetTextHeight()
    {
        return pu::ui::render::GetTextHeight(this->fnt, this->text);
    }

    pu::String TextIcon::GetText()
    {
        return this->text;
    }

    void TextIcon::SetText(pu::String Text, Icons IconType)
    {
        this->text = Text;
        this->iconType = IconType;
        this->ReloadTextures();
    }

    pu::ui::Color TextIcon::GetColor()
    {
        return this->clr;
    }

    void TextIcon::SetColor(pu::ui::Color Color)
    {
        this->clr = Color;
        this->ReloadTextures();
    }

    void TextIcon::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        s32 rdx = X;
        s32 rdy = Y;
        Drawer->RenderTexture(this->itex, rdx, rdy);
        Drawer->RenderTexture(this->ntex, rdx + 30 /*harcoded*/ + this->padding, rdy);
    }

    void TextIcon::OnInput(u64 Down, u64 Up, u64 Held, bool Touch)
    {
    }

    void TextIcon::ReloadTextures()
    {
        if (this->ntex != NULL)
            pu::ui::render::DeleteTexture(this->ntex);
        if (this->itex != NULL)
            pu::ui::render::DeleteTexture(this->itex);
        this->ntex = pu::ui::render::RenderText(this->fnt, this->text, this->clr);
        this->itex = pu::ui::render::RenderIcon(this->icofnt, this->iconType, this->clr);
        switch (this->align)
        {
        case(TextIconAlign::Right):
            this->x -= this->GetTextWidth();
            break;
        default:
            break;
        }
    }
}
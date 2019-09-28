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
#include <ui/ui_LoadingOverlay.hpp>

namespace ui
{

    LoadingOverlay::LoadingOverlay(pu::String imagePath, pu::ui::Color BaseColor) : Overlay(0, 0, 1280, 720, BaseColor)
    {
        // old overlay constructor : Overlay(550, 270, 180, 180, BaseColor) and image at 0,0
        this->Loader = elm::RotatableImage::New(550, 270, imagePath, 10, 50);
        this->tmpImg = pu::ui::elm::Image::New(615, 385, imagePath);
        this->Add(this->Loader);
        //this->Add(this->tmpImg);
    }

    void LoadingOverlay::OnPreRender(pu::ui::render::Renderer::Ref &Drawer)
    {
        Drawer->SetBaseRenderAlpha(200);
    }

    void LoadingOverlay::OnPostRender(pu::ui::render::Renderer::Ref &Drawer)
    {
        Drawer->UnsetBaseRenderAlpha();
    }
}
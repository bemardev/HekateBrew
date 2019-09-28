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
#pragma once
#include <pu/Plutonium>
#include <elm/elm_RotatableImage.hpp>

namespace ui
{
    class LoadingOverlay final : public pu::ui::Overlay
    {
    public:
        LoadingOverlay(pu::String imagePath, pu::ui::Color BaseColor);
        PU_SMART_CTOR(LoadingOverlay)

        void OnPreRender(pu::ui::render::Renderer::Ref &Drawer);
        void OnPostRender(pu::ui::render::Renderer::Ref &Drawer);
    private:
        elm::RotatableImage::Ref Loader;
        pu::ui::elm::Image::Ref tmpImg;
    };
}
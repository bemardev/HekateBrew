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
#include <elm/elm_RotatableImage.hpp>

namespace elm
{

    RotatableImage::RotatableImage(s32 X, s32 Y, pu::String Image, s32 angledelta, s32 delay) : Image::Image(X, Y, Image)
    {
        this->_angle = 0;
        this->_delay = delay;
        this->_delta = angledelta;
        this->delaytime = std::chrono::steady_clock::now();
    }

    void RotatableImage::OnRender(pu::ui::render::Renderer::Ref &Drawer, s32 X, s32 Y)
    {
        s32 rdx = X;
        s32 rdy = Y;
        auto curtime = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(curtime - this->delaytime).count();
        if (diff >= this->_delay)
        {
            this->_angle += this->_delta;
            this->delaytime = std::chrono::steady_clock::now();
        }
        Drawer->RenderTextureRotate(this->ntex, rdx, rdy, this->GetWidth(), this->GetHeight(), this->_angle);
        //Drawer->RenderTextureScaled(this->ntex, rdx, rdy, this->GetWidth(), this->GetHeight());
    }
}
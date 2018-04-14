/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Magnum/Image.h"
#include "Magnum/PixelFormat.h"
#ifdef MAGNUM_TARGET_GL
#include "Magnum/GL/PixelFormat.h"
#include "Magnum/GL/Texture.h"
#endif

using namespace Magnum;
using namespace Magnum::Math::Literals;

int main() {

{
char data[3];
/* [ImageView-usage] */
ImageView2D image{PixelFormat::RGBA8Unorm, {512, 256}, data};
/* [ImageView-usage] */
}

{
char evenFrameData[3], oddFrameData[3];
/* [ImageView-usage-streaming] */
ImageView2D frame{PixelFormat::RGBA8Unorm, {512, 256}};

frame.setData(evenFrameData);
// Use even frame data ...

frame.setData(oddFrameData);
// Use odd frame data ...
/* [ImageView-usage-streaming] */
}

{
char data[3];
/* [ImageView-usage-storage] */
ImageView2D image{
    PixelStorage{}
        .setRowLength(75)
        .setAlignment(4)
        .setSkip({25, 25, 0}),
    PixelFormat::RGBA8Unorm, {25, 25}, data};
/* [ImageView-usage-storage] */
}

#ifdef MAGNUM_TARGET_GL
{
char data[3];
/* [ImageView-usage-gl] */
ImageView2D image{GL::PixelFormat::DepthComponent,
                  GL::PixelType::UnsignedInt, {512, 256}, data};
/* [ImageView-usage-gl] */

/* [ImageView-usage-gl-extract] */
auto format = pixelFormatUnpack<GLenum>(image.format());
auto type = GLenum(image.formatExtra());
/* [ImageView-usage-gl-extract] */
}
#endif

{
char data[3];
#define MTLPixelFormatRGBA8Unorm_sRGB 71
/* [ImageView-usage-metal] */
/* 8-bit sRGB + alpha, four bytes per pixel */
ImageView2D view{MTLPixelFormatRGBA8Unorm_sRGB, {}, 4, {256, 256}, data};
/* [ImageView-usage-metal] */
}

{
/* [ImageView-usage] */
Containers::Array<char> data;
Image2D image{PixelFormat::RGBA8Unorm, {512, 256}, std::move(data)};
/* [ImageView-usage] */
}

{
/* [ImageView-usage-query] */
GL::Texture2D texture;
Image2D image = texture.image(0, {GL::PixelFormat::DepthComponent,
                                  GL::PixelType::UnsignedInt});
/* [ImageView-usage-query] */
}

}

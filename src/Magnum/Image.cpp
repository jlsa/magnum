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

#include "Image.h"

#include "Magnum/PixelFormat.h"

namespace Magnum {

template<UnsignedInt dimensions> Image<dimensions>::Image(PixelStorage storage, PixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::Array<char>&& data) noexcept: _storage{storage}, _format{format}, _formatExtra{}, _pixelSize{Magnum::pixelSize(format)}, _size{size}, _data{std::move(data)} {
    CORRADE_ASSERT(Implementation::imageDataSize(*this) <= _data.size(), "Image::Image(): bad image data size, got" << _data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
}

template<UnsignedInt dimensions> Image<dimensions>::Image(PixelStorage storage, UnsignedInt format, UnsignedInt formatExtra, UnsignedInt pixelSize, const VectorTypeFor<dimensions, Int>& size, Containers::Array<char>&& data) noexcept: _storage{storage}, _format{pixelFormatWrap(format)}, _formatExtra{formatExtra}, _pixelSize{pixelSize}, _size{size}, _data{std::move(data)} {
    CORRADE_ASSERT(Implementation::imageDataSize(*this) <= _data.size(), "Image::Image(): bad image data size, got" << _data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
}

template<UnsignedInt dimensions> Image<dimensions>::Image(PixelStorage storage, PixelFormat format) noexcept: _storage{storage}, _format{format}, _formatExtra{}, _pixelSize{Magnum::pixelSize(format)}, _data{} {}

template<UnsignedInt dimensions> CompressedImage<dimensions>::CompressedImage(const CompressedPixelStorage storage, const CompressedPixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::Array<char>&& data) noexcept: _storage{storage}, _format{format}, _size{size}, _data{std::move(data)} {}

template<UnsignedInt dimensions> CompressedImage<dimensions>::CompressedImage(CompressedPixelStorage storage, UnsignedInt format, const VectorTypeFor<dimensions, Int>& size, Containers::Array<char>&& data) noexcept: CompressedImage{storage, compressedPixelFormatWrap(format), size, std::move(data)} {}

#ifndef DOXYGEN_GENERATING_OUTPUT
template class MAGNUM_GL_EXPORT Image<1>;
template class MAGNUM_GL_EXPORT Image<2>;
template class MAGNUM_GL_EXPORT Image<3>;

template class MAGNUM_GL_EXPORT CompressedImage<1>;
template class MAGNUM_GL_EXPORT CompressedImage<2>;
template class MAGNUM_GL_EXPORT CompressedImage<3>;
#endif

}

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

#include "ImageView.h"

namespace Magnum {

template<UnsignedInt dimensions> ImageView<dimensions>::ImageView(PixelStorage storage, GL::PixelFormat format, GL::PixelType type, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept: _storage{storage}, _format{format}, _type{type}, _size{size}, _data{reinterpret_cast<const char*>(data.data()), data.size()} {
    CORRADE_ASSERT(!_data || Implementation::imageDataSize(*this) <= _data.size(), "ImageView::ImageView(): bad image data size, got" << _data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
}

template<UnsignedInt dimensions> void ImageView<dimensions>::setData(const Containers::ArrayView<const void> data) {
    CORRADE_ASSERT(Implementation::imageDataSize(*this) <= data.size(), "ImageView::setData(): bad image data size, got" << data.size() << "but expected at least" << Implementation::imageDataSize(*this), );
    _data = {reinterpret_cast<const char*>(data.data()), data.size()};
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template class MAGNUM_EXPORT ImageView<1>;
template class MAGNUM_EXPORT ImageView<2>;
template class MAGNUM_EXPORT ImageView<3>;
#endif

}

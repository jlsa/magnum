#ifndef Magnum_ImageView_h
#define Magnum_ImageView_h
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

/** @file
 * @brief Class @ref Magnum::ImageView, @ref Magnum::CompressedImageView, typedef @ref Magnum::ImageView1D, @ref Magnum::ImageView2D, @ref Magnum::ImageView3D, @ref Magnum::CompressedImageView1D, @ref Magnum::CompressedImageView2D, @ref Magnum::CompressedImageView3D
 */

#include <Corrade/Containers/ArrayView.h>

#include "Magnum/DimensionTraits.h"
#include "Magnum/PixelStorage.h"
#include "Magnum/Math/Vector4.h"

namespace Magnum {

/**
@brief Image view

Non-owning view on multi-dimensional image data together with layout and pixel
format description. Unlike @ref Image, this class doesn't delete the data on
destruction, so it is targeted for wrapping data which is either stored in
stack/constant memory (and shouldn't be deleted) or is managed by something
else.

This class can act as drop-in replacement for @ref Image or
@ref Trade::ImageData, these two are additionally convertible to it. Paricular
graphics API wrappers provide additional image classes, for example
@ref GL::BufferImage. See also @ref CompressedImageView for equivalent
functionality targeted on compressed image formats.

@section ImageView-usage Basic usage

Usually, the view is created on some pre-existing data array in order to
describe its layout, with pixel format being one of the values from the generic
@link PixelFormat @endlink:

@snippet Magnum.cpp ImageView-usage

On construction, the image view internally calculates pixel size corresponding
to given pixel format using @ref pixelSize(). This value is needed to check
that the passed data are large enough and also required by most of image
manipulation operations.

It's also possible to create an empty view and assign the memory later. That is
useful for example in case of multi-buffered video streaming, where each frame
has the same properties but a different memory location:

@snippet Magnum.cpp ImageView-usage-streaming

It's possible to have views on image sub-rectangles, 3D texture slices or
images with over-aligned rows by passing a particular @ref PixelStorage as
first parameter. In the following snippet, the view is the center 25x25
sub-rectangle of a 75x75 8-bit RGB image , with rows aligned to four bytes:

@snippet Magnum.cpp ImageView-usage-storage

@subsection ImageView-usage-implementation-specific Implementation-specific formats

For known graphics APIs, there's a set of utility functions converting from
@ref PixelFormat to implementation-specific format identifiers and such
conversion is done implicitly when passing the view to a particular API. See
the enum documentation and documentation of its values for more information.

In some cases, for example when there's no corresponding generic format
available, it's desirable to specify the pixel format using
implementation-specific identifiers directly. In case of OpenGL that would be
the @ref GL::PixelFormat and @ref GL::PixelType pair:

@snippet Magnum.cpp ImageView-usage-gl

In such cases, pixel size is calculated using either @cpp pixelSize(T, U) @ce
or @cpp pixelSize(T) @ce that is found using
[ADL](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup), with
@cpp T @ce and @cpp U @ce corresponding to types of passed arguments. The
implementation-specific format is wrapped in @ref PixelFormat using
@ref pixelFormatWrap() and @ref format() returns the wrapped value. In order to
distinguish if the format is wrapped, use @ref isPixelFormatImplementationSpecific()
and then extract the implementation-specific identifier using @ref pixelFormatUnwrap().
For APIs that have an additional format specifier (such as OpenGL), the second
value is stored verbatim in @ref formatExtra():

@snippet Magnum.cpp ImageView-usage-gl-extract

As a final fallback, types for which the @cpp pixelSize() @ce overload is not
available can be specified directly together with pixel size. In particular,
pixel size of @cpp 0 @ce will cause the image to be treated as fully opaque
data, disabling all slicing operations. The following shows a image view using
Metal-specific format identifier:

@snippet Magnum.cpp ImageView-usage-metal

@see @ref ImageView1D, @ref ImageView2D, @ref ImageView3D
*/
template<UnsignedInt dimensions> class ImageView {
    public:
        enum: UnsignedInt {
            Dimensions = dimensions /**< Image dimension count */
        };

        /**
         * @brief Constructor
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * The @p data are expected to be of proper size for given parameters.
         */
        explicit ImageView(PixelStorage storage, PixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Constructor
         * @param format            Format of pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to calling @ref ImageView(PixelStorage, PixelFormat, const VectorTypeFor<dimensions, Int>&, Containers::ArrayView<const void>)
         * with default-constructed @ref PixelStorage.
         */
        explicit ImageView(PixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept: ImageView{{}, format, size, data} {}

        /**
         * @brief Constructor
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param size              Image size
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        explicit ImageView(PixelStorage storage, PixelFormat format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /** @overload
         * @param format            Format of pixel data
         * @param size              Image size
         *
         * Equivalent to calling @ref ImageView(PixelStorage, PixelFormat, const VectorTypeFor<dimensions, Int>&)
         * with default-constructed @ref PixelStorage.
         */
        explicit ImageView(PixelFormat format, const VectorTypeFor<dimensions, Int>& size) noexcept: ImageView{{}, format, size} {}

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param pixelSize         Size of a pixel in given format
         * @param size              Image size
         * @param data              Image data
         *
         * Unlike with @ref ImageView(PixelStorage, PixelFormat, const VectorTypeFor<dimensions, Int>&, Containers::ArrayView<const void>),
         * where pixel size is calculated automatically using
         * @ref pixelSize(PixelFormat), this allows you to specify an
         * implementation-specific pixel format and pixel format directly. Uses
         * @ref pixelFormatWrap() internally to convert @p format to
         * @ref PixelFormat.
         *
         * The @p data are expected to be of proper size for given parameters.
         */
        explicit ImageView(PixelStorage storage, UnsignedInt format, UnsignedInt formatExtra, UnsignedInt pixelSize, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /** @overload
         * Equivalent to the above for @p format already wrapped with
         * @ref pixelFormatWrap().
         */
        explicit ImageView(PixelStorage storage, PixelFormat format, UnsignedInt formatExtra, UnsignedInt pixelSize, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param pixelSize         Size of a pixel in given format
         * @param size              Image size
         *
         * Unlike with @ref ImageView(PixelStorage, PixelFormat, const VectorTypeFor<dimensions, Int>&),
         * where pixel size is calculated automatically using
         * @ref pixelSize(PixelFormat), this allows you to specify an
         * implementation-specific pixel format and pixel format directly. Uses
         * @ref pixelFormatWrap() internally to convert @p format to
         * @ref PixelFormat.
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        explicit ImageView(PixelStorage storage, UnsignedInt format, UnsignedInt formatExtra, UnsignedInt pixelSize, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /** @overload
         * Equivalent to the above for @p format already wrapped with
         * @ref pixelFormatWrap().
         */
        explicit ImageView(PixelStorage storage, PixelFormat format, UnsignedInt formatExtra, UnsignedInt pixelSize, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param size              Image size
         * @param data              Image data
         *
         * Uses ADL to find a corresponding @cpp pixelSize(T, U) @ce overload,
         * then calls @ref ImageView(PixelStorage, UnsignedInt, UnsignedInt, UnsignedInt, const VectorTypeFor<dimensions, T>&, Containers::ArrayView<const void>)
         * with calculated pixel size.
         */
        template<class T, class U> explicit ImageView(PixelStorage storage, T format, U formatExtra, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Uses ADL to find a corresponding @cpp pixelSize(T) @ce overload,
         * then calls @ref ImageView(PixelStorage, UnsignedInt, UnsignedInt, UnsignedInt, const VectorTypeFor<dimensions, T>&, Containers::ArrayView<const void>)
         * with calculated pixel size and @p formatExtra set to @cpp 0 @ce.
         */
        template<class T> explicit ImageView(PixelStorage storage, T format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to @ref ImageView(PixelStorage, T, U, const VectorTypeFor<dimensions, Int>&, Containers::ArrayView<const void>)
         * with default-constructed @ref PixelStorage.
         */
        template<class T, class U> explicit ImageView(T format, U formatExtra, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept: ImageView{{}, format, formatExtra, size, data} {}

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param format            Format of pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to @ref ImageView(PixelStorage, T, const VectorTypeFor<dimensions, Int>&, Containers::ArrayView<const void>)
         * with default-constructed @ref PixelStorage.
         */
        template<class T> explicit ImageView(T format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept: ImageView{{}, format, size, data} {}

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param size              Image size
         *
         * Uses ADL to find a corresponding @cpp pixelSize(T, U) @ce overload,
         * then calls @ref ImageView(PixelStorage, UnsignedInt, UnsignedInt, UnsignedInt, const VectorTypeFor<dimensions, T>&)
         * with calculated pixel size.
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        template<class T, class U> explicit ImageView(PixelStorage storage, T format, U formatExtra, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param storage           Storage of pixel data
         * @param format            Format of pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Uses ADL to find a corresponding @cpp pixelSize(T) @ce overload,
         * then calls @ref ImageView(PixelStorage, UnsignedInt, UnsignedInt, UnsignedInt, const VectorTypeFor<dimensions, T>&)
         * with calculated pixel size and @p formatExtra set to @cpp 0 @ce.
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        template<class T> explicit ImageView(PixelStorage storage, T format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param format            Format of pixel data
         * @param formatExtra       Additional pixel format specifier
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to @ref ImageView(PixelStorage, T, U, const VectorTypeFor<dimensions, Int>&, Containers::ArrayView<const void>)
         * with default-constructed @ref PixelStorage.
         */
        template<class T, class U> explicit ImageView(T format, U formatExtra, const VectorTypeFor<dimensions, Int>& size) noexcept: ImageView{{}, format, formatExtra, size} {}

        /**
         * @brief Construct an image view with implementation-specific pixel format
         * @param format            Format of pixel data
         * @param size              Image size
         *
         * Equivalent to @ref ImageView(PixelStorage, T, const VectorTypeFor<dimensions, Int>&)
         * with default-constructed @ref PixelStorage.
         */
        template<class T, class U> explicit ImageView(T format, const VectorTypeFor<dimensions, Int>& size) noexcept: ImageView{{}, format, size, data} {}

        /** @brief Storage of pixel data */
        PixelStorage storage() const { return _storage; }

        /**
         * @brief Format of pixel data
         *
         * Returns either a defined value from the @ref PixelFormat enum or a
         * wrapped implementation-defined value. Use @ref isImplementationDefined()
         * to distinguish the case and @ref pixelFormatUnwrap() to extract a
         * implementation-defined value, if needed.
         */
        PixelFormat format() const { return _format; }

        /**
         * @brief Additional pixel format specifier
         *
         * Some implementations define a pixel format with two values. This
         * field contains the implementation-defined value, if any.
         */
        UnsignedInt formatExtra() const { return _formatExtra; }

        #ifdef MAGNUM_BUILD_DEPRECATED
        /**
         * @brief Data type of pixel data
         * @deprecated Cast @ref formatExtra() to @ref GL::PixelType instead.
         */
        CORRADE_DEPRECATED("cast formatExtra() to GL::PixelType instead") GL::PixelType type() const { return GL::PixelType(_formatExtra); }
        #endif

        /**
         * @brief Pixel size (in bytes)
         *
         * @see @ref pixelSize(PixelFormat)
         */
        UnsignedInt pixelSize() const { return _pixelSize; }

        /** @brief Image size */
        constexpr VectorTypeFor<dimensions, Int> size() const { return _size; }

        /**
         * @brief Image data properties
         *
         * See @ref PixelStorage::dataProperties() for more information.
         */
        std::tuple<VectorTypeFor<dimensions, std::size_t>, VectorTypeFor<dimensions, std::size_t>, std::size_t> dataProperties() const {
            return Implementation::imageDataProperties<dimensions>(*this);
        }

        /** @brief Raw data */
        constexpr Containers::ArrayView<const char> data() const { return _data; }

        /** @overload */
        template<class T> const T* data() const {
            return reinterpret_cast<const T*>(_data.data());
        }

        /**
         * @brief Set image data
         *
         * The data are expected to be of proper size for parameters specified
         * in the constructor.
         */
        void setData(Containers::ArrayView<const void> data);

    private:
        PixelStorage _storage;
        PixelFormat _format;
        UnsignedInt _formatExtra;
        UnsignedInt _pixelSize;
        Math::Vector<Dimensions, Int> _size;
        Containers::ArrayView<const char> _data;
};

/** @brief One-dimensional image view */
typedef ImageView<1> ImageView1D;

/** @brief Two-dimensional image view */
typedef ImageView<2> ImageView2D;

/** @brief Three-dimensional image view */
typedef ImageView<3> ImageView3D;

/**
@brief Compressed image view

Adds information about dimensions and compression type to some data in memory.

See @ref ImageView for more information. Interchangeable with
@ref CompressedImage, @ref CompressedBufferImage or @ref Trade::ImageData.
@see @ref CompressedImageView1D, @ref CompressedImageView2D,
    @ref CompressedImageView3D
*/
template<UnsignedInt dimensions> class CompressedImageView {
    public:
        enum: UnsignedInt {
            Dimensions = dimensions /**< Image dimension count */
        };

        /**
         * @brief Constructor
         * @param storage           Storage of compressed pixel data
         * @param format            Format of compressed pixel data
         * @param size              Image size
         * @param data              Image data
         */
        explicit CompressedImageView(CompressedPixelStorage storage, CompressedPixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Constructor
         * @param format            Format of compressed pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to calling @ref CompressedImageView(CompressedPixelStorage, CompressedPixelFormat, const VectorTypeFor<dimensions, Int>&, Containers::Array<char>&&)
         * with default-constructed @ref CompressedPixelStorage.
         */
        explicit CompressedImageView(CompressedPixelFormat format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept: CompressedImageView{{}, format, size, data} {}

        /**
         * @brief Constructor
         * @param storage           Storage of compressed pixel data
         * @param format            Format of compressed pixel data
         * @param size              Image size
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        explicit CompressedImageView(CompressedPixelStorage storage, CompressedPixelFormat format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /**
         * @brief Constructor
         * @param format            Format of compressed pixel data
         * @param size              Image size
         *
         * Equivalent to calling @ref CompressedImageView(CompressedPixelStorage, CompressedPixelFormat, const VectorTypeFor<dimensions, Int>&)
         * with default-constructed @ref CompressedPixelStorage.
         */
        explicit CompressedImageView(CompressedPixelFormat format, const VectorTypeFor<dimensions, Int>& size) noexcept: CompressedImageView{{}, format, size} {}

        /**
         * @brief Construct a compressed image with implementation-specific format
         * @param storage           Storage of compressed pixel data
         * @param format            Format of compressed pixel data
         * @param size              Image size
         * @param data              Image data
         */
        template<class T> explicit CompressedImageView(CompressedPixelStorage storage, T format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Construct a compressed image with implementation-specific format
         * @param format            Format of compressed pixel data
         * @param size              Image size
         * @param data              Image data
         *
         * Equivalent to calling @ref CompressedImageView(CompressedPixelStorage, CompressedPixelFormat, const VectorTypeFor<dimensions, Int>&, Containers::Array<char>&&)
         * with default-constructed @ref CompressedPixelStorage.
         */
        template<class T> explicit CompressedImageView(T format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;

        /**
         * @brief Construct a compressed image with implementation-specific format
         * @param storage           Storage of compressed pixel data
         * @param format            Format of compressed pixel data
         * @param size              Image size
         *
         * Data pointer is set to @cpp nullptr @ce, call @ref setData() to fill
         * the image with data.
         */
        template<class T> explicit CompressedImageView(CompressedPixelStorage storage, T format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /**
         * @brief Construct a compressed image with implementation-specific format
         * @param format            Format of compressed pixel data
         * @param size              Image size
         *
         * Equivalent to calling @ref CompressedImageView(CompressedPixelStorage, CompressedPixelFormat, const VectorTypeFor<dimensions, Int>&)
         * with default-constructed @ref CompressedPixelStorage.
         */
        template<class T> explicit CompressedImageView(T format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        /** @brief Storage of compressed pixel data */
        CompressedPixelStorage storage() const { return _storage; }

        /**
         * @brief Format of compressed pixel data
         *
         * Returns either a defined value from the @ref CompressedPixelFormat
         * enum or a wrapped implementation-defined value. Use
         * @ref isCompressedPixelFormatImplementationDefined() to distinguish
         * the case and @ref compressedPixelFormatUnwrap() to extract an
         * implementation-defined value, if needed.
         */
        CompressedPixelFormat format() const { return _format; }

        /** @brief Image size */
        constexpr VectorTypeFor<dimensions, Int> size() const { return _size; }

        /**
         * @brief Compressed image data properties
         *
         * See @ref CompressedPixelStorage::dataProperties() for more
         * information.
         */
        std::tuple<VectorTypeFor<dimensions, std::size_t>, VectorTypeFor<dimensions, std::size_t>, std::size_t> dataProperties() const {
            return Implementation::compressedImageDataProperties<dimensions>(*this);
        }

        /** @brief Raw data */
        constexpr Containers::ArrayView<const char> data() const { return _data; }

        /** @overload */
        template<class T> const T* data() const {
            return reinterpret_cast<const T*>(_data.data());
        }

        /**
         * @brief Set image data
         * @param data              Image data
         *
         * Dimensions, color compnents and data type remains the same as
         * passed in constructor. The data are not copied nor deleted on
         * destruction.
         */
        void setData(Containers::ArrayView<const void> data) {
            _data = {reinterpret_cast<const char*>(data.data()), data.size()};
        }

    private:
        /* To be made public once block size and block data size are stored
           together with the image */
        explicit CompressedImageView(CompressedPixelStorage storage, UnsignedInt format, const VectorTypeFor<dimensions, Int>& size, Containers::ArrayView<const void> data) noexcept;
        explicit CompressedImageView(CompressedPixelStorage storage, UnsignedInt format, const VectorTypeFor<dimensions, Int>& size) noexcept;

        CompressedPixelStorage _storage;
        CompressedPixelFormat _format;
        Math::Vector<Dimensions, Int> _size;
        Containers::ArrayView<const char> _data;
};

/** @brief One-dimensional compressed image view */
typedef CompressedImageView<1> CompressedImageView1D;

/** @brief Two-dimensional compressed image view */
typedef CompressedImageView<2> CompressedImageView2D;

/** @brief Three-dimensional compressed image view */
typedef CompressedImageView<3> CompressedImageView3D;

namespace Implementation {
    template<class T> inline UnsignedInt pixelSizeAdl(T format) {
//         using Magnum::pixelSize;
        return pixelSize(format);
    }

    template<class T, class U> inline UnsignedInt pixelSizeAdl(T format, U formatExtra) {
//         using Magnum::pixelSize;
        return pixelSize(format, formatExtra);
    }
}

template<UnsignedInt dimensions> template<class T, class U> ImageView<dimensions>::ImageView(const PixelStorage storage, const T format, const U formatExtra, const VectorTypeFor<dimensions, Int>& size, const Containers::ArrayView<const void> data) noexcept: ImageView{storage, UnsignedInt(format), UnsignedInt(formatExtra), Implementation::pixelSizeAdl(format, formatExtra), size, data} {
    static_assert(sizeof(T) <= 4 && sizeof(U) <= 4,
        "format types larger than 32bits are not supported");
}

template<UnsignedInt dimensions> template<class T> ImageView<dimensions>::ImageView(const PixelStorage storage, const T format, const VectorTypeFor<dimensions, Int>& size, const Containers::ArrayView<const void> data) noexcept: ImageView{storage, UnsignedInt(format), {}, Implementation::pixelSizeAdl(format), size, data} {
    static_assert(sizeof(T) <= 4,
        "format types larger than 32bits are not supported");
}

template<UnsignedInt dimensions> template<class T, class U> ImageView<dimensions>::ImageView(const PixelStorage storage, const T format, const U formatExtra, const VectorTypeFor<dimensions, Int>& size) noexcept: ImageView{storage, UnsignedInt(format), UnsignedInt(formatExtra), Implementation::pixelSizeAdl(format, formatExtra), size} {
    static_assert(sizeof(T) <= 4 && sizeof(U) <= 4,
        "format types larger than 32bits are not supported");
}

template<UnsignedInt dimensions> template<class T> ImageView<dimensions>::ImageView(const PixelStorage storage, const T format, const VectorTypeFor<dimensions, Int>& size) noexcept: ImageView{storage, UnsignedInt(format), {}, Implementation::pixelSizeAdl(format), size} {
    static_assert(sizeof(T) <= 4,
        "format types larger than 32bits are not supported");
}

template<UnsignedInt dimensions> template<class T> CompressedImageView<dimensions>::CompressedImageView(const CompressedPixelStorage storage, const T format, const VectorTypeFor<dimensions, Int>& size, const Containers::ArrayView<const void> data) noexcept: CompressedImageView{storage, UnsignedInt(format), size, data} {
    static_assert(sizeof(T) <= 4,
        "format types larger than 32bits are not supported");
}

template<UnsignedInt dimensions> template<class T> CompressedImageView<dimensions>::CompressedImageView(const CompressedPixelStorage storage, const T format, const VectorTypeFor<dimensions, Int>& size) noexcept: CompressedImageView{storage, UnsignedInt(format), size} {
    static_assert(sizeof(T) <= 4,
        "format types larger than 32bits are not supported");
}

}

#endif

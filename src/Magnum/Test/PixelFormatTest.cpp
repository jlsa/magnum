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

#include <sstream>
#include <Corrade/TestSuite/Tester.h>

#include "Magnum/PixelFormat.h"

namespace Magnum { namespace Test {

struct PixelFormatTest: TestSuite::Tester {
    explicit PixelFormatTest();

    void size();
    void sizeImplementationSpecific();

    void isImplementationSpecific();
    void wrap();
    void wrapInvalid();
    void unwrap();
    void unwrapInvalid();

    void compressedIsImplementationSpecific();
    void compressedWrap();
    void compressedWrapInvalid();
    void compressedUnwrap();
    void compressedUnwrapInvalid();

    void debug();
    void debugImplementationSpecific();

    void compressedDebug();
    void compressedDebugImplementationSpecific();
};

PixelFormatTest::PixelFormatTest() {
    addTests({&PixelFormatTest::size,
              &PixelFormatTest::sizeImplementationSpecific,

              &PixelFormatTest::isImplementationSpecific,
              &PixelFormatTest::wrap,
              &PixelFormatTest::wrapInvalid,
              &PixelFormatTest::unwrap,
              &PixelFormatTest::unwrapInvalid,

              &PixelFormatTest::compressedIsImplementationSpecific,
              &PixelFormatTest::compressedWrap,
              &PixelFormatTest::compressedWrapInvalid,
              &PixelFormatTest::compressedUnwrap,
              &PixelFormatTest::compressedUnwrapInvalid,

              &PixelFormatTest::debug,
              &PixelFormatTest::debugImplementationSpecific,

              &PixelFormatTest::compressedDebug,
              &PixelFormatTest::compressedDebugImplementationSpecific});
}

void PixelFormatTest::size() {
    CORRADE_COMPARE(pixelSize(PixelFormat::RGB8Unorm), 3);
    CORRADE_COMPARE(pixelSize(PixelFormat::RGBA16F), 8);
}

void PixelFormatTest::sizeImplementationSpecific() {
    std::ostringstream out;
    Error redirectError{&out};

    Magnum::pixelSize(pixelFormatWrap(0xdead));

    CORRADE_COMPARE(out.str(), "pixelSize(): can't determine pixel size of an implementation-specific format\n");
}

void PixelFormatTest::isImplementationSpecific() {
    CORRADE_VERIFY(!isPixelFormatImplementationSpecific(PixelFormat::RGBA8Unorm));
    CORRADE_VERIFY(isPixelFormatImplementationSpecific(pixelFormatWrap(0xdead)));
}

void PixelFormatTest::wrap() {
    CORRADE_COMPARE(UnsignedInt(pixelFormatWrap(0xdead)), 0x800dead);
}

void PixelFormatTest::wrapInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    pixelFormatWrap(0xdeadbeef);

    CORRADE_COMPARE(out.str(), "pixelFormatWrap(): the highest bit is expected to be unset\n");
}

void PixelFormatTest::unwrap() {
    CORRADE_COMPARE(UnsignedInt(pixelFormatUnwrap(PixelFormat(0x800dead))), 0xdead);
}

void PixelFormatTest::unwrapInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    pixelFormatUnwrap(PixelFormat(0xdead));

    CORRADE_COMPARE(out.str(), "pixelFormatUnwrap(): the highest bit is expected to be set\n");
}

void PixelFormatTest::compressedIsImplementationSpecific() {
    CORRADE_VERIFY(!isPixelFormatImplementationSpecific(PixelFormat::RGBA8Unorm));
    CORRADE_VERIFY(isPixelFormatImplementationSpecific(pixelFormatWrap(0xdead)));
}

void PixelFormatTest::compressedWrap() {
    CORRADE_COMPARE(UnsignedInt(pixelFormatWrap(0xdead)), 0x800dead);
}

void PixelFormatTest::compressedWrapInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    compressedPixelFormatWrap(0xdeadbeef);

    CORRADE_COMPARE(out.str(), "compressedPixelFormatWrap(): the highest bit is expected to be unset\n");
}

void PixelFormatTest::compressedUnwrap() {
    CORRADE_COMPARE(UnsignedInt(compressedPixelFormatUnwrap(CompressedPixelFormat(0x800dead))), 0xdead);
}

void PixelFormatTest::compressedUnwrapInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    compressedPixelFormatUnwrap(CompressedPixelFormat(0xdead));

    CORRADE_COMPARE(out.str(), "compressedPixelFormatUnwrap(): the highest bit is expected to be set\n");
}

void PixelFormatTest::debug() {
    std::ostringstream out;
    Debug{&out} << PixelFormat::RG16Snorm << PixelFormat(0xdead);

    CORRADE_COMPARE(out.str(), "PixelFormat::RG16Snorm PixelFormat(0xdead)\n");
}

void PixelFormatTest::debugImplementationSpecific() {
    std::ostringstream out;
    Debug{&out} << pixelFormatWrap(0xdead);

    CORRADE_COMPARE(out.str(), "PixelFormat::ImplementationSpecific(0xdead)\n");
}

void PixelFormatTest::compressedDebug() {
    std::ostringstream out;
    Debug{&out} << CompressedPixelFormat::Bc3RGBAUnorm << CompressedPixelFormat(0xdead);

    CORRADE_COMPARE(out.str(), "CompressedPixelFormat::Bc3RGBAUnorm CompressedPixelFormat(0xdead)\n");
}

void PixelFormatTest::compressedDebugImplementationSpecific() {
    std::ostringstream out;
    Debug{&out} << compressedPixelFormatWrap(0xdead);

    CORRADE_COMPARE(out.str(), "CompressedPixelFormat::ImplementationSpecific(0xdead)\n");
}

}}

CORRADE_TEST_MAIN(Magnum::Test::PixelFormatTest)

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
#include "Magnum/GL/PixelFormat.h"

namespace Magnum { namespace GL { namespace Test {

struct PixelFormatTest: TestSuite::Tester {
    explicit PixelFormatTest();

    void mapFormatType();
    void mapFormatImplementationSpecific();
    void mapFormatImplementationSpecificInvalid();
    void mapTypeImplementationSpecific();
    void mapTypeImplementationSpecificInvalid();

    void mapCompressedFormat();
    void mapCompressedFormatImplementationSpecific();
    void mapCompressedFormatImplementationSpecificInvalid();

    void debugPixelFormat();
    void debugPixelType();

    void debugCompressedPixelFormat();
};

PixelFormatTest::PixelFormatTest() {
    addTests({&PixelFormatTest::mapFormatType,
              &PixelFormatTest::mapFormatImplementationSpecific,
              &PixelFormatTest::mapFormatImplementationSpecificInvalid,
              &PixelFormatTest::mapTypeImplementationSpecific,
              &PixelFormatTest::mapTypeImplementationSpecificInvalid,

              &PixelFormatTest::mapCompressedFormat,
              &PixelFormatTest::mapCompressedFormatImplementationSpecific,
              &PixelFormatTest::mapCompressedFormatImplementationSpecificInvalid,

              &PixelFormatTest::debugPixelFormat,
              &PixelFormatTest::debugPixelType,
              &PixelFormatTest::debugCompressedPixelFormat});
}

void PixelFormatTest::mapFormatType() {
    /* Yes, this goes through all 31 bits */
    UnsignedInt firstUnhandled = 0x7fffffff;
    UnsignedInt nextHandled = 0;
    for(UnsignedInt i = 0; i != 0x7fffffff; ++i) {
        const auto format = Magnum::PixelFormat(i);
        /* Each case verifies:
           - that the cases are ordered by number (so insertion here is done in
             proper place)
           - that there was no gap (unhandled value inside the range)
           - that a particular pixel format maps to a particular GL format
           - that a particular pixel type maps to a particular GL type */
        switch(format) {
            #define _c(format, expectedFormat, expectedType) \
                case Magnum::PixelFormat::format: \
                    CORRADE_COMPARE(nextHandled, i); \
                    CORRADE_COMPARE(firstUnhandled, 0xffffffff); \
                    CORRADE_COMPARE(pixelFormat(Magnum::PixelFormat::format), Magnum::GL::PixelFormat::expectedFormat); \
                    CORRADE_COMPARE(pixelType(Magnum::PixelFormat::format), Magnum::GL::PixelType::expectedType); \
                    ++nextHandled; \
                    continue;
            #include "Magnum/GL/Implementation/pixelFormatMapping.hpp"
            #undef _c
        }

        /* Not handled by any value, remember -- we might either be at the end
           of the enum range (which is okay) or some value might be unhandled
           here */
        firstUnhandled = i;
    }

    CORRADE_COMPARE(firstUnhandled, 0x7fffffff);
}

void PixelFormatTest::mapFormatImplementationSpecific() {
}

void PixelFormatTest::mapFormatImplementationSpecificInvalid() {
}

void PixelFormatTest::mapTypeImplementationSpecific() {
}

void PixelFormatTest::mapTypeImplementationSpecificInvalid() {
}

void PixelFormatTest::mapCompressedFormat() {
    /* Yes, this goes through all 31 bits */
    UnsignedInt firstUnhandled = 0x7fffffff;
    UnsignedInt nextHandled = 0;
    for(UnsignedInt i = 0; i != 0x7fffffff; ++i) {
        const auto format = Magnum::CompressedPixelFormat(i);
        /* Each case verifies:
           - that the cases are ordered by number (so insertion here is done in
             proper place)
           - that there was no gap (unhandled value inside the range)
           - that a particular pixel format maps to a particular GL format
           - that a particular pixel type maps to a particular GL type */
        switch(format) {
            #define _c(format, expectedFormat) \
                case Magnum::CompressedPixelFormat::format: \
                    CORRADE_COMPARE(nextHandled, i); \
                    CORRADE_COMPARE(firstUnhandled, 0xffffffff); \
                    CORRADE_COMPARE(compressedPixelFormat(Magnum::CompressedPixelFormat::format), Magnum::GL::PixelFormat::expectedFormat); \
                    ++nextHandled; \
                    continue;
            #include "Magnum/GL/Implementation/compressedPixelFormatMapping.hpp"
            #undef _c
        }

        /* Not handled by any value, remember -- we might either be at the end
           of the enum range (which is okay) or some value might be unhandled
           here */
        firstUnhandled = i;
    }

    CORRADE_COMPARE(firstUnhandled, 0x7fffffff);
}

void PixelFormatTest::mapCompressedFormatImplementationSpecific() {
}

void PixelFormatTest::mapCompressedFormatImplementationSpecificInvalid() {
}

void PixelFormatTest::debugPixelFormat() {
    std::ostringstream out;

    Debug(&out) << PixelFormat::RGBA << PixelFormat(0xdead);
    CORRADE_COMPARE(out.str(), "GL::PixelFormat::RGBA GL::PixelFormat(0xdead)\n");
}

void PixelFormatTest::debugPixelType() {
    std::ostringstream out;

    Debug(&out) << PixelType::UnsignedByte << PixelType(0xdead);
    CORRADE_COMPARE(out.str(), "GL::PixelType::UnsignedByte GL::PixelType(0xdead)\n");
}

void PixelFormatTest::debugCompressedPixelFormat() {
    #ifdef MAGNUM_TARGET_GLES
    CORRADE_SKIP("No enum value available");
    #else
    std::ostringstream out;

    Debug(&out) << CompressedPixelFormat::RGBBptcUnsignedFloat << CompressedPixelFormat(0xdead);
    CORRADE_COMPARE(out.str(), "GL::CompressedPixelFormat::RGBBptcUnsignedFloat GL::CompressedPixelFormat(0xdead)\n");
    #endif
}

}}}

CORRADE_TEST_MAIN(Magnum::GL::Test::PixelFormatTest)

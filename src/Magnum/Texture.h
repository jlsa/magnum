#ifndef Magnum_Texture_h
#define Magnum_Texture_h
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
 * @deprecated Use @ref Magnum/GL/Texture.h instead.
 */

#include "Magnum/configure.h"

#ifdef MAGNUM_BUILD_DEPRECATED
#include <Corrade/Utility/Macros.h>

#include "Magnum/Magnum.h"
#include "Magnum/GL/Texture.h"

/* Also include compatibility headers for all indirectly included types, so the
   user gets all compatibility typedefs */
#include "Magnum/AbstractTexture.h"

CORRADE_DEPRECATED_FILE("use Magnum/GL/Texture.h instead")

namespace Magnum {

/** @brief @copybrief GL::Texture
 * @deprecated Use @ref GL::Texture instead.
 */
template<UnsignedInt dimensions> using Texture CORRADE_DEPRECATED("use GL::Texture instead") = GL::Texture<dimensions>;

#ifndef MAGNUM_TARGET_GLES
/** @brief @copybrief GL::Texture1D
 * @deprecated Use @ref GL::Texture1D instead.
 */
typedef CORRADE_DEPRECATED("use GL::Texture1D instead") GL::Texture1D Texture1D;
#endif

/** @brief @copybrief GL::Texture2D
 * @deprecated Use @ref GL::Texture2D instead.
 */
typedef CORRADE_DEPRECATED("use GL::Texture2D instead") GL::Texture2D Texture2D;

#if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
/** @brief @copybrief GL::Texture3D
 * @deprecated Use @ref GL::Texture3D instead.
 */
typedef CORRADE_DEPRECATED("use GL::Texture3D instead") GL::Texture3D Texture3D;
#endif

}
#else
#error use Magnum/GL/Texture.h instead
#endif

#endif

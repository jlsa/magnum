#ifndef Magnum_AbstractFramebuffer_h
#define Magnum_AbstractFramebuffer_h
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
 * @deprecated Use @ref Magnum/GL/AbstractFramebuffer.h instead.
 */

#include "Magnum/configure.h"

#ifdef MAGNUM_BUILD_DEPRECATED
#include <Corrade/Utility/Macros.h>

#include "Magnum/Magnum.h"
#include "Magnum/GL/AbstractFramebuffer.h"

/* Also include compatibility headers for all indirectly included types, so the
   user gets all compatibility typedefs */
#include "Magnum/AbstractObject.h"

CORRADE_DEPRECATED_FILE("use Magnum/GL/AbstractFramebuffer.h instead")

namespace Magnum {

/** @brief @copybrief GL::FramebufferClear
 * @deprecated Use @ref GL::FramebufferClear instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferClear instead") GL::FramebufferClear FramebufferClear;

/** @brief @copybrief GL::FramebufferClearMask
 * @deprecated Use @ref GL::FramebufferClearMask instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferClearMask instead") GL::FramebufferClearMask FramebufferClearMask;

#if !(defined(MAGNUM_TARGET_WEBGL) && defined(MAGNUM_TARGET_GLES2))
/** @brief @copybrief GL::FramebufferBlit
 * @deprecated Use @ref GL::FramebufferBlit instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferBlit instead") GL::FramebufferBlit FramebufferBlit;

/** @brief @copybrief GL::FramebufferBlitMask
 * @deprecated Use @ref GL::FramebufferBlitMask instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferBlitMask instead") GL::FramebufferBlitMask FramebufferBlitMask;

/** @brief @copybrief GL::FramebufferBlitFilter
 * @deprecated Use @ref GL::FramebufferBlitFilter instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferBlitFilter instead") GL::FramebufferBlitFilter FramebufferBlitFilter;
#endif

/** @brief @copybrief GL::FramebufferTarget
 * @deprecated Use @ref GL::FramebufferTarget instead.
 */
typedef CORRADE_DEPRECATED("use GL::FramebufferTarget instead") GL::FramebufferTarget FramebufferTarget;

/** @brief @copybrief GL::AbstractFramebuffer
 * @deprecated Use @ref GL::AbstractFramebuffer instead.
 */
typedef CORRADE_DEPRECATED("use GL::AbstractFramebuffer instead") GL::AbstractFramebuffer AbstractFramebuffer;

}
#else
#error use Magnum/GL/AbstractFramebuffer.h instead
#endif

#endif

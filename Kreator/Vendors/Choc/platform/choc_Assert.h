//
//    ██████ ██   ██  ██████   ██████
//   ██      ██   ██ ██    ██ ██            ** Clean Header-Only Classes **
//   ██      ███████ ██    ██ ██
//   ██      ██   ██ ██    ██ ██           https://github.com/Tracktion/choc
//    ██████ ██   ██  ██████   ██████
//
//   CHOC is (C)2021 Tracktion Corporation, and is offered under the terms of the ISC license:
//
//   Permission to use, copy, modify, and/or distribute this software for any purpose with or
//   without fee is hereby granted, provided that the above copyright notice and this permission
//   notice appear in all copies. THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
//   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
//   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
//   CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
//   WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
//   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#ifndef CHOC_ASSERT_HEADER_INCLUDED
#define CHOC_ASSERT_HEADER_INCLUDED

// If the project doesn't define a custom implementation for CHOC_ASSERT, the default
// behaviour is just to call the normal system assert()
#ifndef CHOC_ASSERT
 #include <cassert>
 #define CHOC_ASSERT(x)  assert(x);
#endif

// It's never a smart idea to include any C headers before your C++ ones, as they
// often pollute your namespace with all kinds of dangerous macros like these ones.
// This file is included by many of the choc headers, so is a convenient place to
// undef these.
#undef max
#undef min

#endif // CHOC_ASSERT_HEADER_INCLUDED


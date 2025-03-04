/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   TTYPES.H                                                              */
/*                                                                         */
/*   provides miscellaneous types used throughout Turbo Vision             */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#if !defined( __TTYPES_H )
#define __TTYPES_H

#if !defined(_NEAR)
#define _NEAR near
#endif

#include <tvision/compat/borland/_defs.h>

#ifdef __BORLANDC__
#define I   asm
#endif

enum { False, True };

#ifdef __BORLANDC__
typedef int Boolean;
#else
typedef bool Boolean;
#endif

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#include <stddef.h>

#ifndef __BORLANDC__
#include <stdint.h>
#else
typedef char int8_t;
typedef short int16_t;
typedef long int32_t;
typedef uchar uint8_t;
typedef ushort uint16_t;
typedef ulong uint32_t;
#endif

#ifdef __BORLANDC__
typedef ushort TScreenCell;
typedef uchar TCellChar;
typedef uchar TColorDesired;
typedef uchar TColorAttr;
typedef ushort TAttrPair;
#else
struct TScreenCell;
struct TCellChar;
struct TColorDesired;
struct TColorAttr;
struct TAttrPair;
#endif

const char EOS = '\0';

enum StreamableInit { streamableInit };

class _FAR ipstream;
class _FAR opstream;
class _FAR TStreamable;
class _FAR TStreamableTypes;

ipstream& _Cdecl operator >> ( ipstream&, char& );
ipstream& _Cdecl operator >> ( ipstream&, signed char& );
ipstream& _Cdecl operator >> ( ipstream&, unsigned char& );
ipstream& _Cdecl operator >> ( ipstream&, signed short& );
ipstream& _Cdecl operator >> ( ipstream&, unsigned short& );
ipstream& _Cdecl operator >> ( ipstream&, signed int& );
ipstream& _Cdecl operator >> ( ipstream&, unsigned int& );
ipstream& _Cdecl operator >> ( ipstream&, signed long& );
ipstream& _Cdecl operator >> ( ipstream&, unsigned long& );
ipstream& _Cdecl operator >> ( ipstream&, float& );
ipstream& _Cdecl operator >> ( ipstream&, double& );
ipstream& _Cdecl operator >> ( ipstream&, long double& );
ipstream& _Cdecl operator >> ( ipstream&, TStreamable& );
ipstream& _Cdecl operator >> ( ipstream&, void _FAR *& );

opstream& _Cdecl operator << ( opstream&, char );
opstream& _Cdecl operator << ( opstream&, signed char );
opstream& _Cdecl operator << ( opstream&, unsigned char );
opstream& _Cdecl operator << ( opstream&, signed short );
opstream& _Cdecl operator << ( opstream&, unsigned short );
opstream& _Cdecl operator << ( opstream&, signed int );
opstream& _Cdecl operator << ( opstream&, unsigned int );
opstream& _Cdecl operator << ( opstream&, signed long );
opstream& _Cdecl operator << ( opstream&, unsigned long );
opstream& _Cdecl operator << ( opstream&, float );
opstream& _Cdecl operator << ( opstream&, double );
opstream& _Cdecl operator << ( opstream&, long double );
opstream& _Cdecl operator << ( opstream&, TStreamable& );
opstream& _Cdecl operator << ( opstream&, TStreamable _FAR * );

#include <tvision/compat/borland/iosfwd.h>
class TStringView;
ostream _FAR & _Cdecl operator<<(ostream _FAR &, TStringView);

typedef int ccIndex;
typedef Boolean (*ccTestFunc)( void *, void * );
typedef void (*ccAppFunc)( void *, void * );

const int ccNotFound = -1;

extern const uchar specialChars[];

#if !defined ( __FLAT__ )
#define _genInt(i) __int__(i)
#endif

// Reserve future keywords
#if __cplusplus < 201103L
#define constexpr
#define noexcept
#define thread_local
#endif

// Do not include unnecessary STL headers if TVISION_NO_STL is defined.
// This speeds up compilation when building the library.
#if !defined( __BORLANDC__ ) && !defined( TVISION_NO_STL )
#define TVISION_STL
#endif

// In types with user-defined constructors, the default assignment operator
// creates a temporary object. If the type is large enough, the compiler
// may not be able to optimize out the temporary object. This has been observed
// with GCC, Clang, MSVC.
//
// To work around this in performance-critical types, we define a template
// assignment operator which just invokes the type constructor with the
// assigned object, so that the initialization is done in-place instead of in a
// temporary location.
//
// In order to use placement new, 'operator new(size_t, void*)' has to be defined.
// Usually this would be accomplished by including the <new> header, but due to
// compilation performance it is more convenient to define a class-overloaded
// operator new. We also define operator delete to avoid a warning in some compilers.
//
// This macro is intended to be used in the definition of types with trivial
// destructors, so there is no need to invoke the destructor before placement
// new nor guard against self-assignment.
//
// This solution is incompatible with non-template assignment operators,
// because of the following:
//
//     "Note that, if a non-template assignment operator from some non-class
//      type is available, it is preferred over the copy/move assignment in
//      'E1 = {}' because '{}' to non-class is an identity conversion, which
//      outranks the user-defined conversion from '{}' to a class type."
// (From https://en.cppreference.com/w/cpp/language/operator_assignment).
//
// This means that if we define a custom 'T& operator=(int)', the assignment
// 't = {}' will be equivalent to 't = 0' instead of 't = T()'. This would
// break the programmer's expectations completely.
#define TV_TRIVIALLY_ASSIGNABLE(S) \
    void* operator new(size_t, void *p) noexcept { return p; } \
    void operator delete(void *, void *) noexcept {}; \
    template <class T> \
    S& operator=(const T &t) { return *new (this) S(t); }


#endif  // __TTYPES_H

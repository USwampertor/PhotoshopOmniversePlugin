#pragma once

#define OV_PLATFORM_WIN32      1             //Windows Platform
#define OV_PLATFORM_LINUX      2             //Linux Platform
#define OV_PLATFORM_OSX        3             //Apple Platform
#define OV_PLATFORM_IOS        4             //I phone Platform
#define OV_PLATFORM_ANDROID    5             //Android Platform
#define OV_PLATFORM_PS4        6             //PlayStation 4 Platform

#define OV_COMPILER_MSVC       1             //Visual Studio Compiler
#define OV_COMPILER_GNUC       2             //GCC Compiler
#define OV_COMPILER_INTEL      3             //Intel Compiler
#define OV_COMPILER_CLANG      4             //Clang Compiler

#define OV_ARCHITECTURE_x86_32 1             //Intel x86 32 bits
#define OV_ARCHITECTURE_x86_64 2             //Intel x86 64 bits

#define OV_ENDIAN_LITTLE       1             //Little Endian
#define OV_ENDIAN_BIG          2             //Big Endian

//Define the actual endian type (little endian for windows, Linux, Apple and PS4

#define OV_ENDIAN OV_ENDIAN_LITTLE

//Define if on a crash we want to report warnings or uNknown symbols
#define OV_DEBUG_DETAILED_SYMBOLS 1


/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * Compiler type and version
 */
 /*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#if defined(_clang__)
# define OV_COMPILER OV_COMPILER_CLANG
# define OV_COMP_VER __clang_version__
# define OV_THREADLOCAL __thread
# define OV_STDCALL __attribute__((stdcall))
# define OV_CDECL __attribute__((cdecl))
# define OV_FALLTHROUGH [[clang::fallthrough]];
#elif defined(__GNUC__) //Check after clang, as clang defines this too
# define OV_COMPILER OV_COMPILER_GNUC
# define OV_COMP_VER (((__GNUC__)*100)+(__GNUC_MINOR__*10)+__GNUC_PATCHLEVEL__)
# define OV_THREADLOCAL __thread
# define OV_STDCALL __attribute__((stdcall))
# define OV_CDECL __attribute__((cdecl))
# define OV_FALLTHROUGH __attribute__((fallthrough));
#elif defined(__INTEL_COMPILER)
# define OV_COMPILER OV_COMPILER_INTEL
# define OV_COMP_VER __INTEL_COMPILER
# define OV_STDCALL __stdcall
# define OV_CDECL __cdecl
# define OV_FALLTHROUGH
/**
 * GF_THREADLOCAL define is down below because Intel compiler defines it
 *differently based on platform
 */

 //Check after Clang and Intel, we could be building with either with VS
#elif defined(_MSC_VER)
# define OV_COMPILER OV_COMPILER_MSVC
# define OV_COMP_VER _MSC_VER
# define OV_THREADLOCAL __declspec(thread)
# define OV_STDCALL __stdcall
# define OV_CDECL __cdecl
# define OV_FALLTHROUGH
# undef __PRETTY_FUNCTION__
# define __PRETTY_FUNCTION__ __FUNCSIG__
#else
//No known compiler found, send the error to the output (if any)
# pragma error "Man, I don't know how you got here... you working in lolcode right? "
#endif

/**
* See if we can use __forceinline or if we need to use __inline instead
*/

#if OV_COMPILER == OV_COMPILER_MSVC         //If we are compiling on Visual Studio
# if OV_COMP_VER >= 1200                     //If we are on Visual Studio 6 or higher
#   define FORCEINLINE __forceinline          //Set __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict              //No alias hint
#   endif
# endif
#elif defined(__MINGW32__)                    //If we are on a Unix type system
# if !defined(FORCEINLINE)
#   define FORCEINLINE __inline               //Set __inline
#   ifndef RESTRICT
#     define RESTRICT                         //No alias hint
#   endif
# endif
#else                                         //Any other compiler
# define FORCEINLINE __inline                 //Set inline
# ifndef RESTRICT
#   define RESTRICT __restrict
# endif
#endif

/**
* Finds the current platform
*/
#if defined(__WIN32__) || defined(_WIN32)     //If its a windows platform
# define OV_PLATFORM OV_PLATFORM_WIN32
#elif defined(__APPLE_CC__)                   //...or is an Apple platform
# define OV_PLATFORM OV_PLATFORM_OSX  
#elif defined(__ORBIS__)                      //...or is a PlayStation
# define OV_PLATFORM OV_PLATFORM_PS4
#else                                         //Will consider it as a Linux platform
# define OV_PLATFORM OV_PLATFORM_LINUX
#endif

/**
* Finds the architecture type
*/
#if defined(__x86_64__)||defined(_M_X64)      //If this is a x64 compile
# define OV_ARCH_TYPE OV_ARCHITECTURE_x86_64
#else //If its a x86 compile
# define OV_ARCH_TYPE OV_ARCHITECTURE_x86_32
#endif

/**
* Memory alignment macros
*/
#if OV_COMPILER == OV_COMPILER_MSVC         //If we are compiling on visual studio
# define MS_ALIGN(n) __declspec(align(n))
# ifndef GCC_PACK
#   define GCC_PACK(n)
# endif
# ifndef GCC_ALIGN
#   define GCC_ALIGN(n)
# endif
#elif (OV_COMPILER == OV_COMPILER_GNUC && OV_PLATFORM == OV_PLATFORM_PS4)
# define MS_ALIGN(n)
# define GCC_PACK(n)
# define GCC_ALIGN(n) __attribute__( (__aligned__(n)) )
#else
# define MS_ALIGN(n)
# define GCC_PACK(n)  __attribute__( (packed, aligned(n)) )
# define GCC_ALIGN(n) __attribute__( (aligned(n)) )
#endif

/**
* For throw override (deprecated on c++11 but visual studio does not have handle
* no exception)
*/
# if OV_COMPILER == OV_COMPILER_MSVC
# define _NOEXCEPT noexcept
# elif OV_COMPILER == OV_COMPILER_INTEL
# define _NOEXCEPT noexcept
# elif OV_COMPILER == OV_COMPILER_GNUC
# define _NOEXCEPT noexcept
# else
# define _NOEXCEPT
# endif

/**
* Library export specifics
*/
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if OV_COMPILER == OV_COMPILER_MSVC
#   if defined( OV_STATIC_LIB )
#     define OV_UTILITY_EXPORT
#   else
#     if defined( OV_UTILITY_EXPORTS )
#       define OV_UTILITY_EXPORT __declspec( dllexport )
#     else
#       define OV_UTILITY_EXPORT __declspec( dllimport )
#     endif
#   endif
# else //Any other compiler
#   if defined( OV_STATIC_LIB )
#     define OV_UTILITY_EXPORT
#   else
#     if defined( OV_UTILITY_EXPORTS )
#       define OV_UTILITY_EXPORT __attribute__ ((dllexport))
#     else
#       define OV_UTILITY_EXPORT __attribute__ ((dllimport))
#     endif
#   endif
# endif
# define OV_UTILITY_HIDDEN
#else //Linux/Mac Settings
# define OV_UTILITY_EXPORT __attribute__ ((visibility ("default")))
# define OV_UTILITY_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

//DLL export for plugins
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if OV_COMPILER == OV_COMPILER_MSVC
#   define OV_PLUGIN_EXPORT __declspec(dllexport)
# else
#   define OV_PLUGIN_EXPORT __attribute__ ((dllexport))
# endif
# define OV_UTILITY_HIDDEN
#else //Linux Mac Settings
# define OV_PLUGIN_EXPORT __attribute__ ((visibility("default")))
#endif

/**
* Windows Specific Settings
*/
//Win32 compilers use _DEBUG for specifying debug builds. For MinGW we set DEBUG
#if OV_PLATFORM == OV_PLATFORM_WIN32
# if defined(_DEBUG)||defined(DEBUG)
#   define OV_DEBUG_MODE 1                   //Specifies that we are on a DEBUG build
# else
#   define OV_DEBUG_MODE 0                   //We are not on DEBUG build
# endif
# if OV_COMPILER == OV_COMPILER_INTEL
#   define OV_THREADLOCAL __declspec(thread) //Set the local thread for the Intel compiler
# endif
#endif  //OV_PLATFORM == OV_PLATFORM_WIN32

/**
* Linux/Apple Specific Settings
*/
#if OV_PLATFORM == OV_PLATFORM_LINUX || OV_PLATFORM == OV_PLATFORM_OSX
# define stricmp strcasecmp

//If we are on a DEBUG build
# if defined(_DEBUG)||defined(DEBUG)
#   define OV_DEBUG_MODE 1                   //Specifies that we are on a DEBUG build
# else
#   define OV_DEBUG_MODE 0                   //We are not on DEBUG build
# endif
# if OV_COMPILER == OV_COMPILER_INTEL
#   define OV_THREADLOCAL __thread           //Set the local thread for the Intel compiler
# endif
#endif  //OV_PLATFORM == OV_PLATFORM_LINUX || OV_PLATFORM == OV_PLATFORM_OSX

/**
* PS4 Specific Settings
*/
#if OV_PLATFORM == OV_PLATFORM_PS4
//If we are on a DEBUG build
# if defined(_DEBUG)||defined(DEBUG)
#   define OV_DEBUG_MODE 1                   //Specifies that we are on a DEBUG build
# else
#   define OV_DEBUG_MODE 0                   //We are not on DEBUG build
# endif
#endif  //OV_PLATFORM == OV_PLATFORM_PS4

/**
* Definition of Debug macros
*/
#if OV_DEBUG_MODE
# define OV_DEBUG_ONLY(x) x
# define OV_ASSERT(x) assert(x)
#else
# define OV_DEBUG_ONLY(x)
# define OV_ASSERT(x)
#endif

/**
* Disable some compiler warnings
*/
//If we are compiling with Visual Studio
#if OV_COMPILER == OV_COMPILER_MSVC
  /**
   * TODO: This is not  deactivated anywhere, therefore it applies to any file
   * that includes this header, Right now I don't have an easier way to apply
   * these warnings globally so I'm keeping it this way
   */

   //Secure versions aren't always multi platform, so we won't be using them
# define _CRT_SECURE_NO_WARNINGS
  /**
   * Something something
   */
# pragma warning (disable: 4251) //Type1 needs an interface for Type2

# pragma warning (disable: 4996) //Deprecated declarations

# pragma warning (disable: 4503) //Name is too large, but this sh*t is for 16-32

# pragma warning (disable: 4275) //DLL interfaces
   /**
    * Disable: nonstandard extension used: override specifier keyword
    * Happens when a keyboard was used that is not in the C++
    */
#endif

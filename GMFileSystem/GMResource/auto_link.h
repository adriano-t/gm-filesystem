//  (C) Copyright John Maddock 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         auto_link.hpp
  *   VERSION      see <boost/version.hpp>
  *   DESCRIPTION: Automatic library inclusion for Borland/Microsoft compilers.
  */

/*************************************************************************

USAGE:
~~~~~~

Before including this header you must define one or more of define the following macros:

BOOST_LIB_NAME:           Required: A string containing the basename of the library,
                          for example boost_regex.
BOOST_LIB_TOOLSET:        Optional: the base name of the toolset.
BOOST_DYN_LINK:           Optional: when set link to dll rather than static library.
BOOST_LIB_DIAGNOSTIC:     Optional: when set the header will print out the name
                          of the library selected (useful for debugging).
BOOST_AUTO_LINK_NOMANGLE: Specifies that we should link to BOOST_LIB_NAME.lib,
                          rather than a mangled-name version.
BOOST_AUTO_LINK_TAGGED:   Specifies that we link to libraries built with the --layout=tagged option.
                          This is essentially the same as the default name-mangled version, but without
                          the compiler name and version, or the Boost version.  Just the build options.

These macros will be undef'ed at the end of the header, further this header
has no include guards - so be sure to include it only once from your library!

Algorithm:
~~~~~~~~~~

Libraries for Borland and Microsoft compilers are automatically
selected here, the name of the lib is selected according to the following
formula:

BOOST_LIB_PREFIX
   + BOOST_LIB_NAME
   + "_"
   + BOOST_LIB_TOOLSET
   + BOOST_LIB_THREAD_OPT
   + BOOST_LIB_RT_OPT
   "-"
   + BOOST_LIB_VERSION

These are defined as:

BOOST_LIB_PREFIX:     "lib" for static libraries otherwise "".

BOOST_LIB_NAME:       The base name of the lib ( for example boost_regex).

BOOST_LIB_TOOLSET:    The compiler toolset name (vc6, vc7, bcb5 etc).

BOOST_LIB_THREAD_OPT: "-mt" for multithread builds, otherwise nothing.

BOOST_LIB_RT_OPT:     A suffix that indicates the runtime library used,
                      contains one or more of the following letters after
                      a hiphen:

                      s      static runtime (dynamic if not present).
                      g      debug/diagnostic runtime (release if not present).
                      y      Python debug/diagnostic runtime (release if not present).
                      d      debug build (release if not present).
                      g      debug/diagnostic runtime (release if not present).
                      p      STLPort Build.

BOOST_LIB_VERSION:    The Boost version, in the form x_y, for Boost version x.y.


***************************************************************************/


//
// Only include what follows for known and supported compilers:
//

#  define GMIMG_STRINGIZE(X) GMIMG_DO_STRINGIZE(X)
#  define GMIMG_DO_STRINGIZE(X) #X


#if defined(_MSC_VER)



#ifndef GMIMG_LIB_NAME
#  error "Macro BOOST_LIB_NAME not set (internal error)"
#endif

// Note: no compilers before 1400 are supported

#if defined(_MSC_VER) && (_MSC_VER == 1400)

   // vc80:
#  define GMIMG_LIB_TOOLSET "vc80"

#elif defined(_MSC_VER) && (_MSC_VER == 1500)

   // vc90:
#  define GMIMG_LIB_TOOLSET "vc90"

#elif defined(_MSC_VER) && (_MSC_VER == 1600)

   // vc10:
#  define GMIMG_LIB_TOOLSET "vc100"

#elif defined(_MSC_VER) && (_MSC_VER >= 1700)

   // vc11:
#  define GMIMG_LIB_TOOLSET "vc110"
#endif




//
// error check:
//
#if defined(__MSVC_RUNTIME_CHECKS) && !defined(_DEBUG)
#  pragma message("Using the /RTC option without specifying a debug runtime will lead to linker errors")
#  pragma message("Hint: go to the code generation options and switch to one of the debugging runtimes")
#  error "Incompatible build options"
#endif
//
// select toolset if not defined already:
//

//
// select thread opt:
//
#if defined(_MT) 
#  define GMIMG_LIB_THREAD_OPT "-mt"
#else
#  define GMIMG_LIB_THREAD_OPT
#endif

#if defined(_MSC_VER) 
#  ifdef _DLL
#    if defined(_DEBUG)
#      define GMIMG_LIB_RT_OPT "-gd"
#    else
#      define GMIMG_LIB_RT_OPT
#    endif
#  else
#    if defined(_DEBUG)
#      define GMIMG_LIB_RT_OPT "-sgd"
#    else
#      define GMIMG_LIB_RT_OPT "-s"
#    endif
#  endif

#endif


//
// now include the lib:
//
#if defined(GMIMG_LIB_NAME) \
      && defined(GMIMG_LIB_TOOLSET) \
      && defined(GMIMG_LIB_THREAD_OPT) \
      && defined(GMIMG_LIB_RT_OPT)


#  pragma comment(lib, GMIMG_STRINGIZE(GMIMG_LIB_NAME) "-" GMIMG_LIB_TOOLSET GMIMG_LIB_THREAD_OPT GMIMG_LIB_RT_OPT  ".lib")
#  ifdef GMIMG_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " GMIMG_STRINGIZE(GMIMG_LIB_NAME) "-" GMIMG_LIB_TOOLSET GMIMG_LIB_THREAD_OPT GMIMG_LIB_RT_OPT  ".lib")
#  endif

#else
#  error "some required macros where not defined (internal logic error)."
#endif


#endif // _MSC_VER || __BORLANDC__

//
// finally undef any macros we may have set:
//

#if defined(GMIMG_LIB_NAME)
#  undef GMIMG_LIB_NAME
#endif
#if defined(GMIMG_LIB_THREAD_OPT)
#  undef GMIMG_LIB_THREAD_OPT
#endif
#if defined(GMIMG_LIB_RT_OPT)
#  undef GMIMG_LIB_RT_OPT
#endif


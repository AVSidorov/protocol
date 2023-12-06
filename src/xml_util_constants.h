#pragma once

#if defined(_MSC_VER)
#  define XML_UTIL_DECL_EXPORT __declspec(dllexport)
#  define XML_UTIL_DECL_IMPORT __declspec(dllimport)
#endif

#ifndef XML_UTIL_DECL_EXPORT
#  define XML_UTIL_DECL_EXPORT
#endif
#ifndef XML_UTIL_DECL_IMPORT
#  define XML_UTIL_DECL_IMPORT
#endif

#if defined(XML_UTIL_SHARED) || !defined(XML_UTIL_STATIC)
#  ifdef XML_UTIL_STATIC
#    error "Both XML_UTIL_SHARED and XML_UTIL_STATIC defined, please make up your mind"
#  endif
#  ifndef XML_UTIL_SHARED
#    define XML_UTIL_SHARED
#  endif
#  if defined(XML_UTIL_BUILD_LIB)
#    define XML_UTIL_EXPORT XML_UTIL_DECL_EXPORT
#  else
#    define XML_UTIL_EXPORT XML_UTIL_DECL_IMPORT
#  endif
#else
#  define XML_UTIL_EXPORT
#endif
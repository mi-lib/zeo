/* zeo_export.h */
/* This file was automatically generated. */
/* 2023年  5月 25日 木曜日 06:18:48 JST by zhidao */
#ifndef __ZEO_EXPORT_H__
#define __ZEO_EXPORT_H__
#include <zeda/zeda_compat.h>
#if defined(__WINDOWS__) && !defined(__CYGWIN__)
# if defined(__ZEO_BUILD_DLL__)
#  define __ZEO_EXPORT extern __declspec(dllexport)
#  define __ZEO_CLASS_EXPORT  __declspec(dllexport)
# else
#  define __ZEO_EXPORT extern __declspec(dllimport)
#  define __ZEO_CLASS_EXPORT  __declspec(dllimport)
# endif
#else
# define __ZEO_EXPORT __EXPORT
# define __ZEO_CLASS_EXPORT
#endif
#endif /* __ZEO_EXPORT_H__ */

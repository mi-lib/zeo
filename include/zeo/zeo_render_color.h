/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_color - rendering: color elements
 */

#ifndef __ZEO_RENDER_COLOR_H__
#define __ZEO_RENDER_COLOR_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \struct zRGB
 * \brief color expression with RGB intensity set.
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zRGB ){
  float r; /* red */
  float g; /* green */
  float b; /* blue */
};

/*! \brief black and white RGB set. */
__ZEO_EXPORT const zRGB zrgbblack; /*!< RGB set for black */
__ZEO_EXPORT const zRGB zrgbwhite; /*!< RGB set for white */
#define ZRGBBLACK ( (zRGB *)&zrgbblack )
#define ZRGBWHITE ( (zRGB *)&zrgbwhite )

#define ZRGB_TOL (1.0e-6)

#define zRGBIsZero(rgb) ( zIsTol((rgb)->r,ZRGB_TOL) && zIsTol((rgb)->g,ZRGB_TOL) && zIsTol((rgb)->b,ZRGB_TOL) )

/*! \brief create a set of RGB parameters.
 *
 * zRGBSet() sets \a red, \a green and \a blue to a set of RGB
 * parameters \a rgb.
 * \return
 * zRGBSet() returns a pointer \a rgb.
 */
__ZEO_EXPORT zRGB *zRGBSet(zRGB *rgb, float red, float green, float blue);

/*! \brief copy a set of RGB parameters.
 *
 * zRGBCopy() copies a set of RGB parameters pointed by \a src to the other set pointed by \a dest.
 * \return
 * zRGBCopy() returns the pointer \a dest.
 */
#define _zRGBCopy(src,dest) ( *(dest) = *(src) )
__ZEO_EXPORT zRGB *zRGBCopy(const zRGB *src, zRGB *dest);

#define zRGB2fv(rgb,colorv) do{\
  (colorv)[0] = (rgb)->r;\
  (colorv)[1] = (rgb)->g;\
  (colorv)[2] = (rgb)->b;\
} while(0)

/*! \brief grayscale color */
#define zGS(color)              ( ( (color)->r + (color)->g + (color)->b ) / 3.0 )
#define zGSSet(color,intensity) ( (color)->r = (color)->g = (color)->b = (intensity) )

/*! \brief multiply a set of RGB parameters by another. */
__ZEO_EXPORT zRGB *zRGBMul(const zRGB *rgb1, const zRGB *rgb2, zRGB *rgb);

/*! \brief blend a pair of RGB parameters at a given ratio. */
__ZEO_EXPORT zRGB *zRGBBlend(const zRGB *rgb1, const zRGB *rgb2, double ratio, zRGB *rgb);

/*! \brief decode a string to RGB.
 *
 * zRGBDecodeStr() decodes a string \a str and converts it to a set of RGB parameters. If the
 * string begins from '#', the string is decoded as hexadecimal expression. Otherwise, it is
 * decoded as floating-point value expression.
 *
 * For hexadecimal expression, the string has to be a sequence of 0-9 or a-f/A-F, and will be
 * segmented into three values with the same length. Consequently, the length of the string
 * should be a multiple of three.
 * Ex. "0f3df8" will be a vivid bluish color.
 *
 * For floating-point value expression, the string has to be a sequence of three floating-point
 * values segmented by ':', each of which should be in the range of 0-1. Ex. "0.1:0.3:0.8" will
 * be a smoky bluish color.
 *
 * The result RGB is set where \a rgb points.
 * \return
 * zRGBDecodeStr() returns a pointer \a rgb.
 */
__ZEO_EXPORT zRGB *zRGBDecodeStr(zRGB *rgb, const char *str);

/*! \brief find RGB by a string.
 *
 * zRGBByName() converts a name of color \a name to a set of RGB parameters pointed by \a rgb.
 * The strings available for this function are "black", "red", "green", "blue", "yellow", "cyan",
 * "magenta", and "white".
 *
 * zRGBByStr() converts a string \a str to a set of RGB parameters pointed by \a rgb.
 * The string is either that is readable by zRGBDecodeStr() or the name of color.
 * \return
 * zRGBByName() and zRGBByStr() return the pointer \a rgb.
 * \sa
 * zRGBDecodeStr
 */
__ZEO_EXPORT zRGB *zRGBByName(zRGB *rgb, const char *name);
__ZEO_EXPORT zRGB *zRGBByStr(zRGB *rgb, const char *str);

/*! \brief read a set of RGB from a ZTK format processor. */
__ZEO_EXPORT zRGB *zRGBFromZTK(zRGB *rgb, ZTK *ztk);
/*! \brief add RGB values to a ZTK format processor. */
__ZEO_EXPORT ZTK *zRGBToZTK(const zRGB *rgb, ZTK *ztk);

/*! \brief scan/print of a set of RGB parameters.
 *
 * zRGBFScan() scans three values for RGB from the current
 * position of a file \a fp, and copies them to \a rgb.
 *
 * zRGBFScan() scans three values for RGB from the standard
 * input and copies them to \a rgb.
 *
 * zRGBFPrint() prints a set of RGB parameters \a rgb out
 * to the current position of a file \a fp.
 *
 * zRGBPrint() prints a set of RGB parameters \a rgb out to
 * the standard out.
 * \return
 * zRGBFScan() and zRGBScan() return a pointer \a rgb.
 *
 * zRGBFPrint() and zRGBPrint() return no value.
 */
__ZEO_EXPORT zRGB *zRGBFScan(FILE *fp, zRGB *rgb);
#define zRGBScan(c)  zRGBFScan( stdin, (c) )
__ZEO_EXPORT void zRGBFPrint(FILE *fp, const zRGB *rgb);
#define zRGBPrint(c) zRGBFPrint( stdout, (c) )

/* ********************************************************** */
/* CLASS: zHSV
 * HSV class - expression of color by hue, saturation and value.
 * ********************************************************** */

typedef struct{
  float hue; /* hue: [0,360] */
  float sat; /* saturation */
  float val; /* value */
} zHSV;

__ZEO_EXPORT zHSV *zRGB2HSV(const zRGB *rgb, zHSV *hsv);
__ZEO_EXPORT zRGB *zHSV2RGB(const zHSV *hsv, zRGB *rgb);
__ZEO_EXPORT void zHSVFPrint(FILE *fp, const zHSV *hsv);
#define zHSVPrint(hsv) zHSVFPrint( stdout, hsv )

__END_DECLS

#endif /* __ZEO_RENDER_COLOR_H__ */

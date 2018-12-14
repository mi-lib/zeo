/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_color - color elements
 */

#ifndef __ZEO_COLOR_H__
#define __ZEO_COLOR_H__

#include <zeo/zeo_misc.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zColor
 * color class - color space expression.
 * ********************************************************** */

typedef struct{
  float r; /* red */
  float g; /* green */
  float b; /* blue */
} zRGB;

/* ********************************************************** */
/* CLASS: zRGB
 * RGB class - expression of color with RGB intensity set.
 * ********************************************************** */

/*! \brief black and white RGB set. */
extern const zRGB zrgbblack, zrgbwhite;
#define ZRGBBLACK ( (zRGB *)&zrgbblack )
#define ZRGBWHITE ( (zRGB *)&zrgbwhite )

/*! \brief create and copy a set of RGB parameters.
 *
 * zRGBSet() sets \a red, \a green and \a blue to a set of RGB
 * parameters \a rgb.
 *
 * zRGBCopy() copies a set of RGB parameters pointed by \a src
 * to the other set pointed by \a dest.
 * \return
 * zRGBSet() returns a pointer \a rgb.
 *
 * zRGBCopy() returns no value.
 */
__EXPORT zRGB *zRGBSet(zRGB *rgb, float red, float green, float blue);
#define zRGBCopy(s,d) ( *(d) = *(s) )

#define zRGB2fv(rgb,c) do{\
  (c)[0] = (rgb)->r;\
  (c)[1] = (rgb)->g;\
  (c)[2] = (rgb)->b;\
} while(0)

/*! \brief grayscale color */
#define zGS(c)       ( ( (c)->r + (c)->g + (c)->b ) / 3.0 )
#define zGSSet(c,i)  ( (c)->r = (c)->g = (c)->b = (i) )

/*! \brief multiply a set of RGB parameters by another. */
__EXPORT zRGB *zRGBMul(zRGB *rgb1, zRGB *rgb2, zRGB *rgb);

/*! \brief blend a pair of RGB parameters at a given ratio. */
__EXPORT zRGB *zRGBBlend(zRGB *rgb1, zRGB *rgb2, double ratio, zRGB *rgb);

/*! \brief decode a string to RGB.
 *
 * zRGBDec() decodes a string \a str and converts it to a set of
 * RGB parameters. If the string begins from '#', the string is
 * decoded as hexadecimal expression.
 * Otherwise, it is decoded as floating-point value expression.
 *
 * For hexadecimal expression, the string has to be a sequence
 * of 0-9 or a-f/A-F, and will be segmented into three values
 * with the same length. Consequently, the length of the string
 * should be a multiple of three.
 * Ex. "0f3df8" will be a vivid bluish color.
 *
 * For floating-point value expression, the string has to be
 * a sequence of three floating-point values segmented by ':',
 * each of which should be in the range of 0-1.
 * Ex. "0.1:0.3:0.8" will be a smoky bluish color.
 *
 * The resulted RGB is set where \a rgb points.
 * \return
 * zRGBDec() returns a pointer \a rgb.
 */
__EXPORT zRGB *zRGBDec(zRGB *rgb, char *str);

/*! \brief input/output of a set of RGB parameters.
 *
 * zRGBFRead() reads a sequence of 3 values as the information
 * of RGB from the current position of the file \a fp, and copies
 * them to \a rgb.
 *
 * zRGBFRead() reads information of RGB from the standard input
 * and copies them to \a rgb.
 *
 * zRGBFWrite() writes a set of RGB parameters \a rgb to the
 * current position of the file \a fp.
 *
 * zRGBWrite() writes a set of RGB parameters \a rgb to the
 * standard out.
 * \return
 * zRGBFRead() and zRGBRead() return a pointer \a rgb.
 *
 * zRGBFWrite() and zRGBWrite() return no value.
 */
__EXPORT zRGB *zRGBFRead(FILE *fp, zRGB *rgb);
#define zRGBRead(c)  zRGBFRead( stdin, (c) )
__EXPORT void zRGBFWrite(FILE *fp, zRGB *rgb);
#define zRGBWrite(c) zRGBFWrite( stdout, (c) )

/* METHOD:
 * zRGBFWriteXML - xml output.
 * ... yet testing.
 */
__EXPORT void zRGBFWriteXML(FILE *fp, zRGB *rgb);

/* ********************************************************** */
/* CLASS: zHSV
 * HSV class - expression of color by hue, saturation and value.
 * ********************************************************** */

typedef struct{
  float hue; /* hue: [0,360] */
  float sat; /* saturation */
  float val; /* value */
} zHSV;

__EXPORT zHSV *zRGB2HSV(zRGB *rgb, zHSV *hsv);
__EXPORT zRGB *zHSV2RGB(zHSV *hsv, zRGB *rgb);
__EXPORT void zHSVFWrite(FILE *fp, zHSV *hsv);
#define zHSVWrite(hsv) zHSVFWrite( stdout, hsv )

__END_DECLS

#endif /* __ZEO_COLOR_H__ */

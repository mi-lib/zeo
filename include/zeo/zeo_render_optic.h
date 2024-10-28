/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_optic - rendering: optical properties
 */

#ifndef __ZEO_RENDER_OPTIC_H__
#define __ZEO_RENDER_OPTIC_H__

#include <zeo/zeo_render_color.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief optical characteristic parameters
 *//********************************************************* */
typedef struct{
  Z_NAMED_CLASS;
  zRGB amb;     /*!< \brief coefficients of reflection for ambient */
  zRGB dif;     /*!< \brief coefficients of diffuse reflection */
  zRGB spc;     /*!< \brief coefficients of specular reflection */
  double esr;   /*!< \brief Phong's exponential for specular reflection */
  double sns;   /*!< \brief shininess */
  double alpha; /*!< \brief alpha value */
} zOpticalInfo;

#define zOpticalInfoSetAmb(o,a) zRGBCopy( a, &(o)->amb )
#define zOpticalInfoSetDif(o,d) zRGBCopy( d, &(o)->dif )
#define zOpticalInfoSetSpc(o,s) zRGBCopy( s, &(o)->spc )

#define zOpticalInfoSetESR(o,e)       ( (o)->esr = (e) )
#define zOpticalInfoSetShininess(o,s) ( (o)->sns = (s) )
#define zOpticalInfoSetAlpha(o,a)     ( (o)->alpha = (a) )

/*! \brief create, initialize, copy and destroy a set of optical parameters.
 *
 * zOpticalInfoCreate() creates a set of optical parameters \a oi.
 * \a ar, \a ag and \a ab are for coefficients of ambient reflection.
 * \a dr, \a dg and \a db are for coefficients of diffuse reflection.
 * \a sr, \a sg and \a sb are for coefficients of specular reflection.
 * \a esr is Phong's exponential for specular reflection.
 * \a sns is the shininess.
 * \a alpha is the alpha value (transparent ratio).
 * \a name is a name of the optical set, which is not mandatory
 * (the null pointer works).
 *
 * zOpticalInfoInit() initializes a set of optical parameters \a oi.
 * All parameters will be set for 1.0.
 *
 * zOpticalInfoCopy() copies a set of optical parameters \a src to \a dest.
 *
 * zOpticalInfoDestroy() destroys a set of optical parameters \a oi.
 * \return
 * zOpticalInfoCreate() and zOpticalInfoInit() return a pointer \a oi.
 *
 * zOpticalInfoCopy() returns a pointer \a dest.
 *
 * zOpticalInfoDestroy() returns no value.
 */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoCreate(zOpticalInfo *oi, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double sns, double alpha, char *name);
#define zOpticalInfoCreateSimple(o,r,g,b,n) \
  zOpticalInfoCreate( (o), 0.5*r, 0.5*g, 0.5*b, r, g, b, 0, 0, 0, 0, 0, 1, (n) )
#define zOpticalInfoInit(o) \
  zOpticalInfoCreate( (o), 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, NULL )
__ZEO_EXPORT zOpticalInfo *zOpticalInfoCopy(zOpticalInfo *src, zOpticalInfo *dest);
__ZEO_EXPORT zOpticalInfo *zOpticalInfoClone(zOpticalInfo *src, zOpticalInfo *dest);
#define zOpticalInfoDestroy(o) do{\
  zNameFree(o);\
  zOpticalInfoInit(o);\
} while(0)

/*! \brief multiply a set of optical parameters to another. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoMul(zOpticalInfo *oi1, zOpticalInfo *oi2, zOpticalInfo *oi);

/*! \brief blend a pair of sets of optical parameters at a given ratio. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoBlend(zOpticalInfo *oi1, zOpticalInfo *oi2, double ratio, zOpticalInfo *oi, char *name);

/*! \brief tag to identify optical info. */
#define ZTK_TAG_OPTIC "optic"

/*! \brief read an optical info from a ZTK format processor. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk);
/*! \brief add an optical info to a ZTK format processor. */
__ZEO_EXPORT ZTK *zOpticalInfoToZTK(zOpticalInfo *oi, ZTK *ztk);

/*! \brief print out an optical info to the current stream of a file. */
__ZEO_EXPORT void zOpticalInfoFPrintZTK(FILE *fp, zOpticalInfo *oi);

/*! \struct zOpticalInfoArray
 * \brief array class of a set of optical parameters.
 */
zArrayClass( zOpticalInfoArray, zOpticalInfo );

__END_DECLS

#endif /* __ZEO_RENDER_OPTIC_H__ */

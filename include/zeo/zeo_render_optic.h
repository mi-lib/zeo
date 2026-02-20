/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_render_optic - rendering: optical properties
 */

#ifndef __ZEO_RENDER_OPTIC_H__
#define __ZEO_RENDER_OPTIC_H__

#include <zeo/zeo_render_color.h>

__BEGIN_DECLS

/*! \struct zOpticalInfo
 * \brief optical characteristic parameters
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zOpticalInfo ){
  Z_NAMED_CLASS;
  zRGB ambient;     /*!< \brief coefficients of reflection for ambient */
  zRGB diffuse;     /*!< \brief coefficients of diffuse reflection */
  zRGB specular;    /*!< \brief coefficients of specular reflection */
  double esr;       /*!< \brief Phong's exponential for specular reflection */
  double shininess; /*!< \brief shininess */
  double alpha;     /*!< \brief alpha value */
#ifdef __cplusplus
  zOpticalInfo();
  ~zOpticalInfo();
  zOpticalInfo *init();
  void destroy();
  zOpticalInfo *create(float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double shininess, double alpha, char *name);
  zOpticalInfo *create(float red, float green, float blue, char *name);
  zOpticalInfo *create(float red, float green, float blue);
  zOpticalInfo *copy(zOpticalInfo *dest);
  zOpticalInfo *copy(zOpticalInfo &dest);
  zOpticalInfo *clone(zOpticalInfo *dest);
  zOpticalInfo *clone(zOpticalInfo &dest);
  zOpticalInfo *multiply(const zOpticalInfo *oi1, const zOpticalInfo *oi2);
  zOpticalInfo *blend(const zOpticalInfo *oi1, const zOpticalInfo *oi2, double ratio);
#endif /* __cplusplus */
};

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
__ZEO_EXPORT zOpticalInfo *zOpticalInfoCreate(zOpticalInfo *oi, float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double shininess, double alpha, char *name);
#define zOpticalInfoCreateSimple(o,r,g,b,n) \
  zOpticalInfoCreate( (o), 0.5*r, 0.5*g, 0.5*b, r, g, b, 0, 0, 0, 0, 0, 1, (n) )
#define zOpticalInfoInit(o) \
  zOpticalInfoCreate( (o), 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, NULL )
__ZEO_EXPORT zOpticalInfo *zOpticalInfoCopy(const zOpticalInfo *src, zOpticalInfo *dest);
__ZEO_EXPORT zOpticalInfo *zOpticalInfoClone(const zOpticalInfo *src, zOpticalInfo *dest);
#define zOpticalInfoDestroy(o) do{\
  zNameFree(o);\
  zOpticalInfoInit(o);\
} while(0)

/*! \brief multiply a set of optical parameters to another. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoMul(const zOpticalInfo *oi1, const zOpticalInfo *oi2, zOpticalInfo *oi);

/*! \brief blend a pair of sets of optical parameters at a given ratio. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoBlend(const zOpticalInfo *oi1, const zOpticalInfo *oi2, double ratio, zOpticalInfo *oi, char *name);

/*! \brief tag to identify optical info. */
#define ZTK_TAG_ZEO_OPTIC           "zeo::optic"

#define ZTK_KEY_ZEO_OPTIC_NAME      "name"
#define ZTK_KEY_ZEO_OPTIC_AMBIENT   "ambient"
#define ZTK_KEY_ZEO_OPTIC_DIFFUSE   "diffuse"
#define ZTK_KEY_ZEO_OPTIC_SPECULAR  "specular"
#define ZTK_KEY_ZEO_OPTIC_ESR       "esr"
#define ZTK_KEY_ZEO_OPTIC_SHININESS "shininess"
#define ZTK_KEY_ZEO_OPTIC_ALPHA     "alpha"

/*! \brief read an optical info from a ZTK format processor. */
__ZEO_EXPORT zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk);
/*! \brief add an optical info to a ZTK format processor. */
__ZEO_EXPORT ZTK *zOpticalInfoToZTK(const zOpticalInfo *oi, ZTK *ztk);

/*! \brief print out an optical info to the current stream of a file. */
__ZEO_EXPORT void zOpticalInfoFPrintZTK(FILE *fp, const zOpticalInfo *oi);

/*! \struct zOpticalInfoArray
 * \brief array class of a set of optical parameters.
 */
zArrayClass( zOpticalInfoArray, zOpticalInfo );

__END_DECLS

#ifdef __cplusplus
inline zOpticalInfo::zOpticalInfo(){ this->init(); }
inline zOpticalInfo::~zOpticalInfo(){ this->destroy(); }
inline zOpticalInfo *zOpticalInfo::init(){ zOpticalInfoInit( this ); return this; }
inline void zOpticalInfo::destroy(){ zOpticalInfoDestroy( this ); }
inline zOpticalInfo *zOpticalInfo::create(float ar, float ag, float ab, float dr, float dg, float db, float sr, float sg, float sb, double esr, double shininess, double alpha, char *name){ return zOpticalInfoCreate( this, ar, ag, ab, dr, dg, db, sr, sg, sb, esr, shininess, alpha, name ); }
inline zOpticalInfo *zOpticalInfo::create(float red, float green, float blue, char *name){ return zOpticalInfoCreateSimple( this, red, green, blue, name ); }
inline zOpticalInfo *zOpticalInfo::create(float red, float green, float blue){ return this->create( red, green, blue, NULL ); }
inline zOpticalInfo *zOpticalInfo::copy(zOpticalInfo *dest){ return zOpticalInfoCopy( this, dest ); }
inline zOpticalInfo *zOpticalInfo::copy(zOpticalInfo &dest){ return zOpticalInfoCopy( this, &dest ); }
inline zOpticalInfo *zOpticalInfo::clone(zOpticalInfo *dest){ return zOpticalInfoClone( this, dest ); }
inline zOpticalInfo *zOpticalInfo::clone(zOpticalInfo &dest){ return zOpticalInfoClone( this, &dest ); }
inline zOpticalInfo *zOpticalInfo::multiply(const zOpticalInfo *oi1, const zOpticalInfo *oi2){ return zOpticalInfoMul( oi1, oi2, this ); }
inline zOpticalInfo *zOpticalInfo::blend(const zOpticalInfo *oi1, const zOpticalInfo *oi2, double ratio){ return zOpticalInfoBlend( oi1, oi2, ratio, this, NULL ); }
#endif /* __cplusplus */

#endif /* __ZEO_RENDER_OPTIC_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape - 3D shape
 */

#ifndef __ZEO_SHAPE_H__
#define __ZEO_SHAPE_H__

#include <zeo/zeo_nurbs.h>
#include <zeo/zeo_optic.h>

__BEGIN_DECLS

/*! \brief common methods of shape class.
 */
typedef struct{
  const char *typestr;
  void *(*_init)(void*);
  void *(*_alloc)(void);
  void *(*_clone)(void*);
  void *(*_mirror)(void*,zAxis);
  void (*_destroy)(void*);
  void *(*_xform)(void*,zFrame3D*,void*);
  void *(*_xforminv)(void*,zFrame3D*,void*);
  double (*_closest)(void*,zVec3D*,zVec3D*);
  double (*_pointdist)(void*,zVec3D*);
  bool (*_pointisinside)(void*,zVec3D*,bool);
  double (*_volume)(void*);
  zVec3D *(*_barycenter)(void*,zVec3D*);
  zMat3D *(*_inertia)(void*,zMat3D*);
  void (*_baryinertia)(void*,zVec3D*,zMat3D*);
  zPH3D *(*_toph)(void*,zPH3D*);
  void *(*_parseZTK)(void*,ZTK*);
  void *(*_fscan)(FILE*,void*);
  void (*_fprint)(FILE*,void*);
} zShape3DCom;

/* ********************************************************** */
/* CLASS: zShape3D
 * 3D unit shape class
 * ********************************************************** */

typedef struct{
  Z_NAMED_CLASS
  void *body;
  zOpticalInfo *optic;
  zShape3DCom *com; /* methods */
} zShape3D;

#define zShape3DOptic(s)      (s)->optic
#define zShape3DSetOptic(s,o) ( (s)->optic = (o) )

/*! \brief initialize a 3D shape instance.
 *
 * zShape3DInit() initializes a 3D shape \a shape as a
 * non-shaped instance.
 * \ret a pointer \a shape
 */
__EXPORT zShape3D *zShape3DInit(zShape3D *shape);

/*! \brief destroy a 3D shape instance.
 *
 * zShape3DDestroy() destroys \a shape, freeing its
 * inner parameters.
 */
__EXPORT void zShape3DDestroy(zShape3D *shape);

__EXPORT zShape3D *zShape3DClone(zShape3D *org, zShape3D *cln, zOpticalInfo *oi);
__EXPORT zShape3D *zShape3DMirror(zShape3D *src, zShape3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D shape.
 *
 * zShape3DXform() transforms coordinates of a 3D shape \a src
 * by a frame \a f and puts it into \a dest.
 *
 * zShape3DXformInv() transforms \a src by the inverse of a frame
 * \a f and puts it into \a dest.
 * \return
 * zShape3DXform() and zShape3DXformInv() return a pointer to \a dest.
 */
__EXPORT zShape3D *zShape3DXform(zShape3D *src, zFrame3D *f, zShape3D *dest);
__EXPORT zShape3D *zShape3DXformInv(zShape3D *src, zFrame3D *f, zShape3D *dest);

#define zShape3DContigVert(s,p,d) zPH3DContigVert( zShape3DPH(s), p, d )

/*! \brief check if a point is inside of a shape.
 *
 * zShape3DPointIsInside() checks if a point \a p is inside of
 * a shape \a shape.
 *
 * \a p on the surface of \a shape is judged to be inside of
 * \ashape if the true value is given for \a rim.
 * \return
 * zShape3DPointIsInside() returns the true value if \a p is
 * inside of \a shape, or the false value otherwise.
 * \sa
 * zPH3DPointIsInside
 */
__EXPORT double zShape3DClosest(zShape3D *shape, zVec3D *p, zVec3D *cp);
__EXPORT double zShape3DPointDist(zShape3D *shape, zVec3D *p);
__EXPORT bool zShape3DPointIsInside(zShape3D *shape, zVec3D *p, bool rim);

__EXPORT zShape3D *zShape3DToPH(zShape3D *shape);

#define ZTK_TAG_SHAPE "shape"

/*! \brief default number of division in conversion from smooth curves to polygonal models. */
#define ZEO_SHAPE_DEFAULT_DIV 32

/*! \brief read the number of division for smooth primitives from a ZTK format processor. */
__EXPORT int zShape3DDivFromZTK(ZTK *ztk);

/*! \brief register a definition of tag-and-keys for a 3D shape to a ZTK format processor. */
__EXPORT bool zShape3DRegZTK(ZTK *ztk);
/*! \brief read a 3D shape from a ZTK format processor. */
__EXPORT zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no, ZTK *ztk);

/*! \brief scan and print a 3D shape.
 *
 * zShape3DFScan() scans information of a 3D shape from the
 * current position of a file \a fp and creates a new shape
 * \a shape. An acceptable data file format is as follows.
 *
 *  name : <name>
 *  optic : <optical info>
 *  .
 *  .
 *
 * Since all types of shapes are internally stored as polyhedra,
 * the data description is followed by that of polyhedron.
 * The bracketed parts must be replaced by real numbers.
 * <name> is the identifier of the shape. One can define
 * arbitrary name which does not involve any white spaces
 * or tab charactors.
 *
 * The shapes defined in advance might be referred by
 * others by mirror key. The candidates should be pointed
 * by \a sarray. \a ns is the size of \a sarray.
 *
 * The candidates of optical information set has to be
 * prepared in \a oarray. \a no is the size of \a oarray.
 *
 * zShape3DScan() scans information of a shape from the
 * standard input.
 *
 * zShape3DFPrint() prints information of \a shape out to
 * the current position of a file \a fp in the same format
 * with the above.
 *
 * zShape3DPrint() prints information of \a shape out to
 * the standard output.
 * \return
 * zShape3DFScan() and zShape3DScan() return a pointer \a shape.
 * zShape3DFPrint() and zShape3DPrint() return no value.
 */
__EXPORT zShape3D *zShape3DFScan(FILE *fp, zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no);
#define zShape3DScan(s,sa,ns,oa,no) \
  zShape3DFScan( stdin, (s), (sa), (ns), (oa), (no) )
__EXPORT void zShape3DFPrint(FILE *fp, zShape3D *shape);
#define zShape3DPrint(s) zShape3DFPrint( stdout, (s) )

/*! \struct zShape3DArray
 * \brief array class of 3D shapes.
 */
zArrayClass( zShape3DArray, zShape3D );

__END_DECLS

#include <zeo/zeo_shape_box.h>    /* box */
#include <zeo/zeo_shape_sphere.h> /* sphere */
#include <zeo/zeo_shape_ellips.h> /* ellipsoid */
#include <zeo/zeo_shape_cyl.h>    /* cylinder */
#include <zeo/zeo_shape_ecyl.h>   /* elliptic cylinder */
#include <zeo/zeo_shape_cone.h>   /* cone */
#include <zeo/zeo_shape_ph.h>     /* polyhedron (for class abstraction) */
#include <zeo/zeo_shape_nurbs.h>  /* NURBS (for class abstraction) */

#include <zeo/zeo_shape_list.h>

#endif /* __ZEO_SHAPE_H__ */

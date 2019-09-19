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
  bool (*_regZTK)(ZTK*,char*);
  void *(*_fromZTK)(void*,ZTK*);
  void (*_fprintZTK)(FILE*,void*);
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

/*! \brief assign a method of a 3D shape by referring a string. */
__EXPORT zShape3D *zShape3DQueryAssign(zShape3D *shape, char *str);

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

/*! \struct zShape3DArray
 * \brief array class of 3D shapes.
 */
zArrayClass( zShape3DArray, zShape3D );

#define ZTK_TAG_SHAPE "shape"

/*! \brief default number of division in conversion from smooth curves to polygonal models. */
#define ZEO_SHAPE_DEFAULT_DIV 32

/*! \brief scan the number of division for smooth primitives from a ZTK format processor. */
__EXPORT int zShape3DDivFromZTK(ZTK *ztk);

/*! \brief register a definition of tag-and-keys for a 3D shape to a ZTK format processor. */
__EXPORT bool zShape3DRegZTK(ZTK *ztk, char *tag);

/*! \brief scan a 3D shape from a ZTK format processor.
 *
 * zShape3DFromZTK() makes up a 3D shape \a shape from a ZTK \a ztk
 * parsed by a ZTK format processor. An acceptable ZTK format is as
 * follows.
 *
 *  name: <name of a shape>
 *  optic: <name of optical info>
 *  type: <type of a shape>
 *
 * followed by key fields that describe actual shapes. The bracketed
 * parts must be replaced by strings. <name> is an arbitrary name to
 * identify the shape. It mustn't include white spaces or tab charactors.
 * <type of a shape> has to be chosen from 'box', 'sphere', 'cylinder',
 * 'cone', 'ellips', 'ellipticcylinder', 'polyhedron' and 'nurbs'.
 *
 * An exceptional description can be done by
 *  mirror: <name of a shape> <name of axis>
 * by which another shape <name of a shape> is mirrored along with an
 * axis <name of axis>.
 *
 * \a sarray and \a oarray are an array of 3D shapes and that of optical
 * information sets, respectively, to be referred.
 * \return
 * zShape3DFromZTK() returns a pointer \a shape.
 */
__EXPORT zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3DArray *sarray, zOpticalInfoArray *oarray, ZTK *ztk);

/*! \brief print a 3D shape to a file stream.
 *
 * zShape3DFPrintZTK() prints information of \a shape out to
 * the current position of a file \a fp in ZTK format.
 * \return
 * zShape3DFPrintZTK() returns no value.
 */
__EXPORT void zShape3DFPrintZTK(FILE *fp, zShape3D *shape);

/*! \brief read a 3D shape from a ZTK format file. */
__EXPORT zShape3D *zShape3DReadZTK(zShape3D *shape, char filename[]);

/*! \brief write a 3D shape to a ZTK format file. */
__EXPORT bool zShape3DWriteZTK(zShape3D *shape, char filename[]);

__END_DECLS

#include <zeo/zeo_shape_box.h>    /* box */
#include <zeo/zeo_shape_sphere.h> /* sphere */
#include <zeo/zeo_shape_ellips.h> /* ellipsoid */
#include <zeo/zeo_shape_cyl.h>    /* cylinder */
#include <zeo/zeo_shape_ecyl.h>   /* elliptic cylinder */
#include <zeo/zeo_shape_cone.h>   /* cone */
#include <zeo/zeo_shape_ph.h>     /* polyhedron (for class abstraction) */
#include <zeo/zeo_shape_nurbs.h>  /* NURBS (for class abstraction) */

__BEGIN_DECLS

/* add the handle to the following list when you create a new shape class. */
#define ZEO_SHAPE_COM_ARRAY \
  zShape3DCom *_zeo_shape_com[] = {\
    &zeo_shape3d_ph_com, &zeo_shape3d_box_com,\
    &zeo_shape3d_sphere_com, &zeo_shape3d_ellips_com,\
    &zeo_shape3d_cyl_com, &zeo_shape3d_ecyl_com, &zeo_shape3d_cone_com,\
    &zeo_shape3d_nurbs_com,\
    NULL,\
  }

__END_DECLS

#include <zeo/zeo_shape_list.h>

#endif /* __ZEO_SHAPE_H__ */

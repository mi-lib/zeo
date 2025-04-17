/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d - 3D shape
 */

#ifndef __ZEO_SHAPE3D_H__
#define __ZEO_SHAPE3D_H__

#include <zeo/zeo_nurbs3d.h>
#include <zeo/zeo_render_optic.h>
#include <zeo/zeo_render_texture.h>

__BEGIN_DECLS

/*! \brief common methods of shape class.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zShape3DCom ){
  const char *typestr;
  void *(*_init)(void*);
  void *(*_alloc)(void);
  void *(*_clone)(void*);
  void *(*_mirror)(void*,zAxis);
  void (*_destroy)(void*);
  void *(*_xform)(void*,const zFrame3D*,void*);
  void *(*_xforminv)(void*,const zFrame3D*,void*);
  double (*_closest)(void*,const zVec3D*,zVec3D*);
  double (*_distfrompoint)(void*,const zVec3D*);
  bool (*_pointisinside)(void*,const zVec3D*,double);
  double (*_volume)(void*);
  zVec3D *(*_barycenter)(void*,zVec3D*);
  zMat3D *(*_baryinertia_m)(void*,double,zMat3D*);
  zMat3D *(*_baryinertia_d)(void*,double,zMat3D*);
  zPH3D *(*_toph)(void*,zPH3D*);
  void *(*_fromZTK)(void*,ZTK*);
  void (*_fprintZTK)(FILE*,void*);
};

/* ********************************************************** */
/* CLASS: zShape3D
 * 3D unit shape class
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zShape3D ){
  Z_NAMED_CLASS;
  void *body;
  zOpticalInfo *optic;
  zTexture *texture;
  zShape3DCom *com; /* methods */
};

#define zShape3DOptic(s)        (s)->optic
#define zShape3DSetOptic(s,o)   ( (s)->optic = (o) )
#define zShape3DTexture(s)      (s)->texture
#define zShape3DSetTexture(s,t) ( (s)->texture = (t) )

/*! \brief initialize a 3D shape instance.
 *
 * zShape3DInit() initializes a 3D shape \a shape as a
 * non-shaped instance.
 * \ret a pointer \a shape
 */
__ZEO_EXPORT zShape3D *zShape3DInit(zShape3D *shape);

/*! \brief assign a method of a 3D shape by referring a string. */
__ZEO_EXPORT zShape3D *zShape3DQueryAssign(zShape3D *shape, const char *str);

/*! \brief destroy a 3D shape instance.
 *
 * zShape3DDestroy() destroys \a shape, freeing its
 * inner parameters.
 */
__ZEO_EXPORT void zShape3DDestroy(zShape3D *shape);

__ZEO_EXPORT zShape3D *zShape3DClone(const zShape3D *org, zShape3D *cln, zOpticalInfo *oi);
__ZEO_EXPORT zShape3D *zShape3DMirror(const zShape3D *src, zShape3D *dest, zAxis axis);

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
__ZEO_EXPORT zShape3D *zShape3DXform(const zShape3D *src, const zFrame3D *f, zShape3D *dest);
__ZEO_EXPORT zShape3D *zShape3DXformInv(const zShape3D *src, const zFrame3D *f, zShape3D *dest);

#define zShape3DContigVert(s,p,d) zPH3DContigVert( zShape3DPH(s), p, d )

/*! \brief check if a point is inside of a shape.
 *
 * zShape3DPointIsInside() checks if a point \a p is inside of
 * a shape \a shape. \a margin is a margin of the inside area outward
 * from the boundary of \a shape.
 * \return
 * zShape3DPointIsInside() returns the true value if \a p is inside of
 * \a shape, or the false value otherwise.
 * \sa
 * zPH3DPointIsInside
 */
__ZEO_EXPORT double zShape3DClosest(const zShape3D *shape, const zVec3D *p, zVec3D *cp);
__ZEO_EXPORT double zShape3DDistFromPoint(const zShape3D *shape, const zVec3D *p);
__ZEO_EXPORT bool zShape3DPointIsInside(const zShape3D *shape, const zVec3D *p, double margin);

/*! \brief volume of a 3D shape. */
__ZEO_EXPORT double zShape3DVolume(const zShape3D *shape);
/*! \brief barycenter of a 3D shape. */
__ZEO_EXPORT zVec3D *zShape3DBarycenter(const zShape3D *shape, zVec3D *c);

/*! \brief inertia tensor about barycenter of a 3D shape from mass. */
__ZEO_EXPORT zMat3D *zShape3DBaryInertiaMass(const zShape3D *shape, double mass, zMat3D *inertia);
/*! \brief inertia tensor about barycenter of a 3D shape. */
__ZEO_EXPORT zMat3D *zShape3DBaryInertia(const zShape3D *shape, double density, zMat3D *inertia);
/*! \brief inertia tensor about origin of a 3D shape from mass. */
__ZEO_EXPORT zMat3D *zShape3DInertiaMass(const zShape3D *shape, double mass, zMat3D *inertia);
/*! \brief inertia tensor about origin of a 3D shape. */
__ZEO_EXPORT zMat3D *zShape3DInertia(const zShape3D *shape, double density, zMat3D *inertia);

/*! \brief convert a shape to a polyhedron. */
__ZEO_EXPORT zShape3D *zShape3DToPH(zShape3D *shape);

/*! \brief read a shape from a STL file. */
__ZEO_EXPORT zShape3D *zShape3DReadFileSTL(zShape3D *shape, const char *filename);
/*! \brief read a shape from a PLY file. */
__ZEO_EXPORT zShape3D *zShape3DReadFilePLY(zShape3D *shape, const char *filename);
/*! \brief read a shape from a OBJ file. */
__ZEO_EXPORT zShape3D *zShape3DReadFileOBJ(zShape3D *shape, const char *filename);
#ifdef __ZEO_USE_DAE
/*! \brief read a shape from a DAE file. */
__ZEO_EXPORT zShape3D *zShape3DReadFileDAE(zShape3D *shape, const char *filename);
#endif /* __ZEO_USE_DAE */

/*! \struct zShape3DArray
 * \brief array class of 3D shapes.
 */
zArrayClass( zShape3DArray, zShape3D );

/* ZTK */

#define ZTK_TAG_ZEO_SHAPE           "zeo::shape"

#define ZTK_KEY_ZEO_SHAPE3D_NAME    "name"
#define ZTK_KEY_ZEO_SHAPE3D_TYPE    "type"
#define ZTK_KEY_ZEO_SHAPE3D_OPTIC   "optic"
#define ZTK_KEY_ZEO_SHAPE3D_TEXTURE "texture"
#define ZTK_KEY_ZEO_SHAPE3D_MIRROR  "mirror"
#define ZTK_KEY_ZEO_SHAPE3D_IMPORT  "import"
#define ZTK_KEY_ZEO_SHAPE3D_POS     "pos"
#define ZTK_KEY_ZEO_SHAPE3D_ATT     "att"
#define ZTK_KEY_ZEO_SHAPE3D_ROT     "rot"
#define ZTK_KEY_ZEO_SHAPE3D_FRAME   "frame"

#define ZTK_KEY_ZEO_SHAPE3D_CENTER  "center"
#define ZTK_KEY_ZEO_SHAPE3D_VERT    "vert"
#define ZTK_KEY_ZEO_SHAPE3D_AX      "ax"
#define ZTK_KEY_ZEO_SHAPE3D_AY      "ay"
#define ZTK_KEY_ZEO_SHAPE3D_AZ      "az"
#define ZTK_KEY_ZEO_SHAPE3D_REF     "ref"
#define ZTK_KEY_ZEO_SHAPE3D_DEPTH   "depth"
#define ZTK_KEY_ZEO_SHAPE3D_WIDTH   "width"
#define ZTK_KEY_ZEO_SHAPE3D_HEIGHT  "height"
#define ZTK_KEY_ZEO_SHAPE3D_RADIUS  "radius"
#define ZTK_KEY_ZEO_SHAPE3D_RX      "rx"
#define ZTK_KEY_ZEO_SHAPE3D_RY      "ry"
#define ZTK_KEY_ZEO_SHAPE3D_RZ      "rz"
#define ZTK_KEY_ZEO_SHAPE3D_DIV     "div"

/*! \brief default number of division in conversion from smooth curves to polygonal models. */
#define ZEO_SHAPE_DEFAULT_DIV 32

/*! \brief scan the number of division for smooth primitives from a ZTK format processor. */
__ZEO_EXPORT int zShape3DDivFromZTK(ZTK *ztk);

/*! \brief scan a 3D shape from a ZTK format processor.
 *
 * zShape3DFromZTK() makes up a 3D shape \a shape from a ZTK \a ztk
 * parsed by a ZTK format processor. An acceptable ZTK format is as
 * follows.
 *
 *  name: <name of a shape>
 *  optic: <name of optical info>
 *  texture: <name of a texture>
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
 * \a sarray, \a oarray and \a tarray are arrays of 3D shapes, optical
 * information sets, and textures, respectively, to be referred.
 * \return
 * zShape3DFromZTK() returns a pointer \a shape.
 */
__ZEO_EXPORT zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3DArray *sarray, zOpticalInfoArray *oarray, zTextureArray *tarray, ZTK *ztk);

/*! \brief print a 3D shape to a file stream.
 *
 * zShape3DFPrintZTK() prints information of \a shape out to
 * the current position of a file \a fp in ZTK format.
 * \return
 * zShape3DFPrintZTK() returns no value.
 */
__ZEO_EXPORT void zShape3DFPrintZTK(FILE *fp, const zShape3D *shape);

/*! \brief read a 3D shape from a ZTK format file. */
__ZEO_EXPORT zShape3D *zShape3DReadZTK(zShape3D *shape, const char filename[]);

/*! \brief write a 3D shape to a ZTK format file. */
__ZEO_EXPORT bool zShape3DWriteZTK(const zShape3D *shape, const char filename[]);

/*! \brief read a 3D shape from a file. */
__ZEO_EXPORT zShape3D *zShape3DReadFile(zShape3D *shape, const char filename[]);

__END_DECLS

#include <zeo/zeo_vec3d_data.h>      /* pointcloud */

#include <zeo/zeo_shape3d_box.h>     /* box */
#include <zeo/zeo_shape3d_sphere.h>  /* sphere */
#include <zeo/zeo_shape3d_ellips.h>  /* ellipsoid */
#include <zeo/zeo_shape3d_cyl.h>     /* cylinder */
#include <zeo/zeo_shape3d_capsule.h> /* capsule */
#include <zeo/zeo_shape3d_ecyl.h>    /* elliptic cylinder */
#include <zeo/zeo_shape3d_cone.h>    /* cone */
#include <zeo/zeo_shape3d_ph.h>      /* polyhedron (for class abstraction) */
#include <zeo/zeo_shape3d_nurbs.h>   /* NURBS (for class abstraction) */

__BEGIN_DECLS

/* add the handle to the following list when you create a new shape class. */
#define ZEO_SHAPE_COM_ARRAY \
  zShape3DCom *_zeo_shape_com[] = {\
    &zeo_shape3d_ph_com, &zeo_shape3d_box_com,\
    &zeo_shape3d_sphere_com, &zeo_shape3d_ellips_com,\
    &zeo_shape3d_cyl_com, &zeo_shape3d_capsule_com, &zeo_shape3d_ecyl_com, &zeo_shape3d_cone_com,\
    &zeo_shape3d_nurbs_com,\
    NULL,\
  }

__END_DECLS

#include <zeo/zeo_shape3d_list.h>

#endif /* __ZEO_SHAPE3D_H__ */

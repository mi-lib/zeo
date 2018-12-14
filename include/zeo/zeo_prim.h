/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim - primitive 3D shapes(box, sphere, ellipsoid, cylinder, cone)
 */

#ifndef __ZEO_PRIM_H__
#define __ZEO_PRIM_H__

#include <zeo/zeo_ph.h>

/* a default number of division required in conversion from
 * sphere, ellipsoid, cylinder and cone to polygon model.
 */
#define ZEO_PRIM_DEFAULT_DIV 32

/*! \brief collection of commands for primitives.
 */
typedef struct{
  void *(*_init)(void*);
  void *(*_clone)(void*,void*);
  void *(*_mirror)(void*,void*,zAxis);
  void (*_destroy)(void*);
  void *(*_xfer)(void*,zFrame3D*,void*);
  void *(*_xferinv)(void*,zFrame3D*,void*);
  double (*_closest)(void*,zVec3D*,zVec3D*);
  double (*_pointdist)(void*,zVec3D*);
  bool (*_pointisinside)(void*,zVec3D*,bool);
  double (*_volume)(void*);
  zVec3D *(*_barycenter)(void*,zVec3D*);
  zMat3D *(*_inertia)(void*,zMat3D*);
  void (*_baryinertia)(void*,zVec3D*,zMat3D*);
  zPH3D *(*_toph)(void*,zPH3D*);
  void *(*_fread)(FILE*,void*);
  void (*_fwrite)(FILE*,void*);
} zPrimCom;

#include <zeo/zeo_prim_box.h>    /* box */
#include <zeo/zeo_prim_sphere.h> /* sphere */
#include <zeo/zeo_prim_ellips.h> /* ellipsoid */
#include <zeo/zeo_prim_cyl.h>    /* cylinder */
#include <zeo/zeo_prim_ecyl.h>   /* elliptic cylinder */
#include <zeo/zeo_prim_cone.h>   /* cone */
#include <zeo/zeo_prim_ph.h>     /* polyhedron (for class abstraction) */

#endif /* __ZEO_PRIM_H__ */

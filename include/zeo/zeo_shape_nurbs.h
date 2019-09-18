/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_nurbs - 3D shapes: NURBS (for class abstraction).
 */

#ifndef __ZEO_SHAPE_NURBS_H__
#define __ZEO_SHAPE_NURBS_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* methods for abstraction */
extern zShape3DCom zeo_shape3d_nurbs_com;

#define zShape3DNURBS(s) ( (zNURBS3D*)(s)->body )

__EXPORT zShape3D *zShape3DNURBSAlloc(zShape3D *shape, int size1, int size2, int dim1, int dim2);

__END_DECLS

#endif /* __ZEO_SHAPE_NURBS_H__ */

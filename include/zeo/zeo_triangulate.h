/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_triangulate - trianglation of non-convex.
 */

#ifndef __ZEO_TRIANGULATE_H__
#define __ZEO_TRIANGULATE_H__

#include <zeo/zeo_elem.h>

__BEGIN_DECLS

/*! \brief trianglate a looped series of vertices of a non-convex.
 *
 * zTriangulate() triangulates a looped series of vertices of a non-convex
 * given by an array of 3D vectors \a v. Namely, it divides the non-convex
 * into triangle pieces. \a n is the number of vertices.
 * As the result, a triangle list \a tlist is newly created.
 * \return
 * zTriangulate() returns the number of the generated triangles, which
 * is up to \a n -2.
 */
__EXPORT int zTriangulate(zVec3D v[], int n, zTri3DList *tlist);

__END_DECLS

#endif /* __ZEO_TRIANGULATE_H__ */

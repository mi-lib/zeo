/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_delaunay2d - 2D Delaunay triangularization.
 */

#ifndef __ZEO_DELAUNAY2D_H__
#define __ZEO_DELAUNAY2D_H__

#include <zeo/zeo_bv2d.h>

__BEGIN_DECLS

/*! \brief 2D triangle class for Delaunay triangularization. */
typedef struct _zDTTri2D{
  zTri2D t;
  struct _zDTTri2D *adj[3];
} zDTTri2D;
zListClass( zDTTri2DList, zDTTri2DListCell, zDTTri2D );

/*! \brief print out 2D triangles of a list to the current position of a file (for debug) */
__EXPORT void zDTTri2DListFPrint(FILE *fp, zDTTri2DList *tl);

/*! \brief 2D Delaunay triangularization
 *
 * zDelaunay2D() finds the Delaunay triangularization of a given list of
 * 2D points \a pl. The result is stored as a list of triangles zDTTri2D
 * pointed by \a tl.
 * \return
 * zDelaunay2D() returns the true value if it succeeds. It returns the
 * false value if it fails due to any reason.
 */
__EXPORT bool zDelaunay2D(zVec2DList *pl, zDTTri2DList *tl);

__END_DECLS

#endif /* __ZEO_DELAUNAY2D_H__ */

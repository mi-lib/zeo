/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_voronoi2d - 2D Voronoi diagram.
 */

#ifndef __ZEO_VORONOI2D_H__
#define __ZEO_VORONOI2D_H__

#include <zeo/zeo_bv2d.h>

__BEGIN_DECLS

/* *** 2D Delaunay triangulation *** */

/*! \brief 2D triangle class for Delaunay triangulation. */
typedef struct _zDTTri2D{
  zTri2D t;
  struct _zDTTri2D *adj[3];
} zDTTri2D;
zListClass( zDTTri2DList, zDTTri2DListCell, zDTTri2D );

#define zDTTri2DListDestroy( l ) zListDestroy( zDTTri2DListCell, l )

/*! \brief 2D Delaunay triangulation
 *
 * zDelaunay2D() finds the Delaunay triangulation of a given list of
 * 2D points \a pl. The result is stored as a list of triangles zDTTri2D
 * pointed by \a tl.
 * \return
 * zDelaunay2D() returns the true value if it succeeds. It returns the
 * false value if it fails due to any reason.
 */
__EXPORT bool zDelaunay2D(zVec2DList *pl, zDTTri2DList *tl);

/*! \brief print out 2D triangles of a list to the current position of a file (for debug) */
__EXPORT void zDTTri2DListFPrint(FILE *fp, zDTTri2DList *tl);

/*! \brief print 2D Voronoi diagram from a set of 2D Delaunay triangles */
__EXPORT void zDTTri2DListVD2DFPrint(FILE *fp, zDTTri2DList *tl);

/* *** 2D Voronoi diagram *** */

/*! \brief 2D Voronoi node class vor Voronoi diagram */
typedef struct{
  zVec2D node;    /*!< node */
  zDTTri2D *t[2]; /*!< pointers to triangles from the node to the next */
} zVD2DLoopData;
zListClass( zVD2DLoop, zVD2DLoopCell, zVD2DLoopData );

/*! \brief 2D Voronoi cell class */
typedef struct{
  zVec2D *seed;
  zVD2DLoop loop;
} zVD2DData;
zListClass( zVD2DList, zVD2DCell, zVD2DData );

/*! \brief destroy a list of 2D Voronoi cell */
__EXPORT void zVD2DListDestroy(zVD2DList *vl);

/*! \brief 2D Voronoi diagram
 *
 * zVoronoi2D() finds the Voronoi diagram of a given list of 2D points
 * \a pl. The result is stored as a list of Voronoi cells zVD2DCell
 * pointed by \a vl.
 * \return
 * zVoronoi2D() returns the true value if it succeeds. It returns the
 * false value if it fails due to any reason.
 */
__EXPORT bool zVoronoi2D(zVec2DList *pl, zVD2DList *vl);

/*! \brief print a list of 2D Voronoi cells */
__EXPORT void zVD2DListFPrint(FILE *fp, zVD2DList *vl);

__END_DECLS

#endif /* __ZEO_VORONOI2D_H__ */

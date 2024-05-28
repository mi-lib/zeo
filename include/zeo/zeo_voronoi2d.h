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
typedef struct _zDelaunayTri2D{
  zTri2D t;
  struct _zDelaunayTri2D *adj[3];
} zDelaunayTri2D;
zListClass( zDelaunayTri2DList, zDelaunayTri2DListCell, zDelaunayTri2D );

#define zDelaunayTri2DListDestroy( l ) zListDestroy( zDelaunayTri2DListCell, l )

/*! \brief 2D Delaunay triangulation
 *
 * zDelaunayTriangulate2D() finds the Delaunay triangulation of a given list of
 * 2D points \a pl. The result is stored as a list of triangles zDelaunayTri2D
 * pointed by \a tl.
 * \return
 * zDelaunayTriangulate2D() returns the true value if it succeeds. It returns the
 * false value if it fails due to any reason.
 */
__ZEO_EXPORT bool zDelaunayTriangulate2D(zVec2DList *pl, zDelaunayTri2DList *tl);

/*! \brief print out 2D triangles of a list to the current position of a file (for debug) */
__ZEO_EXPORT void zDelaunayTri2DListFPrint(FILE *fp, zDelaunayTri2DList *tl);

/*! \brief print 2D Voronoi diagram from a set of 2D Delaunay triangles */
__ZEO_EXPORT void zDelaunayTri2DListVoronoi2DFPrint(FILE *fp, zDelaunayTri2DList *tl);

/* *** 2D Voronoi diagram *** */

/*! \brief 2D Voronoi node class vor Voronoi diagram */
typedef struct{
  zVec2D node;    /*!< node */
  zDelaunayTri2D *t[2]; /*!< pointers to triangles from the node to the next */
} zVoronoi2DLoopData;
zListClass( zVoronoi2DLoop, zVoronoi2DLoopCell, zVoronoi2DLoopData );

/*! \brief 2D Voronoi cell class */
typedef struct{
  zVec2D *seed;
  zVoronoi2DLoop loop;
} zVoronoi2DData;
zListClass( zVoronoi2DList, zVoronoi2DCell, zVoronoi2DData );

/*! \brief destroy a list of 2D Voronoi cell */
__ZEO_EXPORT void zVoronoi2DListDestroy(zVoronoi2DList *vl);

/*! \brief 2D Voronoi diagram
 *
 * zVoronoiDiagram2D() finds the Voronoi diagram of a given list of 2D points
 * \a pl. The result is stored as a list of Voronoi cells zVoronoi2DCell
 * pointed by \a vl.
 * \return
 * zVoronoiDiagram2D() returns the true value if it succeeds. It returns the
 * false value if it fails due to any reason.
 */
__ZEO_EXPORT bool zVoronoiDiagram2D(zVec2DList *pl, zVoronoi2DList *vl);

/*! \brief print a list of 2D Voronoi cells */
__ZEO_EXPORT void zVoronoi2DListFPrint(FILE *fp, zVoronoi2DList *vl);

__END_DECLS

#endif /* __ZEO_VORONOI2D_H__ */

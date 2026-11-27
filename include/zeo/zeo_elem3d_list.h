/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_list - 3D shape element list.
 */

#ifndef __ZEO_ELEM3D_LIST_H__
#define __ZEO_ELEM3D_LIST_H__

/* NOTE: never include this header file in user programs. */

/* ********************************************************** */
/*! \brief loop of 3D vertices.
 *//* ******************************************************* */

typedef zVec3DAddrListCell zLoop3DCell;
typedef zVec3DAddrList zLoop3D;

#define zLoop3DAdd(l,c)    zVec3DAddrListAdd( l, c )
#define zLoop3DCreate(l,a) zVec3DAddrListCreate( l, a )
#define zLoop3DDestroy(l)  zVec3DAddrListDestroy( l )

/*! \struct zTri3DList
 * \brief list of 3D triangles.
 */
ZEDA_DEF_LIST_CLASS( zTri3DList, zTri3DListCell, zTri3D );

__BEGIN_DECLS

/*! add a 3D triangle to a list of 3D triangles.
 *
 * zTri3DListAdd() adds a copy of a 3D triangle \a t at the head
 * of a list of 3D triangles \a list.
 * \return
 * zTri3DListAdd() returns a pointer to the newly added cell.
 */
__ZEO_EXPORT zTri3DListCell *zTri3DListAdd(zTri3DList *list, const zTri3D *t);

/*! destroy a list of 3D triangles.
 *
 * zTri3DListDestroy() destroys a list of 3D triangles \a list.
 * \return
 * zTri3DListDestroy() returns no value.
 */
#define zTri3DListDestroy(l) zListDestroy( zTri3DListCell, l )

/*! \brief align triangles to a direction referred by a vector.
 *
 * zTri3DListAlign() aligns directions of all triangles contained in a
 * list of triangles \a list to one referred by a vector \a ref. Namely,
 * if the direction of a triangle in \a list is opposite to \a ref, the
 * triangle is flipped.
 * \return
 * zTri3DListAlign() returns no value.
 */
__ZEO_EXPORT void zTri3DListAlign(zTri3DList *list, const zVec3D *ref);

/*! \brief copy triangles of a list to an array.
 *
 * zTri3DListCopyArray() copies triangles held by a list \a list to an
 * array \a t. \a n is the size of the array. If the size of \a list is
 * larger than \a n, the leftovers are omitted.
 * \return
 * zTri3DListCopyArray() returns no value.
 */
__ZEO_EXPORT void zTri3DListCopyArray(const zTri3DList *list, zTri3D t[], int n);

/* ********************************************************** *
 * triangulation of a simple polygon.
 * ********************************************************** */

/*! \brief triangulate a simple polygon.
 *
 * zLoop3DTriangulate() divides a simple polygon, the vertices of which are given by a loop \a loop,
 * into triangles. zTriangulate() does the same thing for a polygon given by an array of vertices
 * \a array. The result is a newly created list of triangles \a tlist.
 * The polygon can be non-convex, but has to be planar and must not intersect itself. The triangles
 * are oriented in the same direction with the polygon, namely, counterclockwise viewed from the side
 * from which the vertices are counterclockwise. The triangulation is the constrained Delaunay
 * triangulation, which maximizes the minimum angle of the triangles among all triangulations of the
 * polygon, so that thin triangles are avoided as much as possible.
 * \return
 * zLoop3DTriangulate() and zTriangulate() return the number of the triangles, which is the number of
 * vertices minus two for a non-degenerate polygon. They return zero if the polygon has less than three
 * vertices or all the vertices are collinear.
 */
__ZEO_EXPORT int zLoop3DTriangulate(const zLoop3D *loop, zTri3DList *tlist);
__ZEO_EXPORT int zTriangulate(const zVec3DArray *array, zTri3DList *tlist);

__END_DECLS

#endif /* __ZEO_ELEM3D_LIST_H__ */

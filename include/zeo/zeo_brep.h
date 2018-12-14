/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep - B-Rep (boundary representation)
 */

#ifndef __ZEO_BREP_H__
#define __ZEO_BREP_H__

#include <zeo/zeo_ph.h>
#include <zeo/zeo_col.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zBREPVert, zBREPEdge, zBREPFace
 * B-Rep vertex, edge and face class
 * ********************************************************** */

typedef struct{
  zVec3D p;
  double _d; /* for beneath-beyond test */
  void *_p; /* pointer to destination vector */
} zBREPVert;
zListClass( zBREPVertList, zBREPVertListCell, zBREPVert );

#define zBREPVertListCellInit(c,v) do{\
  zVec3DCopy( v, &(c)->data.p );\
  (c)->data._d = 0; /* dummy */\
  (c)->data._p = NULL;\
} while(0)

/*! \brief find vertex cell from a list.
 *
 * zBREPVertListFind() finds a cell which has the same vertex
 * with \a v from a list \a vlist.
 * \retval
 * zBREPVertListFind() returns a pointer to the found
 * cell. Otherwise, the null pointer is returned.
 */
__EXPORT zBREPVertListCell *zBREPVertListFind(zBREPVertList *vlist, zVec3D *v);

typedef struct{
  zBREPVertListCell *v[2];
  void *_v;
} zBREPEdge;
zListClass( zBREPEdgeList, zBREPEdgeListCell, zBREPEdge );

#define zBREPEdgeListCellInit(e,v1,v2) do{\
  (e)->data.v[0] = (v1);\
  (e)->data.v[1] = (v2);\
  (e)->data._v  = NULL;\
} while(0)

/*! \brief find edge cell from a list.
 *
 * zBREPEdgeListFind() finds a cell which has an edge
 * consisting of vertices \a v1 and \a v2 from a list \a elist.
 * \retval
 * zBREPEdgeListFind() returns a pointer to the found
 * cell. Otherwise, the null pointer is returned.
 */
__EXPORT zBREPEdgeListCell *zBREPEdgeListFind(zBREPEdgeList *elist, zBREPVertListCell *v1, zBREPVertListCell *v2);

typedef struct{
  zBREPVertListCell *v[3];
  zBREPEdgeListCell *e[3];
  zVec3D norm; /* to be used. */
} zBREPFace;
zListClass( zBREPFaceList, zBREPFaceListCell, zBREPFace );

/* ********************************************************** */
/* CLASS: zBREP
 * B-Rep class
 * ********************************************************** */

typedef struct{
  zBREPVertList vlist;
  zBREPEdgeList elist;
  zBREPFaceList flist;
} zBREP;

/*! \brief convert a polyhedron to a B-Rep solid.
 *
 * B-Rep solid, an instance of zBREP class, has internal
 * lists of boundaries, namely, vertices, edges and faces.
 * The solid is represented by a set of faces, which
 * consist of three vertices and three edges bounded.
 *
 * zPH3D2BREP() converts a polyhedron \a ph to a B-Rep aolid.
 * The result is stored where \a brep points.
 * \retval
 * zPH3D2BREP() returns a pointer \a brep.
 */
__EXPORT zBREP *zPH3D2BREP(zPH3D *ph, zBREP *brep);

/*! \brief convert polyhedron restricted in a box to B-Rep solid.
 *
 * zPH3D2BREPInBox() converts vertices and faces of a
 * polyhedron \a ph only intersecting with an axis-aligned
 * box \a box to a B-Rep aolid. The result is stored where
 * \a brep points.
 * \retval
 * zPH3D2BREPInBox() returns a pointer \a brep.
 */
__EXPORT zBREP *zPH3D2BREPInBox(zPH3D *ph, zAABox3D *box, zBREP *brep);

/*! \brief convert a B-Rep solid to a polyhedron.
 *
 * zBREP2PH3D() converts \a brep to a polyhedron \a ph.
 * The internal workspaces of \a ph and \a brep are
 * completely independent; destruction of one does not
 * harm the other.
 * \retval
 * zBREP2PH3D() returns a pointer \a ph.
 */
__EXPORT zPH3D *zBREP2PH3D(zBREP *brep, zPH3D *ph);

/*! \brief destroy a B-Rep solid.
 *
 * zBREPDestroy() destroys B-rep solid \a brep, freeing
 * internal boundary list.
 * \retval
 * zBREPDestroy() returns no value.
 */
__EXPORT void zBREPDestroy(zBREP *brep);

__END_DECLS

#include <zeo/zeo_brep_trunc.h> /* B-Rep truncation */
#include <zeo/zeo_brep_bool.h>  /* boolean operations */

#endif /* __ZEO_BREP_H__ */

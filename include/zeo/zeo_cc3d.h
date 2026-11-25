/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_cc3d - 3D cell complex.
 */

#ifndef __ZEO_CC3D_H__
#define __ZEO_CC3D_H__

#include <zeo/zeo_ph3d.h>

__BEGIN_DECLS

/*! \struct zCC3D
 * \brief 3D cell complex.
 *
 * zCC3D, which stands for "Cell Complex", partitions an axis-aligned box called the universe into
 * convex polyhedral cells. Each cell is labeled as either solid or void, and a solid, which might
 * be non-convex, is represented by the union of solid cells. Every face is shared by the two cells
 * on its both sides (or one cell on the boundary of the universe), so that the boundary of the
 * solid, which consists of the faces between solid and void cells, is always closed.
 *
 * Cells are cut by a plane based on the topology-oriented method proposed by Sugihara et al.:
 *   K. Sugihara, M. Iri, H. Inagaki and T. Imai, Topology-Oriented Implementation - An Approach
 *   to Robust Geometric Algorithms, Algorithmica, 27(1):5-20, 2000.
 * The combinatorial structure is kept consistent regardless of numerical errors.
 */
typedef struct{
  zVec3D p;    /*!< position */
  /*! \cond */
  int _stamp;  /* stamp of the side with respect to a cutting plane */
  int _side;   /* side with respect to a cutting plane */
  int _visit;  /* stamp of visit */
  int _on;     /* identifier of the cutting plane on which the vertex was made */
  /*! \endcond */
} zCC3DVert;

/* The boundary of each cell is represented by half-edges. A face has two sides, each of which is
 * a loop of half-edges belonging to the cell on that side. */
typedef struct{
  int org;     /*!< origin vertex */
  int next;    /*!< next half-edge on the same side of the face */
  int prev;    /*!< previous half-edge on the same side of the face */
  int twin;    /*!< opposite half-edge on the adjacent face of the same cell */
  int mate;    /*!< opposite half-edge on the other side of the same face (-1 if none) */
  int face;    /*!< face */
  int side;    /*!< side of the face (0 or 1) */
  /*! \cond */
  int _aux;    /* auxiliary */
  /*! \endcond */
} zCC3DHalfEdge;

typedef struct{
  int cell[2]; /*!< cells behind and in front of the face (-1 for outside of the universe) */
  int he[2];   /*!< a half-edge on each side of the face (-1 for outside of the universe) */
  int tag;     /*!< identifier of the operation that created the face (0 for the universe) */
} zCC3DFace;

typedef struct{
  int he;        /*!< a half-edge on the boundary of the cell */
  bool solid;    /*!< solid or void */
  zAABox3D aabb; /*!< axis-aligned bounding box (might be larger than the cell) */
  /*! \cond */
  bool _cand;    /* candidate to be inside of a polyhedron */
  /*! \endcond */
} zCC3DCell;

/*! \cond */
/* growable array of integers used as workspaces */
typedef struct{
  int *buf;
  int num, cap;
} _zCC3DIntVec;
#define _ZEO_CC3D_WS_NUM 8
/*! \endcond */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zCC3D ){
  zAABox3D universe; /*!< universe */
  double tol;        /*!< tolerance relative to the size of the universe */
  zCC3DVert *vert;   /*!< array of vertices */
  zCC3DHalfEdge *he; /*!< array of half-edges */
  zCC3DFace *face;   /*!< array of faces */
  zCC3DCell *cell;   /*!< array of cells */
  int vertnum;       /*!< number of vertices */
  int henum;         /*!< number of half-edges */
  int facenum;       /*!< number of faces */
  int cellnum;       /*!< number of cells */
  /*! \cond */
  int _vertcap, _hecap, _facecap, _cellcap;
  int _stamp;
  int _tag;
  int _plane;                      /* identifier of the current cutting plane */
  bool _useon;                     /* flag to reuse vertices on the cutting plane */
  int _corner[8];                  /* corners of the universe */
  _zCC3DIntVec _ws[_ZEO_CC3D_WS_NUM]; /* workspaces reused in operations */
  /*! \endcond */
};

#define zCC3DVertNum(cc)       (cc)->vertnum
#define zCC3DFaceNum(cc)       (cc)->facenum
#define zCC3DCellNum(cc)       (cc)->cellnum
#define zCC3DVert(cc,i)        ( &(cc)->vert[i].p )
#define zCC3DFace(cc,i)        ( &(cc)->face[i] )
#define zCC3DCell(cc,i)        ( &(cc)->cell[i] )
#define zCC3DCellIsSolid(cc,i) (cc)->cell[i].solid

/*! \brief initialize, create and destroy a 3D cell complex.
 *
 * zCC3DInit() initializes a 3D cell complex \a cc as empty.
 *
 * zCC3DCreate() creates a 3D cell complex \a cc which consists of a single void cell that occupies
 * the universe \a universe. The universe is automatically enlarged when a polyhedron out of it is
 * merged.
 *
 * zCC3DFromPH3D() creates a 3D cell complex \a cc from a polyhedron \a ph, which has to satisfy
 * the same condition with that for the operations below. The universe is the axis-aligned bounding
 * box of \a ph enlarged by the maximum width of the box on every side.
 * zCC3DFromPH3DConvex() and zCC3DFromPH3DNonconvex() do the same thing for a convex polyhedron and
 * a general polyhedron, respectively, without checking \a ph (see the operations below).
 *
 * zCC3DFromAABox3D() creates a 3D cell complex \a cc whose solid is an axis-aligned box \a box.
 * The universe is \a box enlarged by the maximum width of it on every side. It is useful to compute
 * the intersection of polyhedra efficiently, since zCC3DIntersect() and its versions only process
 * the faces of a polyhedron near the solid. Namely, the intersection of two polyhedra is computed by
 * intersecting the solid made from the intersection of their bounding boxes (see zIntersectPH3DBox())
 * with them one after the other. The box should be slightly enlarged, so that a face of a polyhedron
 * does not lie on that of the box.
 *
 * zCC3DDestroy() destroys \a cc, freeing the internal memory.
 * \return
 * zCC3DInit() returns a pointer \a cc.
 * zCC3DCreate(), zCC3DFromPH3D(), zCC3DFromPH3DConvex(), zCC3DFromPH3DNonconvex() and
 * zCC3DFromAABox3D() return a pointer \a cc if they succeed. Otherwise, they return the null pointer.
 * zCC3DDestroy() returns no value.
 */
__ZEO_EXPORT zCC3D *zCC3DInit(zCC3D *cc);
__ZEO_EXPORT zCC3D *zCC3DCreate(zCC3D *cc, const zAABox3D *universe);
__ZEO_EXPORT zCC3D *zCC3DFromPH3D(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DFromPH3DConvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DFromPH3DNonconvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DFromAABox3D(zCC3D *cc, const zAABox3D *box);
__ZEO_EXPORT void zCC3DDestroy(zCC3D *cc);

/*! \brief operations of a 3D cell complex.
 *
 * The following functions directly modify the solid represented by a 3D cell complex \a cc. A
 * polyhedron \a ph can be non-convex, but has to be closed (see zPH3DIsClosed()), have faces oriented
 * outward, and not intersect itself.
 *
 * zCC3DCutByPlane3D() cuts the solid by a plane \a plane, and leaves the part behind the plane,
 * namely, the opposite side to which the normal vector of \a plane directs.
 *
 * zCC3DMerge() merges \a ph to the solid. If \a ph sticks out of the universe, the universe is
 * enlarged to include \a ph.
 *
 * zCC3DSubtract() subtracts \a ph from the solid.
 *
 * zCC3DIntersect() makes the intersection of the solid and \a ph, namely, cuts the solid by \a ph
 * and leaves the part inside of \a ph.
 *
 * zCC3DMerge(), zCC3DSubtract() and zCC3DIntersect() check if \a ph is closed and has faces oriented
 * outward, and also check if it is convex (see zPH3DIsConvex()) to choose either of the following
 * versions. The self-intersection is not checked.
 *
 * zCC3DMergeConvex(), zCC3DSubtractConvex() and zCC3DIntersectConvex() are versions for a convex
 * polyhedron, and zCC3DMergeNonconvex(), zCC3DSubtractNonconvex() and zCC3DIntersectNonconvex() are
 * those for a general polyhedron. They do not check \a ph, so that they are faster when the condition
 * on \a ph is known in advance, e.g., when the same polyhedron is used repeatedly. The results for
 * \a ph that does not satisfy the condition are not guaranteed. The versions for a general polyhedron
 * also work for a convex polyhedron, but take more time than those for a convex polyhedron.
 * \return
 * These functions return a pointer \a cc if they succeed. If \a ph does not satisfy the condition
 * (only for zCC3DMerge(), zCC3DSubtract() and zCC3DIntersect()), if they fail to allocate memory, or
 * if they find an inconsistency of the internal data, they return the null pointer.
 */
__ZEO_EXPORT zCC3D *zCC3DCutByPlane3D(zCC3D *cc, const zPlane3D *plane);
__ZEO_EXPORT zCC3D *zCC3DMerge(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DMergeConvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DMergeNonconvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DSubtract(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DSubtractConvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DSubtractNonconvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DIntersect(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DIntersectConvex(zCC3D *cc, const zPH3D *ph);
__ZEO_EXPORT zCC3D *zCC3DIntersectNonconvex(zCC3D *cc, const zPH3D *ph);

/*! \brief convert a 3D cell complex to a polyhedron.
 *
 * zCC3DToPH3D() converts the boundary of the solid represented by a 3D cell complex \a cc to a
 * polyhedron \a ph. The boundary consists of the faces between solid and void cells, which are
 * divided into triangles. The result is a closed polyhedron (see zPH3DIsClosed()), which might be
 * non-convex or consist of more than one components. It might have an edge shared by four or more
 * faces, along which two parts of the solid touch each other, and degenerate triangles.
 * \return
 * zCC3DToPH3D() returns a pointer \a ph if it succeeds. If the solid is empty or it fails to
 * allocate memory, it returns the null pointer.
 */
__ZEO_EXPORT zPH3D *zCC3DToPH3D(zCC3D *cc, zPH3D *ph);

/*! \brief cut a solid by a polyhedron.
 *
 * zCC3DCut() cuts the surface of the solid represented by a 3D cell complex \a cc by a polyhedron
 * \a ph, which has to satisfy the same condition with that for zCC3DSubtract(). Namely, it
 * subtracts \a ph from the solid in the same way with zCC3DSubtract(), and converts the boundary of
 * the result except the part made by \a ph to a polyhedron \a cut. \a cut is the part of the
 * surface of the original solid outside of \a ph, which is open in general, e.g., a vessel.
 * The part of the surface on which a face of \a ph lies is included in \a cut.
 * \a cc keeps the result of the subtraction.
 * \return
 * zCC3DCut() returns a pointer \a cut if it succeeds. If \a cut is empty, if \a ph does not satisfy
 * the condition, or if it fails to allocate memory or finds an inconsistency of the internal data,
 * it returns the null pointer.
 */
__ZEO_EXPORT zPH3D *zCC3DCut(zCC3D *cc, const zPH3D *ph, zPH3D *cut);

/*! \brief check the consistency of a 3D cell complex.
 *
 * zCC3DCheck() checks the consistency of the combinatorial structure of a 3D cell complex \a cc.
 * Namely, every cell is a closed polyhedron of genus zero, every face is referred from the cells
 * on its both sides, and the cell complex is not corrupted.
 * \return
 * zCC3DCheck() returns the true value if \a cc is consistent. Otherwise, it returns the false value.
 */
__ZEO_EXPORT bool zCC3DCheck(zCC3D *cc);

__END_DECLS

#endif /* __ZEO_CC3D_H__ */

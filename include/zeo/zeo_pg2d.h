/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_pg2d - 2D polygon.
 */

#ifndef __ZEO_PG2D_H__
#define __ZEO_PG2D_H__

#include <zeo/zeo_vec2d_data.h>

/*! \struct zPG2D
 * \brief 2D polygon class.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zPG2D ){
  zVec2DArray vert_array;
  zTri2DArray face_array;
#ifdef __cplusplus
 public:
  zPG2D();
  ~zPG2D();
  int numVert();
  int numFace();
  zVec2D *vert(int i);
  zTri2D *face(int i);
  zVec2D *faceVert(int i, int j);
  zPG2D *init();
  bool allocVert(int num);
  bool allocFace(int num);
  zPG2D *alloc(int num_vert, int num_face);
  void destroy();
  zPG2D *clone(const zPG2D *src);
  zPG2D *mirror(const zPG2D *src, zAxis axis);
  zPG2D *scale(const zPG2D *src, double scale);
  zPG2D *scale(double scale);
  zPG2D *xform(const zFrame2D *frame, zPG2D *dest);
  zPG2D *xformInv(const zFrame2D *frame, zPG2D *dest);
  const zVec2D *contigVert(const zVec2D *point, double *distance);
  const zVec2D *contigVert(const zVec2D &point, double *distance);
  double closest(const zVec2D *point, zVec2D *closestpoint);
  double closest(const zVec2D &point, zVec2D *closestpoint);
  double distanceFromPoint(const zVec2D *point);
  double distanceFromPoint(const zVec2D &point);
  bool pointIsInside(const zVec2D *point, double margin);
  bool pointIsInside(const zVec2D &point, double margin);
  double area();
  zVec2D *barycenter(zVec2D *center);
  zVec2D barycenter();
#endif
};

__BEGIN_DECLS

#define zPG2DVertArray(pg)    ( &(pg)->vert_array )
#define zPG2DVertNum(pg)      zArraySize(zPG2DVertArray(pg))
#define zPG2DVertBuf(pg)      zArrayBuf(zPG2DVertArray(pg))
#define zPG2DVert(pg,i)       zArrayElemNC(zPG2DVertArray(pg),i)
#define zPG2DSetVertNum(pg,n) ( zPG2DVertNum(pg) = (n) )
#define zPG2DSetVertBuf(pg,b) ( zPG2DVertBuf(pg) = (b) )

#define zPG2DFaceArray(pg)    ( &(pg)->face_array )
#define zPG2DFaceNum(pg)      zArraySize(zPG2DFaceArray(pg))
#define zPG2DFaceBuf(pg)      zArrayBuf(zPG2DFaceArray(pg))
#define zPG2DFace(pg,i)       zArrayElemNC(zPG2DFaceArray(pg),i)
#define zPG2DSetFaceNum(pg,n) ( zPG2DFaceNum(pg) = (n) )
#define zPG2DSetFaceBuf(pg,b) ( zPG2DFaceBuf(pg) = (b) )

#define zPG2DFaceVert(pg,i,j) zTri2DVert(zPG2DFace(pg,i),j)

/*! \brief initialize a 2D polygon.
 *
 * zPG2DInit() initializes a 2D polygon \a pg by nullifying the array of vertices and faces of it.
 * \return
 * zPG2DInit() returns a pointer \a pg.
 */
__ZEO_EXPORT zPG2D *zPG2DInit(zPG2D *pg);

/*! \brief allocate vertices and faces of a 2D polygon.
 *
 * zPG2DAllocVert() allocates memory for the array of vertices with the size \a num_vert of a 2D
 * polygon \a pg.
 *
 * zPG2DAllocFace() allocates memory for the array of faces with the size \a num_face of a 2D
 * polygon \a pg.
 *
 * zPG2DAlloc() allocates memory for the array of vertices with the size \a num_vert and triangular
 * faces with the size \a num_face of \a pg.
 * \return
 * zPG2DAllocVert() and zPG2DAllocFace() return the true value if they succeed to allocate memory.
 * Otherwise, they return the false value.
 *
 * zPG2DAlloc() returns a pointer \a pg if it succeeds to allocate memory. Otherwise, it returns the
 * null pointer.
 */
__ZEO_EXPORT bool zPG2DAllocVert(zPG2D *pg, int num_vert);
__ZEO_EXPORT bool zPG2DAllocFace(zPG2D *pg, int num_face);
__ZEO_EXPORT zPG2D *zPG2DAlloc(zPG2D *pg, int num_vert, int num_face);

/*! \brief destroy a 2D polygon.
 *
 * zPG2DDestroy() destroys a 2D polygon \a pg. Namely, it frees internal memory for vertices and
 * faces of \a pg.
 * \return
 * zPG2DDestroy() returns no value.
 */
__ZEO_EXPORT void zPG2DDestroy(zPG2D *pg);

/*! \brief copy a 2D polygon.
 *
 * zPG2DClone() clones a 2D polygon \a src to \a dest.
 *
 * zPG2DMirror() creates a 2D polygon \a dest which is a clone of \a src with all vertices mirrored
 * about the axis \a axis.
 *
 * zPG2DScale() creates a 2D polygon \a dest which is a clone of \a src with all vertices multiplied
 * by \a scale.
 *
 * zPG2DScaleDRC() directly scales \a pg by multiplying all vertices by \a scale.
 * \return
 * zPG2DClone(), zPG2DMirror(), and zPG2DScale() return a pointer \a dest.
 * zPG2DScaleDRC() returns a pointer \a pg.
 */
__ZEO_EXPORT zPG2D *zPG2DClone(const zPG2D *src, zPG2D *dest);
__ZEO_EXPORT zPG2D *zPG2DMirror(const zPG2D *src, zPG2D *dest, zAxis axis);
__ZEO_EXPORT zPG2D *zPG2DScale(const zPG2D *src, zPG2D *dest, double scale);
__ZEO_EXPORT zPG2D *zPG2DScaleDRC(zPG2D *pg, double scale);

/*! \brief transform coordinates of a 2D polygon.
 *
 * zPG2DXform() transforms coordinates of vertices of a 2D polygon \a src by a frame \a frame.
 * The result is put into \a dest.
 *
 * zPG2DXformInv() transforms coordinates of vertices of \a src by the inverse of a frame \a frame.
 * The result is put into \a dest.
 * \notes
 * \a dest has to have the same size of internal arrays with \a src.
 * \return
 * zPG2DXform() and zPG2DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zPG2D *zPG2DXform(const zPG2D *src, const zFrame2D *frame, zPG2D *dest);
__ZEO_EXPORT zPG2D *zPG2DXformInv(const zPG2D *src, const zFrame2D *frame, zPG2D *dest);

/*! \brief return the contiguous vertex of a 2D polygon to a point.
 *
 * zPG2DContigVert() finds the closest vertex of a 2D polygon to the given point \a point.
 * The distance from \a point to the vertex is stored where \a distance points if it is not the null pointer.
 * \return
 * zPG2DContigVert() returns a pointer to the contiguous vertex of \a pg.
 */
__ZEO_EXPORT const zVec2D *zPG2DContigVert(const zPG2D *pg, const zVec2D *point, double *distance);

/*! \brief check if a point is inside of a 2D polygon.
 *
 * zPG2DClosest() finds the closest point from a point \a point on a 2D polygon \a pg. The result is
 * put into \a closestpoint. If \a point is inside of \a pg, \a point itself is the closest point.
 * zPG2DDistFromPoint() computes the distance from \a point to \a pg.
 *
 * zPG2DPointIsInside() checks if a point \a point is inside of a 2D polygon \a pg. \a margin is a
 * margin of the inside area outward from the boundary of \a pg, namely, \a point is regarded to be
 * inside if the signed distance from the boundary, which is negative inside, is less than \a margin.
 * A negative \a margin shrinks the inside area.
 * \return
 * zPG2DClosest() and zPG2DDistFromPoint() return the distance from \a point to \a pg, which is zero
 * if \a point is inside of \a pg.
 * zPG2DPointIsInside() returns the true value if \a point is inside of \a pg. Otherwise, it returns
 * the false value.
 * \notes
 * \a pg can be non-convex. But, faces of \a pg must not overlap each other.
 * For a non-positive \a margin, zPG2DPointIsInside() regards edges not shared by two faces as the
 * boundary of \a pg, where shared edges are found by the pointers to the vertices. Hence, faces
 * contiguous to each other have to share the vertices in the array of \a pg.
 */
__ZEO_EXPORT double zPG2DClosest(const zPG2D *pg, const zVec2D *point, zVec2D *closestpoint);
__ZEO_EXPORT double zPG2DDistFromPoint(const zPG2D *pg, const zVec2D *point);
__ZEO_EXPORT bool zPG2DPointIsInside(const zPG2D *pg, const zVec2D *point, double margin);

/*! \brief area of a 2D polygon.
 *
 * zPG2DArea() calculates the area of a polygon \a pg.
 * \return
 * zPG2DArea() returns the calculated area.
 */
__ZEO_EXPORT double zPG2DArea(const zPG2D *pg);

/*! \brief barycenter of a 2D polygon.
 *
 * zPG2DBarycenter() calculates the barycenter of a polygon \a pg.
 * The result is put into \a c.
 * \return
 * zPG2DBarycenter() returns a pointer to \a c.
 */
__ZEO_EXPORT zVec2D *zPG2DBarycenter(const zPG2D *pg, zVec2D *c);

__END_DECLS

#ifdef __cplusplus
inline zPG2D::zPG2D(){ zPG2DInit( this ); }
inline zPG2D::~zPG2D(){ zPG2DDestroy( this ); }
inline int zPG2D::numVert(){ return zPG2DVertNum( this ); }
inline int zPG2D::numFace(){ return zPG2DFaceNum( this ); }
inline zVec2D *zPG2D::vert(int i){ return zPG2DVert( this, i ); }
inline zTri2D *zPG2D::face(int i){ return zPG2DFace( this, i ); }
inline zVec2D *zPG2D::faceVert(int i, int j){ return zPG2DFaceVert( this, i, j ); }
inline zPG2D *zPG2D::init(){ return zPG2DInit( this ); }
inline bool zPG2D::allocVert(int num){ return zPG2DAllocVert( this, num ); }
inline bool zPG2D::allocFace(int num){ return zPG2DAllocFace( this, num ); }
inline zPG2D *zPG2D::alloc(int num_vert, int num_face){ return zPG2DAlloc( this, num_vert, num_face ); }
inline void zPG2D::destroy(){ zPG2DDestroy( this ); }
inline zPG2D *zPG2D::clone(const zPG2D *src){ return zPG2DClone( src, this ); }
inline zPG2D *zPG2D::mirror(const zPG2D *src, zAxis axis = zY){ return zPG2DMirror( src, this, axis ); }
inline zPG2D *zPG2D::scale(const zPG2D *src, double scale){ return zPG2DScale( src, this, scale ); }
inline zPG2D *zPG2D::scale(double scale){ return zPG2DScaleDRC( this, scale ); }
inline zPG2D *zPG2D::xform(const zFrame2D *frame, zPG2D *dest){ return zPG2DXform( this, frame, dest ); }
inline zPG2D *zPG2D::xformInv(const zFrame2D *frame, zPG2D *dest){ return zPG2DXformInv( this, frame, dest ); }
inline const zVec2D *zPG2D::contigVert(const zVec2D *point, double *distance = NULL){ return zPG2DContigVert( this, point, distance ); }
inline const zVec2D *zPG2D::contigVert(const zVec2D &point, double *distance = NULL){ return zPG2DContigVert( this, &point, distance ); }
inline double zPG2D::closest(const zVec2D *point, zVec2D *closestpoint){ return zPG2DClosest( this, point, closestpoint ); }
inline double zPG2D::closest(const zVec2D &point, zVec2D *closestpoint){ return zPG2DClosest( this, &point, closestpoint ); }
inline double zPG2D::distanceFromPoint(const zVec2D *point){ return zPG2DDistFromPoint( this, point ); }
inline double zPG2D::distanceFromPoint(const zVec2D &point){ return zPG2DDistFromPoint( this, &point ); }
inline bool zPG2D::pointIsInside(const zVec2D *point, double margin = zTOL){ return zPG2DPointIsInside( this, point, margin ); }
inline bool zPG2D::pointIsInside(const zVec2D &point, double margin = zTOL){ return zPG2DPointIsInside( this, &point, margin ); }
inline double zPG2D::area(){ return zPG2DArea( this ); }
inline zVec2D *zPG2D::barycenter(zVec2D *center){ return zPG2DBarycenter( this, center ); }
inline zVec2D zPG2D::barycenter(){ zVec2D center; zPG2DBarycenter( this, &center ); return center; }
#endif

#endif /* __ZEO_PG2D_H__ */

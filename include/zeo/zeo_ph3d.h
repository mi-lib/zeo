/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph3d - polyhedron
 */

#ifndef __ZEO_PH3D_H__
#define __ZEO_PH3D_H__

#include <zeo/zeo_vec3d_data.h>

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D polyhedron class.
 *//* ******************************************************* */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zPH3D ){
  zVec3DArray vert_array;
  zTri3DArray face_array;
#ifdef __cplusplus
 public:
  zPH3D();
  ~zPH3D();
  int numVert();
  int numFace();
  zVec3D *vert(int i);
  zTri3D *face(int i);
  zVec3D *faceVert(int i, int j);
  zVec3D *faceNorm(int i);
  zPH3D *init();
  bool allocVert(int num);
  bool allocFace(int num);
  zPH3D *alloc(int num_vert, int num_face);
  void destroy();
  zPH3D *clone(const zPH3D *src);
  zPH3D *mirror(const zPH3D *src, zAxis axis);
  zPH3D *scale(const zPH3D *src, double scale);
  zPH3D *scale(double scale);
  zPH3D *flip(const zPH3D *src);
  zPH3D *flip();
  zPH3D *xform(const zFrame3D *frame, zPH3D *dest);
  zPH3D *xformInv(const zFrame3D *frame, zPH3D *dest);
  const zVec3D *contigVert(const zVec3D *point, double *distance);
  const zVec3D *contigVert(const zVec3D &point, double *distance);
  double closest(const zVec3D *point, zVec3D *closestpoint);
  double closest(const zVec3D &point, zVec3D *closestpoint);
  double distanceFromPoint(const zVec3D *point);
  double distanceFromPoint(const zVec3D &point);
  bool pointIsInside(const zVec3D *point, double margin);
  bool pointIsInside(const zVec3D &point, double margin);
  double volume();
  zVec3D *barycenter(zVec3D *center);
  zVec3D barycenter();
  zMat3D *inertia(double density, zMat3D *_inertia);
  zMat3D inertia(double density);
  zMat3D *inertiaMass(double mass, zMat3D *_inertia);
  zMat3D inertiaMass(double mass);
  zMat3D *baryInertia(double density, zMat3D *_inertia);
  zMat3D baryInertia(double density);
  zMat3D *baryInertiaMass(double mass, zMat3D *_inertia);
  zMat3D baryInertiaMass(double mass);
  zAABox3D *aabb(zAABox3D *box);
  zAABox3D aabb();
  zPH3D *createPrism(const zVec3D bottom[], int n, const zVec3D *shift);
  zPH3D *createPyramid(const zVec3D bottom[], int n, const zVec3D *vert);
  zPH3D *createTorus(const zVec3D loop[], int n, int div, const zVec3D *center, const zVec3D *axis);
  zPH3D *createLathe(const zVec3D rim[], int n, int div, const zVec3D *center, const zVec3D *axis);
  zPH3D *fromZTK(ZTK *ztk);
  void fprintZTK(FILE *fp = stdout);
#endif
};

#define zPH3DVertArray(ph)    ( &(ph)->vert_array )
#define zPH3DVertNum(ph)      zArraySize(zPH3DVertArray(ph))
#define zPH3DVertBuf(ph)      zArrayBuf(zPH3DVertArray(ph))
#define zPH3DVert(ph,i)       zArrayElemNC(zPH3DVertArray(ph),i)
#define zPH3DSetVertNum(ph,n) ( zPH3DVertNum(ph) = (n) )
#define zPH3DSetVertBuf(ph,b) ( zPH3DVertBuf(ph) = (b) )

#define zPH3DFaceArray(ph)    ( &(ph)->face_array )
#define zPH3DFaceNum(ph)      zArraySize(zPH3DFaceArray(ph))
#define zPH3DFaceBuf(ph)      zArrayBuf(zPH3DFaceArray(ph))
#define zPH3DFace(ph,i)       zArrayElemNC(zPH3DFaceArray(ph),i)
#define zPH3DSetFaceNum(ph,n) ( zPH3DFaceNum(ph) = (n) )
#define zPH3DSetFaceBuf(ph,b) ( zPH3DFaceBuf(ph) = (b) )

#define zPH3DFaceVert(ph,i,j) zTri3DVert(zPH3DFace(ph,i),j)
#define zPH3DFaceNorm(ph,i)   zTri3DNorm(zPH3DFace(ph,i))

/*! \brief initialize a 3D polyhedron.
 *
 * zPH3DInit() initializes a 3D polyhedron \a ph by nullifying the array of vertices and faces of it.
 * \return
 * zPH3DInit() returns a pointer \a ph.
 */
__ZEO_EXPORT zPH3D *zPH3DInit(zPH3D *ph);

/*! \brief allocate vertices and faces of a 3D polyhedron.
 *
 * zPH3DAllocVert() allocates memory for the array of vertices with the size \a num_vert of a 3D
 * polyhedron \a ph.
 *
 * zPH3DAllocFace() allocates memory for the array of faces with the size \a num_face of a 3D
 * polyhedron \a ph.
 *
 * zPH3DAlloc() allocates memory for the array of vertices with the size \a num_vert and triangular
 * faces with the size \a num_face of \a ph.
 * \return
 * zPH3DAllocVert() and zPH3DAllocFace() return the true value if they succeed to allocate memory.
 * Otherwise, they return the false value.
 *
 * zPH3DAlloc() return a pointer \a ph if it succeeds to allocate memory. Otherwise, it returns the
 * null pointer.
 */
__ZEO_EXPORT bool zPH3DAllocVert(zPH3D *ph, int num_vert);
__ZEO_EXPORT bool zPH3DAllocFace(zPH3D *ph, int num_face);
__ZEO_EXPORT zPH3D *zPH3DAlloc(zPH3D *ph, int num_vert, int num_face);

/*! \brief destroy a 3D polyhedron.
 *
 * zPH3DDestroy() destroys a 3D polyhedron \a ph. Namely, it frees internal memory for vertices and
 * faces of \a ph.
 * \return
 * zPH3DDestroy() returns no value.
 */
__ZEO_EXPORT void zPH3DDestroy(zPH3D *ph);

/*! \brief copy a 3D polyhedron.
 *
 * zPH3DClone() clones a 3D polyhedron \a src to \a dest.
 *
 * zPH3DMirror() creates a 3D polylhedron \a dest which is a clone of \a src with all vertices mirrored
 * about the axis \a axis.
 *
 * zPH3DScale() creates a 3D polylhedron \a dest which is a clone of \a src with all vertices multiplied
 * by \a scale.
 *
 * zPH3DFlip() creates a 3D polylhedron \a dest which is a clone of \a src with all faces flipped.
 *
 * zPH3DScaleDRC() directly scales \a ph by multiplying all vertices by \a scale.
 *
 * zPH3DFlipDRC() directly flips all faces of \a ph.
 * \a dest.
 * \return
 * zPH3DClone(), zPH3DMirror(), zPH3DScale(), and zPH3DFlip() return a pointer \a dest.
 * zPH3DScaleDRC() and zPH3DFlipDRC() return a pointer \a ph.
 */
__ZEO_EXPORT zPH3D *zPH3DClone(const zPH3D *src, zPH3D *dest);
__ZEO_EXPORT zPH3D *zPH3DMirror(const zPH3D *src, zPH3D *dest, zAxis axis);
__ZEO_EXPORT zPH3D *zPH3DScale(const zPH3D *src, zPH3D *dest, double scale);
__ZEO_EXPORT zPH3D *zPH3DFlip(const zPH3D *src, zPH3D *dest);
__ZEO_EXPORT zPH3D *zPH3DScaleDRC(zPH3D *ph, double scale);
__ZEO_EXPORT zPH3D *zPH3DFlipDRC(zPH3D *ph);

/*! \brief transform coordinates of a 3D polyhedron.
 *
 * zPH3DXform() transforms coordinates of vertices of a 3D polyhedron \a src by a frame \a frame.
 * The result is put into \a dest.
 *
 * zPH3DXformInv() transforms coordinates of vertices of \a src by the inverse of a frame \a frame.
 * The result is put into \a dest.
 * \notes
 * \a dest has to have the same size of internal arrays with \a src.
 * \return
 * zPH3DXform() and zPH3DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zPH3D *zPH3DXform(const zPH3D *src, const zFrame3D *frame, zPH3D *dest);
__ZEO_EXPORT zPH3D *zPH3DXformInv(const zPH3D *src, const zFrame3D *frame, zPH3D *dest);

/*! \brief return the contiguous vertex of a 3D polyhedron to a point.
 *
 * zPH3DContigVert() finds the closest vertex of a 3D polyhedron to the given point \a point.
 * The distance from \a point to the vertex is stored where \a d points if it is not the null pointer.
 * \return
 * zPH3DContigVert() returns a pointer to the contiguous vertex of \a ph.
 */
__ZEO_EXPORT const zVec3D *zPH3DContigVert(const zPH3D *ph, const zVec3D *point, double *distance);

/*! \brief check if a point is inside of a 3D polyhedron.
 *
 * zPH3DClosest() finds the closest point from a point \a point on a 3D polyhedron \a ph. The result is
 * put into \a closestpoint.
 *
 * zPH3DPointIsInside() checks if a point \a point is inside of a 3D polyhedron \a ph. \a margin is a
 * margin of the inside area outward from the boundary of \a ph.
 * \return
 * zPH3DPointIsInside() returns the true value if \a point is inside of \a ph. Otherwise, it returns
 * the false value.
 * \notes
 * zPH3DPointIsInside() assumes that \a ph is convex, \a ph is closed, and the normal vectors of all facets
 * of \a ph direct outward.
 */
__ZEO_EXPORT double zPH3DClosest(const zPH3D *ph, const zVec3D *point, zVec3D *closestpoint);
__ZEO_EXPORT double zPH3DDistFromPoint(const zPH3D *ph, const zVec3D *point);
__ZEO_EXPORT bool zPH3DPointIsInside(const zPH3D *ph, const zVec3D *point, double margin);

/*! \brief volume of a 3D polyhedron.
 *
 * zPH3DVolume() calculates the volume of a polyhedron \a ph.
 * \return
 * zPH3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zPH3DVolume(const zPH3D *ph);

/*! \brief barycenter of a 3D polyhedron.
 *
 * zPH3DBarycenter() calculates the barycenter of a polyhedron \a ph.
 * The result is put into \a c.
 * \return
 * zPH3DBarycenter() returns a pointer to \a c.
 */
__ZEO_EXPORT zVec3D *zPH3DBarycenter(const zPH3D *ph, zVec3D *c);

/*! \brief inertia tensor of a 3D polyhedron.
 *
 * zPH3DInertia() calculates the inertial tensor of a polyhedron \a ph about the original point, given
 * its density by \a density.
 * zPH3DInertiaMass() calculates the inertia tensor of \a ph about the original point, given its mass
 * by \a mass instead of density.
 *
 * zPH3DBaryInertia() calculates the inertia tensor about barycenter of \a ph, given its density by
 * \a density.
 * zPH3DBaryInertiaMass() calculates the inertia tensor about barycenter of \a ph, given its mass by
 * \a mass instead of density.
 *
 * These four functions put the result into \a inertia.
 * \return
 * zPH3DInertia(), zPH3DInertiaMass(), zPH3DBaryInertia() and zPH3DBaryInertiaMass() return a pointer
 * \a inertia.
 */
__ZEO_EXPORT zMat3D *zPH3DInertia(const zPH3D *ph, double density, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zPH3DInertiaMass(const zPH3D *ph, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zPH3DBaryInertia(const zPH3D *ph, double density, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zPH3DBaryInertiaMass(const zPH3D *ph, double mass, zMat3D *inertia);

/*! \brief axis-aligned bounding box of a 3D polyhedron. */
__ZEO_EXPORT zAABox3D *zPH3DAABB(const zPH3D *ph, zAABox3D *aabb);

/* solid modeling */

/*! \brief create prism and pyramid by extrusion.
 *
 * zPH3DCreatePrism() creates a prism by extruding the bottom loop. The bottom face to be extruded forms
 * a loop of \a n vertices given by \a bottom.
 * \a shift is a shifting vector of the bottom for extrusion.
 * The result is put into \a prism.
 *
 * zPH3DCreatePyramid() creates a pyramid from the vertex \a vert and the bottom face to be extruded forms
 * a loop of \a n vertices given by \a bottom.
 * The result is put into \a pyr.
 * \notes
 * A non-convex shape is acceptable for the bottom to these functions, while crossing loops are not
 * acceptable.
 * \return
 * zPH3DCreatePrism() returns a pointer \a prism.
 * zPH3DCreatePyramid() returns a pointer \a pyr.
 */
__ZEO_EXPORT zPH3D *zPH3DCreatePrism(zPH3D *prism, const zVec3D bottom[], int n, const zVec3D *shift);
__ZEO_EXPORT zPH3D *zPH3DCreatePyramid(zPH3D *pyr, const zVec3D bottom[], int n, const zVec3D *vert);

/*! \brief create solid revolution.
 *
 * zPH3DCreateTorus() creates a torus which has a cross-section represented by a loop of \a n vertices
 * given by \a loop. The result is put into \a torus.
 *
 * zPH3DCreateLathe() creates a solid revolution which has a rim represented by \a rim with \a n vertices.
 * The result is put into \a lathe. It is different from zPH3DCreateTorus() at a point that \a rim does
 * not have to be a closed loop.
 *
 * For the both functions, the cross-section is revolved about the unit axis vector \a axis which passes
 * the point \a center. \a div is the number of division of rotation.
 * \return
 * zPH3DCreateTorus() returns a pointer \a torus.
 * zPH3DCreateLathe() returns a pointer \a lathe.
 * \notes
 * These functions do not check the validity of the shape of cross-sections.
 */
__ZEO_EXPORT zPH3D *zPH3DCreateTorus(zPH3D *torus, const zVec3D loop[], int n, int div, const zVec3D *center, const zVec3D *axis);
__ZEO_EXPORT zPH3D *zPH3DCreateLathe(zPH3D *lathe, const zVec3D rim[], int n, int div, const zVec3D *center, const zVec3D *axis);

/*! \brief flag to print status while reading polyhedral model from a file. */
__ZEO_EXPORT bool zeo_ph_echo_while_reading;
#define zPH3DEchoOn()  ( zeo_ph_echo_while_reading = true )
#define zPH3DEchoOff() ( zeo_ph_echo_while_reading = false )

/* ZTK */

#define ZTK_KEY_ZEO_PH_VERT    "vert"
#define ZTK_KEY_ZEO_PH_FACE    "face"
#define ZTK_KEY_ZEO_PH_LOOP    "loop"
#define ZTK_KEY_ZEO_PH_PRISM   "prism"
#define ZTK_KEY_ZEO_PH_PYRAMID "pyramid"

/*! \brief read a 3D polyhedron from a ZTK format processor. */
__ZEO_EXPORT zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk);

__ZEO_EXPORT void zPH3DFPrintZTK(FILE *fp, const zPH3D *ph);

__END_DECLS

#ifdef __cplusplus
inline zPH3D::zPH3D(){ zPH3DInit( this ); }
inline zPH3D::~zPH3D(){ zPH3DDestroy( this ); }
inline int zPH3D::numVert(){ return zPH3DVertNum( this ); }
inline int zPH3D::numFace(){ return zPH3DFaceNum( this ); }
inline zVec3D *zPH3D::vert(int i){ return zPH3DVert( this, i ); }
inline zTri3D *zPH3D::face(int i){ return zPH3DFace( this, i ); }
inline zVec3D *zPH3D::faceVert(int i, int j){ return zPH3DFaceVert( this, i, j ); }
inline zVec3D *zPH3D::faceNorm(int i){ return zPH3DFaceNorm( this, i ); }
inline zPH3D *zPH3D::init(){ return zPH3DInit( this ); }
inline bool zPH3D::allocVert(int num){ return zPH3DAllocVert( this, num ); }
inline bool zPH3D::allocFace(int num){ return zPH3DAllocFace( this, num ); }
inline zPH3D *zPH3D::alloc(int num_vert, int num_face){ return zPH3DAlloc( this, num_vert, num_face ); }
inline void zPH3D::destroy(){ zPH3DDestroy( this ); }
inline zPH3D *zPH3D::clone(const zPH3D *src){ return zPH3DClone( src, this ); }
inline zPH3D *zPH3D::mirror(const zPH3D *src, zAxis axis = zY){ return zPH3DMirror( src, this, axis ); }
inline zPH3D *zPH3D::scale(const zPH3D *src, double scale){ return zPH3DScale( src, this, scale ); }
inline zPH3D *zPH3D::scale(double scale){ return zPH3DScaleDRC( this, scale ); }
inline zPH3D *zPH3D::flip(const zPH3D *src){ return zPH3DFlip( src, this ); }
inline zPH3D *zPH3D::flip(){ return zPH3DFlipDRC( this ); }
inline zPH3D *zPH3D::xform(const zFrame3D *frame, zPH3D *dest){ return zPH3DXform( this, frame, dest ); }
inline zPH3D *zPH3D::xformInv(const zFrame3D *frame, zPH3D *dest){ return zPH3DXformInv( this, frame, dest ); }
inline const zVec3D *zPH3D::contigVert(const zVec3D *point, double *distance = NULL){ return zPH3DContigVert( this, point, distance ); }
inline const zVec3D *zPH3D::contigVert(const zVec3D &point, double *distance = NULL){ return zPH3DContigVert( this, &point, distance ); }
inline double zPH3D::closest(const zVec3D *point, zVec3D *closestpoint){ return zPH3DClosest( this, point, closestpoint ); }
inline double zPH3D::closest(const zVec3D &point, zVec3D *closestpoint){ return zPH3DClosest( this, &point, closestpoint ); }
inline double zPH3D::distanceFromPoint(const zVec3D *point){ return zPH3DDistFromPoint( this, point ); }
inline double zPH3D::distanceFromPoint(const zVec3D &point){ return zPH3DDistFromPoint( this, &point ); }
inline bool zPH3D::pointIsInside(const zVec3D *point, double margin = zTOL){ return zPH3DPointIsInside( this, point, margin ); }
inline bool zPH3D::pointIsInside(const zVec3D &point, double margin = zTOL){ return zPH3DPointIsInside( this, &point, margin ); }
inline double zPH3D::volume(){ return zPH3DVolume( this ); }
inline zVec3D *zPH3D::barycenter(zVec3D *center){ return zPH3DBarycenter( this, center ); }
inline zVec3D zPH3D::barycenter(){ zVec3D center; zPH3DBarycenter( this, &center ); return center; }
inline zMat3D *zPH3D::inertia(double density, zMat3D *_inertia){ return zPH3DInertia( this, density, _inertia ); }
inline zMat3D zPH3D::inertia(double density){ zMat3D _inertia; zPH3DInertia( this, density, &_inertia ); return _inertia; }
inline zMat3D *zPH3D::inertiaMass(double mass, zMat3D *_inertia){ return zPH3DInertiaMass( this, mass, _inertia ); }
inline zMat3D zPH3D::inertiaMass(double mass){ zMat3D _inertia; zPH3DInertiaMass( this, mass, &_inertia ); return _inertia; }
inline zMat3D *zPH3D::baryInertia(double density, zMat3D *_inertia){ return zPH3DBaryInertia( this, density, _inertia ); }
inline zMat3D zPH3D::baryInertia(double density){ zMat3D _inertia; zPH3DBaryInertia( this, density, &_inertia ); return _inertia; }
inline zMat3D *zPH3D::baryInertiaMass(double mass, zMat3D *_inertia){ return zPH3DBaryInertiaMass( this, mass, _inertia ); }
inline zMat3D zPH3D::baryInertiaMass(double mass){ zMat3D _inertia; zPH3DBaryInertiaMass( this, mass, &_inertia ); return _inertia; }
inline zAABox3D *zPH3D::aabb(zAABox3D *box){ return zPH3DAABB( this, box ); }
inline zAABox3D zPH3D::aabb(){ zAABox3D box; zPH3DAABB( this, &box ); return box; }
inline zPH3D *zPH3D::createPrism(const zVec3D bottom[], int n, const zVec3D *shift){ return zPH3DCreatePrism( this, bottom, n, shift ); }
inline zPH3D *zPH3D::createPyramid(const zVec3D bottom[], int n, const zVec3D *vert){ return zPH3DCreatePyramid( this, bottom, n, vert ); }
inline zPH3D *zPH3D::createTorus(const zVec3D loop[], int n, int div, const zVec3D *center, const zVec3D *axis){ return zPH3DCreateTorus( this, loop, n, div, center, axis ); }
inline zPH3D *zPH3D::createLathe(const zVec3D rim[], int n, int div, const zVec3D *center, const zVec3D *axis){ return zPH3DCreateLathe( this, rim, n, div, center, axis ); }
inline zPH3D *zPH3D::fromZTK(ZTK *ztk){ return zPH3DFromZTK( this, ztk ); }
inline void zPH3D::fprintZTK(FILE *fp){ zPH3DFPrintZTK( fp, this ); }
#endif

#include <zeo/zeo_ph3d_stl.h>
#include <zeo/zeo_ph3d_ply.h>
#include <zeo/zeo_ph3d_obj.h>

#ifdef __ZEO_USE_DAE
#include <zeo/zeo_ph3d_dae.h>
#endif /* __ZEO_USE_DAE */

#endif /* __ZEO_PH3D_H__ */

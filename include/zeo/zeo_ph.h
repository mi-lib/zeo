/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph - polyhedron
 */

#ifndef __ZEO_PH_H__
#define __ZEO_PH_H__

#include <zeo/zeo_elem.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zPH3D
 * 3D polyhedron class
 * ********************************************************** */

typedef struct{
  zVec3DArray vert;
  zTri3DArray face;
} zPH3D;

#define zPH3DVertNum(ph)      zArraySize(&(ph)->vert)
#define zPH3DVertBuf(ph)      zArrayBuf(&(ph)->vert)
#define zPH3DVert(ph,i)       zArrayElemNC(&(ph)->vert,i)
#define zPH3DSetVertNum(ph,n) ( zArraySize(&(ph)->vert) = (n) )
#define zPH3DSetVertBuf(ph,b) ( zArrayBuf(&(ph)->vert) = (b) )

#define zPH3DFaceNum(ph)      zArraySize(&(ph)->face)
#define zPH3DFaceBuf(ph)      zArrayBuf(&(ph)->face)
#define zPH3DFace(ph,i)       zArrayElemNC(&(ph)->face,i)
#define zPH3DSetFaceNum(ph,n) ( zArraySize(&(ph)->face) = (n) )
#define zPH3DSetFaceBuf(ph,b) ( zArrayBuf(&(ph)->face) = (b) )

#define zPH3DFaceVert(ph,i,j) zTri3DVert(zPH3DFace(ph,i),j)
#define zPH3DFaceNorm(ph,i)   zTri3DNorm(zPH3DFace(ph,i))

/*! \brief initialize, copy and destroy a 3D polyhedron.
 *
 * zPH3DInit() initializes a 3D polyhedron \a ph by nullifying
 * the array of vertices and faces in it.
 *
 * zPH3DAlloc() allocates memory for the array of vertices with
 * the size \a vn and triangular faces with the size \a fn in
 * \a ph.
 *
 * zPH3DScale() directly scales \a ph by multiplying all vertices
 * by \a scale.
 *
 * zPH3DClone() makes a clone of a 3D polyhedron \a src. The
 * result is put into \a dest.
 * zPH3DMirrorClone() makes a clone of \a src mirrored about
 * the axis \a axis. The result is put into \a dest.
 *
 * zPH3DDestroy() destroys a 3D polyhedron \a ph.
 * \return
 * zPH3DInit() and zPH3DAlloc() return a pointer \a ph.
 * zPH3DDestroy() returns no value.
 */
__EXPORT zPH3D *zPH3DInit(zPH3D *ph);
__EXPORT zPH3D *zPH3DAlloc(zPH3D *ph, int vn, int fn);
__EXPORT zPH3D *zPH3DClone(zPH3D *src, zPH3D *dest);
__EXPORT zPH3D *zPH3DMirror(zPH3D *src, zPH3D *dest, zAxis axis);
__EXPORT zPH3D *zPH3DScale(zPH3D *ph, double scale);
__EXPORT void zPH3DDestroy(zPH3D *ph);

/*! \brief transform coordinates of a 3D polyhedron.
 *
 * zPH3DXform() transforms coordinates of vertices that form
 * a 3D polyhedron \a src by a frame \a f. The result is put
 * into \a dest.
 *
 * zPH3DXformInv() transforms \a src by the inversely of a
 * frame \a f. The result is put into \a dest.
 *
 * In these functions, not only vertices are transformed, but
 * the information of correspondency between vertices and faces
 * is also copied.
 * \notes
 * \a dest must have the same size of internal arrays with \a src.
 * \return
 * zPH3DXform() and zPH3DXformInv() return a pointer \a dest.
 */
__EXPORT zPH3D *zPH3DXform(zPH3D *src, zFrame3D *f, zPH3D *dest);
__EXPORT zPH3D *zPH3DXformInv(zPH3D *src, zFrame3D *f, zPH3D *dest);

/*! \brief return the contiguous vertex to another on a 3D polyhedron.
 *
 * zPH3DClosest() finds the closest point from a point \a p on a 3D
 * polyhedron \a ph and sets it into \a cp.
 * \notes
 * \a ph is treated as a surface model, namely, only the relationship
 * between \a p and the faces are examined.
 * \return
 * zPH3DClosest() returns the distance between \a p and \a cp.
 */
__EXPORT zVec3D *zPH3DContigVert(zPH3D *ph, zVec3D *p, double *d);

/*! \brief check if a point is inside of a 3D polyhedron.
 *
 * zPH3DPointIsInside() checks if a point \a p is inside of
 * a 3D polyhedron \a ph.
 * \a p on the surface of \a ph is judged to be inside of \a ph
 * if the true value is given for \a rim.
 * \return
 * zPH3DPointIsInside() returns the true value if \a p is inside
 * of \a ph, or the false value otherwise.
 * \notes
 * zPH3DPointIsInside() assumes that \a ph has a convex volume.
 */
__EXPORT double zPH3DClosest(zPH3D *ph, zVec3D *p, zVec3D *cp);
__EXPORT double zPH3DPointDist(zPH3D *ph, zVec3D *p);
__EXPORT bool zPH3DPointIsInside(zPH3D *ph, zVec3D *v, bool rim);

/*! \brief volume, barycenter and inertia of a 3D polyhedron.
 *
 * zPH3DVolume() calculates the volume of a polyhedron \a ph.
 *
 * zPH3DBarycenter() calculates the barycenter of \a ph. The
 * result is put into \a c.
 *
 * zPH3DInertia() calculates the inertial tensor of \a ph about
 * the original point. The result is put into \a inertia.
 *
 * zPH3DBaryInertia() calculates the barycenter and the inertia
 * tensor about the barycenter of \a ph simultaneously, and puts
 * them into \a c and \a i, respectively.
 * \return
 * zPH3DVolume() returns the volum calculated.
 *
 * zPH3DBarycenter() returns a pointer to \a c.
 *
 * zPH3DInertia() returns a pointer \a inertia.
 *
 * zPH3DBaryInertia() returns no value.
 */
__EXPORT double zPH3DVolume(zPH3D *ph);
__EXPORT zVec3D *zPH3DBarycenter(zPH3D *ph, zVec3D *c);
__EXPORT zMat3D *zPH3DInertia(zPH3D *ph, zMat3D *inertia);
__EXPORT void zPH3DBaryInertia(zPH3D *ph, zVec3D *c, zMat3D *i);

/*! \brief create prism and pyramid by extrusion.
 *
 * zPH3DPrism() creates a prism by extruding the bottom loop.
 * The bottom face to be extruded forms a loop of \a n vertices
 * given by \a bottom.
 * \a shift is a shifting vector of the bottom for extrusion.
 * The result is put into \a prism.
 *
 * zPH3DPyramid() creates a pyramid from the vertex \a vert and
 * The bottom face to be extruded forms a loop of \a n vertices
 * given by \a bottom.
 * The result is put into \a pyr.
 * \notes
 * A non-convex shape is acceptable for the bottom to these
 * functions, while crossing loops are not acceptable.
 * \return
 * zPH3DPrism() returns a pointer \a prism.
 * zPH3DPyramid() returns a pointer \a pyr.
 */
__EXPORT zPH3D *zPH3DPrism(zPH3D *prism, zVec3D bottom[], int n, zVec3D *shift);
__EXPORT zPH3D *zPH3DPyramid(zPH3D *pyr, zVec3D bottom[], int n, zVec3D *vert);

/*! \brief create solid revolution.
 *
 * zPH3DTorus() creates a torus which has a cross-section
 * represented by a loop of \a n vertices given by \a loop.
 * The result is put into \a torus.
 *
 * zPH3DLathe() creates a solid revolution which has a rim
 * represented by \a rim with \a n vertices. The result is
 * put into \a lathe. It is different from zPH3DTorus() at
 * a point that \a rim does not have to be a closed loop.
 *
 * For the both functions, the cross-section is revolved
 * about the unit axis vector \a axis which passes the point
 * \a center.
 * \a div is the number of division of rotation.
 * \return
 * zPH3DTorus() returns a pointer \a torus.
 * zPH3DLathe() returns a pointer \a lathe.
 * \notes
 * These functions do not check the validity of the shape
 * of cross-sections.
 */
__EXPORT zPH3D *zPH3DTorus(zPH3D *torus, zVec3D loop[], int n, int div, zVec3D *center, zVec3D *axis);
__EXPORT zPH3D *zPH3DLathe(zPH3D *lathe, zVec3D rim[], int n, int div, zVec3D *center, zVec3D *axis);

/*! \brief read a 3D polyhedron from a ZTK format processor. */
__EXPORT zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk);

__EXPORT void zPH3DFPrintZTK(FILE *fp, zPH3D *ph);

__END_DECLS

#include <zeo/zeo_ph_stl.h>
#include <zeo/zeo_ph_ply.h>

#endif /* __ZEO_PH_H__ */

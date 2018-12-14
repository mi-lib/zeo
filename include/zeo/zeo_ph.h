/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph - polyhedron
 */

#ifndef __ZEO_PH_H__
#define __ZEO_PH_H__

#include <zeo/zeo_elem.h>

__BEGIN_DECLS

zArrayClass( zVertArray, zVec3D );
zArrayClass( zFaceArray, zTri3D );

/* ********************************************************** */
/* CLASS: zPH3D
 * 3D polyhedron class
 * ********************************************************** */

typedef struct{
  zVertArray vert;
  zFaceArray face;
} zPH3D;

#define zPH3DVertNum(ph)      zArrayNum(&(ph)->vert)
#define zPH3DVertBuf(ph)      zArrayBuf(&(ph)->vert)
#define zPH3DVert(ph,i)       zArrayElemNC(&(ph)->vert,i)
#define zPH3DSetVertNum(ph,n) zArraySetNum(&(ph)->vert,n)
#define zPH3DSetVertBuf(ph,b) zArraySetBuf(&(ph)->vert,b)

#define zPH3DFaceNum(ph)      zArrayNum(&(ph)->face)
#define zPH3DFaceBuf(ph)      zArrayBuf(&(ph)->face)
#define zPH3DFace(ph,i)       zArrayElemNC(&(ph)->face,i)
#define zPH3DSetFaceNum(ph,n) zArraySetNum(&(ph)->face,n)
#define zPH3DSetFaceBuf(ph,b) zArraySetBuf(&(ph)->face,b)

#define zPH3DFaceVert(ph,i,j) zTri3DVert(zPH3DFace(ph,i),j)
#define zPH3DFaceNorm(ph,i)   zTri3DNorm(zPH3DFace(ph,i))

/* METHOD:
 * zPH3DInit, zPH3DAlloc,
 * zPH3DClone, zPH3DMirrorClone, zPH3DDestroy
 * - initialization, copy and destruction of 3D polyhedron.
 *
 * 'zPH3DInit()' initializes a 3D polyhedron 'ph',
 * setting the array of vertices and faces in 'ph' all
 * for the null pointers.
 * #
 * 'zPH3DAlloc()' newly allocates enough memory
 * for the array of vertices (size 'vn') and triangle
 * faces (size 'fn') in a 3D polyhedron 'ph'.
 * #
 * 'zPH3DClone()' clones the original 'src' to
 * 'dest', allocating vertex/face array, and computing
 * normal vectors of triangle faces.
 * 'zPH3DMirrorClone()' clones the original 'src'
 * to 'dest' with mirroring along 'axis'.
 * #
 * 'zPH3DDestroy()' destroys the array of vertices
 * and faces of 'ph'.
 * [RETURN VALUE]
 * 'zPH3DInit()' and 'zPH3DAlloc()'
 * return a pointer to 'ph'.
 * 'zPH3DDestroy()' returns no value.
 */
__EXPORT zPH3D *zPH3DInit(zPH3D *ph);
__EXPORT zPH3D *zPH3DAlloc(zPH3D *ph, int vn, int fn);
__EXPORT zPH3D *zPH3DClone(zPH3D *src, zPH3D *dest);
__EXPORT zPH3D *zPH3DMirror(zPH3D *src, zPH3D *dest, zAxis axis);
__EXPORT void zPH3DDestroy(zPH3D *ph);

/* METHOD:
 * zPH3DXfer, zPH3DXferInv
 * - transformation of 3D polyhedron.
 *
 * 'zPH3DXfer()' transforms a 3D polyhedron
 * 'src' by a transformation frame 'f' and puts it into
 * 'dest'.
 * #
 * 'zPH3DXferInv()' transforms a 3D
 * polyhedron 'src' by the inverse transformation frame
 * of 'f' and puts it into 'dest'.
 * #
 * In these functions, not only vertices are transformed,
 * but the information of correspondency between vertices
 * and faces is also copied.
 * [NOTES]
 * The polyhedron 'dest' must have the same size of internal
 * arrays with 'src'.
 * [RETURN VALUE]
 * Each of 'zPH3DXfer()' and
 * 'zPH3DXferInv()' return a pointer
 * to 'dest'.
 */
__EXPORT zPH3D *zPH3DXfer(zPH3D *src, zFrame3D *f, zPH3D *dest);
__EXPORT zPH3D *zPH3DXferInv(zPH3D *src, zFrame3D *f, zPH3D *dest);

/* METHOD:
 * zPH3DContigVert
 * - the contiguous vertex on 3D polyhedron to a point.
 *
 * 'zPH3DClosest()' calculates the closest
 * point from a point 'p' on a 3D polyhedron 'ph' and
 * sets it into 'cp'.
 * [NOTES]
 * 'ph' is treated as a surface model, namely, only the
 * relationship between 'p' and the faces are examined.
 * [RETURN VALUE]
 * 'zPH3DClosest()' returns the distance between
 * 'p' and 'cp'.
 */
__EXPORT zVec3D *zPH3DContigVert(zPH3D *ph, zVec3D *p, double *d);

/* METHOD:
 * zPH3DPointIsInside
 * - check if a point is inside of a polyhedron.
 *
 * 'zPH3DPointIsInside()' checks if a point 'p' is
 * inside of a 3D polyhedron 'ph'.
 * #
 * 'p' on the surface of 'ph' is judged to be
 * inside of 'ph' if the true value is given for
 * 'rim'.
 * [RETURN VALUE]
 * 'zPH3DPointIsInside()' returns the true value
 * if 'p' is inside of 'ph', or the false value
 * otherwise.
 * [NOTES]
 * 'zPH3DPointIsInside()' assumes that 'ph' is a convex.
 */
__EXPORT double zPH3DClosest(zPH3D *ph, zVec3D *p, zVec3D *cp);
__EXPORT double zPH3DPointDist(zPH3D *ph, zVec3D *p);
__EXPORT bool zPH3DPointIsInside(zPH3D *ph, zVec3D *v, bool rim);

/* METHOD:
 * zPH3DVolume, zPH3DBarycenter,
 * zPH3DInertia, zPH3DBaryInertia
 * - volume, barycenter and inertia of 3D polyhedron.
 *
 * 'zPH3DVolume()' calculates a volume of
 * a polyhedron 'ph'.
 * #
 * 'zPH3DBarycenter()' calculates the barycenter
 * of 'ph' and sets it into 'c'.
 * #
 * 'zPH3DInertia()' calculates the inertia tensor
 * of 'ph' about the original point and sets it
 * into 'inertia'.
 * #
 * 'zPH3DBaryInertia()' calculates the barycenter
 * and the inertia tensor about the barycenter of
 * 'ph' simultaneously, and sets them into 'c' and
 * 'i', respectively.
 * [RETURN VALUE]
 * 'zPH3DVolume()' returns the volum calculated.
 * #
 * 'zPH3DBarycenter()' returns a pointer to 'c'.
 * #
 * 'zPH3DInertia()' returns a pointer to 'inertia'.
 * #
 * 'zPH3DBaryInertia()' returns no value.
 */
__EXPORT double zPH3DVolume(zPH3D *ph);
__EXPORT zVec3D *zPH3DBarycenter(zPH3D *ph, zVec3D *c);
__EXPORT zMat3D *zPH3DInertia(zPH3D *ph, zMat3D *inertia);
__EXPORT void zPH3DBaryInertia(zPH3D *ph, zVec3D *c, zMat3D *i);

/* METHOD:
 * zPH3DPrism, zPH3DPyramid
 * - create prism and pyramid by extrusion.
 *
 * 'zPH3DPrism()' creates a prism by extrusion and
 * store it into 'prism'.
 * The bottom face to be extruded forms as a loop
 * of vertices given by 'bottom', which is an array
 * of 'n' vectors.
 * 'shift' is a shifting vector of the bottom for
 * extrusion.
 * #
 * 'zPH3DPyramid()' creates a pyramid from the vertex
 * 'vert' and the bottom face given by 'bottom' and 'n',
 * and stores it into 'pyr'.
 * [NOTES]
 * Though it is allowed to give non-convex shapes
 * as a bottom to these functions, crossing loops
 * are not acceptable.
 * [RETURN VALUE]
 * 'zPH3DPrism()' returns a pointer 'prism'.
 * #
 * 'zPH3DPyramid()' returns a pointer 'pyr'.
 */
__EXPORT zPH3D *zPH3DPrism(zPH3D *prism, zVec3D bottom[], int n, zVec3D *shift);
__EXPORT zPH3D *zPH3DPyramid(zPH3D *pyr, zVec3D bottom[], int n, zVec3D *vert);

/* METHOD:
 * zPH3DTorus, zPH3DLathe
 * - create solid revolution.
 *
 * 'zPH3DTorus()' creates a torus which has a cross-section
 * represented by 'loop' with 'n' vertices. The result is
 * put into 'torus'.
 * #
 * 'zPH3DLathe()' creates a solid revolution which has a
 * rim represented by 'rim' with 'n' vertices. The result
 * is put into 'lathe'. Being different from 'zPH3DTorus()',
 * 'rim' does not have to be a closed loop.
 * #
 * For both functions, the cross-section is revolved about
 * the unit axis vector 'axis' which passes the point
 * 'center'. 'div' is the number of division of rotation.
 * [RETURN VALUE]
 * 'zPH3DTorus()' returns a pointer 'torus'.
 * 'zPH3DLathe()' returns a pointer 'lathe'.
 * [NOTES]
 * These functions do not check the validity of the shape
 * of cross-sections. It is programmer's responsibility
 * if it allows self-crossing 'loop' and 'rim'.
 */
__EXPORT zPH3D *zPH3DTorus(zPH3D *torus, zVec3D loop[], int n, int div, zVec3D *center, zVec3D *axis);
__EXPORT zPH3D *zPH3DLathe(zPH3D *lathe, zVec3D rim[], int n, int div, zVec3D *center, zVec3D *axis);

/* METHOD:
 * zPH3DFRead, zPH3DRead, zPH3DFWrite, zPH3DWrite,
 * - input/output of 3D polyhedron.
 *
 * 'zPH3DFRead()' reads the information of a
 * polyhedron from the current position of the file 'fp',
 * and creates the new polyhedron 'ph'.
 * An acceptable data file format is as follows.
 * #
 *  vert 0: <x> <y> <z>
 *   ...
 *  vert <n>: <x> <y> <z>
 *  face <v1> <v2> <v3>
 *   ...
 * #
 * Each bracketed value must be substituted for a real
 * number. 'zPH3DRead()' reads the information
 * for 'ph' simply from the standard input.
 * #
 * 'zPH3DFWrite()' writes the information of 'ph'
 * to the current position of the file 'fp' in the same
 * format with the above. 'zPH3DWrite()' writes
 * the information of 'ph' simply to the standard out.
 * [RETURN VALUE]
 * Each of 'zPH3DFRead()' and 'zPH3DRead()'
 * returns a pointer to 'ph'.
 * #
 * Neither 'zPH3DFWrite()' nor 'zPH3DWrite()'
 * returns no values.
 */
__EXPORT zPH3D *zPH3DFRead(FILE *fp, zPH3D *ph);
#define zPH3DRead(ph)  zPH3DFRead( stdin, (ph) )
__EXPORT void zPH3DFWrite(FILE *fp, zPH3D *ph);
#define zPH3DWrite(ph) zPH3DFWrite( stdout, (ph) )

__END_DECLS

#endif /* __ZEO_PH_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_box - primitive 3D shapes: box.
 */

#ifndef __ZEO_PRIM_BOX_H__
#define __ZEO_PRIM_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zBox3D
 * 3D box class
 * ********************************************************** */

typedef struct{
  zVec3D dia;
  zFrame3D f;
} zBox3D;

#define zBox3DDia(b,d)       ( (b)->dia.e[(d)] )
#define zBox3DDepth(b)       zBox3DDia( b, zX )
#define zBox3DWidth(b)       zBox3DDia( b, zY )
#define zBox3DHeight(b)      zBox3DDia( b, zZ )
#define zBox3DCenter(b)      zFrame3DPos(&(b)->f)
#define zBox3DAxis(b,i)      ( &zFrame3DAtt(&(b)->f)->v[(i)] )

#define zBox3DSetDia(b,d,l)  ( zBox3DDia(b,d) = (l) )
#define zBox3DSetDepth(b,d)  zBox3DSetDia( b, zX, d )
#define zBox3DSetWidth(b,w)  zBox3DSetDia( b, zY, w )
#define zBox3DSetHeight(b,h) zBox3DSetDia( b, zZ, h )
#define zBox3DSetCenter(b,c) zVec3DCopy( c, zBox3DCenter(b) )
#define zBox3DSetAxis(b,i,a) zVec3DCopy( a, zBox3DAxis(b,i) )

/*! \brief initialization, creation and copy of 3D box.
 *
 * zBox3DInit() initializes a box \a box, setting its center for
 * the original point and volume for zero.
 *
 * zBox3DCreate() creates a box whose center, width, height and
 * depth are \a c, \a w, \a h and \a d, respectively.
 * \a ax, \a ay and \a az are expected to be perpendicular with
 * each other.
 *
 * zBox3DCopy() copies a box \a src to the other \a dest.
 * \return
 * Each of zBox3DInit() and zBox3DCreate() returns a pointer
 * \a box.
 * zBox3DCopy() returns a pointer the copied \a dest.
 */
__EXPORT zBox3D *zBox3DCreate(zBox3D *box, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double d, double w, double h);
#define zBox3DCreateAlign(b,c,d,w,h) \
  zBox3DCreate( b, c, ZVEC3DX, ZVEC3DY, ZVEC3DZ, d, w, h )
__EXPORT zBox3D *zBox3DInit(zBox3D *box);
__EXPORT zBox3D *zBox3DCopy(zBox3D *src, zBox3D *dest);
__EXPORT zBox3D *zBox3DMirror(zBox3D *src, zBox3D *dest, zAxis axis);

/*! \brief transfer coordinates of a 3D box.
 *
 * zBox3DXfer() transfers a box \a src by a transformation frame
 * \a f and puts it into \a dest.
 *
 * zBox3DXferInv() inversely transfers \a src by a transformation
 * frame \a f and puts it into \a dest.
 * \return
 * zBox3DXfer() and zBox3DXferInv() return a pointer \a dest.
 */
__EXPORT zBox3D *zBox3DXfer(zBox3D *src, zFrame3D *f, zBox3D *dest);
__EXPORT zBox3D *zBox3DXferInv(zBox3D *src, zFrame3D *f, zBox3D *dest);

/*! \brief distance from a 3D point to a box.
 *
 * zBox3DClosest() calculates the closest point from a 3D point \a p
 * to a box \a box, and puts it into \a cp. When \a p is inside of
 * \a box, it copies \a p to \a cp.
 *
 * zBox3DPointDist() calculates the distance from a 3D point \a p to
 * a box \a box.
 *
 * zBox3DPointIsInside() checks if a 3D point \a p is inside of a box
 * \a box. The point on the surface of \a box is judged as being
 * inside of \a box if the true value is given for \a rim.
 * \return
 * zBox3DClosest() and zBox3DPointDist() return the signed distance
 * from \a p to \a box.
 * The result is a positive value when \a p is outside of \a box, or
 * a negative value when \a p is inside of \a box.
 *
 * zBox3DPointIsInside() returns the true value if \a p is inside of
 * \a box, or the false value otherwise.
 */
__EXPORT double zBox3DClosest(zBox3D *box, zVec3D *p, zVec3D *cp);
__EXPORT double zBox3DPointDist(zBox3D *box, zVec3D *p);
__EXPORT bool zBox3DPointIsInside(zBox3D *box, zVec3D *p, bool rim);

/*! \brief volume and inertia of a box.
 *
 * zBox3DVolume() calculates the volume of a box \a box.
 *
 * zBox3DInertia() calculates the inertia tensor of a box \a box
 * around its center.
 * It treats \a box as a solid model. The result is put into \a inertia.
 * \return
 * zBox3DVolume() returns the volume calculated.
 * zBox3DInertia() returns a pointer \a inertia.
 */
__EXPORT double zBox3DVolume(zBox3D *box);
__EXPORT zMat3D *zBox3DInertia(zBox3D *box, zMat3D *inertia);

/*! \brief pick up a vertex of a box.
 *
 * zBox3DVert() gets the \a i'th vertex of a box \a box, and puts it
 * into \a v. The order is according to the figure below.
 *    2----1
 *   /    /|
 *  3----0 |  z
 *  |(6) | 5  |__y
 *  |    |/   /
 *  7----4   x
 * \return
 * zBox3DVert() returns a pointer \a v.
 */
__EXPORT zVec3D *zBox3DVert(zBox3D *box, int i, zVec3D *v);

/*! \brief convert a box to a polyhedron.
 *
 * zBox3DToPH() converts a box \a box to a polyhedron \a ph as a
 * polygon by allocating memories for eight vertices and twelve
 * polygons. The order of the vertices is according to zBox3DVert().
 *
 * \a ph should be initialized in advance.
 * \return
 * zBox3DToPH() returns a pointer \a ph.
 * \sa
 * zBox3DVert
 */
__EXPORT zPH3D *zBox3DToPH(zBox3D *box, zPH3D *ph);

/*! \brief input/output of a box.
 *
 * zBox3DFRead() reads the information of a box from the current
 * position of the file associated with \a fp, and creates a new
 * box instance \a box. An acceptable data file format is as follows.
 *
 *  center: <x> <y> <z>
 *  ax: <x> <y> <z>
 *  ay: <x> <y> <z>
 *  az: <x> <y> <z>
 *  depth: <d>
 *  width: <w>
 *  height: <h>
 *
 * Each bracketed value must be substituted for a real number.
 * zBox3DRead() reads the information for \a box from the standard
 * input.
 *
 * zBox3DFWrite() writes the information of \a box to the current
 * position of the file associated with \a fp in the same format
 * with the above. zBox3DWrite() writes the information of \a box
 * to the standard out.
 * \return
 * zBox3DFRead() and zBox3DRead() return a pointer \a box.
 *
 * Neither zBox3DFWrite() nor zBox3DWrite() returns any values.
 */
__EXPORT zBox3D *zBox3DFRead(FILE *fp, zBox3D *box);
#define zBox3DRead(b) zBox3DFRead( stdin, b )
__EXPORT void zBox3DFWrite(FILE *fp, zBox3D *box);
#define zBox3DWrite(b) zBox3DFWrite( stdout, b )

/*! \brief output a box to a file in a format to be plotted. */
__EXPORT void zBox3DDataFWrite(FILE *fp, zBox3D *box);

/* methods for abstraction */
extern zPrimCom zprim_box3d_com;

__END_DECLS

#endif /* __ZEO_PRIM_BOX_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_box - 3D shapes: box.
 */

#ifndef __ZEO_SHAPE3D_BOX_H__
#define __ZEO_SHAPE3D_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zBox3D
 * 3D box class
 * ********************************************************** */

ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zBox3D ){
  zVec3D dia;
  zFrame3D f;
};

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
__ZEO_EXPORT zBox3D *zBox3DCreate(zBox3D *box, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double d, double w, double h);
#define zBox3DCreateAlign(b,c,d,w,h) \
  zBox3DCreate( b, c, ZVEC3DX, ZVEC3DY, ZVEC3DZ, d, w, h )
__ZEO_EXPORT zBox3D *zBox3DInit(zBox3D *box);
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zBox3D );
__ZEO_EXPORT zBox3D *zBox3DCopy(const zBox3D *src, zBox3D *dest);
__ZEO_EXPORT zBox3D *zBox3DMirror(const zBox3D *src, zBox3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D box.
 *
 * zBox3DXform() transforms coordinates of a box \a src by a frame
 * \a f and puts it into \a dest.
 *
 * zBox3DXformInv() transforms \a src by the inverse of a frame
 * \a f and puts it into \a dest.
 * \return
 * zBox3DXform() and zBox3DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zBox3D *zBox3DXform(const zBox3D *src, const zFrame3D *f, zBox3D *dest);
__ZEO_EXPORT zBox3D *zBox3DXformInv(const zBox3D *src, const zFrame3D *f, zBox3D *dest);

/*! \brief distance from a 3D point to a box.
 *
 * zBox3DClosest() calculates the closest point from a 3D point \a p
 * to a box \a box, and puts it into \a cp. When \a p is inside of
 * \a box, it copies \a p to \a cp.
 *
 * zBox3DDistFromPoint() calculates the distance from a 3D point \a p to
 * a box \a box.
 *
 * zBox3DPointIsInside() checks if a 3D point \a p is inside of a box
 * \a box. \a margin is a margin of the inside area outward from the
 * boundary of \a box.
 * \return
 * zBox3DClosest() and zBox3DDistFromPoint() return the signed distance
 * from \a p to \a box.
 * The result is a positive value when \a p is outside of \a box, or
 * a negative value when \a p is inside of \a box.
 *
 * zBox3DPointIsInside() returns the true value if \a p is inside of
 * \a box, or the false value otherwise.
 */
__ZEO_EXPORT double zBox3DClosest(const zBox3D *box, const zVec3D *p, zVec3D *cp);
__ZEO_EXPORT double zBox3DDistFromPoint(const zBox3D *box, const zVec3D *p);
__ZEO_EXPORT bool zBox3DPointIsInside(const zBox3D *box, const zVec3D *p, double margin);

/*! \brief volume of a box.
 *
 * zBox3DVolume() calculates the volume of a box \a box.
 * \return
 * zBox3DVolume() returns the calculated volume.
 */
__ZEO_EXPORT double zBox3DVolume(const zBox3D *box);

/*! \brief inertia tensor of a box.
 *
 * zBox3DBaryInertia() calculates the inertia tensor of a box \a box about
 * its barycenter, supposing it is a solid. Its density has to be specified
 * by \a density.
 * zBox3DBaryInertiaMass() calculates the inertia tensor of \a box about
 * its barycenter, where its mass instead of density has to be specified by
 * \a mass.
 * For the both functions, the result is put into \a inertia.
 * \return
 * zBox3DBaryInertia() and zBox3DBaryInertiaMass() return a pointer \a inertia.
 * \sa
 * zBox3DVolume()
 */
__ZEO_EXPORT zMat3D *zBox3DBaryInertiaMass(const zBox3D *box, double mass, zMat3D *inertia);
__ZEO_EXPORT zMat3D *zBox3DBaryInertia(const zBox3D *box, double density, zMat3D *inertia);

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
__ZEO_EXPORT zVec3D *zBox3DVert(const zBox3D *box, int i, zVec3D *v);

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
__ZEO_EXPORT zPH3D *zBox3DToPH(const zBox3D *box, zPH3D *ph);

/*! \brief print a 3D box out to a file in a ZTK format. */
__ZEO_EXPORT void zBox3DFPrintZTK(FILE *fp, const zBox3D *box);

/*! \brief print a box out to a file in a plottable format. */
__ZEO_EXPORT void zBox3DValueFPrint(FILE *fp, const zBox3D *box);

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_box_com;

#define zShape3DBox(s) ( (zBox3D*)(s)->body )

__ZEO_EXPORT zShape3D *zShape3DBoxCreate(zShape3D *shape, const zVec3D *c, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double d, double w, double h);

__ZEO_EXPORT zShape3D *zShape3DBoxCreateAlign(zShape3D *shape, const zVec3D *c, double d, double w, double h);

__END_DECLS

#endif /* __ZEO_SHAPE3D_BOX_H__ */

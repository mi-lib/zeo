/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_box - 3D box.
 */

#ifndef __ZEO_ELEM3D_BOX_H__
#define __ZEO_ELEM3D_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 3D axis-aligned box class.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zAABox3D ){
  zVec3D min; /*!< minimum coordinates */
  zVec3D max; /*!< maximum coordinates */
};

#define zAABox3DXMin(box)   (box)->min.c.x
#define zAABox3DYMin(box)   (box)->min.c.y
#define zAABox3DZMin(box)   (box)->min.c.z
#define zAABox3DXMax(box)   (box)->max.c.x
#define zAABox3DYMax(box)   (box)->max.c.y
#define zAABox3DZMax(box)   (box)->max.c.z

#define zAABox3DDepth(box)  ( zAABox3DXMax(box) - zAABox3DXMin(box) )
#define zAABox3DWidth(box)  ( zAABox3DYMax(box) - zAABox3DYMin(box) )
#define zAABox3DHeight(box) ( zAABox3DZMax(box) - zAABox3DZMin(box) )

#define zAABox3DCenter(box,center) zVec3DMid( &(box)->min, &(box)->max, center )

/*! \brief initializea a 3D axis-aligned box */
__ZEO_EXPORT zAABox3D *zAABox3DInit(zAABox3D *box);

/*! \brief createa a 3D axis-aligned box */
__ZEO_EXPORT zAABox3D *zAABox3DCreate(zAABox3D *box, double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

/*! \brief copya a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DCopy(const zAABox3D *src, zAABox3D *dest);

/*! \brief merge two 3D axis-aligned boxes. */
__ZEO_EXPORT zAABox3D *zAABox3DMerge(zAABox3D *box, const zAABox3D *b1, const zAABox3D *b2);

/*! \brief the closest point from a 3D point to a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DClosest(const zAABox3D *box, const zVec3D *point, zVec3D *cp);

/*! \brief distance from a point to a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DSqrDistFromPoint(const zAABox3D *box, const zVec3D *point);
#define zAABox3DDistFromPoint(box,point) sqrt( zAABox3DSqrDistFromPoint( box, point ) )

/*! \brief check if a point is inside of a 3D axis-aligned box. */
__ZEO_EXPORT bool zAABox3DPointIsInside(const zAABox3D *box, const zVec3D *p, double margin);

/*! \brief compute volume of a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DVolume(const zAABox3D *box);

/*! \brief get a vertex of a 3D axis-aligned box. */
__ZEO_EXPORT zVec3D *zAABox3DVert(const zAABox3D *box, int i, zVec3D *v);

/*! \brief enlarge a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DEnlarge(zAABox3D *aabox, zVec3D *v);

/*! \brief print out a 3D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox3DFPrint(FILE *fp, const zAABox3D *box);
/*! \brief print out a 3D axis-aligned box to a file in a gnuplot-friendly format. */
__ZEO_EXPORT void zAABox3DValueFPrint(FILE *fp, const zAABox3D *box);

/* ********************************************************** */
/*! \brief 3D box class.
 *//* ******************************************************* */
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

/*! \brief convert a 3D axis-aligned box to a general box. */
__ZEO_EXPORT zBox3D *zAABox3DToBox3D(const zAABox3D *aab, zBox3D *box);

/*! \brief compute a 3D axis-aligned box of a 3D box. */
__ZEO_EXPORT zAABox3D *zBox3DToAABox3D(const zBox3D *box, zAABox3D *aabox);

/*! \brief print a box out to a file in a plottable format. */
__ZEO_EXPORT void zBox3DValueFPrint(FILE *fp, const zBox3D *box);

__END_DECLS

#endif /* __ZEO_ELEM3D_BOX_H__ */

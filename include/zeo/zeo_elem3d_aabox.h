/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_aabox - 3D axis-aligned box.
 */

#ifndef __ZEO_ELEM3D_AABOX_H__
#define __ZEO_ELEM3D_AABOX_H__

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

/*! \brief print out a 3D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox3DFPrint(FILE *fp, const zAABox3D *box);
/*! \brief print out a 3D axis-aligned box to a file in a gnuplot-friendly format. */
__ZEO_EXPORT void zAABox3DValueFPrint(FILE *fp, const zAABox3D *box);

__END_DECLS

#endif /* __ZEO_ELEM3D_AABOX_H__ */

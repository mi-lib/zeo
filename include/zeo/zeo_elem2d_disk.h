/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_disk - 2D disk.
 */

#ifndef __ZEO_ELEM2D_DISK_H__
#define __ZEO_ELEM2D_DISK_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D disk class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zDisk2D ){
  zVec2D center;
  double radius;
};

#define zDisk2DCenter(disk)      ( &(disk)->center )
#define zDisk2DRadius(disk)      (disk)->radius

#define zDisk2DSetCenter(disk,c) zVec2DCopy( c, zDisk2DCenter(disk) )
#define zDisk2DSetRadius(disk,r) ( zDisk2DRadius(disk) = (r) )

/*! \brief create a 2D disk. */
__ZEO_EXPORT zDisk2D *zDisk2DCreate(zDisk2D *disk, const zVec2D *c, double r);

/*! \brief signed distance from a 2D point to a disk. */
__ZEO_EXPORT double zDisk2DDistFromPoint(const zDisk2D *disk, const zVec2D *p);

/*! \brief check if a 2D point is inside of a disk. */
__ZEO_EXPORT bool zDisk2DPointIsInside(const zDisk2D *disk, const zVec2D *v, double margin);

/*! \brief create a 2D disk from two points at both ends of diameter. */
__ZEO_EXPORT zDisk2D *zDisk2DFrom2(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2);

/*! \brief create a 2D disk from three points as a circumcircle of them. */
__ZEO_EXPORT zDisk2D *zDisk2DFrom3(zDisk2D *disk, const zVec2D *v1, const zVec2D *v2, const zVec2D *v3);

/* ********************************************************** */
/*! \brief 2D ellipse class.
 *//********************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zEllips2D ){
  zVec2D center;
  double radius[2];
};

#define zEllips2DCenter(ellips)        ( &(ellips)->center )
#define zEllips2DRadius(ellips,i)      (ellips)->radius[(i)]

#define zEllips2DSetCenter(ellips,c)   zVec2DCopy( c, zEllips2DCenter(ellips) )
#define zEllips2DSetRadius(ellips,i,r) ( zEllips2DRadius(ellips,i) = (r) )

/*! \brief create an ellipse. */
__ZEO_EXPORT zEllips2D *zEllips2DCreate(zEllips2D *ellips, const zVec2D *c, double r1, double r2);

/*! \brief closest point to a 2D point on an ellipse. */
__ZEO_EXPORT zVec2D *zEllips2DClosest(const zEllips2D *ellips, const zVec2D *p, zVec2D *cp);

/*! \brief signed distance from a 2D point to an ellipse. */
__ZEO_EXPORT double zEllips2DDistFromPoint(const zEllips2D *ellips, const zVec2D *p);

/*! \brief check if a 2D point is inside of an ellipse. */
__ZEO_EXPORT bool zEllips2DPointIsInside(const zEllips2D *ellips, const zVec2D *p, double margin);

__END_DECLS

#endif /* __ZEO_ELEM2D_DISK_H__ */

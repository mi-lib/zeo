/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_box - 2D box.
 */

#ifndef __ZEO_ELEM2D_BOX_H__
#define __ZEO_ELEM2D_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D axis-aligned box class.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zAABox2D ){
  zVec2D min; /*!< minimum coordinates */
  zVec2D max; /*!< maximum coordinates */
#ifdef __cplusplus
 public:
  zAABox2D() : min{*ZVEC2DZERO}, max{*ZVEC2DZERO} {}
  double xMin();
  double yMin();
  double xMax();
  double yMax();
  double depth();
  double width();
  zVec2D center();
  zAABox2D *init();
  zAABox2D *create(double _xmin, double _ymin, double _xmax, double _ymax);
  zAABox2D *create(zVec2D *center, double _depth, double _width);
  zAABox2D *create(double _depth, double _width);
  zAABox2D *merge(const zAABox2D *box1, const zAABox2D *box2);
  zAABox2D *expand(const zAABox2D *src, double magnitude);
  zAABox2D *expand(double magnitude);
  double closest(const zVec2D *point, zVec2D *closestpoint);
  double distanceFromPoint(const zVec2D *point);
  bool pointIsInside(const zVec2D *point, double margin = zTOL);
  double area();
  zVec2D vert(int i);
  zAABox2D *enlarge(const zVec2D *v);
  void fprint(FILE *fp = stdout);
  void fprintValue(FILE *fp = stdout);
#endif /* __cplusplus */
};

#define zAABox2DXMin(box)   (box)->min.c.x
#define zAABox2DYMin(box)   (box)->min.c.y
#define zAABox2DXMax(box)   (box)->max.c.x
#define zAABox2DYMax(box)   (box)->max.c.y

#define zAABox2DDepth(box)  ( zAABox2DXMax(box) - zAABox2DXMin(box) )
#define zAABox2DWidth(box)  ( zAABox2DYMax(box) - zAABox2DYMin(box) )

#define zAABox2DCenter(box,center) zVec2DMid( &(box)->min, &(box)->max, center )

/*! \brief initializea a 2D axis-aligned box */
__ZEO_EXPORT zAABox2D *zAABox2DInit(zAABox2D *box);

/*! \brief createa a 2D axis-aligned box */
__ZEO_EXPORT zAABox2D *zAABox2DCreate(zAABox2D *box, double xmin, double ymin, double xmax, double ymax);

/*! \brief create a 2D axis-aligned box from its center, depth, and width. */
__ZEO_EXPORT zAABox2D *zAABox2DCreateFromSize(zAABox2D *box, zVec2D *center, double depth, double width);

/*! \brief check if two 2D axis-aligned boxes are equal. */
__ZEO_EXPORT bool zAABox2DEqual(const zAABox2D *box1, const zAABox2D *box2);

/*! \brief copy a 2D axis-aligned box. */
__ZEO_EXPORT zAABox2D *zAABox2DCopy(const zAABox2D *src, zAABox2D *dst);

/*! \brief merge two 2D axis-aligned boxes. */
__ZEO_EXPORT zAABox2D *zAABox2DMerge(zAABox2D *box, const zAABox2D *b1, const zAABox2D *b2);

/*! \brief expand a 2D axis-aligned box. */
__ZEO_EXPORT zAABox2D *zAABox2DExpand(const zAABox2D *src, double magnitude, zAABox2D *dest);

/*! \brief directly expand a 2D axis-aligned box. */
#define zAABox2DExpandDRC(box,magnitude) zAABox2DExpand( box, magnitude, box )

/*! \brief the closest point from a 2D point to a 2D axis-aligned box. */
__ZEO_EXPORT double zAABox2DClosest(const zAABox2D *box, const zVec2D *point, zVec2D *closestpoint);

/*! \brief distance from a point to a 2D axis-aligned box. */
__ZEO_EXPORT double zAABox2DSqrDistFromPoint(const zAABox2D *box, const zVec2D *point);
#define zAABox2DDistFromPoint(box,point) sqrt( zAABox2DSqrDistFromPoint( box, point ) )

/*! \brief check if a point is inside of a 2D axis-aligned box. */
__ZEO_EXPORT bool zAABox2DPointIsInside(const zAABox2D *box, const zVec2D *p, double margin);

/*! \brief compute area of a 2D axis-aligned box. */
__ZEO_EXPORT double zAABox2DArea(const zAABox2D *box);

/*! \brief get a vertex of a 2D axis-aligned box. */
__ZEO_EXPORT zVec2D *zAABox2DVert(const zAABox2D *box, int i, zVec2D *v);

/*! \brief enlarge a 2D axis-aligned box. */
__ZEO_EXPORT zAABox2D *zAABox2DEnlarge(zAABox2D *aabox, const zVec2D *v);

/*! \brief print out a 2D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox2DFPrint(FILE *fp, const zAABox2D *box);
/*! \brief print out a 2D axis-aligned box to a file in a gnuplot-friendly format. */
__ZEO_EXPORT void zAABox2DValueFPrint(FILE *fp, const zAABox2D *box);

__END_DECLS

#ifdef __cplusplus
inline double zAABox2D::xMin(){ return zAABox2DXMin( this ); }
inline double zAABox2D::yMin(){ return zAABox2DYMin( this ); }
inline double zAABox2D::xMax(){ return zAABox2DXMax( this ); }
inline double zAABox2D::yMax(){ return zAABox2DYMax( this ); }
inline double zAABox2D::depth(){ return zAABox2DDepth( this ); }
inline double zAABox2D::width(){ return zAABox2DWidth( this ); }
inline zVec2D zAABox2D::center(){ zVec2D c; zAABox2DCenter( this, &c ); return c; }
inline zAABox2D *zAABox2D::init(){ return zAABox2DInit( this ); }
inline zAABox2D *zAABox2D::create(double _xmin, double _ymin, double _xmax, double _ymax){ return zAABox2DCreate( this, _xmin, _ymin, _xmax, _ymax ); }
inline zAABox2D *zAABox2D::create(zVec2D *center, double _depth, double _width){ return zAABox2DCreateFromSize( this, center, _depth, _width ); }
inline zAABox2D *zAABox2D::create(double _depth, double _width){ return zAABox2DCreateFromSize( this, ZVEC2DZERO, _depth, _width ); }
inline zAABox2D *zAABox2D::merge(const zAABox2D *box1, const zAABox2D *box2){ return zAABox2DMerge( this, box1, box2 ); }
inline zAABox2D *zAABox2D::expand(const zAABox2D *src, double magnitude){ return zAABox2DExpand( src, magnitude, this ); }
inline zAABox2D *zAABox2D::expand(double magnitude){ return zAABox2DExpandDRC( this, magnitude ); }
inline double zAABox2D::closest(const zVec2D *point, zVec2D *closestpoint){ return zAABox2DClosest( this, point, closestpoint ); }
inline double zAABox2D::distanceFromPoint(const zVec2D *point){ return zAABox2DDistFromPoint( this, point ); }
inline bool zAABox2D::pointIsInside(const zVec2D *point, double margin){ return zAABox2DPointIsInside( this, point, margin ); }
inline double zAABox2D::area(){ return zAABox2DArea( this ); }
inline zVec2D zAABox2D::vert(int i){ zVec2D v; zAABox2DVert( this, i, &v ); return v; }
inline zAABox2D *zAABox2D::enlarge(const zVec2D *v){ return zAABox2DEnlarge( this, v ); }
inline void zAABox2D::fprint(FILE *fp){ return zAABox2DFPrint( fp, this ); }
inline void zAABox2D::fprintValue(FILE *fp){ return zAABox2DValueFPrint( fp, this ); }
inline bool operator==(zAABox2D &box1, zAABox2D &box2){ return zAABox2DEqual( &box1, &box2 ); }
#endif /* __cpluspls */

#endif /* __ZEO_ELEM2D_BOX_H__ */

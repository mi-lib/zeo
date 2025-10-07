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
#ifdef __cplusplus
 public:
  zAABox3D() : min{*ZVEC3DZERO}, max{*ZVEC3DZERO} {}
  double xMin();
  double yMin();
  double zMin();
  double xMax();
  double yMax();
  double zMax();
  double depth();
  double width();
  double height();
  zVec3D center();
  zAABox3D *init();
  zAABox3D *create(double _xmin, double _ymin, double _zmin, double _xmax, double _ymax, double _zmax);
  zAABox3D *create(zVec3D *center, double _depth, double _width, double _height);
  zAABox3D *create(double _depth, double _width, double _height);
  zAABox3D *merge(const zAABox3D *box1, const zAABox3D *box2);
  zAABox3D *expand(const zAABox3D *src, double magnitude);
  zAABox3D *expand(double magnitude);
  double closest(const zVec3D *point, zVec3D *closestpoint);
  double distanceFromPoint(const zVec3D *point);
  bool pointIsInside(const zVec3D *point, double margin = zTOL);
  double volume();
  zVec3D vert(int i);
  zAABox3D *enlarge(const zVec3D *v);
  void fprint(FILE *fp = stdout);
  void fprintValue(FILE *fp = stdout);
#endif /* __cplusplus */
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

/*! \brief create a 3D axis-aligned box from its center, depth, width, and height. */
__ZEO_EXPORT zAABox3D *zAABox3DCreateFromSize(zAABox3D *box, zVec3D *center, double depth, double width, double height);

/*! \brief check if two 3D axis-aligned boxes are equal. */
__ZEO_EXPORT bool zAABox3DEqual(const zAABox3D *box1, const zAABox3D *box2);

/*! \brief copya a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DCopy(const zAABox3D *src, zAABox3D *dest);

/*! \brief merge two 3D axis-aligned boxes. */
__ZEO_EXPORT zAABox3D *zAABox3DMerge(zAABox3D *box, const zAABox3D *b1, const zAABox3D *b2);

/*! \brief expand a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DExpand(const zAABox3D *src, double magnitude, zAABox3D *dest);

/*! \brief directly expand a 3D axis-aligned box. */
#define zAABox3DExpandDRC(box,magnitude) zAABox3DExpand( box, magnitude, box )

/*! \brief the closest point from a 3D point to a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DClosest(const zAABox3D *box, const zVec3D *point, zVec3D *closestpoint);

/*! \brief distance from a point to a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DSqrDistFromPoint(const zAABox3D *box, const zVec3D *point);
#define zAABox3DDistFromPoint(box,point) sqrt( zAABox3DSqrDistFromPoint( box, point ) )

/*! \brief check if a point is inside of a 3D axis-aligned box. */
__ZEO_EXPORT bool zAABox3DPointIsInside(const zAABox3D *box, const zVec3D *point, double margin);

/*! \brief compute volume of a 3D axis-aligned box. */
__ZEO_EXPORT double zAABox3DVolume(const zAABox3D *box);

/*! \brief get a vertex of a 3D axis-aligned box. */
__ZEO_EXPORT zVec3D *zAABox3DVert(const zAABox3D *box, int i, zVec3D *v);

/*! \brief enlarge a 3D axis-aligned box. */
__ZEO_EXPORT zAABox3D *zAABox3DEnlarge(zAABox3D *aabox, const zVec3D *v);

/*! \brief print out a 3D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox3DFPrint(FILE *fp, const zAABox3D *box);
/*! \brief print out a 3D axis-aligned box to a file in a gnuplot-friendly format. */
__ZEO_EXPORT void zAABox3DValueFPrint(FILE *fp, const zAABox3D *box);

__END_DECLS

#ifdef __cplusplus
inline double zAABox3D::xMin(){ return zAABox3DXMin( this ); }
inline double zAABox3D::yMin(){ return zAABox3DYMin( this ); }
inline double zAABox3D::zMin(){ return zAABox3DZMin( this ); }
inline double zAABox3D::xMax(){ return zAABox3DXMax( this ); }
inline double zAABox3D::yMax(){ return zAABox3DYMax( this ); }
inline double zAABox3D::zMax(){ return zAABox3DZMax( this ); }
inline double zAABox3D::depth(){ return zAABox3DDepth( this ); }
inline double zAABox3D::width(){ return zAABox3DWidth( this ); }
inline double zAABox3D::height(){ return zAABox3DHeight( this ); }
inline zVec3D zAABox3D::center(){ zVec3D c; zAABox3DCenter( this, &c ); return c; }
inline zAABox3D *zAABox3D::init(){ return zAABox3DInit( this ); }
inline zAABox3D *zAABox3D::create(double _xmin, double _ymin, double _zmin, double _xmax, double _ymax, double _zmax){ return zAABox3DCreate( this, _xmin, _ymin, _zmin, _xmax, _ymax, _zmax ); }
inline zAABox3D *zAABox3D::create(zVec3D *center, double _depth, double _width, double _height){ return zAABox3DCreateFromSize( this, center, _depth, _width, _height ); }
inline zAABox3D *zAABox3D::create(double _depth, double _width, double _height){ return zAABox3DCreateFromSize( this, ZVEC3DZERO, _depth, _width, _height ); }
inline zAABox3D *zAABox3D::merge(const zAABox3D *box1, const zAABox3D *box2){ return zAABox3DMerge( this, box1, box2 ); }
inline zAABox3D *zAABox3D::expand(const zAABox3D *src, double magnitude){ return zAABox3DExpand( src, magnitude, this ); }
inline zAABox3D *zAABox3D::expand(double magnitude){ return zAABox3DExpandDRC( this, magnitude ); }
inline double zAABox3D::closest(const zVec3D *point, zVec3D *closestpoint){ return zAABox3DClosest( this, point, closestpoint ); }
inline double zAABox3D::distanceFromPoint(const zVec3D *point){ return zAABox3DDistFromPoint( this, point ); }
inline bool zAABox3D::pointIsInside(const zVec3D *point, double margin){ return zAABox3DPointIsInside( this, point, margin ); }
inline double zAABox3D::volume(){ return zAABox3DVolume( this ); }
inline zVec3D zAABox3D::vert(int i){ zVec3D v; zAABox3DVert( this, i, &v ); return v; }
inline zAABox3D *zAABox3D::enlarge(const zVec3D *v){ return zAABox3DEnlarge( this, v ); }
inline void zAABox3D::fprint(FILE *fp){ return zAABox3DFPrint( fp, this ); }
inline void zAABox3D::fprintValue(FILE *fp){ return zAABox3DValueFPrint( fp, this ); }
inline bool operator==(zAABox3D &box1, zAABox3D &box2){ return zAABox3DEqual( &box1, &box2 ); }
#endif /* __cpluspls */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief list of 3D axis-aligned boxes.
 *//* ******************************************************* */
zListClass( zAABox3DList, zAABox3DListCell, zAABox3D );

/*! \brief add a 3D axis-aligned box to a list. */
__ZEO_EXPORT zAABox3DListCell *zAABox3DListAdd(zAABox3DList *list, const zAABox3D *box);

/*! \brief destroy a list of 3D axis-aligned boxes. */
#define zAABox3DListDestroy(list) zListDestroy( zAABox3DListCell, list )

/* ********************************************************** */
/*! \brief 3D box class.
 *//* ******************************************************* */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zBox3D ){
  zVec3D edgespan;
  zFrame3D frame;
#ifdef __cplusplus
 public:
  zBox3D() : edgespan{*ZVEC3DZERO}, frame{*ZFRAME3DIDENT} {}
  double span(zAxis axis);
  double depth();
  double width();
  double height();
  zVec3D *center();
  zVec3D *axis(zAxis _axis);
  zBox3D *create(const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double _depth, double _width, double _height);
  zBox3D *create(const zVec3D *center, double _depth, double _width, double _height);
  zBox3D *create(double _depth, double _width, double _height);
  zBox3D *xform(const zFrame3D *frame, zBox3D *dest);
  zBox3D *xformInv(const zFrame3D *frame, zBox3D *dest);
  double closest(const zVec3D *point, zVec3D *closestpoint);
  double distanceFromPoint(const zVec3D *point);
  bool pointIsInside(const zVec3D *point, double margin = zTOL);
  double volume();
  zMat3D *baryInertiaMass(double mass, zMat3D *inertia);
  zMat3D *baryInertia(double density, zMat3D *inertia);
  zVec3D vert(int i);
  zAABox3D toAABox3D();
  void fprintValue(FILE *fp = stdout);
#endif /* __cplusplus */
};

#define zBox3DSpan(b,d)      ( (b)->edgespan.e[(d)] )
#define zBox3DDepth(b)       zBox3DSpan( b, zX )
#define zBox3DWidth(b)       zBox3DSpan( b, zY )
#define zBox3DHeight(b)      zBox3DSpan( b, zZ )
#define zBox3DCenter(b)      zFrame3DPos(&(b)->frame)
#define zBox3DAxis(b,i)      ( &zFrame3DAtt(&(b)->frame)->v[(i)] )

#define zBox3DSetSpan(b,d,l) ( zBox3DSpan(b,d) = (l) )
#define zBox3DSetDepth(b,d)  zBox3DSetSpan( b, zX, d )
#define zBox3DSetWidth(b,w)  zBox3DSetSpan( b, zY, w )
#define zBox3DSetHeight(b,h) zBox3DSetSpan( b, zZ, h )
#define zBox3DSetCenter(b,c) zVec3DCopy( c, zBox3DCenter(b) )
#define zBox3DSetAxis(b,i,a) zVec3DCopy( a, zBox3DAxis(b,i) )

/*! \brief initialization, creation and copy of 3D box.
 *
 * zBox3DInit() initializes a box \a box, setting its center for the original point and volume for zero.
 *
 * zBox3DCreate() creates a box whose center, width, height and depth are \a c, \a w, \a h and \a d,
 * respectively.
 * \a ax, \a ay and \a az are expected to be perpendicular with each other.
 *
 * zBox3DCopy() copies a box \a src to the other \a dest.
 * \return
 * Each of zBox3DInit() and zBox3DCreate() returns a pointer \a box.
 * zBox3DCopy() returns a pointer the copied \a dest.
 */
__ZEO_EXPORT zBox3D *zBox3DCreate(zBox3D *box, const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double depth, double width, double height);
#define zBox3DCreateAlign(box,center,depth,width,height) \
  zBox3DCreate( box, center, ZVEC3DX, ZVEC3DY, ZVEC3DZ, depth, width, height )
__ZEO_EXPORT zBox3D *zBox3DInit(zBox3D *box);
__ZEO_EXPORT ZDEF_ALLOC_FUNCTION_PROTOTYPE( zBox3D );
__ZEO_EXPORT zBox3D *zBox3DCopy(const zBox3D *src, zBox3D *dest);
__ZEO_EXPORT zBox3D *zBox3DMirror(const zBox3D *src, zBox3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D box.
 *
 * zBox3DXform() transforms coordinates of a box \a src by a frame \a f and puts it into \a dest.
 *
 * zBox3DXformInv() transforms \a src by the inverse of a frame \a f and puts it into \a dest.
 * \return
 * zBox3DXform() and zBox3DXformInv() return a pointer \a dest.
 */
__ZEO_EXPORT zBox3D *zBox3DXform(const zBox3D *src, const zFrame3D *frame, zBox3D *dest);
__ZEO_EXPORT zBox3D *zBox3DXformInv(const zBox3D *src, const zFrame3D *frame, zBox3D *dest);

/*! \brief distance from a 3D point to a box.
 *
 * zBox3DClosest() calculates the closest point from a 3D point \a p to a box \a box, and puts it into
 * \a cp. When \a p is inside of \a box, it copies \a p to \a cp.
 *
 * zBox3DDistFromPoint() calculates the distance from a 3D point \a p to a box \a box.
 *
 * zBox3DPointIsInside() checks if a 3D point \a p is inside of a box \a box. \a margin is a margin of
 * the inside area outward from the boundary of \a box.
 * \return
 * zBox3DClosest() and zBox3DDistFromPoint() return the signed distance from \a p to \a box.
 * The result is a positive value when \a p is outside of \a box, or a negative value when \a p is
 * inside of \a box.
 *
 * zBox3DPointIsInside() returns the true value if \a p is inside of \a box, or the false value otherwise.
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
 * zBox3DBaryInertia() calculates the inertia tensor of a box \a box about its barycenter, supposing it
 * is a solid. Its density has to be specified by \a density.
 * zBox3DBaryInertiaMass() calculates the inertia tensor of \a box about its barycenter, where its mass
 * instead of density has to be specified by \a mass.
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
 * zBox3DVert() gets the \a i'th vertex of a box \a box, and puts it into \a v. The order is according
 * to the figure below.
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

#ifdef __cplusplus
inline double zBox3D::span(zAxis _axis){ return zBox3DSpan( this, (int)_axis ); }
inline double zBox3D::depth(){ return zBox3DDepth( this ); }
inline double zBox3D::width(){ return zBox3DWidth( this ); }
inline double zBox3D::height(){ return zBox3DHeight( this ); }
inline zVec3D *zBox3D::center(){ return zBox3DCenter( this ); }
inline zVec3D *zBox3D::axis(zAxis _axis){ return zBox3DAxis( this, (int)_axis ); }
inline zBox3D *zBox3D::create(const zVec3D *center, const zVec3D *ax, const zVec3D *ay, const zVec3D *az, double _depth, double _width, double _height){ return zBox3DCreate( this, center, ax, ay, az, _depth, _width, _height ); }
inline zBox3D *zBox3D::create(const zVec3D *center, double _depth, double _width, double _height){ return zBox3DCreateAlign( this, center, _depth, _width, _height ); }
inline zBox3D *zBox3D::create(double _depth, double _width, double _height){ return zBox3DCreateAlign( this, ZVEC3DZERO, _depth, _width, _height ); }
inline zBox3D *zBox3D::xform(const zFrame3D *frame, zBox3D *dest){ return zBox3DXform( this, frame, dest ); }
inline zBox3D *zBox3D::xformInv(const zFrame3D *frame, zBox3D *dest){ return zBox3DXformInv( this, frame, dest ); }
inline double zBox3D::closest(const zVec3D *point, zVec3D *closestpoint){ return zBox3DClosest( this, point, closestpoint ); }
inline double zBox3D::distanceFromPoint(const zVec3D *point){ return zBox3DDistFromPoint( this, point ); }
inline bool zBox3D::pointIsInside(const zVec3D *point, double margin){ return zBox3DPointIsInside( this, point, margin ); }
inline double zBox3D::volume(){ return zBox3DVolume( this ); }
inline zMat3D *zBox3D::baryInertiaMass(double mass, zMat3D *inertia){ return zBox3DBaryInertiaMass( this, mass, inertia ); }
inline zMat3D *zBox3D::baryInertia(double density, zMat3D *inertia){ return zBox3DBaryInertia( this, density, inertia ); }
inline zVec3D zBox3D::vert(int i){ zVec3D v; zBox3DVert( this, i, &v ); return v; }
inline zAABox3D zBox3D::toAABox3D(){ zAABox3D aabox; zBox3DToAABox3D( this, &aabox ); return aabox; }
inline void zBox3D::fprintValue(FILE *fp){ zBox3DValueFPrint( fp, this ); }
#endif /* __cplusplus */

#endif /* __ZEO_ELEM3D_BOX_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_ellips - primitive 3D shapes: ellipsoid.
 */

#ifndef __ZEO_PRIM_ELLIPS_H__
#define __ZEO_PRIM_ELLIPS_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zEllips3D
 * 3D ellipsoid class
 * ********************************************************** */

typedef struct{
  zVec3D radius;
  zFrame3D f;
  int div;
} zEllips3D;

#define zEllips3DRadius(ellips,d) ( (ellips)->radius.e[(d)] )
#define zEllips3DRadiusX(ellips)  zEllips3DRadius( ellips, zX )
#define zEllips3DRadiusY(ellips)  zEllips3DRadius( ellips, zY )
#define zEllips3DRadiusZ(ellips)  zEllips3DRadius( ellips, zZ )
#define zEllips3DCenter(ellips)   zFrame3DPos(&(ellips)->f)
#define zEllips3DAxis(ellips,i)   ( &zFrame3DAtt(&(ellips)->f)->v[(i)] )
#define zEllips3DDiv(ellips)      (ellips)->div

#define zEllips3DSetRadius(ellips,d,r) ( zEllips3DRadius(ellips,d) = (r) )
#define zEllips3DSetRadiusX(ellips,r)  zEllips3DSetRadius( ellips, zX, r )
#define zEllips3DSetRadiusY(ellips,r)  zEllips3DSetRadius( ellips, zY, r )
#define zEllips3DSetRadiusZ(ellips,r)  zEllips3DSetRadius( ellips, zZ, r )
#define zEllips3DSetCenter(ellips,c)   zVec3DCopy( c, zEllips3DCenter(ellips) )
#define zEllips3DSetAxis(ellips,i,a)   zVec3DCopy( a, zEllips3DAxis(ellips,i) )
#define zEllips3DSetDiv(ellips,d)      ( zEllips3DDiv(ellips) = (d) )

/*! \brief initialize, create and copy a 3D ellipsoid.
 *
 * zEllips3DInit() initializes a 3D ellipsoid \a ellips,
 * setting its center for the original point and all radi
 * for zero.
 *
 * zEllips3DCreate() creates a 3D ellipsoid whose center
 * is \a c and radius along x, y and z axes are \a rx, \a ry and \a yz,
 * respectively.
 * \a div is the number of division for polyhedral approximation.
 * When zero is given for \a div, ZEO_PRIM_DEFAULT_DIV is
 * set instead.
 *
 * zEllips3DCopy() copies a 3D ellipsoid \a src to the other \a dest.
 * \return
 * zEllips3DInit() and zEllips3DCreate() return a pointer \a ellips.
 * zEllips3DCopy() returns a pointer \a dest.
 */
__EXPORT zEllips3D *zEllips3DCreate(zEllips3D *ellips, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double rx, double ry, double rz, int div);
#define zEllips3DCreateAlign(e,c,rx,ry,rz,d) \
  zEllips3DCreate( e, c, ZVEC3DX, ZVEC3DY, ZVEC3DZ, rx, ry, rz, d )
__EXPORT zEllips3D *zEllips3DInit(zEllips3D *ellips);
__EXPORT zEllips3D *zEllips3DCopy(zEllips3D *src, zEllips3D *dest);
__EXPORT zEllips3D *zEllips3DMirror(zEllips3D *src, zEllips3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D ellipsoid.
 *
 * zEllips3DXform() transforms a 3D ellipsoid \a src by a frame \a f,
 * and puts it into \a dest.
 *
 * zEllips3DXformInv() transforms \a src by the inverse of a frame
 * \a f and puts it into \a dest.
 * \return
 * zEllips3DXform() and zEllips3DXformInv() return a pointer \a dest.
 */
__EXPORT zEllips3D *zEllips3DXform(zEllips3D *src, zFrame3D *f, zEllips3D *dest);
__EXPORT zEllips3D *zEllips3DXformInv(zEllips3D *src, zFrame3D *f, zEllips3D *dest);

/*! \brief distance from a 3D point to a 3D ellipsoid.
 *
 * zEllips3DClosest() calculates the closest point from a 3D
 * point \a p to a 3D ellipsoid \a ellips, and puts it into \a cp.
 * When \a p is inside of \a ellips, it copies \a p to \a cp.
 *
 * zEllips3DPointDist() calculates the distance from a 3D point
 * \a p to a 3D ellipsoid \a ellips.
 *
 * zEllips3DPointIsInside() checks if a 3D point \a p is inside
 * of a 3D ellipsoid \a ellips. The point on the surface of
 * \a ellips is judged to be inside of \a ellips if the true value
 * is given for \a rim.
 * \return
 * zEllips3DClosest() and zEllips3DPointDist() return the signed
 * distance from \a p to \a ellips. The result is
 *  - a positive value when \a p is outside of \a ellips, or
 *  - a negative value when \a p is inside of \a ellips.
 *
 * zEllips3DPointIsInside() returns the true value if \a p is
 * inside of \a ellips, or the false value otherwise.
 */
__EXPORT double zEllips3DClosest(zEllips3D *ellips, zVec3D *p, zVec3D *cp);
__EXPORT double zEllips3DPointDist(zEllips3D *ellips, zVec3D *p);
__EXPORT bool zEllips3DPointIsInside(zEllips3D *ellips, zVec3D *p, bool rim);

/*! \brief calculate volume and inertia of a 3D ellipsoid.
 *
 * zEllips3DVolume() calculates the volume of a 3D ellipsoid
 * \a ellips.
 *
 * zEllips3DInertia() calculates the inertia tensor of a 3D
 * ellipsoid \a ellips around its center. It treats \a ellips
 * as a solid model. The result is put into \a inertia.
 * \return
 * zEllips3DVolume() returns the calculated volume.
 * zEllips3DInertia() returns a pointer \a inertia.
 */
__EXPORT double zEllips3DVolume(zEllips3D *ellips);
__EXPORT zMat3D *zEllips3DInertia(zEllips3D *ellips, zMat3D *inertia);

/*! \brief convert an ellipsoid to a polyhedron.
 *
 * zEllips3DToPH() converts an ellipsoid \a ellips to a
 * polyhedron \a ph as a polygon model. \a ph should be
 * initialized in advance.
 * It approximately divides the face into rectangles
 * by the stored number of division.
 * \return
 * zEllips3DToPH() returns a pointer \a ph.
 * \sa
 * zCyl3DToPH(), zCyl3DToPHDRC(), zCone3DToPH(), zCone3DToPHDRC()
 */
/* default longitudinal & latitudinal division number are the same. */
__EXPORT zPH3D *zEllips3DToPH(zEllips3D *ellips, zPH3D *ph);

/*! \brief register a definition of tag-and-keys for a 3D ellipsoid to a ZTK format processor. */
__EXPORT bool zEllips3DDefRegZTK(ZTK *ztk, char *tag);
/*! \brief read a 3D ellipsoid from a ZTK format processor. */
__EXPORT zEllips3D *zEllips3DFromZTK(zEllips3D *ellips, ZTK *ztk);

/*! \brief scan and print a 3D ellipsoid.
 *
 * zEllips3DFScan() scans information of a 3D ellipsoid from
 * the current position of a file \a fp, and creates a new
 * ellipsoid \a ellips.
 * An acceptable data file format is as follows.
 *
 *  center: <x> <y> <z>
 *  ax: <x> <y> <z>
 *  ay: <x> <y> <z>
 *  az: <x> <y> <z>
 *  rx: <r>
 *  ry: <r>
 *  rz: <r>
 *  div: <div>
 *
 * The bracketed parts must be replaced by real numbers.
 * The field div is skippable.
 *
 * zEllips3DScan() scans information for \a ellips from the
 * standard input.
 *
 * zEllips3DFPrint() prints information of \a ellips out to the
 * current position of a file \a fp in the same format with the
 * above.
 * zEllips3DPrint() prints information of \a ellips out to the
 * standard output.
 * \return
 * zEllips3DFScan() and zEllips3DScan() return a pointer \a ellips.
 *
 * Neither zEllips3DFPrint() nor zEllips3DPrint() return any values.
 */
__EXPORT zEllips3D *zEllips3DFScan(FILE *fp, zEllips3D *ellips);
#define zEllips3DScan(e) zEllips3DFScan( stdin, (e) )
__EXPORT void zEllips3DFPrint(FILE *fp, zEllips3D *ellips);
#define zEllips3DPrint(e) zEllips3DFPrint( stdout, (e) )

/* methods for abstraction */
extern zPrimCom zprim_ellips3d_com;

__END_DECLS

#endif /* __ZEO_PRIM_ELLIPS_H__ */

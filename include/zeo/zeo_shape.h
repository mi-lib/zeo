/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape - 3D shape
 */

#ifndef __ZEO_SHAPE_H__
#define __ZEO_SHAPE_H__

#include <zeo/zeo_prim.h>
#include <zeo/zeo_optic.h>

__BEGIN_DECLS

/* ********************************************************** */
/* TYPE: zShapeType
 * shape type identifier
 * ********************************************************** */

typedef ubyte zShapeType;
enum{
  ZSHAPE_NONE=0,
  ZSHAPE_PH,
  ZSHAPE_BOX,
  ZSHAPE_SPHERE,
  ZSHAPE_ELLIPS,
  ZSHAPE_CYLINDER,
  ZSHAPE_ELLIPTICCYLINDER,
  ZSHAPE_CONE,
  ZSHAPE_NURBS,
};

/*! \brief expression for the type of 3D shape.
 *
 * zShapeTypeExpr() returns a string for the type of a 3D shape
 * \a type according to the followings correspondence:
 *  ZSHAPE_NONE     -> "none"
 *  ZSHAPE_PH       -> "polyhedron"
 *  ZSHAPE_BOX      -> "box"
 *  ZSHAPE_SPHERE   -> "sphere"
 *  ZSHAPE_ELLIPS   -> "ellipsoid"
 *  ZSHAPE_CYLINDER -> "cylinder"
 *  ZSHAPE_ELLIPTICCYLINDER -> "ellipticcylinder"
 *  ZSHAPE_CONE     -> "cone"
 *  ZSHAPE_NURBS    -> "nurbs"
 *
 * zShapeTypeByStr() converts a string \a str to the type of
 * 3D shape in accordance with the above correspondence.
 * \return
 * zShapeTypeExpr() returns a pointer to the string which
 * expresses \a type.
 *
 * zShapeTypeByStr() returns the type converted.
 */
__EXPORT char *zShapeTypeExpr(zShapeType type);
__EXPORT zShapeType zShapeTypeByStr(char str[]);

/* ********************************************************** */
/* CLASS: zShape3D
 * 3D unit shape class
 * ********************************************************** */

typedef union{
  zPH3D ph;
  zBox3D box;
  zSphere3D sp;
  zEllips3D el;
  zCyl3D cl;
  zECyl3D ecl;
  zCone3D cn;
  zNURBS3D nurbs;
} zShapeBody;

typedef struct{
  Z_NAMED_CLASS
  zShapeType type;
  zShapeBody body;
  zOpticalInfo *optic;
  zBox3D bb; /* bounding box */
  zPrimCom *com; /* methods */
} zShape3D;

#define zShape3DType(s)         (s)->type

#define zShape3DPH(s)           ( &(s)->body.ph )
#define zShape3DBox(s)          ( &(s)->body.box )
#define zShape3DSphere(s)       ( &(s)->body.sp )
#define zShape3DEllips(s)       ( &(s)->body.el )
#define zShape3DCyl(s)          ( &(s)->body.cl )
#define zShape3DECyl(s)         ( &(s)->body.ecl )
#define zShape3DCone(s)         ( &(s)->body.cn )
#define zShape3DNURBS(s)        ( &(s)->body.nurbs )

#define zShape3DVertNum(s)      zPH3DVertNum(zShape3DPH(s))
#define zShape3DVertBuf(s)      zPH3DVertBuf(zShape3DPH(s))
#define zShape3DVert(s,i)       zPH3DVert(zShape3DPH(s),i)
#define zShape3DFaceNum(s)      zPH3DFaceNum(zShape3DPH(s))
#define zShape3DFaceBuf(s)      zPH3DFaceBuf(zShape3DPH(s))
#define zShape3DFace(s,i)       zPH3DFace(zShape3DPH(s),i)

#define zShape3DFaceVert(s,i,j) zPH3DFaceVert(zShape3DPH(s),i,j)
#define zShape3DFaceNorm(s,i)   zPH3DFaceNorm(zShape3DPH(s),i)
#define zShape3DOptic(s)        (s)->optic
#define zShape3DSetOptic(s,o)   ( (s)->optic = (o) )
#define zShape3DBB(s)           ( &(s)->bb )

/*! \brief initialize a 3D shape instance.
 *
 * zShape3DInit() initializes a 3D shape \a shape as a
 * non-shaped instance.
 * \ret a pointer \a shape
 */
__EXPORT zShape3D *zShape3DInit(zShape3D *shape);

__EXPORT zShape3D *zShape3DCreateBox(zShape3D *shape, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double d, double w, double h);
__EXPORT zShape3D *zShape3DCreateBoxAlign(zShape3D *shape, zVec3D *c, double d, double w, double h);
__EXPORT zShape3D *zShape3DCreateSphere(zShape3D *shape, zVec3D *c, double r, int div);
__EXPORT zShape3D *zShape3DCreateEllips(zShape3D *shape, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double rx, double ry, double rz, int div);
__EXPORT zShape3D *zShape3DCreateEllipsAlign(zShape3D *shape, zVec3D *c, double rx, double ry, double rz, int div);
__EXPORT zShape3D *zShape3DCreateCyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div);
__EXPORT zShape3D *zShape3DCreateECyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div);
__EXPORT zShape3D *zShape3DCreateCone(zShape3D *shape, zVec3D *c, zVec3D *v, double r, int div);
__EXPORT zShape3D *zShape3DAllocNURBS(zShape3D *shape, int size1, int size2, int dim1, int dim2);

/*! \brief destroy a 3D shape instance.
 *
 * zShape3DDestroy() destroys \a shape, freeing its
 * inner parameters.
 */
__EXPORT void zShape3DDestroy(zShape3D *shape);

__EXPORT zShape3D *zShape3DClone(zShape3D *org, zShape3D *cln, zOpticalInfo *oi);
__EXPORT zShape3D *zShape3DMirror(zShape3D *src, zShape3D *dest, zAxis axis);

/*! \brief transform coordinates of a 3D shape.
 *
 * zShape3DXform() transforms coordinates of a 3D shape \a src
 * by a frame \a f and puts it into \a dest.
 *
 * zShape3DXformInv() transforms \a src by the inverse of a frame
 * \a f and puts it into \a dest.
 * \return
 * zShape3DXform() and zShape3DXformInv() return a pointer to \a dest.
 */
__EXPORT zShape3D *zShape3DXform(zShape3D *src, zFrame3D *f, zShape3D *dest);
__EXPORT zShape3D *zShape3DXformInv(zShape3D *src, zFrame3D *f, zShape3D *dest);

#define zShape3DContigVert(s,p,d) zPH3DContigVert( zShape3DPH(s), p, d )

/*! \brief check if a point is inside of a shape.
 *
 * zShape3DPointIsInside() checks if a point \a p is inside of
 * a shape \a shape.
 *
 * \a p on the surface of \a shape is judged to be inside of
 * \ashape if the true value is given for \a rim.
 * \return
 * zShape3DPointIsInside() returns the true value if \a p is
 * inside of \a shape, or the false value otherwise.
 * \sa
 * zPH3DPointIsInside
 */
__EXPORT double zShape3DClosest(zShape3D *shape, zVec3D *p, zVec3D *cp);
__EXPORT double zShape3DPointDist(zShape3D *shape, zVec3D *p);
__EXPORT bool zShape3DPointIsInside(zShape3D *shape, zVec3D *p, bool rim);

__EXPORT zShape3D *zShape3DToPH(zShape3D *shape);

#define ZTK_TAG_SHAPE "shape"

/*! \brief register a definition of tag-and-keys for a 3D shape to a ZTK format processor. */
__EXPORT bool zShape3DRegZTK(ZTK *ztk);
/*! \brief read a 3D shape from a ZTK format processor. */
__EXPORT zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no, ZTK *ztk);

/*! \brief scan and print a 3D shape.
 *
 * zShape3DFScan() scans information of a 3D shape from the
 * current position of a file \a fp and creates a new shape
 * \a shape. An acceptable data file format is as follows.
 *
 *  name : <name>
 *  optic : <optical info>
 *  .
 *  .
 *
 * Since all types of shapes are internally stored as polyhedra,
 * the data description is followed by that of polyhedron.
 * The bracketed parts must be replaced by real numbers.
 * <name> is the identifier of the shape. One can define
 * arbitrary name which does not involve any white spaces
 * or tab charactors.
 *
 * The shapes defined in advance might be referred by
 * others by mirror key. The candidates should be pointed
 * by \a sarray. \a ns is the size of \a sarray.
 *
 * The candidates of optical information set has to be
 * prepared in \a oarray. \a no is the size of \a oarray.
 *
 * zShape3DScan() scans information of a shape from the
 * standard input.
 *
 * zShape3DFPrint() prints information of \a shape out to
 * the current position of a file \a fp in the same format
 * with the above.
 *
 * zShape3DPrint() prints information of \a shape out to
 * the standard output.
 * \return
 * zShape3DFScan() and zShape3DScan() return a pointer \a shape.
 * zShape3DFPrint() and zShape3DPrint() return no value.
 */
__EXPORT zShape3D *zShape3DFScan(FILE *fp, zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no);
#define zShape3DScan(s,sa,ns,oa,no) \
  zShape3DFScan( stdin, (s), (sa), (ns), (oa), (no) )
__EXPORT void zShape3DFPrint(FILE *fp, zShape3D *shape);
#define zShape3DPrint(s) zShape3DFPrint( stdout, (s) )

/*! \struct zShape3DArray
 * \brief array class of 3D shapes.
 */
zArrayClass( zShape3DArray, zShape3D );

__END_DECLS

#include <zeo/zeo_shape_list.h>

#endif /* __ZEO_SHAPE_H__ */

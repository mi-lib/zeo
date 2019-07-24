/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_ecyl - 3D shapes: elliptic cylinder.
 */

#ifndef __ZEO_SHAPE_ECYL_H__
#define __ZEO_SHAPE_ECYL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zECyl3D
 * 3D elliptic cylinder class
 * ********************************************************** */

typedef struct{
  zVec3D center[2];
  double radius[2];
  zVec3D rv[2];
  int div;
} zECyl3D;

#define zECyl3DCenter(c,i)     ( &(c)->center[(i)] )
#define zECyl3DRadius(c,i)     (c)->radius[i]
#define zECyl3DRadVec(c,i)     ( &(c)->rv[i] )
#define zECyl3DDiv(c)          (c)->div

#define zECyl3DSetCenter(c,i,p) zVec3DCopy( p, zECyl3DCenter(c,i) )
#define zECyl3DSetRadius(c,i,r) ( zECyl3DRadius(c,i) = (r) )
#define zECyl3DSetRadVec(c,i,v) zVec3DCopy( v, zECyl3DRadVec(c,i) )
#define zECyl3DSetDiv(c,d)      ( zECyl3DDiv(c) = (d) )

/*! \brief create a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DCreate(zECyl3D *cyl, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div);
/*! \brief initialize a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DInit(zECyl3D *cyl);
/*! \brief allocate memory for a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DAlloc(void);
/*! \brief copy a 3D elliptic cylinder to another. */
__EXPORT zECyl3D *zECyl3DCopy(zECyl3D *src, zECyl3D *dest);
/*! \brief mirror a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DMirror(zECyl3D *src, zECyl3D *dest, zAxis axis);

/*! \brief define orthonormal axes of the bottom face of a 3D elliptic cylinder. */
__EXPORT void zECyl3DDefAxis(zECyl3D *cyl, zVec3D *ref);

/*! \brief transform coordinates of a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DXform(zECyl3D *src, zFrame3D *f, zECyl3D *dest);
/*! \brief inversely transform coordinates of a 3D elliptic cylinder. */
__EXPORT zECyl3D *zECyl3DXformInv(zECyl3D *src, zFrame3D *f, zECyl3D *dest);

/*! \brief the closest point to a 3D elliptic cylinder. */
__EXPORT double zECyl3DClosest(zECyl3D *cyl, zVec3D *p, zVec3D *cp);
/*! \brief distance from a point to a 3D elliptic cylinder. */
__EXPORT double zECyl3DPointDist(zECyl3D *cyl, zVec3D *p);
/*! \brief check if a point is inside of an elliptic cylinder. */
__EXPORT bool zECyl3DPointIsInside(zECyl3D *cyl, zVec3D *p, bool rim);

#define zECyl3DAxis(c,a) \
  zVec3DSub( zECyl3DCenter(c,1), zECyl3DCenter(c,0), a )
/*! \brief height of a 3D elliptic cylinder. */
__EXPORT double zECyl3DHeight(zECyl3D *cyl);
/*! \brief volume of a 3D elliptic cylinder. */
__EXPORT double zECyl3DVolume(zECyl3D *cyl);
/*! \brief barycenter of a 3D elliptic cylinder. */
__EXPORT zVec3D *zECyl3DBarycenter(zECyl3D *cyl, zVec3D *c);
/*! \brief inertia tensor of a 3D elliptic cylinder. */
__EXPORT zMat3D *zECyl3DInertia(zECyl3D *cyl, zMat3D *inertia);

/*! \brief convert an elliptic cylinder to a polyhedron. */
__EXPORT zPH3D *zECyl3DToPH(zECyl3D *cyl, zPH3D *ph);

/*! \brief register a definition of tag-and-keys for a 3D elliptic cylinder to a ZTK format processor. */
__EXPORT bool zECyl3DDefRegZTK(ZTK *ztk, char *tag);
/*! \brief read a 3D elliptic cylinder from a ZTK format processor. */
__EXPORT zECyl3D *zECyl3DFromZTK(zECyl3D *ecyl, ZTK *ztk);

/*! \brief scan a 3D elliptic cylinder from a file. */
__EXPORT zECyl3D *zECyl3DFScan(FILE *fp, zECyl3D *cyl);
#define zECyl3DScan(c) zECyl3DFScan( stdin, (c) )

/*! \brief print a 3D elliptic cylinder out to a file. */
__EXPORT void zECyl3DFPrint(FILE *fp, zECyl3D *cyl);
#define zECyl3DPrint(c) zECyl3DFPrint( stdout, (c) )

/*! \brief methods for abstraction */
extern zShape3DCom zeo_shape_ecyl3d_com;

#define zShape3DECyl(s) ( (zECyl3D*)(s)->body )

__EXPORT zShape3D *zShape3DCreateECyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div);

__END_DECLS

#endif /* __ZEO_SHAPE_ECYL_H__ */

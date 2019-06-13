/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_prim_ecyl - primitive 3D shapes: elliptic cylinder.
 */

#ifndef __ZEO_PRIM_ECYL_H__
#define __ZEO_PRIM_ECYL_H__

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

__EXPORT zECyl3D *zECyl3DCreate(zECyl3D *cyl, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div);
__EXPORT zECyl3D *zECyl3DInit(zECyl3D *cyl);
__EXPORT zECyl3D *zECyl3DCopy(zECyl3D *src, zECyl3D *dest);
__EXPORT zECyl3D *zECyl3DMirror(zECyl3D *src, zECyl3D *dest, zAxis axis);

__EXPORT zECyl3D *zECyl3DXfer(zECyl3D *src, zFrame3D *f, zECyl3D *dest);
__EXPORT zECyl3D *zECyl3DXferInv(zECyl3D *src, zFrame3D *f, zECyl3D *dest);

__EXPORT double zECyl3DClosest(zECyl3D *cyl, zVec3D *p, zVec3D *cp);
__EXPORT double zECyl3DPointDist(zECyl3D *cyl, zVec3D *p);
__EXPORT bool zECyl3DPointIsInside(zECyl3D *cyl, zVec3D *p, bool rim);

#define zECyl3DAxis(c,a) \
  zVec3DSub( zECyl3DCenter(c,1), zECyl3DCenter(c,0), a )
__EXPORT double zECyl3DHeight(zECyl3D *cyl);
__EXPORT double zECyl3DVolume(zECyl3D *cyl);
__EXPORT zVec3D *zECyl3DBarycenter(zECyl3D *cyl, zVec3D *c);
__EXPORT zMat3D *zECyl3DInertia(zECyl3D *cyl, zMat3D *inertia);

__EXPORT zPH3D *zECyl3DToPH(zECyl3D *cyl, zPH3D *ph);

__EXPORT zECyl3D *zECyl3DFScan(FILE *fp, zECyl3D *cyl);
#define zECyl3DScan(c) zECyl3DFScan( stdin, (c) )
__EXPORT void zECyl3DFPrint(FILE *fp, zECyl3D *cyl);
#define zECyl3DPrint(c) zECyl3DFPrint( stdout, (c) )

/* methods for abstraction */
extern zPrimCom zprim_ecyl3d_com;

__END_DECLS

#endif /* __ZEO_PRIM_ECYL_H__ */

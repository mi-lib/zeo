/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_nurbs3d - NURBS surface
 * contributer: 2018 Naoki Wakisaka
 */
#ifndef __ZEO_NURBS3D_H__
#define __ZEO_NURBS3D_H__

#include <zeo/zeo_vec3d.h>

__BEGIN_DECLS

typedef struct{
  zVec3D cp;  /*!< control point */
  double w;   /*!< weight */
} zNURBS3DCPCell;

zArrayClass( zNURBS3DCPArray, zNURBS3DCPCell );

typedef struct{
  int dim[2];    /*!< dimemsion */
  zVec knot[2];  /*!< knot vector */
  int cpsize[2]; /*!< the number of control points */
  zNURBS3DCPArray cparray; /*!< control point array */
} zNURBS3D;

typedef struct{
  int cn[2]; /*!< index of control point */
  double w;  /*!< basis value */
} zNURBS3DBasisData;

#define zNURBS3DDim(n,i)          ( (n)->dim[i] )
#define zNURBS3DOrder(n,i)        ( (n)->dim[i] + 1 )
#define zNURBS3DKnot(n,i,j)       zVecElem((n)->knot[i],j)
#define zNURBS3DKnotVec(n,i)      ( (n)->knot[i] )
#define zNURBS3DCPIndex(n,i,j)    ( (i) * (n)->cpsize[1] + (j) )
#define zNURBS3DWeight(n,i,j)     ( zArrayElem(&(n)->cparray,zNURBS3DCPIndex(n,i,j))->w )
#define zNURBS3DWeightE(n,i)      ( zArrayElem(&(n)->cparray,i)->w )
#define zNURBS3DCP(n,i,j)         ( &zArrayElem(&(n)->cparray,zNURBS3DCPIndex(n,i,j))->cp )
#define zNURBS3DCPE(n,i)          ( &zArrayElem(&(n)->cparray,i)->cp )

/* #define zNURBS3DKnotNum(n,i)      ( zVecSizeNC((n)->knot[i]) - 1 ) */
#define zNURBS3DKnotNum(n,i)      ( zVecSizeNC((n)->knot[i]) )
#define zNURBS3DCalcKnotNum(n,i)  ( zNURBS3DCPSize(n,i) + zNURBS3DOrder(n,i) )
#define zNURBS3DKnot0(n,i)        zNURBS3DKnot(n,i,0)
#define zNURBS3DKnotE(n,i)        zNURBS3DKnot(n,i,zNURBS3DKnotNum(n,i)-1)

#define zNURBS3DCPSize(n,i)       ( (n)->cpsize[i] )
#define zNURBS3DCPNum(n)          ( zArrayNum(&(n)->cparray) )
#define zNURBS3DCalcCPNum(n)      ( (n)->cpsize[0] * (n)->cpsize[1] )

__EXPORT void zNURBS3DInit(zNURBS3D *ns);
__EXPORT bool zNURBS3DCreateGridXY(zNURBS3D *ns, int dim, int xnum, int ynum, double gwidth);
__EXPORT void zNURBS3DDestroy(zNURBS3D *ns);

__EXPORT zNURBS3D *zNURBS3DCloneKnot(zNURBS3D *org, zNURBS3D *cln);
__EXPORT zNURBS3D *zNURBS3DCloneCP(zNURBS3D *org, zNURBS3D *cln);
__EXPORT zNURBS3D *zNURBS3DClone(zNURBS3D *org, zNURBS3D *cln);
__EXPORT zNURBS3D *zNURBS3DCopyNC(zNURBS3D *src, zNURBS3D *dest);
__EXPORT zNURBS3D *zNURBS3DCopy(zNURBS3D *src, zNURBS3D *dest);

__EXPORT void zNURBS3DSetKnotUniform(zVec knot);
__EXPORT void zNURBS3DSetKnotUniformFitEnds(zVec knot, int dim);

__EXPORT void zNURBS3DKnotNormalize(zNURBS3D *ns);
__EXPORT int zNURBS3DSeg(zVec knot, double t);
__EXPORT double zNURBS3DBasis(zVec knot, double t, int i, int r, int k);
__EXPORT zVec3D *zNURBS3DVec(zNURBS3D *ns, double t1, double t2, zVec3D *v);
__EXPORT zVec3D *zNURBS3DVecBD(zNURBS3D *ns, double t1, double t2, zVec3D *v, zNURBS3DBasisData *bd);

__EXPORT zVec3D *zNURBS3DDiff(zNURBS3D *ns, double t1, double t2, zVec3D *v, int diff1, int diff2);
__EXPORT zVec3D *zNURBS3DNormVec(zNURBS3D *ns, double t1, double t2, zVec3D *v);

__EXPORT double zNURBS3DVecOnXY(zNURBS3D *ns, double x, double y, zVec3D *v, double *t1, double *t2, double tol);
#define zNURBS3DVecOnXYRough(ns,x,y,v,s,t) ( zNURBS3DVecOnXY( (ns), (x), (y), (v), (s), (t), 0.0001 ) )
__EXPORT double zNURBS3DVecOnXYBD(zNURBS3D *ns, double x, double y, zVec3D *v, double *t1, double *t2, zNURBS3DBasisData *bd, double tol);
#define zNURBS3DVecOnXYBDRough(ns,x,y,v,s,t,bd) ( zNURBS3DVecOnXY( (ns), (x), (y), (v), (s), (t), (bd), 0.0001 ) )

__EXPORT zVec3D* zIntersectLineNURBS3D(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol);
/* for debug */
__EXPORT zVec3D* zIntersectLineNURBS3D_v2(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol);
__EXPORT zVec3D* zIntersectLineNURBS3D_v3(zNURBS3D *ns, zVec3D *v, zVec3D *dir, zVec3D *cv, double *t1, double *t2, double tol);

/* io */
#define ZNURBS3D_SUFFIX "zn3d"
__EXPORT bool zNURBS3DFRead(FILE *fp, zNURBS3D *ns);
__EXPORT bool zNURBS3DReadFile(zNURBS3D *ns, char filename[]);
__EXPORT void zNURBS3DFWrite(FILE *fp, zNURBS3D *ns);
__EXPORT bool zNURBS3DWriteFile(zNURBS3D *ns, char filename[]);
#define zNURBS3DWrite(ns) zNURBS3DFWrite( stdout, (ns) );

/* ************************************************************************* */
/* sequence */
/* ************************************************************************* */
typedef struct{
  double dt;
  zNURBS3D ns;
} zNURBS3DSeqCell;

zListClass( zNURBS3DSeq, zNURBS3DSeqListCell, zNURBS3DSeqCell );

#define zNURBS3DSeqListCellFree(c) do{\
  if( (c) ){\
    zNURBS3DDestroy( &(c)->data.ns );\
    free( (c) );\
    (c) = NULL;\
  }\
} while(0)

__EXPORT void zNURBS3DSeqInit(zNURBS3DSeq *nseq);
__EXPORT void zNURBS3DSeqFree(zNURBS3DSeq *nseq);

__EXPORT zNURBS3DSeqListCell *zNURBS3DSeqEnqueue(zNURBS3DSeq *nseq, zNURBS3D *ns, double dt);
__EXPORT zNURBS3DSeqListCell *zNURBS3DSeqDequeue(zNURBS3DSeq *nseq);

/* io */
#define ZNURBS3DSEQ_SUFFIX "zn3ds"
__EXPORT zNURBS3DSeq *zNURBS3DSeqFRead(FILE *fp, zNURBS3DSeq *nseq);
__EXPORT zNURBS3DSeq *zNURBS3DSeqReadFile(zNURBS3DSeq *nseq, char filename[]);
__EXPORT void zNURBS3DSeqFWrite(FILE *fp, zNURBS3DSeq *nseq);
__EXPORT void zNURBS3DSeqWriteFile(zNURBS3DSeq *nseq, char filename[]);

/* binary io */
#define ZNURBS3DSEQB_SUFFIX "zn3dsb"
#define ZNURBS3DSEQB_VERSION 1

__EXPORT void zNURBS3DSeqByteSwapKnotCP(zNURBS3DSeq *nseq);
__EXPORT zNURBS3DSeq *zNURBS3DSeqBFReadHeader(FILE *fp, zNURBS3DSeq *nseq, zHeader *header);
__EXPORT zNURBS3DSeq *zNURBS3DSeqBFRead(FILE *fp, zNURBS3DSeq *nseq);
__EXPORT zNURBS3DSeq *zNURBS3DSeqBReadFileHeader(zNURBS3DSeq *nseq, char filename[], zHeader *h);
__EXPORT zNURBS3DSeq *zNURBS3DSeqBReadFile(zNURBS3DSeq *nseq, char filename[]);

__EXPORT zHeader* zNURBS3DSeqBGetHeaderDefault(zHeader *h);
__EXPORT bool zNURBS3DSeqBCellDataFWrite(FILE *fp, zNURBS3D *ns, double dt, zNURBS3D *pns);
__EXPORT bool zNURBS3DSeqBDataFWrite(FILE *fp, zNURBS3DSeq *nseq);
__EXPORT bool zNURBS3DSeqBFWriteHeader(FILE *fp, zNURBS3DSeq *nseq, zHeader *header);
__EXPORT bool zNURBS3DSeqBFWrite(FILE *fp, zNURBS3DSeq *nseq);
__EXPORT bool zNURBS3DSeqBWriteFileHeader(zNURBS3DSeq *nseq, char filename[], zHeader *h);
__EXPORT bool zNURBS3DSeqBWriteFile(zNURBS3DSeq *nseq, char filename[]);

__END_DECLS

#endif /* __ZNURBS3D_H__ */

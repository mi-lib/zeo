/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat6d - 6x6 matrix.
 * This class was originally defined by N. Wakisaka in 2011.
 */

#include <zeo/zeo_mat6d.h>

/* create a 6x6 matrix. */
zMat6D *zMat6DCreate(zMat6D *m, zMat3D *m1, zMat3D *m2, zMat3D *m3, zMat3D *m4)
{
  _zMat6DCreate( m, m1, m2, m3, m4 );
  return m;
}

/* zero a 6x6 matrix. */
zMat6D *zMat6DZero(zMat6D *m)
{
  _zMat6DZero( m );
  return m;
}

/* transpose of a 6x6 matrix. */
zMat6D *zMat6DT(const zMat6D *m, zMat6D *tm)
{
  _zMat6DT( m, tm );
  return tm;
}

/* abstract row vector from a 6x6 matrix. */
zVec6D *zMat6DRow(const zMat6D *m, int i, zVec6D *v)
{
  int j;

  if( i < 0 || i > 5 ){
    ZRUNERROR( ZEO_ERR_INVINDEX );
    return NULL;
  }
  j = i % 3;
  i /= 3;
  _zVec3DCreate( zVec6DLin(v), m->m[i].e[0][j], m->m[i].e[1][j], m->m[i].e[2][j] );
  i += 2;
  _zVec3DCreate( zVec6DAng(v), m->m[i].e[0][j], m->m[i].e[1][j], m->m[i].e[2][j] );
  return v;
}

/* abstract column vector from a 6x6 matrix. */
zVec6D *zMat6DCol(const zMat6D *m, int i, zVec6D *v)
{
  int j;

  if( i < 0 || i > 5 ){
    ZRUNERROR( ZEO_ERR_INVINDEX );
    return NULL;
  }
  j = i % 3;
  i /= 3; i *= 2;
  zVec3DCopy( &m->m[i  ].v[j], zVec6DLin(v) );
  zVec3DCopy( &m->m[i+1].v[j], zVec6DAng(v) );
  return v;
}

/* add two 6x6 matrices. */
zMat6D *zMat6DAdd(const zMat6D *m1, const zMat6D *m2, zMat6D *m)
{
  _zMat6DAdd( m1, m2, m );
  return m;
}

/* subtract a 6x6 matrix from another. */
zMat6D *zMat6DSub(const zMat6D *m1, const zMat6D *m2, zMat6D *m)
{
  _zMat6DSub( m1, m2, m );
  return m;
}

/* multiply a 6x6 matrix by a scalar value. */
zMat6D *zMat6DMul(const zMat6D *m, double k, zMat6D *mm)
{
  _zMat6DMul( m, k, mm );
  return mm;
}

/* divide a 6x6 matrix by a scalar value. */
zMat6D *zMat6DDiv(const zMat6D *m, double k, zMat6D *dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zMat6DMul( m, k, dm );
  return dm;
}

/* directly add a 6x6 matrix to another. */
zMat6D *zMat6DAddDRC(zMat6D *m1, const zMat6D *m2)
{
  _zMat6DAddDRC( m1, m2 );
  return m1;
}

/* directly subtract a 6x6 matrix from another. */
zMat6D *zMat6DSubDRC(zMat6D *m1, const zMat6D *m2)
{
  _zMat6DSubDRC( m1, m2 );
  return m1;
}

/* directly multiply a 6x6 matrix by a scalar. */
zMat6D *zMat6DMulDRC(zMat6D *m, double k)
{
  _zMat6DMulDRC( m, k );
  return m;
}

/* directly divide a 6x6 matrix by a scalar. */
zMat6D *zMat6DDivDRC(zMat6D *m, double k)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zMat6DMulDRC( m, k );
  return m;
}

/* multiply a 6x1 vector by a 6x6 matrix from the left side. */
zVec6D *zMulMat6DVec6D(const zMat6D *m, const zVec6D *vin, zVec6D *vout)
{
  zVec3D v1, v2, v3, v4;

  zMulMat3DVec3D( &m->c.ll, zVec6DLin(vin), &v1 );
  zMulMat3DVec3D( &m->c.al, zVec6DAng(vin), &v2 );
  zMulMat3DVec3D( &m->c.la, zVec6DLin(vin), &v3 );
  zMulMat3DVec3D( &m->c.aa, zVec6DAng(vin), &v4 );
  _zVec3DAdd( &v1, &v2, zVec6DLin(vout) );
  _zVec3DAdd( &v3, &v4, zVec6DAng(vout) );
  return vout;
}

/* multiply a 6x1 vector by the transpose of a 6x6 matrix from the left side. */
zVec6D *zMulMat6DTVec6D(const zMat6D *m, const zVec6D *vin, zVec6D *vout)
{
  zVec3D v1, v2, v3, v4;

  zMulMat3DTVec3D( &m->c.ll, zVec6DLin(vin), &v1 );
  zMulMat3DTVec3D( &m->c.la, zVec6DAng(vin), &v2 );
  zMulMat3DTVec3D( &m->c.al, zVec6DLin(vin), &v3 );
  zMulMat3DTVec3D( &m->c.aa, zVec6DAng(vin), &v4 );
  _zVec3DAdd( &v1, &v2, zVec6DLin(vout) );
  _zVec3DAdd( &v3, &v4, zVec6DAng(vout) );
  return vout;
}

/* multiply a 6x6 matrix by another. */
zMat6D *zMulMat6DMat6D(const zMat6D *m1, const zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

  zMulMat3DMat3D( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMat3DMat3D( &m1->c.al, &m2->c.la, &tmp2 );
  zMulMat3DMat3D( &m1->c.la, &m2->c.ll, &tmp3 );
  zMulMat3DMat3D( &m1->c.aa, &m2->c.la, &tmp4 );
  zMulMat3DMat3D( &m1->c.ll, &m2->c.al, &tmp5 );
  zMulMat3DMat3D( &m1->c.al, &m2->c.aa, &tmp6 );
  zMulMat3DMat3D( &m1->c.la, &m2->c.al, &tmp7 );
  zMulMat3DMat3D( &m1->c.aa, &m2->c.aa, &tmp8 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  _zMat3DAdd( &tmp3, &tmp4, &m->c.la );
  _zMat3DAdd( &tmp5, &tmp6, &m->c.al );
  _zMat3DAdd( &tmp7, &tmp8, &m->c.aa );
  return m;
}

/* multiply a 6x6 matrix by the transpose of another from the left side. */
zMat6D *zMulMat6DTMat6D(const zMat6D *m1, const zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

  zMulMat3DTMat3D( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMat3DTMat3D( &m1->c.la, &m2->c.la, &tmp2 );
  zMulMat3DTMat3D( &m1->c.al, &m2->c.ll, &tmp3 );
  zMulMat3DTMat3D( &m1->c.aa, &m2->c.la, &tmp4 );
  zMulMat3DTMat3D( &m1->c.ll, &m2->c.al, &tmp5 );
  zMulMat3DTMat3D( &m1->c.la, &m2->c.aa, &tmp6 );
  zMulMat3DTMat3D( &m1->c.al, &m2->c.al, &tmp7 );
  zMulMat3DTMat3D( &m1->c.aa, &m2->c.aa, &tmp8 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  _zMat3DAdd( &tmp3, &tmp4, &m->c.la );
  _zMat3DAdd( &tmp5, &tmp6, &m->c.al );
  _zMat3DAdd( &tmp7, &tmp8, &m->c.aa );
  return m;
}

/* multiply a 6x6 matrix by the transpose of another from the right side. */
zMat6D *zMulMat6DMat6DT(const zMat6D *m1, const zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;

  zMulMat3DMat3DT( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMat3DMat3DT( &m1->c.al, &m2->c.al, &tmp2 );
  zMulMat3DMat3DT( &m1->c.la, &m2->c.ll, &tmp3 );
  zMulMat3DMat3DT( &m1->c.aa, &m2->c.al, &tmp4 );
  zMulMat3DMat3DT( &m1->c.ll, &m2->c.la, &tmp5 );
  zMulMat3DMat3DT( &m1->c.al, &m2->c.aa, &tmp6 );
  zMulMat3DMat3DT( &m1->c.la, &m2->c.la, &tmp7 );
  zMulMat3DMat3DT( &m1->c.aa, &m2->c.aa, &tmp8 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  _zMat3DAdd( &tmp3, &tmp4, &m->c.la );
  _zMat3DAdd( &tmp5, &tmp6, &m->c.al );
  _zMat3DAdd( &tmp7, &tmp8, &m->c.aa );
  return m;
}

/* dyadic product of two 6x1 vectors. */
zMat6D *zMat6DDyad(zMat6D *m, const zVec6D *v1, const zVec6D *v2)
{
  _zMat6DDyad( m, v1, v2 );
  return m;
}

/* print a 6x6 matrix out to a file. */
void zMat6DFPrint(FILE *fp, const zMat6D *m)
{
  if( !m ) return;
  fprintf( fp, "{\n" );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.ll.c.xx, m->c.ll.c.yx, m->c.ll.c.zx,
    m->c.la.c.xx, m->c.la.c.yx, m->c.la.c.zx );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.ll.c.xy, m->c.ll.c.yy, m->c.ll.c.zy,
    m->c.la.c.xy, m->c.la.c.yy, m->c.la.c.zy );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.ll.c.xz, m->c.ll.c.yz, m->c.ll.c.zz,
    m->c.la.c.xz, m->c.la.c.yz, m->c.la.c.zz );
  fprintf( fp, "------------------------------------------\n" );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.al.c.xx, m->c.al.c.yx, m->c.al.c.zx,
    m->c.aa.c.xx, m->c.aa.c.yx, m->c.aa.c.zx );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.al.c.xy, m->c.al.c.yy, m->c.al.c.zy,
    m->c.aa.c.xy, m->c.aa.c.yy, m->c.aa.c.zy );
  fprintf( fp, " %.10g, %.10g, %.10g | %.10g, %.10g, %.10g\n",
    m->c.al.c.xz, m->c.al.c.yz, m->c.al.c.zz,
    m->c.aa.c.xz, m->c.aa.c.yz, m->c.aa.c.zz );
  fprintf( fp, "}\n" );
}

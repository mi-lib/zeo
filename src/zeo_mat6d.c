/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mat6d - 6x6 matrix.
 * This class was originally defined by N. Wakisaka in 2011.
 */

#include <zeo/zeo_mat6d.h>

/* zMat6DCreate
 * - create a 6x6 matrix.
 */
zMat6D *zMat6DCreate(zMat6D *m, zMat3D *m1, zMat3D *m2, zMat3D *m3, zMat3D *m4)
{
  _zMat6DCreate( m, m1, m2, m3, m4 );
  return m;
}

/* zMat6DPutMat3D
 * - put a sub 3x3 matrix into a 6x6 matrix.
 */
zMat6D *zMat6DPutMat3D(zMat6D *m6d, int i, int j, zMat3D *m3d)
{
  zMat3DCopy( m3d, zMat6DMat3D(m6d,i,j) );
  return m6d;
}

/* zMat6DClear
 * - clear a 6x6 matrix to be zero.
 */
zMat6D *zMat6DClear(zMat6D *m)
{
  _zMat6DClear( m );
  return m;
}

/* zMat6DT
 * - transpose of a 6x6 matrix.
 */
zMat6D *zMat6DT(zMat6D *m, zMat6D *tm)
{
  _zMat6DT( m, tm );
  return tm;
}

/* zMat6DRow
 * - abstract row vector from a 6D matrix.
 */
zVec6D *zMat6DRow(zMat6D *m, int i, zVec6D *v)
{
  int j;

  j = i % 3;
  i /= 3; i *= 2;
  _zVec3DCreate( zVec6DLin(v), m->m[i].e[0][j], m->m[i].e[1][j], m->m[i].e[2][j] );
  i++;
  _zVec3DCreate( zVec6DAng(v), m->m[i].e[0][j], m->m[i].e[1][j], m->m[i].e[2][j] );
  return v;
}

/* zMat6DCol
 * - abstract column vector from a 6D matrix.
 */
zVec6D *zMat6DCol(zMat6D *m, int i, zVec6D *v)
{
  int j;

  j = i % 3;
  i /= 3;
  zVec3DCopy( &m->m[i].v[j], zVec6DLin(v) );
  i += 2;
  zVec3DCopy( &m->m[i].v[j], zVec6DAng(v) );
  return v;
}

/* zMat6DAdd
 * - add two 6x6 matrices.
 */
zMat6D *zMat6DAdd(zMat6D *m1, zMat6D *m2, zMat6D *m)
{
  _zMat6DAdd( m1, m2, m );
  return m;
}

/* zMat6DSub
 * - subtract a 6x6 matrix from another.
 */
zMat6D *zMat6DSub(zMat6D *m1, zMat6D *m2, zMat6D *m)
{
  _zMat6DSub( m1, m2, m );
  return m;
}

/* zMat6DMul
 * - multiply a 6x6 matrix by a scalar value.
 */
zMat6D *zMat6DMul(zMat6D *m, double k, zMat6D *mm)
{
  _zMat6DMul( m, k, mm );
  return mm;
}

/* zMat6DDiv
 * - divide a 6x6 matrix by a scalar value.
 */
zMat6D *zMat6DDiv(zMat6D *m, double k, zMat6D *dm)
{
  if( k == 0 ){
    ZRUNWARN( ZEO_ERR_ZERODIV );
    return NULL;
  }
  k = 1.0 / k;
  _zMat6DMul( m, k, dm );
  return dm;
}

/* zMat6DAddDRC
 * - directly add a 6x6 matrix to another.
 */
zMat6D *zMat6DAddDRC(zMat6D *m1, zMat6D *m2)
{
  _zMat6DAddDRC( m1, m2 );
  return m1;
}

/* zMat6DSubDRC
 * - directly subtract a 6x6 matrix from another.
 */
zMat6D *zMat6DSubDRC(zMat6D *m1, zMat6D *m2)
{
  _zMat6DSubDRC( m1, m2 );
  return m1;
}

/* zMat6DMulDRC
 * - directly multiply a 6x6 matrix by a scalar.
 */
zMat6D *zMat6DMulDRC(zMat6D *m, double k)
{
  _zMat6DMulDRC( m, k );
  return m;
}

/* zMat6DDivDRC
 * - directly divide a 6x6 matrix by a scalar.
 */
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

/* zMulMat6DVec6D
 * - multiply a 6x1 vector by a 6x6 matrix from the left side.
 */
zVec6D *zMulMat6DVec6D(zMat6D *m, zVec6D *vin, zVec6D *vout)
{
  zVec3D v1, v2;

  zMulMatVec3D( &m->c.ll, zVec6DLin(vin), &v1 );
  zMulMatVec3D( &m->c.la, zVec6DAng(vin), &v2 );
  _zVec3DAdd( &v1, &v2, zVec6DLin(vout) );
  zMulMatVec3D( &m->c.al, zVec6DLin(vin), &v1 );
  zMulMatVec3D( &m->c.aa, zVec6DAng(vin), &v2 );
  _zVec3DAdd( &v1, &v2, zVec6DAng(vout) );
  return vout;
}

/* zMulMat6DTVec6D
 * - multiply a 6x1 vector by the transpose of a 6x6 matrix from the left side.
 */
zVec6D *zMulMat6DTVec6D(zMat6D *m, zVec6D *vin, zVec6D *vout)
{
  zVec3D v1, v2;

  zMulMatTVec3D( &m->c.ll, zVec6DLin(vin), &v1 );
  zMulMatTVec3D( &m->c.al, zVec6DAng(vin), &v2 );
  _zVec3DAdd( &v1, &v2, zVec6DLin(vout) );
  zMulMatTVec3D( &m->c.la, zVec6DLin(vin), &v1 );
  zMulMatTVec3D( &m->c.aa, zVec6DAng(vin), &v2 );
  _zVec3DAdd( &v1, &v2, zVec6DAng(vout) );
  return vout;
}

/* zMulMatMat6D
 * - multiply a 6x6 matrix by another.
 */
zMat6D *zMulMatMat6D(zMat6D *m1, zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2;

  zMulMatMat3D( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMatMat3D( &m1->c.la, &m2->c.al, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  zMulMatMat3D( &m1->c.ll, &m2->c.la, &tmp1 );
  zMulMatMat3D( &m1->c.la, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.la );
  zMulMatMat3D( &m1->c.al, &m2->c.ll, &tmp1 );
  zMulMatMat3D( &m1->c.aa, &m2->c.al, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.al );
  zMulMatMat3D( &m1->c.al, &m2->c.la, &tmp1 );
  zMulMatMat3D( &m1->c.aa, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.aa );
  return m;
}

/* zMulMatTMat6D
 * - multiply a 6x6 matrix by the transpose of another from the left side.
 */
zMat6D *zMulMatTMat6D(zMat6D *m1, zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2;

  zMulMatTMat3D( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMatTMat3D( &m1->c.al, &m2->c.al, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  zMulMatTMat3D( &m1->c.ll, &m2->c.la, &tmp1 );
  zMulMatTMat3D( &m1->c.al, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.la );
  zMulMatTMat3D( &m1->c.la, &m2->c.ll, &tmp1 );
  zMulMatTMat3D( &m1->c.aa, &m2->c.al, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.al );
  zMulMatTMat3D( &m1->c.la, &m2->c.la, &tmp1 );
  zMulMatTMat3D( &m1->c.aa, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.aa );
  return m;
}

/* zMulMatMatT6D
 * - multiply a 6x6 matrix by the transpose of another from the right side.
 */
zMat6D *zMulMatMatT6D(zMat6D *m1, zMat6D *m2, zMat6D *m)
{
  zMat3D tmp1, tmp2;

  zMulMatMatT3D( &m1->c.ll, &m2->c.ll, &tmp1 );
  zMulMatMatT3D( &m1->c.la, &m2->c.la, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.ll );
  zMulMatMatT3D( &m1->c.ll, &m2->c.al, &tmp1 );
  zMulMatMatT3D( &m1->c.la, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.la );
  zMulMatMatT3D( &m1->c.al, &m2->c.ll, &tmp1 );
  zMulMatMatT3D( &m1->c.aa, &m2->c.la, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.al );
  zMulMatMatT3D( &m1->c.al, &m2->c.al, &tmp1 );
  zMulMatMatT3D( &m1->c.aa, &m2->c.aa, &tmp2 );
  _zMat3DAdd( &tmp1, &tmp2, &m->c.aa );
  return m;
}

/* zMat6DDyad
 * - dyadic product of two 6x1 vectors.
 */
zMat6D *zMat6DDyad(zMat6D *m, zVec6D *v1, zVec6D *v2)
{
  _zMat6DDyad( m, v1, v2 );
  return m;
}

/* zMat6DFWrite
 * - output a 6x6 matrix to the current position of a file.
 */
void zMat6DFWrite(FILE *fp, zMat6D *m)
{
  if( !m )
    fprintf( fp, "(null 6D matrix)\n" );
  else{
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
}

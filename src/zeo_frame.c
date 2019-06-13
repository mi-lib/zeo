/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame - 3D frame.
 */

#include <zeo/zeo_frame.h>

/* ********************************************************** */
/* CLASS: zFrame3D
 * 3D frame class
 * ********************************************************** */

/* the identity frame. */
const zFrame3D zframe3Dident = {
  { { 0, 0, 0 } },
  { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } }
};

/* create a 3D frame from a position vector and an attitude matrix. */
zFrame3D *zFrame3DCreate(zFrame3D *f, zVec3D *p, zMat3D *m)
{
  zFrame3DSetPos( f, p );
  zFrame3DSetAtt( f, m );
  return f;
}

/* invert a 3D frame. */
zFrame3D *zFrame3DInv(zFrame3D *f, zFrame3D *fi)
{
  zMat3DT( zFrame3DAtt(f), zFrame3DAtt(fi) );
  zMulMat3DVec3D( zFrame3DAtt(fi), zFrame3DPos(f), zFrame3DPos(fi) );
  zVec3DRevDRC( zFrame3DPos(fi) );
  return fi;
}

/* cascade a 3D frame to another. */
zFrame3D *zFrame3DCascade(zFrame3D *f1, zFrame3D *f2, zFrame3D *f)
{
  zMulMat3DVec3D( zFrame3DAtt(f1), zFrame3DPos(f2), zFrame3DPos(f) );
  zVec3DAddDRC( zFrame3DPos(f), zFrame3DPos(f1) );
  zMulMat3DMat3D( zFrame3DAtt(f1), zFrame3DAtt(f2), zFrame3DAtt(f) );
  return f;
}

/* transfer a 3D frame to that with respect to another. */
zFrame3D *zFrame3DXfer(zFrame3D *f1, zFrame3D *f2, zFrame3D *f)
{
  zFrame3D tmp;

  zFrame3DInv( f1, &tmp );
  return zFrame3DCascade( &tmp, f2, f );
}

/* transfer a position vector by a frame. */
zVec3D *zXfer3D(zFrame3D *f, zVec3D *v, zVec3D *tv)
{
  zMulMat3DVec3D( zFrame3DAtt(f), v, tv );
  return zVec3DAddDRC( tv, zFrame3DPos(f) );
}

/* inversely transfer a position vector by a frame. */
zVec3D *zXfer3DInv(zFrame3D *f, zVec3D *v, zVec3D *tv)
{
  zVec3DSub( v, zFrame3DPos(f), tv );
  return zMulMat3DTVec3DDRC( zFrame3DAtt(f), tv );
}

/* vc_lin = R^T ( v_lin + v_ang x p )
   vc_ang = R^T   v_ang
 */
zVec6D *zXfer6DLin(zFrame3D *f, zVec6D *v, zVec6D *vc)
{
  zVec6D tmp;

  zVec6DLinShift( v, zFrame3DPos(f), &tmp );
  return zMulMat3DTVec6D( zFrame3DAtt(f), &tmp, vc );
}

/* vc_lin = R       v_lin
   vc_ang = R ( p x v_lin + v_ang )
 */
zVec6D *zXfer6DAng(zFrame3D *f, zVec6D *v, zVec6D *vc)
{
  zMulMat3DVec6D( zFrame3DAtt(f), v, vc );
  return zVec6DAngShiftDRC( vc, zFrame3DPos(f) );
}

/* twist a 3D frame by a torsion vector
 * (position offset & angle-axis rotation). */
zFrame3D *zFrame3DTwist(zFrame3D *f1, zVec6D *t, zFrame3D *f2)
{
  zVec3DAdd( zFrame3DPos(f1), zVec6DLin(t), zFrame3DPos(f2) );
  zMat3DRot( zFrame3DAtt(f1), zVec6DAng(t), zFrame3DAtt(f2) );
  return f2;
}

/* error between two frames. */
zVec6D *zFrame3DError(zFrame3D *f1, zFrame3D *f2, zVec6D *err)
{
  zVec3DSub( zFrame3DPos(f1), zFrame3DPos(f2), zVec6DLin(err) );
  zMat3DError( zFrame3DAtt(f1), zFrame3DAtt(f2), zVec6DAng(err) );
  return err;
}

/* create a 3D frame from z-y-x Eulerian angles. */
zFrame3D *zFrame3DFromZYX(zFrame3D *f, double x, double y, double z, double azim, double elev, double tilt)
{
  zVec3DCreate( zFrame3DPos(f), x, y, z );
  zMat3DFromZYX( zFrame3DAtt(f), azim, elev, tilt );
  return f;
}

/* create a 3D frame from z-y-z Eulerian angle. */
zFrame3D *zFrame3DFromZYZ(zFrame3D *f, double x, double y, double z, double heading, double pitch, double bank)
{
  zVec3DCreate( zFrame3DPos(f), x, y, z );
  zMat3DFromZYZ( zFrame3DAtt(f), heading, pitch, bank );
  return f;
}

/* create a 3D frame from a position vector and an angle-axis vector. */
zFrame3D *zFrame3DFromAA(zFrame3D *f, double x, double y, double z, double xa, double ya, double za)
{
  zVec3D aa;

  _zVec3DCreate( zFrame3DPos(f), x, y, z );
  _zVec3DCreate( &aa, xa, ya, za );
  zMat3DFromAA( zFrame3DAtt(f), &aa );
  return f;
}

/* create a 3D frame from Denaviet-Hartenberg parameters. */
zFrame3D *zFrame3DFromDH(zFrame3D *f, double a, double alpha, double d, double theta)
{
  double ca, sa, ct, st;

  zSinCos( alpha, &sa, &ca );
  zSinCos( theta, &st, &ct );
  zVec3DCreate( zFrame3DPos(f), a, -d*sa, d*ca );
  zMat3DCreate( zFrame3DAtt(f),
       ct,   -st,  0,
    ca*st, ca*ct,-sa,
    sa*st, sa*ct, ca );
  return f;
}

/* convert an array of values for position and z-y-x Eulerian angles to a 3D frame. */
zFrame3D *zArrayToFrame3DZYX(double *array, zFrame3D *f)
{
  return zFrame3DFromZYX( f, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and z-y-x Eulerian angles. */
double *zFrame3DToArrayZYX(zFrame3D *f, double *array)
{
  register int i;
  zVec3D angle;

  zMat3DToZYX( zFrame3DAtt(f), &angle );
  for( i=0; i<3; i++ ){
    array[i]   = zFrame3DPos(f)->e[i];
    array[i+3] = angle.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and z-y-x Eulerian angles to a 3D frame. */
zFrame3D *zVec6DToFrame3DZYX(zVec6D *v, zFrame3D *f)
{
  return zArrayToFrame3DZYX( v->e, f );
}

/* convert a 3D frame to 6D quasi vector for position and z-y-x Eulerian angles. */
zVec6D *zFrame3DToVec6DZYX(zFrame3D *f, zVec6D *v)
{
  zFrame3DToArrayZYX( f, v->e );
  return v;
}

/* convert an array of values for position and z-y-z Eulerian angles to a 3D frame. */
zFrame3D *zArrayToFrame3DZYZ(double *array, zFrame3D *f)
{
  return zFrame3DFromZYZ( f, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and z-y-z Eulerian angles. */
double *zFrame3DToArrayZYZ(zFrame3D *f, double *array)
{
  register int i;
  zVec3D angle;

  zMat3DToZYZ( zFrame3DAtt(f), &angle );
  for( i=0; i<3; i++ ){
    array[i]   = zFrame3DPos(f)->e[i];
    array[i+3] = angle.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and z-y-z Eulerian angles to a 3D frame. */
zFrame3D *zVec6DToFrame3DZYZ(zVec6D *v, zFrame3D *f)
{
  return zArrayToFrame3DZYZ( v->e, f );
}

/* convert a 3D frame to a 6D quasi vector for position and z-y-z Eulerian angles. */
zVec6D *zFrame3DToVec6DZYZ(zFrame3D *f, zVec6D *v)
{
  zFrame3DToArrayZYZ( f, v->e );
  return v;
}

/* convert an array of values for position and angle-axis vector to a 3D frame. */
zFrame3D* zArrayToFrame3DAA(double *array, zFrame3D *f)
{
  return zFrame3DFromAA( f, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and angle-axis vector. */
double* zFrame3DToArrayAA(zFrame3D *f, double *array)
{
  zVec3D aa;
  register int i;

  zMat3DToAA( zFrame3DAtt(f), &aa );
  for( i=0; i<3; i++ ){
    array[i  ] = zFrame3DPos(f)->e[i];
    array[i+3] = aa.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and angle-axis to a 3D frame. */
zFrame3D* zVec6DToFrame3DAA(zVec6D *v, zFrame3D *f)
{
  return zArrayToFrame3DAA( v->e, f );
}

/* convert a 3D frame to a 6D quasi vector for position and angle-axis vector. */
zVec6D* zFrame3DToVec6DAA(zFrame3D *f, zVec6D *v)
{
  zFrame3DToArrayAA( f, v->e );
  return v;
}

/* scan a 3D frame from a file. */
zFrame3D *zFrame3DFScan(FILE *fp, zFrame3D *f)
{
  register int i, j;

  for( i=0; i<3; i++ ){
    for( j=0; j<3; j++ )
      zFrame3DAtt(f)->e[j][i] = zFDouble(fp);
    zFrame3DPos(f)->e[i] = zFDouble(fp);
  }
  return f;
}

/* scan D-H parameters for a frame from a file. */
zFrame3D *zFrame3DDHFScan(FILE *fp, zFrame3D *f)
{
  register int i;
  double val[4];

  for( i=0; i<4; i++ )
    val[i] = zFDouble( fp );
  val[1] = zDeg2Rad( val[1] );
  val[3] = zDeg2Rad( val[3] );
  return zFrame3DFromDH( f, val[0], val[1], val[2], val[3] );
}

/* print a 3D frame out to a file. */
void zFrame3DFPrint(FILE *fp, zFrame3D *f)
{
  register int i;

  if( !f )
    fprintf( fp, "(null 3D frame)\n" );
  else{
    fprintf( fp, "{\n" );
    for( i=0; i<3; i++ )
      fprintf( fp, " %.10g, %.10g, %.10g, %.10g\n",
        zFrame3DAtt(f)->e[0][i],
        zFrame3DAtt(f)->e[1][i],
        zFrame3DAtt(f)->e[2][i],
        zFrame3DPos(f)->e[i] );
    fprintf( fp, "}\n" );
  }
}

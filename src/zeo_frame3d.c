/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame3d - 3D coordinate frame.
 */

#include <zeo/zeo_frame3d.h>

/* ********************************************************** */
/* CLASS: zFrame3D
 * 3D coordinate frame class
 * ********************************************************** */

/* the 3D identity frame. */
#ifdef __cplusplus
const zFrame3D zFrame3D::zframe3Dident
#else
const zFrame3D zframe3Dident
#endif /* __cplusplus */
= {
  { { 0, 0, 0 } },
  { { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } } }
};

/* create a 3D coordinate frame from a position vector and an attitude matrix. */
zFrame3D *zFrame3DCreate(zFrame3D *frame, zVec3D *p, zMat3D *m)
{
  zFrame3DSetPos( frame, p );
  zFrame3DSetAtt( frame, m );
  return frame;
}

/* check if two 3D coordinate frames match. */
bool zFrame3DMatch(const zFrame3D *frame1, const zFrame3D *frame2)
{
  return _zFrame3DMatch( frame1, frame2 );
}

/* check if two 3D coordinate frames are equal. */
bool zFrame3DEqual(const zFrame3D *frame1, const zFrame3D *frame2)
{
  return _zFrame3DEqual( frame1, frame2 );
}

/* check if a 3D transformation frame is the identity frame. */
bool zFrame3DIsIdent(const zFrame3D *frame)
{
  return _zFrame3DIsIdent( frame );
}

/* transform a 3D position vector by a 3D frame. */
zVec3D *zXform3D(const zFrame3D *frame, const zVec3D *v, zVec3D *tv)
{
  _zXform3D( frame, v, tv );
  return tv;
}

/* inversely transform a 3D position vector by a 3D frame. */
zVec3D *zXform3DInv(const zFrame3D *frame, const zVec3D *v, zVec3D *tv)
{
  _zXform3DInv( frame, v, tv );
  return tv;
}

/* invert a 3D coordinate frame. */
zFrame3D *zFrame3DInv(const zFrame3D *frame, zFrame3D *invframe)
{
  _zFrame3DInv( frame, invframe );
  return invframe;
}

/* cascade a 3D coordinate frame to another. */
zFrame3D *zFrame3DCascade(const zFrame3D *frame1, const zFrame3D *frame2, zFrame3D *frame)
{
  _zFrame3DCascade( frame1, frame2, frame );
  return frame;
}

/* transform a 3D coordinate frame to that with respect to another. */
zFrame3D *zFrame3DXform(const zFrame3D *frame1, const zFrame3D *frame2, zFrame3D *frame)
{
  _zFrame3DXform( frame1, frame2, frame );
  return frame;
}

/* vc_lin = R^T ( v_lin + v_ang x p )
   vc_ang = R^T   v_ang
 */
zVec6D *zXform6DLin(const zFrame3D *frame, const zVec6D *v, zVec6D *vc)
{
  zVec6D tmp;

  zVec6DLinShift( v, zFrame3DPos(frame), &tmp );
  return zMulMat3DTVec6D( zFrame3DAtt(frame), &tmp, vc );
}

/* vc_lin = R       v_lin
   vc_ang = R ( p x v_lin + v_ang )
 */
zVec6D *zXform6DAng(const zFrame3D *frame, const zVec6D *v, zVec6D *vc)
{
  zMulMat3DVec6D( zFrame3DAtt(frame), v, vc );
  return zVec6DAngShiftDRC( vc, zFrame3DPos(frame) );
}

/* twist a 3D frame by a torsion vector (position offset & angle-axis rotation). */
zFrame3D *zFrame3DTwist(const zFrame3D *frame1, const zVec6D *twist, zFrame3D *frame2)
{
  zVec3DAdd( zFrame3DPos(frame1), zVec6DLin(twist), zFrame3DPos(frame2) );
  zMat3DRot( zFrame3DAtt(frame1), zVec6DAng(twist), zFrame3DAtt(frame2) );
  return frame2;
}

/* error between two frames. */
zVec6D *zFrame3DError(const zFrame3D *frame1, const zFrame3D *frame2, zVec6D *error)
{
  zVec3DSub( zFrame3DPos(frame1), zFrame3DPos(frame2), zVec6DLin(error) );
  zMat3DError( zFrame3DAtt(frame1), zFrame3DAtt(frame2), zVec6DAng(error) );
  return error;
}

/* create a 3D frame from z-y-x Eulerian angles. */
zFrame3D *zFrame3DFromZYX(zFrame3D *frame, double x, double y, double z, double azim, double elev, double tilt)
{
  zVec3DCreate( zFrame3DPos(frame), x, y, z );
  zMat3DFromZYX( zFrame3DAtt(frame), azim, elev, tilt );
  return frame;
}

/* create a 3D frame from z-y-z Eulerian angle. */
zFrame3D *zFrame3DFromZYZ(zFrame3D *frame, double x, double y, double z, double heading, double pitch, double bank)
{
  zVec3DCreate( zFrame3DPos(frame), x, y, z );
  zMat3DFromZYZ( zFrame3DAtt(frame), heading, pitch, bank );
  return frame;
}

/* create a 3D frame from a position vector and an angle-axis vector. */
zFrame3D *zFrame3DFromAA(zFrame3D *frame, double x, double y, double z, double xa, double ya, double za)
{
  zVec3D aa;

  _zVec3DCreate( zFrame3DPos(frame), x, y, z );
  _zVec3DCreate( &aa, xa, ya, za );
  zMat3DFromAA( zFrame3DAtt(frame), &aa );
  return frame;
}

/* create a 3D frame from Denaviet-Hartenberg parameters. */
zFrame3D *zFrame3DFromDH(zFrame3D *frame, double a, double alpha, double d, double theta)
{
  double ca, sa, ct, st;

  zSinCos( alpha, &sa, &ca );
  zSinCos( theta, &st, &ct );
  zVec3DCreate( zFrame3DPos(frame), a, -d*sa, d*ca );
  zMat3DCreate( zFrame3DAtt(frame),
       ct,   -st,  0,
    ca*st, ca*ct,-sa,
    sa*st, sa*ct, ca );
  return frame;
}

/* translate a 3D view frame with respect to the current view. */
zFrame3D *zFrame3DTranslateView(zFrame3D *frame, double x, double y, double z)
{
  zVec3D trans;

  zVec3DCreate( &trans, x, y, z );
  zMulMat3DVec3DDRC( zFrame3DAtt(frame), &trans );
  zVec3DAddDRC( zFrame3DPos(frame), &trans );
  return frame;
}

/* rotate a 3D view frame with respect to the current view. */
zFrame3D *zFrame3DRotateView(zFrame3D *frame, double angle, double x, double y, double z)
{
  zVec3D aa;
  zMat3D r;

  zVec3DCreate( &aa, x, y, z );
  if( !zIsTiny( zVec3DNormalizeDRC( &aa ) ) ){
    zVec3DMulDRC( &aa, angle );
    zMat3DFromAA( &r, &aa );
    zMulMat3DMat3D( zFrame3DAtt(frame), &r, zFrame3DAtt(frame) );
  }
  return frame;
}

/* locate a 3D view frame as to view a 3D point from a another specified point. */
zFrame3D *zFrame3DLookAtView(zFrame3D *frame, double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz)
{
  zVec3D gazevec, uppervec;

  zVec3DCreate( zFrame3DPos(frame), eyex, eyey, eyez );
  zVec3DCreate( &gazevec, eyex - centerx, eyey - centery, eyez - centerz );
  zVec3DCreate( &uppervec, upx, upy, upz );
  zVec3DOrthogonalize( &uppervec, &gazevec, &uppervec );
  zVec3DNormalize( &gazevec, zMat3DVec(zFrame3DAtt(frame),zX) );
  zVec3DNormalize( &uppervec, zMat3DVec(zFrame3DAtt(frame),zZ) );
  zVec3DOuterProd( zMat3DVec(zFrame3DAtt(frame),zZ), zMat3DVec(zFrame3DAtt(frame),zX), zMat3DVec(zFrame3DAtt(frame),zY) );
  return frame;
}

/* rotate a 3D view frame as to keep gazing a 3D point at a distance. */
zFrame3D *zFrame3DGazeAndRotateView(zFrame3D *frame, double centerx, double centery, double centerz, double distance, double pan, double tilt, double roll)
{
  zVec3D center;
  zMat3D r;

  zMat3DFromZYX( &r, pan, tilt, roll );
  zMulMat3DMat3D( zFrame3DAtt(frame), &r, zFrame3DAtt(frame) );
  zVec3DCreate( &center, centerx, centery, centerz );
  zVec3DCat( &center, distance, zMat3DVec(zFrame3DAtt(frame),zX), zFrame3DPos(frame) );
  return frame;
}

/* convert an array of values for position and z-y-x Eulerian angles to a 3D frame. */
zFrame3D *zArrayToFrame3DZYX(const double *array, zFrame3D *frame)
{
  return zFrame3DFromZYX( frame, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and z-y-x Eulerian angles. */
double *zFrame3DToArrayZYX(const zFrame3D *frame, double *array)
{
  int i;
  zVec3D angle;

  zMat3DToZYX( zFrame3DAtt(frame), &angle );
  for( i=0; i<3; i++ ){
    array[i]   = zFrame3DPos(frame)->e[i];
    array[i+3] = angle.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and z-y-x Eulerian angles to a 3D frame. */
zFrame3D *zVec6DToFrame3DZYX(const zVec6D *v, zFrame3D *frame)
{
  return zArrayToFrame3DZYX( v->e, frame );
}

/* convert a 3D frame to 6D quasi vector for position and z-y-x Eulerian angles. */
zVec6D *zFrame3DToVec6DZYX(const zFrame3D *frame, zVec6D *v)
{
  zFrame3DToArrayZYX( frame, v->e );
  return v;
}

/* convert an array of values for position and z-y-z Eulerian angles to a 3D frame. */
zFrame3D *zArrayToFrame3DZYZ(const double *array, zFrame3D *frame)
{
  return zFrame3DFromZYZ( frame, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and z-y-z Eulerian angles. */
double *zFrame3DToArrayZYZ(const zFrame3D *frame, double *array)
{
  int i;
  zVec3D angle;

  zMat3DToZYZ( zFrame3DAtt(frame), &angle );
  for( i=0; i<3; i++ ){
    array[i]   = zFrame3DPos(frame)->e[i];
    array[i+3] = angle.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and z-y-z Eulerian angles to a 3D frame. */
zFrame3D *zVec6DToFrame3DZYZ(const zVec6D *v, zFrame3D *frame)
{
  return zArrayToFrame3DZYZ( v->e, frame );
}

/* convert a 3D frame to a 6D quasi vector for position and z-y-z Eulerian angles. */
zVec6D *zFrame3DToVec6DZYZ(const zFrame3D *frame, zVec6D *v)
{
  zFrame3DToArrayZYZ( frame, v->e );
  return v;
}

/* convert an array of values for position and angle-axis vector to a 3D frame. */
zFrame3D* zArrayToFrame3DAA(const double *array, zFrame3D *frame)
{
  return zFrame3DFromAA( frame, array[0], array[1], array[2], array[3], array[4], array[5] );
}

/* convert a 3D frame to an array of values for position and angle-axis vector. */
double* zFrame3DToArrayAA(const zFrame3D *frame, double *array)
{
  zVec3D aa;
  int i;

  zMat3DToAA( zFrame3DAtt(frame), &aa );
  for( i=0; i<3; i++ ){
    array[i  ] = zFrame3DPos(frame)->e[i];
    array[i+3] = aa.e[i];
  }
  return array;
}

/* convert a 6D quasi vector for position and angle-axis to a 3D frame. */
zFrame3D* zVec6DToFrame3DAA(const zVec6D *v, zFrame3D *frame)
{
  return zArrayToFrame3DAA( v->e, frame );
}

/* convert a 3D frame to a 6D quasi vector for position and angle-axis vector. */
zVec6D* zFrame3DToVec6DAA(const zFrame3D *frame, zVec6D *v)
{
  zFrame3DToArrayAA( frame, v->e );
  return v;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 3D frame from a ZTK format processor. */
zFrame3D *zFrame3DFromZTK(zFrame3D *frame, ZTK *ztk)
{
  zFrame3DAtt(frame)->e[0][0] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[1][0] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[2][0] = ZTKDouble(ztk);
  zFrame3DPos(frame)->e[0]    = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[0][1] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[1][1] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[2][1] = ZTKDouble(ztk);
  zFrame3DPos(frame)->e[1]    = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[0][2] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[1][2] = ZTKDouble(ztk);
  zFrame3DAtt(frame)->e[2][2] = ZTKDouble(ztk);
  zFrame3DPos(frame)->e[2]    = ZTKDouble(ztk);
  return frame;
}

/* read DH parameters from a ZTK format processor. */
zFrame3D *zFrame3DDHFromZTK(zFrame3D *frame, ZTK *ztk)
{
  double a, alpha, d, theta;

  a = ZTKDouble(ztk);
  alpha = zDeg2Rad( ZTKDouble(ztk) );
  d = ZTKDouble(ztk);
  theta = zDeg2Rad( ZTKDouble(ztk) );
  return zFrame3DFromDH( frame, a, alpha, d, theta );
}

/* scan a 3D frame from a file. */
zFrame3D *zFrame3DFScan(FILE *fp, zFrame3D *frame)
{
  int i, j;

  for( i=0; i<3; i++ ){
    for( j=0; j<3; j++ )
      zFDouble( fp, &zFrame3DAtt(frame)->e[j][i] );
    zFDouble( fp, &zFrame3DPos(frame)->e[i] );
  }
  return frame;
}

/* print a 3D frame out to a file. */
void zFrame3DFPrint(FILE *fp, const zFrame3D *frame)
{
  int i;

  if( !frame ) return;
  fprintf( fp, "{\n" );
  for( i=0; i<3; i++ )
    fprintf( fp, " %.10g, %.10g, %.10g, %.10g\n",
      zFrame3DAtt(frame)->e[0][i],
      zFrame3DAtt(frame)->e[1][i],
      zFrame3DAtt(frame)->e[2][i],
      zFrame3DPos(frame)->e[i] );
  fprintf( fp, "}\n" );
}

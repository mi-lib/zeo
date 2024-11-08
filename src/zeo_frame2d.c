/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_frame2d - 2D coordinate frame.
 */

#include <zeo/zeo_frame2d.h>

/* ********************************************************** */
/* CLASS: zFrame2D
 * 2D coordinate frame class
 * ********************************************************** */

/* the 2D identity frame. */
#ifdef __cplusplus
const zFrame2D zFrame2D::zframe2Dident
#else
const zFrame2D zframe2Dident
#endif /* __cplusplus */
= {
  { { 0, 0 } },
  { { { 1, 0 }, { 0, 1 } } }
};

/* create a 2D coordinate frame from a position vector and an attitude matrix. */
zFrame2D *zFrame2DCreate(zFrame2D *frame, const zVec2D *p, const zMat2D *m)
{
  zFrame2DSetPos( frame, p );
  zFrame2DSetAtt( frame, m );
  return frame;
}

/* create a 2D coordinate frame from a position vector and a rotation angle. */
zFrame2D *zFrame2DCreateAngle(zFrame2D *frame, const zVec2D *p, double angle)
{
  zFrame2DSetPos( frame, p );
  zMat2DCreateAngle( zFrame2DAtt(frame), angle );
  return frame;
}

/* check if two 2D coordinate frames are equal. */
bool zFrame2DEqual(const zFrame2D *frame1, const zFrame2D *frame2)
{
  return _zFrame2DEqual( frame1, frame2 );
}

/* check if a 2D transformation frame is the identity frame. */
bool zFrame2DIsIdent(const zFrame2D *frame)
{
  return _zFrame2DIsIdent( frame );
}

/* transform a 2D position vector by a 2D frame. */
zVec2D *zXform2D(const zFrame2D *frame, const zVec2D *v, zVec2D *tv)
{
  _zXform2D( frame, v, tv );
  return tv;
}

/* inversely transform a 2D position vector by a 2D frame. */
zVec2D *zXform2DInv(const zFrame2D *frame, const zVec2D *v, zVec2D *tv)
{
  _zXform2DInv( frame, v, tv );
  return tv;
}

/* invert a 2D coordinate frame. */
zFrame2D *zFrame2DInv(const zFrame2D *frame, zFrame2D *invframe)
{
  _zFrame2DInv( frame, invframe );
  return invframe;
}

/* cascade a 2D coordinate frame to another. */
zFrame2D *zFrame2DCascade(const zFrame2D *frame1, const zFrame2D *frame2, zFrame2D *frame)
{
  _zFrame2DCascade( frame1, frame2, frame );
  return frame;
}

/* transform a 2D coordinate frame to that with respect to another. */
zFrame2D *zFrame2DXform(const zFrame2D *frame1, const zFrame2D *frame2, zFrame2D *frame)
{
  _zFrame2DXform( frame1, frame2, frame );
  return frame;
}

/* convert a 2D coordinate to a 2D frame. */
zFrame2D *zFrame2DFromCoord2D(zFrame2D *frame, const zCoord2D *coord)
{
  return zFrame2DCreateAngle( frame, zCoord2DPos(coord), zCoord2DAngle(coord) );
}

/* convert a 2D frame to a 2D coordinate. */
zCoord2D *zCoord2DFromFrame2D(zCoord2D *coord, const zFrame2D *frame)
{
  zCoord2DSetPos( coord, zFrame2DPos(frame) );
  zCoord2DSetAngle( coord, _zMat2DAngle(zFrame2DAtt(frame)) );
  return coord;
}

/* ********************************************************** */
/* I/O
 * ********************************************************** */

/* read a 2D frame from a ZTK format processor. */
zFrame2D *zFrame2DFromZTK(zFrame2D *frame, ZTK *ztk)
{
  zFrame2DAtt(frame)->e[0][0] = ZTKDouble(ztk);
  zFrame2DAtt(frame)->e[1][0] = ZTKDouble(ztk);
  zFrame2DPos(frame)->e[0]    = ZTKDouble(ztk);
  zFrame2DAtt(frame)->e[0][1] = ZTKDouble(ztk);
  zFrame2DAtt(frame)->e[1][1] = ZTKDouble(ztk);
  zFrame2DPos(frame)->e[1]    = ZTKDouble(ztk);
  return frame;
}

/* scan a 2D frame from a file. */
zFrame2D *zFrame2DFScan(FILE *fp, zFrame2D *frame)
{
  int i, j;

  for( i=0; i<2; i++ ){
    for( j=0; j<2; j++ )
      zFDouble( fp, &zFrame2DAtt(frame)->e[j][i] );
    zFDouble( fp, &zFrame2DPos(frame)->e[i] );
  }
  return frame;
}

/* print a 2D frame out to a file. */
void zFrame2DFPrint(FILE *fp, const zFrame2D *frame)
{
  int i;

  if( !frame ) return;
  fprintf( fp, "{\n" );
  for( i=0; i<2; i++ )
    fprintf( fp, " %.10g, %.10g, %.10g\n",
      zFrame2DAtt(frame)->e[0][i],
      zFrame2DAtt(frame)->e[1][i],
      zFrame2DPos(frame)->e[i] );
  fprintf( fp, "}\n" );
}

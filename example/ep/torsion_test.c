#include <zeo/zeo_ep.h>

zVec3D *torsion1(zMat3D *r1, zMat3D *r2, zDir d, zVec3D *t)
{
  double angle;

  zVec3DOuterProd( zMat3DVec(r1,d), zMat3DVec(r2,d), t );
  return zIsTiny( ( angle = zVec3DAngle( zMat3DVec(r1,d), zMat3DVec(r2,d), t ) ) ) ?
    zVec3DClear(t) : zVec3DMulDRC( t, angle/zVec3DNorm(t) );
}

zVec3D *torsion2(zMat3D *r1, zMat3D *r2, zVec3D *t)
{
  zVec3D tx, ty;

  torsion1( r1, r2, zX, &tx );
  torsion1( r1, r2, zY, &ty );
  return zVec3DMid( &tx, &ty, t );
}

int main(int argc, char *argv[])
{
  zMat3D r1, r2, rm, r;
  zVec3D t1, t2, t, err;

  zRandInit();
  zVec3DCreate( &t, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( &t );
  zVec3DMulDRC( &t, zRandF(0,zPI) );

  zMat3DZYX( &r1, zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DRot( &r1, &t, &r2 );

  /* torsion 1 */
  torsion1( &r1, &r2, zZ, &t1 );

  /* medival attitude */
  zMat3DRot( &r1, &t1, &rm );

  /* torsion 2 */
  torsion2( &rm, &r2, &t2 );

  /* terminal attitude */
  zMat3DRot( &rm, &t2, &r );

  zMat3DError( &r, &r1, &err );
  zVec3DWrite( &err );
  zVec3DWrite( &t );
  zVec3DWrite( zVec3DSubDRC(&err,&t) );
  return 0;
}

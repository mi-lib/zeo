#include <zeo/zeo_mat3d.h>

#define V    0.001
#define STEP 100

int main(void)
{
  zVec3D zyxvel, angvel, zyx, err;
  zMat3D m1, m2, tmp;
  register int i;

  zRandInit();
  zVec3DClear( &zyx );
  zMat3DIdent( &m2 );
  zVec3DCreate( &zyxvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  for( i=0; i<=STEP; i++ ){
    /* z-y-x Eulerian angle velocity -> angular velocity */
    zVec3DZYXVel2AngVel( &zyxvel, &zyx, &angvel );
    /* z-y-x Eulerian angle -> matrix */
    zVec3DAddDRC( &zyx, &zyxvel );
    zMat3DZYX( &m1, zyx.e[zX], zyx.e[zY], zyx.e[zZ] );
    /* angular velocity vector -> matrix */
    zMat3DCopy( &m2, &tmp );
    zMat3DRot( &tmp, &angvel, &m2 );
    /* error */
    zMat3DError( &m1, &m2, &err );
    zVec3DDataNLWrite( &err );
  }
  return 0;
}

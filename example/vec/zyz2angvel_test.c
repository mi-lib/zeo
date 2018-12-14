#include <zeo/zeo_mat3d.h>

#define V    0.001
#define STEP 100

int main(void)
{
  zVec3D zyzvel, angvel, zyz, err;
  zMat3D m1, m2, tmp;
  register int i;

  zRandInit();
  zVec3DClear( &zyz );
  zMat3DIdent( &m2 );
  zVec3DCreate( &zyzvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  for( i=0; i<=STEP; i++ ){
    /* z-y-x Eulerian angle velocity -> angular velocity */
    zVec3DZYZVel2AngVel( &zyzvel, &zyz, &angvel );
    /* z-y-x Eulerian angle -> matrix */
    zVec3DAddDRC( &zyz, &zyzvel );
    zMat3DZYZ( &m1, zyz.e[zX], zyz.e[zY], zyz.e[zZ] );
    /* angular velocity vector -> matrix */
    zMat3DCopy( &m2, &tmp );
    zMat3DRot( &tmp, &angvel, &m2 );
    /* error */
    zMat3DError( &m1, &m2, &err );
    zVec3DDataNLWrite( &err );
  }
  return 0;
}

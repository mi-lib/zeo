#include <zeo/zeo.h>

#define N 100

int main(void)
{
  register int i;
  zEP ep, epvel;
  zVec3D av1, av2, err;
  zMat3D m;

  zRandInit();
  zMat3DZYX( &m, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zMat3DToEP( &m, &ep );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &av1, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
    zAngVel2EPVel( &av1, &ep, &epvel );
    zEPVel2AngVel( &epvel, &ep, &av2 );
    zVec3DSub( &av1, &av2, &err );
    printf( "%.10f %.10f %.10f %s\n", err.e[zX], err.e[zY], err.e[zZ],
      zBoolExpr( zVec3DIsTiny( &err ) ) );
  }
  return 0;
}

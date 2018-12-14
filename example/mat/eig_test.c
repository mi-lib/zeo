#include <zeo/zeo_mat3d.h>

void verify(int i, zMat3D *m, zVec3D *v, double val)
{
  zVec3D tmp1, tmp2, d;

  zMulMatVec3D( m, v, &tmp1 );
  zVec3DMul( v, val, &tmp2 );
  zVec3DSub( &tmp1, &tmp2, &d );
  printf( "test %d - %s: ", i, zBoolExpr( zVec3DIsTiny( &d ) ) );
  zVec3DWrite( &d );
}

int main(void)
{
  zMat3D m;
  zVec3D evec[3];
  double eval[3], v[6];
  register int i;

  zRandInit();
  for( i=0; i<6; i++ )
    v[i] = zRandF(-10,10);
  zMat3DCreate( &m,
    v[0], v[1], v[2],
    v[1], v[3], v[4],
    v[2], v[4], v[5] );
  zMat3DSymEig( &m, eval, evec );
  for( i=0; i<3; i++ ){
    printf( "eig.%d %g - ", i, eval[i] );
    zVec3DWrite( &evec[i] );
  }

  printf( ">> confirmation\n" );
  for( i=0; i<3; i++ )
    verify( i, &m, &evec[i], eval[i] );
  return 0;
}

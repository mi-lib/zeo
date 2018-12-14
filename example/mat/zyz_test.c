#include <zeo/zeo_mat3d.h>

int main(void)
{
  zVec3D v;
  zMat3D m;
  double heading, pitch, bank;

  do{
    printf( "heading  = " ); if( scanf( "%lf", &heading ) == 0 ) return 1;
    printf( "pitch = " );    if( scanf( "%lf", &pitch   ) == 0 ) return 1;
    printf( "bank   = " );   if( scanf( "%lf", &bank    ) == 0 ) return 1;
    zMat3DWrite( zMat3DZYZ( &m, zDeg2Rad(heading), zDeg2Rad(pitch), zDeg2Rad(bank) ) );
    zMat3DToZYZ( &m, &v );
    heading = zRad2Deg( v.e[zX] );
    pitch   = zRad2Deg( v.e[zY] );
    bank    = zRad2Deg( v.e[zZ] );
    printf( "%f %f %f\n", heading, pitch, bank );
  } while( heading!=0 || pitch!=0 || bank!=0 );
  return 0;
}

#include <zeo/zeo_mat3d.h>

int main(void)
{
  zVec3D v;
  zMat3D m;
  double azim, elev, tilt;

  do{
    printf( "azimuth   = " ); if( scanf( "%lf", &azim ) == 0 ) return 1;
    printf( "elevation = " ); if( scanf( "%lf", &elev ) == 0 ) return 1;
    printf( "tilt      = " ); if( scanf( "%lf", &tilt ) == 0 ) return 1;
    zMat3DWrite( zMat3DZYX( &m, zDeg2Rad(azim), zDeg2Rad(elev), zDeg2Rad(tilt) ) );
    zMat3DToZYX( &m, &v );
    azim = zRad2Deg( v.e[zX] );
    elev = zRad2Deg( v.e[zY] );
    tilt = zRad2Deg( v.e[zZ] );
    printf( "%f %f %f\n", azim, elev, tilt );
  } while( azim!=0 || elev!=0 || tilt!=0 );
  return 0;
}

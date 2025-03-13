#include <zeo/zeo_vec3d_profile.h>

void generate_frame(zFrame3D *frame)
{
  zVec3DCreate( zFrame3DPos(frame), zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zMat3DFromZYX( zFrame3DAtt(frame), 0.2*zPI*zRandF(-1,1), 0.2*zPI*zRandF(-1,1), 0.2*zPI*zRandF(-1,1) );
}

void generate_points(zVec3DData *src, zVec3DData *dest, zFrame3D *frame, const char *filename, int n)
{
  zVec3D *po, p;

  if( filename )
    zVec3DDataReadPCDFile( src, filename );
  else{
    zVec3DDataInitList( src );
    while( --n >= 0 ){
      zVec3DCreate( &p, zRandF(-1,1), zRandF(-0.5,0.5), zRandF(-0.2,0.2) );
      zVec3DDataAdd( src, &p );
    }
  }
  zVec3DDataRewind( src );
  zVec3DDataInitList( dest );
  while( ( po = zVec3DDataFetch( src ) ) ){
    zXform3D( frame, po, &p );
    zVec3DDataAdd( dest, &p );
  }
}

void output_points(zVec3DData *src, zVec3DData *dest, zFrame3D *frame)
{
  FILE *fp[3];
  zVec3D *v, p;

  fp[0] = fopen( "s", "w" );
  fp[1] = fopen( "d", "w" );
  fp[2] = fopen( "e", "w" );
  zVec3DDataRewind( src );
  while( ( v = zVec3DDataFetch( src ) ) ){
    zVec3DValueNLFPrint( fp[0], v );
    zXform3D( frame, v, &p );
    zVec3DValueNLFPrint( fp[2], &p );
  }
  zVec3DDataValueFPrint( fp[1], dest );
  fclose( fp[0] );
  fclose( fp[1] );
  fclose( fp[2] );
}

int main(int argc, char *argv[])
{
  int testnum = 1000;
  zVec3DData src, dest;
  zFrame3D frame;

  zRandInit();
  generate_frame( &frame );
  generate_points( &src, &dest, &frame, argc > 1 ? argv[1] : NULL, testnum );
  zVec3DDataICP( &src, &dest, &frame, 0.1, 1.0e-6 );
  output_points( &src, &dest, &frame );
  zVec3DDataDestroy( &dest );
  zVec3DDataDestroy( &src );
  return 0;
}

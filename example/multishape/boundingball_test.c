#include <zeo/zeo_multishape3d.h>

void output(zMultiShape3D *ms, zSphere3D *bb)
{
  zMultiShape3DFPrintZTK( stdout, ms );
  printf( "\n[optic]\n" );
  printf( "name: transparent\n" );
  printf( "diffuse: 1.0 1.0 1.0\n" );
  printf( "alpha: 0.2\n\n" );
  printf( "[shape]\n" );
  printf( "name: bounding_ball\n" );
  printf( "type: sphere\n" );
  printf( "optic: transparent\n" );
  printf( "center: " );
  zVec3DPrint( zSphere3DCenter(bb) );
  printf( "radius: %.10g\n\n", zSphere3DRadius(bb) );
}

int main(int argc, char *argv[])
{
  zMultiShape3D ms;
  zSphere3D bball;

  if( argc < 2 ) return 1;
  if( !zMultiShape3DReadZTK( &ms, argv[1] ) ) return 1;
  zMultiShape3DBoundingBall( &ms, &bball );
  output( &ms, &bball );
  zMultiShape3DDestroy( &ms );
  return 0;
}

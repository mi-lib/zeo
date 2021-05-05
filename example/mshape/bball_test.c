#include <zeo/zeo_mshape3d.h>

void output(zMShape3D *ms, zSphere3D *bb)
{
  zMShape3DFPrintZTK( stdout, ms );
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
  zMShape3D ms;
  zSphere3D bball;

  if( argc < 2 ) return 1;
  if( !zMShape3DReadZTK( &ms, argv[1] ) ) return 1;
  zMShape3DBall( &ms, &bball );
  output( &ms, &bball );
  zMShape3DDestroy( &ms );
  return 0;
}

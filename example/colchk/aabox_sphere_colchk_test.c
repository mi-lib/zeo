#include <zeo/zeo_col.h>

void output_sphere_aabox(zSphere3D *sphere, zAABox3D *aabox)
{
  FILE *fp;
  zBox3D box;

  fp = fopen( "sb.ztk", "w" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.5 0.5 0.5\n" );
  fprintf( fp, "diffuse: 0.4 0.4 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 0.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.5 0.5 0.5\n" );
  fprintf( fp, "diffuse: 1.0 0.4 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 0.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: sphere\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: blue\n" );
  zSphere3DFPrintZTK( fp, sphere );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: box\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zAABox3DToBox3D( aabox, &box );
  zBox3DFPrintZTK( fp, &box );
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zSphere3D sphere;
  zAABox3D aabox;
  zVec3D center;

  zRandInit();
  zVec3DCreate( &center, zRandF(-1,5), zRandF(-1,5), zRandF(-1,5) );
  zSphere3DCreate( &sphere, &center, zRandF(1,5), 0 );
  zAABox3DCreate( &aabox, zRandF(-5,0), zRandF(-5,0), zRandF(-5,0), zRandF(0,1), zRandF(0,1), zRandF(0,1) );
  output_sphere_aabox( &sphere, &aabox );
  printf( "collision - %s\n", zBoolStr( zColChkSphereAABox3D( &sphere, &aabox ) ) );
  return 0;
}

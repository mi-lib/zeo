#include <zeo/zeo_prim.h>

void output_prox(FILE *fp, zVec3D *p, zVec3D *cp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: gray\n" );
  fprintf( fp, "ambient: 0.4 0.4 0.4\n" );
  fprintf( fp, "diffuse: 0.7 0.7 0.7\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.0 0.0\n" );
  fprintf( fp, "diffuse: 0.9 0.0 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.0 0.0 0.6\n" );
  fprintf( fp, "diffuse: 0.0 0.0 0.9\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "exp: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: point\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: blue\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "radius: 0.02\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: closestpoint\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: red\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, cp );
  fprintf( fp, "radius: 0.02\n\n" );

  if( zIsTiny( zVec3DDist( p, cp ) ) ) return;
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: tether\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: gray\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, p );
  fprintf( fp, "center: " ); zVec3DDataNLFPrint( fp, cp );
  fprintf( fp, "radius: 0.005\n\n" );
}

void output(zSphere3D *sphere, zVec3D *p, zVec3D *cp)
{
  FILE *fp;

  fp = fopen( "sphere.z3d", "w" );
  output_prox( fp, p, cp );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: sphere\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: gray\n" );
  zSphere3DFPrint( fp, sphere );
  fclose( fp );
}

int main(void)
{
  zSphere3D sphere;
  zVec3D center;
  zVec3D p, cp;

  zRandInit();
  zVec3DCreate( &center, zRandF(-0.1,0.1), zRandF(-0.1,0.1), zRandF(-0.1,0.1) );
  zSphere3DCreate( &sphere, &center, 0.15, 0 );

  zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zVec3DAddDRC( &p, zSphere3DCenter(&sphere) );
  zSphere3DClosest( &sphere, &p, &cp );

  output( &sphere, &p, &cp );
  return 0;
}

#include <zeo/zeo_shape3d.h>

void output_prox(FILE *fp, zVec3D *p, zVec3D *cp)
{
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: gray\n" );
  fprintf( fp, "ambient: 0.4 0.4 0.4\n" );
  fprintf( fp, "diffuse: 0.7 0.7 0.7\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.0 0.0\n" );
  fprintf( fp, "diffuse: 0.9 0.0 0.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.0 0.0 0.6\n" );
  fprintf( fp, "diffuse: 0.0 0.0 0.9\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );

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

void output(zECyl3D *ecyl, zVec3D *p, zVec3D *cp)
{
  FILE *fp;

  fp = fopen( "ellipticcylinder.ztk", "w" );
  output_prox( fp, p, cp );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ellipticcylinder\n" );
  fprintf( fp, "type: ellipticcylinder\n" );
  fprintf( fp, "optic: gray\n" );
  zECyl3DFPrintZTK( fp, ecyl );
  fclose( fp );
}

int main(void)
{
  zECyl3D ecyl;
  zVec3D c[2];
  zVec3D p, cp;

  zRandInit();
  zVec3DCreate( &c[0],-0.05,-0.05,-0.05 );
  zVec3DCreate( &c[1], 0.05, 0.05, 0.05 );
  zECyl3DCreate( &ecyl, &c[0], &c[1], 0.06, 0.1, ZVEC3DX, 0 );

  zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zECyl3DClosest( &ecyl, &p, &cp );
  output( &ecyl, &p, &cp );
  return 0;
}

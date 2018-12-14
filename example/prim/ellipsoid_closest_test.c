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
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, p );
  fprintf( fp, "radius: 0.02\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: closestpoint\n" );
  fprintf( fp, "type: sphere\n" );
  fprintf( fp, "optic: red\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, cp );
  fprintf( fp, "radius: 0.02\n\n" );

  if( zIsTiny( zVec3DDist( p, cp ) ) ) return;
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: tether\n" );
  fprintf( fp, "type: cylinder\n" );
  fprintf( fp, "optic: gray\n" );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, p );
  fprintf( fp, "center: " ); zVec3DDataNLFWrite( fp, cp );
  fprintf( fp, "radius: 0.005\n\n" );
}

void output(zEllips3D *el, zVec3D *p, zVec3D *cp)
{
  FILE *fp;
  zPH3D ph;

  fp = fopen( "ellipsoid.z3d", "w" );
  output_prox( fp, p, cp );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: ellipsoid\n" );
#if 0
  zEllips3DToPH( el, &ph );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: gray\n" );
  zPH3DFWrite( fp, &ph );
  zPH3DDestroy( &ph );
#else
  fprintf( fp, "type: ellipsoid\n" );
  fprintf( fp, "optic: gray\n" );
  zEllips3DFWrite( fp, el );
#endif

  fclose( fp );
}

int main(void)
{
  zEllips3D el;
  zVec3D center, ax, ay, az;
  zVec3D p, cp;

  zRandInit();
  zVec3DCreate( &center, 0, 0, 0 );
  zVec3DCreate( &ax, 1, 0, 0 );
  zVec3DCreate( &ay, 0,-1.0/sqrt(2), 1.0/sqrt(2) );
  zVec3DOuterProd( &ax, &ay, &az );
  zEllips3DCreate( &el, &center, &ax, &ay, &az, 0.15, 0.2, 0.1, 0 );

  zVec3DCreate( &p, zRandF(-0.2,0.2), zRandF(-0.2,0.2), zRandF(-0.2,0.2) );
  zVec3DAddDRC( &p, zEllips3DCenter(&el) );
  zEllips3DClosest( &el, &p, &cp );

  output( &el, &p, &cp );
  return 0;
}

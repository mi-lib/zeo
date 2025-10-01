#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_col.h>

void test_tri_box(zVec3D p[], zTri3D *t, zAABox3D *b)
{
  zRandInit();
  zVec3DCreate( &p[0], zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zVec3DCreate( &p[1], zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zVec3DCreate( &p[2], zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zTri3DCreate( t, &p[0], &p[1], &p[2] );
  zAABox3DCreate( b, zRandF(-0.3,0), zRandF(-0.3,0), zRandF(-0.3,0), zRandF(0,0.3), zRandF(0,0.3), zRandF(0,0.3) );
}

void output(zTri3D *t, zAABox3D *b, zVec3D ip[], int n)
{
  FILE *fp;
  zBox3D box;
  int i;

  /* for visualization */
  fp = fopen( "tb.ztk", "w" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.6\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 0.0\n\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.6 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.5\n" );
  fprintf( fp, "esr: 0.0\n\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.6 0.6 0.6\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 1.0\n" );
  fprintf( fp, "esr: 0.0\n\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: plane\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: yellow\n" );
  fprintf( fp, "vert: 0 " ); zVec3DValueNLFPrint( fp, zTri3DVert(t,0) );
  fprintf( fp, "vert: 1 " ); zVec3DValueNLFPrint( fp, zTri3DVert(t,1) );
  fprintf( fp, "vert: 2 " ); zVec3DValueNLFPrint( fp, zTri3DVert(t,2) );
  fprintf( fp, "face: 0 1 2\n" );

  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: c\n" );
  fprintf( fp, "type: box\n" );
  fprintf( fp, "optic: red\n" );
  zAABox3DToBox3D( b, &box );
  zBox3DFPrintZTK( fp, &box );

  for( i=0; i<n; i++ ){
    fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
    fprintf( fp, "name: p%d\n", i );
    fprintf( fp, "type: sphere\n" );
    fprintf( fp, "optic: blue\n" );
    fprintf( fp, "center: " );
    zVec3DValueNLFPrint( fp, &ip[i] );
    fprintf( fp, "radius: 0.01\n" );
  }

  fclose( fp );
}

int main(int argc, char *argv[])
{
  zTri3D t;
  zAABox3D b;
  zVec3D p[3], ip[9];
  int n;

  test_tri_box( p, &t, &b );
  eprintf( "collision = %s\n", zBoolStr( zColChkTriAABox3D( &t, &b ) ) );
  n = zIntersectTriAABox3D( &t, &b, ip );
  eprintf( "num. of intersection point = %d\n", n );
  output( &t, &b, ip, n );
  return 0;
}

/* qhull: 3D convex hull computation based on quickhull algorithm. */

#include <zeo/zeo_mshape.h>
#include <zeo/zeo_bv.h>

void usage(char *arg)
{
  eprintf( "Usage: %s <vlist file>\n", arg );
  eprintf( "       %s <.z3d file>\n", arg );
  exit( EXIT_SUCCESS );
}

void mshape2vlist(zMShape3D *ms, zVec3DAddrList *vl)
{
  register int i, j;
  zShape3D *s;

  zListInit( vl );
  for( i=0; i<zMShape3DShapeNum(ms); i++ ){
    s = zMShape3DShape(ms,i);
    for( j=0; j<zShape3DVertNum(s); j++ )
      if( !zVec3DAddrListAdd( vl, zShape3DVert(s,j) ) ){
        ZALLOCERROR();
        exit( EXIT_FAILURE );
      }
  }
}

void read_vlist(char filename[], zVec3DAddrList *vl)
{
  FILE *fp;
  zVec3D v;
  double x, y, z;
  char buf[BUFSIZ];

  if( !( fp = fopen( filename, "r" ) ) ){
    ZOPENERROR( filename );
    exit( EXIT_FAILURE );
  }
  zListInit( vl );
  do{
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    x = atof( buf );
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    y = atof( buf );
    if( !zFToken( fp, buf, BUFSIZ ) ) break;
    z = atof( buf );
    zVec3DCreate( &v, x, y, z );
    zVec3DAddrListAdd( vl, &v );
  } while( !feof( fp ) );
  fclose( fp );
}

void output(zPH3D *ch)
{
  /* for visualization */
  printf( "[optic]\n" );
  printf( "name: cyan\n" );
  printf( "ambient: 0.7 1.0 1.0\n" );
  printf( "diffuse: 0.7 1.0 1.0\n" );
  printf( "specular: 0.0 0.0 0.0\n" );
  printf( "alpha: 0.4\n" );
  printf( "exp: 0.0\n\n" );
  printf( "[shape]\n" );
  printf( "name: ch\n" );
  printf( "type: polyhedron\n" );
  printf( "optic: cyan\n" );
  zPH3DFPrintZTK( stdout, ch );
}

int main(int argc, char *argv[])
{
  zMShape3D ms;
  zVec3DAddrList vl;
  zPH3D ch;
  char *sfx;

  if( argc < 2 ) usage( argv[0] );
  sfx = zGetSuffix( argv[1] );
  if( sfx && strcmp( sfx, ZEDA_ZTK_SUFFIX ) == 0 ){
    if( !zMShape3DReadZTK( &ms, argv[1] ) ) return 1;
    mshape2vlist( &ms, &vl );
  } else{
    zMShape3DInit( &ms );
    read_vlist( argv[1], &vl );
  }

  zCH3DPL( &ch, &vl );
  output( &ch );
  zPH3DDestroy( &ch );
  zVec3DAddrListDestroy( &vl );
  zMShape3DDestroy( &ms );
  return 0;
}

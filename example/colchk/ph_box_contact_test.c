#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_col.h>
#include <zeo/zeo_brep.h> /* for comparison */

void output(char filename[], zPH3D *a, zPH3D *b, zPH3D *c)
{
  FILE *fp;

  fp = fopen( filename, "w" );
  /* for visualization */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: red\n" );
  fprintf( fp, "ambient: 0.8 0.2 0.2\n" );
  fprintf( fp, "diffuse: 1.0 0.2 0.2\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: blue\n" );
  fprintf( fp, "ambient: 0.2 0.2 0.8\n" );
  fprintf( fp, "diffuse: 0.2 0.2 1.0\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.4\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  fprintf( fp, "name: yellow\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.4\n" );
  fprintf( fp, "diffuse: 1.0 1.0 0.4\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.6\n" );
  fprintf( fp, "esr: 1.0\n\n" );
  /* intersection convex */
  if( c ){
    fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
    fprintf( fp, "name: c\n" );
    fprintf( fp, "type: polyhedron\n" );
    fprintf( fp, "optic: yellow\n" );
    zPH3DFPrintZTK( fp, c );
  }
  /* convex 1 */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, a );
  /* convex 2 */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, b );

  fclose( fp );
}

zPH3D* create_ph_from_ztk_file(const char *filename, zPH3D *ph)
{
  ZTK ztk;
  FILE *fp;

  if( !(fp = zOpenZTKFile( filename, "r" )) ){
    return NULL;
  }
  ZTKInit( &ztk );
  if( !ZTKParseFP( &ztk, fp ) ){
    ZRUNERROR( "Failed parse %s.", filename );
    fclose( fp );
    ZTKDestroy(&ztk);
    return NULL;
  }
  fclose( fp );
  if( !zPH3DFromZTK( ph, &ztk) ){
    ZTKDestroy(&ztk);
    ZRUNERROR( "Failed create PH3D from %s.", filename );
    return NULL;
  }
  ZTKDestroy(&ztk);

  return ph;
}

int main(int argc, char *argv[])
{
  zPH3D a, b, c, ip;
  clock_t t1, t2;
  int dt;

  if( !create_ph_from_ztk_file( "ph_box_ztk/ph_box_a.ztk", &a ) )
    return 1;
  if( !create_ph_from_ztk_file( "ph_box_ztk/ph_box_b.ztk", &b ) )
    return 1;
  if( !create_ph_from_ztk_file( "ph_box_ztk/ph_box_c.ztk", &c ) )
    return 1;

  output( "ph_box_org_ab.ztk", &a, &b, NULL );
  t1 = clock();
  if( !zIntersectPH3D( &a, &b, &ip ) ){
    eprintf( "ab not intersect.\n" );
  }
  t2 = clock();
  dt = t2 - t1;
  eprintf( "[ph_box_contact] time=%d\n", dt );
  output( "ph_box_contact_ab.ztk", &a, &b, &ip );
  zPH3DDestroy( &ip );

  output( "ph_box_org_cb.ztk", &c, &b, NULL );
  t1 = clock();
  if( !zIntersectPH3D( &c, &b, &ip ) ){
    eprintf( "cb not intersect.\n" );
    return 1;
  }
  t2 = clock();
  dt = t2 - t1;
  eprintf( "[ph_box_contact] time=%d\n", dt );
  output( "ph_box_contact_cb.ztk", &c, &b, &ip );
  zPH3DDestroy( &ip );

  zPH3DDestroy( &c );
  zPH3DDestroy( &b );
  zPH3DDestroy( &a );

  return 0;
}

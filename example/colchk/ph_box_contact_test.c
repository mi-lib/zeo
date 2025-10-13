#include <zeo/zeo_shape3d.h>
#include <zeo/zeo_col.h>

const zVec3D vert_a[] = {
  { { -0.0499181786366895003, 0.0476764034617871216, 0.0268075982701843699 } },
  { { -0.0983260953879252636, 0.053838117320302005, 0.114092802503955912 } },
  { { -0.101031977119079616, -0.0459713476494253356, 0.119637972159951825 } },
  { { -0.0526240603678438529, -0.052133061507940219, 0.0323527679261802689 } },
  { { -0.137378751467322369, 0.0473539367463602298, -0.0216748131318386383 } },
  { { -0.185786668218558132, 0.0535156506048751132, 0.0656103911019329245 } },
  { { -0.188492549949712485, -0.0462938143648522274, 0.0711555607579288235 } },
  { { -0.140084633198476721, -0.0524555282233671108, -0.0161296434758427254 } } };

const zVec3D vert_b[] = {
  { { 0.05, 0.05, 0.05 } },
  { { -0.05, 0.05, 0.05 } },
  { { -0.05, -0.05, 0.05 } },
  { { 0.05, -0.05, 0.05 } },
  { { 0.05, 0.05, -0.05 } },
  { { -0.05, 0.05, -0.05 } },
  { { -0.05, -0.05, -0.05 } },
  { { 0.05, -0.05, -0.05 } } };

const zVec3D vert_c[] = {
  { { -0.105524345295836611, 0.0489486182471013942, 0.150705549082312373 } },
  { { -0.106232658329327628, 0.0489486819473484297, 0.0507080576505643046 } },
  { { -0.106268653013158368, -0.0510513115743821411, 0.0507082489101562048 } },
  { { -0.10556033997966735, -0.0510513752746291766, 0.150705740341904287 } },
  { { -0.00552686034231729493, 0.0489126231115052518, 0.149997236071779089 } },
  { { -0.00623517337580831253, 0.0489126868117522873, 0.04999974464003102 } },
  { { -0.00627116805963905209, -0.0510873067099782835, 0.0499999358996229201 } },
  { { -0.00556285502614803449, -0.051087370410225319, 0.149997427331371003 } } };

const zVec3D vert_d[] = {
  { { -0.123697229383016222, 0.0499970207665908842, 0.165604591710331428 } },
  { { -0.1407697465052129, 0.049998396215206721, 0.067072722971225282 } },
  { { -0.140768398431413644, -0.0500016037624300483, 0.0670710934489536781 } },
  { { -0.123695881309216965, -0.0500029792110458851, 0.16560296218805981 } },
  { { -0.0251653606435317465, 0.049998627249366108, 0.148532074610494752 } },
  { { -0.0422378777657284321, 0.0500000026979819448, 0.0500002058713886199 } },
  { { -0.0422365296919291686, -0.0499999972796548245, 0.049998576349117016 } },
  { { -0.0251640125697324829, -0.0500013727282706613, 0.148530445088223162 } } };

void output(const char *filename, zPH3D *a, zPH3D *b, zPH3D *ip)
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
  /* convex 1 */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: a\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: red\n" );
  zPH3DFPrintZTK( fp, a );
  fprintf( fp, "\n" );
  /* convex 2 */
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: b\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: blue\n" );
  zPH3DFPrintZTK( fp, b );
  fprintf( fp, "\n" );
  /* intersection convex */
  if( ip ){
    fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
    fprintf( fp, "name: intersection\n" );
    fprintf( fp, "type: polyhedron\n" );
    fprintf( fp, "optic: yellow\n" );
    zPH3DFPrintZTK( fp, ip );
  }
  fclose( fp );
}

bool create_ph_box(zPH3D *ph, const zVec3D vert[])
{
  int i;

  if( !zPH3DAlloc( ph, 8, 12 ) ) return false;
  for( i=0; i<8; i++ )
    zVec3DCopy( &vert[i], zPH3DVert(ph,i) );
  zTri3DCreate( zPH3DFace(ph,0),  zPH3DVert(ph,0), zPH3DVert(ph,1), zPH3DVert(ph,2) );
  zTri3DCreate( zPH3DFace(ph,1),  zPH3DVert(ph,0), zPH3DVert(ph,2), zPH3DVert(ph,3) );
  zTri3DCreate( zPH3DFace(ph,2),  zPH3DVert(ph,0), zPH3DVert(ph,4), zPH3DVert(ph,5) );
  zTri3DCreate( zPH3DFace(ph,3),  zPH3DVert(ph,0), zPH3DVert(ph,5), zPH3DVert(ph,1) );
  zTri3DCreate( zPH3DFace(ph,4),  zPH3DVert(ph,1), zPH3DVert(ph,5), zPH3DVert(ph,6) );
  zTri3DCreate( zPH3DFace(ph,5),  zPH3DVert(ph,1), zPH3DVert(ph,6), zPH3DVert(ph,2) );
  zTri3DCreate( zPH3DFace(ph,6),  zPH3DVert(ph,2), zPH3DVert(ph,6), zPH3DVert(ph,7) );
  zTri3DCreate( zPH3DFace(ph,7),  zPH3DVert(ph,2), zPH3DVert(ph,7), zPH3DVert(ph,3) );
  zTri3DCreate( zPH3DFace(ph,8),  zPH3DVert(ph,0), zPH3DVert(ph,3), zPH3DVert(ph,7) );
  zTri3DCreate( zPH3DFace(ph,9),  zPH3DVert(ph,0), zPH3DVert(ph,7), zPH3DVert(ph,4) );
  zTri3DCreate( zPH3DFace(ph,10), zPH3DVert(ph,7), zPH3DVert(ph,6), zPH3DVert(ph,5) );
  zTri3DCreate( zPH3DFace(ph,11), zPH3DVert(ph,7), zPH3DVert(ph,5), zPH3DVert(ph,4) );
  return true;
}

void test_ph_box(zPH3D *a, zPH3D *b, const char *filename)
{
  zPH3D ip;

  if( !zIntersectPH3D( a, b, &ip ) ){
    eprintf( "boxes do not intersect.\n" );
  } else{
    eprintf( "intersection volume is output to %s.\n", filename );
    output( filename, a, b, &ip );
  }
  zPH3DDestroy( &ip );
}

int main(int argc, char *argv[])
{
  zPH3D a, b, c, d;

  if( !create_ph_box( &a, vert_a ) ||
      !create_ph_box( &b, vert_b ) ||
      !create_ph_box( &c, vert_c ) ||
      !create_ph_box( &d, vert_d ) ) return 1;

  eprintf( "[case 1]\n" );
  test_ph_box( &a, &b, "ph_box_contact_ab.ztk" );
  eprintf( "[case 2]\n" );
  test_ph_box( &c, &b, "ph_box_contact_cb.ztk" );
  eprintf( "[case 3]\n" );
  test_ph_box( &d, &b, "ph_box_contact_db.ztk" );

  zPH3DDestroy( &d );
  zPH3DDestroy( &c );
  zPH3DDestroy( &b );
  zPH3DDestroy( &a );
  return 0;
}

#include <zeo/zeo_prim.h>

int main(void)
{
  zBox3D box;
  zVec3D v;
  register int i;

  zVec3DCreate( &v, 0.25, 0.15, 0.05 );
  zBox3DCreateAlign( &box, &v, 5.0, 1.0, 3.0 );
  zBox3DPrint( &box );
  for( i=0; i<8; i++ ){
    zBox3DVert( &box, i, &v );
    printf( "vert #%d: ", i );
    zVec3DPrint( &v );
  }
  return 0;
}

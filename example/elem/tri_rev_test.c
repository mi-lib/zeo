#include <zeo/zeo_elem.h>

int main(void)
{
  zVec3D p[] = {
    { { 0, 0, 0 } },
    { { 2, 0, 0 } },
    { { 1, 1, 0 } }
  };
  zTri3D t1, t2;

  zTri3DCreate( &t1, &p[0], &p[1], &p[2] );
  printf( "original triangle\n" );
  zTri3DPrint( &t1 );
  zTri3DRev( &t1, &t2 );
  printf( "reversed triangle\n" );
  zTri3DPrint( &t2 );
  zTri3DRevDRC( &t1 );
  printf( "reversed triangle (destructive)\n" );
  zTri3DPrint( &t1 );
  return 0;
}

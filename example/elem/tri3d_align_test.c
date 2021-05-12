#include <zeo/zeo_elem3d.h>

int main(void)
{
  zVec3D v[] = {
    { { 0, 0, 0 } },
    { { 0, 1, 0 } },
    { { 1, 0, 0 } },
    { { 1, 1, 0 } },
    { { 2, 0, 0 } },
    { { 2, 1, 0 } },
  };
  zTri3D t;
  zTri3DList tlist;
  zTri3DListCell *tp;

  zListInit( &tlist );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  zTri3DListAdd( &tlist, &t );
  zTri3DCreate( &t, &v[2], &v[3], &v[1] );
  zTri3DListAdd( &tlist, &t );
  zTri3DCreate( &t, &v[2], &v[3], &v[5] );
  zTri3DListAdd( &tlist, &t );
  zTri3DCreate( &t, &v[2], &v[4], &v[5] );
  zTri3DListAdd( &tlist, &t );

  printf( ">>normal vectors\n" );
  zListForEach( &tlist, tp )
    zTri3DPrint( &tp->data );

  zTri3DListAlign( &tlist, ZVEC3DZ );
  printf( ">>normal vectors (after aligned)\n" );
  zListForEach( &tlist, tp )
    zTri3DPrint( &tp->data );

  zTri3DListDestroy( &tlist );
  return 0;
}

#include <zeo/zeo_elem.h>

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
  zTri3DListInsert( &tlist, &t, true );
  zTri3DCreate( &t, &v[2], &v[3], &v[1] );
  zTri3DListInsert( &tlist, &t, true );
  zTri3DCreate( &t, &v[2], &v[3], &v[5] );
  zTri3DListInsert( &tlist, &t, true );
  zTri3DCreate( &t, &v[2], &v[4], &v[5] );
  zTri3DListInsert( &tlist, &t, true );

  printf( ">>normal vectors\n" );
  zListForEach( &tlist, tp )
    zTri3DWrite( tp->data );

  zTri3DListAlign( &tlist, Z_UNITZVEC3D );
  printf( ">>normal vectors (after aligned)\n" );
  zListForEach( &tlist, tp )
    zTri3DWrite( tp->data );

  zTri3DListDestroy( &tlist, true );
  return 0;
}

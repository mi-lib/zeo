#include <zeo/zeo_vec3d.h>

#define N 20

void test_vert(zVec3DList *vl)
{
  register int i;
  zVec3D v;

  zRandInit();
  zListInit( vl );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DListInsert( vl, &v );
  }
}

int cmp(void *a, void *b, void *dummy)
{
  zVec3D *v1, *v2;

  v1 = ((zVec3DListCell*)a)->data;
  v2 = ((zVec3DListCell*)b)->data;
  if( v1->e[zX] > v2->e[zX] ) return 1;
  if( v1->e[zX] < v2->e[zX] ) return -1;
  if( v1->e[zY] > v2->e[zY] ) return 1;
  if( v1->e[zY] < v2->e[zY] ) return -1;
  if( v1->e[zZ] > v2->e[zZ] ) return 1;
  if( v1->e[zZ] < v2->e[zZ] ) return -1;
  return 0;
}

int main(void)
{
  zVec3DList vl;

  test_vert( &vl );
  zVec3DListWrite( &vl );
  zVec3DListQuickSort( &vl, cmp, NULL );
  zVec3DListWrite( &vl );
  zVec3DListDestroy( &vl );
  return 0;
}

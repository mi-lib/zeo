#include <zeo/zeo_vec3d.h>

#define N 100

void create_test_list(zVec3DList *vl, int n)
{
  zVec3D v;
  int i;

  zListInit( vl );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DListAdd( vl, &v );
  }
}

void assert_list_clone(void)
{
  zVec3DList src, dest;
  zVec3DListCell *sp, *dp;
  bool result;

  create_test_list( &src, N );
  zVec3DListClone( &src, &dest );
  for( result=true, sp=zListTail(&src), dp=zListTail(&dest);
       sp!=zListRoot(&src) && dp!=zListRoot(&dest);
       sp=zListCellNext(sp), dp=zListCellNext(dp) ){
    if( !zVec3DEqual( &sp->data, &dp->data ) ) result = false;
  }
  zVec3DListDestroy( &src );
  zVec3DListDestroy( &dest );
  zAssert( zVec3DListClone, result );
}

void assert_addr_list_clone(void)
{
  zVec3DAddrList src, dest;
  zVec3DAddrListCell *sp, *dp;
  zVec3D v[N];
  int i;
  bool result;

  zListInit( &src );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v[i], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DAddrListAdd( &src, &v[i] );
  }
  zVec3DAddrListClone( &src, &dest );
  for( result=true, sp=zListTail(&src), dp=zListTail(&dest);
       sp!=zListRoot(&src) && dp!=zListRoot(&dest);
       sp=zListCellNext(sp), dp=zListCellNext(dp) ){
    if( !zVec3DEqual( sp->data, dp->data ) ) result = false;
  }
  zVec3DAddrListDestroy( &src );
  zVec3DAddrListDestroy( &dest );
  zAssert( zVec3DAddrListClone, result );
}

int cmp(void *a, void *b, void *dummy)
{
  zVec3D *v1, *v2;

  v1 = &((zVec3DListCell*)a)->data;
  v2 = &((zVec3DListCell*)b)->data;
  if( v1->e[zX] > v2->e[zX] ) return 1;
  if( v1->e[zX] < v2->e[zX] ) return -1;
  if( v1->e[zY] > v2->e[zY] ) return 1;
  if( v1->e[zY] < v2->e[zY] ) return -1;
  if( v1->e[zZ] > v2->e[zZ] ) return 1;
  if( v1->e[zZ] < v2->e[zZ] ) return -1;
  return 0;
}

void assert_vec_list_quicksort(void)
{
  zVec3DList vl;
  zVec3DListCell *cp;
  bool result = true;

  create_test_list( &vl, N );
  zVec3DListQuickSort( &vl, cmp, NULL );
  zListForEach( &vl, cp ){
    if( zListCellNext(cp) == zListRoot(&vl) ) break;
    if( cp->data.c.x > zListCellNext(cp)->data.c.x ){
      result = false;
      break;
    }
  }
  zVec3DListDestroy( &vl );
  zAssert( zVec3DListQuickSort, result );
}

int main(void)
{
  zRandInit();
  assert_list_clone();
  assert_addr_list_clone();
  assert_vec_list_quicksort();
  return 0;
}

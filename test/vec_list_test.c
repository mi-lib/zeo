#include <zeo/zeo_vec3d.h>

#define N 10

void assert_list_clone(void)
{
  zVec3DList src, dest;
  zVec3DListCell *sp, *dp;
  zVec3D v;
  int i;
  bool result;

  zListInit( &src );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DListAdd( &src, &v );
  }
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

int main(void)
{
  zRandInit();
  assert_list_clone();
  assert_addr_list_clone();
  return 0;
}

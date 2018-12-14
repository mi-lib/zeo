#include <zeo/zeo_vec6d.h>

int main(void)
{
  zVec6D v, vo;

  zVec3DToVec6D( &v, Z_UNITXVEC3D, Z_UNITZVEC3D );
  printf( "suppose the object moves at the velocity:\n" );
  zVec6DWrite( &v );
  printf( "the velocity of the point at:\n" );
  zVec3DWrite( Z_UNITXVEC3D );
  printf( "is:\n" );
  zVec6DWrite( zVec6DLinShift( &v, Z_UNITXVEC3D, &vo ) );

  printf( "\nsuppose the force:\n" );
  zVec6DWrite( &v );
  printf( "is exerted to the object at the point:\n" );
  zVec3DWrite( Z_UNITYVEC3D );
  printf( "the equivalent 6D force at the original point is:\n" );
  zVec6DWrite( zVec6DAngShift( &v, Z_UNITYVEC3D, &vo ) );

  return 0;
}

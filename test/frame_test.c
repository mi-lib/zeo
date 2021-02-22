#include <zeo/zeo.h>

zFrame3D *frame_gen_rand(zFrame3D *f)
{
  return zFrame3DFromZYX( f, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

zVec3D *vec_gen_rand(zVec3D *v)
{
  return zVec3DCreate( v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
}

#define N 1000

void assert_xform(void)
{
  zFrame3D f;
  zVec3D v1, v2, v, e;
  int i;
  bool result1 = true, result2 = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f );
    vec_gen_rand( &v1 );

    _zXform3D( &f, &v1, &v2 );
    _zXform3DInv( &f, &v2, &v );
    if( !zVec3DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec3DPrint( zVec3DSub( &v, &v1, &e ) );
      result1 = false;
    }
    _zXform3DInv( &f, &v1, &v2 );
    _zXform3D( &f, &v2, &v );
    if( !zVec3DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec3DPrint( zVec3DSub( &v, &v1, &e ) );
      result2 = false;
    }
  }
  zAssert( zXform3D + zXform3DInv, result1 );
  zAssert( zXform3DInv + zXform3D, result2 );
}

void assert_frame_inv(void)
{
  zFrame3D f1, f2;
  zVec3D v1, v2, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    vec_gen_rand( &v1 );

    _zFrame3DInv( &f1, &f2 );
    _zXform3D( &f2, &v1, &v2 );
    _zXform3DInv( &f1, &v1, &v );
    if( !zVec3DEqual( &v, &v2 ) ){
      eprintf( "(error): " );
      zVec3DPrint( zVec3DSub( &v, &v2, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DInv, result );
}

void assert_frame_cascade(void)
{
  zFrame3D f1, f2, f3;
  zVec3D v1, v2, v3, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    frame_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform3D( &f2, &v2, &v1 );
    _zXform3D( &f1, &v1, &v );
    _zFrame3DCascade( &f1, &f2, &f3 );
    _zXform3D( &f3, &v2, &v3 );
    if( !zVec3DEqual( &v, &v3 ) ){
      eprintf( "(error): " );
      zVec3DPrint( zVec3DSub( &v, &v3, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DCascade, result );
}

void assert_frame_xform(void)
{
  zFrame3D f1, f2, f3;
  zVec3D v1, v2, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    frame_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform3D( &f2, &v2, &v );
    _zXform3DInv( &f1, &v, &v1 );
    _zFrame3DXform( &f1, &f2, &f3 );
    _zXform3D( &f3, &v2, &v );
    if( !zVec3DEqual( &v, &v1 ) ){
      eprintf( "(error): " );
      zVec3DPrint( zVec3DSub( &v, &v1, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DXform, result );
}

int main(void)
{
  zRandInit();
  assert_xform();
  assert_frame_inv();
  assert_frame_cascade();
  assert_frame_xform();
  return 0;
}

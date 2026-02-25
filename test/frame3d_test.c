#include <zeo/zeo.h>

zFrame3D *frame3d_gen_rand(zFrame3D *f)
{
  return zFrame3DFromPosZYX( f, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

zVec3D *vec_gen_rand(zVec3D *v)
{
  return zVec3DCreate( v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
}

void assert_frame3d_misc(void)
{
  zFrame3D f;

  frame3d_gen_rand( &f );
  zAssert( zFrame3DPos, zVec3DEqual( zFrame3DPos(&f), &f.pos ) );
  zAssert( zFrame3DAtt, zMat3DEqual( zFrame3DAtt(&f), &f.att ) );
  zAssert( zFrame3DVec,
    zVec3DEqual( zFrame3DVec(&f,zX), &f.att.b.x ) &&
    zVec3DEqual( zFrame3DVec(&f,zY), &f.att.b.y ) &&
    zVec3DEqual( zFrame3DVec(&f,zZ), &f.att.b.z ) );
}

void assert_xform(void)
{
  zFrame3D f;
  zVec3D v1, v2, v, e;
  const int n = 100;
  int i;
  bool result1 = true, result2 = true;

  for( i=0; i<n; i++ ){
    frame3d_gen_rand( &f );
    vec_gen_rand( &v1 );

    _zXform3D( &f, &v1, &v2 );
    _zXform3DInv( &f, &v2, &v );
    if( !zVec3DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, zVec3DSub( &v, &v1, &e ) );
      result1 = false;
    }
    _zXform3DInv( &f, &v1, &v2 );
    _zXform3D( &f, &v2, &v );
    if( !zVec3DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, zVec3DSub( &v, &v1, &e ) );
      result2 = false;
    }
  }
  zAssert( zXform3D + zXform3DInv, result1 );
  zAssert( zXform3DInv + zXform3D, result2 );
}

void assert_frame3d_inv(void)
{
  zFrame3D f1, f2;
  zVec3D v1, v2, v, e;
  const int n = 100;
  int i;
  bool result = true;

  for( i=0; i<n; i++ ){
    frame3d_gen_rand( &f1 );
    vec_gen_rand( &v1 );

    _zFrame3DInv( &f1, &f2 );
    _zXform3D( &f2, &v1, &v2 );
    _zXform3DInv( &f1, &v1, &v );
    if( !zVec3DEqual( &v, &v2 ) ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, zVec3DSub( &v, &v2, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DInv, result );
}

void assert_frame3d_cascade(void)
{
  zFrame3D f1, f2, f3;
  zVec3D v1, v2, v3, v, e;
  const int n = 100;
  int i;
  bool result = true;

  for( i=0; i<n; i++ ){
    frame3d_gen_rand( &f1 );
    frame3d_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform3D( &f2, &v2, &v1 );
    _zXform3D( &f1, &v1, &v );
    _zFrame3DCascade( &f1, &f2, &f3 );
    _zXform3D( &f3, &v2, &v3 );
    if( !zVec3DEqual( &v, &v3 ) ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, zVec3DSub( &v, &v3, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DCascade, result );
}

void assert_frame3d_xform(void)
{
  zFrame3D f1, f2, f3;
  zVec3D v1, v2, v, e;
  const int n = 100;
  int i;
  bool result = true;

  for( i=0; i<n; i++ ){
    frame3d_gen_rand( &f1 );
    frame3d_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform3D( &f2, &v2, &v );
    _zXform3DInv( &f1, &v, &v1 );
    _zFrame3DXform( &f1, &f2, &f3 );
    _zXform3D( &f3, &v2, &v );
    if( !zVec3DEqual( &v, &v1 ) ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, zVec3DSub( &v, &v1, &e ) );
      result = false;
    }
  }
  zAssert( zFrame3DXform, result );
}

void assert_frame3d_ztk(void)
{
  ZTK ztk;
  zFrame3D src, dest;
  zVec6D error;

  zMat3DCreate( zFrame3DAtt(&src), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( zFrame3DPos(&src), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  ZTKInit( &ztk );
  ZTKAddTag( &ztk, "" );
  ZTKAddKey( &ztk, "" );
  zFrame3DToZTK( &src, &ztk );
  ZTKRewind( &ztk );
  zFrame3DFromZTK( &dest, &ztk );
  ZTKDestroy( &ztk );
  zFrame3DError( &src, &dest, &error );
  zAssert( zFrame3DFromZTK + zFrame3DToZTK, zVec6DIsTol( &error, 1.0e-9 ) );
}

int main(void)
{
  zRandInit();
  assert_frame3d_misc();
  assert_xform();
  assert_frame3d_inv();
  assert_frame3d_cascade();
  assert_frame3d_xform();
  assert_frame3d_ztk();
  return 0;
}

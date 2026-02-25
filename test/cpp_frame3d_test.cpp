#include <zeo/zeo_frame3d.h>

void frame3d_gen_rand(zFrame3D &f)
{
  f.createPosZYX( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-zPI,zPI), 0.5*zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
}

void vec_gen_rand(zVec3D &v)
{
  v.create( zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
}

void assert_xform()
{
  const int n = 100;
  bool result1 = true, result2 = true;

  for(int i=0; i<n; i++ ){
    zFrame3D f; frame3d_gen_rand( f );
    zVec3D v1;  vec_gen_rand( v1 );
    zVec3D v2 = f.xform( v1 );
    zVec3D v3 = f.xformInv( v2 );
    zVec3D e = v3 - v1;
    if( !e.isTol() ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, &e );
      result1 = false;
    }
    v2 = f.xformInv( v1 );
    v3 = f.xform( v2 );
    e = v3 - v1;
    if( !e.isTol() ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, &e );
      result2 = false;
    }
  }
  zAssert( C++::zFrame3D (xform + xformInv), result1 );
  zAssert( C++::zFrame3D (xformInv + xform), result2 );
}

void assert_frame3d_inv()
{
  const int n = 100;
  bool result = true;

  for(int i=0; i<n; i++ ){
    zFrame3D f1; frame3d_gen_rand( f1 );
    zVec3D v1;   vec_gen_rand( v1 );
    zFrame3D f2 = f1.inv();
    zVec3D v2 = f2.xform( v1 );
    zVec3D v3 = f1.xformInv( v1 );
    zVec3D e = v2 - v3;
    if( !e.isTol() ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, &e );
      result = false;
    }
  }
  zAssert( C++::zFrame3D (inv), result );
}

void assert_frame3d_cascade()
{
  const int n = 100;
  bool result1 = true, result2 = true;

  for(int i=0; i<n; i++ ){
    zFrame3D f1; frame3d_gen_rand( f1 );
    zFrame3D f2; frame3d_gen_rand( f2 );
    zVec3D v1;   vec_gen_rand( v1 );
    zVec3D vtmp = f2.xform( v1 );
    zVec3D v2 = f1.xform( vtmp );
    zFrame3D f3 = f1 * f2;
    zVec3D v3 = f3.xform( v1 );
    zVec3D e1 = v2 - v3;
    if( !e1.isTol() ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, &e1 );
      result1 = false;
    }
    zVec3D v4 = f1 * f2 * v1;
    zVec3D e2 = v2 - v4;
    if( !e2.isTol() ){
      eprintf( "(error): " );
      zVec3DFPrint( stderr, &e2 );
      result2 = false;
    }
  }
  zAssert( C++::zFrame3D (cascade stepwise), result1 );
  zAssert( C++::zFrame3D (cascade batch), result2 );
}

void assert_frame3d_xform()
{
  const int n = 100;
  bool result1 = true, result2 = true;

  for(int i=0; i<n; i++ ){
    zFrame3D f1;  frame3d_gen_rand( f1 );
    zFrame3D df1; frame3d_gen_rand( df1 );
    zFrame3D f2 = f1 * df1;
    zFrame3D df2 = f1.inv() * f2;
    zVec6D e = df1 - df2;
    if( !e.isTol() ){
      eprintf( "(error): " );
      zVec6DFPrint( stderr, &e );
      result1 = false;
    }
    zFrame3D df3;
    zFrame3DXform( &f1, &f2, &df3 );
    e = df1 - df3;
    if( !e.isTol() ){
      eprintf( "(error): " );
      zVec6DFPrint( stderr, &e );
      result2 = false;
    }
  }
  zAssert( C++::zFrame3D (equivalent xform), result1 );
  zAssert( C++::zFrame3D (xform), result2 );
}

int main()
{
  zRandInit();
  assert_xform();
  assert_frame3d_inv();
  assert_frame3d_cascade();
  assert_frame3d_xform();
  return 0;
}

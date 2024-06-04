#include <zeo/zeo.h>

zFrame2D *frame_gen_rand(zFrame2D *f)
{
  zVec2D pos;

  zVec2DCreate( &pos, zRandF(-10.0,10.0), zRandF(-10.0,10.0) );
  return zFrame2DCreateAngle( f, &pos, zRandF(-zPI,zPI) );
}

zVec2D *vec_gen_rand(zVec2D *v)
{
  return zVec2DCreate( v, zRandF(-10,10), zRandF(-10,10) );
}

#define N 1000

void assert_frame_misc(void)
{
  zFrame2D f;

  frame_gen_rand( &f );
  zAssert( zFrame2DPos, zVec2DEqual( zFrame2DPos(&f), &f.pos ) );
  zAssert( zFrame2DAtt, zMat2DEqual( zFrame2DAtt(&f), &f.att ) );
  zAssert( zFrame2DVec,
    zVec2DEqual( zFrame2DVec(&f,zX), &f.att.b.x ) &&
    zVec2DEqual( zFrame2DVec(&f,zY), &f.att.b.y ) );
}

void assert_xform(void)
{
  zFrame2D f;
  zVec2D v1, v2, v, e;
  int i;
  bool result1 = true, result2 = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f );
    vec_gen_rand( &v1 );

    _zXform2D( &f, &v1, &v2 );
    _zXform2DInv( &f, &v2, &v );
    if( !zVec2DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec2DPrint( zVec2DSub( &v, &v1, &e ) );
      result1 = false;
    }
    _zXform2DInv( &f, &v1, &v2 );
    _zXform2D( &f, &v2, &v );
    if( !zVec2DEqual( &v1, &v ) ){
      eprintf( "(error): " );
      zVec2DPrint( zVec2DSub( &v, &v1, &e ) );
      result2 = false;
    }
  }
  zAssert( zXform2D + zXform2DInv, result1 );
  zAssert( zXform2DInv + zXform2D, result2 );
}

void assert_frame_inv(void)
{
  zFrame2D f1, f2;
  zVec2D v1, v2, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    vec_gen_rand( &v1 );

    _zFrame2DInv( &f1, &f2 );
    _zXform2D( &f2, &v1, &v2 );
    _zXform2DInv( &f1, &v1, &v );
    if( !zVec2DEqual( &v, &v2 ) ){
      eprintf( "(error): " );
      zVec2DPrint( zVec2DSub( &v, &v2, &e ) );
      result = false;
    }
  }
  zAssert( zFrame2DInv, result );
}

void assert_frame_cascade(void)
{
  zFrame2D f1, f2, f3;
  zVec2D v1, v2, v3, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    frame_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform2D( &f2, &v2, &v1 );
    _zXform2D( &f1, &v1, &v );
    _zFrame2DCascade( &f1, &f2, &f3 );
    _zXform2D( &f3, &v2, &v3 );
    if( !zVec2DEqual( &v, &v3 ) ){
      eprintf( "(error): " );
      zVec2DPrint( zVec2DSub( &v, &v3, &e ) );
      result = false;
    }
  }
  zAssert( zFrame2DCascade, result );
}

void assert_frame_xform(void)
{
  zFrame2D f1, f2, f3;
  zVec2D v1, v2, v, e;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    frame_gen_rand( &f1 );
    frame_gen_rand( &f2 );
    vec_gen_rand( &v2 );

    _zXform2D( &f2, &v2, &v );
    _zXform2DInv( &f1, &v, &v1 );
    _zFrame2DXform( &f1, &f2, &f3 );
    _zXform2D( &f3, &v2, &v );
    if( !zVec2DEqual( &v, &v1 ) ){
      eprintf( "(error): " );
      zVec2DPrint( zVec2DSub( &v, &v1, &e ) );
      result = false;
    }
  }
  zAssert( zFrame2DXform, result );
}

int main(void)
{
  zRandInit();
  assert_frame_misc();
  assert_xform();
  assert_frame_inv();
  assert_frame_cascade();
  assert_frame_xform();
  return 0;
}

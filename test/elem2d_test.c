#include <zeo/zeo_elem2d.h>

#define N 10000

void create_tri_rand(zVec2D v[], zTri2D *t, double min, double max)
{
  zVec2DCreate( &v[0], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[1], zRandF(min,max), zRandF(min,max) );
  zVec2DCreate( &v[2], zRandF(min,max), zRandF(min,max) );
  zTri2DCreate( t, &v[0], &v[1], &v[2] );
}

#define TOL (1.0e-5)

void assert_tri_center(void)
{
  zVec2D v[3], c, h[2], tmp;
  zTri2D t;
  zEdge2D e;
  register int i, j;
  double r;
  bool ret;

  /* barycenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri_rand( v, &t, -5, 5 );
    zTri2DBarycenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec2DSub( &c, &v[j], &tmp );
      zVec2DMulDRC( &tmp, 1.5 );
      zVec2DSub( &v[(j+1)%3], &v[j], &h[0] );
      zVec2DSubDRC( &h[0], &tmp );
      zVec2DSub( &v[(j+2)%3], &v[j], &h[1] );
      zVec2DSubDRC( &h[1], &tmp );
      if( !zIsTol( zVec2DOuterProd( &h[0], &h[1] ), TOL ) ) ret = false;
    }
  }
  zAssert( zTri2DBarycenter, ret );
  /* circumcenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri_rand( v, &t, -5, 5 );
    zTri2DCircumcenter( &t, &c );
    r = zVec2DDist( &v[0], &c );
    if( !zIsTol( zVec2DDist( &v[1], &c ) - r, TOL ) || !zIsTol( zVec2DDist( &v[2], &c ) - r, TOL ) ){
      eprintf( "%.10g, %.10g\n", zVec2DDist( &v[1], &c ) - r, zVec2DDist( &v[2], &c ) - r );
      ret = false;
    }
  }
  zAssert( zTri2DCircumcenter, ret );
  /* incenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri_rand( v, &t, -5, 5 );
    zTri2DIncenter( &t, &c );
    r = zEdge2DPointDist( zEdge2DCreate( &e, &v[0], &v[1] ), &c );
    if( !zIsTol( zEdge2DPointDist( zEdge2DCreate( &e, &v[1], &v[2] ), &c ) - r, TOL ) ||
        !zIsTol( zEdge2DPointDist( zEdge2DCreate( &e, &v[2], &v[0] ), &c ) - r, TOL ) )
      ret = false;
  }
  zAssert( zTri2DIncenter, ret );
  /* orthocenter */
  for( ret=true, i=0; i<N; i++ ){
    create_tri_rand( v, &t, -5, 5 );
    zTri2DOrthocenter( &t, &c );
    for( j=0; j<3; j++ ){
      zVec2DSub( &c, &v[j], &tmp );
      zVec2DSub( &v[(j+2)%3], &v[(j+1)%3], &h[0] );
      if( !zIsTol( zVec2DInnerProd( &tmp, &h[0] ), TOL ) ){
        eprintf( "%.10g\n",zVec2DInnerProd( &tmp, &h[0] ) );
        ret = false;
      }
    }
  }
  zAssert( zTri2DOrthocenter, ret );
}

int main(void)
{
  zRandInit();
  assert_tri_center();
  return 0;
}

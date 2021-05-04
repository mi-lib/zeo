#include <zeo/zeo_elem3d.h>

void vec_create_rand(zVec3D *v, double min, double max)
{
  zVec3DCreate( v, zRandF(min,max), zRandF(min,max), zRandF(min,max) );
}

int main(void)
{
  zVec3D v[3], c, h;
  zTri3D t;
  zEdge3D e[3];

  zRandInit();
  vec_create_rand( &v[0], -5, 5 );
  vec_create_rand( &v[1], -5, 5 );
  vec_create_rand( &v[2], -5, 5 );
  zTri3DCreate( &t, &v[0], &v[1], &v[2] );
  zEdge3DCreate( &e[0], &v[1], &v[2] );
  zEdge3DCreate( &e[1], &v[2], &v[0] );
  zEdge3DCreate( &e[2], &v[0], &v[1] );

  printf( "<triangle>\n" );
  zVec3DPrint( &v[0] );
  zVec3DPrint( &v[1] );
  zVec3DPrint( &v[2] );

  /* circumcenter */
  zTri3DCircumcenter( &t, &c );
  printf( "<circumcenter>\n" );
  zVec3DPrint( &c );
  printf( "%f=%f=%f\n", zVec3DDist(&v[0],&c), zVec3DDist(&v[1],&c), zVec3DDist(&v[2],&c) );

  /* incenter */
  zTri3DIncenter( &t, &c );
  printf( "<incenter>\n" );
  zVec3DPrint( &c );
  printf( "%f=%f=%f\n", zEdge3DPointDist(&e[0],&c), zEdge3DPointDist(&e[1],&c), zEdge3DPointDist(&e[2],&c) );

  /* orthocenter */
  zTri3DOrthocenter( &t, &c );
  printf( "<orthocenter>\n" );
  zVec3DPrint( &c );
  zVec3DSub( &v[0], &c, &h );
  printf( "%f\n", zVec3DInnerProd( &h, zEdge3DVec(&e[0]) ) );
  zVec3DSub( &v[1], &c, &h );
  printf( "%f\n", zVec3DInnerProd( &h, zEdge3DVec(&e[1]) ) );
  zVec3DSub( &v[2], &c, &h );
  printf( "%f\n", zVec3DInnerProd( &h, zEdge3DVec(&e[2]) ) );

  return 0;
}

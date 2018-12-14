#include <zeo/zeo_mat3d.h>

double *zVec3DOnTri(zVec3D *v1, zVec3D *v2, zVec3D *v3, zVec3D *v, double ratio[])
{
  zMat3D w, iw;
  zVec3D b;

  zMat3DCreate( &w,
    zVec3DInnerProd(v1,v1)+1, zVec3DInnerProd(v1,v2)+1, zVec3DInnerProd(v1,v3)+1,
    zVec3DInnerProd(v2,v1)+1, zVec3DInnerProd(v2,v2)+1, zVec3DInnerProd(v2,v3)+1,
    zVec3DInnerProd(v3,v1)+1, zVec3DInnerProd(v3,v2)+1, zVec3DInnerProd(v3,v3)+1 );
  zVec3DCreate( &b,
    zVec3DInnerProd(v1,v)+1, zVec3DInnerProd(v2,v)+1, zVec3DInnerProd(v3,v)+1 );
  zMat3DInv( &w, &iw );
  zMulMatVec3D( &iw, &b, (zVec3D*)ratio );
  return ratio;
}

int main(void)
{
  double ratio[3];
  zVec3D u[3], v;

  zRandInit();
  zVec3DCreate( &u[0], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &u[1], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &u[2], zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  printf( "enter three values: " );
  ratio[0] = zFDouble( stdin );
  ratio[1] = zFDouble( stdin );
  ratio[2] = 1 - ratio[0] - ratio[1];
  printf( "entered concatenate ratio: %g %g (%g)\n", ratio[0], ratio[1], ratio[2] );

  zVec3DMul( &u[0], ratio[0], &v );
  zVec3DCatDRC( &v, ratio[1], &u[1] );
  zVec3DCatDRC( &v, ratio[2], &u[2] );

  ratio[0] = ratio[1] = ratio[2] = 0; /* dummy */

  zVec3DOnTri( &u[0], &u[1], &u[2], &v, ratio );
  printf( "computed concatenate ratio: %g %g %g\n", ratio[0], ratio[1], ratio[2] );

  return 0;
}

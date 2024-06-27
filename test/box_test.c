#include <zeo/zeo.h>

void generate_box_rand(zBox3D *box, zVec3D *ax, zVec3D *ay, zVec3D *az)
{
  zVec3D center, tmp;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, ax, ay );
  zVec3DOuterProd( ax, ay, az );
  zVec3DNormalizeDRC( ax );
  zVec3DNormalizeDRC( ay );
  zVec3DNormalizeDRC( az );

  zBox3DCreate( box, &center, ax, ay, az, zRandF(0,5), zRandF(0,5), zRandF(0,5) );
}

void assert_vert(void)
{
  zBox3D box;
  zVec3D ax, ay, az, tmp;
  zVec3D v[8], e[12];
  int i;

  generate_box_rand( &box, &ax, &ay, &az );
  for( i=0; i<8; i++ )
    zBox3DVert( &box, i, &v[i] );
  zVec3DSub( &v[0], &v[1], &e[0] );
  zVec3DSub( &v[3], &v[2], &e[1] );
  zVec3DSub( &v[4], &v[5], &e[2] );
  zVec3DSub( &v[7], &v[6], &e[3] );

  zVec3DSub( &v[1], &v[2], &e[4] );
  zVec3DSub( &v[0], &v[3], &e[5] );
  zVec3DSub( &v[5], &v[6], &e[6] );
  zVec3DSub( &v[4], &v[7], &e[7] );

  zVec3DSub( &v[4], &v[0], &e[8] );
  zVec3DSub( &v[5], &v[1], &e[9] );
  zVec3DSub( &v[6], &v[2], &e[10] );
  zVec3DSub( &v[7], &v[3], &e[11] );

  zAssert( zBox3DVert,
    zIsTiny( zVec3DNorm(&e[0]) - zBox3DDepth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[1]) - zBox3DDepth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[2]) - zBox3DDepth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[3]) - zBox3DDepth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[4]) - zBox3DWidth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[5]) - zBox3DWidth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[6]) - zBox3DWidth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[7]) - zBox3DWidth(&box) ) &&
    zIsTiny( zVec3DNorm(&e[8]) - zBox3DHeight(&box) ) &&
    zIsTiny( zVec3DNorm(&e[9]) - zBox3DHeight(&box) ) &&
    zIsTiny( zVec3DNorm(&e[10]) - zBox3DHeight(&box) ) &&
    zIsTiny( zVec3DNorm(&e[11]) - zBox3DHeight(&box) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[0],&ax,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[1],&ax,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[2],&ax,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[3],&ax,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[4],&ay,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[5],&ay,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[6],&ay,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[7],&ay,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[8],&az,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[9],&az,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[10],&az,&tmp) ) &&
    zVec3DIsTiny( zVec3DOuterProd(&e[11],&az,&tmp) ) );
}

void assert_volume_inertia(void)
{
  zBox3D box;
  zPH3D ph;
  zVec3D ax, ay, az;
  zMat3D i, iph;

  generate_box_rand( &box, &ax, &ay, &az );
  zBox3DToPH( &box, &ph );
  zAssert( zBox3DVolume, zIsTiny( zBox3DVolume( &box ) - zPH3DVolume( &ph ) ) );

  zBox3DBaryInertia( &box, 1, &i );
  zPH3DBaryInertia( &ph, 1, &iph );
  zMat3DSubDRC( &i, &iph );
  zAssert( zBox3DInertia, zMat3DIsTiny( &i ) );
  zPH3DDestroy( &ph );
}

bool box_cat(zBox3D *box, double d, double w, double h, zVec3D *p)
{
  zVec3DCat( zBox3DCenter(box), d*zBox3DDepth(box), zBox3DAxis(box,zX), p );
  zVec3DCatDRC( p, w*zBox3DWidth(box), zBox3DAxis(box,zY) );
  zVec3DCatDRC( p, h*zBox3DHeight(box), zBox3DAxis(box,zZ) );
  return fabs(d) < 0.5 && fabs(w) < 0.5 && fabs(h) < 0.5;
}

void assert_inside(void)
{
  zBox3D box;
  zVec3D ax, ay, az, p;
  double d, w, h;
  int i;
  int n = 1000, ni, no, nitest, notest;

  generate_box_rand( &box, &ax, &ay, &az );
  for( ni=no=nitest=notest=0, i=0; i<n; i++ ){
    d = zRandF(-1.0,1.0);
    w = zRandF(-1.0,1.0);
    h = zRandF(-1.0,1.0);
    box_cat( &box, d, w, h, &p ) ? ni++ : no++;
    zBox3DPointIsInside( &box, &p, zTOL ) ? nitest++ : notest++;
  }
  zAssert( zBox3DPointIsInside, nitest == ni && notest == no );
}

int main(void)
{
  zRandInit();
  assert_vert();
  assert_volume_inertia();
  assert_inside();
  return EXIT_SUCCESS;
}

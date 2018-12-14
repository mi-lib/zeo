#include <zeo/zeo.h>

#define DT 0.001
#define STEP 1000

int main(void)
{
  register int i;
  zEP ep0, ep1, epvel;
  zVec3D w, dw, we, err;
  zMat3D m0, m1;

  zRandInit();
  zMat3DIdent( &m0 );
  for( i=0; i<STEP; i++ ){
    zMat3DToEP( &m0, &ep0 );
    zVec3DCreate( &w, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
    zVec3DMul( &w, DT, &dw ); /* dw = w * DT */
    /* m1=R(dw).m0 -> ep1 -> d_ep=(ep1-ep0)/dt -> we */
    zMat3DRot( &m0, &dw, &m1 );
    zMat3DToEP( &m1, &ep1 );
    zEPDif( &ep0, &ep1, DT, &epvel );
    zEPVel2AngVel( &epvel, &ep0, &we );
    /* compare */
    zVec3DSub( &we, &w, &err );
    printf( "%.16f %.16f %.16f %.16f %.16f %.16f %.16f %.16f %.16f\n",
      w.e[0], w.e[1], w.e[2], we.e[0], we.e[1], we.e[2],
      err.e[0], err.e[1], err.e[2] );
    /* update */
    zMat3DCopy( &m1, &m0 );
  }
  return 0;
}

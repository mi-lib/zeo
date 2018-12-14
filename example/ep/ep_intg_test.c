#include <zeo/zeo.h>

#define DT 0.001
#define STEP 1000

int main(void)
{
  register int i;
  zEP e, e1, e2, epvel;
  zVec3D a, av, a2, err1, err2, err3;
  zMat3D m0, m1, m2, m3, m;

  zRandInit();
  zMat3DIdent( &m0 );
  for( i=0; i<STEP; i++ ){
    zMat3DToEP( &m0, &e );
    zMat3DToAA( &m0, &a );
    /* w*DT */
    zVec3DCreate( &av, zRandF(-1.0,1.0), zRandF(-1.0,1.0), zRandF(-1.0,1.0) );
    zVec3DMulDRC( &av, DT );
    /* e1: e + de*DT & normalize */
    zAngVel2EPVel( &av, &e, &epvel );
    zEPCat( &e, 1, &epvel, &e1 );
    zMat3DEP( &m1, &e1 );
    /* e2: e (x) de*DT */
    zAA2EP( &av, &epvel );
    zEPCascade( &e, &epvel, &e2 );
    zMat3DEP( &m2, &e2 );
    /* a2: a (x) w*DT */
    zAACascade( &a, &av, &a2 );
    zMat3DAA( &m3, &a2 );
    /* m=R(w*DT).m0 */
    zMat3DRot( &m0, &av, &m );
    /* error */
    zMat3DError( &m, &m1, &err1 );
    zMat3DError( &m, &m2, &err2 );
    zMat3DError( &m, &m3, &err3 );
    /* compare */
    printf( "%.16f %.16f %.16f\n", zVec3DNorm(&err1), zVec3DNorm(&err2), zVec3DNorm(&err3) );
    /* udpate */
    zMat3DCopy( &m, &m0 );
  }
  return 0;
}

#include <zeo/zeo_ep.h>

int main(void)
{
  zEP ep, epc;
  zMat3D m;

  zEPIdent( &ep );
  printf( "identity transformation ? - %s\n", zBoolExpr(zEPIsIdent(&ep)) );
  zMat3DEP( &m, &ep );
  zEPCopy( &ep, &epc );
  zEPWrite( &ep );
  zEPWrite( &epc );
  zMat3DWrite( &m );
  return 0;
}

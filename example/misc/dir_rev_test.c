#include <zeo/zeo_misc.h>

int main(void)
{
  zDir dir;

  for( dir=zNONE; dir<=zDOWN; dir++ )
    printf( "%s <-> %s\n", zDirExpr(dir), zDirExpr( zDirRev(dir) ) );
  return 0;
}

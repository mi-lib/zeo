#include <zeo/zeo_misc.h>

int main(void)
{
  zDir dir;

  for( dir=ZEO_DIR_NONE; dir<=ZEO_DIR_CCW; dir++ )
    printf( "%s <-> %s\n", zDirStr(dir), zDirStr( zDirRev(dir) ) );
  return 0;
}

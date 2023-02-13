#include <zeo/zeo.h>

void assert_dir(void)
{
  zAssert( zDirRev,
    zDirRev(zNONE) == zNONE &&
    zDirRev(zRIGHT) == zLEFT &&
    zDirRev(zLEFT) == zRIGHT &&
    zDirRev(zFORWARD) == zBACKWARD &&
    zDirRev(zBACKWARD) == zFORWARD &&
    zDirRev(zUP) == zDOWN &&
    zDirRev(zDOWN) == zUP &&
    zDirRev(zCW) == zCCW &&
    zDirRev(zCCW) == zCW );
}

int main(void)
{
  assert_dir();
  return EXIT_SUCCESS;
}

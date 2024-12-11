#include <zeo/zeo.h>

void assert_3denum(void)
{
  zAssert( zAxis, zX == 0 && zY == 1 && zZ == 2 && zXA == 3 && zYA == 4 && zZA == 5 );
}

void assert_dir(void)
{
  zAssert( zDirRev,
    zDirRev(ZEO_DIR_NONE) == ZEO_DIR_NONE &&
    zDirRev(ZEO_DIR_RIGHT) == ZEO_DIR_LEFT &&
    zDirRev(ZEO_DIR_LEFT) == ZEO_DIR_RIGHT &&
    zDirRev(ZEO_DIR_FORWARD) == ZEO_DIR_BACKWARD &&
    zDirRev(ZEO_DIR_BACKWARD) == ZEO_DIR_FORWARD &&
    zDirRev(ZEO_DIR_UP) == ZEO_DIR_DOWN &&
    zDirRev(ZEO_DIR_DOWN) == ZEO_DIR_UP &&
    zDirRev(ZEO_DIR_CW) == ZEO_DIR_CCW &&
    zDirRev(ZEO_DIR_CCW) == ZEO_DIR_CW );
}

int main(void)
{
  assert_3denum();
  assert_dir();
  return EXIT_SUCCESS;
}

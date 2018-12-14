/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_misc - miscellanies.
 */

#include <zeo/zeo_misc.h>

static char *__zaxisname[] = {
  "x", "y", "z", "tilt", "elev", "azim", NULL };

/* zAxisExpr
 * - expression of the name of axis.
 */
char *zAxisExpr(zAxis axis)
{
  return __zaxisname[zLimit(axis,zX,zZA)];
}

/* zAxisByStr
 * - axis identification from string.
 */
zAxis zAxisByStr(char str[])
{
  char **jp;
  zAxis axis;

  for( axis=zX, jp=__zaxisname; *jp; jp++, axis++ )
    if( !strcmp( str, *jp ) ) return axis;
  return -1; /* invalid string */
}

/* zDirExpr
 * - expression of the name of direction.
 */
char *zDirExpr(zDir dir)
{
  char *__zdirname[] = {
    "none", "right", "left", "forward", "backward", "up", "down", "(invalid)",
  };
  return __zdirname[zLimit(dir,zNONE,zDOWN+1)];
}

/* zDirRev
 * - reverse direction.
 */
zDir zDirRev(zDir dir)
{
  return dir == zNONE ? zNONE : ( 1 + dir - (dir+1)%2 *2 );
}

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_misc - miscellanies.
 */

#include <zeo/zeo_misc.h>

static const char *__zaxisname[] = { "x", "y", "z", "tilt", "elev", "azim", NULL };

/* string for the name of axis. */
char *zAxisStr(zAxis axis)
{
  return (char *)__zaxisname[_zLimit(axis,zX,zZA)];
}

/* identify axis from a string. */
zAxis zAxisFromStr(char str[])
{
  char **jp;
  zAxis axis;

  for( axis=zX, jp=(char **)__zaxisname; *jp; jp++, axis++ )
    if( strcmp( str, *jp ) == 0 ) return axis;
  return zAxisInvalid; /* invalid string */
}

/* string for the name of direction. */
char *zDirStr(zDir dir)
{
  const char *__zdirname[] = {
    "none", "right", "left", "forward", "backward", "up", "down", "(invalid)",
  };
  return (char *)__zdirname[_zLimit(dir,zNONE,zDOWN+1)];
}

/* reverse direction. */
zDir zDirRev(zDir dir)
{
  return dir == zNONE ? zNONE : ( 1 + dir - (dir+1)%2 *2 );
}

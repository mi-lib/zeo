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
  if( axis < zX || axis > zZA ){
    ZRUNERROR( ZEO_ERR_AXIS_INVALID, axis );
    return NULL;
  }
  return (char *)__zaxisname[axis];
}

/* identify axis from a string. */
zAxis zAxisFromStr(char str[])
{
  char **jp;
  zAxis axis;

  for( axis=zX, jp=(char **)__zaxisname; *jp; jp++, axis++ )
    if( strcmp( str, *jp ) == 0 ) return axis;
  ZRUNERROR( ZEO_ERR_AXIS_INVNAME, str );
  return zAxisInvalid; /* invalid string */
}

/* string for the name of direction. */
char *zDirStr(zDir dir)
{
  const char *__zdirname[] = {
    "none", "right", "left", "forward", "backward", "up", "down", NULL,
  };
  if( dir < zNONE || dir > zDOWN ){
    ZRUNERROR( ZEO_ERR_DIR_INV, dir );
    return NULL;
  }
  return (char *)__zdirname[dir];
}

/* reverse direction. */
zDir zDirRev(zDir dir)
{
  return dir == zNONE ? zNONE : ( 1 + dir - (dir+1)%2 *2 );
}

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
zAxis zAxisFromStr(char *str)
{
  char **jp;
  zAxis axis;

  if( !str ) return zAxisInvalid;
  for( axis=zX, jp=(char **)__zaxisname; *jp; jp++, axis++ )
    if( strcmp( str, *jp ) == 0 ) return axis;
  ZRUNERROR( ZEO_ERR_AXIS_INVNAME, str );
  return zAxisInvalid; /* invalid string */
}

static const char *__zdirname[] = {
  "none", "right", "left", "forward", "backward", "up", "down", "cw", "ccw", NULL,
};

/* string for the name of direction. */
char *zDirStr(zDir dir)
{
  if( dir < ZEO_DIR_NONE || dir > ZEO_DIR_CCW ){
    ZRUNERROR( ZEO_ERR_DIR_INVALID, dir );
    return NULL;
  }
  return (char *)__zdirname[dir];
}

/* identify direction from a string. */
zDir zDirFromStr(char *str)
{
  char **jp;
  zDir dir;

  if( !str ) return ZEO_DIR_NONE;
  for( dir=ZEO_DIR_NONE, jp=(char **)__zdirname; *jp; jp++, dir++ )
    if( strcmp( str, *jp ) == 0 ) return dir;
  ZRUNERROR( ZEO_ERR_DIR_INVNAME, str );
  return ZEO_DIR_NONE;
}

/* reverse direction. */
zDir zDirRev(zDir dir)
{
  return dir == ZEO_DIR_NONE ? ZEO_DIR_NONE : ( 1 + dir - (dir+1)%2 *2 );
}

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_misc - miscellanies.
 */

#ifndef __ZEO_MISC_H__
#define __ZEO_MISC_H__

#include <zm/zm.h>

#include <zeo/zeo_export.h>
#include <zeo/zeo_errmsg.h>

__BEGIN_DECLS

/*! \brief axis identifiers */
typedef byte zAxis;
enum{
  zAxisInvalid=-1, zX=0, zY, zZ, zXA, zYA, zZA,
};

/*! \brief convert a string to an axis.
 *
 * zAxisStr() returns a string for the name of \a axis.
 * zAxisFromStr() returns an axis identifier for a string \a str.
 * The correspondence between strings and axis identifiers are as follows:
 *  zX   <-> "x"
 *  zY   <-> "y"
 *  zZ   <-> "z"
 *  zXA  <-> "tilt"
 *  zYA  <-> "elev"
 *  zZA  <-> "azim"
 * \return
 * zAxisStr() returns a pointer to the string which expresses
 * the name of \a axis.
 * zAxisFromStr() returns the corresponding axis identifier.
 */
__ZEO_EXPORT char *zAxisStr(zAxis axis);
__ZEO_EXPORT zAxis zAxisFromStr(char *str);

/*! \brief direction identifiers */
typedef byte zDir;
enum{
  ZEO_DIR_NONE=0, ZEO_DIR_RIGHT, ZEO_DIR_LEFT, ZEO_DIR_FORWARD, ZEO_DIR_BACKWARD, ZEO_DIR_UP, ZEO_DIR_DOWN, ZEO_DIR_CW, ZEO_DIR_CCW,
};

/*! \brief string for the name of direction.
 *
 * zDirStr() returns a string for the name of a direction identifier \a dir.
 * zDirFromStr() returns a direction identifier for a string \a str.
 * The correspondence between strings and direction identifiers are as follows:
 *  ZEO_DIR_NONE     <-> "none"
 *  ZEO_DIR_RIGHT    <-> "right"
 *  ZEO_DIR_LEFT     <-> "left"
 *  ZEO_DIR_FORWARD  <-> "forward"
 *  ZEO_DIR_BACKWARD <-> "backward"
 *  ZEO_DIR_UP       <-> "up"
 *  ZEO_DIR_DOWN     <-> "down"
 *  ZEO_DIR_CW       <-> "cw"
 *  ZEO_DIR_CCW      <-> "ccw"
 * \return
 * zDirStr() returns a pointer to the string which expresses the name of \a dir.
 * zDirFromStr() returns the corresponding direction identifier.
 */
__ZEO_EXPORT char *zDirStr(zDir dir);
__ZEO_EXPORT zDir zDirFromStr(char *str);

/*! \brief reverse a direction.
 *
 * zDirRev() returns the reverse direction of \a dir, namely:
 * ZEO_DIR_LEFT for ZEO_DIR_RIGHT, ZEO_DIR_RIGHT for ZEO_DIR_LEFT,
 * ZEO_DIR_BACKWARD for ZEO_DIR_FORWARD, ZEO_DIR_FORWARD for ZEO_DIR_BACKWARD,
 * ZEO_DIR_DOWN for ZEO_DIR_UP and ZEO_DIR_UP for ZEO_DIR_DOWN,
 * and ZEO_DIR_CCW for ZEO_DIR_CW and ZEO_DIR_CW for ZEO_DIR_CCW.
 * When \a dir is ZEO_DIR_NONE, ZEO_DIR_NONE is returned.
 */
__ZEO_EXPORT zDir zDirRev(zDir dir);

__END_DECLS

#endif /* __ZEO_MISC_H__ */

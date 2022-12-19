/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_misc - miscellanies.
 */

#ifndef __ZEO_MISC_H__
#define __ZEO_MISC_H__

#include <zm/zm.h>

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
__EXPORT char *zAxisStr(zAxis axis);
__EXPORT zAxis zAxisFromStr(char str[]);

/*! \brief direction identifiers */
typedef byte zDir;
enum{
  zNONE=0, zRIGHT, zLEFT, zFORWARD, zBACKWARD, zUP, zDOWN
};

/*! \brief string for the name of direction.
 *
 * zDirStr() returns a string for the name of a direction identifier
 * \a dir. The correspondence between strings and direction identifiers
 * are as follows:
 *  zNONE     -> "none"
 *  zRIGHT    -> "right"
 *  zLEFT     -> "left"
 *  zFORWARD  -> "forward"
 *  zBACKWARD -> "backward"
 *  zUP       -> "up"
 *  zDOWN     -> "down"
 * \return
 * zDirStr() returns a pointer to the string which expresses the name
 * of \a dir.
 */
__EXPORT char *zDirStr(zDir dir);

/*! \brief reverse a direction.
 *
 * zDirRev() returns the reverse direction of \a dir, namely:
 * zLEFT for zRIGHT, zRIGHT for zLEFT, zBACKWARD for zFORWARD,
 * zFORWARD for zBACKWARD, zDOWN for zUP and zUP for zDOWN.
 * When \a dir is zNONE, zNONE is returned.
 */
__EXPORT zDir zDirRev(zDir dir);

__END_DECLS

#endif /* __ZEO_MISC_H__ */

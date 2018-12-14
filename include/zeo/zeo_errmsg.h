/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_errmsg - error messages.
 */

#ifndef __ZEO_ERRMSG_H__
#define __ZEO_ERRMSG_H__

/* NOTE: never include this header file in user programs. */

#define ZEO_ERR_ZERODIV      "cannot divid by zero value"
#define ZEO_ERR_ZERONORM     "cannot normalize zero vector"
#define ZEO_ERR_SINGULARMAT  "matrix is singular"

#define ZEO_ERR_RGB          "invalid size of RGB specification, strlen(%s)=%d"

#define ZEO_ERR_ELEM_DEGP    "too small normal vector to define a plane"
#define ZEO_ERR_ELEM_DEGE    "edge degenerated"
#define ZEO_ERR_ELEM_DEGT    "triangle degenerated"

#define ZEO_ERR_NOFACE       "polyhedron has no face"
#define ZEO_ERR_CENTER_MANY  "too many center points"
#define ZEO_ERR_RADIUS_MANY  "too many radii"

#define ZEO_ERR_LONGNAME     "too long name, truncated to %s"

#define ZEO_ERR_OPT_MANY     "too many optical-infos specified"
#define ZEO_ERR_OPT_INV      "invalid description for optical-info"
#define ZEO_ERR_OPT_UNNAME   "unnamed optical-info exists"

#define ZEO_ERR_PRIM_ZEROC   "zero-distance centers specified"

#define ZEO_ERR_SHAPE_MANY   "too many shapes specified"
#define ZEO_ERR_SHAPE_INV    "invalid description for the shape"
#define ZEO_ERR_SHAPE_UNNAME "unnamed shape exists"
#define ZEO_ERR_SHAPE_UNDEF  "undefined shape %s referred"
#define ZEO_ERR_SHAPE_INVBB  "unknown bounding box type specified"

#define ZEO_ERR_EMPTYSET     "empty set assigned"

#define ZEO_ERR_CH_DEG1      "point set degenerated into a single point"
#define ZEO_ERR_CH_DEG2      "point set degenerated onto a single line"
#define ZEO_ERR_CH_DEG3      "point set degenerated onto a single plane"

#define ZEO_ERR_BREP_CONV    "cannot convert polyhedron to B-Rep solid"

#define ZEO_ERR_TERRA_INVSIZ "grid size unspecified"
#define ZEO_ERR_TERRA_INVRSL "too fine (or negative) grid resolution"
#define ZEO_ERR_TERRA_OOREG   "out of region (%g,%g): cannot estimate ground height"
#define ZEO_ERR_TERRA_OORAN  "grid out of range"

#define ZEO_ERR_FATAL        "fatal error! - please report to the author"

#endif /* __ZEO_ERRMSG_H__ */

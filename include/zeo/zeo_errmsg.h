/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_errmsg - error messages.
 */

#ifndef __ZEO_ERRMSG_H__
#define __ZEO_ERRMSG_H__

/* NOTE: never include this header file in user programs. */

/* error messages */

#define ZEO_ERR_ZERODIV      "cannot divid by zero value"
#define ZEO_ERR_ZERONORM     "cannot normalize zero vector"
#define ZEO_ERR_SINGULARMAT  "matrix is singular"
#define ZEO_ERR_NONSO3       "matrix is not orthonormal"

#define ZEO_ERR_RGB          "invalid size of RGB specification, strlen(%s)=%d"

#define ZEO_ERR_INVINDEX     "invalid index specified"

#define ZEO_ERR_ELEM_DEGP    "too small normal vector to define a plane"
#define ZEO_ERR_ELEM_DEGE    "edge degenerated"
#define ZEO_ERR_ELEM_DEGT    "triangle degenerated"

#define ZEO_ERR_NOFACE       "polyhedron has no face"
#define ZEO_ERR_CENTER_MANY  "too many center points"
#define ZEO_ERR_RADIUS_MANY  "too many radii"

#define ZEO_ERR_PH_INVALID_VERT_ID "%d: invalid identifier of a vertex specified."

#define ZEO_ERR_LONGNAME     "too long name, truncated to %s"

#define ZEO_ERR_OPT_MANY     "too many optical-infos specified"
#define ZEO_ERR_OPT_INV      "invalid description for optical-info"
#define ZEO_ERR_OPT_UNNAME   "unnamed optical-info exists"

#define ZEO_ERR_STL_INCOMPLETE "incomplete STL file"

#define ZEO_ERR_NURBS_INVDIM "invalid dimension specified for NURBS, or lack of control points"
#define ZEO_ERR_NURBS_SIZMIS      "size mismatch of NURBS surfaces"
#define ZEO_ERR_NURBS_KNOTALREADY "knot already allocated"
#define ZEO_ERR_NURBS_CPALREADY   "control point already allocated"
#define ZEO_ERR_NURBS_INVCP       "invalid index of control point specified"

#define ZEO_ERR_SHAPE_INVALID "invalid shape type assigned."
#define ZEO_ERR_SHAPE_ZEROC   "zero-distance centers specified"
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

/* warning messages */

#define ZEO_WARN_PH_EMPTY         "empty set of vertices assigned for a polyhedron."
#define ZEO_WARN_PH_VERT_UNMATCH  "%d: unmatched identifier of a vertex"

#define ZEO_WARN_STL_MIS_LOOP     "missing the initiator of outer loop"
#define ZEO_WARN_STL_TOOMANYVERT  "too many vertices in an outer loop"
#define ZEO_WARN_STL_TOOFEWVERT   "too few vertices in an outer loop"
#define ZEO_WARN_STL_WRONGNORMAL  "inconsistent normal vector, ignored"

#define ZEO_WARN_STL_MISSINGDATA  "missing data in STL"

#define ZEO_WARN_SHAPE_DUP        "%s: name of a shape duplicated."
#define ZEO_WARN_SHAPE_BB_INVALID "bounding-box not generated because it is only for polyhedra."
#define ZEO_WARN_SHAPE_UNKNOWNOPTIC "%s: unknown name of optical info set."

#define ZEO_WARN_MSHAPE_EMPTY     "empty shapes assigned."

#endif /* __ZEO_ERRMSG_H__ */

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

#define ZEO_ERR_AXIS_INVALID "invalid axis identifier : %d"
#define ZEO_ERR_AXIS_INVNAME "invalid axis name : %s"
#define ZEO_ERR_DIR_INVALID  "invalid direction identifier : %d"
#define ZEO_ERR_DIR_INVNAME  "invalid direction name : %s"

#define ZEO_ERR_RGB          "invalid size of RGB specification, strlen(%s)=%d"

#define ZEO_ERR_TEXTURE_INVALID_COORD_ID "%d: invalid identifier of a coordinate specified."

#define ZEO_ERR_INVINDEX     "invalid index specified"

#define ZEO_ERR_ELEM_DEG_PLANE   "too small normal vector to define a plane"
#define ZEO_ERR_ELEM_DEG_EDGE    "edge degenerated"
#define ZEO_ERR_ELEM_DEG_TRI     "triangle degenerated"

#define ZEO_ERR_ELEM_NPTR_SPECIFIED(XD) "null pointer specified for position / direction vector of a " #XD " line"
#define ZEO_ERR_ELEM_ZERO_DIRVEC(XD)    "cannot create a " #XD " line from a zero direction vector"

#define ZEO_WARN_LINE3D_NOT_UNIQUE_COMMONPERP "cannot find a unique common parpendicular of parallel 3D lines, choose a nominal one"

#define ZEO_ERR_NOFACE       "polyhedron has no face"
#define ZEO_ERR_CENTER_MANY  "too many center points"
#define ZEO_ERR_RADIUS_MANY  "too many radii"

#define ZEO_ERR_PH_INVALID_VERT_ID "%d: invalid identifier of a vertex specified."
#define ZEO_ERR_PH_INV_FACE    "found %d invalid face keys"
#define ZEO_ERR_PH_LOOP_INVALID "invalid format for loop"
#define ZEO_ERR_PH_ARC_INV_RADIUS "too small radius for an arc: %g"

#define ZEO_ERR_LONGNAME     "too long name, truncated to %s"

#define ZEO_ERR_OPT_MANY     "too many optical-infos specified"
#define ZEO_ERR_OPT_INV      "invalid description for optical-info"
#define ZEO_ERR_OPT_UNNAME   "unnamed optical-info exists"

#define ZEO_ERR_PCD_INVALID_VERSION       "invalid string for version: %s"
#define ZEO_ERR_PCD_UNKNOWN_FIELDID       "unknown field identifier: %s"
#define ZEO_ERR_PCD_MISMATCH_NUMOFFIELDS  "mismatch number of fields: %d"
#define ZEO_ERR_PCD_INVALID_FIELDSIZE     "invalid field size: %d"
#define ZEO_ERR_PCD_UNKNOWN_FIELDTYPE     "unknown field type identifier: %s"
#define ZEO_ERR_PCD_INVALID_COUNT         "can only handle 3D point cloud"
#define ZEO_ERR_PCD_UNSPEC_WIDTH          "width not specified"
#define ZEO_ERR_PCD_UNSPEC_HEIGHT         "height not specified"
#define ZEO_ERR_PCD_INVALID_VIEWPOINT     "invalid specification of viewpoint"
#define ZEO_ERR_PCD_UNSPEC_NUMOFPOINTS    "number of points not specified"
#define ZEO_ERR_PCD_INVALID_NUMOFPOINTS   "inconsistent number of points: %d VS %d x %d"
#define ZEO_ERR_PCD_UNSPEC_DATATYPE       "data type not specified"
#define ZEO_ERR_PCD_UNKNOWN_DATATYPE      "unknown data type identifier: %s"
#define ZEO_ERR_PCD_INVALID_HEADER        "invalid PCD file header"
#define ZEO_ERR_PCD_INVALID_DATATYPE      "invalid data type"
#define ZEO_ERR_PCD_INVALID_FORMAT        "unknown point clound format %s"

#define ZEO_ERR_STL_UNREADABLE "File unreadable. Probably not a STL file."
#define ZEO_ERR_STL_INCOMPLETE "incomplete STL file"

#define ZEO_ERR_PLY_UNREADABLE  "File unreadable. Probably not a PLY file"
#define ZEO_ERR_PLY_INCOMPLETE  "incomplete PLY file"
#define ZEO_ERR_PLY_INV_FORMAT  "invalid format %s"
#define ZEO_ERR_PLY_TOOMANYELEM "too many definitions of elements"
#define ZEO_ERR_PLY_TOOMANYPRP  "too many definitions of properties"
#define ZEO_ERR_PLY_UNKNOWNELEM "unknown element: %s"
#define ZEO_ERR_PLY_UNKNOWNPRP  "unknown property: %s"
#define ZEO_ERR_PLY_UNSUPPORTED "unsupported description"

#define ZEO_ERR_DAE_UNSUPPORTED         "DAE file not supported"
#define ZEO_ERR_DAE_ACCESSOR_UNASSIGNED "accessor unassigned to source %s"
#define ZEO_ERR_DAE_ARRSIZMIS           "inconsistent size of array %d/%d"      
#define ZEO_ERR_DAE_STRIDEMIS           "inconsistent stride %d/%d"
#define ZEO_ERR_DAE_URI_EMPTY           "empty URI specified"
#define ZEO_ERR_DAE_URI_INVALID         "invalid URI: %s"
#define ZEO_ERR_DAE_UNKNOWN_SRC         "unknown source %s"
#define ZEO_ERR_DAE_INVALID_SRC         "invalid type of source"
#define ZEO_ERR_DAE_VERT_UNASSIGNED     "vertices unassigned"
#define ZEO_ERR_DAE_IDENTMIS            "identifiers mismatch %s / %s"
#define ZEO_ERR_DAE_EMPTYNODE           "empty node specified"
#define ZEO_ERR_DAE_SCENE_UNDEF         "visual_scene undefined"
#define ZEO_ERR_DAE_NOT_COLLADA         "This is not a COLLADA file"
#define ZEO_ERR_DAE_NO_GEOMETRY         "no geometries contained"
#define ZEO_ERR_DAE_PRIM_UNSUPPORTED    "non-triangle face, not supported"
#define ZEO_ERR_DAE_PRIM_INVALID        "invalid DAE file %s"
#define ZEO_ERR_DAE_FAILCONV            "failed to create a polyhedron from a DAE file %s"

#define ZEO_ERR_NURBS_INVORDER    "invalid order specified for NURBS, or lack of control points"
#define ZEO_ERR_NURBS_SIZMIS      "size mismatch of NURBS surfaces"
#define ZEO_ERR_NURBS_KNOTALREADY "knot already allocated"
#define ZEO_ERR_NURBS_CPALREADY   "control point already allocated"
#define ZEO_ERR_NURBS_INVCP       "invalid index of control point specified"

#define ZEO_ERR_SHAPE_INVALID         "invalid shape type assigned."
#define ZEO_ERR_SHAPE_ZEROC           "zero-distance centers specified"
#define ZEO_ERR_SHAPE_INV             "invalid description for the shape"
#define ZEO_ERR_SHAPE_UNNAME          "unnamed shape exists"
#define ZEO_ERR_SHAPE_UNDEF           "undefined shape %s referred"
#define ZEO_ERR_SHAPE_MIRROR_INV_AXIS "cannot mirror about axis: %s"

#define ZEO_ERR_EMPTYSET     "empty set assigned"

#define ZEO_ERR_CH_DEG1      "point set degenerated into a single point"
#define ZEO_ERR_CH_DEG2      "point set degenerated onto a single line"
#define ZEO_ERR_CH_DEG3      "point set degenerated onto a single plane"

#define ZEO_ERR_BREP_CONV    "cannot convert polyhedron to B-Rep solid"

#define ZEO_ERR_COLCHK_LINE_PARALLEL  "lines are parallel"
#define ZEO_ERR_COLCHK_PLANE_IDENT    "planes are identical"
#define ZEO_ERR_COLCHK_PLANE_PARALLEL "planes are parallel"

#define ZEO_ERR_TERRA_INVSIZ "grid size unspecified"
#define ZEO_ERR_TERRA_INVRSL "too fine (or negative) grid resolution"
#define ZEO_ERR_TERRA_OOREG  "out of region (%g,%g): cannot estimate ground height"
#define ZEO_ERR_TERRA_OORAN  "grid out of range"

#define ZEO_ERR_MAP_UNSPEC   "map type unspecified."

#define ZEO_ERR_FATAL        "fatal error! - please report to the author"

/* warning messages */

#define ZEO_WARN_TEXTURE_COORD_UNMATCH "%d: unmatched identifier of a coordinate"
#define ZEO_WARN_TEXTURE_UNKNOWN_TYPE  "unknown texture type: %s"
#define ZEO_WARN_TEXTURE_EMPTY         "empty set of coordinates assigned for a texture."
#define ZEO_WARN_TEXTURE_READFUNCTION_NOT_ASSIGNED "function to read texture not assigned"

#define ZEO_WARN_PCD_LACKOFDATA           "short of data"

#define ZEO_WARN_PH_VERT_UNMATCH  "%d: unmatched identifier of a vertex"
#define ZEO_WARN_PH_DUPDEF        "duplicate definition of polyhedron, ignored"

#define ZEO_WARN_STL_MIS_LOOP     "missing the initiator of outer loop"
#define ZEO_WARN_STL_TOOMANYVERT  "too many vertices in an outer loop"
#define ZEO_WARN_STL_TOOFEWVERT   "too few vertices in an outer loop"

#define ZEO_WARN_STL_MISSINGDATA  "missing data in STL"

#define ZEO_WARN_SHAPE_DUP        "%s: name of a shape duplicated."
#define ZEO_WARN_SHAPE_BB_INVALID "bounding-box not generated because it is only for polyhedra."
#define ZEO_WARN_SHAPE_UNKNOWNOPTIC "%s: unknown name of optical info set."
#define ZEO_WARN_SHAPE_UNKNOWNTEXTURE "%s: unknown name of a texture data."
#define ZEO_WARN_SHAPE_OVRRDN_PH "shape type overridden by polyhedron"
#define ZEO_WARN_SHAPE_UNKNOWNFORMAT "%s: unknown format of a shape."
#define ZEO_WARN_SHAPE_MIRROR_INV_TYPE "cannot specify type for mirrored shape : %s. destroyed"

#define ZEO_WARN_MSHAPE_EMPTY     "empty shapes assigned."

#define ZEO_WARN_MAPNET_EMPTY     "empty map net assigned."

#endif /* __ZEO_ERRMSG_H__ */

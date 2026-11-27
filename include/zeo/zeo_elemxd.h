/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elemxd - 2D/3D shape element.
 */

#ifndef __ZEO_ELEMXD_H__
#define __ZEO_ELEMXD_H__

/* NOTE: never include this header file in user programs. */

/* ********************************************************** */
/* commonized definition of 2D/3D line class.
 * ********************************************************** */
#define ZEO_ELEM_LINEXD_DEF_STRUCT(XD) \
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zLine##XD ){ \
  zVec##XD *origin; \
  zVec##XD direction; \
}

#define zLineXDOrg(XD,line)       (line)->origin
#define zLineXDDir(XD,line)       ( &(line)->direction )
#define zLineXDSetOrg(XD,line,p)  ( zLine##XD##Org(line) = (p) )
#define zLineXDSetDir(XD,line,d)  zVec##XD##Copy( d, zLine##XD##Dir(line) )

/* initilize a 2D/3D line. */
#define ZEO_ELEM_LINEXD_INIT_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##Init(zLine##XD *line)
#define ZEO_ELEM_LINEXD_INIT(XD) \
  ZEO_ELEM_LINEXD_INIT_PROTOTYPE( XD ){ \
    zLine##XD##SetOrg( line, ZVEC##XD##ZERO ); \
    zLine##XD##SetDir( line, ZVEC##XD##X ); \
    return line; \
  }

/* create a 2D/3D line. */
#define ZEO_ELEM_LINEXD_CREATE_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##Create(zLine##XD *line, const zVec##XD *p, const zVec##XD *direction)
#define ZEO_ELEM_LINEXD_CREATE(XD) \
  ZEO_ELEM_LINEXD_CREATE_PROTOTYPE( XD ){ \
    if( !p || !direction ){ \
      ZRUNERROR( ZEO_ERR_ELEM_NPTR_SPECIFIED( XD ) ); \
      return NULL; \
    } \
    if( zVec##XD##IsTiny( direction ) ){ \
      ZRUNERROR( ZEO_ERR_ELEM_NPTR_SPECIFIED( XD ) ); \
      return NULL; \
    } \
    zLine##XD##SetOrg( line, (zVec##XD*)p ); \
    zVec##XD##NormalizeNC( direction, zLine##XD##Dir(line) ); \
    return line; \
  }

/* create a 2D/3D line from that passes given two points. */
#define ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##CreateTwoPoints(zLine##XD *line, const zVec##XD *p1, const zVec##XD *p2)
#define ZEO_ELEM_LINEXD_CREATE_TWOPOINT(XD) \
  ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE( XD ){ \
    zLine##XD##SetOrg( line, (zVec##XD*)p1 ); \
    zVec##XD##Sub( p2, p1, zLine##XD##Dir(line) ); \
    zVec##XD##NormalizeDRC( zLine##XD##Dir(line), 0 ); \
    return line; \
  }

/* find a point on a 2D/3D line. */
#define ZEO_ELEM_LINEXD_POINT_PROTOTYPE(XD) \
  zVec##XD *zLine##XD##Point(const zLine##XD *line, double magnitude, zVec##XD *p)
#define ZEO_ELEM_LINEXD_POINT(XD) \
  ZEO_ELEM_LINEXD_POINT_PROTOTYPE( XD ){ \
    _zVec##XD##Cat( zLine##XD##Org(line), magnitude, zLine##XD##Dir(line), p ); \
    return p; \
  }

/* project a point onto a 2D/3D line. */
#define ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE(XD) \
  zVec##XD *zLine##XD##ProjPoint(const zLine##XD *line, const zVec##XD *src, zVec##XD *projection)
#define ZEO_ELEM_LINEXD_PROJ_POINT(XD) \
  ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE( XD ){ \
    zVec##XD v; \
    return zLine##XD##Point( line, zVec##XD##InnerProd( zLine##XD##Dir(line), zVec##XD##Sub( src, zLine##XD##Org(line), &v ) ), projection ); \
  }

/* calculate distance from a point to a 2D/3D line. */
#define ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE(XD) \
  double zLine##XD##DistFromPoint(const zLine##XD *line, const zVec##XD *p)
#define ZEO_ELEM_LINEXD_DIST_FROM_POINT(XD) \
  ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE( XD ){ \
    zVec##XD v; \
    zVec##XD##Sub( p, zLine##XD##Org(line), &v ); \
    return zVec##XD##OuterProdNorm( &v, zLine##XD##Dir(line) ); \
  }

/* check if a point is on a 2D/3D line. */
#define ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE(XD) \
  bool zLine##XD##PointIsOn(const zLine##XD *line, const zVec##XD *p, double margin)
#define ZEO_ELEM_LINEXD_POINT_IS_ON(XD) \
  ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE( XD ){ \
    return zLine##XD##DistFromPoint( line, p ) < margin; \
  }

/* ********************************************************** */
/* commonized definition of 2D/3D edge class.
 * ********************************************************** */
#define ZEO_ELEM_EDGEXD_DEF_STRUCT(XD) \
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zEdge##XD ){ \
  zVec##XD *vert[2]; /*!< \brief two vertices */ \
  zVec##XD vec;      /*!< \brief difference vector */ \
}

#define zEdgeXDVert(XD,edge,i)        (edge)->vert[(i)]
#define zEdgeXDVec(XD,edge)           ( &(edge)->vec )

#define zEdgeXDSetVert(XD,edge,i,ptr) ( (edge)->vert[(i)] = (ptr) )
#define zEdgeXDSetVec(XD,edge,vec)    zVec##XD##Copy( vec, zEdge##XD##Vec(edge) )

/* initialize a 2D/3D edge. */
#define ZEO_ELEM_EDGEXD_INIT_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Init(zEdge##XD *edge)
#define ZEO_ELEM_EDGEXD_INIT(XD) \
  ZEO_ELEM_EDGEXD_INIT_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( edge, 0, ZVEC##XD##ZERO ); \
    zEdge##XD##SetVert( edge, 1, ZVEC##XD##ZERO ); \
    zEdge##XD##SetVec( edge, ZVEC##XD##ZERO ); \
    return edge; \
  }

/* create a 2D/3D edge from two endpoints. */
#define ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Create(zEdge##XD *edge, const zVec##XD *v1, const zVec##XD *v2)
#define ZEO_ELEM_EDGEXD_CREATE(XD) \
  ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( edge, 0, (zVec##XD*)v1 ); \
    zEdge##XD##SetVert( edge, 1, (zVec##XD*)v2 ); \
    zEdge##XD##CalcVec( edge ); \
    return edge; \
  }

/* calculate the difference vector of a 2D/3D edge. */
#define ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##CalcVec(zEdge##XD *edge)
#define ZEO_ELEM_EDGEXD_CALC_VEC(XD) \
  ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE( XD ){ \
    return zVec##XD##Sub( zEdge##XD##Vert(edge,1), zEdge##XD##Vert(edge,0), zEdge##XD##Vec(edge) ); \
  }

/* length of a 2D/3D edge. */
#define ZEO_ELEM_EDGEXD_LEN(XD,edge)     zVec##XD##Norm( zEdge##XD##Vec(edge) )

/* find the direction vector of a 2D/3D edge. */
#define ZEO_ELEM_EDGEXD_DIR(XD,edge,dir) zVec##XD##Normalize( zEdge##XD##Vec(edge), 0, dir )

/* TO BE REMOVED. */
/* unit_dir  ... unit direction vector of the edge
 * rel_point ... relative position vector of the point from the 1st vertex of the edge
 * len       ... length of the edge
 */
#define _ZEO_ELEM_EDGEXD_PROJ_SET(XD) \
  static bool _zEdge##XD##ProjSet(const zEdge##XD *edge, const zVec##XD *point, zVec##XD *unit_dir, zVec##XD *rel_point, double *len){ \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,0), rel_point ); \
    *len = ZEO_ELEM_EDGEXD_DIR( XD, edge, unit_dir ); \
    return !zVec##XD##IsTiny( rel_point ); \
  }

/* TO BE REMOVED. */
#define ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##ProjPoint(const zEdge##XD *edge, const zVec##XD *point, zVec##XD *cp)
#define ZEO_ELEM_EDGEXD_PROJ_POINT(XD) \
  ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE( XD ){ \
    zVec##XD unit_dir, rel_point; \
    double len; \
    _zEdge##XD##ProjSet( edge, point, &unit_dir, &rel_point, &len ); \
    return zVec##XD##Cat( zEdge##XD##Vert(edge,0), zVec##XD##InnerProd( &rel_point, &unit_dir ), &unit_dir, cp ); \
  }

/* TO BE REMOVED. */
#define ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE(XD) \
  bool zEdge##XD##PointIsOnLine(const zEdge##XD *edge, const zVec##XD *point, double margin)
#define ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE(XD) \
  ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE( XD ){ \
    zVec##XD unit_dir, rel_point; \
    double len; \
    if( !_zEdge##XD##ProjSet( edge, point, &unit_dir, &rel_point, &len ) ) return true; \
    return zVec##XD##OuterProdNorm( &rel_point, &unit_dir ) < margin; \
  }

/* TO BE REMOVED. */
#define _ZEO_ELEM_EDGEXD_LINSCALE(XD) \
  static double _zEdge##XD##LinScale(const zEdge##XD *edge, const zVec##XD *point, double *l0, double *l1, zVec##XD *cp){ \
    zVec##XD dp0, dp1, v; \
    double d, ret; \
    int ni; \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,0), &dp0 ); \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,1), &dp1 ); \
    ni = ( zVec##XD##SqrNorm(&dp0) <= zVec##XD##SqrNorm(&dp1) ) ? 0 : 1; \
    if( ( d = zVec##XD##Normalize( zEdge##XD##Vec(edge), 0, &v ) ) <= 0 ){ \
      ZRUNERROR( ZEO_ERR_ELEM_DEG_EDGE ); \
      return -1; \
    } \
    if( ni == 0 ){ \
      *l1 = _zVec##XD##InnerProd( &dp0, &v ) / d; \
      *l0 = 1 - *l1; \
    } else{ \
      *l0 =-_zVec##XD##InnerProd( &dp1, &v ) / d; \
      *l1 = 1 - *l0; \
    } \
    if( *l1 < 0 ){ \
      ret = -*l1 * d; \
      *l1 = 0; *l0 = 1; \
      zVec##XD##Copy( zEdge##XD##Vert(edge,0), cp ); \
      return ret; \
    } else \
    if( *l0 < 0 ){ \
      ret = -*l0 * d; \
      *l0 = 0; *l1 = 1; \
      zVec##XD##Copy( zEdge##XD##Vert(edge,1), cp ); \
      return ret; \
    } \
    zVec##XD##Copy( point, cp ); \
    return 0; \
  }

#define ZEO_ELEM_EDGEXD_CONTIG_VERT_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##ContigVert(const zEdge##XD *edge, const zVec##XD *point, double *distance)
#define ZEO_ELEM_EDGEXD_CONTIG_VERT(XD) \
  ZEO_ELEM_EDGEXD_CONTIG_VERT_PROTOTYPE( XD ){ \
    double d0, d1, _distance; \
    if( !distance ) distance = &_distance; \
    d0 = zVec##XD##SqrDist( point, zEdge##XD##Vert(edge,0) ); \
    d1 = zVec##XD##SqrDist( point, zEdge##XD##Vert(edge,1) ); \
    if( d0 <= d1 ){ \
      if( distance ) *distance = sqrt( d0 ); \
      return zEdge##XD##Vert(edge,0); \
    } else{ \
      if( distance ) *distance = sqrt( d1 ); \
      return zEdge##XD##Vert(edge,1); \
    } \
  }

#define ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN_PROTOTYPE(XD) \
  double zEdge##XD##ClosestFromOrigin(const zVec##XD *vert0, const zVec##XD *vert1, double *s0, double *s1, zVec##XD *closestpoint)
#define ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN(XD) \
  ZEO_ELEM_EDGEXD_CLOSEST_FROM_ORIGIN_PROTOTYPE( XD ){ \
    zVec2D l, u; \
    zMat2D vtv; \
    double d; \
    _zVec2DCreate( &l, 1.0, 1.0 ); \
    vtv.c.xx = zVec##XD##SqrNorm(vert0); \
    vtv.c.xy = vtv.c.yx = zVec##XD##InnerProd(vert0,vert1); \
    vtv.c.yy = zVec##XD##SqrNorm(vert1); \
    zMulAdjMat2DVec2D( &vtv, &l, &u ); \
    d = 1.0 / ( u.c.x + u.c.y ); \
    *s0 = u.c.x * d; \
    *s1 = u.c.y * d; \
    if( *s0 < 0 ){ \
      *s0 = 0; *s1 = 1; \
      zVec##XD##Copy( vert1, closestpoint ); \
    } else\
    if( *s1 < 0 ){ \
      *s0 = 1; *s1 = 0; \
      zVec##XD##Copy( vert0, closestpoint ); \
    } else{\
      zVec##XD##Mul( vert0, *s0, closestpoint ); \
      zVec##XD##CatDRC( closestpoint, *s1, vert1 ); \
    } \
    return zVec##XD##Norm( closestpoint ); \
  }

#define ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE(XD) \
  double zEdge##XD##Closest(const zEdge##XD *edge, const zVec##XD *point, zVec##XD *closestpoint)
#define ZEO_ELEM_EDGEXD_CLOSEST(XD) \
  ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE( XD ){ \
    zVec##XD v0, v1; \
    double dist; \
    double s0, s1; \
    zVec##XD##Sub( zEdge##XD##Vert(edge,0), point, &v0 ); \
    zVec##XD##Sub( zEdge##XD##Vert(edge,1), point, &v1 ); \
    dist = zEdge##XD##ClosestFromOrigin( &v0, &v1, &s0, &s1, closestpoint ); \
    zVec##XD##AddDRC( closestpoint, point ); \
    return dist; \
  }

#define ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE(XD) \
  double zEdge##XD##DistFromPoint(const zEdge##XD *edge, const zVec##XD *point)
#define ZEO_ELEM_EDGEXD_DIST_FROM_POINT(XD) \
  ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE( XD ){ \
    zVec##XD cp; \
    return zEdge##XD##Closest( edge, point, &cp ); \
  }

#define ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE(XD) \
  bool zEdge##XD##PointIsOn(const zEdge##XD *edge, const zVec##XD *point, double margin)
#define ZEO_ELEM_EDGEXD_POINT_IS_ON(XD) \
  ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE( XD ){ \
    return zEdge##XD##DistFromPoint( edge, point ) < margin; \
  }

#define ZEO_ELEM_EDGEXD_FPRINT_PROTOTYPE(XD) \
  void zEdge##XD##FPrint(FILE *fp, const zEdge##XD *edge)
#define ZEO_ELEM_EDGEXD_FPRINT(XD) \
  ZEO_ELEM_EDGEXD_FPRINT_PROTOTYPE( XD ){ \
    if( !edge ) \
      fprintf( fp, "(null edge)\n" ); \
    else{ \
      fprintf( fp, "vert: 0 " ); \
      zVec##XD##FPrint( fp, zEdge##XD##Vert(edge,0) ); \
      fprintf( fp, "vert: 1 " ); \
      zVec##XD##FPrint( fp, zEdge##XD##Vert(edge,1) ); \
      fprintf( fp, "vec: " ); \
      zVec##XD##FPrint( fp, zEdge##XD##Vec(edge) ); \
    } \
  }

/* ********************************************************** */
/* commonized definition of 2D/3D triangle class.
 * ********************************************************** */

#define ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Barycenter(const zTri##XD *tri, zVec##XD *c)
#define ZEO_ELEM_TRIXD_BARYCENTER(XD) \
  ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE( XD ){ \
    zVec##XD##Add( zTri##XD##Vert(tri,0), zTri##XD##Vert(tri,1), c ); \
    zVec##XD##AddDRC( c, zTri##XD##Vert(tri,2) ); \
    return zVec##XD##DivDRC( c, 3 ); \
  }

#define ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Circumcenter(const zTri##XD *tri, zVec##XD *c)
/* _zTri##XD##CircumcenterRel() has to be defined for each dimension in advance. */
#define ZEO_ELEM_TRIXD_CIRCUMCENTER(XD) \
  ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE( XD ){ \
    zVec##XD u, w, oa; \
    int a; \
    if( ( a = _zTri##XD##CircumcenterRel( tri, &u, &w, &oa ) ) < 0 ){ \
      ZRUNERROR( ZEO_ERR_ELEM_DEG_TRI ); \
      return NULL; \
    } \
    return zVec##XD##Add( zTri##XD##Vert(tri,a), &oa, c ); \
  }

#define ZEO_ELEM_TRIXD_INCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Incenter(const zTri##XD *tri, zVec##XD *c)
#define ZEO_ELEM_TRIXD_INCENTER(XD) \
  ZEO_ELEM_TRIXD_INCENTER_PROTOTYPE( XD ){ \
    double s[3]; \
    s[0] = zVec##XD##Dist( zTri##XD##Vert(tri,1), zTri##XD##Vert(tri,2) ); \
    s[1] = zVec##XD##Dist( zTri##XD##Vert(tri,2), zTri##XD##Vert(tri,0) ); \
    s[2] = zVec##XD##Dist( zTri##XD##Vert(tri,0), zTri##XD##Vert(tri,1) ); \
    _zTri##XD##WeightedCenter( tri, s[0], s[1], s[2], c ); \
    return c; \
  }

#define ZEO_ELEM_TRIXD_ORTHOCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Orthocenter(const zTri##XD *tri, zVec##XD *c)
/* the orthocenter H is computed from the circumcenter O by the relation H = A + B + C - 2 O,
 * where the vertex A is the origin of the edge vectors u = B - A and w = C - A. */
#define ZEO_ELEM_TRIXD_ORTHOCENTER(XD) \
  ZEO_ELEM_TRIXD_ORTHOCENTER_PROTOTYPE( XD ){ \
    zVec##XD u, w, oa; \
    int a; \
    if( ( a = _zTri##XD##CircumcenterRel( tri, &u, &w, &oa ) ) < 0 ){ \
      ZRUNERROR( ZEO_ERR_ELEM_DEG_TRI ); \
      return NULL; \
    } \
    zVec##XD##AddDRC( &u, &w ); \
    zVec##XD##CatDRC( &u, -2, &oa ); \
    return zVec##XD##Add( zTri##XD##Vert(tri,a), &u, c ); \
  }

#define ZEO_ELEM_TRIXD_CONTIG_VERT_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##ContigVert(const zTri##XD *tri, const zVec##XD *p, double *d)
#define ZEO_ELEM_TRIXD_CONTIG_VERT(XD) \
  ZEO_ELEM_TRIXD_CONTIG_VERT_PROTOTYPE( XD ){ \
    double d0, d1, d2; \
    d0 = zVec##XD##SqrDist( p, zTri##XD##Vert(tri,0) ); \
    d1 = zVec##XD##SqrDist( p, zTri##XD##Vert(tri,1) ); \
    d2 = zVec##XD##SqrDist( p, zTri##XD##Vert(tri,2) ); \
    if( d0 <= d1 ){ \
      if( d0 <= d2 ){ \
        if( d ) *d = sqrt( d0 ); \
        return zTri##XD##Vert(tri,0); \
      }{ \
        if( d ) *d = sqrt( d2 ); \
        return zTri##XD##Vert(tri,2); \
      } \
    }{ \
      if( d1 <= d2 ){ \
        if( d ) *d = sqrt( d1 ); \
        return zTri##XD##Vert(tri,1); \
      }{ \
        if( d ) *d = sqrt( d2 ); \
        return zTri##XD##Vert(tri,2); \
      } \
    } \
  }

#define ZEO_ELEM_TRIXD_DIST_FROM_POINT_PROTOTYPE(XD) \
  double zTri##XD##DistFromPoint(const zTri##XD *tri, const zVec##XD *point)
#define ZEO_ELEM_TRIXD_DIST_FROM_POINT(XD) \
  ZEO_ELEM_TRIXD_DIST_FROM_POINT_PROTOTYPE( XD ){ \
    zVec##XD cp; \
    return zTri##XD##Closest( tri, point, &cp ); \
  }

#define ZEO_ELEM_TRIXD_POINT_IS_INSIDE_PROTOTYPE(XD) \
  bool zTri##XD##PointIsInside(const zTri##XD *tri, const zVec##XD *point, double margin)
#define ZEO_ELEM_TRIXD_POINT_IS_INSIDE(XD) \
  ZEO_ELEM_TRIXD_POINT_IS_INSIDE_PROTOTYPE( XD ){ \
    return zTri##XD##DistFromPoint( tri, point ) < margin; \
  }

/* ********************************************************** */
/* commonized definition of 2D ellipse and 3D ellipsoid.
 * ********************************************************** */

/* the closest point on the boundary of an axis-aligned ellipse (dim=2) or ellipsoid (dim=3), the
 * center of which is at the origin and the radii of which along the axes are r, to a point y.
 * The closest point x satisfies x_i = r_i^2 y_i / ( t + r_i^2 ) for a Lagrange multiplier t, which is
 * the root of S(t) = 1 in t > -rmin^2, where S(t) = sum_i ( r_i y_i / ( t + r_i^2 ) )^2 and rmin is
 * the minimum radius. The root is non-negative if y is outside, and non-positive if y is inside.
 * The Newton method is applied to psi(t) = S(t)^(-1/2) - 1, which is exactly linear if only one term
 * of S(t) remains. Since S(t)^(-1/2) is a weighted power mean of t + r_i^2 of the order -2 except a
 * constant factor, psi(t) is increasing and concave in t > -rmin^2. Hence, the iteration from a lower
 * bound of the root monotonically converges to the root without overshooting. A lower bound is
 * max_i( r_i |y_i| - r_i^2 ), since every term of S(t) is not larger than one at the root, and it is
 * replaced by zero if y is outside.
 * The root can be very close to -rmin^2 if y is inside and y_i of the minimum radius is tiny. In order
 * to avoid the cancellation, s = t + rmin^2 is used as the variable instead of t, and t + r_i^2 is
 * computed as s + ( r_i^2 - rmin^2 ).
 * If y_i = 0 for every i of the minimum radius, S(t) may be less than one at t = -rmin^2. In this
 * case, the closest point is at t = -rmin^2, where x_i of the minimum radius is determined so that x
 * is on the boundary. */
#define ZEO_ELEM_ELLIPSXD_CLOSEST(dim) \
static void _zEllips##dim##DClosest(const double r[], const zVec##dim##D *y, zVec##dim##D *x) \
{ \
  double s, ds, d, u, sum, dsum, q, rmin2, a[dim]; \
  int i, imin, iter; \
\
  for( q=0, rmin2=HUGE_VAL, imin=0, i=0; i<dim; i++ ){ \
    q += zSqr( y->e[i] / r[i] ); \
    if( r[i] * r[i] < rmin2 ) rmin2 = r[imin = i] * r[i]; \
  } \
  for( i=0; i<dim; i++ ) /* t + r_i^2 = s + a_i */ \
    a[i] = i == imin ? 0 : r[i] * r[i] - rmin2; \
  for( s=0, i=0; i<dim; i++ ) \
    if( y->e[i] != 0 && ( d = r[i] * fabs( y->e[i] ) - a[i] ) > s ) s = d; \
  if( q > 1 && s < rmin2 ) s = rmin2; /* outside */ \
  for( iter=0; iter<Z_MAX_ITER_NUM; iter++ ){ \
    for( sum=dsum=0, i=0; i<dim; i++ ){ \
      if( y->e[i] == 0 ) continue; \
      d = s + a[i]; \
      u = r[i] * y->e[i] / d; \
      sum += ( u *= u ); \
      dsum += u / d; \
    } \
    if( sum <= 1 ) break; \
    /* psi = sum^(-1/2) - 1 and psi' = sum^(-3/2) dsum, where dsum = -S'(t)/2. */ \
    ds = sum * ( sqrt( sum ) - 1 ) / dsum; \
    if( s + ds == s ) break; \
    s += ds; \
  } \
  for( sum=0, i=0; i<dim; i++ ){ \
    x->e[i] = y->e[i] == 0 ? 0 : r[i] * r[i] * y->e[i] / ( s + a[i] ); \
    if( i != imin ) sum += zSqr( x->e[i] / r[i] ); \
  } \
  if( s == 0 && y->e[imin] == 0 ) /* the degenerate case */ \
    x->e[imin] = sum < 1 ? sqrt( rmin2 * ( 1 - sum ) ) : 0; \
}

/* the signed distance from a point y to an axis-aligned ellipse (dim=2) or ellipsoid (dim=3) with
 * radii r, which is negative if y is inside. ZEO_ELEM_ELLIPSXD_CLOSEST(dim) is required. */
#define ZEO_ELEM_ELLIPSXD_SIGNED_DIST(dim) \
static double _zEllips##dim##DSignedDist(const double r[], const zVec##dim##D *y) \
{ \
  zVec##dim##D x; \
  double q = 0, d; \
  int i; \
\
  _zEllips##dim##DClosest( r, y, &x ); \
  for( i=0; i<dim; i++ ) q += zSqr( y->e[i] / r[i] ); \
  d = zVec##dim##DDist( y, &x ); \
  return q < 1 ? -d : d; \
}

/* check if the signed distance from a point y to an axis-aligned ellipse (dim=2) or ellipsoid
 * (dim=3) with radii r is less than a margin. The exact distance is computed only if y is between
 * two ellipsoids that bound the offset (margin>0) or the inner parallel body (margin<0) of the
 * original ellipsoid E. For a positive margin m, E enlarged along the axes by m is inside of the
 * offset, and E scaled by 1+m/rmin includes it, where rmin is the minimum radius. For a negative
 * margin -d, E scaled by 1-d/rmin is inside of the inner parallel body, and E shrunk along the axes by
 * d includes it. These follow from the triangle inequality of the support functions.
 * ZEO_ELEM_ELLIPSXD_SIGNED_DIST(dim) is required. */
#define ZEO_ELEM_ELLIPSXD_IS_INSIDE(dim) \
static bool _zEllips##dim##DIsInside(const double r[], const zVec##dim##D *y, double margin) \
{ \
  double q, qm, rmin; \
  int i; \
\
  for( q=qm=0, rmin=HUGE_VAL, i=0; i<dim; i++ ){ \
    q += zSqr( y->e[i] / r[i] ); \
    if( r[i] < rmin ) rmin = r[i]; \
  } \
  if( margin > 0 ){ \
    if( q <= 1 ) return true; \
    if( q >= zSqr( 1 + margin / rmin ) ) return false; \
    for( i=0; i<dim; i++ ) qm += zSqr( y->e[i] / ( r[i] + margin ) ); \
    if( qm < 1 ) return true; \
  } else{ \
    if( q >= 1 ) return false; \
    if( margin == 0 ) return true; \
    if( rmin + margin <= 0 ) return false; /* no inner parallel body */ \
    if( q < zSqr( 1 + margin / rmin ) ) return true; \
    for( i=0; i<dim; i++ ) qm += zSqr( y->e[i] / ( r[i] + margin ) ); \
    if( qm >= 1 ) return false; \
  } \
  return _zEllips##dim##DSignedDist( r, y ) < margin; \
}

#endif /* __ZEO_ELEMXD_H__ */

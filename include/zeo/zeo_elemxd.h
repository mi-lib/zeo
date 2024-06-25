/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elemxd - 2D/3D shape element.
 */

#ifndef __ZEO_ELEMXD_H__
#define __ZEO_ELEMXD_H__

/* NOTE: never include this header file in user programs. */

/* ********************************************************** */
/* XD line class
 * ********************************************************** */

#define ZEO_ELEM_LINEXD_DEF_STRUCT(XD) \
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zLine##XD ){ \
  zVec##XD *origin; \
  zVec##XD direction; \
}

#define zLineXDOrg(XD,line)       (line)->origin
#define zLineXDDir(XD,line)       ( &(line)->direction )
#define zLineXDSetOrg(XD,line,p)  ( zLine##XD##Org(line) = p )
#define zLineXDSetDir(XD,line,d)  zVec##XD##Copy( d, zLine##XD##Dir(line) )

#define ZEO_ELEM_LINEXD_INIT_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##Init(zLine##XD *line)
#define ZEO_ELEM_LINEXD_INIT(XD) \
  ZEO_ELEM_LINEXD_INIT_PROTOTYPE( XD ){ \
    zLine##XD##SetOrg( line, ZVEC##XD##ZERO ); \
    zLine##XD##SetDir( line, ZVEC##XD##X ); \
    return line; \
  }

#define ZEO_ELEM_LINEXD_CREATE_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##Create(zLine##XD *line, zVec##XD *p, zVec##XD *direction)
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
    zLine##XD##SetOrg( line, p ); \
    zVec##XD##NormalizeNC( direction, zLine##XD##Dir(line) ); \
    return line; \
  }

#define ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE(XD) \
  zLine##XD *zLine##XD##CreateTwoPoints(zLine##XD *line, zVec##XD *p1, zVec##XD *p2)
#define ZEO_ELEM_LINEXD_CREATE_TWOPOINT(XD) \
  ZEO_ELEM_LINEXD_CREATE_TWOPOINT_PROTOTYPE( XD ){ \
    zLine##XD##SetOrg( line, p1 ); \
    zVec##XD##Sub( p2, p1, zLine##XD##Dir(line) ); \
    zVec##XD##NormalizeDRC( zLine##XD##Dir(line) ); \
    return line; \
  }

#define ZEO_ELEM_LINEXD_POINT_PROTOTYPE(XD) \
  zVec##XD *zLine##XD##Point(zLine##XD *line, double magnitude, zVec##XD *p)
#define ZEO_ELEM_LINEXD_POINT(XD) \
  ZEO_ELEM_LINEXD_POINT_PROTOTYPE( XD ){ \
    _zVec##XD##Cat( zLine##XD##Org(line), magnitude, zLine##XD##Dir(line), p ); \
    return p; \
  }

#define ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE(XD) \
  zVec##XD *zLine##XD##ProjPoint(zLine##XD *line, zVec##XD *src, zVec##XD *projection)
#define ZEO_ELEM_LINEXD_PROJ_POINT(XD) \
  ZEO_ELEM_LINEXD_PROJ_POINT_PROTOTYPE( XD ){ \
    zVec##XD v; \
    return zLine##XD##Point( line, zVec##XD##InnerProd( zLine##XD##Dir(line), zVec##XD##Sub( src, zLine##XD##Org(line), &v ) ), projection ); \
  }

#define ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE(XD) \
  double zLine##XD##DistFromPoint(zLine##XD *line, zVec##XD *p)
#define ZEO_ELEM_LINEXD_DIST_FROM_POINT(XD) \
  ZEO_ELEM_LINEXD_DIST_FROM_POINT_PROTOTYPE( XD ){ \
    zVec##XD v; \
    zVec##XD##Sub( p, zLine##XD##Org(line), &v ); \
    return zVec##XD##OuterProdNorm( &v, zLine##XD##Dir(line) ); \
  }

#define ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE(XD) \
  bool zLine##XD##PointIsOn(zLine##XD *line, zVec##XD *p, double margin)
#define ZEO_ELEM_LINEXD_POINT_IS_ON(XD) \
  ZEO_ELEM_LINEXD_POINT_IS_ON_PROTOTYPE( XD ){ \
    return zLine##XD##DistFromPoint( line, p ) < margin; \
  }

/* ********************************************************** */
/* XD edge class
 * ********************************************************** */

#define ZEO_ELEM_EDGEXD_DEF_STRUCT(XD) \
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zEdge##XD ){ \
  zVec##XD *vert[2]; /*!< \brief two vertices */ \
  zVec##XD vec;      /*!< \brief edge vector */ \
}

#define zEdgeXDVert(XD,edge,i)        (edge)->vert[(i)]
#define zEdgeXDVec(XD,edge)           ( &(edge)->vec )

#define zEdgeXDSetVert(XD,edge,i,ptr) ( (edge)->vert[(i)] = (ptr) )
#define zEdgeXDSetVec(XD,edge,vec)    zVec##XD##Copy( vec, zEdge##XD##Vec(edge) )

#define ZEO_ELEM_EDGEXD_INIT_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Init(zEdge##XD *edge)
#define ZEO_ELEM_EDGEXD_INIT(XD) \
  ZEO_ELEM_EDGEXD_INIT_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( edge, 0, ZVEC##XD##ZERO ); \
    zEdge##XD##SetVert( edge, 1, ZVEC##XD##ZERO ); \
    zEdge##XD##SetVec( edge, ZVEC##XD##ZERO ); \
    return edge; \
  }

#define ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Create(zEdge##XD *edge, zVec##XD *v1, zVec##XD *v2)
#define ZEO_ELEM_EDGEXD_CREATE(XD) \
  ZEO_ELEM_EDGEXD_CREATE_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( edge, 0, v1 ); \
    zEdge##XD##SetVert( edge, 1, v2 ); \
    zEdge##XD##CalcVec( edge ); \
    return edge; \
  }

#define ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##CalcVec(zEdge##XD *edge)
#define ZEO_ELEM_EDGEXD_CALC_VEC(XD) \
  ZEO_ELEM_EDGEXD_CALC_VEC_PROTOTYPE( XD ){ \
    return zVec##XD##Sub( zEdge##XD##Vert(edge,1), zEdge##XD##Vert(edge,0), zEdge##XD##Vec(edge) ); \
  }

#define ZEO_ELEM_EDGEXD_LEN(XD,edge)     zVec##XD##Norm( zEdge##XD##Vec(edge) )
#define ZEO_ELEM_EDGEXD_DIR(XD,edge,dir) zVec##XD##Normalize( zEdge##XD##Vec(edge), dir )

/* unit_dir  ... unit direction vector of the edge
 * rel_point ... relative position vector of the point from the 1st vertex of the edge
 * len       ... length of the edge
 */
#define _ZEO_ELEM_EDGEXD_PROJ_SET(XD) \
  static bool _zEdge##XD##ProjSet(zEdge##XD *edge, zVec##XD *point, zVec##XD *unit_dir, zVec##XD *rel_point, double *len){ \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,0), rel_point ); \
    *len = ZEO_ELEM_EDGEXD_DIR( XD, edge, unit_dir ); \
    return !zVec##XD##IsTiny( rel_point ); \
  }

#define ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##ProjPoint(zEdge##XD *edge, zVec##XD *point, zVec##XD *cp)
#define ZEO_ELEM_EDGEXD_PROJ_POINT(XD) \
  ZEO_ELEM_EDGEXD_PROJ_POINT_PROTOTYPE( XD ){ \
    zVec##XD unit_dir, rel_point; \
    double len; \
    _zEdge##XD##ProjSet( edge, point, &unit_dir, &rel_point, &len ); \
    return zVec##XD##Cat( zEdge##XD##Vert(edge,0), zVec##XD##InnerProd( &rel_point, &unit_dir ), &unit_dir, cp ); \
  }

#define ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE(XD) \
  double zEdge##XD##DistFromPoint(zEdge##XD *edge, zVec##XD *point)
#define ZEO_ELEM_EDGEXD_DIST_FROM_POINT(XD) \
  ZEO_ELEM_EDGEXD_DIST_FROM_POINT_PROTOTYPE( XD ){ \
    zVec##XD cp; \
    return zEdge##XD##Closest( edge, point, &cp ); \
  }

#define ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE(XD) \
  bool zEdge##XD##PointIsOnLine(zEdge##XD *edge, zVec##XD *point, double margin)
#define ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE(XD) \
  ZEO_ELEM_EDGEXD_POINT_IS_ON_LINE_PROTOTYPE( XD ){ \
    zVec##XD unit_dir, rel_point; \
    double len; \
    if( !_zEdge##XD##ProjSet( edge, point, &unit_dir, &rel_point, &len ) ) return true; \
    return zVec##XD##OuterProdNorm( &rel_point, &unit_dir ) < margin; \
  }

#define ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE(XD) \
  bool zEdge##XD##PointIsOn(zEdge##XD *edge, zVec##XD *point, double margin)
#define ZEO_ELEM_EDGEXD_POINT_IS_ON(XD) \
  ZEO_ELEM_EDGEXD_POINT_IS_ON_PROTOTYPE( XD ){ \
    return zEdge##XD##DistFromPoint( edge, point ) < margin; \
  }

#define _ZEO_ELEM_EDGEXD_LINSCALE(XD) \
  static double _zEdge##XD##LinScale(zEdge##XD *edge, zVec##XD *point, double *l0, double *l1, zVec##XD *cp){ \
    zVec##XD dp0, dp1, v; \
    double d, ret; \
    int ni; \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,0), &dp0 ); \
    zVec##XD##Sub( point, zEdge##XD##Vert(edge,1), &dp1 ); \
    ni = ( zVec##XD##SqrNorm(&dp0) <= zVec##XD##SqrNorm(&dp1) ) ? 0 : 1; \
    if( ( d = zVec##XD##Normalize( zEdge##XD##Vec(edge), &v ) ) < 0 ){ \
      ZRUNERROR( ZEO_ERR_ELEM_DEGE ); \
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

#define ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE(XD) \
  double zEdge##XD##Closest(zEdge##XD *edge, zVec##XD *point, zVec##XD *cp)
#define ZEO_ELEM_EDGEXD_CLOSEST(XD) \
  ZEO_ELEM_EDGEXD_CLOSEST_PROTOTYPE( XD ){ \
    zVec##XD unit_dir, rel_point; \
    double len, len_proj; \
    _zEdge##XD##ProjSet( edge, point, &unit_dir, &rel_point, &len ); \
    len_proj = zVec##XD##InnerProd( &rel_point, &unit_dir ); \
    if( len_proj <= 0 ){ \
      zVec##XD##Copy( zEdge##XD##Vert(edge,0), cp ); \
    } else \
    if( len_proj > 0 && len_proj < len ){ \
      zVec##XD##Cat( zEdge##XD##Vert(edge,0), zVec##XD##InnerProd( &rel_point, &unit_dir ), &unit_dir, cp ); \
    } else \
      zVec##XD##Copy( zEdge##XD##Vert(edge,1), cp ); \
    return zVec##XD##Dist( point, cp ); \
  }

#define ZEO_ELEM_EDGEXD_CONTIG_VERT_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##ContigVert(zEdge##XD *edge, zVec##XD *point, double *distance)
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

#define ZEO_ELEM_EDGEXD_FPRINT_PROTOTYPE(XD) \
  void zEdge##XD##FPrint(FILE *fp, zEdge##XD *edge)
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
/* XD triangle class
 * ********************************************************** */

#define ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Barycenter(zTri##XD *tri, zVec##XD *c)
#define ZEO_ELEM_TRIXD_BARYCENTER(XD) \
  ZEO_ELEM_TRIXD_BARYCENTER_PROTOTYPE( XD ){ \
    zVec##XD##Add( zTri##XD##Vert(tri,0), zTri##XD##Vert(tri,1), c ); \
    zVec##XD##AddDRC( c, zTri##XD##Vert(tri,2) ); \
    return zVec##XD##DivDRC( c, 3 ); \
  }

#define ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Circumcenter(zTri##XD *tri, zVec##XD *c)
#define ZEO_ELEM_TRIXD_CIRCUMCENTER(XD) \
  ZEO_ELEM_TRIXD_CIRCUMCENTER_PROTOTYPE( XD ){ \
    double r[3], s[3]; \
    r[0] = zVec##XD##SqrDist( zTri##XD##Vert(tri,2), zTri##XD##Vert(tri,1) ); \
    r[1] = zVec##XD##SqrDist( zTri##XD##Vert(tri,0), zTri##XD##Vert(tri,2) ); \
    r[2] = zVec##XD##SqrDist( zTri##XD##Vert(tri,1), zTri##XD##Vert(tri,0) ); \
    s[0] = r[0] * ( r[1] + r[2] - r[0] ); \
    s[1] = r[1] * ( r[2] + r[0] - r[1] ); \
    s[2] = r[2] * ( r[0] + r[1] - r[2] ); \
    _zTri##XD##WeightedCenter( tri, s[0], s[1], s[2], c ); \
    return c; \
  }

#define ZEO_ELEM_TRIXD_INCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Incenter(zTri##XD *tri, zVec##XD *c)
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
  zVec##XD *zTri##XD##Orthocenter(zTri##XD *tri, zVec##XD *c)
#define ZEO_ELEM_TRIXD_ORTHOCENTER(XD) \
  ZEO_ELEM_TRIXD_ORTHOCENTER_PROTOTYPE( XD ){ \
    zVec##XD e[3]; \
    zTri##XD circum_tri; \
    zVec##XD##Add( zTri##XD##Vert(tri,0), zTri##XD##Vert(tri,1), &e[0] ); \
    zVec##XD##SubDRC( &e[0], zTri##XD##Vert(tri,2) ); \
    zVec##XD##Add( zTri##XD##Vert(tri,1), zTri##XD##Vert(tri,2), &e[1] ); \
    zVec##XD##SubDRC( &e[1], zTri##XD##Vert(tri,0) ); \
    zVec##XD##Add( zTri##XD##Vert(tri,2), zTri##XD##Vert(tri,0), &e[2] ); \
    zVec##XD##SubDRC( &e[2], zTri##XD##Vert(tri,1) ); \
    zTri##XD##Create( &circum_tri, &e[0], &e[1], &e[2] ); \
    return zTri##XD##Circumcenter( &circum_tri, c ); \
  }

#endif /* __ZEO_ELEMXD_H__ */

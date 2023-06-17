/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elemxd - 2D/3D shape element.
 */

#ifndef __ZEO_ELEMXD_H__
#define __ZEO_ELEMXD_H__

/* NOTE: never include this header file in user programs. */

/* ********************************************************** */
/* XD edge class
 * ********************************************************** */

#define ZEDGEXD_INIT_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Init(zEdge##XD *e)
#define ZEDGEXD_INIT(XD) \
  ZEDGEXD_INIT_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( e, 0, NULL ); \
    zEdge##XD##SetVert( e, 1, NULL ); \
    zEdge##XD##SetVec( e, ZVEC##XD##ZERO ); \
    return e; \
  }

#define ZEDGEXD_CREATE_PROTOTYPE(XD) \
  zEdge##XD *zEdge##XD##Create(zEdge##XD *e, zVec##XD *v1, zVec##XD *v2)
#define ZEDGEXD_CREATE(XD) \
  ZEDGEXD_CREATE_PROTOTYPE( XD ){ \
    zEdge##XD##SetVert( e, 0, v1 ); \
    zEdge##XD##SetVert( e, 1, v2 ); \
    zEdge##XD##CalcVec( e ); \
    return e; \
  }

#define ZEDGEXD_CALC_VEC_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##CalcVec(zEdge##XD *e)
#define ZEDGEXD_CALC_VEC(XD) \
  ZEDGEXD_CALC_VEC_PROTOTYPE( XD ){ \
    return zVec##XD##Sub( zEdge##XD##Vert(e,1), zEdge##XD##Vert(e,0), zEdge##XD##Vec(e) ); \
  }

#define _ZEDGEXD_PROJ_SET_PROTOTYPE(XD) \
  static bool _zEdge##XD##ProjSet(zEdge##XD *e, zVec##XD *p, zVec##XD *v, zVec##XD *dp)
#define _ZEDGEXD_PROJ_SET(XD) \
  _ZEDGEXD_PROJ_SET_PROTOTYPE( XD ){ \
    zVec##XD##Sub( p, zEdge##XD##Vert(e,0), dp ); \
    zVec##XD##Normalize( zEdge##XD##Vec(e), v ); \
    return !zVec##XD##IsTiny( dp ); \
  }

#define ZEDGEXD_PROJ_PROTOTYPE(XD) \
  zVec##XD *zEdge##XD##Proj(zEdge##XD *e, zVec##XD *p, zVec##XD *cp)
#define ZEDGEXD_PROJ(XD) \
  ZEDGEXD_PROJ_PROTOTYPE( XD ){ \
    zVec##XD v, dp; \
    _zEdge##XD##ProjSet( e, p, &v, &dp ); \
    zVec##XD##Mul( &v, zVec##XD##InnerProd(&dp,&v), cp ); \
    return zVec##XD##AddDRC( cp, zEdge##XD##Vert(e,0) ); \
  }

#define ZEDGEXD_POINT_DIST_PROTOTYPE(XD) \
  double zEdge##XD##PointDist(zEdge##XD *e, zVec##XD *p)
#define ZEDGEXD_POINT_DIST(XD) \
  ZEDGEXD_POINT_DIST_PROTOTYPE( XD ){ \
    zVec##XD v, dp; \
    _zEdge##XD##ProjSet( e, p, &v, &dp ); \
    return zVec##XD##OuterProdNorm( &dp, &v ); \
  }

#define ZEDGEXD_LINSCALE_PROTOTYPE(XD) \
  double zEdge##XD##LinScale(zEdge##XD *e, zVec##XD *p, double *l0, double *l1, zVec##XD *cp)
#define ZEDGEXD_LINSCALE(XD) \
  ZEDGEXD_LINSCALE_PROTOTYPE( XD ){ \
    zVec##XD dp0, dp1, v; \
    double d, ret; \
    int ni; \
    zVec##XD##Sub( p, zEdge##XD##Vert(e,0), &dp0 ); \
    zVec##XD##Sub( p, zEdge##XD##Vert(e,1), &dp1 ); \
    ni = ( zVec##XD##SqrNorm(&dp0) <= zVec##XD##SqrNorm(&dp1) ) ? 0 : 1; \
    if( ( d = zVec##XD##Normalize( zEdge##XD##Vec(e), &v ) ) < 0 ){ \
      ZRUNERROR( ZEO_ERR_ELEM_DEGE ); \
      return -1; \
    } \
    if( ni == 0 ){ \
      *l1 = zVec##XD##InnerProd( &dp0, &v ) / d; \
      *l0 = 1 - *l1; \
    } else{ \
      *l0 =-zVec##XD##InnerProd( &dp1, &v ) / d; \
      *l1 = 1 - *l0; \
    } \
    if( *l1 < 0 ){ \
      ret = -*l1 * d; \
      *l1 = 0; *l0 = 1; \
      zVec##XD##Copy( zEdge##XD##Vert(e,0), cp ); \
      return ret; \
    } else \
    if( *l0 < 0 ){ \
      ret = -*l0 * d; \
      *l0 = 0; *l1 = 1; \
      zVec##XD##Copy( zEdge##XD##Vert(e,1), cp ); \
      return ret; \
    } \
    zVec##XD##Copy( p, cp ); \
    return 0; \
  }

#define ZEDGEXD_CLOSEST_PROTOTYPE(XD) \
  double zEdge##XD##Closest(zEdge##XD *e, zVec##XD *p, zVec##XD *cp)
#define ZEDGEXD_CLOSEST(XD) \
  ZEDGEXD_CLOSEST_PROTOTYPE(XD){ \
    zVec##XD tmp; \
    double l0, l1; \
    zEdge##XD##Proj( e, p, &tmp ); \
    zEdge##XD##LinScale( e, &tmp, &l0, &l1, cp ); \
    return zVec##XD##Dist( p, cp ); \
  }

/* ********************************************************** */
/* XD triangle class
 * ********************************************************** */

#define ZTRIXD_BARYCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Barycenter(zTri##XD *t, zVec##XD *c)
#define ZTRIXD_BARYCENTER(XD) \
  ZTRIXD_BARYCENTER_PROTOTYPE( XD ){ \
    zVec##XD##Add( zTri##XD##Vert(t,0), zTri##XD##Vert(t,1), c ); \
    zVec##XD##AddDRC( c, zTri##XD##Vert(t,2) ); \
    return zVec##XD##DivDRC( c, 3 ); \
  }

#define ZTRIXD_CIRCUMCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Circumcenter(zTri##XD *t, zVec##XD *c)
#define ZTRIXD_CIRCUMCENTER(XD) \
  ZTRIXD_CIRCUMCENTER_PROTOTYPE( XD ){ \
    double r[3], s[3]; \
    r[0] = zVec##XD##SqrDist( zTri##XD##Vert(t,2), zTri##XD##Vert(t,1) ); \
    r[1] = zVec##XD##SqrDist( zTri##XD##Vert(t,0), zTri##XD##Vert(t,2) ); \
    r[2] = zVec##XD##SqrDist( zTri##XD##Vert(t,1), zTri##XD##Vert(t,0) ); \
    s[0] = r[0] * ( r[1] + r[2] - r[0] ); \
    s[1] = r[1] * ( r[2] + r[0] - r[1] ); \
    s[2] = r[2] * ( r[0] + r[1] - r[2] ); \
    _zTri##XD##WeightedCenter( t, s[0], s[1], s[2], c ); \
    return c; \
  }

#define ZTRIXD_INCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Incenter(zTri##XD *t, zVec##XD *c)
#define ZTRIXD_INCENTER(XD) \
  ZTRIXD_INCENTER_PROTOTYPE( XD ){ \
    double s[3]; \
    s[0] = zVec##XD##Dist( zTri##XD##Vert(t,1), zTri##XD##Vert(t,2) ); \
    s[1] = zVec##XD##Dist( zTri##XD##Vert(t,2), zTri##XD##Vert(t,0) ); \
    s[2] = zVec##XD##Dist( zTri##XD##Vert(t,0), zTri##XD##Vert(t,1) ); \
    _zTri##XD##WeightedCenter( t, s[0], s[1], s[2], c ); \
    return c; \
  }

#define ZTRIXD_ORTHOCENTER_PROTOTYPE(XD) \
  zVec##XD *zTri##XD##Orthocenter(zTri##XD *t, zVec##XD *c)
#define ZTRIXD_ORTHOCENTER(XD) \
  ZTRIXD_ORTHOCENTER_PROTOTYPE( XD ){ \
    zVec##XD cg, cc; \
    zTri##XD##Circumcenter( t, &cc ); \
    zTri##XD##Barycenter( t, &cg ); \
    zVec##XD##Mul( &cg, 3, c ); \
    return zVec##XD##CatDRC( c, -2, &cc ); \
  }

#endif /* __ZEO_ELEMXD_H__ */

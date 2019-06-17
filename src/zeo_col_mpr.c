/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_mpr - collision checking: Minkowski Portal Refinement algorithm.
 */

#include <zeo/zeo_col.h>

typedef struct{
  zVec3D v;  /* support point in Minkowski's sum */
  zVec3D v1; /* vertex on object 1 corresponding to the support map */
  zVec3D v2; /* vertex on object 2 corresponding to the support map */
} zMPRSlot;

#define _zMPRSlotCopy(src,dest) zCopy( zMPRSlot, src, dest )

typedef struct{
  int n;            /* number of active slots */
  zMPRSlot slot[4]; /* slots of simplex */
} zMPRSimplex;

static zVec3D *_zMPRSupportMap(zMPRSlot *s, zVec3D p1[], int n1, zVec3D p2[], int n2, zVec3D *v);
static zVec3D *_zMPROrigin(zMPRSlot *center, zVec3D p1[], int n1, zVec3D p2[], int n2);
static zVec3D *_zMPRSimplexDir(zMPRSimplex *s, int i0, int i1, int i2, zVec3D *dir);
static int _zMPRFindPortal(zMPRSimplex *portal, zVec3D p1[], int n1, zVec3D p2[], int n2);
static zVec3D *_zMPRPortalDir(zMPRSimplex* portal, zVec3D* dir);
static bool _zMPRPortalIsReached(zMPRSimplex *portal, zMPRSlot *s, zVec3D *dir);
static zMPRSlot *_zMPRExpandPortal(zMPRSimplex* portal, zMPRSlot *s);
static bool _zMPRRefinePortal(zMPRSimplex* portal, zVec3D p1[], int n1, zVec3D p2[], int n2);
static void _zMPRDepthPair(zMPRSimplex* portal, zVec3D *pos);
static double _zMPRCalcSegDist(zVec3D *p0, zVec3D *p1, zVec3D *dir);
static double _zMPRCalcDepth(zVec3D *p0, zVec3D *p1, zVec3D *p2, zVec3D *dir);
static bool _zMPRDepth(zMPRSimplex* portal, zVec3D p1[], int n1, zVec3D p2[], int n2, double *depth, zVec3D *pos, zVec3D *dir);

/* support map of Minkowski difference. */
zVec3D *_zMPRSupportMap(zMPRSlot *s, zVec3D p1[], int n1, zVec3D p2[], int n2, zVec3D *v)
{
  zVec3D nv;

  _zVec3DRev( v, &nv );
  zVec3DCopy( zVec3DSupportMap( p1, n1,   v ), &s->v1 );
  zVec3DCopy( zVec3DSupportMap( p2, n2, &nv ), &s->v2 );
  _zVec3DSub( &s->v1, &s->v2, &s->v );
  return &s->v;
}

/* original Minkowski portal of sets of points. */
zVec3D *_zMPROrigin(zMPRSlot *center, zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  register int i;

  for( zVec3DZero( &center->v1 ), i=0; i<n1; i++ )
    zVec3DAddDRC( &center->v1, &p1[i] );
  zVec3DDivDRC( &center->v1, n1 );
  for( zVec3DZero( &center->v2 ), i=0; i<n2; i++ )
    zVec3DAddDRC( &center->v2, &p2[i] );
  zVec3DDivDRC( &center->v2, n2 );
  _zVec3DSub( &center->v1, &center->v2, &center->v );
  return &center->v;
}

/* direction vector of with respect to two relative support maps of a simplex. */
zVec3D *_zMPRSimplexDir(zMPRSimplex *s, int i0, int i1, int i2, zVec3D *dir)
{
  zVec3D d1, d2;

  zVec3DSub( &s->slot[i1].v, &s->slot[i0].v, &d1 );
  zVec3DSub( &s->slot[i2].v, &s->slot[i0].v, &d2 );
  _zVec3DOuterProd( &d1, &d2, dir );
  zVec3DNormalizeNCDRC( dir );
  return dir;
}

#define Z_MPR_BIAS ( zTOL * 10 )

enum{ Z_MPR_PORTAL_OUTSIDE = -1, Z_MPR_PORTAL_TO_REFINE = 0, Z_MPR_PORTAL_AT_POINT = 1, Z_MPR_PORTAL_ON_SEG = 2 };

/* find Minkowski portal. */
int _zMPRFindPortal(zMPRSimplex *portal, zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  zVec3D dir;
  zMPRSlot tmp;

  /* vertex 0: the center of portal */
  _zMPROrigin( &portal->slot[0], p1, n1, p2, n2 );
  portal->n = 1;
  if( zVec3DIsTiny( &portal->slot[0].v ) )
    /* intersecting case: the center is slightly biased in order to compute penetration depth. */
    portal->slot[0].v.e[0] += Z_MPR_BIAS;

  /* vertex 1 = support in direction to origin */
  zVec3DNormalizeNCDRC( zVec3DRev( &portal->slot[0].v, &dir ) );
  _zMPRSupportMap( &portal->slot[1], p1, n1, p2, n2, &dir );
  portal->n = 2;
  if( zVec3DInnerProd( &portal->slot[1].v, &dir ) < zTOL ) return Z_MPR_PORTAL_OUTSIDE;

  /* vertex 2 */
  zVec3DOuterProd( &portal->slot[1].v, &portal->slot[0].v, &dir );
  if( zVec3DIsTiny( &dir ) ) /* origin lies at vertex 1 or between vertices 0 and 1. */
    return zVec3DIsTiny( &portal->slot[1].v ) ? Z_MPR_PORTAL_AT_POINT : Z_MPR_PORTAL_ON_SEG;
  zVec3DNormalizeNCDRC( &dir );
  _zMPRSupportMap( &portal->slot[2], p1, n1, p2, n2, &dir );
  portal->n = 3;
  if( zVec3DInnerProd( &portal->slot[2].v, &dir ) < zTOL ) return Z_MPR_PORTAL_OUTSIDE;

  /* vertex 3 */
  _zMPRSimplexDir( portal, 0, 1, 2, &dir );
  if( zVec3DInnerProd( &dir, &portal->slot[0].v ) > 0 ){
    /* swap slot[1] and slot[2] in order to form portal faces outward */
    _zMPRSlotCopy( &portal->slot[1], &tmp );
    _zMPRSlotCopy( &portal->slot[2], &portal->slot[1] );
    _zMPRSlotCopy( &tmp, &portal->slot[2] );
    _zVec3DRevDRC( &dir );
  }
  while( 1 ){
    _zMPRSupportMap( &portal->slot[3], p1, n1, p2, n2, &dir );
    if( zVec3DInnerProd( &portal->slot[3].v, &dir ) < zTOL ) return Z_MPR_PORTAL_OUTSIDE;
    /* test if origin is outside of (v1, v0, v3) - set v3 for v2 and continue */
    if( zVec3DGrassmannProd( &portal->slot[0].v, &portal->slot[1].v, &portal->slot[3].v ) <= -zTOL ){
      _zMPRSlotCopy( &portal->slot[3], &portal->slot[2] );
      _zMPRSimplexDir( portal, 0, 1, 2, &dir );
    } else{
      /* test if origin is outside of (v3, v0, v2) - set v3 for v1 and continue */
      if( zVec3DGrassmannProd( &portal->slot[0].v, &portal->slot[3].v, &portal->slot[2].v ) <= -zTOL ){
        _zMPRSlotCopy( &portal->slot[3], &portal->slot[1] );
        _zMPRSimplexDir( portal, 0, 1, 2, &dir );
      } else
        break;
    }
  }
  portal->n = 4;
  return Z_MPR_PORTAL_TO_REFINE;
}

/* direction vector of Minkowski portal. */
zVec3D *_zMPRPortalDir(zMPRSimplex* portal, zVec3D* dir)
{
  _zMPRSimplexDir( portal, 1, 2, 3, dir );
  return dir;
}

/* check if Minkowski portal can no longer expanded. */
bool _zMPRPortalIsReached(zMPRSimplex *portal, zMPRSlot *s, zVec3D *dir)
{
  double r;

  r = _zVec3DInnerProd( &s->v, dir );
  return r - zVec3DInnerProd( &portal->slot[1].v, dir ) < zTOL &&
         r - zVec3DInnerProd( &portal->slot[2].v, dir ) < zTOL &&
         r - zVec3DInnerProd( &portal->slot[3].v, dir ) < zTOL;
}

/* expand Minkowski portal. */
zMPRSlot *_zMPRExpandPortal(zMPRSimplex* portal, zMPRSlot *s)
{
  zVec3D d;

  zVec3DOuterProd( &s->v, &portal->slot[0].v, &d );
  if( zVec3DInnerProd( &portal->slot[1].v, &d ) > 0 )
    return zVec3DInnerProd( &portal->slot[2].v, &d ) > 0 ?
      _zMPRSlotCopy( s, &portal->slot[1] ) : _zMPRSlotCopy( s, &portal->slot[3] );
  return zVec3DInnerProd( &portal->slot[3].v, &d ) > 0 ?
    _zMPRSlotCopy( s, &portal->slot[2] ) : _zMPRSlotCopy( s, &portal->slot[1] );
}

/* refine Minkowski portal. */
bool _zMPRRefinePortal(zMPRSimplex* portal, zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  zVec3D dir;
  zMPRSlot s;

  while( 1 ){
    /* compute outward direction of portal */
    _zMPRPortalDir( portal, &dir );
    /* test if origin is inside of portal */
    if( zVec3DInnerProd( &portal->slot[1].v, &dir ) > -zTOL ) return true;
    /* next support point */
    _zMPRSupportMap( &s, p1, n1, p2, n2, &dir );
    /* test if portal can be expanded toward origin. */
    if( zVec3DInnerProd( &s.v, &dir ) <= -zTOL ||
        _zMPRPortalIsReached( portal, &s, &dir ) ) return false;
    _zMPRExpandPortal( portal, &s );
  }
  return false;
}

/* set the origin of Minkowski portal for the barycenter of a tetrahedral simplex. */
void _zMPRDepthPair(zMPRSimplex* portal, zVec3D *pos)
{
  zVec3D dir;
  size_t i;
  double b[4], sum;
  zVec3D p1, p2;

  _zMPRPortalDir( portal, &dir );
  b[0] = zVec3DGrassmannProd( &portal->slot[3].v, &portal->slot[1].v, &portal->slot[2].v );
  b[1] = zVec3DGrassmannProd( &portal->slot[0].v, &portal->slot[3].v, &portal->slot[2].v );
  b[2] = zVec3DGrassmannProd( &portal->slot[3].v, &portal->slot[0].v, &portal->slot[1].v );
  b[3] = zVec3DGrassmannProd( &portal->slot[0].v, &portal->slot[2].v, &portal->slot[1].v );
  if( ( sum = b[0] + b[1] + b[2] + b[3] ) < zTOL ){ /* degenerated case */
    b[0] = 0.0;
    b[1] = zVec3DGrassmannProd( &dir, &portal->slot[2].v, &portal->slot[3].v );
    b[2] = zVec3DGrassmannProd( &dir, &portal->slot[3].v, &portal->slot[1].v );
    b[3] = zVec3DGrassmannProd( &dir, &portal->slot[1].v, &portal->slot[2].v );
    sum = b[1] + b[2] + b[3];
  }
  _zVec3DZero( &p1 );
  _zVec3DZero( &p2 );
  for( i=0; i<4; i++ ){
    zVec3DCatDRC( &p1, b[i], &portal->slot[i].v1 );
    zVec3DCatDRC( &p2, b[i], &portal->slot[i].v2 );
  }
  sum = 1.0 / sum;
  _zVec3DMulDRC( &p1, sum );
  _zVec3DMulDRC( &p2, sum );
  _zVec3DMid( &p1, &p2, pos );
}

/* calculate distance of an extreme segment on Minkowski portal. */
double _zMPRCalcSegDist(zVec3D *p0, zVec3D *p1, zVec3D *dir)
{
  double t;
  zVec3D d;

  _zVec3DSub( p1, p0, &d );
  t = -_zVec3DInnerProd( p0, &d ) / _zVec3DSqrNorm( &d );
  if( t < zTOL ){
    if( dir ) zVec3DCopy( p0, dir );
    return _zVec3DSqrNorm( p0 );
  }
  if( t > zTOL + 1 ){
    if( dir ) zVec3DCopy( p1, dir );
    return _zVec3DSqrNorm( p1 );
  }
  if( dir ){
    _zVec3DCat( p0, t, &d, dir );
    return _zVec3DSqrNorm( dir );
  }
  _zVec3DCat( p0, t, &d, &d );
  return _zVec3DSqrNorm( &d );
}

/* calculate penetration depth of colliding objects. */
double _zMPRCalcDepth(zVec3D *p0, zVec3D *p1, zVec3D *p2, zVec3D *dir)
{
  zVec3D d1, d2, w2;
  double u1, u2, u3, v1, v2, v3, s, t, dist, d;

  _zVec3DSub( p1, p0, &d1 );
  _zVec3DSub( p2, p0, &d2 );

  u1 = _zVec3DSqrNorm( p0 );
  u2 = _zVec3DSqrNorm( &d1 );
  u3 = _zVec3DSqrNorm( &d2 );
  v1 = _zVec3DInnerProd( p0, &d1 );
  v2 = _zVec3DInnerProd( p0, &d2 );
  v3 = _zVec3DInnerProd( &d1, &d2 );

  s = ( v2 * v3 - u3 * v1 ) / ( u3 * u2 - v3 * v3 );
  t = (-s * v3 - v2 ) / u3;
  if( ( s > zTOL ) && ( s < zTOL + 1 ) && 
      ( t > zTOL ) && ( t < zTOL + 1 ) && 
      ( t + s < zTOL + 1 ) ){
    if( dir ){
      _zVec3DCat( p0, s, &d1, dir );
      _zVec3DCatDRC( dir, t, &d2 );
      return _zVec3DSqrNorm( dir );
    } else
      return u1 + s * s * u2 + t * t * u3 + 2 * ( s * t * v3 + s * v1 + t * v2 );
  }
  dist = _zMPRCalcSegDist( p0, p1, dir );
  if( ( d = _zMPRCalcSegDist( p0, p2, &w2 ) ) < dist ){
    dist = d;
    if( dir ) zVec3DCopy( &w2, dir );
  }
  if( ( d = _zMPRCalcSegDist( p1, p2, &w2 ) ) < dist ){
    dist = d;
    if( dir ) zVec3DCopy( &w2, dir );
  }
  return dist;
}

/* calculate penetration depth of colliding objects based on MPR algorithm. */
bool _zMPRDepth(zMPRSimplex* portal, zVec3D p1[], int n1, zVec3D p2[], int n2, double *depth, zVec3D *pos, zVec3D *dir)
{
  zVec3D d;
  zMPRSlot s;
  int iter = 0;
  register int i;

  ZITERINIT( iter );
  for( i=0; i<iter; i++ ){
    /* compute portal direction and obtain next support point */
    _zMPRPortalDir( portal, &d );
    _zMPRSupportMap( &s, p1, n1, p2, n2, &d );
    /* reached tolerance -> find penetration info */
    if( _zMPRPortalIsReached( portal, &s, &d ) ){
      if( depth && dir ){
        *depth = sqrt( _zMPRCalcDepth( &portal->slot[1].v, &portal->slot[2].v, &portal->slot[3].v, dir ) );
        zVec3DNormalizeNCDRC( dir );
      }
      if( pos ) _zMPRDepthPair( portal, pos );
      return true;
    }
    _zMPRExpandPortal( portal, &s );
  }
  ZITERWARN( iter );
  return false;
}

/* Minkowski Portal Refinement algorithm. */
bool zMPR(zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  zMPRSimplex portal;

  switch( _zMPRFindPortal( &portal, p1, n1, p2, n2 ) ){
  case Z_MPR_PORTAL_OUTSIDE: return false;
  case Z_MPR_PORTAL_TO_REFINE: return _zMPRRefinePortal( &portal, p1, n1, p2, n2 );
  default: ;
  }
  return true;
}

/* Minkowski Portal Refinement algorithm with penetration depth */
bool zMPRDepth(zVec3D p1[], int n1, zVec3D p2[], int n2, double *depth, zVec3D *pos, zVec3D *dir)
{
  zMPRSimplex portal;

  /* Phase 1: find portal */
  switch( _zMPRFindPortal( &portal, p1, n1, p2, n2 ) ){
  case Z_MPR_PORTAL_AT_POINT: /* contact at a point. */
    *depth = 0;
    _zVec3DZero( dir );
    zVec3DMid( &portal.slot[1].v1, &portal.slot[1].v2, pos );
    break;
  case Z_MPR_PORTAL_ON_SEG: /* contact on a segment */
    *depth = zVec3DNormalizeNCDRC( zVec3DCopy( &portal.slot[1].v, dir ) );
    zVec3DMid( &portal.slot[1].v1, &portal.slot[1].v2, pos );
    break;
  case Z_MPR_PORTAL_TO_REFINE: /* Phase 2: refine portal */
    if( _zMPRRefinePortal( &portal, p1, n1, p2, n2 ) ){
      /* compute penetration depth */
      return _zMPRDepth( &portal, p1, n1, p2, n2, depth, pos, dir );
    }
  default: /* no collision */
    return false;
  }
  return true;
}

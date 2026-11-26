/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_gjk - collision checking: Gilbert-Johnson-Keerthi's algorithm.
 */

#include <zeo/zeo_col.h>

typedef struct{
  bool sw_w;  /* included in W (the smallest simplex) */
  bool sw_y;  /* included in Y (the updated simplex) */
  zVec3D w;   /* the destination of support map for Minkowski's sum */
  const zVec3D *p1; /* corresponding vertex on object 1 to the support map */
  const zVec3D *p2; /* corresponding vertex on object 2 to the support map */
  double s;   /* linear sum coefficient : Weights used to represent nearest neighbors as linear combinations of simplex vertices, and used to exclude unnecessary vertices. If the weight becomes zero, that vertex is considered unused. */
} zGJKSlot;

typedef struct{
  int n;
  zGJKSlot slot[4];
} zGJKSimplex;

/* initialize GJK slot. */
static void _zGJKSlotInit(zGJKSlot *slot)
{
  slot->sw_w = slot->sw_y = false;
  zVec3DZero( &slot->w );
  slot->p1 = slot->p2 = NULL;
  slot->s = 0;
}

#if defined( DEBUG ) && DEBUG == 1
/* print out slot information. */
static void _zGJKSlotPrint(zGJKSlot *slot)
{
  printf( " w: " ); zVec3DPrint( &slot->w );
  printf( " p1: " ); zVec3DPrint( slot->p1 );
  printf( " p2: " ); zVec3DPrint( slot->p2 );
  printf( " s = %g\n", slot->s );
}

/* print out vertices of a slot. */
static void _zGJKSlotVertFPrint(FILE *fp, zGJKSlot *slot)
{
  zVec3DValueFPrint( fp, slot->p1 );
  zVec3DValueFPrint( fp, slot->p2 );
}
#endif

/* initialize GJK simplex set. */
static void _zGJKSimplexInit(zGJKSimplex *s)
{
  s->n = 0;
  _zGJKSlotInit( &s->slot[0] );
  _zGJKSlotInit( &s->slot[1] );
  _zGJKSlotInit( &s->slot[2] );
  _zGJKSlotInit( &s->slot[3] );
}

#if defined( DEBUG ) && DEBUG == 1
/* print out GJK simplex set Y (non-minimum simplex) */
static void _zGJKSimplexYPrint(zGJKSimplex *s)
{
  int i;

  printf( "[Y]\n" );
  for( i=0; i<4; i++ )
    if( s->slot[i].sw_y ){
      printf( "<slot %d>\n", i );
      _zGJKSlotPrint( &s->slot[i] );
    }
}

/* print out GJK simplex set W (minimum simplex) */
static void _zGJKSimplexWPrint(zGJKSimplex *s)
{
  int i;

  printf( "[W]\n" );
  for( i=0; i<4; i++ )
    if( s->slot[i].sw_w ){
      printf( "<slot %d>\n", i );
      _zGJKSlotPrint( &s->slot[i] );
    }
}

/* print out vertices of GJK simplex set Y (non-minimum simplex) */
static void _zGJKSimplexYVertFPrint(FILE *fp, zGJKSimplex *s)
{
  int i;

  for( i=0; i<4; i++ )
    if( s->slot[i].sw_y )
      _zGJKSlotVertFPrint( fp, &s->slot[i] );
}

/* print out vertices GJK simplex set W (minimum simplex) */
static void _zGJKSimplexWVertFPrint(FILE *fp, zGJKSimplex *s)
{
  int i;

  for( i=0; i<4; i++ )
    if( s->slot[i].sw_w )
      _zGJKSlotVertFPrint( fp, &s->slot[i] );
}
#endif

/* check if the specified slot is already in the previous testing simplex. */
static bool _zGJKSimplexCheckDupSlot(zGJKSimplex *s, zGJKSlot *slot)
{
  int i;

  for( i=0; i<4; i++ )
    if( s->slot[i].sw_y && zVec3DMatch( &s->slot[i].w, &slot->w ) )
      return true;
  return false;
}

/* add a slot into the current testing simplex. */
static int _zGJKSimplexAddSlot(zGJKSimplex *s, zGJKSlot *slot)
{
  int i;

  for( i=0; i<4; i++ )
    s->slot[i].sw_y = s->slot[i].sw_w;
  for( i=0; i<4; i++ )
    if( !s->slot[i].sw_y ){
      s->slot[i].sw_y = true;
      zVec3DCopy( &slot->w, &s->slot[i].w );
      s->slot[i].p1 = slot->p1;
      s->slot[i].p2 = slot->p2;
      s->slot[i].s = slot->s;
      return i;
    }
  ZRUNERROR( ZEO_ERR_FATAL );
  return -1;
}

/* find the closest point in/on the testing simplex. */
/* @param[in,out] s  simplex */
/* @param[out]    v  a separating vector when closest */
/* @return  if successful, closest point, otherwise NULL. */
static zVec3D *_zGJKSimplexClosest(zGJKSimplex *s, zVec3D *v)
{
  zGJKSlot *slot[4];
  int i, n;

  /* create index */
  for( n=0, i=0; i<4; i++ )
    if( s->slot[i].sw_y ) slot[n++] = &s->slot[i];
  switch( n ){
  case 1:
    zVec3DCopy( &slot[0]->w, v );
    slot[0]->s = 1;
    break;
  case 2:
    zEdge3DClosestFromOrigin( &slot[0]->w, &slot[1]->w, &slot[0]->s, &slot[1]->s, v );
    break;
  case 3:
    zTri3DClosestFromOrigin( &slot[0]->w, &slot[1]->w, &slot[2]->w, &slot[0]->s, &slot[1]->s, &slot[2]->s, v );
    break;
  case 4:
    zTetra3DClosestFromOrigin( &slot[0]->w, &slot[1]->w, &slot[2]->w, &slot[3]->w, &slot[0]->s, &slot[1]->s, &slot[2]->s, &slot[3]->s, v );
    break;
  default:
    ZRUNERROR( ZEO_ERR_FATAL );
    return NULL;
  }
  /* minimize the simplex. */
  for( s->n=0, i=0; i<4; i++ ){
    if( zIsTiny( s->slot[i].s ) ){
      s->slot[i].s = 0;
      s->slot[i].sw_w = false;
    } else{
      s->slot[i].sw_w = true;
      s->n++;
    }
  }
  return v;
}

/* support map of Minkowski difference. */
/* @param[out] s      slots that are simplex elements (3D vertex w, etc.) */
/* @param[in]  data1  3D vertices 1 (must be made from convex hull) */
/* @param[in]  data2  3D vertices 2 (must be made from convex hull) */
/* @param[in]  v      separating axis for suppport mapping (and get reverse vector nv) */
/* @return  the edge in Minkowski difference space (data1 - data2) */
static zVec3D *_zGJKSupportMap(zGJKSlot *slot, zVec3DData *data1, zVec3DData *data2, zVec3D *v)
{
  zVec3D nv;

  zVec3DRev( v, &nv );
  slot->p1 = zVec3DDataSupportMap( data1, &nv );
  slot->p2 = zVec3DDataSupportMap( data2,   v );
  zVec3DSub( slot->p1, slot->p2, &slot->w );
  return &slot->w;
}

/* a pair of points on the original convex hulls. */
static void _zGJKPair(zGJKSimplex *s, zVec3D *c1, zVec3D *c2)
{
  int i;

  zVec3DZero( c1 );
  zVec3DZero( c2 );
  for( i=0; i<4; i++ )
    if( s->slot[i].sw_w ){
      zVec3DCatDRC( c1, s->slot[i].s, s->slot[i].p1 );
      zVec3DCatDRC( c2, s->slot[i].s, s->slot[i].p2 );
    }
}

/* The tolerances of the termination are relative to the size of the problem, namely, the maximum
 * norm of the support points of the simplex and the corresponding points on the original convex
 * hulls, so that they do not depend on the scale and the position of the convex hulls. The norms of
 * the original points are taken into account, since the rounding errors of the support points are
 * proportional to them. */

/* maximum number of iterations, which is a safety guard against an infinite loop. */
#define ZEO_GJK_MAX_ITER Z_MAX_ITER_NUM

/* squared size of a slot. */
static double _zGJKSlotSqrSize(const zGJKSlot *slot)
{
  double r2, l2;

  r2 = _zVec3DSqrNorm( &slot->w );
  if( ( l2 = _zVec3DSqrNorm( slot->p1 ) ) > r2 ) r2 = l2;
  if( ( l2 = _zVec3DSqrNorm( slot->p2 ) ) > r2 ) r2 = l2;
  return r2;
}

/* update the squared size of the problem with a new slot. */
#define _zGJKUpdateSqrSize(r2,slot) do{\
  double __r2 = _zGJKSlotSqrSize( slot );\
  if( __r2 > (r2) ) (r2) = __r2;\
} while(0)

/* check if the origin is reached, namely, the distance between the two convex hulls, which is the
 * norm of v, is not larger than zTOL times the size of the problem (r2 is the squared size). */
static bool _zGJKIsTouched(const zVec3D *v, double r2)
{
  return _zVec3DSqrNorm( v ) <= zTOL * zTOL * r2;
}

/* check if the iteration of GJK converges. v is the closest point of the current simplex to the
 * origin, and w is the support point in the direction of -v. The distance between the convex hulls
 * is between v.w/|v| and |v|, so that the iteration terminates if the difference is not larger
 * than zTOL times the size of the problem, namely, |v|^2 - v.w <= zTOL |v| r. */
static bool _zGJKIsConverged(const zVec3D *v, const zVec3D *w, double r2)
{
  double vv, d;

  vv = _zVec3DSqrNorm( v );
  return ( d = vv - _zVec3DInnerProd( v, w ) ) <= 0 || d * d <= zTOL * zTOL * vv * r2;
}

/* check if the two convex hulls collide, namely, the simplex contains the origin, or the origin is
 * reached. */
static bool _zGJKCheck(zGJKSimplex *s, const zVec3D *v, double r2)
{
  return s->n == 4 || _zGJKIsTouched( v, r2 );
}

static bool _zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2, zGJKSimplex *s)
{
  zGJKSimplex _s; /* simplex */
  zGJKSlot slot;
  zVec3D proximity;
  double r2 = 0;
  int iter = 0;

  if( s == NULL ) s = &_s;
  zVec3DDataRewind( data1 );
  zVec3DDataRewind( data2 );
  zVec3DSub( zVec3DDataPeek( data1 ), zVec3DDataPeek( data2 ), &proximity );
  _zGJKSimplexInit( s );
  do{
    _zGJKSupportMap( &slot, data1, data2, &proximity );
    _zGJKUpdateSqrSize( r2, &slot );
    if( ( s->n > 0 && _zGJKIsConverged( &proximity, &slot.w, r2 ) ) ||
        _zGJKSimplexCheckDupSlot( s, &slot ) )
      break; /* succeed */
    _zGJKSimplexAddSlot( s, &slot );
    _zGJKSimplexClosest( s, &proximity );
    if( _zGJKIsTouched( &proximity, r2 ) ) break; /* collide */
  } while( s->n < 4 && ++iter < ZEO_GJK_MAX_ITER );
  _zGJKPair( s, c1, c2 );
  return _zGJKCheck( s, &proximity, r2 );
}

/* Gilbert-Johnson-Keerthi algorithm. */
bool zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2)
{
  return _zGJK( data1, data2, c1, c2, NULL );
}

/* support map of Minkowski difference. */
static zVec3D *_zGJKPointSupportMap(zGJKSlot *s, zVec3DData *data, zVec3D *v)
{
  zVec3D nv;

  zVec3DRev( v, &nv );
  s->p1 = zVec3DDataSupportMap( data, &nv );
  zVec3DSub( s->p1, s->p2, &s->w );
  return &s->w;
}

/* a pair of points on the original convex hulls. */
static void _zGJKPointClosest(zGJKSimplex *s, zVec3D *c)
{
  int i;

  zVec3DZero( c );
  for( i=0; i<4; i++ )
    if( s->slot[i].sw_w )
      zVec3DCatDRC( c, s->slot[i].s, s->slot[i].p1 );
}

/* Gilbert-Johnson-Keerthi algorithm for a set of points and an independent point. */
bool zGJKPoint(zVec3DData *data, zVec3D *p, zVec3D *c)
{
  zGJKSimplex s; /* simplex */
  zGJKSlot slot;
  zVec3D proximity;
  double r2 = 0;
  int iter = 0;

  slot.p2 = p;
  zVec3DDataRewind( data );
  zVec3DSub( zVec3DDataPeek( data ), p, &proximity );
  _zGJKSimplexInit( &s );
  do{
    _zGJKPointSupportMap( &slot, data, &proximity );
    _zGJKUpdateSqrSize( r2, &slot );
    if( ( s.n > 0 && _zGJKIsConverged( &proximity, &slot.w, r2 ) ) ||
        _zGJKSimplexCheckDupSlot( &s, &slot ) )
      break; /* succeed */
    _zGJKSimplexAddSlot( &s, &slot );
    _zGJKSimplexClosest( &s, &proximity );
    if( _zGJKIsTouched( &proximity, r2 ) ) break; /* collide */
  } while( s.n < 4 && ++iter < ZEO_GJK_MAX_ITER );
  _zGJKPointClosest( &s, c );
  return _zGJKCheck( &s, &proximity, r2 );
}

/* Expanding Polytope Algorithm (EPA) for the penetration depth based on a method proposed by van den
 * Bergen:
 *   G. van den Bergen, Proximity Queries and Penetration Depth Computation on 3D Game Objects, Game
 *   Developers Conference, 2001.
 * The polytope is initialized with the simplex found by GJK that contains the origin, and is expanded
 * toward the boundary of the Minkowski difference by adding the support point in the direction of the
 * face closest to the origin. The faces visible from the new support point are removed, and the
 * horizon is connected with the support point. Since the visibility is judged by the exact orientation
 * predicate, the polytope is kept to be convex without computing the convex hull of the vertices. */

/* a face of the polytope */
typedef struct{
  int v[3];    /* vertex indices in counterclockwise order viewed from outside */
  int adj[3];  /* faces adjacent across the edges v[i]-v[i+1] */
  int adje[3]; /* edge indices of the adjacent faces */
  zVec3D n;    /* unit outward normal vector */
  double d;    /* signed distance of the face from the origin */
  bool inner;  /* true if the projection of the origin onto the face is inside of it */
  bool alive;  /* false if removed */
} zEPAFace;

/* an edge of the horizon, namely, an edge of an alive face adjacent to a removed face */
typedef struct{
  int f; /* face index */
  int e; /* edge index */
} zEPAEdge;

ZEDA_DEF_ARRAY_CLASS( zEPAVertArray, zGJKSlot );
ZEDA_DEF_ARRAY_CLASS( zEPAFaceArray, zEPAFace );
ZEDA_DEF_ARRAY_CLASS( zEPAEdgeArray, zEPAEdge );

typedef struct{
  zVec3DData *data1, *data2;
  zEPAVertArray vert;
  zEPAFaceArray face;
  zEPAEdgeArray horizon;
  double r2; /* squared size of the problem */
} zEPA;

#define _zEPAVert(epa,i)   ( &zArrayElemNC(&(epa)->vert,i)->w )
#define _zEPAFace(epa,i)   zArrayElemNC(&(epa)->face,i)
#define _zEPAFaceVert(epa,f,i) _zEPAVert( epa, (f)->v[i] )

/* tolerance of the problem, namely, zTOL times the size of the problem. */
#define _zEPATol(epa)      ( zTOL * sqrt( (epa)->r2 ) )

/* reserve a cell of an array, which is doubled if full. */
#define _zEPAArrayReserve(array,type) do{\
  if( zArraySize(array) == zArrayCapacity(array) )\
    zArrayRealloc( array, type, zArrayCapacity(array) > 0 ? 2 * zArrayCapacity(array) : 16 );\
} while(0)

/* initialize the polytope. */
static void _zEPAInit(zEPA *epa, zVec3DData *data1, zVec3DData *data2)
{
  epa->data1 = data1;
  epa->data2 = data2;
  zArrayInit( &epa->vert );
  zArrayInit( &epa->face );
  zArrayInit( &epa->horizon );
  epa->r2 = 0;
}

/* destroy the polytope. */
static void _zEPADestroy(zEPA *epa)
{
  zArrayFree( &epa->vert );
  zArrayFree( &epa->face );
  zArrayFree( &epa->horizon );
}

/* support point of the Minkowski difference in the direction of u. */
static void _zEPASupportMap(zEPA *epa, const zVec3D *u, zGJKSlot *slot)
{
  zVec3D v;

  zVec3DRev( u, &v );
  _zGJKSupportMap( slot, epa->data1, epa->data2, &v );
  _zGJKUpdateSqrSize( epa->r2, slot );
}

/* add a vertex to the polytope. The index of the vertex is returned, or -1 on failure. */
static int _zEPAAddVert(zEPA *epa, const zGJKSlot *slot)
{
  _zEPAArrayReserve( &epa->vert, zGJKSlot );
  if( zArraySize(&epa->vert) == zArrayCapacity(&epa->vert) ) return -1;
  *zArrayElemNC(&epa->vert,zArraySize(&epa->vert)) = *slot;
  _zGJKUpdateSqrSize( epa->r2, slot );
  return zArraySize(&epa->vert)++;
}

/* check if the projection of the origin onto a face is inside of it within the tolerance, namely,
 * the barycentric coordinates of the projection are not negative. */
static bool _zEPAFaceIsInner(zEPA *epa, zEPAFace *f)
{
  zVec3D p, d1, d2, e;
  double l[3], s;
  int i;

  zVec3DMul( &f->n, f->d, &p );
  for( s=0, i=0; i<3; i++ ){
    zVec3DSub( _zEPAFaceVert(epa,f,(i+1)%3), &p, &d1 );
    zVec3DSub( _zEPAFaceVert(epa,f,(i+2)%3), &p, &d2 );
    zVec3DOuterProd( &d1, &d2, &e );
    s += ( l[i] = zVec3DInnerProd( &f->n, &e ) );
  }
  if( s <= 0 ) return false;
  for( i=0; i<3; i++ )
    if( l[i] < -zTOL * s ) return false;
  return true;
}

/* add a face of vertices a, b and c to the polytope. The index of the face is returned, or -1 on
 * failure. A degenerate face, which has the zero normal vector, is put the infinite distance so as not
 * to be chosen as the closest face. */
static int _zEPAAddFace(zEPA *epa, int a, int b, int c)
{
  zEPAFace *f;
  zVec3D e1, e2;
  double l;

  _zEPAArrayReserve( &epa->face, zEPAFace );
  if( zArraySize(&epa->face) == zArrayCapacity(&epa->face) ) return -1;
  f = _zEPAFace( epa, zArraySize(&epa->face) );
  f->v[0] = a; f->v[1] = b; f->v[2] = c;
  f->adj[0] = f->adj[1] = f->adj[2] = -1;
  f->adje[0] = f->adje[1] = f->adje[2] = -1;
  f->alive = true;
  zVec3DSub( _zEPAVert(epa,b), _zEPAVert(epa,a), &e1 );
  zVec3DSub( _zEPAVert(epa,c), _zEPAVert(epa,a), &e2 );
  zVec3DOuterProd( &e1, &e2, &f->n );
  if( ( l = zVec3DNorm( &f->n ) ) > 0 ){
    zVec3DDivDRC( &f->n, l );
    f->d = zVec3DInnerProd( &f->n, _zEPAVert(epa,a) );
    f->inner = _zEPAFaceIsInner( epa, f );
  } else{
    f->d = HUGE_VAL;
    f->inner = false;
  }
  return zArraySize(&epa->face)++;
}

/* link the edges of the faces in [first,last) that are not linked yet with each other. */
static void _zEPALink(zEPA *epa, int first, int last)
{
  zEPAFace *f, *g;
  int i, j, k, l;

  for( i=first; i<last; i++ ){
    f = _zEPAFace( epa, i );
    for( j=0; j<3; j++ ){
      if( f->adj[j] >= 0 ) continue;
      for( k=i+1; k<last; k++ ){
        g = _zEPAFace( epa, k );
        for( l=0; l<3; l++ )
          if( g->adj[l] < 0 && g->v[l] == f->v[(j+1)%3] && g->v[(l+1)%3] == f->v[j] ){
            f->adj[j] = k; f->adje[j] = l;
            g->adj[l] = i; g->adje[l] = j;
            goto NEXT;
          }
      }
     NEXT: ;
    }
  }
}

/* check if a point p is strictly in front of a face. */
static bool _zEPAFaceIsVisible(zEPA *epa, zEPAFace *f, const zVec3D *p)
{
  return zOrient3D( _zEPAFaceVert(epa,f,0)->e, _zEPAFaceVert(epa,f,1)->e, _zEPAFaceVert(epa,f,2)->e, p->e ) > 0;
}

/* find the horizon from a point p by the depth-first search from the e-th edge of the i-th face. The
 * faces visible from p are removed. */
static bool _zEPASilhouette(zEPA *epa, int i, int e, const zVec3D *p)
{
  zEPAFace *f;
  zEPAEdge edge;

  if( i < 0 ) return false; /* not linked, which never happens */
  if( !( f = _zEPAFace( epa, i ) )->alive ) return true;
  if( !_zEPAFaceIsVisible( epa, f, p ) ){
    _zEPAArrayReserve( &epa->horizon, zEPAEdge );
    if( zArraySize(&epa->horizon) == zArrayCapacity(&epa->horizon) ) return false;
    edge.f = i; edge.e = e;
    *zArrayElemNC(&epa->horizon,zArraySize(&epa->horizon)++) = edge;
    return true;
  }
  f->alive = false;
  return _zEPASilhouette( epa, f->adj[(e+1)%3], f->adje[(e+1)%3], p ) &&
         _zEPASilhouette( epa, f->adj[(e+2)%3], f->adje[(e+2)%3], p );
}

/* expand the polytope by a vertex of index iv visible from the i-th face. */
static bool _zEPAExpand(zEPA *epa, int i, int iv)
{
  zEPAFace *f;
  zEPAEdge *edge;
  int j, k, first;

  zArraySetSize( &epa->horizon, 0 );
  f = _zEPAFace( epa, i );
  f->alive = false;
  for( j=0; j<3; j++ )
    if( !_zEPASilhouette( epa, f->adj[j], f->adje[j], _zEPAVert(epa,iv) ) ) return false;
  first = zArraySize(&epa->face);
  for( j=0; j<zArraySize(&epa->horizon); j++ ){
    edge = zArrayElemNC( &epa->horizon, j );
    f = _zEPAFace( epa, edge->f );
    if( ( k = _zEPAAddFace( epa, f->v[(edge->e+1)%3], f->v[edge->e], iv ) ) < 0 ) return false;
    f = _zEPAFace( epa, edge->f ); /* the array of faces might be reallocated */
    f->adj[edge->e] = k; f->adje[edge->e] = 0;
    _zEPAFace(epa,k)->adj[0] = edge->f;
    _zEPAFace(epa,k)->adje[0] = edge->e;
  }
  _zEPALink( epa, first, zArraySize(&epa->face) );
  return true;
}

/* the alive face closest to the origin. The candidates are restricted to the faces onto which the
 * projection of the origin is inside, as proposed by van den Bergen. It is because the closest point
 * of the polytope to the origin is inside of such a face, and the normal vector of a nearly degenerate
 * face is inaccurate, which can make its distance from the origin wrong, even negative. If no such
 * face is found due to rounding errors, all the alive faces are the candidates. */
static int _zEPAClosestFace(zEPA *epa)
{
  zEPAFace *f;
  double dmin = HUGE_VAL, dmin_all = HUGE_VAL;
  int i, imin = -1, imin_all = -1;

  for( i=0; i<zArraySize(&epa->face); i++ ){
    if( !( f = _zEPAFace( epa, i ) )->alive ) continue;
    if( f->inner && f->d < dmin ){
      dmin = f->d;
      imin = i;
    }
    if( f->d < dmin_all ){
      dmin_all = f->d;
      imin_all = i;
    }
  }
  return imin >= 0 ? imin : imin_all;
}

/* add a vertex to a simplex of vertices in the polytope, which is chosen from candidates cand[] or the
 * support points in the directions of u[], so that its distance from the affine hull of the simplex
 * is larger than the tolerance. The distance of a point p is computed by dist(epa,p,util). If no such
 * vertex is found, the Minkowski difference is regarded to be degenerate. */
static int _zEPASimplexAdd(zEPA *epa, zGJKSlot cand[], int nc, zVec3D u[], int nu, double (* dist)(zEPA*,const zVec3D*,void*), void *util)
{
  zGJKSlot slot;
  double d, dmax = 0;
  int i, imax = -1;

  for( i=0; i<nc; i++ )
    if( ( d = dist( epa, &cand[i].w, util ) ) > dmax ){
      dmax = d;
      imax = i;
    }
  if( imax >= 0 && dmax > _zEPATol(epa) ) return _zEPAAddVert( epa, &cand[imax] );
  for( i=0; i<nu; i++ ){
    _zEPASupportMap( epa, &u[i], &slot );
    if( dist( epa, &slot.w, util ) > _zEPATol(epa) ) return _zEPAAddVert( epa, &slot );
  }
  return -2; /* degenerate */
}

/* distance from the first vertex. */
static double _zEPADistFromPoint(zEPA *epa, const zVec3D *p, void *util)
{
  return zVec3DDist( p, _zEPAVert(epa,0) );
}

/* distance from the line passing the first and second vertices, whose direction is util. */
static double _zEPADistFromLine(zEPA *epa, const zVec3D *p, void *util)
{
  zVec3D d, e;

  zVec3DSub( p, _zEPAVert(epa,0), &d );
  zVec3DOuterProd( (zVec3D *)util, &d, &e );
  return zVec3DNorm( &e );
}

/* distance from the plane passing the first three vertices, whose normal vector is util. */
static double _zEPADistFromPlane(zEPA *epa, const zVec3D *p, void *util)
{
  zVec3D d;

  zVec3DSub( p, _zEPAVert(epa,0), &d );
  return fabs( zVec3DInnerProd( (zVec3D *)util, &d ) );
}

/* create a tetrahedron from the simplex found by GJK. If the simplex is not a tetrahedron, namely, the
 * origin is reached on a lower-dimensional simplex, vertices are added from the support points in the
 * directions orthogonal to the simplex. Note that the simplex can be a single point which is not on
 * the boundary of the Minkowski difference, if GJK starts from coincident points. It returns 1 if a
 * tetrahedron is created, 0 if the Minkowski difference is degenerate, namely, thinner than the
 * tolerance in a direction, in which case the penetration depth is regarded as zero, and -1 on
 * failure. */
static int _zEPACreateTetra(zEPA *epa, zGJKSimplex *s)
{
  zGJKSlot cand[4];
  zVec3D e, u[6];
  int i, nc, ret;

  for( nc=0, i=0; i<4; i++ )
    if( s->slot[i].sw_w ) cand[nc++] = s->slot[i];
  for( i=0; i<nc; i++ ) _zGJKUpdateSqrSize( epa->r2, &cand[i] );
  if( nc == 0 || _zEPAAddVert( epa, &cand[0] ) < 0 ) return -1;
  /* second vertex */
  for( i=0; i<3; i++ ){
    zVec3DZero( &u[i] );
    u[i].e[i] = 1;
  }
  zVec3DRev( &u[0], &u[3] );
  zVec3DRev( &u[1], &u[4] );
  zVec3DRev( &u[2], &u[5] );
  if( ( ret = _zEPASimplexAdd( epa, cand+1, nc-1, u, 6, _zEPADistFromPoint, NULL ) ) < 0 )
    return ret == -2 ? 0 : -1;
  /* third vertex */
  zVec3DSub( _zEPAVert(epa,1), _zEPAVert(epa,0), &e );
  zVec3DDivDRC( &e, zVec3DNorm( &e ) ); /* not normalized with the absolute tolerance */
  if( !zVec3DOrthonormalSpace( &e, &u[0], &u[2] ) ) return -1;
  zVec3DRev( &u[0], &u[1] );
  zVec3DRev( &u[2], &u[3] );
  if( ( ret = _zEPASimplexAdd( epa, cand+1, nc-1, u, 4, _zEPADistFromLine, &e ) ) < 0 )
    return ret == -2 ? 0 : -1;
  /* fourth vertex */
  zVec3DSub( _zEPAVert(epa,1), _zEPAVert(epa,0), &e );
  zVec3DSub( _zEPAVert(epa,2), _zEPAVert(epa,0), &u[1] );
  zVec3DOuterProd( &e, &u[1], &u[0] );
  zVec3DDivDRC( &u[0], zVec3DNorm( &u[0] ) );
  zVec3DRev( &u[0], &u[1] );
  if( ( ret = _zEPASimplexAdd( epa, cand+1, nc-1, u, 2, _zEPADistFromPlane, &u[0] ) ) < 0 )
    return ret == -2 ? 0 : -1;
  /* faces directing outward */
  if( ( i = zOrient3D( _zEPAVert(epa,0)->e, _zEPAVert(epa,1)->e, _zEPAVert(epa,2)->e, _zEPAVert(epa,3)->e ) ) == 0 )
    return 0;
  if( i < 0 ){
    if( _zEPAAddFace( epa, 0, 1, 2 ) < 0 || _zEPAAddFace( epa, 0, 3, 1 ) < 0 ||
        _zEPAAddFace( epa, 1, 3, 2 ) < 0 || _zEPAAddFace( epa, 0, 2, 3 ) < 0 ) return -1;
  } else{
    if( _zEPAAddFace( epa, 0, 2, 1 ) < 0 || _zEPAAddFace( epa, 0, 1, 3 ) < 0 ||
        _zEPAAddFace( epa, 1, 2, 3 ) < 0 || _zEPAAddFace( epa, 0, 3, 2 ) < 0 ) return -1;
  }
  _zEPALink( epa, 0, 4 );
  return 1;
}

/* a pair of points on the original convex hulls that corresponds to the foot of the perpendicular
 * from the origin to a face. */
static void _zEPAPair(zEPA *epa, zEPAFace *f, zVec3D *c1, zVec3D *c2)
{
  zVec3D p, d1, d2, e;
  zGJKSlot *slot;
  double l[3], s;
  int i;

  zVec3DMul( &f->n, f->d, &p );
  for( s=0, i=0; i<3; i++ ){
    zVec3DSub( _zEPAFaceVert(epa,f,(i+1)%3), &p, &d1 );
    zVec3DSub( _zEPAFaceVert(epa,f,(i+2)%3), &p, &d2 );
    zVec3DOuterProd( &d1, &d2, &e );
    s += ( l[i] = zVec3DInnerProd( &f->n, &e ) );
  }
  zVec3DZero( c1 );
  zVec3DZero( c2 );
  for( i=0; i<3; i++ ){
    slot = zArrayElemNC( &epa->vert, f->v[i] );
    zVec3DCatDRC( c1, l[i]/s, slot->p1 );
    zVec3DCatDRC( c2, l[i]/s, slot->p2 );
  }
}

/* penetration depth by EPA. If the penetration depth is regarded as zero, c1 and c2 are not modified. */
static bool _zEPA(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2, zGJKSimplex *s)
{
  zEPA epa;
  zEPAFace *f;
  zGJKSlot slot;
  int i, iv, iter;
  bool ret = false;

  _zEPAInit( &epa, data1, data2 );
  if( ( i = _zEPACreateTetra( &epa, s ) ) <= 0 ){
    ret = i == 0;
    goto TERMINATE;
  }
  for( iter=0; ; iter++ ){
    if( ( i = _zEPAClosestFace( &epa ) ) < 0 ) goto TERMINATE;
    f = _zEPAFace( &epa, i );
    _zEPASupportMap( &epa, &f->n, &slot );
    /* the penetration depth is between the distance of the closest face and that of the support plane */
    if( iter >= ZEO_GJK_MAX_ITER || !_zEPAFaceIsVisible( &epa, f, &slot.w ) ||
        zVec3DInnerProd( &f->n, &slot.w ) - f->d <= _zEPATol(&epa) ) break;
    if( ( iv = _zEPAAddVert( &epa, &slot ) ) < 0 || !_zEPAExpand( &epa, i, iv ) ) goto TERMINATE;
  }
  _zEPAPair( &epa, f, c1, c2 );
  ret = true;
 TERMINATE:
  _zEPADestroy( &epa );
  return ret;
}

/* GJK algorithm followed by EPA for the penetration depth. */
bool zGJKDepth(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2)
{
  zGJKSimplex s;

  return _zGJK( data1, data2, c1, c2, &s ) ? _zEPA( data1, data2, c1, c2, &s ) : false;
}

/* check if two convex hulls penetrate each other deeper than a given depth.
 * If the final simplex of the GJK algorithm is a tetrahedron, the origin is strictly inside of it,
 * since the vertices with tiny weights have been removed. Then, the origin is also strictly inside of
 * the Minkowski difference, which contains the tetrahedron, namely, the penetration depth is
 * positive. Hence, the expanding polytope algorithm is skipped in this case if the given depth is
 * zero. */
bool _zGJKPenetrate(zVec3DData *data1, zVec3DData *data2, double depth, zVec3D *c1, zVec3D *c2)
{
  zGJKSimplex s;
  zVec3D d1, d2;

  if( !_zGJK( data1, data2, c1, c2, &s ) ) return false;
  if( depth == 0 && s.n == 4 ) return true;
  return _zEPA( data1, data2, &d1, &d2, &s ) && zVec3DSqrDist( &d1, &d2 ) > depth * depth;
}

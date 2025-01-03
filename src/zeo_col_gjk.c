/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_gjk - collision checking: Gilbert-Johnson-Keerthi's algorithm.
 */

#include <zeo/zeo_col.h>

typedef struct{
  bool sw_w;  /* included in W (the smallest simplex) */
  bool sw_y;  /* included in Y (the updated simplex) */
  zVec3D w;   /* support map of Minkowski's sum */
  const zVec3D *p1; /* corresponding vertex on object 1 to the support map */
  const zVec3D *p2; /* corresponding vertex on object 2 to the support map */
  double s;   /* linear sum coefficient */
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

#ifdef DEBUG
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

#ifdef DEBUG
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

/* prion out vertices GJK simplex set W (minimum simplex) */
static void _zGJKSimplexWVertFPrint(FILE *fp, zGJKSimplex *s)
{
  int i;

  for( i=0; i<4; i++ )
    if( s->slot[i].sw_w )
      _zGJKSlotVertFPrint( fp, &s->slot[i] );
}
#endif

/* check if the specified slot is already in the previous testing simplex. */
static bool _zGJKSimplexCheckSlot(zGJKSimplex *s, zGJKSlot *slot)
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
static zVec3D *_zGJKSimplexClosest(zGJKSimplex *s, zVec3D *v)
{
  double _q[9], _c[3];
  double _a[] = { -1.0, -1.0, -1.0 }, _b[] = { -1.0 }, _l[3];
  zMatStruct q, a;
  zVecStruct c, b, l;
  zVec3D dp[3];
  double s0, cost;
  int i, j, n, n1, index[4];

  /* create index */
  for( n=0, i=0; i<4; i++ )
    if( s->slot[i].sw_y ) index[n++] = i;
  /* setup & solve QP */
  n1 = n - 1;
  zMatBufNC(&q) = _q; zMatSetSizeNC(&q,n1,n1);
  zVecBufNC(&c) = _c; zVecSetSizeNC(&c,n1);
  zMatBufNC(&a) = _a; zMatSetSizeNC(&a,1,n1);
  zVecBufNC(&b) = _b; zVecSetSizeNC(&b,1);
  zVecBufNC(&l) = _l; zVecSetSizeNC(&l,n1);
  for( i=1; i<n; i++ )
    zVec3DSub( &s->slot[index[i]].w, &s->slot[index[0]].w, &dp[i-1] );
  for( i=0; i<n1; i++ ){
    for( j=0; j<n1; j++ )
      zMatElemNC(&q,i,j) = zVec3DInnerProd(&dp[i],&dp[j]);
    zVecElemNC(&c,i) = zVec3DInnerProd(&s->slot[index[0]].w,&dp[i]);
  }
  zQPSolveLemke( &q, &c, &a, &b, &l, &cost );
  /* solve QP */
  zVec3DCopy( &s->slot[index[0]].w, v );
  s0 = 0;
  for( i=0; i<n1; i++ ){
    zVec3DCatDRC( v, zVecElemNC(&l,i), &dp[i] );
    s0 += ( s->slot[index[i+1]].s = zVecElemNC(&l,i) );
  }
  s->slot[index[0]].s = 1.0 - s0;
  return v;
}

/* minimize the testing simplex. */
static void _zGJKSimplexMinimize(zGJKSimplex *s)
{
  int i;
  zVec3D ws;

  for( s->n=0, i=0; i<4; i++ ){
    zVec3DMul( &s->slot[i].w, s->slot[i].s, &ws );
    if( zVec3DIsTiny( &ws ) ){
      s->slot[i].s = 0;
      s->slot[i].sw_w = false;
    } else{
      s->slot[i].sw_w = true;
      s->n++;
    }
  }
}

/* support map of Minkowski difference. */
static zVec3D *_zGJKSupportMap(zGJKSlot *s, zVec3DData *data1, zVec3DData *data2, zVec3D *v)
{
  zVec3D nv;

  zVec3DRev( v, &nv );
  s->p1 = zVec3DDataSupportMap( data1, &nv );
  s->p2 = zVec3DDataSupportMap( data2,   v );
  zVec3DSub( s->p1, s->p2, &s->w );
  return &s->w;
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

static double _zGJKPH3DClosest(zPH3D *ph, zVec3D *p, zVec3D *cp, int *id)
{
  int i;
  zVec3D ncp;
  double d, dmin;

  if( zPH3DFaceNum(ph) == 0 ){
    ZRUNWARN( ZEO_ERR_NOFACE );
    zVec3DCopy( p, cp );
    return 0;
  }
  dmin = zTri3DClosest( zPH3DFace(ph,0), p, cp );
  *id = 0;
  for( i=1; i<zPH3DFaceNum(ph); i++ )
    if( ( d = zTri3DClosest( zPH3DFace(ph,i), p, &ncp ) ) < dmin ){
      zVec3DCopy( &ncp, cp );
      dmin = d;
      *id = i;
    }
  return dmin;
}

zListClass( zGJKSlotList, zGJKSlotListCell, zGJKSlot );

static zGJKSlotListCell *_zGJKSlotListInsert(zGJKSlotList *sl, zGJKSlot *s)
{
  zGJKSlotListCell *sc;

  if( !( sc = zAlloc( zGJKSlotListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zVec3DCopy( &s->w, &sc->data.w );
  sc->data.p1 = s->p1;
  sc->data.p2 = s->p2;
  zListInsertHead( sl, sc );
  return sc;
}

static bool _zGJKPDAddPoint(zVec3DData *data1, zVec3DData *data2, zGJKSlotList *slist, zVec3DData *vert_data, zVec3D *v, zEdge3D *edge, zTri3D *tri)
{
  zGJKSlot  ns;

  _zGJKSupportMap( &ns, data1, data2, v );
  if( ( edge != NULL && zIsTiny( zEdge3DDistFromPoint( edge, &ns.w ) ) ) ||
      ( tri != NULL && zIsTiny( zTri3DDistFromPoint( tri, &ns.w ) ) ) )
    return false;
  _zGJKSlotListInsert( slist, &ns );
  zVec3DDataAdd( vert_data, &ns.w );
  return true;
}

static bool _zGJKPDInit(zVec3DData *data1, zVec3DData *data2, zGJKSimplex *s, zGJKSlotList *slist, zVec3DData *vert_data)
{
  int i;
  zGJKSlotListCell *sc;
  zPH3D ph;
  zTri3D tri;
  zEdge3D edge;
  zVec3D v1, v2;

  zListInit( slist );
  zVec3DDataInitList( vert_data );
  for( i=0; i<s->n; i++ )
    _zGJKSlotListInsert( slist, &s->slot[i] );
  zListForEach( slist, sc )
    zVec3DDataAdd( vert_data, &sc->data.w );

  if( s->n == 2 ){
    zEdge3DCreate( &edge, &s->slot[0].w, &s->slot[1].w );
    zVec3DOrthoSpace( zEdge3DVec(&edge), &v1, &v2 );
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, &v1, &edge, NULL ) )
      goto FAILURE;
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, &v2, &edge, NULL ) )
      goto FAILURE;
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, zVec3DRevDRC( &v1 ), &edge, NULL ) )
      goto FAILURE;
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, zVec3DRevDRC( &v2 ), &edge, NULL ) )
      goto FAILURE;
    zVec3DDataConvexHull( vert_data, &ph );
    if( !zPH3DPointIsInside( &ph, ZVEC3DZERO, -zTOL ) ){
      zPH3DDestroy( &ph );
      goto FAILURE;
    }
    zPH3DDestroy(&ph);
  } else
  if( s->n == 3 ){
    zTri3DCreate( &tri, &s->slot[0].w, &s->slot[1].w, &s->slot[2].w );
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, zTri3DNorm(&tri), NULL, &tri ) )
      goto FAILURE;
    if( !_zGJKPDAddPoint( data1, data2, slist, vert_data, zVec3DRev( zTri3DNorm(&tri), &v1 ), NULL, &tri ) )
      goto FAILURE;
  }
  return true;

 FAILURE:
  zVec3DDataDestroy( vert_data );
  zListDestroy( zGJKSlotListCell, slist );
  return false;
}

/* penetration depth */
static bool _zGJKPD(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2, zGJKSimplex *s)
{
  int i, j;
  zGJKSlot  ns;
  zGJKSlotList slist;
  zGJKSlotListCell *sc;
  zVec3D v, v_temp;
  zVec3DData vert_data;
  zPH3D ph;
  int id = 0;
  double l[3];

  if( !_zGJKPDInit( data1, data2, s, &slist, &vert_data ) ) return false;
  zVec3DZero( &v_temp );
  while( 1 ){
    zVec3DDataConvexHull( &vert_data, &ph );
    _zGJKPH3DClosest( &ph, ZVEC3DZERO, &v, &id );
    if( zVec3DEqual( &v, &v_temp ) ) break; /* success! */
    zVec3DCopy( &v, &v_temp );
    zVec3DRevDRC( &v );
    _zGJKSupportMap( &ns, data1, data2, &v );
    zListForEach( &slist, sc )
      if( zVec3DEqual( &ns.w, &sc->data.w ) ) goto BREAK;
    _zGJKSlotListInsert( &slist, &ns );
    zVec3DDataAdd( &vert_data, &ns.w );
    zPH3DDestroy( &ph );
  }
 BREAK:
  zVec3DZero( c1 );
  zVec3DZero( c2 );
  j = 0;
  zTri3DLinScale( zPH3DFace(&ph,id), &v, &l[0], &l[1], &l[2], &v_temp );
  zListForEach( &slist, sc ){
    for( i=0; i<3; i++ ){
      if( zVec3DEqual( zPH3DFaceVert(&ph,id,i), &sc->data.w ) ){
        zVec3DCatDRC( c1, l[i], sc->data.p1 );
        zVec3DCatDRC( c2, l[i], sc->data.p2 );
        j++;
      }
    }
    if( j == 3 ) break;
  }
  zPH3DDestroy( &ph );
  zVec3DDataDestroy( &vert_data );
  zListDestroy( zGJKSlotListCell, &slist );
  return true;
}

static bool _zGJKCheck(zGJKSimplex *s)
{
  zTri3D t;
  zEdge3D e;

  switch( s->n ){
  case 4: return true;
  case 3: zTri3DCreate( &t, &s->slot[0].w, &s->slot[1].w, &s->slot[2].w );
          return zTri3DPointIsOnPlane( &t, ZVEC3DZERO, zTOL ) ? true : false;
  case 2: zEdge3DCreate( &e, &s->slot[0].w, &s->slot[1].w );
          return zEdge3DPointIsOnLine( &e, ZVEC3DZERO, zTOL ) ? true : false;
  case 1: return zVec3DEqual( &s->slot[0].w, ZVEC3DZERO );
  default: ;
  }
  return false;
}

static bool _zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2, zGJKSimplex *s)
{
  zGJKSimplex _s; /* simplex */
  zGJKSlot slot;
  zVec3D proximity, *v1, *v2;
  double dv2norm = 0;

  if( s == NULL ) s = &_s;
  zVec3DDataRewind( data1 );
  zVec3DDataRewind( data2 );
  while( ( v1 = zVec3DDataFetch( data1 ) ) ){
    while( ( v2 = zVec3DDataFetch( data2 ) ) ){
      zVec3DSub( v1, v2, &proximity );
      if( !zIsTiny( ( dv2norm = zVec3DSqrNorm( &proximity ) ) ) ) break;
    }
  }
  _zGJKSimplexInit( s );
  do{
    _zGJKSupportMap( &slot, data1, data2, &proximity );
    if( _zGJKSimplexCheckSlot( s, &slot ) ||
        dv2norm - zVec3DInnerProd(&slot.w,&proximity) <= zTOL ){
      break; /* succeed */
    }
    _zGJKSimplexAddSlot( s, &slot );
    _zGJKSimplexClosest( s, &proximity );
    _zGJKSimplexMinimize( s );
    dv2norm = zVec3DSqrNorm( &proximity );
  } while( s->n < 4 );
  _zGJKPair( s, c1, c2 );
  return _zGJKCheck( s );
}

/* Gilbert-Johnson-Keerthi algorithm. */
bool zGJK(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2)
{
  return _zGJK( data1, data2, c1, c2, NULL );
}

/* GJK algorithm followed by Johnson's penetration depth */
bool zGJKDepth(zVec3DData *data1, zVec3DData *data2, zVec3D *c1, zVec3D *c2)
{
  zGJKSimplex s;

  return _zGJK( data1, data2, c1, c2, &s ) ?
    _zGJKPD( data1, data2, c1, c2, &s ) : false;
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
  zVec3D proximity, *v;
  double dv2norm = 0;

  slot.p2 = p;
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zVec3DSub( v, p, &proximity );
    if( !zIsTiny( ( dv2norm = zVec3DSqrNorm( &proximity ) ) ) ) break;
  }
  _zGJKSimplexInit( &s );
  do{
    _zGJKPointSupportMap( &slot, data, &proximity );
    if( _zGJKSimplexCheckSlot( &s, &slot ) ||
        dv2norm - zVec3DInnerProd(&slot.w,&proximity) <= zTOL ){
      break; /* succeed */
    }
    _zGJKSimplexAddSlot( &s, &slot );
    _zGJKSimplexClosest( &s, &proximity );
    _zGJKSimplexMinimize( &s );
    dv2norm = zVec3DSqrNorm( &proximity );
  } while( s.n < 4 );
  _zGJKPointClosest( &s, c );
  return _zGJKCheck( &s );
}

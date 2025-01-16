/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_voronoi2d - 2D Voronoi diagram.
 */

#include <zeo/zeo_voronoi2d.h>

/* *** 2D Delaunay triangulation *** */

/* create a 2D triangle for Delaunay triangulation */
static zDelaunayTri2D *_zDelaunayTri2DCreate(zDelaunayTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3, zDelaunayTri2D *t1, zDelaunayTri2D *t2, zDelaunayTri2D *t3)
{
  zTri2DCreate( &t->t, v1, v2, v3 );
  t->adj[0] = t1;
  t->adj[1] = t2;
  t->adj[2] = t3;
  return t;
}

/* bind two adjacent 2D triangles for Delaunay triangulation */
static int _zDelaunayTri2DBind(zDelaunayTri2D *t, int i, zDelaunayTri2D *a)
{
  int ia;
  zVec2D *pj;

  if( !( t->adj[i] = a ) ) return -1;
  pj = zTri2DVertNext( &t->t, i );
  for( ia=0; ia<3; ia++ ){
    if( zTri2DVert(&a->t,ia) == pj ){
      a->adj[ia] = t;
      return ia;
    }
  }
  return -1; /* invalid case */
}

/* find index of a reference vertex of the specified adjacent 2D triangle for Delaunay triangulation */
static int _zDelaunayTri2DFindAdj(zDelaunayTri2D *t, zDelaunayTri2D *at)
{
  int i;

  for( i=0; i<3; i++ )
    if( t->adj[i] == at ) return i;
  return -1; /* not bound */
}

#ifdef DEBUG
/* print out a 2D triangle to the current position of a file (for debug) */
static void _zDelaunayTri2DFPrint(FILE *fp, zDelaunayTri2D *t)
{
  fprintf( fp, "<DelaunayTri2D> %p\n", t );
  fprintf( fp, " vert #0: %p\n", zTri2DVert(&t->t,0) );
  fprintf( fp, " vert #1: %p\n", zTri2DVert(&t->t,1) );
  fprintf( fp, " vert #2: %p\n", zTri2DVert(&t->t,2) );
  fprintf( fp, " adj  #0: %p\n", t->adj[0] );
  fprintf( fp, " adj  #1: %p\n", t->adj[1] );
  fprintf( fp, " adj  #2: %p\n", t->adj[2] );
}
#endif

/* find a 2D triangle that includes a specified point for Delaunay triangulation */
static zDelaunayTri2DListCell *_zDelaunayTri2DListFind(zDelaunayTri2DList *tl, zVec2D *p)
{
  zDelaunayTri2DListCell *tc;

  zListForEach( tl, tc )
    if( zTri2DPointIsInside( &tc->data.t, p, zTOL ) ) return tc;
  return NULL;
}

/* add a new 2D triangle to a list for Delaunay triangulation */
static zDelaunayTri2DListCell *_zDelaunayTri2DListAdd(zDelaunayTri2DList *tl)
{
  zDelaunayTri2DListCell *tc;

  if( !( tc = zAlloc( zDelaunayTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInsertHead( tl, tc );
  return tc;
}

/* print out 2D triangles of a list to the current position of a file (for debug) */
void zDelaunayTri2DListFPrint(FILE *fp, zDelaunayTri2DList *tl)
{
  zDelaunayTri2DListCell *tc;

  zListForEach( tl, tc ){
    zVec2DValueNLFPrint( fp, zTri2DVert(&tc->data.t,0) );
    zVec2DValueNLFPrint( fp, zTri2DVert(&tc->data.t,1) );
    zVec2DValueNLFPrint( fp, zTri2DVert(&tc->data.t,2) );
    zVec2DValueNLFPrint( fp, zTri2DVert(&tc->data.t,0) );
    fprintf( fp, "\n" );
  }
}

/* stack of an edge of a 2D triangle to be legalized for Delaunay triangulation */
typedef struct{
  zDelaunayTri2D *t;
  int i; /* index of the new adjacent */
} zDelaunayPair2D;
zListClass( zDelaunayPair2DStack, zDelaunayPair2DCell, zDelaunayPair2D );

/* push an edge of a 2D triangle to be legalized on a stack for Delaunay triangulation */
static bool _zDelaunayPair2DStackPush(zDelaunayPair2DStack *ps, zDelaunayTri2D *t, int i)
{
  zDelaunayPair2DCell *pc;

  if( !t->adj[i] ) return true;
  if( !( pc = zAlloc( zDelaunayPair2DCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  pc->data.t = t;
  pc->data.i = i;
  zStackPush( ps, pc );
  return true;
}

/* legalize a pair of 2D triangles for Delaunay triangulation */
static bool _zDelaunay2DLegalizeOne(zDelaunayTri2DList *tl, zDelaunayTri2D *t, int i, zDelaunayPair2DStack *ps)
{
  zVec2D c, *pk, *pn;
  zDelaunayTri2D *a, *tj, *aj;
  int j, k, ia, ja, ka;
  double r;

  if( !( a = t->adj[i] ) ) return true;
  ia = _zDelaunayTri2DFindAdj( a, t );
  zTri2DCircumcenter( &t->t, &c );
  r = zVec2DDist( zTri2DVert( &t->t, 0 ), &c );
  pn = zTri2DVert( &a->t, ( ka = ( ia + 2 ) % 3 ) );
  if( zVec2DDist( pn, &c ) > r ) return true;
  /* flip t and a */
  pk = zTri2DVert( &t->t, ( k = ( i + 2 ) % 3 ) );
  tj = t->adj[ ( j = ( i + 1 ) % 3 ) ];
  aj = a->adj[ ( ja = ( ia + 1 ) % 3 ) ];
  zTri2DSetVert( &t->t, j,  pn );
  _zDelaunayTri2DBind( t, i, aj );
  _zDelaunayTri2DBind( t, j, a );
  zTri2DSetVert( &a->t, ja, pk );
  _zDelaunayTri2DBind( a, ia, tj );
  _zDelaunayTri2DBind( a, ja, t );
  return _zDelaunayPair2DStackPush( ps, t, i ) && _zDelaunayPair2DStackPush( ps, a, ka );
}

/* legalize pairs of 2D triangles in a stack for Delaunay triangulation */
static void _zDelaunay2DLegalize(zDelaunayTri2DList *tl, zDelaunayPair2DStack *ps)
{
  zDelaunayPair2DCell *pc;

  while( !zListIsEmpty( ps ) ){
    zStackPop( ps, &pc );
    _zDelaunay2DLegalizeOne( tl, pc->data.t, pc->data.i, ps );
    free( pc );
  }
}

/* create the outer 2D triangle that contains all points for Delaunay triangulation */
static zDelaunayTri2D *_zDelaunay2DCreateOuter(zDelaunayTri2DList *tl, zVec2DData *data, zVec2D sp[])
{
  zDisk2D d;
  zDelaunayTri2DListCell *tc;

  if( !zVec2DDataBoundingBallRecursive( data, &d, NULL ) ) return NULL;
  if( !( tc = zAlloc( zDelaunayTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInit( tl );
  zListInsertHead( tl, tc );
  zVec2DCreate( &sp[0], zDisk2DCenter(&d)->c.x, zDisk2DCenter(&d)->c.y+3*zDisk2DRadius(&d) );
  zVec2DCreate( &sp[1], zDisk2DCenter(&d)->c.x-1.5*sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-1.5*zDisk2DRadius(&d) );
  zVec2DCreate( &sp[2], zDisk2DCenter(&d)->c.x+1.5*sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-1.5*zDisk2DRadius(&d) );
  _zDelaunayTri2DCreate( &tc->data, &sp[0], &sp[1], &sp[2], NULL, NULL, NULL );
  return &tc->data;
}

/* update a list of 2D triangles Delaunay triangulation */
static bool _zDelaunay2DUpdate(zDelaunayTri2DList *tl, zVec2D *p)
{
  zDelaunayTri2DListCell *tc, *t1, *t2, *t3;
  zDelaunayPair2DStack ps;

  if( !( tc = _zDelaunayTri2DListFind( tl, p ) ) ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return false;
  }
  zListPurge( tl, tc );
  t1 = _zDelaunayTri2DListAdd( tl );
  t2 = _zDelaunayTri2DListAdd( tl );
  t3 = _zDelaunayTri2DListAdd( tl );
  if( !t1 || !t2 || !t3 ){
    zFree( t1 ); zFree( t2 ); zFree( t3 ); free( tc );
    return false;
  }
  _zDelaunayTri2DCreate( &t1->data, zTri2DVert(&tc->data.t,0), zTri2DVert(&tc->data.t,1), p, tc->data.adj[0], &t2->data, &t3->data );
  _zDelaunayTri2DBind( &t1->data, 0, tc->data.adj[0] );
  _zDelaunayTri2DCreate( &t2->data, p, zTri2DVert(&tc->data.t,1), zTri2DVert(&tc->data.t,2), &t1->data, tc->data.adj[1], &t3->data );
  _zDelaunayTri2DBind( &t2->data, 1, tc->data.adj[1] );
  _zDelaunayTri2DCreate( &t3->data, zTri2DVert(&tc->data.t,0), p, zTri2DVert(&tc->data.t,2), &t1->data, &t2->data, tc->data.adj[2] );
  _zDelaunayTri2DBind( &t3->data, 2, tc->data.adj[2] );
  free( tc );
  zListInit( &ps );
  if( !_zDelaunayPair2DStackPush( &ps, &t1->data, 0 ) ||
      !_zDelaunayPair2DStackPush( &ps, &t2->data, 1 ) ||
      !_zDelaunayPair2DStackPush( &ps, &t3->data, 2 ) ){
    zListDestroy( zDelaunayPair2DCell, &ps );
    return false;
  } else
    _zDelaunay2DLegalize( tl, &ps );
  return true;
}

/* purge the outer 2D triangle from the list for Delaunay triangulation */
static void _zDelaunay2DPurgeOuter(zDelaunayTri2DList *tl, zVec2D *sp)
{
  zDelaunayTri2DListCell *tc, *tmp;

  zListForEach( tl, tc ){
    if( zTri2DVert(&tc->data.t,0) == &sp[0] || zTri2DVert(&tc->data.t,0) == &sp[1] || zTri2DVert(&tc->data.t,0) == &sp[2] ||
        zTri2DVert(&tc->data.t,1) == &sp[0] || zTri2DVert(&tc->data.t,1) == &sp[1] || zTri2DVert(&tc->data.t,1) == &sp[2] ||
        zTri2DVert(&tc->data.t,2) == &sp[0] || zTri2DVert(&tc->data.t,2) == &sp[1] || zTri2DVert(&tc->data.t,2) == &sp[2] ){
      tmp = tc;
      tc = zListCellPrev( tc );
      zListPurge( tl, tmp );
      free( tmp );
    }
  }
}

/* 2D Delaunay triangulation */
bool zVec2DDataDelaunayTriangulate(zVec2DData *data, zDelaunayTri2DList *tl)
{
  zVec2D sp[3], *v;

  _zDelaunay2DCreateOuter( tl, data, sp );
  zVec2DDataRewind( data );
  while( ( v = zVec2DDataFetch( data ) ) )
    if( !_zDelaunay2DUpdate( tl, v ) ) return false;
  _zDelaunay2DPurgeOuter( tl, sp );
  return true;
}

/* print 2D Voronoi diagram from a set of 2D Delaunay triangles */
void zDelaunayTri2DListVoronoi2DFPrint(FILE *fp, zDelaunayTri2DList *tl)
{
  zVec2D c, c0, c1, c2;
  zDelaunayTri2DListCell *tc;

  zListForEach( tl, tc ){
    zTri2DCircumcenter( &tc->data.t, &c );
    if( tc->data.adj[0] ){
      zTri2DCircumcenter( &tc->data.adj[0]->t, &c0 );
      zVec2DValueNLFPrint( fp, &c );
      zVec2DValueNLFPrint( fp, &c0 );
      fprintf( fp, "\n" );
    }
    if( tc->data.adj[1] ){
      zTri2DCircumcenter( &tc->data.adj[1]->t, &c1 );
      zVec2DValueNLFPrint( fp, &c );
      zVec2DValueNLFPrint( fp, &c1 );
      fprintf( fp, "\n" );
    }
    if( tc->data.adj[2] ){
      zTri2DCircumcenter( &tc->data.adj[2]->t, &c2 );
      zVec2DValueNLFPrint( fp, &c );
      zVec2DValueNLFPrint( fp, &c2 );
      fprintf( fp, "\n" );
    }
  }
}

/* *** 2D Voronoi diagram *** */

/* initialize a list of triangles by 2D Delaunay triangulation for 2D Voronoi diagram */
static bool _zVoronoi2DListInit(zVoronoi2DList *vl, zVec2DData *data, zDelaunayTri2DList *tl, zVec2D sp[])
{
  zVec2D *v;
  zVoronoi2DCell *vc;

  zListInit( vl );
  _zDelaunay2DCreateOuter( tl, data, sp );
  zVec2DDataRewind( data );
  while( ( v = zVec2DDataFetch( data ) ) ){
    if( !_zDelaunay2DUpdate( tl, v ) ) return false;
    if( !( vc = zAlloc( zVoronoi2DCell, 1 ) ) ){
      ZALLOCERROR();
      return false;
    }
    vc->data.seed = v;
    zListInit( &vc->data.loop );
    zListInsertHead( vl, vc );
  }
  return true;
}

/* destroy a list of 2D Voronoi cells */
void zVoronoi2DListDestroy(zVoronoi2DList *vl)
{
  zVoronoi2DCell *vc;

  while( !zListIsEmpty( vl ) ){
    zListDeleteHead( vl, &vc );
    zListDestroy( zVoronoi2DLoopCell, &vc->data.loop );
    zFree( vc );
  }
}

/* find a 2D Voronoi cell with the specified seed */
static zVoronoi2DCell *_zVoronoi2DListFind(zVoronoi2DList *vl, zVec2D *p)
{
  zVoronoi2DCell *vc;

  zListForEach( vl, vc )
    if( vc->data.seed == p ) return vc;
  return NULL;
}

/* add a node of a 2D Voronoi cell */
static zVoronoi2DLoopCell *_zVoronoi2DCellAddLoop(zVoronoi2DData *vd, zDelaunayTri2D *t, zDelaunayTri2D *a, zVec2D *node)
{
  zVoronoi2DLoopCell *lc;

  if( !( lc = zAlloc( zVoronoi2DLoopCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  lc->data.t[0] = t;
  lc->data.t[1] = a;
  zVec2DCopy( node, &lc->data.node );
  zListInsertHead( &vd->loop, lc );
  return lc;
}

/* close the outer loop of a 2D Voronoi cell */
static void _zVoronoi2DListLoopClose(zVoronoi2DList *vl)
{
  zVoronoi2DCell *vc;
  zVoronoi2DLoopCell *lc1, *lc2;

  zListForEach( vl, vc )
    zListForEach( &vc->data.loop, lc1 )
      for( lc2=zListCellNext(lc1); lc2!=zListRoot(&vc->data.loop); lc2=zListCellNext(lc2) )
        if( lc2->data.t[0] == lc1->data.t[1] ){
          zListPurge( &vc->data.loop, lc2 );
          zListCellInsertNext( lc1, lc2 );
        }
}

/* convert a vertex of a 2D triangle to a node of a 2D Voronoi cell */
static bool _zVoronoi2DListFromDelaunayTri2D(zVoronoi2DList *vl, zDelaunayTri2DListCell *tc, int i, zVec2D *node)
{
  zVoronoi2DCell *vc;

  if( !tc->data.adj[i] ) return true;
  if( !( vc = _zVoronoi2DListFind( vl, zTri2DVert(&tc->data.t,i) ) ) ) return true;
  return _zVoronoi2DCellAddLoop( &vc->data, &tc->data, tc->data.adj[i], node ) ? true : false;
}

/* convert a list of 2D Delaunay triangles to a list of 2D Voronoi cells */
static zVoronoi2DList *_zVoronoi2DListFromDelaunayTri2DList(zVoronoi2DList *vl, zDelaunayTri2DList *tl)
{
  zDelaunayTri2DListCell *tc;
  zVec2D node;

  zListForEach( tl, tc ){
    zTri2DCircumcenter( &tc->data.t, &node );
    if( !_zVoronoi2DListFromDelaunayTri2D( vl, tc, 0, &node ) ) return NULL;
    if( !_zVoronoi2DListFromDelaunayTri2D( vl, tc, 1, &node ) ) return NULL;
    if( !_zVoronoi2DListFromDelaunayTri2D( vl, tc, 2, &node ) ) return NULL;
  }
  _zVoronoi2DListLoopClose( vl );
  return vl;
}

/* print a list of 2D Voronoi cells */
void zVoronoi2DListFPrint(FILE *fp, zVoronoi2DList *vl)
{
  zVoronoi2DCell *vc;
  zVoronoi2DLoopCell *lc;

  zListForEach( vl, vc ){
    zListForEach( &vc->data.loop, lc )
      zVec2DValueNLFPrint( fp, &lc->data.node );
    zVec2DValueNLFPrint( fp, &zListTail(&vc->data.loop)->data.node );
    fprintf( fp, "\n" );
  }
}

/* 2D Voronoi diagram */
bool zVec2DDataVoronoiDiagram(zVec2DData *data, zVoronoi2DList *vl)
{
  zVec2D sp[3];
  zDelaunayTri2DList tl;
  bool ret = true;

  if( _zVoronoi2DListInit( vl, data, &tl, sp ) ){
    _zVoronoi2DListFromDelaunayTri2DList( vl, &tl );
  } else
    ret = false;
  zDelaunayTri2DListDestroy( &tl );
  return ret;
}

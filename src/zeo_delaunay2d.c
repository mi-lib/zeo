/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_delaunay2d - 2D Delaunay triangularization.
 */

#include <zeo/zeo_delaunay2d.h>

/* create a 2D triangle for Delaunay triangularization */
static zDTTri2D *_zDTTri2DCreate(zDTTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3, zDTTri2D *t1, zDTTri2D *t2, zDTTri2D *t3)
{
  zTri2DCreate( &t->t, v1, v2, v3 );
  t->adj[0] = t1;
  t->adj[1] = t2;
  t->adj[2] = t3;
  return t;
}

/* bind two adjacent 2D triangles for Delaunay triangularization */
static int _zDTTri2DBind(zDTTri2D *t, int i, zDTTri2D *a)
{
  register int ia;
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

/* find index of a reference vertex of the specified adjacent 2D triangle for Delaunay triangularization */
static int _zDTTri2DFindAdj(zDTTri2D *t, zDTTri2D *at)
{
  register int i;

  for( i=0; i<3; i++ )
    if( t->adj[i] == at ){
      return i;
    }
  return -1; /* not bound */
}

#ifdef DEBUG
/* print out a 2D triangle to the current position of a file (for debug) */
static void _zDTTri2DFPrint(FILE *fp, zDTTri2D *t)
{
  fprintf( fp, "<DTTri2D> %p\n", t );
  fprintf( fp, " vert #0: %p\n", zTri2DVert(&t->t,0) );
  fprintf( fp, " vert #1: %p\n", zTri2DVert(&t->t,1) );
  fprintf( fp, " vert #2: %p\n", zTri2DVert(&t->t,2) );
  fprintf( fp, " adj  #0: %p\n", t->adj[0] );
  fprintf( fp, " adj  #1: %p\n", t->adj[1] );
  fprintf( fp, " adj  #2: %p\n", t->adj[2] );
}
#endif

/* find a 2D triangle that includes a specified point for Delaunay triangularization */
static zDTTri2DListCell *_zDTTri2DListFind(zDTTri2DList *tl, zVec2D *p)
{
  zDTTri2DListCell *tc;

  zListForEach( tl, tc )
    if( zTri2DPointIsInside( &tc->data.t, p, true ) ) return tc;
  return NULL;
}

/* add a new 2D triangle to a list for Delaunay triangularization */
static zDTTri2DListCell *_zDTTri2DListAdd(zDTTri2DList *tl)
{
  zDTTri2DListCell *tc;

  if( !( tc = zAlloc( zDTTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInsertHead( tl, tc );
  return tc;
}

/* print out 2D triangles of a list to the current position of a file (for debug) */
void zDTTri2DListFPrint(FILE *fp, zDTTri2DList *tl)
{
  zDTTri2DListCell *tc;

  zListForEach( tl, tc ){
    zVec2DDataNLFPrint( fp, zTri2DVert(&tc->data.t,0) );
    zVec2DDataNLFPrint( fp, zTri2DVert(&tc->data.t,1) );
    zVec2DDataNLFPrint( fp, zTri2DVert(&tc->data.t,2) );
    zVec2DDataNLFPrint( fp, zTri2DVert(&tc->data.t,0) );
    fprintf( fp, "\n" );
  }
}

/* stack of an edge of a 2D triangle to be legalized for Delaunay triangularization */
typedef struct{
  zDTTri2D *t;
  int i; /* index of the new adjacent */
} zDTPair2D;
zListClass( zDTPair2DStack, zDTPair2DCell, zDTPair2D );

/* push an edge of a 2D triangle to be legalized on a stack for Delaunay triangularization */
static bool _zDTPair2DStackPush(zDTPair2DStack *ps, zDTTri2D *t, int i)
{
  zDTPair2DCell *pc;

  if( !t->adj[i] ) return true;
  if( !( pc = zAlloc( zDTPair2DCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  pc->data.t = t;
  pc->data.i = i;
  zStackPush( ps, pc );
  return true;
}

/* legalize a pair of 2D triangles for Delaunay triangularization */
static bool _zDT2DLegalizeOne(zDTTri2DList *tl, zDTTri2D *t, int i, zDTPair2DStack *ps)
{
  zVec2D c, *pk, *pn;
  zDTTri2D *a, *tj, *aj;
  int j, k, ia, ja, ka;
  double r;

  if( !( a = t->adj[i] ) ) return true;
  ia = _zDTTri2DFindAdj( a, t );
  zTri2DCircumcenter( &t->t, &c );
  r = zVec2DDist( zTri2DVert( &t->t, 0 ), &c );
  pn = zTri2DVert( &a->t, ( ka = ( ia + 2 ) % 3 ) );
  if( zVec2DDist( pn, &c ) > r ) return true;
  /* flip t and a */
  pk = zTri2DVert( &t->t, ( k = ( i + 2 ) % 3 ) );
  tj = t->adj[ ( j = ( i + 1 ) % 3 ) ];
  aj = a->adj[ ( ja = ( ia + 1 ) % 3 ) ];
  zTri2DSetVert( &t->t, j,  pn );
  _zDTTri2DBind( t, i, aj );
  _zDTTri2DBind( t, j, a );
  zTri2DSetVert( &a->t, ja, pk );
  _zDTTri2DBind( a, ia, tj );
  _zDTTri2DBind( a, ja, t );
  return _zDTPair2DStackPush( ps, t, i ) && _zDTPair2DStackPush( ps, a, ka );
}

/* legalize pairs of 2D triangles in a stack for Delaunay triangularization */
static void _zDT2DLegalize(zDTTri2DList *tl, zDTPair2DStack *ps)
{
  zDTPair2DCell *pc;

  while( !zListIsEmpty( ps ) ){
    zStackPop( ps, &pc );
    _zDT2DLegalizeOne( tl, pc->data.t, pc->data.i, ps );
    free( pc );
  }
}

/* create the outer 2D triangle that contains all points for Delaunay triangularization */
static zDTTri2D *_zDT2DCreateOuter(zDTTri2DList *tl, zVec2DList *pl, zVec2D sp[])
{
  zDisk2D d;
  zDTTri2DListCell *tc;

  if( !zBDisk2DPL( &d, pl, NULL ) ) return NULL;
  if( !( tc = zAlloc( zDTTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInit( tl );
  zListInsertHead( tl, tc );
  zVec2DCreate( &sp[0], zDisk2DCenter(&d)->c.x, zDisk2DCenter(&d)->c.y+3*zDisk2DRadius(&d) );
  zVec2DCreate( &sp[1], zDisk2DCenter(&d)->c.x-1.5*sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-1.5*zDisk2DRadius(&d) );
  zVec2DCreate( &sp[2], zDisk2DCenter(&d)->c.x+1.5*sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-1.5*zDisk2DRadius(&d) );
  _zDTTri2DCreate( &tc->data, &sp[0], &sp[1], &sp[2], NULL, NULL, NULL );
  return &tc->data;
}

/* update a list of 2D triangles Delaunay triangularization */
static bool _zDT2DUpdate(zDTTri2DList *tl, zVec2D *p)
{
  zDTTri2DListCell *tc, *t1, *t2, *t3;
  zDTPair2DStack ps;

  if( !( tc = _zDTTri2DListFind( tl, p ) ) ){
    ZRUNERROR( "fatal error" );
    return false;
  }
  zListPurge( tl, tc );
  t1 = _zDTTri2DListAdd( tl );
  t2 = _zDTTri2DListAdd( tl );
  t3 = _zDTTri2DListAdd( tl );
  if( !t1 || !t2 || !t3 ){
    zFree( t1 ); zFree( t2 ); zFree( t3 ); free( tc );
    return false;
  }
  _zDTTri2DCreate( &t1->data, zTri2DVert(&tc->data.t,0), zTri2DVert(&tc->data.t,1), p, tc->data.adj[0], &t2->data, &t3->data );
  _zDTTri2DBind( &t1->data, 0, tc->data.adj[0] );
  _zDTTri2DCreate( &t2->data, p, zTri2DVert(&tc->data.t,1), zTri2DVert(&tc->data.t,2), &t1->data, tc->data.adj[1], &t3->data );
  _zDTTri2DBind( &t2->data, 1, tc->data.adj[1] );
  _zDTTri2DCreate( &t3->data, zTri2DVert(&tc->data.t,0), p, zTri2DVert(&tc->data.t,2), &t1->data, &t2->data, tc->data.adj[2] );
  _zDTTri2DBind( &t3->data, 2, tc->data.adj[2] );
  free( tc );
  zListInit( &ps );
  if( !_zDTPair2DStackPush( &ps, &t1->data, 0 ) ||
      !_zDTPair2DStackPush( &ps, &t2->data, 1 ) ||
      !_zDTPair2DStackPush( &ps, &t3->data, 2 ) ){
    zListDestroy( zDTPair2DCell, &ps );
    return false;
  } else
    _zDT2DLegalize( tl, &ps );
  return true;
}

/* purge the outer 2D triangle from the list for Delaunay triangularization */
static void _zDT2DPurgeOuter(zDTTri2DList *tl, zVec2D *sp)
{
  zDTTri2DListCell *tc, *tmp;

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

/* 2D Delaunay triangularization */
bool zDelaunay2D(zVec2DList *pl, zDTTri2DList *tl)
{
  zVec2DListCell *pc;
  zVec2D sp[3];

  _zDT2DCreateOuter( tl, pl, sp );
  zListForEach( pl, pc )
    if( !_zDT2DUpdate( tl, pc->data ) ) return false;
  _zDT2DPurgeOuter( tl, sp );
  return true;
}

#include <zeo/zeo_bv2d.h>

zVec2DAddrList *zVec2DAddrListCopy(zVec2DAddrList *al, zVec2DList *vl)
{
  zVec2DAddr *ac;
  zVec2DListCell *vc;

  zListInit( al );
  zListForEach( vl, vc ){
    if( !( ac = zAlloc( zVec2DAddr, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    ac->data = vc->data;
    zListInsertHead( al, ac );
  }
  return al;
}

typedef struct _zDTTri2D{
  zTri2D t;
  struct _zDTTri2D *adj[3];
} zDTTri2D;
zListClass( zDTTri2DList, zDTTri2DListCell, zDTTri2D );

static zDTTri2D *_zDTTri2DCreate(zDTTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3, zDTTri2D *t1, zDTTri2D *t2, zDTTri2D *t3)
{
  zTri2DCreate( &t->t, v1, v2, v3 );
  t->adj[0] = t1;
  t->adj[1] = t2;
  t->adj[2] = t3;
  return t;
}

static int _zDTTri2DBind(zDTTri2D *at, zDTTri2D *t_org, zDTTri2D *t_new)
{
  register int i;

  if( !at ) return -1;
  for( i=0; i<3; i++ ){
    if( at->adj[i] == t_org ){
      at->adj[i] = t_new;
      return i;
    }
  }
  return -1; /* invalid case */
}

static int _zDTTri2DFindAdj(zDTTri2D *t, zDTTri2D *at)
{
  register int i;

  for( i=0; i<3; i++ )
    if( at->adj[i] == t ) return i;
  return -1; /* not bound */
}

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

static zDTTri2DListCell *_zDTTri2DListFind(zDTTri2DList *tl, zVec2D *p)
{
  zDTTri2DListCell *tc;

  zListForEach( tl, tc )
    if( zTri2DPointIsInside( &tc->data.t, p, true ) ) return tc;
  return NULL;
}

static zDTTri2DListCell *_zDTTri2DListCellAlloc(zDTTri2DList *tl)
{
  zDTTri2DListCell *tc;

  if( !( tc = zAlloc( zDTTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInsertHead( tl, tc );
  return tc;
}

static void _zDTTri2DListCellCreate(zDTTri2DListCell *tc, zVec2D *v1, zVec2D *v2, zVec2D *v3, zDTTri2D *a1, zDTTri2D *a2, zDTTri2D *a3, zDTTri2D *t_org, int i)
{
  _zDTTri2DCreate( &tc->data, v1, v2, v3, a1, a2, a3 );
  _zDTTri2DBind( t_org->adj[i], t_org, &tc->data );
}

/* for debug */
static void _zDTTri2DListFPrint(FILE *fp, zDTTri2DList *tl)
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

static void _zDTTri2DListPrintFile(zDTTri2DList *tl)
{
  FILE *fp;

  fp = fopen( "t", "w" );
  _zDTTri2DListFPrint( fp, tl );
  fclose( fp );
}

typedef struct{
  zDTTri2D *t;
  int i; /* index of the new adjacent */
} zDTPair2D;
zListClass( zDTPair2DStack, zDTPair2DCell, zDTPair2D );

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

static bool _zDT2DTestFlipOne(zDTTri2DList *tl, zDTTri2D *t, int i, zDTPair2DStack *ps)
{
  zVec2D c, *pi, *pj, *pk, *pn;
  zDTTri2D *a, *tj, *tk, *aj, *ak;
  int j, k, ia, ja, ka;
  double r;

  if( ( ia = _zDTTri2DFindAdj( t, ( a = t->adj[i] ) ) ) < 0 ) return true;
  zTri2DCircumcenter( &t->t, &c );
  r = zVec2DDist( zTri2DVert( &t->t, 0 ), &c );
  pi = zTri2DVert( &t->t, i );
  pj = zTri2DVert( &t->t, ( j = ( i + 1 ) % 3 ) );
  pk = zTri2DVert( &t->t, ( k = ( i + 2 ) % 3 ) );
  pn = zTri2DVert( &a->t, ( ka = ( ia + 2 ) % 3 ) );
  if( zVec2DDist( pn, &c ) > r ) return true;
  tj = t->adj[j];
  tk = t->adj[k];
  aj = a->adj[ ( ja = ( ia + 1 ) % 3 ) ];
  ak = a->adj[ka];
_zDTTri2DFPrint( stderr, t );
_zDTTri2DFPrint( stderr, a );
eprintf( "flip!\n" );
  _zDTTri2DCreate( t, pi, pn, pk, aj, a, tk );
  _zDTTri2DCreate( a, pj, pk, pn, tj, t, ak );
_zDTTri2DFPrint( stderr, t );
_zDTTri2DFPrint( stderr, a );
_zDTTri2DListPrintFile( tl );
getchar();
  _zDTTri2DBind( t, a, aj );
  _zDTTri2DBind( a, t, tj );
  return _zDTPair2DStackPush( ps, t, 0 ) &&
         _zDTPair2DStackPush( ps, t, 2 ) &&
         _zDTPair2DStackPush( ps, a, 0 ) &&
         _zDTPair2DStackPush( ps, a, 2 );
}

static void _zDT2DTestFlip(zDTTri2DList *tl, zDTPair2DStack *ps)
{
  zDTPair2DCell *pc;

  while( !zListIsEmpty( ps ) ){
    zStackPop( ps, &pc );
    _zDT2DTestFlipOne( tl, pc->data.t, pc->data.i, ps );
    free( pc );
  }
}

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
  zVec2DCreate( &sp[0], zDisk2DCenter(&d)->c.x, zDisk2DCenter(&d)->c.y+2*zDisk2DRadius(&d) );
  zVec2DCreate( &sp[1], zDisk2DCenter(&d)->c.x-sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  zVec2DCreate( &sp[2], zDisk2DCenter(&d)->c.x+sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  _zDTTri2DCreate( &tc->data, &sp[0], &sp[1], &sp[2], NULL, NULL, NULL );
  return &tc->data;
}

static bool _zDT2DUpdateOne(zDTTri2DList *tl, zVec2D *p)
{
  zDTTri2DListCell *tc, *t1, *t2, *t3;
  zDTPair2DStack ps;

  if( !( tc = _zDTTri2DListFind( tl, p ) ) ){
    ZRUNERROR( "fatal error" );
    return false;
  }
  zListPurge( tl, tc );
  t1 = _zDTTri2DListCellAlloc( tl );
  t2 = _zDTTri2DListCellAlloc( tl );
  t3 = _zDTTri2DListCellAlloc( tl );
  if( !t1 || !t2 || !t3 ){
    zFree( t1 ); zFree( t2 ); zFree( t3 ); free( tc );
    return false;
  }
  _zDTTri2DListCellCreate( t1, zTri2DVert(&tc->data.t,0), zTri2DVert(&tc->data.t,1), p, tc->data.adj[0], &t2->data, &t3->data, &tc->data, 0 );
  _zDTTri2DListCellCreate( t2, p, zTri2DVert(&tc->data.t,1), zTri2DVert(&tc->data.t,2), &t1->data, tc->data.adj[1], &t3->data, &tc->data, 1 );
  _zDTTri2DListCellCreate( t3, zTri2DVert(&tc->data.t,0), p, zTri2DVert(&tc->data.t,2), &t1->data, &t2->data, tc->data.adj[2], &tc->data, 2 );
eprintf("add a new point.\n");
_zDTTri2DListPrintFile( tl );
getchar();
  free( tc );
  zListInit( &ps );
  if( !_zDTPair2DStackPush( &ps, &t1->data, 0 ) ||
      !_zDTPair2DStackPush( &ps, &t2->data, 1 ) ||
      !_zDTPair2DStackPush( &ps, &t3->data, 2 ) ){
    zListDestroy( zDTPair2DCell, &ps );
    return false;
  } else
    _zDT2DTestFlip( tl, &ps );
  return true;
}

bool zDelauney2D(zVec2DList *pl, zDTTri2DList *tl)
{
  zVec2DListCell *pc;
  zVec2D sp[3];

  _zDT2DCreateOuter( tl, pl, sp );
  zListForEach( pl, pc ){
    if( !_zDT2DUpdateOne( tl, pc->data ) ){
      ZRUNERROR( "gyahyon!" );
      return false;
    }
  }
  _zDTTri2DListPrintFile( tl );
  return true;
}



void generate_points(FILE *fp, zVec2DList *pl, int n)
{
  register int i;
  zVec2D p;

  zListInit( pl );
  for( i=0; i<n; i++ ){
    zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
    zVec2DListAdd( pl, &p );
    zVec2DDataFPrint( fp, &p );
    fprintf( fp, "\n" );
  }
}

#define N  20

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zDTTri2DList tl;
  FILE *fp;

  zRandInit();
  fp = fopen( "v", "w" );
  generate_points( fp, &pl, N );
  fclose( fp );

  zDelauney2D( &pl, &tl );

  zVec2DListDestroy( &pl );
  zListDestroy( zDTTri2DListCell, &tl );
  return 0;
}

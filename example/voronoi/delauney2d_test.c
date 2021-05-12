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

#define zTri2DVertNext(t,i)  zTri2DVert( t, ( (i)+1 ) % 3 )

/* check if a point is inside of a triangle. */
bool zTri2DPointIsInside(zTri2D *t, zVec2D *v, bool rim)
{
  register int i;
  zVec2D e, tmp;
  double d[3];

  for( i=0; i<3; i++ ){
    if( zVec2DIsTiny( zVec2DSub( v, zTri2DVert(t,i), &tmp ) ) ) return rim; /* coincide with a vertex */
    zVec2DSub( zTri2DVertNext(t,i), zTri2DVert(t,i), &e );
    d[i] = _zVec2DOuterProd( &e, &tmp );
  }
  return rim ? ( d[0] >= 0 && d[1] >= 0 && d[2] >= 0 ) : ( d[0] > zTOL && d[1] > zTOL && d[2] > zTOL );
}

typedef struct _zDTTri2D{
  zTri2D t;
  struct _zDTTri2D *adj[3];
  zVec2DAddrList vl;
} zDTTri2D;

zListClass( zDTTri2DList, zDTTri2DListCell, zDTTri2D );

static zDTTri2D *_zDTTri2DCreate(zDTTri2D *tri, zVec2D *v1, zVec2D *v2, zVec2D *v3, zDTTri2D *t1, zDTTri2D *t2, zDTTri2D *t3)
{
  zTri2DCreate( &tri->t, v1, v2, v3 );
  tri->adj[0] = t1;
  tri->adj[1] = t2;
  tri->adj[2] = t3;
  zListInit( &tri->vl );
  return tri;
}

static bool _zDTTri2DTestFlip(zDTTri2D *t, zDTTri2D *adj_org, zDTTri2D *adj_new, int i, int j, int k)
{
  int i2, j2, k2;

  if( t->adj[0] == adj_org ){
    t->adj[0] = adj_new; i2 = 0; j2 = 1; k2 = 2;
  } else
  if( t->adj[1] == adj_org ){
    t->adj[1] = adj_new; i2 = 1; j2 = 2; k2 = 0;
  } else
  if( t->adj[2] == adj_org ){
    t->adj[2] = adj_new; i2 = 2; j2 = 0; k2 = 1;
  } else
    return false;
  return true;
}

static zDTTri2D *_zDT2DCreateOuter(zDTTri2DList *tl, zVec2DList *pl)
{
  zDisk2D d;
  zVec2D v1, v2, v3;
  zDTTri2DListCell *tc;

  if( !zBDisk2DPL( &d, pl, NULL ) ) return NULL;
  if( !( tc = zAlloc( zDTTri2DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zListInsertHead( tl, tc );
  zVec2DCreate( &v1, zDisk2DCenter(&d)->c.x, zDisk2DCenter(&d)->c.y+2*zDisk2DRadius(&d) );
  zVec2DCreate( &v2, zDisk2DCenter(&d)->c.x-sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  zVec2DCreate( &v3, zDisk2DCenter(&d)->c.x+sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  zVec2DListAdd( pl, &v1 );
  zVec2DListAdd( pl, &v2 );
  zVec2DListAdd( pl, &v3 );
  _zDTTri2DCreate( &tc->data, &v1, &v2, &v3, NULL, NULL, NULL );
  if( !zVec2DAddrListCopy( &tc->data.vl, pl ) ){
    zFree( tc );
    return NULL;
  }
  return &tc->data;
}

static bool _zDT2DReassign3(zDTTri2D *t1, zDTTri2D *t2, zDTTri2D *t3, zVec2DAddrList *vl)
{
  zVec2DAddr *ac;

  while( !zListIsEmpty( vl ) ){
    zListDeleteHead( vl, &ac );
    if( zTri2DPointIsInside( &t1->t, ac->data, true ) )
      zListInsertHead( &t1->vl, ac );
    else
    if( zTri2DPointIsInside( &t2->t, ac->data, true ) )
      zListInsertHead( &t2->vl, ac );
    else
    if( zTri2DPointIsInside( &t3->t, ac->data, true ) )
      zListInsertHead( &t3->vl, ac );
    else{
      ZRUNERROR( "fatal error" );
      free( ac );
      return false;
    }
  }
  return true;
}

static bool _zDT2DUpdate(zDTTri2DList *tl, zDTTri2DListCell *tc)
{
  zDTTri2DListCell *t1, *t2, *t3;
  zVec2DAddr *ac;

  t1 = zAlloc( zDTTri2DListCell, 1 );
  t2 = zAlloc( zDTTri2DListCell, 1 );
  t3 = zAlloc( zDTTri2DListCell, 1 );
  if( !t1 || !t2 || !t3 ){
    zFree( t1 ); zFree( t2 ); zFree( t3 );
    return false;
  }
  zListPurge( tl, tc );
  zListDeleteTail( &tc->data.vl, &ac );
  _zDTTri2DCreate( &t1->data, zTri2DVert(&tc->data.t,0), zTri2DVert(&tc->data.t,1), ac->data, tc->data.adj[0], &t2->data, &t3->data );
  _zDTTri2DCreate( &t2->data, ac->data, zTri2DVert(&tc->data.t,1), zTri2DVert(&tc->data.t,2), &t1->data, tc->data.adj[1], &t3->data );
  _zDTTri2DCreate( &t3->data, zTri2DVert(&tc->data.t,0), ac->data, zTri2DVert(&tc->data.t,2), &t1->data, &t2->data, tc->data.adj[2] );
  _zDT2DReassign3( &t1->data, &t2->data, &t3->data, &tc->data.vl );
  zListInsertHead( tl, t1 );
  zListInsertHead( tl, t2 );
  zListInsertHead( tl, t3 );
  if( tc->data.adj[0] ) _zDTTri2DTestFlip( tc->data.adj[0], &tc->data, &t1->data, 0, 1, 2 );
  if( tc->data.adj[1] ) _zDTTri2DTestFlip( tc->data.adj[1], &tc->data, &t2->data, 1, 2, 0 );
  if( tc->data.adj[2] ) _zDTTri2DTestFlip( tc->data.adj[2], &tc->data, &t3->data, 2, 0, 1 );
  if( !zListIsEmpty( &tc->data.vl ) ){
    ZRUNERROR( "fatal error" );
    zVec2DAddrListDestroy( &tc->data.vl );
  }
  free( tc );
  free( ac );
  if( !zListIsEmpty( &t1->data.vl ) ) _zDT2DUpdate( tl, t1 );
  if( !zListIsEmpty( &t2->data.vl ) ) _zDT2DUpdate( tl, t2 );
  if( !zListIsEmpty( &t3->data.vl ) ) _zDT2DUpdate( tl, t3 );
  return true;
}

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

#define N  100

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zDTTri2DList tl;
  FILE *fp;

  zRandInit();
  fp = fopen( "v", "w" );
  generate_points( fp, &pl, N );
  fclose( fp );

  zListInit( &tl );
  _zDT2DCreateOuter( &tl, &pl );
  _zDT2DUpdate( &tl, zListHead(&tl) );

  fp = fopen( "t", "w" );
  zDTTri2DListFPrint( fp, &tl );
  fclose( fp );

  zVec2DListDestroy( &pl );
  zListDestroy( zDTTri2DListCell, &tl );
  return 0;
}

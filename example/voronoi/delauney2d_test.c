#include <zeo/zeo_bv2d.h>

typedef struct _zDTTri2D{
  zTri2D t;
  zTri2D *adj[3];
  zVec2DAddrList vl;
} zDTTri2D;

zListClass( zDTTri2DList, zDTTri2DListCell, zDTTri2D );

zVec2DAddrList *zVec2DAddrListCopy(zVec3DAddrList *al, zVec3DList *vl)
{
  zVec3DAddr *ac;
  zVec3DListCell *vc;

  zListInit( al );
  zListForEach( vl, vc ){
    if( !( ac = zAlloc( zVec3DAddr, 1 ) ) ){
      ZALLOCERROR();
      return NULL;
    }
    ac->data = vc->data;
    zListInsertHead( al, ac );
  }
  return al;
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
  if( !zVec2DAddrListCopy( &tc->data.vl, pl ) ){
    zFree( tc );
    return NULL;
  }
  zListInsertHead( tl, tc );
  zVec2DCreate( &v1, zDisk2DCenter(&d)->c.x, zDisk2DCenter(&d)->c.y+2*zDisk2DRadius(&d) );
  zVec2DCreate( &v2, zDisk2DCenter(&d)->c.x-sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  zVec2DCreate( &v3, zDisk2DCenter(&d)->c.x+sqrt(3)*zDisk2DRadius(&d), zDisk2DCenter(&d)->c.y-zDisk2DRadius(&d) );
  zVec2DListAdd( pl, &v1 );
  zVec2DListAdd( pl, &v2 );
  zVec2DListAdd( pl, &v3 );
  zTri2DCreate( &tc->data.t, &v1, &v2, &v3 );
  tc->data.adj[0] = tc->data.adj[1] = tc->data.adj[2] = NULL;
  return &tc->data;
}

static bool _zDT2DUpdate(zDTTri2DList *tl, zDTTri2DListCell *tc)
{
  zVec3DAddrList *ac;

  zListDeleteHead( &tc->data.vl, &ac );
}

void zDTTri2DListFPrint(FILE *fp, zDTTri2DList *tl)
{
  zDTTri2DListCell *tc;

  zListForEach( tl, tc ){
    zVec2DDataFPrint( fp, zTri2DVert(&tc->data.t,0) ); fprintf( fp, "\n" );
    zVec2DDataFPrint( fp, zTri2DVert(&tc->data.t,1) ); fprintf( fp, "\n" );
    zVec2DDataFPrint( fp, zTri2DVert(&tc->data.t,2) ); fprintf( fp, "\n" );
    zVec2DDataFPrint( fp, zTri2DVert(&tc->data.t,0) ); fprintf( fp, "\n\n" );
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

  fp = fopen( "t", "w" );
  zDTTri2DListFPrint( fp, &tl );
  fclose( fp );

  zVec2DListDestroy( &pl );
  zListDestroy( zDTTri2DListCell, &tl );
  return 0;
}

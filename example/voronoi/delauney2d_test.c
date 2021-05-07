#include <zeo/zeo_bv2d.h>

zListClass( zTri2DList, zTri2DListCell, zTri2D );

static zTri2D *_zDT2DCreateOuter(zTri2DList *tl, zVec2DList *pl)
{
  zDisk2D d;
  zVec2D v1, v2, v3;
  zTri2DListCell *tc;

  if( !zBDiskPL( &d, pl, NULL ) ) return NULL;
  if( !( tc = zAlloc( zTri2DListCell, 1 ) ) ){
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
  return zTri2DCreate( &tc->data, &v1, &v2, &v3 );
}

#define N  100

int main(int argc, char *argv[])
{
  zVec2DList pl;
  zVec2D p;
  register int i;
  FILE *fp;
  zTri2DList tl;

  zRandInit();
  fp = fopen( "v", "w" );
  zListInit( &pl );
  for( i=0; i<N; i++ ){
    zVec2DCreate( &p, zRandF(-10,10), zRandF(-10,10) );
    zVec2DListAdd( &pl, &p );
    zVec2DDataFPrint( fp, &p );
    fprintf( fp, "\n" );
  }
  fclose( fp );

  zListInit( &tl );
  _zDT2DCreateOuter( &tl, &pl );
  fp = fopen( "t", "w" );
  zVec2DDataFPrint( fp, zTri2DVert(&zListHead(&tl)->data,0) ); fprintf( fp, "\n" );
  zVec2DDataFPrint( fp, zTri2DVert(&zListHead(&tl)->data,1) ); fprintf( fp, "\n" );
  zVec2DDataFPrint( fp, zTri2DVert(&zListHead(&tl)->data,2) ); fprintf( fp, "\n" );
  zVec2DDataFPrint( fp, zTri2DVert(&zListHead(&tl)->data,0) ); fprintf( fp, "\n" );
  fclose( fp );

  zVec2DListDestroy( &pl );
  zListDestroy( zTri2DListCell, &tl );
  return 0;
}

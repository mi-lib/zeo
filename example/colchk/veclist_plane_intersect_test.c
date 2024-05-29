#include <zeo/zeo_col.h>
#include <zeo/zeo_bv2d.h>

/* intersection between a loop of vertices and 3D plane */
int intersect_vlist_plane(zVec3DList *vl, zPlane3D *p, zVec3D ip[])
{
  zEdge3D e;
  zVec3DListCell *cp1, *cp2;
  int n = 0;

  cp1 = zListHead(vl);
  zListForEach( vl, cp2 ){
    zEdge3DCreate( &e, cp1->data, cp2->data );
    if( zIntersectEdgePlane3D( &e, p, &ip[n] ) ) n++;
    cp1 = cp2;
  }
  return n;
}

#define N 100

int main(void)
{
  int i, n;
  zVec3DList vlist;
  zLoop3D ch;
  zLoop3DCell *vp;
  zVec3D v, ip[N], po;
  zPlane3D pl;
  FILE *fp;

  zRandInit();
  zListInit( &vlist );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
    zVec3DListAdd( &vlist, &v );
    zVec3DDataNLFPrint( fp, &v );
  }
  fclose( fp );

  zVec3DCreate( &po, -1, 0, 0 );
  zPlane3DCreate( &pl, &po, ZVEC3DX );

  n = intersect_vlist_plane( &vlist, &pl, ip );
  fp = fopen( "ip", "w" );
  for( i=0; i<n; i++ )
    zVec3DDataNLFPrint( fp, &ip[i] );
  fclose( fp );

  zConvexHull2DPL( &ch, &vlist );
  fp = fopen( "ch", "w" );
  zListForEach( &ch, vp )
    zVec3DDataNLFPrint( fp, vp->data );
  zVec3DDataNLFPrint( fp, zListTail(&ch)->data );
  fclose( fp );

  n = intersect_vlist_plane( &ch, &pl, ip );
  fp = fopen( "ipch", "w" );
  for( i=0; i<n; i++ )
    zVec3DDataNLFPrint( fp, &ip[i] );
  fclose( fp );

  zLoop3DDestroy( &ch );
  zVec3DListDestroy( &vlist );
  return 0;
}

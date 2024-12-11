#include <zeo/zeo_col.h>
#include <zeo/zeo_bv2d.h>

/* intersection between a loop of vertices and 3D plane */
int intersect_vec_plane(zVec3DData *data, zPlane3D *p, zVec3D ip[])
{
  zEdge3D e;
  zVec3D *v0, *v;
  int n = 0;

  zVec3DDataRewind( data );
  v0 = zVec3DDataFetch( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    zEdge3DCreate( &e, v0, v );
    if( zIntersectEdgePlane3D( &e, p, &ip[n] ) ) n++;
    v0 = v;
  }
  return n;
}

#define N 100

int main(void)
{
  int i, n;
  zVec3DData data;
  zLoop3D ch;
  zLoop3DCell *vp;
  zVec3D v, ip[N], po;
  zPlane3D pl;
  FILE *fp;

  zRandInit();
  zVec3DDataInitList( &data );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
    zVec3DDataAdd( &data, &v );
    zVec3DValueNLFPrint( fp, &v );
  }
  fclose( fp );

  zVec3DCreate( &po, -1, 0, 0 );
  zPlane3DCreate( &pl, &po, ZVEC3DX );

  n = intersect_vec_plane( &data, &pl, ip );
  fp = fopen( "ip", "w" );
  for( i=0; i<n; i++ )
    zVec3DValueNLFPrint( fp, &ip[i] );
  fclose( fp );

  zVec3DDataConvexHull2D( &data, &ch );
  fp = fopen( "ch", "w" );
  zListForEach( &ch, vp )
    zVec3DValueNLFPrint( fp, vp->data );
  zVec3DValueNLFPrint( fp, zListTail(&ch)->data );
  fclose( fp );
  zVec3DDataDestroy( &data );

  zVec3DDataAssignAddrList( &data, &ch );
  n = intersect_vec_plane( &data, &pl, ip );
  fp = fopen( "ipch", "w" );
  for( i=0; i<n; i++ )
    zVec3DValueNLFPrint( fp, &ip[i] );
  fclose( fp );
  zVec3DDataDestroy( &data );

  zLoop3DDestroy( &ch );
  return 0;
}

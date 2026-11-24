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
  zVec2DData data2d;
  zPG2D ch;
  zVec2D q;
  zVec3D v, ip[N], po, chv[N];
  zPlane3D pl;
  FILE *fp;

  zRandInit();
  zVec3DDataInitList( &data );
  zVec2DDataInitList( &data2d );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
    zVec3DDataAdd( &data, &v );
    zVec2DDataAdd( &data2d, zVec2DCreate( &q, v.c.x, v.c.y ) );
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

  /* convex hull on the plane z = 0, the vertices of which are in counterclockwise order */
  zVec2DDataConvexHull( &data2d, &ch );
  zVec2DDataDestroy( &data2d );
  fp = fopen( "ch", "w" );
  for( i=0; i<zPG2DVertNum(&ch); i++ ){
    zVec3DCreate( &chv[i], zPG2DVert(&ch,i)->c.x, zPG2DVert(&ch,i)->c.y, 0 );
    zVec3DValueNLFPrint( fp, &chv[i] );
  }
  zVec3DValueNLFPrint( fp, &chv[0] );
  fclose( fp );
  zVec3DDataDestroy( &data );

  zVec3DDataAssignArrayDirect( &data, chv, zPG2DVertNum(&ch) );
  n = intersect_vec_plane( &data, &pl, ip );
  fp = fopen( "ipch", "w" );
  for( i=0; i<n; i++ )
    zVec3DValueNLFPrint( fp, &ip[i] );
  fclose( fp );
  zVec3DDataDestroy( &data );

  zPG2DDestroy( &ch );
  return 0;
}

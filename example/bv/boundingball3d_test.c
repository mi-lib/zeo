#include <zeo/zeo_bv3d.h>

void generate_point(zVec3DData *pointdata, int n)
{
  zVec3D p;

  zVec3DDataInitList( pointdata );
  while( --n >= 0 ){
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( !zVec3DDataAdd( pointdata, &p ) ) break;
  }
}

#define N 1000

bool check(zVec3DData *pointdata, zSphere3D *bb, zVec3D *vp[], int num)
{
  zVec3D *v;
  bool ret = true;
  int i;

  zVec3DDataRewind( pointdata );
  while( ( v = zVec3DDataFetch( pointdata ) ) )
    if( !zSphere3DPointIsInside( bb, v, zTOL ) ){
      eprintf( "(inlier error)\n" );
      ret = false;
    }
  for( i=0; i<num; i++ ){
    if( !zIsTiny( zVec3DDist( zSphere3DCenter(bb), vp[i] ) - zSphere3DRadius(bb) ) ){
      eprintf( "(boundary error %g)\n", zVec3DDist(vp[i],zSphere3DCenter(bb)) - zSphere3DRadius(bb) );
      ret = false;
    }
  }
  return ret;
}

int main(int argc, char *argv[])
{
  zVec3DData pointdata;
  zSphere3D bb1, bb2;
  zVec3D *vp1[4], *vp2[4];
  int num1, num2;
  int i, success1, success2;

  zRandInit();
  for( success1=success2=0, i=0; i<1000; i++ ){
    generate_point( &pointdata, N );
    num1 = zVec3DDataBoundingBall( &pointdata, &bb1, vp1 );
    num2 = zVec3DDataBoundingBallRecursive( &pointdata, &bb2, vp2 );
    if( check(&pointdata,&bb1,vp1,num1) ) success1++;
    if( check(&pointdata,&bb2,vp2,num2) ) success2++;
    zVec3DDataDestroy( &pointdata );
  }
  eprintf( "success rate (non-recursive implementation) = %d / 1000\n", success1 );
  eprintf( "success rate (    recursive implementation) = %d / 1000\n", success2 );
  return 0;
}

#include <zeo/zeo_bv3d.h>

#define DIV 10

int main(int argc, char *argv[])
{
  zAABox3D box;
  zVec3D p;
  register int i, j, k;

  zAABox3DCreate( &box, -1, -1, -1, 1, 1, 1 );
  for( i=-2; i<=2; i++ ){
    for( j=-2; j<=2; j++ ){
      for( k=0; k<DIV; k++ ){
        zVec3DCreate( &p, j+(double)k/DIV, 0, (double)i );
        printf( "%s ", zAABox3DPointIsInside(&box,&p,false) ? ">in " : ">out" );
        zVec3DPrint( &p );
      }
    }
  }
  return 0;
}

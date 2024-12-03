#include <zeo/zeo_bv3d.h>

void gen_box(zBox3D *box)
{
  double d, w, h;
  zVec3D c;
  zMat3D r;

  d = zRandF(-0.5,0.5);
  w = zRandF(-0.5,0.5);
  h = zRandF(-0.5,0.5);
  zVec3DCreate( &c, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
  zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zBox3DCreate( box, &c, &r.v[0], &r.v[1], &r.v[2], d, w, h );
}

int main(int argc, char *argv[])
{
  zAABox3D aabox;
  zBox3D box;
  FILE *fp;

  zRandInit();
  gen_box( &box );
  zBox3DToAABox3D( &box, &aabox );

  fp = fopen( "b", "w" );
  zBox3DValueFPrint( fp, &box );
  fclose( fp );
  fp = fopen( "a", "w" );
  zAABox3DValueFPrint( fp, &aabox );
  fclose( fp );
  return 0;
}

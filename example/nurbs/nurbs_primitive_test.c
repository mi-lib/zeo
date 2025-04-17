#include <zeo/zeo_nurbs3d_shape.h>

void foutput(FILE *fp, zNURBS3D *nurbs, const char *shapename, const char *opticname, float rd, float gd, float bd)
{
  fprintf( fp, "[zeo::optic]\n" );
  fprintf( fp, "name: %s\n", opticname );
  fprintf( fp, "ambient: 0.5, 0.5, 0.5\n" );
  fprintf( fp, "diffuse: %g, %g, %g\n", rd, gd, bd );
  fprintf( fp, "[zeo::shape]\n" );
  fprintf( fp, "name: %s\n", shapename );
  fprintf( fp, "type: nurbs\n" );
  fprintf( fp, "optic: %s\n", opticname );
  zNURBS3DFPrintZTK( fp, nurbs );
}

int main(void)
{
  zNURBS3D nurbs;
  zBox3D box;
  zSphere3D sphere;
  zCyl3D cylinder;
  zCone3D cone;
  zEllips3D ellips;
  zECyl3D ecyl;
  zVec3D center1, center2;
  zVec3D ax, ay, az;
  FILE *fp;

  fp = fopen( "primitive.ztk", "w" );

  zVec3DCreate( &center1, 0, 0, 1.7 );
  zVec3DCreate( &az, 0, 0, 10 );
  zVec3DOrthoNormalSpace( &az, &ax, &ay );
  zBox3DCreate( &box, &center1, &ax, &ay, &az, 0.8, 0.6, 0.5 );
  zNURBS3DBox( &nurbs, &box );
  foutput( fp, &nurbs, "box", "white", 1.0, 1.0, 1.0 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0, 1, 1.5 );
  zSphere3DCreate( &sphere, &center1, 0.5, 0 );
  zNURBS3DSphere( &nurbs, &sphere );
  foutput( fp, &nurbs, "sphere", "lightblue", 0.5, 0.5, 1.0 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0,-1, 0.9 );
  zVec3DCreate( &center2, 0,-1.2, 1.8 );
  zCyl3DCreate( &cylinder, &center1, &center2, 0.3, 0 );
  zNURBS3DCyl( &nurbs, &cylinder );
  foutput( fp, &nurbs, "cylinder", "orange", 1.0, 0.5, 0.0 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0, 1, 0 );
  zVec3DCreate( &center2, 0, 1.8, 1.0 );
  zVec3DSub( &center2, &center1, &az );
  zVec3DOrthoNormalSpace( &az, &ax, &ay );
  zECyl3DCreate( &ecyl, &center1, &center2, 0.2, 0.4, &ax, 0 );
  zNURBS3DECyl( &nurbs, &ecyl );
  foutput( fp, &nurbs, "ellipticcylinder", "lightgreen", 0.5, 1.0, 0.5 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0, -1, 0 );
  zVec3DCreate( &center2, 0, -1.8, 1.0 );
  zCone3DCreate( &cone, &center1, &center2, 0.5, 0 );
  zNURBS3DCone( &nurbs, &cone );
  foutput( fp, &nurbs, "cone", "lightcyan", 0.5, 1.0, 1.0 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0, 0, 0.2 );
  zVec3DCreate( &az, 1.0, 1.0, 0.2 );
  zVec3DOrthoNormalSpace( &az, &ax, &ay );
  zEllips3DCreate( &ellips, &center1, &ax, &ay, &az, 0.5, 0.4, 0.6, 0 );
  zNURBS3DEllips( &nurbs, &ellips );
  foutput( fp, &nurbs, "ellipsoid", "yellow", 1.0, 1.0, 0.0 );
  zNURBS3DDestroy( &nurbs );

  zVec3DCreate( &center1, 0, 0, 1 );
  zVec3DCreate( &az, 0, 0.1, 0.8 );
  zVec3DOrthoNormalSpace( &az, &ax, &ay );
  zNURBS3DTorus( &nurbs, &center1, &ax, &ay, &az, 0.5, 0.2 );
  foutput( fp, &nurbs, "torus", "red", 1.0, 0.0, 0.0 );
  zNURBS3DDestroy( &nurbs );

  fclose( fp );
  return 0;
}

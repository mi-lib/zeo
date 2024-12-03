#include <zeo/zeo_ph.h>
#include <zeo/zeo_brep.h>

/* test */

void create_src(zPH3D *ph)
{
  zVec3D v[] = {
    { { 0.0, 0.0, 0 } },
    { {-0.1, 0.2, 0 } },
    { { 0.1, 0.2, 0 } },
    { { 0.05, 0.5, 0 } },
    { { 0.3, 0.5, 0 } },
    { { 0.3, 0.0, 0 } },
  };
  zVec3D shift = { { 0, 0.1, 0.4 } };

  zPH3DCreatePrism( ph, v, sizeof(v)/sizeof(zVec3D), &shift );
}

void output(zPH3D *ph, char name[])
{
  FILE *fp;

  fp = fopen( name, "w" );
  /* for visualization */
  fprintf( fp, "[optic]\n" );
  fprintf( fp, "name: white\n" );
  fprintf( fp, "ambient: 0.8 0.8 0.8\n" );
  fprintf( fp, "diffuse: 0.8 0.8 0.8\n" );
  fprintf( fp, "specular: 0.0 0.0 0.0\n" );
  fprintf( fp, "alpha: 0.8\n" );
  fprintf( fp, "esr: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: prism\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

void brep_output(zBREP *brep)
{
  zBREPFaceListCell *cp;
  FILE *fp;

  /* for gnuplot */
  fp = fopen( "brep", "w" );
  zListForEach( &brep->flist, cp ){
    zVec3DValueNLFPrint( fp, &cp->data.v[0]->data.p );
    zVec3DValueNLFPrint( fp, &cp->data.v[1]->data.p );
    fprintf( fp, "\n" );
    zVec3DValueNLFPrint( fp, &cp->data.v[2]->data.p );
    zVec3DValueNLFPrint( fp, &cp->data.v[2]->data.p );
    fprintf( fp, "\n\n" );
  }
  fclose( fp );
}

int main(int argc, char *argv[])
{
  zPH3D prism, ph;
  zBREP brep;
  zPlane3D pl;
#if 1
  zVec3D c = { { 0.2, 0.2, 0.2 } }, n = { { 1.0, 0.0, 0.5 } };
#else /* ijiwaru case */
  zVec3D c = { { 1.0, 0.0, 0.0 } }, n = { { -1.0, 0.0, 0.0 } };
#endif

  create_src( &prism );

  zPH3D2BREP( &prism, &brep );

  zVec3DNormalizeDRC( &n );
  zPlane3DCreate( &pl, &c, &n );
  zBREPTrunc( &brep, &pl );
  brep_output( &brep );

  zBREP2PH3D( &brep, &ph );
  output( &prism, "src.ztk" );
  output( &ph, "dest.ztk" );

  zPH3DDestroy( &prism );
  zPH3DDestroy( &ph );
  zBREPDestroy( &brep );
  return 0;
}

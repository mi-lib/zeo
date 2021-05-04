#include <zeo/zeo_ph.h>
#include <zeo/zeo_brep.h>

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

  zPH3DPrism( ph, v, sizeof(v)/sizeof(zVec3D), &shift );
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
  fprintf( fp, "exp: 1.0\n\n" );

  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: prism\n" );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: white\n" );
  zPH3DFPrintZTK( fp, ph );
  fclose( fp );
}

void brep_output(zBREP *brep)
{
  zBREPEdgeListCell *cp;

  zListForEach( &brep->elist, cp ){
    zVec3DDataNLPrint( &cp->data.v[0]->data.p );
    zVec3DDataNLPrint( &cp->data.v[1]->data.p );
  }
}

int main(int argc, char *argv[])
{
  zPH3D prism, ph;
  zBREP brep;

  create_src( &prism );

  zPH3D2BREP( &prism, &brep );
  zBREP2PH3D( &brep, &ph );

  brep_output( &brep );
  output( &prism, "src.ztk" );
  output( &ph, "dest.ztk" );

  zPH3DDestroy( &prism );
  zPH3DDestroy( &ph );
  zBREPDestroy( &brep );
  return 0;
}

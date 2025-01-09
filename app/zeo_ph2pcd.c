#include <zeo/zeo.h>

enum{
  PH2PCD_INPUTFILE = 0,
  PH2PCD_OUTPUTFILE,
  PH2PCD_VERBOSE,
  PH2PCD_HELP,
  PH2PCD_INVALID
};
zOption ph2pcd_option[] = {
  { "i", "in",    "<input file>",  "input STL/PLY/ZTK file", NULL, false },
  { "o", "out",   "<output file>", "output PCD file", NULL, false },
  { "v", "verbose", NULL,          "make this program verbose", NULL, false },
  { "h", "help", NULL,             "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void ph2pcd_usage(const char *arg)
{
  eprintf( "Usage: %s [options] <.ply/.stl file> [.pcd file]\n", arg );
  zOptionHelp( ph2pcd_option );
  exit( EXIT_SUCCESS );
}

bool ph2pcd_cmdarg(int argc, char *argv[])
{
  zStrAddrList arglist;
  char *srcfile, *dstfile;

  if( argc <= 1 ) ph2pcd_usage( argv[0] );
  zOptionRead( ph2pcd_option, argv+1, &arglist );
  if( ph2pcd_option[PH2PCD_HELP].flag ) ph2pcd_usage( argv[0] );
  zStrListGetPtr( &arglist, 2, &srcfile, &dstfile );
  if( srcfile ){
    ph2pcd_option[PH2PCD_INPUTFILE].flag = true;
    ph2pcd_option[PH2PCD_INPUTFILE].arg  = srcfile;
  }
  if( dstfile ){
    ph2pcd_option[PH2PCD_OUTPUTFILE].flag = true;
    ph2pcd_option[PH2PCD_OUTPUTFILE].arg  = dstfile;
  }
  zStrAddrListDestroy( &arglist );
  return true;
}

bool ph2pcd_read(zShape3D *shape)
{
  zShape3DInit( shape );
  if( !ph2pcd_option[PH2PCD_INPUTFILE].flag ){
    eprintf( "input file not specified.\n" );
    return false;
  }
  if( !zShape3DReadFile( shape, ph2pcd_option[PH2PCD_INPUTFILE].arg ) )
    return false;
  if( shape->com != &zeo_shape3d_ph_com ){
    ZRUNERROR( "input shape is not a polyhedron" );
    return false;
  }
  return true;
}

bool ph2pcd_write(zShape3D *shape)
{
  char filename[BUFSIZ];
  zVec3DData pointdata;

  if( !ph2pcd_option[PH2PCD_OUTPUTFILE].flag ){
    zReplaceSuffix( ph2pcd_option[PH2PCD_INPUTFILE].arg, ZEO_PCD_SUFFIX, filename, BUFSIZ );
    ph2pcd_option[PH2PCD_OUTPUTFILE].arg = filename;
  }
  zVec3DDataAssignArray( &pointdata, &zShape3DPH(shape)->vert );
  return zVec3DDataWritePCDFile( &pointdata, ph2pcd_option[PH2PCD_OUTPUTFILE].arg, "binary" );
}

int main(int argc, char *argv[])
{
  zShape3D shape;
  int retval = EXIT_FAILURE;

  if( !ph2pcd_cmdarg( argc, argv ) ) return EXIT_FAILURE;
  if( ph2pcd_read( &shape ) )
    if( ph2pcd_write( &shape ) ) retval = EXIT_SUCCESS;
  zShape3DDestroy( &shape );
  return retval;
}

#include <zeo/zeo.h>

enum{
  PHCONV_INPUTFILE = 0,
  PHCONV_OUTPUTFILE,
  PHCONV_ASCII,
  PHCONV_BINARY,
  PHCONV_TRANSLATE,
  PHCONV_ROTATE,
  PHCONV_SCALE,
  PHCONV_HELP,
  PHCONV_INVALID
};
zOption option[] = {
  { "i", "in",    "<input file>",  "input STL/PLY/ZTK file", NULL, false },
  { "o", "out",   "<output file>", "output STL/PLY/ZTK file", NULL, false },
  { "a", "ascii", NULL,            "output ASCII STL/PLY file", NULL, false },
  { "b", "bin",   NULL,            "output binary STL/PLY file", NULL, false },
  { "t", "translate", "<x> <y> <z>",   "translate geometry", (char *)"0.0 0.0 0.0", false },
  { "r", "rotate", "<rxx> <rxy> <rxz> <ryz> <ryy> <ryz> <rzx> <rzy> <rzz>", "rotate geometry", (char *)"1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0", false },
  { "s", "scale", "<factor>",      "scale geometry", (char *)"1.0", false },
  { "h", "help",  NULL,            "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void phconv_usage(char *arg)
{
  eprintf( "Usage: %s [options] <input file> <output file>", arg );
  zOptionHelp( option );
  exit( EXIT_SUCCESS );
}

bool phconv_cmdarg(int argc, char *argv[])
{
  zStrAddrList arglist;
  char *srcfile, *dstfile;

  if( argc <= 1 ) phconv_usage( argv[0] );
  zOptionRead( option, argv+1, &arglist );
  if( option[PHCONV_HELP].flag ) phconv_usage( argv[0] );
  zStrListGetPtr( &arglist, 2, &srcfile, &dstfile );
  if( srcfile ){
    option[PHCONV_INPUTFILE].flag = true;
    option[PHCONV_INPUTFILE].arg  = srcfile;
  }
  if( dstfile ){
    option[PHCONV_OUTPUTFILE].flag = true;
    option[PHCONV_OUTPUTFILE].arg  = dstfile;
  }

  if( !option[PHCONV_INPUTFILE].flag ){
    eprintf( "input file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  if( !option[PHCONV_OUTPUTFILE].flag ){
    eprintf( "output file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  zStrAddrListDestroy( &arglist );
  return true;
}

bool phconv_read_stl(zShape3D *shape)
{
  bool ret = true;

  eprintf( "read STL file.\n" );
  zShape3DInit( shape );
  if( !zShape3DReadFileSTL( shape, option[PHCONV_INPUTFILE].arg ) ){
    eprintf( "read failure.\n" );
    ret = false;
  }
  return ret;
}

bool phconv_read_ply(zShape3D *shape)
{
  bool ret = true;
  eprintf( "read PLY file.\n" );
  zShape3DInit( shape );
  if( !zShape3DReadFilePLY( shape, option[PHCONV_INPUTFILE].arg ) ){
    eprintf( "read failure.\n" );
    ret = false;
  }
  return ret;
}

bool phconv_read(zShape3D *shape)
{
  char *suffix;

  suffix = zGetSuffix( option[PHCONV_INPUTFILE].arg );
  if( strcmp( suffix, "stl" ) == 0 || strcmp( suffix, "STL" ) == 0 )
    return phconv_read_stl( shape );
  if( strcmp( suffix, "ply" ) == 0 || strcmp( suffix, "PLY" ) == 0 )
    return phconv_read_ply( shape );
  if( strcmp( suffix, "ztk" ) == 0 || strcmp( suffix, "ZTK" ) == 0 ){
    eprintf( "read ZTK file.\n" );
    if( !zShape3DReadZTK( shape, option[PHCONV_INPUTFILE].arg ) ){
      eprintf( "read failure.\n" );
      return false;
    }
    return true;
  }
  eprintf( "unknown file type.\n" );
  return false;
}

bool phconv_write_stl(zShape3D *shape)
{
  FILE *fout;

  if( !( fout = fopen( option[PHCONV_OUTPUTFILE].arg, "w" ) ) ){
    ZOPENERROR( option[PHCONV_OUTPUTFILE].arg );
    return false;
  }
  if( option[PHCONV_BINARY].flag ){
    eprintf( "write binary STL file.\n" );
    zPH3DFWriteSTL_Bin( fout, zShape3DPH(shape), zName(shape) );
  } else{
    eprintf( "write ASCII STL file.\n" );
    zPH3DFWriteSTL_ASCII( fout, zShape3DPH(shape), zName(shape) );
  }
  fclose( fout );
  return true;
}

bool phconv_write_ply(zShape3D *shape)
{
  if( option[PHCONV_BINARY].flag ){
    eprintf( "write binary PLY file.\n" );
    zPH3DWriteFilePLY_Bin( zShape3DPH(shape), option[PHCONV_OUTPUTFILE].arg );
  } else{
    eprintf( "write ASCII PLY file.\n" );
    zPH3DWriteFilePLY_ASCII( zShape3DPH(shape), option[PHCONV_OUTPUTFILE].arg );
  }
  return true;
}

bool phconv_xform(zShape3D *shape)
{
  zFrame3D frame;

  sscanf( option[PHCONV_TRANSLATE].arg, "%lf %lf %lf",
    &frame.pos.c.x, &frame.pos.c.y, &frame.pos.c.z );
  sscanf( option[PHCONV_ROTATE].arg, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
    &frame.att.c.xx, &frame.att.c.xy, &frame.att.c.xz,
    &frame.att.c.yx, &frame.att.c.yy, &frame.att.c.yz,
    &frame.att.c.zx, &frame.att.c.zy, &frame.att.c.zz );
  zPH3DXform( zShape3DPH(shape), &frame, zShape3DPH(shape) );
  return true;
}

bool phconv_write(zShape3D *shape)
{
  char *suffix;

  if( option[PHCONV_TRANSLATE].flag || option[PHCONV_ROTATE].flag )
    phconv_xform( shape );
  if( option[PHCONV_SCALE].flag )
    zPH3DScale( zShape3DPH(shape), atof( option[PHCONV_SCALE].arg ) );
  suffix = zGetSuffix( option[PHCONV_OUTPUTFILE].arg );
  if( strcmp( suffix, "stl" ) == 0 || strcmp( suffix, "STL" ) == 0 )
    return phconv_write_stl( shape );
  if( strcmp( suffix, "ply" ) == 0 || strcmp( suffix, "PLY" ) == 0 )
    return phconv_write_ply( shape );
  if( strcmp( suffix, "ztk" ) == 0 || strcmp( suffix, "ZTK" ) == 0 ){
    eprintf( "write ZTK file.\n" );
    return zShape3DWriteZTK( shape, option[PHCONV_OUTPUTFILE].arg );
  }
  eprintf( "unknown file type.\n" );
  return false;
}

int main(int argc, char *argv[])
{
  zShape3D shape;

  if( !phconv_cmdarg( argc, argv ) ) return EXIT_FAILURE;
  if( phconv_read( &shape ) )
    phconv_write( &shape );
  zShape3DDestroy( &shape );
  return 0;
}

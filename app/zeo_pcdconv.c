#include <zeo/zeo.h>

enum{
  PCDCONV_INPUTFILE = 0,
  PCDCONV_OUTPUTFILE,
  PCDCONV_ASCII,
  PCDCONV_BINARY,
  PCDCONV_TRANSLATE,
  PCDCONV_ROTATE,
  PCDCONV_SCALE,
  PCDCONV_HELP,
  PCDCONV_INVALID
};
zOption pcdconv_option[] = {
  { "i", "in",    "<input file>",  "input PCD file", NULL, false },
  { "o", "out",   "<output file>", "output PCD file", NULL, false },
  { "a", "ascii", NULL,            "output ASCII PCD file", NULL, false },
  { "b", "bin",   NULL,            "output binary PCD file", NULL, false },
  { "t", "translate", "<x> <y> <z>",   "translate geometry", (char *)"0.0 0.0 0.0", false },
  { "r", "rotate", "<rxx> <rxy> <rxz> <ryz> <ryy> <ryz> <rzx> <rzy> <rzz>", "rotate geometry", (char *)"1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0", false },
  { "s", "scale", "<factor>",      "scale geometry", (char *)"1.0", false },
  { "h", "help",  NULL,            "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void pcdconv_usage(char *arg)
{
  eprintf( "Usage: %s [options] <input file> <output file>", arg );
  zOptionHelp( pcdconv_option );
  exit( EXIT_SUCCESS );
}

bool pcdconv_cmdarg(int argc, char *argv[])
{
  zStrAddrList arglist;
  char *srcfile, *dstfile;

  if( argc <= 1 ) pcdconv_usage( argv[0] );
  zOptionRead( pcdconv_option, argv+1, &arglist );
  if( pcdconv_option[PCDCONV_HELP].flag ) pcdconv_usage( argv[0] );
  zStrListGetPtr( &arglist, 2, &srcfile, &dstfile );
  if( srcfile ){
    pcdconv_option[PCDCONV_INPUTFILE].flag = true;
    pcdconv_option[PCDCONV_INPUTFILE].arg  = srcfile;
  }
  if( dstfile ){
    pcdconv_option[PCDCONV_OUTPUTFILE].flag = true;
    pcdconv_option[PCDCONV_OUTPUTFILE].arg  = dstfile;
  }

  if( !pcdconv_option[PCDCONV_INPUTFILE].flag ){
    eprintf( "input file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  if( !pcdconv_option[PCDCONV_OUTPUTFILE].flag ){
    eprintf( "output file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  zStrAddrListDestroy( &arglist );
  return true;
}

bool pcdconv_read(zVec3DData *points)
{
  if( !zVec3DDataReadPCDFile( points, pcdconv_option[PCDCONV_INPUTFILE].arg ) ){
    eprintf( "read failure.\n" );
    return false;
  }
  return true;
}

bool pcdconv_write(zVec3DData *points)
{
  bool ret = true;

  if( pcdconv_option[PCDCONV_BINARY].flag ){
    eprintf( "write a binary PCD file.\n" );
    if( !zVec3DDataWritePCDFile( points, pcdconv_option[PCDCONV_OUTPUTFILE].arg, "binary" ) )
      ret = false;
  } else{
    eprintf( "write an ASCII PCD file.\n" );
    if( !zVec3DDataWritePCDFile( points, pcdconv_option[PCDCONV_OUTPUTFILE].arg, "ascii" ) )
      ret = false;
  }
  return ret;
}

bool pcdconv_xform(zVec3DData *points_input, zVec3DData *points_output)
{
  zFrame3D frame;
  double scale;

  sscanf( pcdconv_option[PCDCONV_TRANSLATE].arg, "%lf %lf %lf",
    &frame.pos.c.x, &frame.pos.c.y, &frame.pos.c.z );
  sscanf( pcdconv_option[PCDCONV_ROTATE].arg, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
    &frame.att.c.xx, &frame.att.c.xy, &frame.att.c.xz,
    &frame.att.c.yx, &frame.att.c.yy, &frame.att.c.yz,
    &frame.att.c.zx, &frame.att.c.zy, &frame.att.c.zz );
  sscanf( pcdconv_option[PCDCONV_SCALE].arg, "%lf", &scale );
  zVec3DDataXform( points_input, &frame, scale, points_output );
  return true;
}

int main(int argc, char *argv[])
{
  zVec3DData points_input, points_output;
  int retval = EXIT_SUCCESS;

  if( !pcdconv_cmdarg( argc, argv ) ||
      !pcdconv_read( &points_input ) ||
      !pcdconv_xform( &points_input, &points_output ) ||
      !pcdconv_write( &points_output ) )
    retval = EXIT_FAILURE;
  zVec3DDataDestroy( &points_input );
  zVec3DDataDestroy( &points_output );
  return retval;
}

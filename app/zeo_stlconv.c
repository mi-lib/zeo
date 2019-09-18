#include <zeo/zeo.h>

enum{
  STLCONV_INPUTFILE = 0,
  STLCONV_OUTPUTFILE,
  STLCONV_ASCII,
  STLCONV_BINARY,
  STLCONV_HELP,
  STLCONV_INVALID
};
zOption option[] = {
  { "i", "in",    "<input file>",  "input STL/ZTK file", NULL, false },
  { "o", "out",   "<output file>", "output STL/ZTK file", NULL, false },
  { "a", "ascii", NULL,            "output ASCII STL file", NULL, false },
  { "b", "bin",   NULL,            "output binary STL file", NULL, false },
  { "h", "help",  NULL,            "show this message", NULL, false },
  { NULL, NULL, NULL, NULL, NULL, false },
};

void stlconv_usage(char *arg)
{
  ZECHO( "Usage: %s [options] <input file> <output file>", arg );
  zOptionHelp( option );
  exit( EXIT_SUCCESS );
}

bool stlconv_cmdarg(int argc, char *argv[])
{
  zStrAddrList arglist;
  char *srcfile, *dstfile;

  if( argc <= 1 ) stlconv_usage( argv[0] );
  zOptionRead( option, argv+1, &arglist );
  if( option[STLCONV_HELP].flag ) stlconv_usage( argv[0] );
  zStrListGetPtr( &arglist, 2, &srcfile, &dstfile );
  if( srcfile ){
    option[STLCONV_INPUTFILE].flag = true;
    option[STLCONV_INPUTFILE].arg  = srcfile;
  }
  if( dstfile ){
    option[STLCONV_OUTPUTFILE].flag = true;
    option[STLCONV_OUTPUTFILE].arg  = dstfile;
  }

  if( !option[STLCONV_INPUTFILE].flag ){
    eprintf( "input file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  if( !option[STLCONV_OUTPUTFILE].flag ){
    eprintf( "output file not specified.\n" );
    exit( EXIT_FAILURE );
  }
  zStrAddrListDestroy( &arglist );
  return true;
}

bool stlconv_read(zShape3D *shape)
{
  FILE *fin;
  char *suffix;
  bool ret = true;
  char buf[BUFSIZ];

  if( !( fin = fopen( option[STLCONV_INPUTFILE].arg, "r" ) ) ){
    ZOPENERROR( option[STLCONV_INPUTFILE].arg );
    return false;
  }
  suffix = zGetSuffix( option[STLCONV_INPUTFILE].arg );
  if( strcmp( suffix, "stl" ) == 0 ){
    zShape3DInit( shape );
    shape->com = &zeo_shape3d_ph_com;
    eprintf( "read STL file.\n" );
    if( !( ret = zPH3DFReadSTL( fin, zShape3DPH(shape), buf, BUFSIZ ) ? true : false ) )
      eprintf( "read failure.\n" );
    else if( !zNameSet( shape, buf ) ){
      ZALLOCERROR();
      ret = false;
    }
  } else
  if( strcmp( suffix, "ztk" ) == 0 ){
    eprintf( "read ZTK file.\n" );
    if( !zShape3DFScan( fin, shape, NULL, 0, NULL, 0 ) ){
      eprintf( "read failure.\n" );
      ret = false;
    }
  } else{
    eprintf( "unknown file type.\n" );
    ret = false;
  }
  fclose( fin );
  return ret;
}

bool stlconv_write(zShape3D *shape)
{
  FILE *fout;
  char *suffix;

  if( !( fout = fopen( option[STLCONV_OUTPUTFILE].arg, "w" ) ) ){
    ZOPENERROR( option[STLCONV_OUTPUTFILE].arg );
    return false;
  }
  suffix = zGetSuffix( option[STLCONV_OUTPUTFILE].arg );
  if( strcmp( suffix, "stl" ) == 0 ){
    if( option[STLCONV_BINARY].flag ){
      eprintf( "write binary STL file.\n" );
      zPH3DFWriteSTL_Bin( fout, zShape3DPH(shape), zName(shape) );
    } else{
      eprintf( "write ASCII STL file.\n" );
      zPH3DFWriteSTL_ASCII( fout, zShape3DPH(shape), zName(shape) );
    }
  } else
  if( strcmp( suffix, "ztk" ) == 0 ){
    eprintf( "write ZTK file.\n" );
    zShape3DFPrint( fout, shape );
  } else{
    eprintf( "unknown file type.\n" );
    return false;
  }
  fclose( fout );
  return true;
}

int main(int argc, char *argv[])
{
  zShape3D shape;

  if( !stlconv_cmdarg( argc, argv ) ) return EXIT_FAILURE;
  if( stlconv_read( &shape ) )
    stlconv_write( &shape );
  zShape3DDestroy( &shape );
  return 0;
}

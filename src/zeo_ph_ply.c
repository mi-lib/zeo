/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_ply - polyhedron I/O on PLY (Stanford Triangle) format
 * for specification of the format, refer:
 * http://paulbourke.net/dataformats/ply/
 * for public PLY files, refer:
 * http://graphics.stanford.edu/data/3Dscanrep/
 */

#include <zeo/zeo_ph.h>

#define ZEO_PLY_DEF_FREAD_DOUBLE( type ) \
static double zPLY_fread_double_##type(FILE *fp){ return (double)fread_##type( fp ); }

ZEO_PLY_DEF_FREAD_DOUBLE( byte )
ZEO_PLY_DEF_FREAD_DOUBLE( word )
ZEO_PLY_DEF_FREAD_DOUBLE( dword )
ZEO_PLY_DEF_FREAD_DOUBLE( float )
ZEO_PLY_DEF_FREAD_DOUBLE( double )

#define ZEO_PLY_DEF_FREAD_DOUBLE_REV( type ) \
static double zPLY_fread_double_##type##_rev(FILE *fp){ return (double)fread_##type##_rev( fp ); }

ZEO_PLY_DEF_FREAD_DOUBLE_REV( word )
ZEO_PLY_DEF_FREAD_DOUBLE_REV( dword )
ZEO_PLY_DEF_FREAD_DOUBLE_REV( float )
ZEO_PLY_DEF_FREAD_DOUBLE_REV( double )

#define ZEO_PLY_DEF_FREAD_INT( type ) \
static int zPLY_fread_int_##type(FILE *fp){ return (int)fread_##type( fp ); }

ZEO_PLY_DEF_FREAD_INT( byte )
ZEO_PLY_DEF_FREAD_INT( word )
ZEO_PLY_DEF_FREAD_INT( dword )
ZEO_PLY_DEF_FREAD_INT( float )
ZEO_PLY_DEF_FREAD_INT( double )

#define ZEO_PLY_DEF_FREAD_INT_REV( type ) \
static int zPLY_fread_int_##type##_rev(FILE *fp){ return (int)fread_##type##_rev( fp ); }

ZEO_PLY_DEF_FREAD_INT_REV( word )
ZEO_PLY_DEF_FREAD_INT_REV( dword )
ZEO_PLY_DEF_FREAD_INT_REV( float )
ZEO_PLY_DEF_FREAD_INT_REV( double )

static double zPLY_fread_ASCII_double_int(FILE *fp){
  int val;
  zFInt( fp, &val );
  return (double)val;
}
static double zPLY_fread_ASCII_double_double(FILE *fp){
  double val;
  zFDouble( fp, &val );
  return val;
}
static int zPLY_fread_ASCII_int_int(FILE *fp){
  int val;
  zFInt( fp, &val );
  return val;
}
static int zPLY_fread_ASCII_int_double(FILE *fp){
  double val;
  zFDouble( fp, &val );
  return (int)val;
}

typedef enum{
  ZEO_PLY_DATA_NONE=-1,
  ZEO_PLY_DATA_CHAR=0, ZEO_PLY_DATA_UCHAR,
  ZEO_PLY_DATA_SHORT, ZEO_PLY_DATA_USHORT,
  ZEO_PLY_DATA_INT, ZEO_PLY_DATA_UINT,
  ZEO_PLY_DATA_FLOAT, ZEO_PLY_DATA_DOUBLE,
  ZEO_PLY_DATA_LIST,
} zPLYDATA;

typedef struct{
  const char *typestr;
  zPLYDATA type;
  double (* read_double)(FILE*);
  int (* read_int)(FILE*);
} zPLYPrp;

static zPLYPrp z_ply_prp[] = {
  { "char",   ZEO_PLY_DATA_CHAR,   NULL, NULL },
  { "uchar",  ZEO_PLY_DATA_UCHAR,  NULL, NULL },
  { "short",  ZEO_PLY_DATA_SHORT,  NULL, NULL },
  { "ushort", ZEO_PLY_DATA_USHORT, NULL, NULL },
  { "int",    ZEO_PLY_DATA_INT,    NULL, NULL },
  { "uint",   ZEO_PLY_DATA_UINT,   NULL, NULL },
  { "float",  ZEO_PLY_DATA_FLOAT,  NULL, NULL },
  { "double", ZEO_PLY_DATA_DOUBLE, NULL, NULL },
  { "list",   ZEO_PLY_DATA_LIST,   NULL, NULL },
  { NULL,     ZEO_PLY_DATA_NONE,   NULL, NULL },
};

static const zPLYPrp *_zPLYPrpFind(char *str)
{
  int i;

  for( i=0; z_ply_prp[i].typestr; i++ )
    if( strcmp( str, z_ply_prp[i].typestr ) == 0 )
      return &z_ply_prp[i];
  return NULL;
}

typedef enum{
  ZEO_PLY_ELEM_NONE=-1,
  ZEO_PLY_ELEM_VERTEX, ZEO_PLY_ELEM_FACE, ZEO_PLY_ELEM_EDGE, ZEO_PLY_ELEM_MATERIAL,
  ZEO_PLY_ELEM_NUM
} zPLYElemType;

#define ZEO_PLY_ELEM_MAX_NUM ZEO_PLY_ELEM_NUM
#define ZEO_PLY_PRP_MAX_NUM  12

typedef struct{
  zPLYElemType type;
  int num;
  int prpnum;
  zPLYPrp *prp[ZEO_PLY_PRP_MAX_NUM];
} zPLYElement;

static void _zPLYElementInit(zPLYElement *elem)
{
  elem->type = ZEO_PLY_ELEM_NONE;
  elem->num = 0;
  elem->prpnum = 0;
  memset( elem->prp, 0, sizeof(zPLYPrp*)*ZEO_PLY_PRP_MAX_NUM );
}

#ifdef DEBUG
/* for debug */
static void _zPLYElementFPrint(FILE *fp, zPLYElement *elem)
{
  int i;

  switch( elem->type ){
  case ZEO_PLY_ELEM_VERTEX:   fprintf( fp, "vertex" );    break;
  case ZEO_PLY_ELEM_FACE:     fprintf( fp, "face" );      break;
  case ZEO_PLY_ELEM_EDGE:     fprintf( fp, "edge" );      break;
  case ZEO_PLY_ELEM_MATERIAL: fprintf( fp, "material" );  break;
  default:                    fprintf( fp, "(unknown)" ); break;
  }
  fprintf( fp, ", num=%d\n", elem->num );
  for( i=0; i<elem->prpnum; i++ )
    fprintf( fp, " type: %s\n", elem->prp[i]->typestr );
}
#endif

typedef enum{
  ZEO_PLY_FORMAT_NONE=-1,
  ZEO_PLY_FORMAT_ASCII, ZEO_PLY_FORMAT_BIN, ZEO_PLY_FORMAT_BIN_REV,
} zPLYFormat;

typedef struct{
  zPLYFormat format;
  char version[4];
  int elemnum;
  zPLYElement elem[ZEO_PLY_ELEM_MAX_NUM];
  byte vertidx[3];
  zPLYPrp *facelistnum;
  zPLYPrp *facelistelem;
} zPLY;

static void _zPLYInit(zPLY *ply)
{
  int i;

  ply->format = ZEO_PLY_FORMAT_NONE;
  strcpy( ply->version, "\0\0\0\0" );
  ply->elemnum = -1;
  for( i=0; i<ZEO_PLY_ELEM_MAX_NUM; i++ )
    _zPLYElementInit( &ply->elem[i] );
  ply->vertidx[0] = ply->vertidx[1] = ply->vertidx[2] = 0;
}

static void _zPLYPrpInit(zPLY *ply)
{
  double (* read_double_ASCII[])(FILE*) = {
    zPLY_fread_ASCII_double_int,    zPLY_fread_ASCII_double_int,
    zPLY_fread_ASCII_double_int,    zPLY_fread_ASCII_double_int,
    zPLY_fread_ASCII_double_int,    zPLY_fread_ASCII_double_int,
    zPLY_fread_ASCII_double_double, zPLY_fread_ASCII_double_double };
  double (* read_double_bin[])(FILE*) = {
    zPLY_fread_double_byte,         zPLY_fread_double_byte,
    zPLY_fread_double_word,         zPLY_fread_double_word,
    zPLY_fread_double_dword,        zPLY_fread_double_dword,
    zPLY_fread_double_float,        zPLY_fread_double_double };
  double (* read_double_bin_rev[])(FILE*) = {
    zPLY_fread_double_byte,         zPLY_fread_double_byte,
    zPLY_fread_double_word_rev,     zPLY_fread_double_word_rev,
    zPLY_fread_double_dword_rev,    zPLY_fread_double_dword_rev,
    zPLY_fread_double_float_rev,    zPLY_fread_double_double_rev };
  int (* read_int_ASCII[])(FILE*) = {
    zPLY_fread_ASCII_int_int,       zPLY_fread_ASCII_int_int,
    zPLY_fread_ASCII_int_int,       zPLY_fread_ASCII_int_int,
    zPLY_fread_ASCII_int_int,       zPLY_fread_ASCII_int_int,
    zPLY_fread_ASCII_int_double,    zPLY_fread_ASCII_int_double };
  int (* read_int_bin[])(FILE*) = {
    zPLY_fread_int_byte,            zPLY_fread_int_byte,
    zPLY_fread_int_word,            zPLY_fread_int_word,
    zPLY_fread_int_dword,           zPLY_fread_int_dword,
    zPLY_fread_int_float,           zPLY_fread_int_double };
  int (* read_int_bin_rev[])(FILE*) = {
    zPLY_fread_int_byte,            zPLY_fread_int_byte,
    zPLY_fread_int_word_rev,        zPLY_fread_int_word_rev,
    zPLY_fread_int_dword_rev,       zPLY_fread_int_dword_rev,
    zPLY_fread_int_float_rev,       zPLY_fread_int_double_rev };
  double (** read_double)(FILE*) = NULL;
  int (** read_int)(FILE*) = NULL;
  int i;

  switch( ply->format ){
  case ZEO_PLY_FORMAT_ASCII:
    read_double = read_double_ASCII;   read_int = read_int_ASCII;   break;
  case ZEO_PLY_FORMAT_BIN:
    read_double = read_double_bin;     read_int = read_int_bin;     break;
  case ZEO_PLY_FORMAT_BIN_REV:
    read_double = read_double_bin_rev; read_int = read_int_bin_rev; break;
  default: ;
  }
  for( i=ZEO_PLY_DATA_CHAR; i<ZEO_PLY_DATA_LIST; i++ ){
    z_ply_prp[i].read_double = read_double[i];
    z_ply_prp[i].read_int = read_int[i];
  }
}

#ifdef DEBUG
/* for debug */
static void _zPLYFPrint(FILE *fp, zPLY *ply)
{
  int i;

  switch( ply->format ){
  case ZEO_PLY_FORMAT_ASCII:   fprintf( fp, "ASCII" ); break;
  case ZEO_PLY_FORMAT_BIN:     fprintf( fp, "binary" ); break;
  case ZEO_PLY_FORMAT_BIN_REV: fprintf( fp, "binary (reverted)" ); break;
  default: ;
  }
  fprintf( fp, " ver. %s\n", ply->version );
  for( i=0; i<=ply->elemnum; i++ ){
    _zPLYElementFPrint( fp, &ply->elem[i] );
  }
  fprintf( fp, "vertex component index: x->%d y->%d z->%d\n", ply->vertidx[0], ply->vertidx[1], ply->vertidx[2] );
  fprintf( fp, "type of number of vertex indices: %s\n", ply->facelistnum->typestr );
  fprintf( fp, "type of vertex indices: %s\n", ply->facelistelem->typestr );
}
#endif

static bool _zPLYIsValid(FILE *fp)
{
  char buf[3];
  long pos;

  pos = ftell( fp );
  if( fread( buf, sizeof(char), 3, fp ) < 3 ||
      strncmp( buf, "ply", 3 ) != 0 ){
    ZRUNERROR( ZEO_ERR_PLY_UNREADABLE );
    fseek( fp, pos, SEEK_SET );
    return false;
  }
  return true;
}

static bool _zPLYFReadFormat(zPLY *ply, char *buf)
{
  char tkn[BUFSIZ];

  zSToken( buf, tkn, BUFSIZ );
  if( strcmp( tkn, "ascii" ) == 0 ){
    ply->format = ZEO_PLY_FORMAT_ASCII;
  } else
  if( strcmp( tkn, "binary_big_endian" ) == 0 ){
    ply->format = endian_check() == Z_ENDIAN_BIG ? ZEO_PLY_FORMAT_BIN : ZEO_PLY_FORMAT_BIN_REV;
  } else
  if( strcmp( tkn, "binary_little_endian" ) == 0 ){
    ply->format = endian_check() == Z_ENDIAN_LITTLE ? ZEO_PLY_FORMAT_BIN : ZEO_PLY_FORMAT_BIN_REV;
  } else{
    ZRUNERROR( ZEO_ERR_PLY_INV_FORMAT, tkn );
    ply->format = ZEO_PLY_FORMAT_NONE;
    return false;
  }
  _zPLYPrpInit( ply );
  return *zSToken( buf, ply->version, 4 ) ? true : false;
}

static bool _zPLYFReadElement(zPLY *ply, char *buf)
{
  char tkn[BUFSIZ];
  int num;

  if( ++ply->elemnum >= ZEO_PLY_ELEM_MAX_NUM ){
    ZRUNERROR( ZEO_ERR_PLY_TOOMANYELEM );
    return false;
  }
  zSToken( buf, tkn, BUFSIZ );
  if( strcmp( tkn, "vertex" ) == 0 ){
    ply->elem[ply->elemnum].type = ZEO_PLY_ELEM_VERTEX;
  } else
  if( strcmp( tkn, "face" ) == 0 ){
    ply->elem[ply->elemnum].type = ZEO_PLY_ELEM_FACE;
  } else
  if( strcmp( tkn, "edge" ) == 0 ){
    ply->elem[ply->elemnum].type = ZEO_PLY_ELEM_EDGE;
  } else
  if( strcmp( tkn, "material" ) == 0 ){
    ply->elem[ply->elemnum].type = ZEO_PLY_ELEM_MATERIAL;
  } else{
    ZRUNERROR( ZEO_ERR_PLY_UNKNOWNELEM, tkn );
    ply->elem[ply->elemnum].type = ZEO_PLY_ELEM_NONE;
  }
  if( !zSInt( buf, &num ) ) return false;
  ply->elem[ply->elemnum].num = num;
  return true;
}

static bool _zPLYFReadProperty(zPLY *ply, char *buf)
{
  char tkn[BUFSIZ];
  zPLYElement *elem;

  elem = &ply->elem[ply->elemnum];
  zSToken( buf, tkn, BUFSIZ );
  if( !( elem->prp[elem->prpnum] = (zPLYPrp*)_zPLYPrpFind( tkn ) ) ){
    ZRUNERROR( ZEO_ERR_PLY_UNKNOWNPRP, tkn );
    return false;
  }
  if( elem->prp[elem->prpnum]->type == ZEO_PLY_DATA_LIST ){
    zSToken( buf, tkn, BUFSIZ );
    ply->facelistnum = (zPLYPrp*)_zPLYPrpFind( tkn );
    zSToken( buf, tkn, BUFSIZ );
    ply->facelistelem = (zPLYPrp*)_zPLYPrpFind( tkn );
    zSToken( buf, tkn, BUFSIZ );
    if( strcmp( tkn, "vertex_indices" ) != 0 ){
      ZRUNERROR( ZEO_ERR_PLY_UNKNOWNPRP, tkn );
      return false;
    }
  } else{
    zSToken( buf, tkn, BUFSIZ );
    if( ply->elem[ply->elemnum].type == ZEO_PLY_ELEM_VERTEX ){
      if( strcmp( tkn, "x" ) == 0 )
        ply->vertidx[0] = ply->elem[ply->elemnum].prpnum;
      else
      if( strcmp( tkn, "y" ) == 0 )
        ply->vertidx[1] = ply->elem[ply->elemnum].prpnum;
      else
      if( strcmp( tkn, "z" ) == 0 )
        ply->vertidx[2] = ply->elem[ply->elemnum].prpnum;
    }
  }
  if( ++ply->elem[ply->elemnum].prpnum >= ZEO_PLY_PRP_MAX_NUM ){
    ZRUNERROR( ZEO_ERR_PLY_TOOMANYPRP );
    return false;
  }
  return true;
}

static bool _zPLYFReadHeader(FILE *fp, zPLY *ply)
{
  char buf[BUFSIZ];

  _zPLYInit( ply );
  while( !feof( fp ) ){
    if( !fgets( buf, BUFSIZ, fp ) ){
      ZRUNERROR( ZEO_ERR_PLY_INCOMPLETE );
      return false;
    }
    if( strncmp( buf, "end_header", 10 ) == 0 ) break;
    if( strncmp( buf, "format", 6 ) == 0 ){
      if( !_zPLYFReadFormat( ply, buf+6 ) ) return false;
    } else
    if( strncmp( buf, "element", 7 ) == 0 ){
      if( !_zPLYFReadElement( ply, buf+7 ) ) return false;
    } else
    if( strncmp( buf, "property", 8 ) == 0 ){
      if( !_zPLYFReadProperty( ply, buf+8 ) ) return false;
    }
    /* other keywords, e.g., comment, obj_info, are skipped. */
  }
#ifdef DEBUG
  _zPLYFPrint( stderr, ply ); /* for debug */
#endif
  return true;
}

static void _zPH3DSetPLYVert(zPH3D *ph, zPLY *ply, int i, int j, double val)
{
  if( j == ply->vertidx[0] ) zPH3DVert(ph,i)->e[0] = val;
  if( j == ply->vertidx[1] ) zPH3DVert(ph,i)->e[1] = val;
  if( j == ply->vertidx[2] ) zPH3DVert(ph,i)->e[2] = val;
}

static bool _zPH3DFReadPLYVert(FILE *fp, zPH3D *ph, zPLY *ply, zPLYElement *elem)
{
  int i, j;
  double val;

  if( elem->num > 0 ){ /* vertices */
    zArrayAlloc( &ph->vert, zVec3D, elem->num );
    if( !zPH3DVertBuf(ph) ) return false;
  }
  for( i=0; i<elem->num; i++ ){
    for( j=0; j<elem->prpnum; j++ ){
      if( elem->prp[j]->type == ZEO_PLY_DATA_LIST ){
        ZRUNERROR( ZEO_ERR_PLY_UNSUPPORTED );
        return false;
      }
      val = elem->prp[j]->read_double( fp );
      _zPH3DSetPLYVert( ph, ply, i, j, val );
    }
    zeo_ph_echo_while_reading ? eprintf( "\r%d/%d vertex", i, elem->num ) : 0;
  }
  zeo_ph_echo_while_reading ? eprintf( "\n" ) : 0;
  return true;
}

static bool _zPH3DFReadPLYFace(FILE *fp, zPH3D *ph, zPLY *ply, zPLYElement *elem)
{
  int i, j, k;
  int nv; /* number of vertices of a face */
  int nf = 0; /* number of faces */
  int v1, v2, v3; /* vertex indices */
  long pos;

  pos = ftell( fp );
  for( i=0; i<elem->num; i++ ){
    for( j=0; j<elem->prpnum; j++ ){
      if( elem->prp[j]->type != ZEO_PLY_DATA_LIST ){
        elem->prp[j]->read_double( fp ); /* discard data */
        continue;
      }
      nv = ply->facelistnum->read_int( fp );
      for( k=0; k<nv; k++ )
        ply->facelistelem->read_int( fp );
      nf += zMax( nv - 2, 0 );
    }
  }
  if( nf > 0 ){
    zArrayAlloc( &ph->face, zTri3D, nf );
    if( !zPH3DFaceBuf(ph) ) return false;
  }
  fseek( fp, pos, SEEK_SET );
  nf = 0;
  for( i=0; i<elem->num; i++ ){
    for( j=0; j<elem->prpnum; j++ ){
      if( elem->prp[j]->type != ZEO_PLY_DATA_LIST ){
        elem->prp[j]->read_double( fp ); /* discard data */
        continue;
      }
      nv = ply->facelistnum->read_int( fp );
      if( --nv < 0 ) continue;
      v1 = ply->facelistelem->read_int( fp );
      if( --nv < 0 ) continue;
      v2 = ply->facelistelem->read_int( fp );
      for( k=0; k<nv; k++ ){
        v3 = ply->facelistelem->read_int( fp );
        zTri3DCreate( zPH3DFace(ph,nf++), zPH3DVert(ph,v1), zPH3DVert(ph,v2), zPH3DVert(ph,v3) );
      }
    }
    zeo_ph_echo_while_reading ? eprintf( "\r%d/%d face", i, elem->num ) : 0;
  }
  zeo_ph_echo_while_reading ? eprintf( "\n" ) : 0;
  return true;
}

static bool _zPH3DFReadPLYElem(FILE *fp, zPH3D *ph, zPLY *ply, zPLYElement *elem)
{
  int i, j;

  for( i=0; i<elem->num; i++ ){
    for( j=0; j<elem->prpnum; j++ ){
      if( elem->prp[j]->type == ZEO_PLY_DATA_LIST ){
        ZRUNERROR( ZEO_ERR_PLY_UNSUPPORTED );
        return false;
      }
      elem->prp[j]->read_double( fp ); /* discard data */
    }
  }
  return true;
}

static bool _zPH3DFReadPLYData(FILE *fp, zPH3D *ph, zPLY *ply)
{
  int i;

  for( i=0; i<=ply->elemnum; i++ ){
    switch( ply->elem[i].type ){
    case ZEO_PLY_ELEM_VERTEX:
      if( !_zPH3DFReadPLYVert( fp, ph, ply, &ply->elem[i] ) ) return false;
      break;
    case ZEO_PLY_ELEM_FACE:
      if( !_zPH3DFReadPLYFace( fp, ph, ply, &ply->elem[i] ) ) return false;
      break;
    default:
      if( !_zPH3DFReadPLYElem( fp, ph, ply, &ply->elem[i] ) ) return false;
    }
  }
  return true;
}

/* read a 3D polyhedron from PLY format */
zPH3D *zPH3DReadFilePLY(zPH3D *ph, const char *filename)
{
  FILE *fp;
  zPLY ply;
  long pos;

  if( !( fp = fopen( filename, "rt" ) ) ){
    ZOPENERROR( filename );
    return NULL;
  }
  if( !_zPLYIsValid( fp ) ) return NULL;
  if( !_zPLYFReadHeader( fp, &ply ) ){
    ZRUNERROR( ZEO_ERR_PLY_INCOMPLETE );
    return NULL;
  }
  if( ply.format == ZEO_PLY_FORMAT_BIN || ply.format == ZEO_PLY_FORMAT_BIN_REV ){
    pos = ftell ( fp );
    fclose( fp );
    if( !( fp = fopen( filename, "rb" ) ) ){
      ZOPENERROR( filename );
      return NULL;
    }
    fseek( fp, pos, SEEK_SET );
  }
  zPH3DInit( ph );
  if( !_zPH3DFReadPLYData( fp, ph, &ply ) ){
    ZRUNERROR( ZEO_ERR_PLY_INCOMPLETE );
    return NULL;
  }
  fclose( fp );
  return ph;
}

static void _zPH3DFWritePLYHeader(FILE *fp, zPH3D *ph, const char *format)
{
  fprintf( fp, "ply\n" );
  fprintf( fp, "format %s 1.0\n", format );
  fprintf( fp, "element vertex %d\n", zPH3DVertNum(ph) );
  fprintf( fp, "property float x\n" );
  fprintf( fp, "property float y\n" );
  fprintf( fp, "property float z\n" );
  fprintf( fp, "element face %d\n", zPH3DFaceNum(ph) );
  fprintf( fp, "property list uchar int vertex_indices\n" );
  fprintf( fp, "end_header\n" );
}

static void _zPH3DFWritePLYDataASCII(FILE *fp, zPH3D *ph)
{
  int i;

  for( i=0; i<zPH3DVertNum(ph); i++ )
    zVec3DDataNLFPrint( fp, zPH3DVert(ph,i) );
  for( i=0; i<zPH3DFaceNum(ph); i++ )
    fprintf( fp, "3 %d %d %d\n",
      (int)( zPH3DFaceVert(ph,i,0)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,1)-zPH3DVertBuf(ph) ),
      (int)( zPH3DFaceVert(ph,i,2)-zPH3DVertBuf(ph) ) );
}

static void _zPH3DFWritePLYDataBin(FILE *fp, zPH3D *ph)
{
  int i;

  for( i=0; i<zPH3DVertNum(ph); i++ ){
    fwrite_float( fp, zPH3DVert(ph,i)->e[0] );
    fwrite_float( fp, zPH3DVert(ph,i)->e[1] );
    fwrite_float( fp, zPH3DVert(ph,i)->e[2] );
  }
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fwrite_byte( fp, 3 );
    fwrite_dword( fp, (int32_t)( zPH3DFaceVert(ph,i,0)-zPH3DVertBuf(ph) ) );
    fwrite_dword( fp, (int32_t)( zPH3DFaceVert(ph,i,1)-zPH3DVertBuf(ph) ) );
    fwrite_dword( fp, (int32_t)( zPH3DFaceVert(ph,i,2)-zPH3DVertBuf(ph) ) );
  }
}

/* write a 3D polyhedron in PLY format (ASCII) */
bool zPH3DWriteFilePLY_ASCII(zPH3D *ph, const char *filename)
{
  FILE *fp;

  if( !( fp = fopen( filename, "wt" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  _zPH3DFWritePLYHeader( fp, ph, "ascii" );
  _zPH3DFWritePLYDataASCII( fp, ph );
  fclose( fp );
  return true;
}

/* write a 3D polyhedron in PLY format (binary) */
bool zPH3DWriteFilePLY_Bin(zPH3D *ph, const char *filename)
{
  FILE *fp;

  if( !( fp = fopen( filename, "wt" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  _zPH3DFWritePLYHeader( fp, ph,
    endian_check() == Z_ENDIAN_BIG ? "binary_big_endian" : "binary_little_endian" );
  fclose( fp );
  if( !( fp = fopen( filename, "ab" ) ) ){
    ZOPENERROR( filename );
    return false;
  }
  _zPH3DFWritePLYDataBin( fp, ph );
  fclose( fp );
  return true;
}

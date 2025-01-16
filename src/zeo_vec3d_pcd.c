/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pcd - read/write of PCD format files (3D point cloud).
 */

#include <zeo/zeo_vec3d_data.h>

/* for binary_compressed data type */
#ifdef __ZEO_USE_PCD_BINARY_COMPRESSED
#include <liblzf/lzf.h>
#endif /* __ZEO_USE_PCD_BINARY_COMPRESSED */

typedef enum{
  ZEO_PCD_DATATYPE_INVALID = -1,
  ZEO_PCD_DATATYPE_ASCII,
  ZEO_PCD_DATATYPE_BINARY,
  ZEO_PCD_DATATYPE_BINARY_COMPRESSED,
} _zPCDDataType;
static const char *__z_pcd_data_type[] = { "ascii", "binary", "binary_compressed", NULL };

/* ********************************************************** */
/* PCD format decoder
 * ********************************************************** */

typedef enum{
  ZEO_PCD_FIELDTYPE_INVALID = -1,
  ZEO_PCD_FIELDTYPE_X = zX, ZEO_PCD_FIELDTYPE_Y = zY, ZEO_PCD_FIELDTYPE_Z = zZ,
  ZEO_PCD_FIELDTYPE_UNKNOWN,
} _zPCDFieldType;
static const char *__z_pcd_field_type[] = {
  "x", "y", "z",
  NULL,
};

typedef enum{
  ZEO_PCD_VALUETYPE_INVALID =-1,
  ZEO_PCD_VALUETYPE_INT     = 0,
  ZEO_PCD_VALUETYPE_UINT    = 1,
  ZEO_PCD_VALUETYPE_FP      = 2,
} _zPCDValueType;
static const char *__z_pcd_value_type[] = { "I", "U", "F", NULL };

typedef struct _zPCDField{
  _zPCDFieldType fieldtype;
  int size;
  int bitsize;
  _zPCDValueType valuetype;
  int count;
  void (* read_ascii)(struct _zPCDField *, const char *, zVec3D *);
  void (* read_bin)(struct _zPCDField *, FILE *, zVec3D *);
  void (* read_bincom)(struct _zPCDField *, byte *, zVec3D *);
} _zPCDField;

#define DEF_zPCDFieldReadASCIIFunc(type,a2v) \
  static void _zPCDFieldReadASCII##type(_zPCDField *field, const char *tkn, zVec3D *v){\
    type val;\
    val = a2v( tkn );\
    v->e[field->fieldtype] = (double)val;\
  }
DEF_zPCDFieldReadASCIIFunc( int8_t, atoi );
DEF_zPCDFieldReadASCIIFunc( uint8_t, atoi );
DEF_zPCDFieldReadASCIIFunc( int16_t, atoi );
DEF_zPCDFieldReadASCIIFunc( uint16_t, atoi );
DEF_zPCDFieldReadASCIIFunc( int32_t, atoi );
DEF_zPCDFieldReadASCIIFunc( uint32_t, atoi );
DEF_zPCDFieldReadASCIIFunc( int64_t, atoi );
DEF_zPCDFieldReadASCIIFunc( uint64_t, atoi );
DEF_zPCDFieldReadASCIIFunc( float, atof );
DEF_zPCDFieldReadASCIIFunc( double, atof );

static void (* __z_pcd_read_ascii[])(_zPCDField *, const char *, zVec3D *) = {
  _zPCDFieldReadASCIIint8_t,
  _zPCDFieldReadASCIIint16_t,
  _zPCDFieldReadASCIIint32_t,
  _zPCDFieldReadASCIIint64_t,
  _zPCDFieldReadASCIIuint8_t,
  _zPCDFieldReadASCIIuint16_t,
  _zPCDFieldReadASCIIuint32_t,
  _zPCDFieldReadASCIIuint64_t,
  NULL,
  NULL,
  _zPCDFieldReadASCIIfloat,
  _zPCDFieldReadASCIIdouble,
};

static bool _zPCDFieldIsCoord(_zPCDField *field)
{
  return field->fieldtype >= ZEO_PCD_FIELDTYPE_X && field->fieldtype <= ZEO_PCD_FIELDTYPE_Z;
}

static void _zPCDFieldAssignReadASCII(_zPCDField *field)
{
  if( _zPCDFieldIsCoord( field ) )
    field->read_ascii = __z_pcd_read_ascii[field->valuetype*4+field->bitsize];
}

#define DEF_zPCDFieldReadBINFunc(type) \
  static void _zPCDFieldReadBIN##type(_zPCDField *field, FILE *fp, zVec3D *v){\
    type val;\
    if( fread( &val, field->size, 1, fp ) == 1 )\
      v->e[field->fieldtype] = (double)val;\
  }
DEF_zPCDFieldReadBINFunc( int8_t );
DEF_zPCDFieldReadBINFunc( uint8_t );
DEF_zPCDFieldReadBINFunc( int16_t );
DEF_zPCDFieldReadBINFunc( uint16_t );
DEF_zPCDFieldReadBINFunc( int32_t );
DEF_zPCDFieldReadBINFunc( uint32_t );
DEF_zPCDFieldReadBINFunc( int64_t );
DEF_zPCDFieldReadBINFunc( uint64_t );
DEF_zPCDFieldReadBINFunc( float );
DEF_zPCDFieldReadBINFunc( double );

static void (* __z_pcd_read_bin[])(_zPCDField *, FILE *, zVec3D *) = {
  _zPCDFieldReadBINint8_t,
  _zPCDFieldReadBINint16_t,
  _zPCDFieldReadBINint32_t,
  _zPCDFieldReadBINint64_t,
  _zPCDFieldReadBINuint8_t,
  _zPCDFieldReadBINuint16_t,
  _zPCDFieldReadBINuint32_t,
  _zPCDFieldReadBINuint64_t,
  NULL,
  NULL,
  _zPCDFieldReadBINfloat,
  _zPCDFieldReadBINdouble,
};

#define DEF_zPCDFieldSkipBINFunc(type) \
  static void _zPCDFieldSkipBIN##type(_zPCDField *field, FILE *fp, zVec3D *v){\
    type val;\
    if( fread( &val, field->size, 1, fp ) == 1 );\
  }
DEF_zPCDFieldSkipBINFunc( int8_t );
DEF_zPCDFieldSkipBINFunc( uint8_t );
DEF_zPCDFieldSkipBINFunc( int16_t );
DEF_zPCDFieldSkipBINFunc( uint16_t );
DEF_zPCDFieldSkipBINFunc( int32_t );
DEF_zPCDFieldSkipBINFunc( uint32_t );
DEF_zPCDFieldSkipBINFunc( int64_t );
DEF_zPCDFieldSkipBINFunc( uint64_t );
DEF_zPCDFieldSkipBINFunc( float );
DEF_zPCDFieldSkipBINFunc( double );

static void (* __z_pcd_skip_bin[])(_zPCDField *, FILE *, zVec3D *) = {
  _zPCDFieldSkipBINint8_t,
  _zPCDFieldSkipBINint16_t,
  _zPCDFieldSkipBINint32_t,
  _zPCDFieldSkipBINint64_t,
  _zPCDFieldSkipBINuint8_t,
  _zPCDFieldSkipBINuint16_t,
  _zPCDFieldSkipBINuint32_t,
  _zPCDFieldSkipBINuint64_t,
  NULL,
  NULL,
  _zPCDFieldSkipBINfloat,
  _zPCDFieldSkipBINdouble,
};

static void _zPCDFieldAssignReadBIN(_zPCDField *field)
{
  field->read_bin = _zPCDFieldIsCoord( field ) ?
    __z_pcd_read_bin[field->valuetype*4+field->bitsize] :
    __z_pcd_skip_bin[field->valuetype*4+field->bitsize];
}

#define DEF_zPCDFieldReadBINCOMFunc(type) \
  static void _zPCDFieldReadBINCOM##type(_zPCDField *field, byte *data_ptr, zVec3D *v){\
    union __zeo_pcd_field_val{\
      byte b[8];\
      type v;\
    } *val_ptr;\
    val_ptr = (union __zeo_pcd_field_val *)data_ptr;\
    v->e[field->fieldtype] = (double)val_ptr->v;\
  }
DEF_zPCDFieldReadBINCOMFunc( int8_t );
DEF_zPCDFieldReadBINCOMFunc( uint8_t );
DEF_zPCDFieldReadBINCOMFunc( int16_t );
DEF_zPCDFieldReadBINCOMFunc( uint16_t );
DEF_zPCDFieldReadBINCOMFunc( int32_t );
DEF_zPCDFieldReadBINCOMFunc( uint32_t );
DEF_zPCDFieldReadBINCOMFunc( int64_t );
DEF_zPCDFieldReadBINCOMFunc( uint64_t );
DEF_zPCDFieldReadBINCOMFunc( float );
DEF_zPCDFieldReadBINCOMFunc( double );

static void (* __z_pcd_read_bincom[])(_zPCDField *, byte *, zVec3D *) = {
  _zPCDFieldReadBINCOMint8_t,
  _zPCDFieldReadBINCOMint16_t,
  _zPCDFieldReadBINCOMint32_t,
  _zPCDFieldReadBINCOMint64_t,
  _zPCDFieldReadBINCOMuint8_t,
  _zPCDFieldReadBINCOMuint16_t,
  _zPCDFieldReadBINCOMuint32_t,
  _zPCDFieldReadBINCOMuint64_t,
  NULL,
  NULL,
  _zPCDFieldReadBINCOMfloat,
  _zPCDFieldReadBINCOMdouble,
};

static void _zPCDFieldAssignReadBINCOM(_zPCDField *field)
{
  if( _zPCDFieldIsCoord( field ) )
    field->read_bincom = __z_pcd_read_bincom[field->valuetype*4+field->bitsize];
}

static void _zPCDFieldInit(_zPCDField *field)
{
  field->fieldtype = ZEO_PCD_FIELDTYPE_INVALID;
  field->size = 0;
  field->bitsize = 0;
  field->valuetype = ZEO_PCD_VALUETYPE_INVALID;
  field->count = 0;
  field->read_ascii = NULL;
  field->read_bin = NULL;
}

typedef struct{
  int version_major;
  int version_minor;
  _zPCDField *field;
  int fieldnum;
  int width;
  int height;
  zFrame3D viewpoint;
  _zPCDDataType datatype;
} _zPCD;

static void _zPCDInit(_zPCD *pcd)
{
  pcd->version_major = pcd->version_minor = 0;
  pcd->field = NULL;
  pcd->fieldnum = 0;
  pcd->width = pcd->height = 0;
  zFrame3DIdent( &pcd->viewpoint );
  pcd->datatype = ZEO_PCD_DATATYPE_INVALID;
}

static int _zPCDFieldAttrFind(const char *tkn, const char *attrlist[])
{
  int i;

  for( i=0; attrlist[i]; i++ )
    if( strcmp( tkn, attrlist[i] ) == 0 ) return i;
  return -1;
}

static bool _zPCDVersionFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  char *cp;

  if( ( cp = strchr( buf, '.' ) ) == NULL ){
    ZRUNERROR( ZEO_ERR_PCD_INVALID_VERSION, buf );
    return false;
  }
  if( cp == buf ){
    pcd->version_major = 0;
  } else{
    *cp = '\0';
    pcd->version_major = atoi( buf );
    pcd->version_minor = atoi( cp+1 );
  }
  return true;
}

static bool _zPCDFieldsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;
  char bufcpy[BUFSIZ];

  strcpy( bufcpy, buf );
  for( pcd->fieldnum=0; *zSToken( bufcpy, tkn, BUFSIZ ); pcd->fieldnum++ );
  if( !( pcd->field = zAlloc( _zPCDField, pcd->fieldnum ) ) ){
    ZALLOCERROR();
    return false;
  }
  for( i=0; i<pcd->fieldnum; i++ ){
    if( !*zSToken( buf, tkn, BUFSIZ ) ) break;
    _zPCDFieldInit( &pcd->field[i] );
    if( ( pcd->field[i].fieldtype = (_zPCDFieldType)_zPCDFieldAttrFind( tkn, __z_pcd_field_type ) ) < 0 ){
      pcd->field[i].fieldtype = ZEO_PCD_FIELDTYPE_UNKNOWN;
    }
  }
  return true;
}

static bool _zPCDCheckFieldNum(_zPCD *pcd, int n)
{
  if( n == pcd->fieldnum ) return true;
  ZRUNERROR( ZEO_ERR_PCD_MISMATCH_NUMOFFIELDS, n );
  return false;
}

static bool _zPCDSizeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; *zSToken( buf, tkn, BUFSIZ ) && i<pcd->fieldnum; i++ ){
    switch( ( pcd->field[i].size = atoi( tkn ) ) ){
    case 1: pcd->field[i].bitsize = 0; break;
    case 2: pcd->field[i].bitsize = 1; break;
    case 4: pcd->field[i].bitsize = 2; break;
    case 8: pcd->field[i].bitsize = 3; break;
    default:
      ZRUNERROR( ZEO_ERR_PCD_INVALID_FIELDSIZE, pcd->field[i].size );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

static bool _zPCDValueTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; *zSToken( buf, tkn, BUFSIZ ) && i<pcd->fieldnum; i++ ){
    if( ( pcd->field[i].valuetype = (_zPCDValueType)_zPCDFieldAttrFind( tkn, __z_pcd_value_type ) ) < 0 ){
      ZRUNERROR( ZEO_ERR_PCD_UNKNOWN_VALUETYPE, tkn );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

static bool _zPCDCountFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; *zSToken( buf, tkn, BUFSIZ ) && i<pcd->fieldnum; i++ ){
    if( ( pcd->field[i].count = atoi( tkn ) ) != 1 ){
      ZRUNERROR( ZEO_ERR_PCD_INVALID_COUNT );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

static bool _zPCDWidthFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( !*zSToken( buf, tkn, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_PCD_UNSPEC_WIDTH );
    return false;
  }
  pcd->width = atoi( tkn );
  return true;
}

static bool _zPCDHeightFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( !*zSToken( buf, tkn, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_PCD_UNSPEC_HEIGHT );
    return false;
  }
  pcd->height = atoi( tkn );
  return true;
}

static bool _zPCDViewpointFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  double val[7];
  int i;
  zEP ep;

  for( i=0; i<7; i++ ){
    if( !*zSToken( buf, tkn, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_PCD_INVALID_VIEWPOINT );
      return false;
    }
    val[i] = atof( tkn );
  }
  zVec3DCreate( zFrame3DPos(&pcd->viewpoint), val[0], val[1], val[2] );
  ep.e[0] = val[3]; ep.e[1] = val[4]; ep.e[2] = val[5]; ep.e[3] = val[6];
  zEPNormalize( &ep );
  zMat3DFromEP( zFrame3DAtt(&pcd->viewpoint), &ep );
  return true;
}

static bool _zPCDPointsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int points;

  if( !*zSToken( buf, tkn, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_PCD_UNSPEC_NUMOFPOINTS );
    return false;
  }
  points = atoi( tkn );
  if( points != pcd->width * pcd->height ){
    ZRUNERROR( ZEO_ERR_PCD_INVALID_NUMOFPOINTS, points, pcd->width, pcd->height );
    return false;
  }
  return true;
}

static bool _zPCDDataTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( !*zSToken( buf, tkn, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_PCD_UNSPEC_DATATYPE );
    return false;
  }
  if( ( pcd->datatype = (_zPCDDataType)_zPCDFieldAttrFind( tkn, __z_pcd_data_type ) ) < 0 ){
    ZRUNERROR( ZEO_ERR_PCD_UNKNOWN_DATATYPE, tkn );
    return false;
  }
  return true;
}

static const struct _zPCDProperty{
  const char *key;
  bool (* read)(FILE*,_zPCD*,char*,char*);
} __z_pcd_property[] = {
  { "VERSION",   _zPCDVersionFRead },
  { "FIELDS",    _zPCDFieldsFRead },
  { "SIZE",      _zPCDSizeFRead },
  { "TYPE",      _zPCDValueTypeFRead },
  { "COUNT",     _zPCDCountFRead },
  { "WIDTH",     _zPCDWidthFRead },
  { "HEIGHT",    _zPCDHeightFRead },
  { "VIEWPOINT", _zPCDViewpointFRead },
  { "POINTS",    _zPCDPointsFRead },
  { "DATA",      _zPCDDataTypeFRead },
  { NULL, NULL },
};

static bool _zPCDHeaderFRead(FILE *fp, _zPCD *pcd)
{
  char buf[BUFSIZ], tkn[BUFSIZ];
  struct _zPCDProperty *property;

  for( property=(struct _zPCDProperty *)__z_pcd_property; property->key; ){
    if( fgets( buf, BUFSIZ, fp ) == NULL ) break;
    if( buf[0] == '#' ) continue; /* comment */
    if( !*zSToken( buf, tkn, BUFSIZ ) ) break;
    for( ; strcmp( tkn, property->key ) != 0; property++ ){
      if( !property->key ){
        ZRUNERROR( ZEO_ERR_PCD_UNKNOWN_KEY, tkn );
        return false;
      }
    }
    if( !property->read( fp, pcd, buf, tkn ) ) return false;
    if( strcmp( property->key, "DATA" ) == 0 ) return true;
  }
  ZRUNERROR( ZEO_ERR_PCD_INVALID_HEADER );
  return false;
}

static bool _zPCDDataASCIIFRead(FILE *fp, _zPCD *pcd, zVec3DData *data)
{
  char buf[BUFSIZ], tkn[BUFSIZ];
  int i;
  zVec3D v, tf;

  while( fgets( buf, BUFSIZ, fp ) ){
    zVec3DZero( &v );
    for( i=0; i<pcd->fieldnum; i++ ){
      if( !*zSToken( buf, tkn, BUFSIZ ) ){
        ZRUNWARN( ZEO_WARN_PCD_LACKOFDATA );
        break;
      }
      if( pcd->field[i].read_ascii )
        pcd->field[i].read_ascii( &pcd->field[i], tkn, &v );
    }
    if( !zVec3DIsNan( &v ) ){
      zXform3D( &pcd->viewpoint, &v, &tf );
      zVec3DDataAdd( data, &tf );
    }
  }
  return true;
}

static bool _zPCDDataBINFRead(FILE *fp, _zPCD *pcd, zVec3DData *data)
{
  int i;
  zVec3D v, tf;

  while( !feof( fp ) ){
    zVec3DZero( &v );
    for( i=0; i<pcd->fieldnum; i++ ){
      if( pcd->field[i].read_bin )
        pcd->field[i].read_bin( &pcd->field[i], fp, &v );
    }
    if( !zVec3DIsNan( &v ) ){
      zXform3D( &pcd->viewpoint, &v, &tf );
      zVec3DDataAdd( data, &tf );
    }
  }
  return true;
}

static bool _zPCDDataBINCOMFRead(FILE *fp, _zPCD *pcd, zVec3DData *data)
{
#ifndef __ZEO_USE_PCD_BINARY_COMPRESSED
  ZRUNWARN( "binary_compressed not supported" );
  return false;
#else
  uint32_t size_compressed = 0;
  uint32_t size_decompressed = 0;
  uint32_t size_processed = 0;
  byte *data_compressed = NULL;
  byte *data_decompressed = NULL;
  byte *data_ptr;
  bool result = false;
  zVec3D *v;
  int i, j, n;

  if( fread( &size_compressed, sizeof(uint32_t), 1, fp ) != 1 ) return false;
  if( fread( &size_decompressed, sizeof(uint32_t), 1, fp ) != 1 ) return false;
  data_compressed = zAlloc( byte, size_compressed );
  data_decompressed = zAlloc( byte, size_decompressed );
  if( !data_compressed || !data_decompressed ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  if( ( size_processed = fread( data_compressed, sizeof(byte), size_compressed, fp ) ) != size_compressed ){
    ZRUNERROR( ZEO_ERR_PCD_FAILED_TO_READ_DATA, size_processed, size_compressed );
    goto TERMINATE;
  }
  if( ( size_processed = lzf_decompress( data_compressed, size_compressed, data_decompressed, size_decompressed ) ) != size_decompressed ){
    ZRUNERROR( ZEO_ERR_PCD_FAILED_TO_DECOMPRESS, size_processed, size_decompressed );
    goto TERMINATE;
  }
  data_ptr = data_decompressed;
  n = pcd->width * pcd->height;
  for( i=0; i<pcd->fieldnum; i++ ){
    if( _zPCDFieldIsCoord( &pcd->field[i] ) ){
      zVec3DDataRewind( data );
      for( j=0; j<n; j++ ){
        if( !( v = zVec3DDataFetch( data ) ) ) continue;
        pcd->field[i].read_bincom( &pcd->field[i], data_ptr, v );
        data_ptr += pcd->field[i].size;
      }
    } else
      data_ptr += pcd->field[i].size * n;
  }
  result = true;
 TERMINATE:
  zFree( data_compressed );
  zFree( data_decompressed );
  return result;
#endif /* __ZEO_USE_PCD_BINARY_COMPRESSED */
}

/* read point cloud from a stream of PCD file. */
bool zVec3DDataFReadPCD(FILE *fp, zVec3DData *data)
{
  _zPCD pcd;
  int i;
  bool result = false;

  _zPCDInit( &pcd );
  if( !_zPCDHeaderFRead( fp, &pcd ) ) return false;
  if( !zVec3DDataInitArray( data, pcd.width * pcd.height ) ) return false;
  switch( pcd.datatype ){
  case ZEO_PCD_DATATYPE_ASCII:
    for( i=0; i<pcd.fieldnum; i++ )
      _zPCDFieldAssignReadASCII( &pcd.field[i] );
    result = _zPCDDataASCIIFRead( fp, &pcd, data );
    break;
  case ZEO_PCD_DATATYPE_BINARY:
    for( i=0; i<pcd.fieldnum; i++ )
      _zPCDFieldAssignReadBIN( &pcd.field[i] );
    result = _zPCDDataBINFRead( fp, &pcd, data );
    break;
  case ZEO_PCD_DATATYPE_BINARY_COMPRESSED:
    for( i=0; i<pcd.fieldnum; i++ )
      _zPCDFieldAssignReadBINCOM( &pcd.field[i] );
    result = _zPCDDataBINCOMFRead( fp, &pcd, data );
    break;
  default: ;
  }
  free( pcd.field );
  if( !result )
    ZRUNERROR( ZEO_ERR_PCD_INVALID_DATATYPE );
  return result;
}

/* read point cloud from a PCD file. */
bool zVec3DDataReadPCDFile(zVec3DData *data, const char filename[])
{
  FILE *fp;
  bool ret;

  if( !( fp = zOpenFile( filename, (char *)ZEO_PCD_SUFFIX, (char *)"r" ) ) )
    return false;
  ret = zVec3DDataFReadPCD( fp, data );
  fclose( fp );
  return ret;
}

/* ********************************************************** */
/* PCD format encoder
 * ********************************************************** */

static void _zPCDHeaderFWrite(FILE *fp, zVec3DData *data)
{
  fprintf( fp, "# Point Cloud Data generated by %s\n", __FUNCTION__ );
  fprintf( fp, "VERSION .7\n" );
  fprintf( fp, "FIELDS x y z\n" );
  fprintf( fp, "SIZE 8 8 8\n" );
  fprintf( fp, "TYPE F F F\n" );
  fprintf( fp, "COUNT 1 1 1\n" );
  fprintf( fp, "WIDTH %d\n", zVec3DDataSize(data) );
  fprintf( fp, "HEIGHT 1\n" );
  fprintf( fp, "VIEWPOINT 0 0 0 1 0 0 0\n" );
  fprintf( fp, "POINTS %d\n", zVec3DDataSize(data) );
}

static void _zPCDDataASCIIFWrite(FILE *fp, zVec3DData *data)
{
  zVec3D *v;

  fprintf( fp, "DATA ascii\n" );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zVec3DValueNLFPrint( fp, v );
}

static void _zPCDDataBinFWrite(FILE *fp, zVec3DData *data)
{
  zVec3D *v;

  fprintf( fp, "DATA binary\n" );
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    fwrite( v->e, sizeof(double), 3, fp );
}

/* write point cloud to a stream of PCD file. */
bool zVec3DDataFWritePCD(FILE *fp, zVec3DData *data, const char *format)
{
  _zPCDDataType type;

  _zPCDHeaderFWrite( fp, data );
  switch( ( type = (_zPCDDataType)_zPCDFieldAttrFind( format, __z_pcd_data_type ) ) ){
  case ZEO_PCD_DATATYPE_ASCII:  _zPCDDataASCIIFWrite( fp, data ); break;
  case ZEO_PCD_DATATYPE_BINARY: _zPCDDataBinFWrite( fp, data ); break;
  default: ZRUNERROR( ZEO_ERR_PCD_INVALID_FORMAT, format ); return false;
  }
  return true;
}

/* write point cloud to a PCD file. */
bool zVec3DDataWritePCDFile(zVec3DData *data, const char filename[], const char *format)
{
  FILE *fp;
  char filename_full[BUFSIZ];
  bool ret;

  zAddSuffix( filename, ZEO_PCD_SUFFIX, filename_full, BUFSIZ );
  if( !( fp = fopen( filename_full, "w" ) ) )
    return false;
  ret = zVec3DDataFWritePCD( fp, data, format );
  fclose( fp );
  return ret;
}

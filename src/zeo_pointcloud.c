/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_pointcloud - 3D point cloud.
 */

#include <zeo/zeo_pointcloud.h>

/* ********************************************************** */
/* PCD format decoder
 * ********************************************************** */

#define ZEO_PCD_FIELD_NUM_MAX 10

typedef enum{
  ZEO_PCD_NONE = -1,
  ZEO_PCD_X, ZEO_PCD_Y, ZEO_PCD_Z, ZEO_PCD_RGB,
  ZEO_PCD_NX, ZEO_PCD_NY, ZEO_PCD_NZ,
  ZEO_PCD_J1, ZEO_PCD_J2, ZEO_PCD_J3,
} _zPCDDef;
static const char *__z_pcd_def[] = {
  "x", "y", "z", "rgb",
  "normal_x", "normal_y", "normal_z",
  "j1", "j2", "j3",
  NULL,
};

typedef enum{
  ZEO_PCD_TYPE_INVALID = -1,
  ZEO_PCD_TYPE_INT,
  ZEO_PCD_TYPE_UINT,
  ZEO_PCD_TYPE_FP,
} _zPCDType;
static const char *__z_pcd_type[] = { "I", "U", "F", NULL };

typedef enum{
  ZEO_PCD_DATATYPE_INVALID = -1,
  ZEO_PCD_DATATYPE_ASCII,
  ZEO_PCD_DATATYPE_BINARY,
} _zPCDDataType;
static const char *__z_pcd_datatype[] = { "ascii", "binary", NULL };

typedef struct _zPCDField{
  _zPCDDef def;
  int size;
  int bitsize;
  _zPCDType type;
  int count;
  void (* read_ascii)(struct _zPCDField *, const char *, zVec3D *);
  void (* read_bin)(struct _zPCDField *, FILE *, zVec3D *);
} _zPCDField;

#define DEF_zPCDFieldReadASCIIFunc(type,a2v) \
  static void _zPCDFieldReadASCII##type(_zPCDField *field, const char *tkn, zVec3D *v){\
    type val;\
    val = a2v( tkn );\
    v->e[field->def] = (double)val;\
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

static void _zPCDFieldAssignReadASCII(_zPCDField *field)
{
  if( field->def == ZEO_PCD_X || field->def == ZEO_PCD_Y || field->def == ZEO_PCD_Z )
    field->read_ascii = __z_pcd_read_ascii[field->type*4+field->bitsize];
}

#define DEF_zPCDFieldReadBINFunc(type,a2v) \
  static void _zPCDFieldReadBIN##type(_zPCDField *field, FILE *fp, zVec3D *v){\
    type val;\
    if( fread( &val, field->size, 1, fp ) == 1 )\
      v->e[field->def] = (double)val;\
  }
DEF_zPCDFieldReadBINFunc( int8_t, atoi );
DEF_zPCDFieldReadBINFunc( uint8_t, atoi );
DEF_zPCDFieldReadBINFunc( int16_t, atoi );
DEF_zPCDFieldReadBINFunc( uint16_t, atoi );
DEF_zPCDFieldReadBINFunc( int32_t, atoi );
DEF_zPCDFieldReadBINFunc( uint32_t, atoi );
DEF_zPCDFieldReadBINFunc( int64_t, atoi );
DEF_zPCDFieldReadBINFunc( uint64_t, atoi );
DEF_zPCDFieldReadBINFunc( float, atof );
DEF_zPCDFieldReadBINFunc( double, atof );

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

#define DEF_zPCDFieldSkipBINFunc(type,a2v) \
  static void _zPCDFieldSkipBIN##type(_zPCDField *field, FILE *fp, zVec3D *v){\
    type val;\
    if( fread( &val, field->size, 1, fp ) == 1 );\
  }
DEF_zPCDFieldSkipBINFunc( int8_t, atoi );
DEF_zPCDFieldSkipBINFunc( uint8_t, atoi );
DEF_zPCDFieldSkipBINFunc( int16_t, atoi );
DEF_zPCDFieldSkipBINFunc( uint16_t, atoi );
DEF_zPCDFieldSkipBINFunc( int32_t, atoi );
DEF_zPCDFieldSkipBINFunc( uint32_t, atoi );
DEF_zPCDFieldSkipBINFunc( int64_t, atoi );
DEF_zPCDFieldSkipBINFunc( uint64_t, atoi );
DEF_zPCDFieldSkipBINFunc( float, atof );
DEF_zPCDFieldSkipBINFunc( double, atof );

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
  if( field->def >= ZEO_PCD_X && field->def <= ZEO_PCD_Z )
    field->read_bin = __z_pcd_read_bin[field->type*4+field->bitsize];
  else
    field->read_bin = __z_pcd_skip_bin[field->type*4+field->bitsize];
}

static void _zPCDFieldInit(_zPCDField *field)
{
  field->def = ZEO_PCD_NONE;
  field->size = 0;
  field->bitsize = 0;
  field->type = ZEO_PCD_TYPE_INVALID;
  field->count = 0;
  field->read_ascii = NULL;
  field->read_bin = NULL;
}

typedef struct{
  int version_major;
  int version_minor;
  _zPCDField field[ZEO_PCD_FIELD_NUM_MAX];
  int fieldnum;
  int width;
  int height;
  zFrame3D viewpoint;
  _zPCDDataType datatype;
} _zPCD;

static void _zPCDInit(_zPCD *pcd);
static int _zPCDFieldAttrFind(char *tkn, const char *attrlist[]);
static bool _zPCDVersionFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDFieldsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDCheckFieldNum(_zPCD *pcd, int n);
static bool _zPCDSizeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDCountFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDWidthFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDHeightFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDViewpointFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDPointsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);
static bool _zPCDDataTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn);

static bool _zPCDHeaderFRead(FILE *fp, _zPCD *pcd);
static bool _zPCDDataASCIIFRead(FILE *fp, _zPCD *pcd, zVec3DList *pc);
static bool _zPCDDataBINFRead(FILE *fp, _zPCD *pcd, zVec3DList *pc);

void _zPCDInit(_zPCD *pcd)
{
  int i;

  pcd->version_major = pcd->version_minor = 0;
  for( i=0; i<ZEO_PCD_FIELD_NUM_MAX; i++ ){
    _zPCDFieldInit( &pcd->field[i] );
  }
  pcd->fieldnum = 0;
  pcd->width = pcd->height = 0;
  zFrame3DIdent( &pcd->viewpoint );
  pcd->datatype = ZEO_PCD_DATATYPE_INVALID;
}

int _zPCDFieldAttrFind(char *tkn, const char *attrlist[])
{
  int i;

  for( i=0; attrlist[i]; i++ )
    if( strcmp( tkn, attrlist[i] ) == 0 ) return i;
  return -1;
}

bool _zPCDVersionFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  char *cp;

  if( ( cp = strchr( buf, '.' ) ) == NULL ){
    ZRUNERROR( "invalid string for version: %s", buf );
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

bool _zPCDFieldsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; zSToken( buf, tkn, BUFSIZ ) && i<ZEO_PCD_FIELD_NUM_MAX; i++ ){
    if( ( pcd->field[i].def = _zPCDFieldAttrFind( tkn, __z_pcd_def ) ) < 0 ){
      ZRUNERROR( "unknown field identifier: %s", tkn );
      return false;
    }
  }
  pcd->fieldnum = i;
  return true;
}

bool _zPCDCheckFieldNum(_zPCD *pcd, int n)
{
  if( n != pcd->fieldnum ){
    ZRUNERROR( "mismatch number of fields: %d", n );
    return false;
  }
  return true;
}

bool _zPCDSizeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; zSToken( buf, tkn, BUFSIZ ) && i<ZEO_PCD_FIELD_NUM_MAX; i++ ){
    switch( ( pcd->field[i].size = atoi( tkn ) ) ){
    case 1: pcd->field[i].bitsize = 0; break;
    case 2: pcd->field[i].bitsize = 1; break;
    case 4: pcd->field[i].bitsize = 2; break;
    case 8: pcd->field[i].bitsize = 3; break;
    default:
      ZRUNERROR( "invalid field size: %d", pcd->field[i].size );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

bool _zPCDTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; zSToken( buf, tkn, BUFSIZ ) && i<ZEO_PCD_FIELD_NUM_MAX; i++ ){
    if( ( pcd->field[i].type = _zPCDFieldAttrFind( tkn, __z_pcd_type ) ) < 0 ){
      ZRUNERROR( "unknown field type identifier: %s", tkn );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

bool _zPCDCountFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int i;

  for( i=0; zSToken( buf, tkn, BUFSIZ ) && i<ZEO_PCD_FIELD_NUM_MAX; i++ ){
    if( ( pcd->field[i].count = atoi( tkn ) ) != 1 ){
      ZRUNERROR( "can only handle 3D point cloud" );
      return false;
    }
  }
  return _zPCDCheckFieldNum( pcd, i );
}

bool _zPCDWidthFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( zSToken( buf, tkn, BUFSIZ ) == NULL ){
    ZRUNERROR( "width not specified" );
    return false;
  }
  pcd->width = atoi( tkn );
  return true;
}

bool _zPCDHeightFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( zSToken( buf, tkn, BUFSIZ ) == NULL ){
    ZRUNERROR( "height not specified" );
    return false;
  }
  pcd->height = atoi( tkn );
  return true;
}

bool _zPCDViewpointFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  double val[7];
  int i;
  zEP ep;

  for( i=0; i<7; i++ ){
    if( zSToken( buf, tkn, BUFSIZ ) == NULL ){
      ZRUNERROR( "short of data to specify viewpoint" );
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

bool _zPCDPointsFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  int points;

  if( zSToken( buf, tkn, BUFSIZ ) == NULL ){
    ZRUNERROR( "number of points not specified" );
    return false;
  }
  points = atoi( tkn );
  if( points != pcd->width * pcd->height ){
    ZRUNERROR( "inconsistent number of points: %d VS %d x %d",
      points, pcd->width, pcd->height );
    return false;
  }
  return true;
}

bool _zPCDDataTypeFRead(FILE *fp, _zPCD *pcd, char *buf, char *tkn)
{
  if( zSToken( buf, tkn, BUFSIZ ) == NULL ){
    ZRUNERROR( "data type not specified" );
    return false;
  }
  if( ( pcd->datatype = _zPCDFieldAttrFind( tkn, __z_pcd_datatype ) ) < 0 ){
    ZRUNERROR( "unknown data type identifier: %s", tkn );
    return false;
  }
  return true;
}

static const struct _zPCDProperty{
  char *key;
  bool (* read)(FILE*,_zPCD*,char*,char*);
} __z_pcd_property[] = {
  { "VERSION",   _zPCDVersionFRead },
  { "FIELDS",    _zPCDFieldsFRead },
  { "SIZE",      _zPCDSizeFRead },
  { "TYPE",      _zPCDTypeFRead },
  { "COUNT",     _zPCDCountFRead },
  { "WIDTH",     _zPCDWidthFRead },
  { "HEIGHT",    _zPCDHeightFRead },
  { "VIEWPOINT", _zPCDViewpointFRead },
  { "POINTS",    _zPCDPointsFRead },
  { "DATA",      _zPCDDataTypeFRead },
  { NULL, NULL },
};

bool _zPCDHeaderFRead(FILE *fp, _zPCD *pcd)
{
  char buf[BUFSIZ], tkn[BUFSIZ];
  struct _zPCDProperty *property;

  for( property=(struct _zPCDProperty *)__z_pcd_property; property->key; ){
    if( fgets( buf, BUFSIZ, fp ) == NULL ) break;
    if( buf[0] == '#' ) continue; /* comment */
    if( zSToken( buf, tkn, BUFSIZ ) == NULL ) break;
    if( strcmp( tkn, property->key ) != 0 ) continue;
    if( !property->read( fp, pcd, buf, tkn ) ) return false;
    if( strcmp( property->key, "DATA" ) == 0 ) return true;
    property++;
  }
  ZRUNERROR( "invalid PCD file header" );
  return false;
}

bool _zPCDDataASCIIFRead(FILE *fp, _zPCD *pcd, zVec3DList *pc)
{
  char buf[BUFSIZ], tkn[BUFSIZ];
  int i;
  zVec3D v, tf;

  while( fgets( buf, BUFSIZ, fp ) ){
    zVec3DClear( &v );
    for( i=0; i<pcd->fieldnum; i++ ){
      if( !zSToken( buf, tkn, BUFSIZ ) ){
        ZRUNWARN( "short of data" );
        break;
      }
      if( pcd->field[i].read_ascii )
        pcd->field[i].read_ascii( &pcd->field[i], tkn, &v );
    }
    if( !zVec3DIsNan( &v ) ){
      zXform3D( &pcd->viewpoint, &v, &tf );
      zVec3DListInsert( pc, &tf );
    }
  }
  return true;
}

bool _zPCDDataBINFRead(FILE *fp, _zPCD *pcd, zVec3DList *pc)
{
  int i;
  zVec3D v, tf;

  while( !feof( fp ) ){
    zVec3DClear( &v );
    for( i=0; i<pcd->fieldnum; i++ ){
      if( pcd->field[i].read_bin )
        pcd->field[i].read_bin( &pcd->field[i], fp, &v );
    }
    if( !zVec3DIsNan( &v ) ){
      zXform3D( &pcd->viewpoint, &v, &tf );
      zVec3DListInsert( pc, &tf );
    }
  }
  return true;
}

/* zVec3DListPCDFRead
 * - read point cloud from a stream of PCD file.
 */
bool zVec3DListPCDFRead(FILE *fp, zVec3DList *pc)
{
  _zPCD pcd;
  int i;

  zListInit( pc );
  _zPCDInit( &pcd );
  if( !_zPCDHeaderFRead( fp, &pcd ) ) return false;
  if( pcd.datatype == ZEO_PCD_DATATYPE_ASCII ){
    for( i=0; i<ZEO_PCD_FIELD_NUM_MAX; i++ )
      _zPCDFieldAssignReadASCII( &pcd.field[i] );
    _zPCDDataASCIIFRead( fp, &pcd, pc );
  } else
  if( pcd.datatype == ZEO_PCD_DATATYPE_BINARY ){
    for( i=0; i<ZEO_PCD_FIELD_NUM_MAX; i++ )
      _zPCDFieldAssignReadBIN( &pcd.field[i] );
    _zPCDDataBINFRead( fp, &pcd, pc );
  } else{
    ZRUNERROR( "invalid data type" );
    return false;
  }
  return true;
}

/* zVec3DListReadPCDFile
 * - read point cloud from PCD file.
 */
bool zVec3DListReadPCDFile(zVec3DList *pc, char filename[])
{
  FILE *fp;
  bool ret;

  if( !( fp = zOpenFile( filename, ZEO_PCD_SUFFIX, "r" ) ) )
    return false;
  ret = zVec3DListPCDFRead( fp, pc );
  fclose( fp );
  return ret;
}

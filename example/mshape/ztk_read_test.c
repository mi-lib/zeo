#include <zeo/zeo_mshape.h>
#include <zeo/zeo_bv_aabb.h>
#include <zeo/zeo_bv_obb.h>

#define ZTK_WARN_TOOMANY_TAGS "too many tag %s specified, skipped."
#define ZTK_WARN_TOOMANY_KEYS "too many key %s specified, skipped."

#define ZEO_ERR_PH_INVALID_VERT_ID "%d: invalid identifier of a vertex specified."

#define ZEO_WARN_PH_EMPTY "empty set of vertices assigned for a polyhedron."
#define ZEO_WARN_PH_VERT_UNMATCH "%d: unmatched identifier of a vertex"

#define ZEO_WARN_SHAPE_BB_INVALID "bounding-box not generated because it is only for polyhedra."

#define ZEO_WARN_MSHAPE_EMPTY "empty shapes assigned."
#define ZEO_WARN_UNKNOWN_BB_TYPE "%s: unknown type of bounding-box"

/*! \struct ZTKPrp
 *
 * \brief properties of a class described by a set of key string, encoding function and print-out function.
 */
typedef struct{
  char *key;
  int num;
  void *(* _encode)(void *, int, void *, ZTK *);
  void (* _fprintf)(FILE *, int, void *);
} ZTKKeyPrp;

bool ZTKDefListRegKeyPrp(ZTKDefList *list, char *tag, ZTKKeyPrp prp[], int num)
{
  register int i;

  for( i=0; i<num; i++ )
    if( !ZTKDefListRegOne( list, tag, prp[i].key ) ) return false;
  return true;
}

#define ZTKDefRegKeyPrp(ztk,tag,prp) ZTKDefListRegKeyPrp( &(ztk)->deflist, tag, prp, sizeof(prp)/sizeof(ZTKKeyPrp) )

void *_ZTKKeyEncode(void *obj, void *arg, ZTK *ztk, ZTKKeyPrp prp[], int num)
{
  register int i;
  int *count;

  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( !( count = zAlloc( int, num ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  do{
    for( i=0; i<num; i++ )
      if( ZTKKeyCmp( ztk, prp[i].key ) && prp[i]._encode ){
        if( prp[i].num > 0 && count[i] >= prp[i].num ){
          ZRUNWARN( ZTK_WARN_TOOMANY_KEYS, prp[i].key );
        } else
        if( !prp[i]._encode( obj, count[i]++, arg, ztk ) ){
          obj = NULL;
          goto TERMINATE;
        }
        break;
      }
  } while( ZTKKeyNext(ztk) );
 TERMINATE:
  free( count );
  return obj;
}

void _ZTKKeyPrpFPrint(FILE *fp, void *obj, ZTKKeyPrp prp[], int num)
{
  register int i, j;

  for( i=0; i<num; i++ )
    if( prp[i]._fprintf ){
      for( j=0; j<prp[i].num; j++ ){
        fprintf( fp, "%s: ", prp[i].key );
        prp[i]._fprintf( fp, j, obj );
      }
    }
}

#define ZTKKeyEncode(obj,arg,ztk,prp) _ZTKKeyEncode( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKKeyPrp) )
#define ZTKKeyPrpFPrint(fp,obj,prp) _ZTKKeyPrpFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKKeyPrp) )


void *_ZTKTagEncode(void *obj, void *arg, ZTK *ztk, ZTKKeyPrp prp[], int num)
{
  register int i;
  int *count;

  if( !ZTKTagRewind( ztk ) ) return NULL;
  if( !( count = zAlloc( int, num ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  for( i=0; i<num; i++ ){
    ZTKTagRewind( ztk );
    if( prp[i]._encode ) do{
      if( ZTKTagCmp( ztk, prp[i].key ) ){
        if( prp[i].num > 0 && count[i] >= prp[i].num ){
          ZRUNWARN( ZTK_WARN_TOOMANY_TAGS, prp[i].key );
        } else
        if( !prp[i]._encode( obj, count[i]++, arg, ztk ) ){
          obj = NULL;
          goto TERMINATE;
        }
      }
    } while( ZTKTagNext(ztk) );
  }
 TERMINATE:
  free( count );
  return obj;
}

void _ZTKTagPrpFPrint(FILE *fp, void *obj, ZTKKeyPrp prp[], int num)
{
  register int i, j;

  for( i=0; i<num; i++ )
    if( prp[i]._fprintf ){
      for( j=0; j<prp[i].num; j++ ){
        fprintf( fp, "[%s]\n", prp[i].key );
        prp[i]._fprintf( fp, j, obj );
      }
    }
}

#define ZTKTagEncode(obj,arg,ztk,prp) _ZTKTagEncode( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKKeyPrp) )
#define ZTKTagPrpFPrint(fp,obj,prp) _ZTKTagPrpFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKKeyPrp) )




zRGB *zRGBFromZTK(zRGB *rgb, ZTK *ztk)
{
  rgb->r = ZTKDouble(ztk);
  rgb->g = ZTKDouble(ztk);
  rgb->b = ZTKDouble(ztk);
  return rgb;
}

void *_zOpticalInfoNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zOpticalInfo *)obj, ZTKVal(ztk) );
  return zNamePtr((zOpticalInfo *)obj) ? obj : NULL; }
void *_zOpticalInfoAmbFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->amb, ztk );
  return obj; }
void *_zOpticalInfoDifFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->dif, ztk );
  return obj; }
void *_zOpticalInfoSpcFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->spc, ztk );
  return obj; }
void *_zOpticalInfoEsrFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->esr = ZTKDouble(ztk);
  return obj; }
void *_zOpticalInfoSnsFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->sns = ZTKDouble(ztk);
  return obj; }
void *_zOpticalInfoAlphaFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->alpha = ZTKDouble(ztk);
  return obj; }

void _zOpticalInfoNameFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zOpticalInfo*)obj) ); }
void _zOpticalInfoAmbFPrint(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->amb ); }
void _zOpticalInfoDifFPrint(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->dif ); }
void _zOpticalInfoSpcFPrint(FILE *fp, int i, void *obj){
  zRGBFPrint( fp, &((zOpticalInfo*)obj)->spc ); }
void _zOpticalInfoEsrFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->esr ); }
void _zOpticalInfoSnsFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->sns ); }
void _zOpticalInfoAlphaFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->alpha ); }

static ZTKKeyPrp __ztk_prp_optic[] = {
  { "name", 1, _zOpticalInfoNameFromZTK, _zOpticalInfoNameFPrint },
  { "ambient", 1, _zOpticalInfoAmbFromZTK, _zOpticalInfoAmbFPrint },
  { "diffuse", 1, _zOpticalInfoDifFromZTK, _zOpticalInfoDifFPrint },
  { "specular", 1, _zOpticalInfoSpcFromZTK, _zOpticalInfoSpcFPrint },
  { "esr", 1, _zOpticalInfoEsrFromZTK, _zOpticalInfoEsrFPrint },
  { "shininess", 1, _zOpticalInfoSnsFromZTK, _zOpticalInfoSnsFPrint },
  { "alpha", 1, _zOpticalInfoAlphaFromZTK, _zOpticalInfoAlphaFPrint },
};

bool zOpticalInfoRegZTK(ZTK *ztk)
{
  return ZTKDefRegKeyPrp( ztk, ZTK_TAG_OPTIC, __ztk_prp_optic );
}

zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk)
{
  zOpticalInfoInit( oi );
  return ZTKKeyEncode( oi, NULL, ztk, __ztk_prp_optic );
}

void _zOpticalInfoFPrint(FILE *fp, zOpticalInfo *oi)
{
  ZTKKeyPrpFPrint( fp, oi, __ztk_prp_optic );
  fprintf( fp, "\n" );
}





zVec3D *zVec3DFromZTK(zVec3D *v, ZTK *ztk)
{
  v->e[0] = ZTKDouble(ztk);
  v->e[1] = ZTKDouble(ztk);
  v->e[2] = ZTKDouble(ztk);
  return v;
}


zVec3D *zBox3DAxisFromZTK(zBox3D *box, int i0, int i1, int i2, ZTK *ztk)
{
  if( ZTKValCmp( ztk, "auto" ) )
    zVec3DOuterProd( zBox3DAxis(box,i1), zBox3DAxis(box,i2), zBox3DAxis(box,i0) );
  else
    zVec3DFromZTK( zBox3DAxis(box,i0), ztk );
  zVec3DNormalizeDRC( zBox3DAxis(box,i0) );
  return zBox3DAxis(box,i0);
}

void *_zBox3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zVec3DFromZTK( zBox3DCenter((zBox3D*)obj), ztk ) ? obj : NULL; }
void *_zBox3DAxisXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DAxisFromZTK( (zBox3D*)obj, 0, 1, 2, ztk );
  return obj; }
void *_zBox3DAxisYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DAxisFromZTK( (zBox3D*)obj, 1, 2, 0, ztk );
  return obj; }
void *_zBox3DAxisZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DAxisFromZTK( (zBox3D*)obj, 2, 0, 1, ztk );
  return obj; }
void *_zBox3DDepthFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DDepth((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }
void *_zBox3DWidthFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DWidth((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }
void *_zBox3DHeightFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zBox3DHeight((zBox3D*)obj) = ZTKDouble(ztk);
  return obj; }

void _zBox3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DCenter((zBox3D*)obj) ); }
void _zBox3DAxisXFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zX) ); }
void _zBox3DAxisYFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zY) ); }
void _zBox3DAxisZFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zZ) ); }
void _zBox3DDepthFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DDepth((zBox3D*)obj) ); }
void _zBox3DWidthFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DWidth((zBox3D*)obj) ); }
void _zBox3DHeightFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zBox3DHeight((zBox3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_box[] = {
  { "center", 1, _zBox3DCenterFromZTK, _zBox3DCenterFPrint },
  { "ax", 1, _zBox3DAxisXFromZTK, _zBox3DAxisXFPrint },
  { "ay", 1, _zBox3DAxisYFromZTK, _zBox3DAxisYFPrint },
  { "az", 1, _zBox3DAxisZFromZTK, _zBox3DAxisZFPrint },
  { "depth", 1, _zBox3DDepthFromZTK, _zBox3DDepthFPrint },
  { "width", 1, _zBox3DWidthFromZTK, _zBox3DWidthFPrint },
  { "height", 1, _zBox3DHeightFromZTK, _zBox3DHeightFPrint },
};

zBox3D *zBox3DFromZTK(zBox3D *box, ZTK *ztk)
{
  zBox3DInit( box );
  return ZTKKeyEncode( box, NULL, ztk, __ztk_prp_prim_box );
}

void _zBox3DFPrint(FILE *fp, zBox3D *box)
{
  ZTKKeyPrpFPrint( fp, box, __ztk_prp_prim_box );
  fprintf( fp, "\n" );
}



int zPrimDivFromZTK(ZTK *ztk)
{
  int val;
  return ( val = ZTKInt(ztk) ) > 0 ? val : ZEO_PRIM_DEFAULT_DIV;
}

void *_zSphere3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zSphere3DCenter((zSphere3D*)obj), ztk );
  return obj;
}
void *_zSphere3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zSphere3DRadius((zSphere3D*)obj) = ZTKDouble(ztk);
  return obj;
}
void *_zSphere3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zSphere3DDiv((zSphere3D*)obj) = zPrimDivFromZTK(ztk);
  return obj; }

void _zSphere3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zSphere3DCenter((zSphere3D*)obj) ); }
void _zSphere3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zSphere3DRadius((zSphere3D*)obj) ); }
void _zSphere3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zSphere3DDiv((zSphere3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_sphere[] = {
  { "center", 1, _zSphere3DCenterFromZTK, _zSphere3DCenterFPrint },
  { "radius", 1, _zSphere3DRadiusFromZTK, _zSphere3DRadiusFPrint },
  { "div", 1, _zSphere3DDivFromZTK, _zSphere3DDivFPrint },
};

zSphere3D *zSphere3DFromZTK(zSphere3D *sphere, ZTK *ztk)
{
  zSphere3DInit( sphere );
  return ZTKKeyEncode( sphere, NULL, ztk, __ztk_prp_prim_sphere );
}

void _zSphere3DFPrint(FILE *fp, zSphere3D *sphere)
{
  ZTKKeyPrpFPrint( fp, sphere, __ztk_prp_prim_sphere );
  fprintf( fp, "\n" );
}



void *_zCyl3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCyl3DCenter((zCyl3D*)obj,i), ztk );
  return obj;
}
void *_zCyl3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCyl3DRadius((zCyl3D*)obj) = ZTKDouble(ztk);
  return obj;
}
void *_zCyl3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCyl3DDiv((zCyl3D*)obj) = zPrimDivFromZTK(ztk);
  return obj; }

void _zCyl3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCyl3DCenter((zCyl3D*)obj,i) ); }
void _zCyl3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCyl3DRadius((zCyl3D*)obj) ); }
void _zCyl3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCyl3DDiv((zCyl3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_cyl[] = {
  { "center", 2, _zCyl3DCenterFromZTK, _zCyl3DCenterFPrint },
  { "radius", 1, _zCyl3DRadiusFromZTK, _zCyl3DRadiusFPrint },
  { "div", 1, _zCyl3DDivFromZTK, _zCyl3DDivFPrint },
};

zCyl3D *zCyl3DFromZTK(zCyl3D *cyl, ZTK *ztk)
{
  zCyl3DInit( cyl );
  return ZTKKeyEncode( cyl, NULL, ztk, __ztk_prp_prim_cyl );
}

void _zCyl3DFPrint(FILE *fp, zCyl3D *cyl)
{
  ZTKKeyPrpFPrint( fp, cyl, __ztk_prp_prim_cyl );
  fprintf( fp, "\n" );
}




void *_zCone3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCone3DCenter((zCone3D*)obj), ztk );
  return obj;
}
void *_zCone3DVertFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zCone3DVert((zCone3D*)obj), ztk );
  return obj;
}
void *_zCone3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCone3DRadius((zCone3D*)obj) = ZTKDouble(ztk);
  return obj;
}
void *_zCone3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zCone3DDiv((zCone3D*)obj) = zPrimDivFromZTK(ztk);
  return obj; }

void _zCone3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DCenter((zCone3D*)obj) ); }
void _zCone3DVertFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zCone3DVert((zCone3D*)obj) ); }
void _zCone3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zCone3DRadius((zCone3D*)obj) ); }
void _zCone3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zCone3DDiv((zCone3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_cone[] = {
  { "center", 1, _zCone3DCenterFromZTK, _zCone3DCenterFPrint },
  { "vert", 1, _zCone3DVertFromZTK, _zCone3DVertFPrint },
  { "radius", 1, _zCone3DRadiusFromZTK, _zCone3DRadiusFPrint },
  { "div", 1, _zCone3DDivFromZTK, _zCone3DDivFPrint },
};

zCone3D *zCone3DFromZTK(zCone3D *cone, ZTK *ztk)
{
  zCone3DInit( cone );
  return ZTKKeyEncode( cone, NULL, ztk, __ztk_prp_prim_cone );
}

void _zCone3DFPrint(FILE *fp, zCone3D *cone)
{
  ZTKKeyPrpFPrint( fp, cone, __ztk_prp_prim_cone );
}




zVec3D *zEllips3DAxisFromZTK(zEllips3D *ellips, int i0, int i1, int i2, ZTK *ztk)
{
  if( ZTKValCmp( ztk, "auto" ) )
    zVec3DOuterProd( zEllips3DAxis(ellips,i1), zEllips3DAxis(ellips,i2), zEllips3DAxis(ellips,i0) );
  else
    zVec3DFromZTK( zEllips3DAxis(ellips,i0), ztk );
  zVec3DNormalizeDRC( zEllips3DAxis(ellips,i0) );
  return zEllips3DAxis(ellips,i0);
}

void *_zEllips3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zEllips3DCenter((zEllips3D*)obj), ztk );
  return obj; }
void *_zEllips3DAxisXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DAxisFromZTK( (zEllips3D*)obj, 0, 1, 2, ztk );
  return obj; }
void *_zEllips3DAxisYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DAxisFromZTK( (zEllips3D*)obj, 1, 2, 0, ztk );
  return obj; }
void *_zEllips3DAxisZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DAxisFromZTK( (zEllips3D*)obj, 2, 0, 1, ztk );
  return obj; }
void *_zEllips3DRadiusXFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusX((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
void *_zEllips3DRadiusYFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusY((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
void *_zEllips3DRadiusZFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DRadiusZ((zEllips3D*)obj) = ZTKDouble(ztk);
  return obj; }
void *_zEllips3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zEllips3DDiv((zEllips3D*)obj) = zPrimDivFromZTK(ztk);
  return obj; }

void _zEllips3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DCenter((zEllips3D*)obj) ); }
void _zEllips3DAxisXFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zX) ); }
void _zEllips3DAxisYFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zY) ); }
void _zEllips3DAxisZFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zZ) ); }
void _zEllips3DRadiusXFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusX((zEllips3D*)obj) ); }
void _zEllips3DRadiusYFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusY((zEllips3D*)obj) ); }
void _zEllips3DRadiusZFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zEllips3DRadiusZ((zEllips3D*)obj) ); }
void _zEllips3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zEllips3DDiv((zEllips3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_ellips[] = {
  { "center", 1, _zEllips3DCenterFromZTK, _zEllips3DCenterFPrint },
  { "ax", 1, _zEllips3DAxisXFromZTK, _zEllips3DAxisXFPrint },
  { "ay", 1, _zEllips3DAxisYFromZTK, _zEllips3DAxisYFPrint },
  { "az", 1, _zEllips3DAxisZFromZTK, _zEllips3DAxisZFPrint },
  { "rx", 1, _zEllips3DRadiusXFromZTK, _zEllips3DRadiusXFPrint },
  { "ry", 1, _zEllips3DRadiusYFromZTK, _zEllips3DRadiusYFPrint },
  { "rz", 1, _zEllips3DRadiusZFromZTK, _zEllips3DRadiusZFPrint },
  { "div", 1, _zEllips3DDivFromZTK, _zEllips3DDivFPrint },
};

zEllips3D *zEllips3DFromZTK(zEllips3D *ellips, ZTK *ztk)
{
  zEllips3DInit( ellips );
  return ZTKKeyEncode( ellips, NULL, ztk, __ztk_prp_prim_ellips );
}

void _zEllips3DFPrint(FILE *fp, zEllips3D *ellips)
{
  ZTKKeyPrpFPrint( fp, ellips, __ztk_prp_prim_ellips );
  fprintf( fp, "\n" );
}





void *_zECyl3DCenterFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3DFromZTK( zECyl3DCenter((zECyl3D*)obj,i), ztk );
  return obj; }
void *_zECyl3DRadiusFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zECyl3DRadius((zECyl3D*)obj,i) = ZTKDouble(ztk);
  return obj; }
void *_zECyl3DRefFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zVec3D ref;
  zVec3DFromZTK( &ref, ztk );
  zECyl3DDefAxis( (zECyl3D*)obj, &ref );
  return obj;
}
void *_zECyl3DDivFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zECyl3DDiv((zECyl3D*)obj) = zPrimDivFromZTK(ztk);
  return obj; }

void _zECyl3DCenterFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DCenter((zECyl3D*)obj,i) ); }
void _zECyl3DRadiusFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zECyl3DRadius((zECyl3D*)obj,i) ); }
void _zECyl3DRefFPrint(FILE *fp, int i, void *obj){
  zVec3DFPrint( fp, zECyl3DRadVec((zECyl3D*)obj,0) ); }
void _zECyl3DDivFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d\n", zECyl3DDiv((zECyl3D*)obj) ); }

static ZTKKeyPrp __ztk_prp_prim_ecyl[] = {
  { "center", 2, _zECyl3DCenterFromZTK, _zECyl3DCenterFPrint },
  { "radius", 2, _zECyl3DRadiusFromZTK, _zECyl3DRadiusFPrint },
  { "ref", 1, _zECyl3DRefFromZTK, _zECyl3DRefFPrint },
  { "div", 1, _zECyl3DDivFromZTK, _zECyl3DDivFPrint },
};

zECyl3D *zECyl3DFromZTK(zECyl3D *ecyl, ZTK *ztk)
{
  zECyl3DInit( ecyl );
  return ZTKKeyEncode( ecyl, NULL, ztk, __ztk_prp_prim_ecyl );
}

void _zECyl3DFPrint(FILE *fp, zECyl3D *ecyl)
{
  ZTKKeyPrpFPrint( fp, ecyl, __ztk_prp_prim_ecyl );
  fprintf( fp, "\n" );
}




void *_zPH3DVertFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int vi;

  if( ( vi = ZTKInt(ztk) ) != i )
    ZRUNWARN( ZEO_WARN_PH_VERT_UNMATCH, vi );
  zVec3DFromZTK( zPH3DVert((zPH3D*)obj,i), ztk );
  return obj;
}

void *_zPH3DFaceFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  int i0, i1, i2;

  if( ( i0 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i0 );
    return NULL;
  }
  if( ( i1 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i1 );
    return NULL;
  }
  if( ( i2 = ZTKInt(ztk) ) >= zPH3DVertNum((zPH3D*)obj) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i2 );
    return NULL;
  }
  zTri3DCreate( zPH3DFace((zPH3D*)obj,i),
    zPH3DVert((zPH3D*)obj,i0), zPH3DVert((zPH3D*)obj,i1), zPH3DVert((zPH3D*)obj,i2) );
  return obj;
}

static ZTKKeyPrp __ztk_prp_ph[] = {
  { "vert", -1, _zPH3DVertFromZTK, NULL },
  { "face", -1, _zPH3DFaceFromZTK, NULL },
};

zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk)
{
  int num_vert, num_face;

  zPH3DInit( ph );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  if( ( num_vert = ZTKCountKey( ztk, "vert" ) ) == 0 ){
    ZRUNWARN( ZEO_WARN_PH_EMPTY );
    return NULL;
  }
  num_face = ZTKCountKey( ztk, "face" );
  zArrayAlloc( &ph->vert, zVec3D, num_vert );
  zArrayAlloc( &ph->face, zTri3D, num_face );
  if( zPH3DVertNum(ph) != num_vert ||
      zPH3DFaceNum(ph) != num_face ) return NULL;
  /* vertices & faces */
  return ZTKKeyEncode( ph, NULL, ztk, __ztk_prp_ph );
}





zVec zNURBS3DKnotFromZTK(zNURBS3D *nurbs, int id, ZTK *ztk)
{
  register int i;

  if( !ZTKValRewind(ztk) ) return NULL;
  if( nurbs->knot[id] ){
    ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
    zVecFree( nurbs->knot[id] );
  }
  if( !( nurbs->knot[id] = zVecAlloc( ZTKInt(ztk) ) ) ) return NULL;
  for( i=0; i<zNURBS3DKnotNum(nurbs,id); i++ )
    zNURBS3DKnot(nurbs,id,i) = ZTKDouble(ztk);
  return nurbs->knot[id];
}

void *_zNURBS3DDimFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zNURBS3D*)obj)->dim[0] = ZTKInt( ztk );
  ((zNURBS3D*)obj)->dim[1] = ZTKInt( ztk );
  return obj;
}
void *_zNURBS3DUKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNURBS3DKnotFromZTK( obj, 0, ztk );
  return obj;
}
void *_zNURBS3DVKnotFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNURBS3DKnotFromZTK( obj, 1, ztk );
  return obj;
}
void *_zNURBS3DSliceFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zNURBS3D*)obj)->ns[0] = ZTKInt( ztk );
  ((zNURBS3D*)obj)->ns[1] = ZTKInt( ztk );
  return obj;
}
void *_zNURBS3DSizeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  int size1, size2;
  if( zNURBS3DCPNum((zNURBS3D*)obj,0) > 0 || zNURBS3DCPNum((zNURBS3D*)obj,1) > 0 ){
    ZRUNWARN( ZEO_ERR_NURBS_CPALREADY );
    zArray2Free( &((zNURBS3D*)obj)->cpnet );
  }
  size1 = ZTKInt( ztk );
  size2 = ZTKInt( ztk );
  zArray2Alloc( &((zNURBS3D*)obj)->cpnet, zNURBS3DCPCell, size1, size2 );
  if( zArray2RowSize(&((zNURBS3D*)obj)->cpnet) != size1 ||
      zArray2ColSize(&((zNURBS3D*)obj)->cpnet) != size2 ) return NULL;
  return obj;
}
void *_zNURBS3DCPFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  int j, k;
  j = ZTKInt( ztk );
  k = ZTKInt( ztk );
  if( !zArray2PosIsValid( &((zNURBS3D*)obj)->cpnet, j, k ) ){
    ZRUNERROR( ZEO_ERR_NURBS_INVCP );
    return NULL;
  }
  zNURBS3DSetWeight( ((zNURBS3D*)obj), j, k, ZTKDouble(ztk) );
  zVec3DFromZTK( zNURBS3DCP(((zNURBS3D*)obj),j,k), ztk );
  return obj;
}

void _zNURBS3DDimFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", ((zNURBS3D*)obj)->dim[0], ((zNURBS3D*)obj)->dim[1] );
}
void _zNURBS3DUKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->knot[0]) > 0 ) zVecFPrint( fp, ((zNURBS3D*)obj)->knot[0] );
}
void _zNURBS3DVKnotFPrint(FILE *fp, int i, void *obj){
  if( zVecSizeNC(((zNURBS3D*)obj)->knot[1]) > 0 ) zVecFPrint( fp, ((zNURBS3D*)obj)->knot[1] );
}
void _zNURBS3DSliceFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", ((zNURBS3D*)obj)->ns[0], ((zNURBS3D*)obj)->ns[1] );
}
void _zNURBS3DSizeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", zNURBS3DCPNum((zNURBS3D*)obj,0), zNURBS3DCPNum((zNURBS3D*)obj,1) );
}

static ZTKKeyPrp __ztk_prp_nurbs[] = {
  { "dim", 1, _zNURBS3DDimFromZTK, _zNURBS3DDimFPrint },
  { "uknot", 1, _zNURBS3DUKnotFromZTK, _zNURBS3DUKnotFPrint },
  { "vknot", 1, _zNURBS3DVKnotFromZTK, _zNURBS3DVKnotFPrint },
  { "size", 1, _zNURBS3DSizeFromZTK, _zNURBS3DSizeFPrint },
  { "cp", -1, _zNURBS3DCPFromZTK, NULL },
  { "slice", 1, _zNURBS3DSliceFromZTK, _zNURBS3DSliceFPrint },
};

zNURBS3D *zNURBS3DFromZTK(zNURBS3D *nurbs, ZTK *ztk)
{
  zNURBS3DInit( nurbs );
  return ZTKKeyEncode( nurbs, NULL, ztk, __ztk_prp_nurbs );
}

void _zNURBS3DFPrint(FILE *fp, zNURBS3D *nurbs)
{
  register int i, j;

  ZTKKeyPrpFPrint( fp, nurbs, __ztk_prp_nurbs );
  for( i=0; i<zNURBS3DCPNum(nurbs,0); i++ )
    for( j=0; j<zNURBS3DCPNum(nurbs,1); j++ ){
      fprintf( fp, "cp: %d %d %.12g ", i, j, zNURBS3DWeight(nurbs,i,j) );
      zVec3DFPrint( fp, zNURBS3DCP(nurbs,i,j) );
    }
}




typedef enum{ ZSHAPE_BB_NONE=0, ZSHAPE_BB_AABB, ZSHAPE_BB_OBB } zShape3DBBType;

typedef struct{
  zShape3D *sarray;
  int ns;
  zOpticalInfo *oarray;
  int no;
  zShape3DBBType bbtype;
  bool mirrored;
} _zShape3DRefProp;

void *_zShape3DNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zShape3D*)obj, ZTKVal(ztk) );
  return zNamePtr((zShape3D*)obj) ? obj : NULL;
}
void *_zShape3DTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3DType((zShape3D*)obj) = zShapeTypeByStr( ZTKVal(ztk) );
  return obj;
}
void *_zShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameFind( ((_zShape3DRefProp*)arg)->oarray, ((_zShape3DRefProp*)arg)->no, ZTKVal(ztk), zShape3DOptic((zShape3D*)obj) );
  return zShape3DOptic((zShape3D*)obj) ? obj : NULL;
}
void *_zShape3DBBTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  if( ZTKValCmp( ztk, "AABB" ) ) ((_zShape3DRefProp*)arg)->bbtype = ZSHAPE_BB_AABB;
  else
  if( ZTKValCmp( ztk, "OBB" ) ) ((_zShape3DRefProp*)arg)->bbtype = ZSHAPE_BB_OBB;
  else{
    ZRUNWARN( ZEO_WARN_UNKNOWN_BB_TYPE, ZTKVal(ztk) );
    ((_zShape3DRefProp*)arg)->bbtype = ZSHAPE_BB_NONE;
  }
  return obj;
}
void *_zShape3DMirrorFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zShape3D *ref;
  zNameFind( ((_zShape3DRefProp*)arg)->sarray, ((_zShape3DRefProp*)arg)->ns, ZTKVal(ztk), ref );
  if( ref ){
    if( !ZTKValNext(ztk) ) return NULL;
    if( !zShape3DMirror( ref, (zShape3D*)obj, zAxisByStr(ZTKVal(ztk)) ) ) return NULL;
    ((_zShape3DRefProp*)arg)->mirrored = true;
    return obj;
  }
  ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, ZTKVal(ztk) );
  return obj;
}

void _zShape3DNameFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zShape3D*)obj) );
}
void _zShape3DTypeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zShapeTypeExpr(zShape3DType((zShape3D*)obj)) );
}
void _zShape3DOpticFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName(zShape3DOptic((zShape3D*)obj)) );
}

static ZTKKeyPrp __ztk_prp_shape[] = {
  { "name", 1, _zShape3DNameFromZTK, _zShape3DNameFPrint },
  { "type", 1, _zShape3DTypeFromZTK, _zShape3DTypeFPrint },
  { "optic", 1, _zShape3DOpticFromZTK, _zShape3DOpticFPrint },
  { "bb", 1, _zShape3DBBTypeFromZTK, NULL },
  { "mirror", 1, _zShape3DMirrorFromZTK, NULL },
};

bool zShape3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_shape ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_box ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_sphere ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_cyl ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_cone ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_ellips ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_prim_ecyl ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_ph ) &&
         ZTKDefRegKeyPrp( ztk, ZTK_TAG_SHAPE, __ztk_prp_nurbs ) ? true : false;
}

zShape3D *zShape3DFromZTK(zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no, ZTK *ztk)
{
  zPrimCom *com[] = {
    &zprim_none_com,
    &zprim_ph3d_com, &zprim_box3d_com,
    &zprim_sphere3d_com, &zprim_ellips3d_com,
    &zprim_cyl3d_com, &zprim_ecyl3d_com, &zprim_cone3d_com,
    &zprim_nurbs_com,
  };
  _zShape3DRefProp prp;

  zShape3DInit( shape );
  /* type, name, associated optical info and mirroring */
  prp.sarray = sarray;
  prp.ns = ns;
  prp.oarray = oarray;
  prp.no = no;
  prp.bbtype = ZSHAPE_BB_NONE;
  prp.mirrored = false;
  if( !ZTKKeyEncode( shape, &prp, ztk, __ztk_prp_shape ) ) return NULL;
  if( prp.mirrored ) return shape;
  /* type-specific decoding */
  shape->com = com[zShape3DType(shape)];

  switch( zShape3DType(shape) ){ /* need to check the returned value for the final implementation */
  case ZSHAPE_BOX:
    zBox3DFromZTK( zShape3DBox(shape), ztk );
    break;
  case ZSHAPE_SPHERE:
    zSphere3DFromZTK( zShape3DSphere(shape), ztk );
    break;
  case ZSHAPE_CYLINDER:
    zCyl3DFromZTK( zShape3DCyl(shape), ztk );
    break;
  case ZSHAPE_CONE:
    zCone3DFromZTK( zShape3DCone(shape), ztk );
    break;
  case ZSHAPE_ELLIPS:
    zEllips3DFromZTK( zShape3DEllips(shape), ztk );
    break;
  case ZSHAPE_ELLIPTICCYLINDER:
    zECyl3DFromZTK( zShape3DECyl(shape), ztk );
    break;
  case ZSHAPE_PH:
    zPH3DFromZTK( zShape3DPH(shape), ztk );
    break;
  case ZSHAPE_NURBS:
    zNURBS3DFromZTK( zShape3DNURBS(shape), ztk );
    break;
  default: ;
  }
  /* bounding box */
  if( prp.bbtype != ZSHAPE_BB_NONE ){
    if( zShape3DType(shape) != ZSHAPE_PH ){
      ZRUNWARN( ZEO_WARN_SHAPE_BB_INVALID );
    } else{
      if( prp.bbtype == ZSHAPE_BB_AABB ){
        zAABox3D aabb;
        zAABB( &aabb, zShape3DVertBuf(shape), zShape3DVertNum(shape), NULL );
        zAABox3DToBox3D( &aabb, zShape3DBB(shape) );
      } else{ /* ZSHAPE_BB_OBB */
        zOBB( zShape3DBB(shape), zShape3DVertBuf(shape), zShape3DVertNum(shape) );
      }
    }
  }
  return shape;
}

void _zShape3DFPrint(FILE *fp, zShape3D *shape)
{
  if( !shape ) return;
  ZTKKeyPrpFPrint( fp, shape, __ztk_prp_shape );
  shape->com->_fprint( fp, &shape->body );
  fprintf( fp, "\n" );
}



void *_zMShape3DOpticFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zOpticalInfoFromZTK( zMShape3DOptic((zMShape3D*)obj,i), ztk ) ? obj : NULL;
}
void *_zMShape3DShapeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zShape3DFromZTK( zMShape3DShape((zMShape3D*)obj,i),
    zMShape3DShapeBuf((zMShape3D*)obj), zMShape3DShapeNum((zMShape3D*)obj),
    zMShape3DOpticBuf((zMShape3D*)obj), zMShape3DOpticNum((zMShape3D*)obj), ztk ) ? obj : NULL;
}

void _zMShape3DOpticFPrint(FILE *fp, int i, void *obj){
  _zOpticalInfoFPrint( fp, zMShape3DOptic((zMShape3D*)obj,i) );
}
void _zMShape3DShapeFPrint(FILE *fp, int i, void *obj){
  _zShape3DFPrint( fp, zMShape3DShape((zMShape3D*)obj,i) );
}

static ZTKKeyPrp __ztk_prp_mshape[] = {
  { "optic", -1, _zMShape3DOpticFromZTK, _zMShape3DOpticFPrint },
  { "shape", -1, _zMShape3DShapeFromZTK, _zMShape3DShapeFPrint },
};

zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk)
{
  int num_optic, num_shape;

  zMShape3DInit( ms );
  num_optic = ZTKCountTag( ztk, ZTK_TAG_OPTIC );
  num_shape = ZTKCountTag( ztk, ZTK_TAG_SHAPE );
  zArrayAlloc( &ms->optic, zOpticalInfo, num_optic );
  zArrayAlloc( &ms->shape, zShape3D, num_shape );
  if( zMShape3DOpticNum(ms) != num_optic ||
      zMShape3DShapeNum(ms) != num_shape ) return NULL;
  if( zMShape3DShapeNum(ms) == 0 ){
    ZRUNWARN( ZEO_WARN_MSHAPE_EMPTY );
    return NULL;
  }
  ZTKTagEncode( ms, NULL, ztk, __ztk_prp_mshape );
  return ms;
}

zMShape3D *zMShape3DParseZTK(zMShape3D *ms, char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  if( !zShape3DRegZTK( &ztk ) || !zOpticalInfoRegZTK( &ztk ) ) return NULL;
  ZTKParse( &ztk, filename );
  ms = zMShape3DFromZTK( ms, &ztk );
  ZTKDestroy( &ztk );
  return ms;
}

void _zMShape3DFPrint(FILE *fp, zMShape3D *ms)
{
  register int i;

  for( i=0; i<zMShape3DOpticNum(ms); i++ ){
    fprintf( fp, "[%s]\n", ZTK_TAG_OPTIC );
    _zOpticalInfoFPrint( fp, zMShape3DOptic(ms,i) );
  }
  for( i=0; i<zMShape3DShapeNum(ms); i++ ){
    fprintf( fp, "[%s]\n", ZTK_TAG_SHAPE );
    _zShape3DFPrint( fp, zMShape3DShape(ms,i) );
  }
}


int main(int argc, char *argv[])
{
  zMShape3D ms;

  if( argc <= 1 ) return 1;
  if( zMShape3DParseZTK( &ms, argv[1] ) )
    _zMShape3DFPrint( stdout, &ms );
  zMShape3DDestroy( &ms );
  return 0;
}

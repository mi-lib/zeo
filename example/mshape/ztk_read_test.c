#include <zeo/zeo_mshape.h>
#include <zeo/zeo_bv_aabb.h>
#include <zeo/zeo_bv_obb.h>

#define ZEO_ERR_PH_INVALID_VERT_ID "%d: invalid identifier of a vertex specified."

#define ZEO_WARN_PH_EMPTY "empty set of vertices assigned for a polyhedron."
#define ZEO_WARN_PH_VERT_UNMATCH "%d: unmatched identifier of a vertex"

#define ZEO_WARN_PRIM_TOOMANY_CENTER "too many centers, skipped."
#define ZEO_WARN_PRIM_TOOMANY_RADIUS "too many radii, skipped."
#define ZEO_WARN_PRIM_TOOMANY_VERT   "too many vertices, skipped."
#define ZEO_WARN_PRIM_TOOMANY_AXIS   "too many axes, skipped."

#define ZEO_WARN_SHAPE_BB_INVALID "bounding-box not generated because it is only for polyhedra."

#define ZEO_WARN_MSHAPE_EMPTY "empty shapes assigned."
#define ZEO_WARN_UNKNOWN_BB_TYPE "%s: unknown type of bounding-box"

/*! \struct ZTKProp
 *
 * \brief properties of a class described by a set of key string, encoding function and print-out function.
 */
typedef struct{
  char *key;
  int num;
  void (* _encode)(void *, void *, ZTK *);
  void (* _fprintf)(FILE *, int, void *);
} ZTKProp;

bool ZTKDefListRegProp(ZTKDefList *list, char *tag, ZTKProp prp[], int num)
{
  register int i;

  for( i=0; i<num; i++ )
    if( !ZTKDefListRegOne( list, tag, prp[i].key ) ) return false;
  return true;
}

#define ZTKDefRegProp(ztk,tag,prp) ZTKDefListRegProp( &(ztk)->deflist, tag, prp, sizeof(prp)/sizeof(ZTKProp) )

void *_ZTKEncode(void *obj, void *arg, ZTK *ztk, ZTKProp prp[], int num)
{
  register int i;

  if( !ZTKKeyRewind( ztk ) ) return NULL;
  do{
    for( i=0; i<num; i++ )
      if( ZTKKeyCmp( ztk, prp[i].key ) && prp[i]._encode ){
        prp[i]._encode( obj, arg, ztk );
        break;
      }
  } while( ZTKKeyNext(ztk) );
  return obj;
}

void _ZTKPropFPrint(FILE *fp, void *obj, ZTKProp prp[], int num)
{
  register int i, j;

  for( i=0; i<num; i++ )
    if( prp[i]._fprintf ){
      for( j=0; j<prp[i].num; j++ ){
        fprintf( fp, "%s: ", prp[i].key );
        prp[i]._fprintf( fp, j, obj );
      }
    }
  fprintf( fp, "\n" );
}

#define ZTKEncode(obj,arg,ztk,prp) _ZTKEncode( obj, arg, ztk, prp, sizeof(prp)/sizeof(ZTKProp) )
#define ZTKPropFPrint(fp,obj,prp) _ZTKPropFPrint( fp, obj, prp, sizeof(prp)/sizeof(ZTKProp) )




zRGB *zRGBFromZTK(zRGB *rgb, ZTK *ztk)
{
  rgb->r = ZTKDouble(ztk);
  rgb->g = ZTKDouble(ztk);
  rgb->b = ZTKDouble(ztk);
  return rgb;
}

void _zOpticalInfoNameFromZTK(void *obj, void *arg, ZTK *ztk){
  zNameSet( (zOpticalInfo *)obj, ZTKVal(ztk) ); }
void _zOpticalInfoAmbFromZTK(void *obj, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->amb, ztk ); }
void _zOpticalInfoDifFromZTK(void *obj, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->dif, ztk ); }
void _zOpticalInfoSpcFromZTK(void *obj, void *arg, ZTK *ztk){
  zRGBFromZTK( &((zOpticalInfo*)obj)->spc, ztk ); }
void _zOpticalInfoEsrFromZTK(void *obj, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->esr = ZTKDouble(ztk); }
void _zOpticalInfoSnsFromZTK(void *obj, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->sns = ZTKDouble(ztk); }
void _zOpticalInfoAlphaFromZTK(void *obj, void *arg, ZTK *ztk){
  ((zOpticalInfo*)obj)->alpha = ZTKDouble(ztk); }

void _zOpticalInfoNameFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%s\n", zName((zOpticalInfo*)obj) ); }
void _zOpticalInfoAmbFPrint(FILE *fp, int i, void *obj){ zRGBFPrint( fp, &((zOpticalInfo*)obj)->amb ); }
void _zOpticalInfoDifFPrint(FILE *fp, int i, void *obj){ zRGBFPrint( fp, &((zOpticalInfo*)obj)->dif ); }
void _zOpticalInfoSpcFPrint(FILE *fp, int i, void *obj){ zRGBFPrint( fp, &((zOpticalInfo*)obj)->spc ); }
void _zOpticalInfoEsrFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->esr ); }
void _zOpticalInfoSnsFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->sns ); }
void _zOpticalInfoAlphaFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", ((zOpticalInfo*)obj)->alpha ); }

static ZTKProp __ztk_prp_optic[] = {
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
  return ZTKDefRegProp( ztk, "optic", __ztk_prp_optic );
}

zOpticalInfo *zOpticalInfoFromZTK(zOpticalInfo *oi, ZTK *ztk)
{
  zOpticalInfoInit( oi );
  return ZTKEncode( oi, NULL, ztk, __ztk_prp_optic );
}

void _zOpticalInfoFPrint(FILE *fp, zOpticalInfo *oi)
{
  ZTKPropFPrint( fp, oi, __ztk_prp_optic );
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

void _zBox3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){ zVec3DFromZTK( zBox3DCenter((zBox3D*)obj), ztk ); }
void _zBox3DAxisXFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DAxisFromZTK( (zBox3D*)obj, 0, 1, 2, ztk ); }
void _zBox3DAxisYFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DAxisFromZTK( (zBox3D*)obj, 1, 2, 0, ztk ); }
void _zBox3DAxisZFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DAxisFromZTK( (zBox3D*)obj, 2, 0, 1, ztk ); }
void _zBox3DDepthFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DDepth((zBox3D*)obj) = ZTKDouble(ztk); }
void _zBox3DWidthFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DWidth((zBox3D*)obj) = ZTKDouble(ztk); }
void _zBox3DHeightFromZTK(void *obj, void *arg, ZTK *ztk){ zBox3DHeight((zBox3D*)obj) = ZTKDouble(ztk); }

void _zBox3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zBox3DCenter((zBox3D*)obj) ); }
void _zBox3DAxisXFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zX) ); }
void _zBox3DAxisYFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zY) ); }
void _zBox3DAxisZFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zBox3DAxis((zBox3D*)obj,zZ) ); }
void _zBox3DDepthFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zBox3DDepth((zBox3D*)obj) ); }
void _zBox3DWidthFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zBox3DWidth((zBox3D*)obj) ); }
void _zBox3DHeightFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zBox3DHeight((zBox3D*)obj) ); }

static ZTKProp __ztk_prp_prim_box[] = {
  { "center", 1, _zBox3DCenterFromZTK, _zBox3DCenterFPrint },
  { "ax", 1, _zBox3DAxisXFromZTK, _zBox3DAxisXFPrint },
  { "ay", 1, _zBox3DAxisYFromZTK, _zBox3DAxisYFPrint },
  { "az", 1, _zBox3DAxisZFromZTK, _zBox3DAxisZFPrint },
  { "depth", 1, _zBox3DDepthFromZTK, _zBox3DDepthFPrint },
  { "width", 1, _zBox3DWidthFromZTK, _zBox3DWidthFPrint },
  { "height", 1, _zBox3DHeightFromZTK, _zBox3DHeightFPrint },
};

bool zBox3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_box );
}

zBox3D *zBox3DFromZTK(zBox3D *box, ZTK *ztk)
{
  zBox3DInit( box );
  return ZTKEncode( box, NULL, ztk, __ztk_prp_prim_box );
}

void _zBox3DFPrint(FILE *fp, zBox3D *box)
{
  ZTKPropFPrint( fp, box, __ztk_prp_prim_box );
}



int zPrimDivFromZTK(ZTK *ztk)
{
  int val;
  return ( val = ZTKInt(ztk) ) > 0 ? val : ZEO_PRIM_DEFAULT_DIV;
}

void _zSphere3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[0]++ > 0 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_CENTER );
  else
    zVec3DFromZTK( zSphere3DCenter((zSphere3D*)obj), ztk );
}
void _zSphere3DRadiusFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[1]++ > 0 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_RADIUS );
  else
    zSphere3DRadius((zSphere3D*)obj) = ZTKDouble(ztk);
}
void _zSphere3DDivFromZTK(void *obj, void *arg, ZTK *ztk){
  zSphere3DDiv((zSphere3D*)obj) = zPrimDivFromZTK(ztk); }

void _zSphere3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zSphere3DCenter((zSphere3D*)obj) ); }
void _zSphere3DRadiusFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zSphere3DRadius((zSphere3D*)obj) ); }
void _zSphere3DDivFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%d\n", zSphere3DDiv((zSphere3D*)obj) ); }

static ZTKProp __ztk_prp_prim_sphere[] = {
  { "center", 1, _zSphere3DCenterFromZTK, _zSphere3DCenterFPrint },
  { "radius", 1, _zSphere3DRadiusFromZTK, _zSphere3DRadiusFPrint },
  { "div", 1, _zSphere3DDivFromZTK, _zSphere3DDivFPrint },
};

bool zSphere3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_sphere );
}

zSphere3D *zSphere3DFromZTK(zSphere3D *sphere, ZTK *ztk)
{
  int n[] = { 0, 0 };
  zSphere3DInit( sphere );
  return ZTKEncode( sphere, n, ztk, __ztk_prp_prim_sphere );
}

void _zSphere3DFPrint(FILE *fp, zSphere3D *sphere)
{
  ZTKPropFPrint( fp, sphere, __ztk_prp_prim_sphere );
}



void _zCyl3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[0] > 1 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_CENTER );
  else
    zVec3DFromZTK( zCyl3DCenter((zCyl3D*)obj,((int*)arg)[0]++), ztk );
}
void _zCyl3DRadiusFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[1]++ > 0 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_RADIUS );
  else
    zCyl3DRadius((zCyl3D*)obj) = ZTKDouble(ztk);
}
void _zCyl3DDivFromZTK(void *obj, void *arg, ZTK *ztk){ zCyl3DDiv((zCyl3D*)obj) = zPrimDivFromZTK(ztk); }

void _zCyl3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zCyl3DCenter((zCyl3D*)obj,i) ); }
void _zCyl3DRadiusFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zCyl3DRadius((zCyl3D*)obj) ); }
void _zCyl3DDivFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%d\n", zCyl3DDiv((zCyl3D*)obj) ); }

static ZTKProp __ztk_prp_prim_cyl[] = {
  { "center", 2, _zCyl3DCenterFromZTK, _zCyl3DCenterFPrint },
  { "radius", 1, _zCyl3DRadiusFromZTK, _zCyl3DRadiusFPrint },
  { "div", 1, _zCyl3DDivFromZTK, _zCyl3DDivFPrint },
};

bool zCyl3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_cyl );
}

zCyl3D *zCyl3DFromZTK(zCyl3D *cyl, ZTK *ztk)
{
  int n[] = { 0, 0 };
  zCyl3DInit( cyl );
  return ZTKEncode( cyl, n, ztk, __ztk_prp_prim_cyl );
}

void _zCyl3DFPrint(FILE *fp, zCyl3D *cyl)
{
  ZTKPropFPrint( fp, cyl, __ztk_prp_prim_cyl );
}




void _zCone3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[0]++ > 1 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_CENTER );
  else
    zVec3DFromZTK( zCone3DCenter((zCone3D*)obj), ztk );
}
void _zCone3DVertFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[1]++ > 1 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_VERT );
  else
    zVec3DFromZTK( zCone3DVert((zCone3D*)obj), ztk );
}
void _zCone3DRadiusFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[2]++ > 0 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_RADIUS );
  else
    zCone3DRadius((zCone3D*)obj) = ZTKDouble(ztk);
}
void _zCone3DDivFromZTK(void *obj, void *arg, ZTK *ztk){ zCone3DDiv((zCone3D*)obj) = zPrimDivFromZTK(ztk); }

void _zCone3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zCone3DCenter((zCone3D*)obj) ); }
void _zCone3DVertFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zCone3DVert((zCone3D*)obj) ); }
void _zCone3DRadiusFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zCone3DRadius((zCone3D*)obj) ); }
void _zCone3DDivFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%d\n", zCone3DDiv((zCone3D*)obj) ); }

static ZTKProp __ztk_prp_prim_cone[] = {
  { "center", 1, _zCone3DCenterFromZTK, _zCone3DCenterFPrint },
  { "vert", 1, _zCone3DVertFromZTK, _zCone3DVertFPrint },
  { "radius", 1, _zCone3DRadiusFromZTK, _zCone3DRadiusFPrint },
  { "div", 1, _zCone3DDivFromZTK, _zCone3DDivFPrint },
};

bool zCone3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_cone );
}

zCone3D *zCone3DFromZTK(zCone3D *cone, ZTK *ztk)
{
  int n[] = { 0, 0, 0 };
  zCone3DInit( cone );
  return ZTKEncode( cone, n, ztk, __ztk_prp_prim_cone );
}

void _zCone3DFPrint(FILE *fp, zCone3D *cone)
{
  ZTKPropFPrint( fp, cone, __ztk_prp_prim_cone );
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

void _zEllips3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){ zVec3DFromZTK( zEllips3DCenter((zEllips3D*)obj), ztk ); }
void _zEllips3DAxisXFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DAxisFromZTK( (zEllips3D*)obj, 0, 1, 2, ztk ); }
void _zEllips3DAxisYFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DAxisFromZTK( (zEllips3D*)obj, 1, 2, 0, ztk ); }
void _zEllips3DAxisZFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DAxisFromZTK( (zEllips3D*)obj, 2, 0, 1, ztk ); }
void _zEllips3DRadiusXFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DRadiusX((zEllips3D*)obj) = ZTKDouble(ztk); }
void _zEllips3DRadiusYFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DRadiusY((zEllips3D*)obj) = ZTKDouble(ztk); }
void _zEllips3DRadiusZFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DRadiusZ((zEllips3D*)obj) = ZTKDouble(ztk); }
void _zEllips3DDivFromZTK(void *obj, void *arg, ZTK *ztk){ zEllips3DDiv((zEllips3D*)obj) = zPrimDivFromZTK(ztk); }

void _zEllips3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zEllips3DCenter((zEllips3D*)obj) ); }
void _zEllips3DAxisXFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zX) ); }
void _zEllips3DAxisYFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zY) ); }
void _zEllips3DAxisZFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zEllips3DAxis((zEllips3D*)obj,zZ) ); }
void _zEllips3DRadiusXFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zEllips3DRadiusX((zEllips3D*)obj) ); }
void _zEllips3DRadiusYFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zEllips3DRadiusY((zEllips3D*)obj) ); }
void _zEllips3DRadiusZFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zEllips3DRadiusZ((zEllips3D*)obj) ); }
void _zEllips3DDivFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%d\n", zEllips3DDiv((zEllips3D*)obj) ); }

static ZTKProp __ztk_prp_prim_ellips[] = {
  { "center", 1, _zEllips3DCenterFromZTK, _zEllips3DCenterFPrint },
  { "ax", 1, _zEllips3DAxisXFromZTK, _zEllips3DAxisXFPrint },
  { "ay", 1, _zEllips3DAxisYFromZTK, _zEllips3DAxisYFPrint },
  { "az", 1, _zEllips3DAxisZFromZTK, _zEllips3DAxisZFPrint },
  { "rx", 1, _zEllips3DRadiusXFromZTK, _zEllips3DRadiusXFPrint },
  { "ry", 1, _zEllips3DRadiusYFromZTK, _zEllips3DRadiusYFPrint },
  { "rz", 1, _zEllips3DRadiusZFromZTK, _zEllips3DRadiusZFPrint },
  { "div", 1, _zEllips3DDivFromZTK, _zEllips3DDivFPrint },
};

bool zEllips3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_ellips );
}

zEllips3D *zEllips3DFromZTK(zEllips3D *ellips, ZTK *ztk)
{
  zEllips3DInit( ellips );
  return ZTKEncode( ellips, NULL, ztk, __ztk_prp_prim_ellips );
}

void _zEllips3DFPrint(FILE *fp, zEllips3D *ellips)
{
  ZTKPropFPrint( fp, ellips, __ztk_prp_prim_ellips );
}





void _zECyl3DCenterFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[0] > 1 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_CENTER );
  else
    zVec3DFromZTK( zECyl3DCenter((zECyl3D*)obj,((int*)arg)[0]++), ztk ); }
void _zECyl3DRadiusFromZTK(void *obj, void *arg, ZTK *ztk){
  if( ((int*)arg)[1] > 1 )
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_RADIUS );
  else
    zECyl3DRadius((zECyl3D*)obj,((int*)arg)[1]++) = ZTKDouble(ztk); }
void _zECyl3DRefFromZTK(void *obj, void *arg, ZTK *ztk){
  zVec3D ref;
  if( ((int*)arg)[2] > 1 ){
    ZRUNWARN( ZEO_WARN_PRIM_TOOMANY_AXIS );
    return;
  }
  zVec3DFromZTK( &ref, ztk );
  zECyl3DDefAxis( (zECyl3D*)obj, &ref );
}
void _zECyl3DDivFromZTK(void *obj, void *arg, ZTK *ztk){ zECyl3DDiv((zECyl3D*)obj) = zPrimDivFromZTK(ztk); }

void _zECyl3DCenterFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zECyl3DCenter((zECyl3D*)obj,i) ); }
void _zECyl3DRadiusFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%.10g\n", zECyl3DRadius((zECyl3D*)obj,i) ); }
void _zECyl3DRefFPrint(FILE *fp, int i, void *obj){ zVec3DFPrint( fp, zECyl3DRadVec((zECyl3D*)obj,0) ); }
void _zECyl3DDivFPrint(FILE *fp, int i, void *obj){ fprintf( fp, "%d\n", zECyl3DDiv((zECyl3D*)obj) ); }

static ZTKProp __ztk_prp_prim_ecyl[] = {
  { "center", 2, _zECyl3DCenterFromZTK, _zECyl3DCenterFPrint },
  { "radius", 2, _zECyl3DRadiusFromZTK, _zECyl3DRadiusFPrint },
  { "ref", 1, _zECyl3DRefFromZTK, _zECyl3DRefFPrint },
  { "div", 1, _zECyl3DDivFromZTK, _zECyl3DDivFPrint },
};

bool zECyl3DRegZTK(ZTK *ztk)
{
  return ZTKDefRegProp( ztk, "shape", __ztk_prp_prim_ecyl );
}

zECyl3D *zECyl3DFromZTK(zECyl3D *ecyl, ZTK *ztk)
{
  int n[] = { 0, 0, 0 };
  zECyl3DInit( ecyl );
  return ZTKEncode( ecyl, n, ztk, __ztk_prp_prim_ecyl );
}

void _zECyl3DFPrint(FILE *fp, zECyl3D *ecyl)
{
  ZTKPropFPrint( fp, ecyl, __ztk_prp_prim_ecyl );
}




zVec3D *zPH3DVertFromZTK(zPH3D *ph, int i, ZTK *ztk)
{
  int vi;

  if( ( vi = ZTKInt(ztk) ) != i )
    ZRUNWARN( ZEO_WARN_PH_VERT_UNMATCH, vi );
  return zVec3DFromZTK( zPH3DVert(ph,i), ztk );
}

zTri3D *zPH3DFaceFromZTK(zPH3D *ph, int i, ZTK *ztk)
{
  int i0, i1, i2;

  if( ( i0 = ZTKInt(ztk) ) >= zPH3DVertNum(ph) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i0 );
    return NULL;
  }
  if( ( i1 = ZTKInt(ztk) ) >= zPH3DVertNum(ph) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i1 );
    return NULL;
  }
  if( ( i2 = ZTKInt(ztk) ) >= zPH3DVertNum(ph) ){
    ZRUNERROR( ZEO_ERR_PH_INVALID_VERT_ID, i2 );
    return NULL;
  }
  return zTri3DCreate( zPH3DFace(ph,i), zPH3DVert(ph,i0), zPH3DVert(ph,i1), zPH3DVert(ph,i2) );
}

zPH3D *zPH3DFromZTK(zPH3D *ph, ZTK *ztk)
{
  int num_vert, num_face;
  int i, j;

  zPH3DInit( ph );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  num_vert = ZTKCountKey( ztk, "vert" );
  num_face = ZTKCountKey( ztk, "face" );
  zArrayAlloc( &ph->vert, zVec3D, num_vert );
  zArrayAlloc( &ph->face, zTri3D, num_face );
  if( zPH3DVertNum(ph) != num_vert ||
      zPH3DFaceNum(ph) != num_face ) return NULL;
  if( zPH3DVertNum(ph) == 0 ){
    ZRUNWARN( ZEO_WARN_PH_EMPTY );
    return NULL;
  }
  /* vertices & faces */
  i = j = 0;
  if( ZTKKeyRewind( ztk ) ) do{
    if( ZTKKeyCmp( ztk, "vert" ) )
      zPH3DVertFromZTK( ph, i++, ztk );
    else
    if( ZTKKeyCmp( ztk, "face" ) )
      zPH3DFaceFromZTK( ph, j++, ztk );
  } while( ZTKKeyNext(ztk) );
  return ph;
}

zVec zNURBS3DKnotFromZTK(ZTK *ztk)
{
  register int i;
  zVec knot;

  if( !ZTKValRewind(ztk) ) return NULL;
  if( !( knot = zVecAlloc( ZTKInt(ztk) ) ) ) return NULL;
  for( i=0; i<zVecSizeNC(knot); i++ )
    zVecElemNC(knot,i) = ZTKDouble(ztk);
  return knot;
}

zNURBS3D *zNURBS3DFromZTK(zNURBS3D *nurbs, ZTK *ztk)
{
  int size1, size2;
  int i, j;

  zNURBS3DInit( nurbs );
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  do{
    if( ZTKKeyCmp( ztk, "dim" ) ){
      nurbs->dim[0] = ZTKInt( ztk );
      nurbs->dim[1] = ZTKInt( ztk );
    } else
    if( ZTKKeyCmp( ztk, "uknot" ) ){
      if( nurbs->knot[0] ){
        ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
        zVecFree( nurbs->knot[0] );
      }
      if( !( nurbs->knot[0] = zNURBS3DKnotFromZTK( ztk ) ) ) return NULL;
    } else
    if( ZTKKeyCmp( ztk, "vknot" ) ){
      if( nurbs->knot[1] ){
        ZRUNWARN( ZEO_ERR_NURBS_KNOTALREADY );
        zVecFree( nurbs->knot[1] );
      }
      if( !( nurbs->knot[1] = zNURBS3DKnotFromZTK( ztk ) ) ) return NULL;
    } else
    if( ZTKKeyCmp( ztk, "slice" ) ){
      nurbs->ns[0] = ZTKInt( ztk );
      nurbs->ns[1] = ZTKInt( ztk );
    } else
    if( ZTKKeyCmp( ztk, "size" ) ){
      if( zNURBS3DCPNum(nurbs,0) > 0 || zNURBS3DCPNum(nurbs,1) > 0 ){
        ZRUNWARN( ZEO_ERR_NURBS_CPALREADY );
        zArray2Free( &nurbs->cpnet );
      }
      size1 = ZTKInt( ztk );
      size2 = ZTKInt( ztk );
      zArray2Alloc( &nurbs->cpnet, zNURBS3DCPCell, size1, size2 );
      if( zArray2RowSize(&nurbs->cpnet) != size1 ||
          zArray2ColSize(&nurbs->cpnet) != size2 ) return NULL;
    } else
    if( ZTKKeyCmp( ztk, "cp" ) ){
      i = ZTKInt( ztk );
      j = ZTKInt( ztk );
      if( !zArray2PosIsValid( &nurbs->cpnet, i, j ) ){
        ZRUNERROR( ZEO_ERR_NURBS_INVCP );
        return NULL;
      }
      zNURBS3DSetWeight( nurbs, i, j, ZTKDouble(ztk) );
      zVec3DFromZTK( zNURBS3DCP(nurbs,i,j), ztk );
    }
  } while( ZTKKeyNext(ztk) );
  return nurbs;
}





bool zShape3DRegZTK(ZTK *ztk)
{
  char *key[] = { /* for any shape */
    "name", "type", "optic", "bb", "mirror",
  };
  char *key_ph[] = {
    "vert", "face",
  };
  char *key_nurbs[] = {
    "dim", "uknot", "vknot", "slice", "size", "cp",
  };
  return ZTKDefReg( ztk, "shape", key ) &&
         zBox3DRegZTK( ztk ) &&
         zSphere3DRegZTK( ztk ) &&
         zCyl3DRegZTK( ztk ) &&
         zCone3DRegZTK( ztk ) &&
         zEllips3DRegZTK( ztk ) &&
         zECyl3DRegZTK( ztk ) &&
         ZTKDefReg( ztk, "shape", key_ph ) &&
         ZTKDefReg( ztk, "shape", key_nurbs ) ? true : false;
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
  zShape3D *ref;

  zShape3DInit( shape );
  /* type, name, associated optical info and mirroring */
  if( !ZTKKeyRewind( ztk ) ) return NULL;
  do{
    if( ZTKKeyCmp( ztk, "name" ) ) zNameSet( shape, ZTKVal(ztk) );
    else
    if( ZTKKeyCmp( ztk, "type" ) ) zShape3DType(shape) = zShapeTypeByStr( ZTKVal(ztk) );
    else
    if( ZTKKeyCmp( ztk, "optic" ) ) zNameFind( oarray, no, ZTKVal(ztk), zShape3DOptic(shape) );
    else
    if( ZTKKeyCmp( ztk, "mirror" ) ){
      zNameFind( sarray, ns, ZTKVal(ztk), ref );
      if( !ref ){
        ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, ZTKVal(ztk) );
      } else{
        return ZTKValNext(ztk) && zShape3DMirror( ref, shape, zAxisByStr(ZTKVal(ztk)) ) ?
          shape : NULL;
      }
    }
  } while( ZTKKeyNext(ztk) );
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
  if( ZTKKeyRewind( ztk ) ) do{
    if( ZTKKeyCmp( ztk, "bb" ) ){
      if( zShape3DType(shape) != ZSHAPE_PH ){
        ZRUNWARN( ZEO_WARN_SHAPE_BB_INVALID );
        break;
      }
      if( ZTKValCmp( ztk, "AABB" ) ){
        zAABox3D aabb;
        zAABB( &aabb, zShape3DVertBuf(shape), zShape3DVertNum(shape), NULL );
        zAABox3DToBox3D( &aabb, zShape3DBB(shape) );
      } else
      if( ZTKValCmp( ztk, "OBB" ) ){
        zOBB( zShape3DBB(shape), zShape3DVertBuf(shape), zShape3DVertNum(shape) );
      } else
        ZRUNWARN( ZEO_WARN_UNKNOWN_BB_TYPE, ZTKVal(ztk) );
    }
  } while( ZTKKeyNext(ztk) );
  return shape;
}

zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk)
{
  int num_shape, num_optic;
  int i;

  zMShape3DInit( ms );
  num_shape = ZTKCountTag( ztk, "shape" );
  num_optic = ZTKCountTag( ztk, "optic" );
  zArrayAlloc( &ms->shape, zShape3D, num_shape );
  zArrayAlloc( &ms->optic, zOpticalInfo, num_optic );
  if( zMShape3DShapeNum(ms) != num_shape ||
      zMShape3DOpticNum(ms) != num_optic ) return NULL;
  if( zMShape3DShapeNum(ms) == 0 ){
    ZRUNWARN( ZEO_WARN_MSHAPE_EMPTY );
    return NULL;
  }
  /* complete optical info */
  i = 0;
  if( ZTKTagRewind( ztk ) ) do{
    if( ZTKTagCmp( ztk, "optic" ) )
      zOpticalInfoFromZTK( zMShape3DOptic(ms,i++), ztk );
  } while( ZTKTagNext(ztk) );
  /* complete shapes */
  i = 0;
  if( ZTKTagRewind( ztk ) ) do{
    if( ZTKTagCmp( ztk, "shape" ) )
      zShape3DFromZTK( zMShape3DShape(ms,i++),
        zMShape3DShapeBuf(ms), zMShape3DShapeNum(ms),
        zMShape3DOpticBuf(ms), zMShape3DOpticNum(ms), ztk );
  } while( ZTKTagNext(ztk) );
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

int main(int argc, char *argv[])
{
  zMShape3D ms;

  if( argc <= 1 ) return 1;
  if( zMShape3DParseZTK( &ms, argv[1] ) ){
    zMShape3DPrint( &ms );
    int i;
    for( i=0; i<zMShape3DOpticNum(&ms); i++ )
      _zOpticalInfoFPrint( stdout, zMShape3DOptic(&ms,i) );
    for( i=0; i<zMShape3DShapeNum(&ms); i++ ){
      switch( zShape3DType(zMShape3DShape(&ms,i)) ){
      case ZSHAPE_BOX:
        _zBox3DFPrint( stdout, zShape3DBox(zMShape3DShape(&ms,i)) ); break;
      case ZSHAPE_SPHERE:
        _zSphere3DFPrint( stdout, zShape3DSphere(zMShape3DShape(&ms,i)) ); break;
      case ZSHAPE_CYLINDER:
        _zCyl3DFPrint( stdout, zShape3DCyl(zMShape3DShape(&ms,i)) ); break;
      case ZSHAPE_CONE:
        _zCone3DFPrint( stdout, zShape3DCone(zMShape3DShape(&ms,i)) ); break;
      case ZSHAPE_ELLIPS:
        _zEllips3DFPrint( stdout, zShape3DEllips(zMShape3DShape(&ms,i)) ); break;
      case ZSHAPE_ELLIPTICCYLINDER:
        _zECyl3DFPrint( stdout, zShape3DECyl(zMShape3DShape(&ms,i)) ); break;
      default: ;
      }
    }
  }

  zMShape3DDestroy( &ms );
  return 0;
}

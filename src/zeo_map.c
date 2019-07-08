/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map - map class (a stub).
 */

#include <zeo/zeo_map.h>

static const char *__zmap_type_expr[] = {
  "none",
  "terra",
  NULL,
};

const char *zMapTypeExpr(zMapType type)
{
  return __zmap_type_expr[zLimit(type,0,ZMAP_TERRA)];
}

zMapType zMapTypeByStr(char *str)
{
  register int i;
  for( i=0; __zmap_type_expr[i]; i++ )
    if( strcmp( __zmap_type_expr[i], str ) == 0 ) return (zMapType)i;
  return ZMAP_NONE;
}

void zMapInit(zMap *map)
{
  zNameSetPtr( map, NULL );
  map->type = ZMAP_NONE;
  zTerraInit( &map->body.terra );
}

void zMapDestroy(zMap *map)
{
  zNameDestroy( map );
  if( map->type == ZMAP_TERRA )
    zTerraFree( &map->body.terra );
}

static void *_zMapNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zMap*)obj, ZTKVal(ztk) );
  return zNamePtr((zMap*)obj) ? obj : NULL;
}
static void *_zMapTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zMap*)obj)->type = zMapTypeByStr( ZTKVal(ztk) );
  return obj;
}

static void _zMapNameFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zMap*)obj) );
}
static void _zMapTypeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zMapTypeExpr(((zMap*)obj)->type) );
}

static ZTKPrp __ztk_prp_map_key[] = {
  { "name", 1, _zMapNameFromZTK, _zMapNameFPrint },
  { "type", 1, _zMapTypeFromZTK, _zMapTypeFPrint },
};

static void *_zMapFromZTK(void *obj, int i, void *arg, ZTK *ztk)
{
  if( !ZTKEncodeKey( obj, NULL, ztk, __ztk_prp_map_key ) ) return NULL;
  if( ((zMap*)obj)->type == ZMAP_TERRA )
    if( !zTerraFromZTK( &((zMap*)obj)->body.terra, ztk ) ) return NULL;
  return obj;
}

static void _zMapFPrint(FILE *fp, int i, void *obj)
{
  ZTKPrpKeyFPrint( fp, obj, __ztk_prp_map_key );
  if( ((zMap*)obj)->type == ZMAP_TERRA )
    zTerraFPrint( fp, &((zMap*)obj)->body.terra );
}

static ZTKPrp __ztk_prp_map[] = {
  { "map", 1, _zMapFromZTK, _zMapFPrint },
};

static bool _zMapRegZTK(ZTK *ztk)
{
  return ZTKDefRegPrp( ztk, ZTK_TAG_MAP, __ztk_prp_map_key ) &&
         zTerraDefRegZTK( ztk, ZTK_TAG_MAP );
}

zMap *zMapFromZTK(zMap *map, ZTK *ztk)
{
  zMapInit( map );
  return ZTKEncodeTag( map, NULL, ztk, __ztk_prp_map );
}

/* scan a ZTK format file and create a map. */
zMap *zMapScanFile(zMap *map, char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  if( !_zMapRegZTK( &ztk ) ) return NULL;
  ZTKParse( &ztk, filename );
  map = zMapFromZTK( map, &ztk );
  ZTKDestroy( &ztk );
  return map;
}

void zMapFPrint(FILE *fp, zMap *map)
{
  ZTKPrpTagFPrint( fp, map, __ztk_prp_map );
}

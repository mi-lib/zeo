/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map - map class.
 */

#include <zeo/zeo_map.h>

/* initialize a map. */
zMap *zMapInit(zMap *map)
{
  zNameSetPtr( map, NULL );
  map->body = NULL;
  map->com = NULL;
  return map;
}

/* destroy a map. */
void zMapDestroy(zMap *map)
{
  zNameFree( map );
  if( map->com )
    map->com->_destroy( map->body );
  zFree( map->body );
  map->com = NULL;
}

static void *_zMapNameFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zNameSet( (zMap*)obj, ZTKVal(ztk) );
  return zNamePtr((zMap*)obj) ? obj : NULL;
}
static void *_zMapTypeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zMapCom *com[] = {
    &zeo_map_terra_com,
    NULL,
  };
  register int k;

  for( k=0; com[k]; k++ )
    if( strcmp( com[k]->typestr, ZTKVal(ztk) ) == 0 ){
      ((zMap*)obj)->com = com[k];
      return ( ((zMap*)obj)->body = ((zMap*)obj)->com->_alloc() ) ? obj : NULL;
    }
  return NULL;
}

static void _zMapNameFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zMap*)obj) );
}
static void _zMapTypeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", ((zMap*)obj)->com->typestr );
}

static ZTKPrp __ztk_prp_map_key[] = {
  { "name", 1, _zMapNameFromZTK, _zMapNameFPrint },
  { "type", 1, _zMapTypeFromZTK, _zMapTypeFPrint },
};

bool zMapRegZTK(ZTK *ztk)
{
  return ZTKDefRegPrp( ztk, ZTK_TAG_MAP, __ztk_prp_map_key ) &&
         zTerraDefRegZTK( ztk, ZTK_TAG_MAP );
}

zMap *zMapFromZTK(zMap *map, ZTK *ztk)
{
  zMapInit( map );
  if( !ZTKEncodeKey( map, NULL, ztk, __ztk_prp_map_key ) ) return NULL;
  if( !map->body ){
    ZRUNERROR( ZEO_ERR_MAP_UNSPEC );
    return NULL;
  }
  return map->com->_parseZTK( map->body, ztk ) ? map : NULL;
}

void zMapFPrint(FILE *fp, zMap *map)
{
  ZTKPrpKeyFPrint( fp, map, __ztk_prp_map_key );
  map->com->_fprint( fp, map->body );
}

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
  int k;

  for( k=0; com[k]; k++ )
    if( strcmp( com[k]->typestr, ZTKVal(ztk) ) == 0 ){
      ((zMap*)obj)->com = com[k];
      return ( ((zMap*)obj)->body = ((zMap*)obj)->com->_alloc() ) ? obj : NULL;
    }
  return NULL;
}

static bool _zMapNameFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", zName((zMap*)obj) );
  return true;
}
static bool _zMapTypeFPrintZTK(FILE *fp, int i, void *obj){
  fprintf( fp, "%s\n", ((zMap*)obj)->com->typestr );
  return true;
}

static const ZTKPrp __ztk_prp_map[] = {
  { ZTK_KEY_ZEO_MAP_NAME, 1, _zMapNameFromZTK, _zMapNameFPrintZTK },
  { ZTK_KEY_ZEO_MAP_TYPE, 1, _zMapTypeFromZTK, _zMapTypeFPrintZTK },
};

zMap *zMapFromZTK(zMap *map, ZTK *ztk)
{
  zMapInit( map );
  if( !_ZTKEvalKey( map, NULL, ztk, __ztk_prp_map ) ) return NULL;
  if( !map->body ){
    ZRUNERROR( ZEO_ERR_MAP_TYPENOTSPECIFIED );
    return NULL;
  }
  return map->com->_fromZTK( map->body, ztk ) ? map : NULL;
}

void zMapFPrintZTK(FILE *fp, zMap *map)
{
  _ZTKPrpKeyFPrint( fp, map, __ztk_prp_map );
  map->com->_fprintZTK( fp, map->body );
}

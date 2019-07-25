/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mapnet - net of maps.
 */

#include <zeo/zeo_mapnet.h>

/* initialize map net. */
zMapNet *zMapNetInit(zMapNet *mn)
{
  zArrayInit( &mn->maparray );
  return mn;
}

/* allocate memory of map net. */
zMapNet *zMapNetAlloc(zMapNet *mn, int size)
{
  zArrayAlloc( &mn->maparray, zMap, size );
  if( zArraySize(&mn->maparray) != size ) return NULL;
  if( zArraySize(&mn->maparray) == 0 ){
    ZRUNWARN( ZEO_WARN_MAPNET_EMPTY );
    return NULL;
  }
  return mn;
}

/* destroy map net. */
void zMapNetDestroy(zMapNet *mn)
{
  register int i;

  for( i=0; i<zArraySize(&mn->maparray); i++ )
    zMapDestroy( zMapNetMap(mn,i) );
  zArrayFree( &mn->maparray );
}

/* parse ZTK format */

static void *_zMapNetMapFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  return zMapFromZTK( zMapNetMap((zMapNet*)obj,i), ztk ) ? obj : NULL;
}

static ZTKPrp __ztk_prp_mapnet[] = {
  { "map", -1, _zMapNetMapFromZTK, NULL },
};

/* register a definition of tag-and-keys for map net to a ZTK format processor. */
bool zMapNetRegZTK(ZTK *ztk)
{
  return zMapRegZTK( ztk );
}

/* read multiple 3D shapes from a ZTK format processor. */
zMapNet *zMapNetFromZTK(zMapNet *mn, ZTK *ztk)
{
  int num_map;

  zMapNetInit( mn );
  num_map = ZTKCountTag( ztk, ZTK_TAG_MAP );
  if( !zMapNetAlloc( mn, num_map ) ) return NULL;
  ZTKEncodeTag( mn, NULL, ztk, __ztk_prp_mapnet );
  return mn;
}

/* read map net from a ZTK format file. */
zMapNet *zMapNetReadZTK(zMapNet *mn, char filename[])
{
  ZTK ztk;

  ZTKInit( &ztk );
  if( !zMapNetRegZTK( &ztk ) ) return NULL;
  ZTKParse( &ztk, filename );
  mn = zMapNetFromZTK( mn, &ztk );
  ZTKDestroy( &ztk );
  return mn;
}

/* print information of map net out to a file. */
void zMapNetFPrint(FILE *fp, zMapNet *mn)
{
  register int i;

  for( i=0; i<zArraySize(&mn->maparray); i++ ){
    fprintf( fp, "[%s]\n", ZTK_TAG_MAP );
    zMapFPrint( fp, zMapNetMap(mn,i) );
  }
}

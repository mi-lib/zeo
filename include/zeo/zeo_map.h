/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map - map class (a stub).
 */

#ifndef __ZEO_MAP_H__
#define __ZEO_MAP_H__

#include <zeo/zeo_terra.h>

__BEGIN_DECLS

typedef enum{ ZMAP_NONE=0, ZMAP_TERRA } zMapType;

__EXPORT const char *zMapTypeExpr(zMapType type);
__EXPORT zMapType zMapTypeByStr(char *str);

typedef union{
  zTerra terra;
} zMapBody;

typedef struct{
  Z_NAMED_CLASS
  zMapType type;
  zMapBody body;
} zMap;

#define ZTK_TAG_MAP "map"

__EXPORT void zMapInit(zMap *map);
__EXPORT void zMapDestroy(zMap *map);

__EXPORT zMap *zMapFromZTK(zMap *map, ZTK *ztk);
__EXPORT zMap *zMapScanFile(zMap *map, char filename[]);
__EXPORT void zMapFPrint(FILE *fp, zMap *map);

__END_DECLS

#endif /* __ZEO_MAP_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map - map class.
 */

#ifndef __ZEO_MAP_H__
#define __ZEO_MAP_H__

#include <zeo/zeo_mat3d.h>

__BEGIN_DECLS

/*! \struct zMapCom
 * \brief common methods for map class.
 */
typedef struct{
  const char *typestr;
  void *(* _alloc)(void);
  void (* _destroy)(void *);
  void *(* _parseZTK)(void*,ZTK*);
  void (* _fprint)(FILE*,void*);
} zMapCom;

/*! \struct zMap
 * \brief map class.
 */
typedef struct{
  Z_NAMED_CLASS
  void *body;
  zMapCom *com;
} zMap;

/* initialize a map. */
__EXPORT zMap *zMapInit(zMap *map);
/* destroy a map. */
__EXPORT void zMapDestroy(zMap *map);

#define ZTK_TAG_MAP "map"

__EXPORT bool zMapRegZTK(ZTK *ztk);
__EXPORT zMap *zMapFromZTK(zMap *map, ZTK *ztk);
__EXPORT void zMapFPrint(FILE *fp, zMap *map);

/*! \struct zMapArray
 * \brief array class of maps.
 */
zArrayClass( zMapArray, zMap );

__END_DECLS

#include <zeo/zeo_map_terra.h>

#endif /* __ZEO_MAP_H__ */

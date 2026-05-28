/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map - map class.
 */

#ifndef __ZEO_MAP_H__
#define __ZEO_MAP_H__

#include <zeo/zeo_mat3d.h>

/*! \struct zMapCom
 * \brief common methods for map class.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zMapCom ){
  const char *typestr;
  void *(* _alloc)(void);
  void (* _destroy)(void *);
  void *(* _fromZTK)(void*,ZTK*);
  void (* _fprintZTK)(FILE*,const void*);
};

/*! \struct zMap
 * \brief map class.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zMap ){
  Z_NAMED_CLASS;
  void *body;
  zMapCom *com;
};

__BEGIN_DECLS

/* initialize a map. */
__ZEO_EXPORT zMap *zMapInit(zMap *map);
/* destroy a map. */
__ZEO_EXPORT void zMapDestroy(zMap *map);

#define ZTK_TAG_ZEO_MAP "zeo::map"

#define ZTK_KEY_ZEO_MAP_NAME "name"
#define ZTK_KEY_ZEO_MAP_TYPE "type"

__ZEO_EXPORT zMap *zMapFromZTK(zMap *map, ZTK *ztk);
__ZEO_EXPORT void zMapFPrintZTK(FILE *fp, const zMap *map);

__END_DECLS

/*! \struct zMapArray
 * \brief array class of maps.
 */
ZEDA_DEF_ARRAY_CLASS( zMapArray, zMap );

#include <zeo/zeo_map_terra.h>

#endif /* __ZEO_MAP_H__ */

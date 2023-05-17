/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mapnet - net of maps.
 */

#ifndef __ZEO_MAPNET_H__
#define __ZEO_MAPNET_H__

#include <zeo/zeo_map.h>

__BEGIN_DECLS

/*! \struct zMapNet
 * \brief net of maps (tentative)
 */
typedef struct{
  zMapArray maparray;
} zMapNet;

#define zMapNetMap(mn,i) ((zMap*)zArrayElem(&(mn)->maparray,i))

/*! \brief initialize map net. */
__ZEO_EXPORT zMapNet *zMapNetInit(zMapNet *mn);
/*! \brief allocate memory of map net. */
__ZEO_EXPORT zMapNet *zMapNetAlloc(zMapNet *mn, int size);
/*! \brief destroy map net. */
__ZEO_EXPORT void zMapNetDestroy(zMapNet *mn);

/*! \brief parse ZTK format */

/*! \brief read multiple 3D shapes from a ZTK format processor. */
__ZEO_EXPORT zMapNet *zMapNetFromZTK(zMapNet *mn, ZTK *ztk);
/*! \brief print information of map net out to a file in ZTK format. */
__ZEO_EXPORT void zMapNetFPrintZTK(FILE *fp, zMapNet *mn);

/*! \brief read map net from a ZTK format file. */
__ZEO_EXPORT zMapNet *zMapNetReadZTK(zMapNet *mn, char filename[]);
/*! \brief write map net to a ZTK format file. */
__ZEO_EXPORT bool zMapNetWriteZTK(zMapNet *mn, char filename[]);

__END_DECLS

#endif /* __ZEO_MAPNET_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_pointcloud - 3D point cloud.
 */

#ifndef __ZEO_POINTCLOUD_H__
#define __ZEO_POINTCLOUD_H__

#include <zeo/zeo_ep.h>
#include <zeo/zeo_frame.h>

__BEGIN_DECLS

/*! \brief read point cloud from PCD file.
 *
 * zVec3DListPCDFRead() reads a point cloud from a stream of PCD file.
 * zVec3DListReadPCDFile() reads a point cloud from a PCD file.
 * \return
 * zVec3DListPCDFRead() and zVec3DListReadPCDFile() return the true value
 * if they succeed to read a PCD file. In the case it fails to read the
 * given PCD file in any reason such as invalid format of memory allocation
 * failure, the false value is returned.
 */
__EXPORT bool zVec3DListPCDFRead(FILE *fp, zVec3DList *pc);
__EXPORT bool zVec3DListReadPCDFile(zVec3DList *pc, char filename[]);

#define ZEO_PCD_SUFFIX "pcd"

__END_DECLS

#endif /* __ZEO_POINTCLOUD_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_pointcloud - 3D point cloud.
 */

#ifndef __ZEO_POINTCLOUD_H__
#define __ZEO_POINTCLOUD_H__

#include <zeo/zeo_ep.h>
#include <zeo/zeo_frame3d.h>

__BEGIN_DECLS

/*! \brief read point cloud from a PCD file.
 *
 * zVec3DListPCDFRead() reads a point cloud from a stream of PCD file pointed by \a fp.
 * zVec3DListReadPCDFile() reads a point cloud from a PCD file \a filename.
 * The points are stored in a list pointed by \a pc.
 * \return
 * zVec3DListPCDFRead() and zVec3DListReadPCDFile() return the true value
 * if they succeed to read a PCD file. In the case it fails to read the
 * given PCD file in any reason such as invalid format of memory allocation
 * failure, the false value is returned.
 */
__ZEO_EXPORT bool zVec3DListPCDFRead(FILE *fp, zVec3DList *pc);
__ZEO_EXPORT bool zVec3DListReadPCDFile(zVec3DList *pc, char filename[]);

/*! \brief write point cloud to a PCD file.
 *
 * zVec3DListPCDFWrite() writes a point cloud to a stream of PCD file pointed by \a fp.
 * zVec3DListWritePCDFile() writes a point cloud to a PCD file \a filename.
 * \a pc is the list of points.
 * \a format is either "ascii" or "binary", which defines the data format of the file.
 * \return
 * zVec3DListPCDFWrite() and zVec3DListWritePCDFile() return the true value
 * if they succeed to write a PCD file. In the case it fails to write the
 * given PCD file in any reason, the false value is returned.
 */
__ZEO_EXPORT bool zVec3DListPCDFWrite(FILE *fp, zVec3DList *pc, const char *format);
__ZEO_EXPORT bool zVec3DListWritePCDFile(zVec3DList *pc, char filename[], const char *format);

#define ZEO_PCD_SUFFIX "pcd"

__END_DECLS

#endif /* __ZEO_POINTCLOUD_H__ */

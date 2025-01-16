/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pcd - read/write of PCD format files (3D point cloud).
 */

#ifndef __ZEO_VEC3D_PCD_H__
#define __ZEO_VEC3D_PCD_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read point cloud from a PCD file.
 *
 * zVec3DDataFReadPCD() reads point cloud from a stream of PCD file pointed by \a fp.
 * zVec3DDataReadPCDFile() reads point cloud from a PCD file \a filename.
 * The points are stored in a set of 3D vectors \a data.
 * \return
 * zVec3DDataFReadPCD() and zVec3DDataReadPCDFile() return the true value if they succeed to
 * read the PCD file. If they fail in any reason such as invalid format of memory allocation
 * failure, the false value is returned.
 */
__ZEO_EXPORT bool zVec3DDataFReadPCD(FILE *fp, zVec3DData *data);
__ZEO_EXPORT bool zVec3DDataReadPCDFile(zVec3DData *data, const char filename[]);

/*! \brief write point cloud to a PCD file.
 *
 * zVec3DDataFWritePCD() outputs point cloud \a data to a stream of PCD file pointed by \a fp.
 * zVec3DDataWritePCDFile() outputs point cloud \a data to a PCD file \a filename.
 * \a format is either "ascii" or "binary", which defines the data format of the file.
 * \return
 * zVec3DDataFWritePCD() and zVec3DDataWritePCDFile() return the true value if they succeed
 * to output to the PCD file. If they fail in any reason, the false value is returned.
 */
__ZEO_EXPORT bool zVec3DDataFWritePCD(FILE *fp, zVec3DData *data, const char *format);
__ZEO_EXPORT bool zVec3DDataWritePCDFile(zVec3DData *data, const char filename[], const char *format);

#define ZEO_PCD_SUFFIX "pcd"

__END_DECLS

#endif /* __ZEO_VEC3D_PCD_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph3d_stl - polyhedron I/O on STL format
 */

#ifndef __ZEO_PH3D_STL_H__
#define __ZEO_PH3D_STL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read and write a 3D polyhedron in ASCII STL format. */
__ZEO_EXPORT zPH3D *zPH3DFReadSTL_ASCII(FILE *fp, zPH3D *ph, char shapenamebuf[], size_t bufsize);
__ZEO_EXPORT void zPH3DFWriteSTL_ASCII(FILE *fp, zPH3D *ph, const char shapename[]);

/*! \brief check if STL format is binary. */
__ZEO_EXPORT bool zSTLIsBin(FILE *fp);

/*! \brief read and write a 3D polyhedron in binary STL format. */
__ZEO_EXPORT zPH3D *zPH3DFReadSTL_Bin(FILE *fp, zPH3D *ph, char shapenamebuf[]);
__ZEO_EXPORT void zPH3DFWriteSTL_Bin(FILE *fp, zPH3D *ph, const char shapename[]);

/*! \brief read and write a 3D polyhedron in STL format. */
__ZEO_EXPORT zPH3D *zPH3DReadFileSTL(zPH3D *ph, const char *filename, char shapenamebuf[], size_t bufsize);
__ZEO_EXPORT void zPH3DWriteFileSTL(zPH3D *ph, const char *filename, const char shapename[]);

__END_DECLS

#endif /* __ZEO_PH3D_STL_H__ */

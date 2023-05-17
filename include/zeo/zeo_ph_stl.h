/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_stl - polyhedron I/O on STL format
 */

#ifndef __ZEO_PH_STL_H__
#define __ZEO_PH_STL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read and write a 3D polyhedron in ASCII STL format. */
__ZEO_EXPORT zPH3D *zPH3DFReadSTL_ASCII(FILE *fp, zPH3D *ph, char name[], size_t namesize);
__ZEO_EXPORT void zPH3DFWriteSTL_ASCII(FILE *fp, zPH3D *ph, char name[]);

/*! \brief check if STL format is binary. */
__ZEO_EXPORT bool zSTLIsBin(FILE *fp);

/*! \brief read and write a 3D polyhedron in binary STL format. */
__ZEO_EXPORT zPH3D *zPH3DFReadSTL_Bin(FILE *fp, zPH3D *ph, char name[]);
__ZEO_EXPORT void zPH3DFWriteSTL_Bin(FILE *fp, zPH3D *ph, char name[]);

/*! \brief read and write a 3D polyhedron in STL format. */
__ZEO_EXPORT zPH3D *zPH3DFReadSTL(FILE *fp, zPH3D *ph, char name[], size_t namesize);
#define zPH3DFWriteSTL zPH3DFWriteSTL_ASCII

__END_DECLS

#endif /* __ZEO_PH_STL_H__ */

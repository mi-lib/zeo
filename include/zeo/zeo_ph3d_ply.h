/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph3d_ply - polyhedron I/O on PLY (Stanford Triangle) format
 * for specification of the format, refer:
 * http://paulbourke.net/dataformats/ply/
 * for public PLY files, refer:
 * http://graphics.stanford.edu/data/3Dscanrep/
 */

#ifndef __ZEO_PH3D_PLY_H__
#define __ZEO_PH3D_PLY_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read a 3D polyhedron in PLY format. */
__ZEO_EXPORT zPH3D *zPH3DReadFilePLY(zPH3D *ph, const char *filename);

/*! \brief write a 3D polyhedron in PLY format (ASCII) */
__ZEO_EXPORT bool zPH3DWriteFilePLY_ASCII(zPH3D *ph, const char *filename);

/*! \brief write a 3D polyhedron in PLY format (binary) */
__ZEO_EXPORT bool zPH3DWriteFilePLY_Bin(zPH3D *ph, const char *filename);

#define zPH3DWriteFilePLY zPH3DWriteFilePLY_ASCII

__END_DECLS

#endif /* __ZEO_PH3D_PLY_H__ */

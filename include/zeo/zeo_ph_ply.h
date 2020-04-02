/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_ply - polyhedron I/O on PLY (Stanford Triangle) format
 * for specification of the format, refer:
 * http://paulbourke.net/dataformats/ply/
 * for public PLY files, refer:
 * http://graphics.stanford.edu/data/3Dscanrep/
 */

#ifndef __ZEO_PH_PLY_H__
#define __ZEO_PH_PLY_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read a 3D polyhedron in PLY format. */
__EXPORT zPH3D *zPH3DFReadPLY(FILE *fp, zPH3D *ph);

/*! \brief write a 3D polyhedron in PLY format (ASCII) */
__EXPORT void zPH3DFWritePLY_ASCII(FILE *fp, zPH3D *ph);

/*! \brief write a 3D polyhedron in PLY format (binary) */
__EXPORT void zPH3DFWritePLY_Bin(FILE *fp, zPH3D *ph);

#define zPH3DFWritePLY zPH3DFWritePLY_ASCII

__END_DECLS

#endif /* __ZEO_PH_PLY_H__ */

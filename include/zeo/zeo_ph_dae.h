/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_dae - polyhedron I/O on DAE format
 */

#ifndef __ZEO_PH_DAE_H__
#define __ZEO_PH_DAE_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief read a 3D polyhedron in DAE format. */
__ZEO_EXPORT zPH3D *zPH3DFReadDAE(zPH3D *ph, char *filename);

__END_DECLS

#endif /* __ZEO_PH_DAE_H__ */

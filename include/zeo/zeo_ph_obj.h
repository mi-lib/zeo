/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_obj - polyhedron I/O on OBJ format
 */

#ifndef __ZEO_PH_OBJ_H__
#define __ZEO_PH_OBJ_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief  read a 3D polyhedron from OBJ format */
__ZEO_EXPORT zPH3D *zPH3DFReadOBJ(FILE *fp, zPH3D *ph);
__ZEO_EXPORT zPH3D *zPH3DReadFileOBJ(zPH3D *ph, const char *filename);

__END_DECLS

#endif /* __ZEO_PH_OBJ_H__ */

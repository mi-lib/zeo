/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_brep_trunc - truncation of B-Rep.
 */

#ifndef __ZEO_BREP_TRUNC_H__
#define __ZEO_BREP_TRUNC_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \brief truncate B-rep.
 *
 * zBREPTrunc() directly truncates a B-Rep shape \a brep by
 * a cutting plane \a pl. Note that this operation is destructive.
 *
 * zBREPTruncPH3D() directly truncates \a brep by a polyhedron
 * \a ph. It internally truncate \a brep by all faces of \a ph.
 * \return
 * zBREPTrunc() returns a pointer \a brep if it succeeds to create
 * a truncated shape. If it fails to allocate internal workspace,
 * the null pointer is returned.
 *
 * zBREPTruncPH3D() returns \a brep if it succeeds.
 * The null pointer is returned according to the same condition
 * with zBREPTrunc().
 */
__EXPORT zBREP *zBREPTrunc(zBREP *brep, zPlane3D *pl);
__EXPORT zBREP *zBREPTruncPH3D(zBREP *brep, zPH3D *ph);

__END_DECLS

#endif /* __ZEO_BREP_TRUNC_H__ */

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape - multiple 3D shape
 */

#ifndef __ZEO_MSHAPE_H__
#define __ZEO_MSHAPE_H__

#include <zeo/zeo_shape.h>

__BEGIN_DECLS

zArrayClass( zShapeArray, zShape3D );
zArrayClass( zOpticArray, zOpticalInfo );

/* ********************************************************** */
/* CLASS: zMShape3D
 * multiple 3D shape class
 * ********************************************************** */

typedef struct{
  zShapeArray shape;
  zOpticArray optic;
} zMShape3D;

#define zMShape3DShapeNum(s)      zArraySize(&(s)->shape)
#define zMShape3DShapeBuf(s)      zArrayBuf(&(s)->shape)
#define zMShape3DShape(s,i)       zArrayElemNC(&(s)->shape,i)
#define zMShape3DSetShapeNum(s,n) ( zArraySize(&(s)->shape) = (n) )
#define zMShape3DSetShapeBuf(s,b) ( zArrayBuf(&(s)->shape) = (b) )
#define zMShape3DOpticNum(s)      zArraySize(&(s)->optic)
#define zMShape3DOpticBuf(s)      zArrayBuf(&(s)->optic)
#define zMShape3DOptic(s,i)       zArrayElemNC(&(s)->optic,i)
#define zMShape3DSetOpticNum(s,n) ( zArraySize(&(s)->optic) = (n) )
#define zMShape3DSetOpticBuf(s,b) ( zArrayBuf(&(s)->optic) = (b) )

/*! \brief initialize and destroy multiple 3D shapes.
 *
 * zMShape3DInit() initializes multiple 3D shapes \a ms,
 * setting all arrays for the null pointer and all the
 * sizes of arrays for 0.
 *
 * zMShape3DDestroy() destroys the all inner arrays of \a ms.
 * \return
 * zMShape3DInit() returns a pointer to \a ms.
 *
 * zMShape3DDestroy() returns no value.
 */
__EXPORT zMShape3D *zMShape3DInit(zMShape3D *ms);
__EXPORT void zMShape3DDestroy(zMShape3D *ms);
__EXPORT zMShape3D *zMShape3DClone(zMShape3D *org);

/*! \brief check if a point is inside of multiple shapes.
 *
 * zMShape3DPointIsInside() checks if a point \a p is
 * inside of the multiple shapes \a ms.
 *
 * \a p on the surface of \a ms is judged to be inside
 * of \a ms if and only if \a rim is the true value.
 * \return
 * zMShape3DPointIsInside() returns the true value
 * when \a p is inside of \a ms, or the false value
 * otherwise.
 * \sa
 * zShape3DPointIsInside, zPH3DPointIsInside
 */
__EXPORT zVec3D *zMShape3DContigVert(zMShape3D *ms, zVec3D *p, double *d);
__EXPORT double zMShape3DClosest(zMShape3D *ms, zVec3D *p, zVec3D *cp);
__EXPORT bool zMShape3DPointIsInside(zMShape3D *ms, zVec3D *p, bool rim);

__EXPORT zMShape3D *zMShape3DToPH(zMShape3D *ms);

/*! \brief input/output of multiple 3D shapes.
 *
 * zMShape3DReadFile() reads the file \a filename and
 * creates new multiple 3D shapes \a ms. The suffix can be
 * omitted; the \a filename.z3d file is examined in that
 * case.
 *
 * zMShape3DFRead() reads the information from the current
 * position of the file \a fp.
 *
 * An acceptable data file for these two functions should
 * contain the information of 1) optical information sets and
 * 2) shapes. Each information is separeted by tags [optic]
 * and [shape].
 * The denotation of optical information and 3D shape
 * follows each format.
 * See also zOpticalInfoFRead() and zShape3DFRead().
 *
 * zMShape3DWriteFile() writes the information of multiple
 * 3D shapes \a ms to the file \a filename. In this function,
 * one can also omit the suffix as zMShape3DReadFile().
 *
 * zMShape3DFWrite() writes the information of \a ms to the
 * current position of the file \a fp in the same format with
 * zMShape3DWriteFile().
 *
 * zShape3DWrite() writes the information of \a ms simply to
 * the standard output.
 * \return
 * Each of zMShape3DReadFile() and zMShape3DFRead() returns
 * a pointer to \a ms if it succeeds to build new multiple
 * 3D shapes. Otherwise, the null pointer is returned.
 *
 * zMShape3DWriteFile(), zMShape3DFWrite() and zMShape3DWrite()
 * return no values.
 * \sa
 * zOpticalInfoFRead, zShape3DFRead
 */
#define ZMULTISHAPE3D_SUFFIX "z3d"
__EXPORT zMShape3D *zMShape3DReadFile(zMShape3D *ms, char filename[]);
__EXPORT zMShape3D *zMShape3DFRead(FILE *fp, zMShape3D *ms);
__EXPORT bool zMShape3DWriteFile(zMShape3D *ms, char filename[]);
__EXPORT void zMShape3DFWrite(FILE *fp, zMShape3D *ms);
#define zMShape3DWrite(s) zMShape3DFWrite( stdout, (s) )

__END_DECLS

#endif /* __ZEO_MSHAPE_H__ */

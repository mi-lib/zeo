/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_mshape - multiple 3D shape
 */

#ifndef __ZEO_MSHAPE_H__
#define __ZEO_MSHAPE_H__

#include <zeo/zeo_shape.h>

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zMShape3D
 * multiple 3D shape class
 * ********************************************************** */

typedef struct{
  zShape3DArray shape;
  zOpticalInfoArray optic;
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

/*! \brief register a definition of tag-and-keys for multiple shapes to a ZTK format processor. */
__EXPORT bool zMShape3DRegZTK(ZTK *ztk);

/*! \brief read multiple 3D shapes from a ZTK format processor. */
__EXPORT zMShape3D *zMShape3DFromZTK(zMShape3D *ms, ZTK *ztk);

/*! \brief read multiple 3D shapes from a ZTK format file. */
__EXPORT zMShape3D *zMShape3DReadZTK(zMShape3D *ms, char filename[]);

/*! \brief scan and print multiple 3D shapes.
 *
 * zMShape3DScanFile() scans a file \a filename and creates
 * new multiple 3D shapes \a ms. The suffix can be omitted;
 * the \a filename.ztk file is examined in that case.
 *
 * zMShape3DFScan() scans information from the current
 * position of the file \a fp.
 *
 * An acceptable data file for these two functions should
 * contain the information of 1) optical information sets and
 * 2) shapes. Each information is separeted by tags [optic]
 * and [shape].
 * The denotation of optical information and 3D shape
 * follows each format.
 * See also zOpticalInfoFScan() and zShape3DFScan().
 *
 * zMShape3DPrintFile() prints information of multiple 3D
 * shapes \a ms to a file \a filename. The suffix can be
 * omitted as well as zMShape3DScanFile().
 *
 * zMShape3DFPrint() prints the information of \a ms out to
 * the current position of a file \a fp in the same format with
 * zMShape3DPrintFile().
 *
 * zShape3DPrint() prints information of \a ms out to the
 * standard output.
 * \return
 * zMShape3DScanFile() and zMShape3DFScan() return a pointer
 * \a ms if succeeding to build new multiple 3D shapes.
 * Otherwise, the null pointer is returned.
 *
 * zMShape3DPrintFile(), zMShape3DFPrint() and zMShape3DPrint()
 * return no values.
 * \sa
 * zOpticalInfoFScan, zShape3DFScan
 */
__EXPORT zMShape3D *zMShape3DFScan(FILE *fp, zMShape3D *ms);
__EXPORT bool zMShape3DPrintFile(zMShape3D *ms, char filename[]);
__EXPORT void zMShape3DFPrint(FILE *fp, zMShape3D *ms);
#define zMShape3DPrint(s) zMShape3DFPrint( stdout, (s) )

__END_DECLS

#endif /* __ZEO_MSHAPE_H__ */

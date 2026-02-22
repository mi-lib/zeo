/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_list - 3D shape: shape list
 */

#ifndef __ZEO_SHAPE3D_LIST_H__
#define __ZEO_SHAPE3D_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/*! \struct zShape3DListCell & zShape3DList
 * \brief shape list class
 */
zListClass( zShape3DList, zShape3DListCell, zShape3D* );

#define zShape3DListCellShape(c)      (c)->data
#define zShape3DListCellOptic(c)      zShape3DOptic(zShape3DListCellShape(c))
#define zShape3DListCellSetShape(c,s) ( zShape3DListCellShape(c) = (s) )

#define zShape3DListCellInit(c) do{\
  zListCellInit( c );\
  zShape3DListCellSetShape( c, NULL );\
} while(0)

/*! \brief push and pop a shape from a list.
 *
 * zShape3DListPush() pushes a new shape \a shape to a list of shapes \a list.
 *
 * zShape3DListPop() pops the last shape of \a list.
 *
 * zShape3DListDestroy() destroys a shape list \a list.
 * \notes
 * When \a list includes statically-allocated cells, zShape3DListDestroy() violates the memory segmentation.
 * \return
 * zShape3DListPush() returns a pointer to the pushed cell.
 * zShape3DListPop() returns a pointer to the poped shape.
 * zShape3DListDestroy() returns no value.
 */
__ZEO_EXPORT zShape3DListCell *zShape3DListPush(zShape3DList *list, zShape3D *shape);
__ZEO_EXPORT zShape3D *zShape3DListPop(zShape3DList *list);
#define zShape3DListDestroy(list) zListDestroy( zShape3DListCell, list )

/*! \brief contiguous vertex of a list of 3D shapes to the given 3D point.
 */
__ZEO_EXPORT const zVec3D *zShape3DListContigVert(const zShape3DList *list, const zVec3D *point, double *distance);
__ZEO_EXPORT double zShape3DListClosest(const zShape3DList *list, const zVec3D *point, zVec3D *closestpoint);

/*! \brief print out a shape list to a file in a ZTK format.
 *
 * zShape3DListCellFPrintZTK() prints out properties of a cell of
 * a shape list \a cell to the current position of a file \a fp
 * in a ZTK format conforming to that of zShape3D.
 *
 * zShape3DListCellPrintZTK() prints out properties of \a cell to
 * the standard output.
 * \return
 * Neither zShape3DListCellFPrintZTK() nor zShape3DListCellPrintZTK()
 * return any values.
 */
__ZEO_EXPORT void zShape3DListCellFPrintZTK(FILE *fp, const zShape3DListCell *cell);
#define zShape3DListCellPrintZTK(c) zShape3DListCellFPrintZTK( stdout, (c) )
__ZEO_EXPORT void zShape3DListFPrintZTK(FILE *fp, const zShape3DList *list);
#define zShape3DListPrintZTK(l) zShape3DListFPrintZTK( stdout, (l) )

__END_DECLS

#endif /* __ZEO_SHAPE3D_LIST_H__ */

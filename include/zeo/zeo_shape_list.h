/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape_list - 3D shape: shape list
 */

#ifndef __ZEO_SHAPE_LIST_H__
#define __ZEO_SHAPE_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zShapeListCell & zShapeList
 * shape list class
 * ********************************************************** */

zListClass( zShapeList, zShapeListCell, zShape3D* );

#define zShapeListCellShape(c)      (c)->data
#define zShapeListCellOptic(c)      zShape3DOptic(zShapeListCellShape(c))
#define zShapeListCellSetShape(c,s) ( zShapeListCellShape(c) = (s) )

#define zShapeListCellInit(c) do{\
  zListCellInit( c );\
  zShapeListCellSetShape( c, NULL );\
} while(0)

/*! \brief push and pop a shape from a list.
 *
 * zShapeListPush() pushes a new shape \a shape to a list of
 * shapes \a l.
 *
 * zShapeListPop() pops the last shape of \a l.
 *
 * zShapeListDestroy() destroys a shape list \a l.
 * \notes
 * When \a l includes statically-allocated cells,
 * zShapeListDestroy() violates the memory segmentation.
 * \return
 * zShapeListPush() returns a pointer to the pushed cell.
 * zShapeListPop() returns a pointer to the poped shape.
 * zShapeListDestroy() returns no value.
 */
__EXPORT zShapeListCell *zShapeListPush(zShapeList *l, zShape3D *shape);
__EXPORT zShape3D *zShapeListPop(zShapeList *l);
#define zShapeListDestroy(l) zListDestroy( zShapeListCell, l )

/*
 */
__EXPORT zVec3D *zShapeListContigVert(zShapeList *l, zVec3D *p, double *d);
__EXPORT double zShapeListClosest(zShapeList *l, zVec3D *p, zVec3D *cp);

/*! \brief print out a shape list to a file in a ZTK format.
 *
 * zShapeListCellFPrintZTK() prints out properties of a cell of
 * a shape list \a cell to the current position of a file \a fp
 * in a ZTK format conforming to that of zShape3D.
 *
 * zShapeListCellPrintZTK() prints out properties of \a cell to
 * the standard output.
 * \return
 * Neither zShapeListCellFPrintZTK() nor zShapeListCellPrintZTK()
 * return any values.
 */
__EXPORT void zShapeListCellFPrintZTK(FILE *fp, zShapeListCell *cell);
#define zShapeListCellPrintZTK(c) zShapeListCellFPrintZTK( stdout, (c) )
__EXPORT void zShapeListFPrintZTK(FILE *fp, zShapeList *list);
#define zShapeListPrintZTK(l) zShapeListFPrintZTK( stdout, (l) )

__END_DECLS

#endif /* __ZEO_SHAPE_LIST_H__ */

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

/* METHOD:
 * zShapeListPush, zShapeListPop, zShapeListDestroy
 * - push and pop of shape list.
 *
 * 'zShapeListPush()' pushes a new shape 'shape' to the
 * shape list 'l'.
 * #
 * 'zShapeListPop()' pops the last shape of 'l'.
 * #
 * 'zShapeListDestroy()' destroys 'l', freeing all cells.
 * [NOTES]
 * When 'l' includes statically-allocated cells,
 * 'zShapeListDestroy()' causes segmentation fault.
 * [RETURN VALUE]
 * 'zShapeListPush()' returns a pointer to the cell pushed.
 * 'zShapeListPop()' returns a pointer to the shape poped.
 * 'zShapeListDestroy()' returns no value.
 */
__EXPORT zShapeListCell *zShapeListPush(zShapeList *l, zShape3D *shape);
__EXPORT zShape3D *zShapeListPop(zShapeList *l);
#define zShapeListDestroy(l) zListDestroy( zShapeListCell, l )

/*
 */
__EXPORT zVec3D *zShapeListContigVert(zShapeList *l, zVec3D *p, double *d);
__EXPORT double zShapeListClosest(zShapeList *l, zVec3D *p, zVec3D *cp);

/* METHOD:
 * zShapeListCellFWrite, zShapeListCellWrite,
 * zShapeListFWrite, zShapeListWrite,
 * - output of shape list.
 *
 * 'zShapeListCellFWrite()' writes out the properties
 * of the shape list cell 'cell' to the current position
 * in the file 'fp'. The format is according to that of zShape3D.
 * #
 * 'zShapeListCellWrite()' writes out the properties of
 * 'cell' simply to the standard output.
 * [RETURN VALUE]
 * Neither 'zShapeListCellFWrite()' nor 'zShapeListCellWrite()'
 * returns any values.
 */
__EXPORT void zShapeListCellFWrite(FILE *fp, zShapeListCell *cell);
#define zShapeListCellWrite(c) zShapeListCellFWrite( stdout, (c) )
__EXPORT void zShapeListFWrite(FILE *fp, zShapeList *list);
#define zShapeListWrite(l) zShapeListFWrite( stdout, (l) )

__END_DECLS

#endif /* __ZEO_SHAPE_LIST_H__ */

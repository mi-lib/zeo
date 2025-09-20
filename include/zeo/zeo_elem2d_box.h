/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d_box - 2D box.
 */

#ifndef __ZEO_ELEM2D_BOX_H__
#define __ZEO_ELEM2D_BOX_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief 2D axis-aligned box class.
 *//* ******************************************************* */
typedef struct{
  zVec2D min; /*!< minimum coordinates */
  zVec2D max; /*!< maximum coordinates */
} zAABox2D;

/*! \brief initializea a 2D axis-aligned box */
__ZEO_EXPORT zAABox2D *zAABox2DInit(zAABox2D *box);

/*! \brief createa a 2D axis-aligned box */
__ZEO_EXPORT zAABox2D *zAABox2DCreate(zAABox2D *box, double xmin, double ymin, double xmax, double ymax);

/*! \brief copy a 2D axis-aligned box. */
__ZEO_EXPORT zAABox2D *zAABox2DCopy(zAABox2D *src, zAABox2D *dst);

/*! \brief merge two 2D axis-aligned boxes. */
__ZEO_EXPORT zAABox2D *zAABox2DMerge(zAABox2D *box, zAABox2D *b1, zAABox2D *b2);

/*! \brief check if a point is inside of a 2D axis-aligned box. */
__ZEO_EXPORT bool zAABox2DPointIsInside(zAABox2D *box, zVec2D *p, double margin);

/*! \brief compute area of a 2D axis-aligned box. */
__ZEO_EXPORT double zAABox2DArea(zAABox2D *box);

/*! \brief print out a 2D axis-aligned box to a file. */
__ZEO_EXPORT void zAABox2DValueFPrint(FILE *fp, zAABox2D *box);

__END_DECLS

#endif /* __ZEO_ELEM2D_BOX_H__ */

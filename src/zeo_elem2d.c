/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem2d - 2D element.
 */

#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* CLASS: zTri2D
 * 2D triangle class
 * ********************************************************** */

/* create a triangle. */
zTri2D *zTri2DCreate(zTri2D *t, zVec2D *v1, zVec2D *v2, zVec2D *v3)
{
  zTri2DSetVert( t, 0, v1 );
  zTri2DSetVert( t, 1, v2 );
  zTri2DSetVert( t, 2, v3 );
  return t;
}

/* print information of a triangle to a file. */
void zTri2DFPrint(FILE *fp, zTri2D *t)
{
  if( !t )
    fprintf( fp, "(null triangle)\n" );
  else{
    fprintf( fp, "vert: 0 " );
    zVec2DFPrint( fp, zTri2DVert(t,0) );
    fprintf( fp, "vert: 1 " );
    zVec2DFPrint( fp, zTri2DVert(t,1) );
    fprintf( fp, "vert: 2 " );
    zVec2DFPrint( fp, zTri2DVert(t,2) );
  }
}

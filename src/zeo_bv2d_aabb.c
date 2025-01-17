/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv2d_aabb - 2D bounding volume: axis-aligned bounding box.
 */

#include <zeo/zeo_bv2d.h>

/* enlarge bounding box if a 2D point is out of the current box. */
static int _zAABB2DTest(zAABox2D *bb, zVec2D *p, zDir u)
{
  if( p->e[(int)u] > bb->max.e[(int)u] ){
    bb->max.e[(int)u] = p->e[(int)u];
    return 0;
  }
  if( p->e[(int)u] < bb->min.e[(int)u] ){
    bb->min.e[(int)u] = p->e[(int)u];
    return 2;
  }
  return -1;
}

/* bounding box of 2D points. */
zAABox2D *zVec2DDataAABB(zVec2DData *data, zAABox2D *bb, zVec2D **vp)
{
  zVec2D *v;
  int s;

  if( zVec2DDataIsEmpty( data ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zAABox2DInit( bb );
  zVec2DDataRewind( data );
  v = zVec2DDataFetch( data );
  zVec2DCopy( v, &bb->min );
  zVec2DCopy( v, &bb->max );
  if( vp ) vp[0] = vp[1] = vp[2] = vp[3] = v;
  while( ( v = zVec2DDataFetch( data ) ) ){
    if( ( s = _zAABB2DTest( bb, v, zX ) ) != -1 && vp ) vp[zX+s] = v;
    if( ( s = _zAABB2DTest( bb, v, zY ) ) != -1 && vp ) vp[zY+s] = v;
  }
  return bb;
}

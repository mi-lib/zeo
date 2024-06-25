/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_box - collision checking: axis-aligned box and oriented box.
 */

#include <zeo/zeo_col.h>

/* axis-aligned box */

/* check if an edge of an axis-aligned box intersects with a plane. */
static int _zIntersectPlaneAABox3DEdge(zPlane3D *p, zAxis axis, double w1min, double w1max, double w2, double w3, zVec3D *ip)
{
  zAxis a2, a3;
  double w1;

  if( zIsTiny( zPlane3DNorm(p)->e[(int)axis] ) ) return 0;
  a2 = ( axis + 1 ) % 3;
  a3 = ( axis + 2 ) % 3;
  w1 = ( zVec3DInnerProd(zPlane3DNorm(p),zPlane3DOrg(p))
       - zPlane3DNorm(p)->e[(int)a2]*w2
       - zPlane3DNorm(p)->e[(int)a3]*w3 ) / zPlane3DNorm(p)->e[(int)axis];
  if( w1 > w1min && w1 < w1max ){
    ip->e[(int)axis] = w1;
    ip->e[(int)a2] = w2;
    ip->e[(int)a3] = w3;
    return 1;
  }
  return 0;
}

/* check if a plane and an axis-aligned boxes intersect with each other. */
bool zColChkPlaneAABox3D(zPlane3D *p, zAABox3D *box)
{
  double x1, y1, z1, x2, y2, z2;
  zVec3D ip;

  x1 = box->min.e[zX];
  y1 = box->min.e[zY];
  z1 = box->min.e[zZ];
  x2 = box->max.e[zX];
  y2 = box->max.e[zY];
  z2 = box->max.e[zZ];
  return _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y1, z1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y2, z1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y2, z2, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y1, z2, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z1, x1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z2, x1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z2, x2, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z1, x2, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x1, y1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x2, y1, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x2, y2, &ip ) > 0 ||
         _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x1, y2, &ip ) > 0 ?
    true : false;
}

/* get an intersection between an axis-aligned box and a plane. */
int zIntersectPlaneAABox3D(zPlane3D *p, zAABox3D *box, zVec3D ip[])
{
  int n = 0;
  double x1, y1, z1, x2, y2, z2;

  x1 = box->min.e[zX];
  y1 = box->min.e[zY];
  z1 = box->min.e[zZ];
  x2 = box->max.e[zX];
  y2 = box->max.e[zY];
  z2 = box->max.e[zZ];
  n += _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y1, z1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y2, z1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y2, z2, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zX, x1, x2, y1, z2, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z1, x1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z2, x1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z2, x2, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zY, y1, y2, z1, x2, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x1, y1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x2, y1, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x2, y2, &ip[n] );
  n += _zIntersectPlaneAABox3DEdge( p, zZ, z1, z2, x1, y2, &ip[n] );
  return n;
}

/* check if an edge of a triangle intersects with a face of an axis-aligned box. */
static bool _zColChkTriAABox3DEdgeAxOne(zVec3D *v1, zVec3D *v2, zAxis axis, zVec3D *p, zAABox3D *box, zVec3D *ip)
{
  double d1, d2, s1, s2;
  int a1, a2;

  d1 = v1->e[(int)axis] - p->e[(int)axis];
  d2 = v2->e[(int)axis] - p->e[(int)axis];
  if( ( d1 > 0 && d2 > 0 ) || ( d1 < 0 && d2 < 0 ) ) return false;
  a1 = ( axis + 1 ) % 3;
  s1 = v1->e[a1] - ( v2->e[a1] - v1->e[a1] ) * d1 / ( d2 - d1 );
  a2 = ( axis + 2 ) % 3;
  s2 = v1->e[a2] - ( v2->e[a2] - v1->e[a2] ) * d1 / ( d2 - d1 );
  if( s1 > box->min.e[a1] && s1 < box->max.e[a1] &&
      s2 > box->min.e[a2] && s2 < box->max.e[a2] ){
    if( ip ){
      ip->e[(int)axis] = p->e[(int)axis];
      ip->e[a1] = s1;
      ip->e[a2] = s2;
    }
    return true;
  }
  return false;
}

/* check if an edge of a triangle intersects with a pair of parallel faces of an axis-aligned box. */
static bool _zColChkTriAABox3DEdgeAx(zVec3D *v1, zVec3D *v2, zAxis axis, zAABox3D *box)
{
  return _zColChkTriAABox3DEdgeAxOne( v1, v2, axis, &box->min, box, NULL ) ||
         _zColChkTriAABox3DEdgeAxOne( v1, v2, axis, &box->max, box, NULL ) ?
    true : false;
}

/* check if an edge of a triangle intersects with an axis-aligned box. */
static bool _zColChkTriAABox3DEdge(zVec3D *v1, zVec3D *v2, zAABox3D *box)
{
  return _zColChkTriAABox3DEdgeAx( v1, v2, zX, box ) ||
         _zColChkTriAABox3DEdgeAx( v1, v2, zY, box ) ||
         _zColChkTriAABox3DEdgeAx( v1, v2, zZ, box ) ? true : false;
}

/* check if a triangle intersects with edges of an axis-aligned box. */
static bool _zColChkTriAABox3DPlane(zTri3D *t, zAABox3D *box)
{
  zPlane3D p;
  zVec3D ip[6];
  int i, n;

  zTri3DToPlane3D( t, &p );
  n = zIntersectPlaneAABox3D( &p, box, ip );
  for( i=0; i<n; i++ )
    if( zTri3DPointIsInside( t, &ip[i], -zTOL ) ) return true;
  return false;
}

/* check if a triangle intersects with an axis-aligned box. */
bool zColChkTriAABox3D(zTri3D *t, zAABox3D *box)
{
  if( zAABox3DPointIsInside( box, zTri3DVert(t,0), -zTOL ) ||
      zAABox3DPointIsInside( box, zTri3DVert(t,1), -zTOL ) ||
      zAABox3DPointIsInside( box, zTri3DVert(t,2), -zTOL ) )
    return true;
  if( _zColChkTriAABox3DEdge( zTri3DVert(t,0), zTri3DVert(t,1), box ) ||
      _zColChkTriAABox3DEdge( zTri3DVert(t,1), zTri3DVert(t,2), box ) ||
      _zColChkTriAABox3DEdge( zTri3DVert(t,2), zTri3DVert(t,0), box ) )
    return true;
  return _zColChkTriAABox3DPlane( t, box );
}

/* intersection of an edge of a triangle and a face of an axis-aligned box. */
static int _zIntersectTriAABox3DEdgeAxOne(zVec3D *v1, zVec3D *v2, zAxis axis, zVec3D *p, zAABox3D *box, zVec3D ip[], int n)
{
  if( _zColChkTriAABox3DEdgeAxOne( v1, v2, axis, p, box, &ip[n] ) ) n++;
  return n;
}

/* intersection of an edge of a triangle and a pair of parallel faces of an axis-aligned box. */
static int _zIntersectTriAABox3DEdgeAx(zVec3D *v1, zVec3D *v2, zAxis axis, zAABox3D *box, zVec3D ip[], int n)
{
  n = _zIntersectTriAABox3DEdgeAxOne( v1, v2, axis, &box->min, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAxOne( v1, v2, axis, &box->max, box, ip, n );
  return n;
}

/* intersection of an edge of a triangle and an axis-aligned box. */
static int _zIntersectTriAABox3DEdge(zVec3D *v1, zVec3D *v2, zAABox3D *box, zVec3D ip[], int n)
{
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zX, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zY, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zZ, box, ip, n );
  return n;
}

/* intersection of a triangle and edges of an axis-aligned box. */
static int _zIntersectTriAABox3DPlane(zTri3D *t, zAABox3D *box, zVec3D ip[], int n)
{
  zPlane3D p;
  zVec3D pip[6];
  int i, np;

  zTri3DToPlane3D( t, &p );
  np = zIntersectPlaneAABox3D( &p, box, pip );
  for( i=0; i<np; i++ )
    if( zTri3DPointIsInside( t, &pip[i], -zTOL ) )
      zVec3DCopy( &pip[i], &ip[n++] );
  return n;
}

/* intersection of a triangle and an axis-aligned box. */
int zIntersectTriAABox3D(zTri3D *t, zAABox3D *box, zVec3D ip[])
{
  int n = 0;

  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,0), zTri3DVert(t,1), box, ip, n );
  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,1), zTri3DVert(t,2), box, ip, n );
  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,2), zTri3DVert(t,0), box, ip, n );
  n = _zIntersectTriAABox3DPlane( t, box, ip, n );
  return n;
}

/* check if two axis-aligned boxes intersect with each other. */
bool zColChkAABox3D(zAABox3D *b1, zAABox3D *b2)
{
  return b1->min.e[zX] < b2->max.e[zX] &&
         b2->min.e[zX] < b1->max.e[zX] &&
         b1->min.e[zY] < b2->max.e[zY] &&
         b2->min.e[zY] < b1->max.e[zY] &&
         b1->min.e[zZ] < b2->max.e[zZ] &&
         b2->min.e[zZ] < b1->max.e[zZ] ?
    true : false;
}

/* intersection of two axis-aligned boxes. */
zAABox3D *zIntersectAABox3D(zAABox3D *dst, zAABox3D *src1, zAABox3D *src2)
{
  if( !zColChkAABox3D( src1, src2 ) ) return NULL;
  return zAABox3DCreate( dst,
    _zMax( src1->min.e[zX], src2->min.e[zX] ),
    _zMax( src1->min.e[zY], src2->min.e[zY] ),
    _zMax( src1->min.e[zZ], src2->min.e[zZ] ),
    _zMin( src1->max.e[zX], src2->max.e[zX] ),
    _zMin( src1->max.e[zY], src2->max.e[zY] ),
    _zMin( src1->max.e[zZ], src2->max.e[zZ] ) );
}

/* intersection of AABBs of two polyhedra. */
zAABox3D *zIntersectPH3DBox(zPH3D *ph1, zPH3D *ph2, zAABox3D *box)
{
  zAABox3D b1, b2;

  zAABB3D( &b1, zPH3DVertBuf(ph1), zPH3DVertNum(ph1), NULL );
  zAABB3D( &b2, zPH3DVertBuf(ph2), zPH3DVertNum(ph2), NULL );
  return zIntersectAABox3D( box, &b1, &b2 ) ? box : NULL;
}

/* box vs box */

/* deflated collision checking along an axis on a box. */
static bool _zColChkBox3DAlong(zBox3D *b1, zDir axis, zBox3D *b2, zVec3D *l)
{
  zVec3D *a;
  double d;

  a = zBox3DAxis( b1, (int)axis );
  d = 0.5*
     ( fabs( zVec3DInnerProd(zBox3DAxis(b2,zX),a) )*zBox3DDepth(b2)
     + fabs( zVec3DInnerProd(zBox3DAxis(b2,zY),a) )*zBox3DWidth(b2)
     + fabs( zVec3DInnerProd(zBox3DAxis(b2,zZ),a) )*zBox3DHeight(b2)
     + zBox3DDia(b1,(int)axis) );
  return fabs( zVec3DInnerProd(l,a) ) < d ? true : false;
}

/* deflated collision checking along an axis perpendicular to edges on boxes. */
static bool _zColChkBox3DPerp(zBox3D *b1, zDir ax1, zBox3D *b2, zDir ax2, zVec3D *l)
{
  zVec3D p;
  zDir ax12, ax13, ax22, ax23;
  double d;

  zVec3DOuterProd( zBox3DAxis(b1,(int)ax1), zBox3DAxis(b2,(int)ax2), &p );
  if( zVec3DIsTiny( &p ) )
    return true; /* parallel edges, should have been already checked. */
  zVec3DNormalizeDRC( &p );
  ax12 = ( ax1 + 1 ) % 3;
  ax13 = ( ax1 + 2 ) % 3;
  ax22 = ( ax2 + 1 ) % 3;
  ax23 = ( ax2 + 2 ) % 3;
  d = 0.5*
    ( fabs( zVec3DInnerProd(zBox3DAxis(b1,(int)ax12),&p) )*zBox3DDia(b1,(int)ax12)
    + fabs( zVec3DInnerProd(zBox3DAxis(b1,(int)ax13),&p) )*zBox3DDia(b1,(int)ax13)
    + fabs( zVec3DInnerProd(zBox3DAxis(b2,(int)ax22),&p) )*zBox3DDia(b2,(int)ax22)
    + fabs( zVec3DInnerProd(zBox3DAxis(b2,(int)ax23),&p) )*zBox3DDia(b2,(int)ax23) );
  return fabs( zVec3DInnerProd(l,&p) ) < d ? true : false;
}

/* check if two (oriented) boxes intersect with each other. */
bool zColChkBox3D(zBox3D *b1, zBox3D *b2)
{
  zVec3D l;

  zVec3DSub( zBox3DCenter(b1), zBox3DCenter(b2), &l );
  return !_zColChkBox3DAlong( b1, zX, b2, &l ) ||
         !_zColChkBox3DAlong( b1, zY, b2, &l ) ||
         !_zColChkBox3DAlong( b1, zZ, b2, &l ) ||
         !_zColChkBox3DAlong( b2, zX, b1, &l ) ||
         !_zColChkBox3DAlong( b2, zY, b1, &l ) ||
         !_zColChkBox3DAlong( b2, zZ, b1, &l ) ||
         !_zColChkBox3DPerp( b1, zX, b2, zX, &l ) ||
         !_zColChkBox3DPerp( b1, zY, b2, zX, &l ) ||
         !_zColChkBox3DPerp( b1, zZ, b2, zX, &l ) ||
         !_zColChkBox3DPerp( b1, zX, b2, zY, &l ) ||
         !_zColChkBox3DPerp( b1, zY, b2, zY, &l ) ||
         !_zColChkBox3DPerp( b1, zZ, b2, zY, &l ) ||
         !_zColChkBox3DPerp( b1, zX, b2, zZ, &l ) ||
         !_zColChkBox3DPerp( b1, zY, b2, zZ, &l ) ||
         !_zColChkBox3DPerp( b1, zZ, b2, zZ, &l ) ? false : true;
}

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_box - collision checking: axis-aligned box and oriented box.
 */

#include <zeo/zeo_col.h>

/* axis-aligned box */

/* check if an edge of an axis-aligned box intersects with a plane. */
static int _zIntersectPlaneAABox3DEdge(const zPlane3D *p, zAxis axis, double w1min, double w1max, double w2, double w3, zVec3D *ip)
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

/* check if a plane and an axis-aligned box are in collision with each other.
 * The signed distance between them is max(dmin,-dmax), where dmin and dmax are the minimum and
 * maximum signed distances of points in the box from the plane, respectively. */
bool zColChkPlaneAABox3D(const zPlane3D *p, const zAABox3D *box, double margin)
{
  double dmin, dmax;

  zAABox3DDistRangeFromPlane( box, p, &dmin, &dmax );
  return _zMax( dmin, -dmax ) < margin;
}

/* get an intersection between an axis-aligned box and a plane. */
int zIntersectPlaneAABox3D(const zPlane3D *p, const zAABox3D *box, zVec3D ip[])
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
static bool _zColChkTriAABox3DEdgeAxOne(const zVec3D *v1, const zVec3D *v2, zAxis axis, const zVec3D *p, const zAABox3D *box, zVec3D *ip)
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

/* separation of a triangle and an axis-aligned box along an axis, where the vertices of the
 * triangle are relative to the center of the box, and h is the half of the size of the box. */
static double _zColChkTriAABox3DSep(const zVec3D v[], const zVec3D *h, const zVec3D *a)
{
  double l, p, pmin, pmax, r;
  int i;

  if( ( l = zVec3DNorm( a ) ) == 0 ) return -HUGE_VAL; /* not an axis */
  pmin = pmax = zVec3DInnerProd( &v[0], a );
  for( i=1; i<3; i++ ){
    p = zVec3DInnerProd( &v[i], a );
    if( p < pmin ) pmin = p;
    if( p > pmax ) pmax = p;
  }
  r = h->c.x * fabs( a->c.x ) + h->c.y * fabs( a->c.y ) + h->c.z * fabs( a->c.z );
  return _zMax( pmin - r, -pmax - r ) / l;
}

/* check if a triangle and an axis-aligned box apart from each other are within a margin. */
static bool _zColChkTriAABox3DGJK(const zTri3D *t, const zAABox3D *box, double margin)
{
  zVec3D v[3], bv[8];
  int i;

  for( i=0; i<3; i++ ) zVec3DCopy( zTri3DVert(t,i), &v[i] );
  for( i=0; i<8; i++ ) zAABox3DVert( box, i, &bv[i] );
  return _zColChkVec3DArray( v, 3, bv, 8, margin );
}

/* check if a triangle and an axis-aligned box are in collision with each other.
 * The maximum separation of them along the axes perpendicular to faces of them and the outer
 * products of edges of them is the signed distance between them if it is non-positive (separating
 * axis theorem), and is a lower bound of the distance otherwise. In the latter case, the distance is
 * computed by the GJK algorithm only if the lower bound is less than the margin. */
bool zColChkTriAABox3D(const zTri3D *t, const zAABox3D *box, double margin)
{
  zVec3D c, h, v[3], e, a;
  double s, smax;
  int i, j;

  zAABox3DCenter( box, &c );
  zVec3DSub( &box->max, &c, &h );
  for( i=0; i<3; i++ ) zVec3DSub( zTri3DVert(t,i), &c, &v[i] );
  /* normal vectors of the box */
  smax = -HUGE_VAL;
  for( i=0; i<3; i++ ){
    zVec3DZero( &a );
    a.e[i] = 1;
    if( ( s = _zColChkTriAABox3DSep( v, &h, &a ) ) >= margin ) return false;
    if( s > smax ) smax = s;
  }
  /* normal vector of the triangle */
  zVec3DSub( &v[1], &v[0], &e );
  zVec3DSub( &v[2], &v[1], &a );
  zVec3DOuterProd( &e, &a, &a );
  if( ( s = _zColChkTriAABox3DSep( v, &h, &a ) ) >= margin ) return false;
  if( s > smax ) smax = s;
  /* outer products of edges of the triangle and the box */
  for( j=0; j<3; j++ ){
    zVec3DSub( &v[(j+1)%3], &v[j], &e );
    for( i=0; i<3; i++ ){
      zVec3DZero( &a );
      a.e[(i+1)%3] = -e.e[(i+2)%3];
      a.e[(i+2)%3] = e.e[(i+1)%3];
      if( ( s = _zColChkTriAABox3DSep( v, &h, &a ) ) >= margin ) return false;
      if( s > smax ) smax = s;
    }
  }
  return smax <= 0 ? smax < margin : _zColChkTriAABox3DGJK( t, box, margin );
}

/* intersection of an edge of a triangle and a face of an axis-aligned box. */
static int _zIntersectTriAABox3DEdgeAxOne(const zVec3D *v1, const zVec3D *v2, zAxis axis, const zVec3D *p, const zAABox3D *box, zVec3D ip[], int n)
{
  if( _zColChkTriAABox3DEdgeAxOne( v1, v2, axis, p, box, &ip[n] ) ) n++;
  return n;
}

/* intersection of an edge of a triangle and a pair of parallel faces of an axis-aligned box. */
static int _zIntersectTriAABox3DEdgeAx(const zVec3D *v1, const zVec3D *v2, zAxis axis, const zAABox3D *box, zVec3D ip[], int n)
{
  n = _zIntersectTriAABox3DEdgeAxOne( v1, v2, axis, &box->min, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAxOne( v1, v2, axis, &box->max, box, ip, n );
  return n;
}

/* intersection of an edge of a triangle and an axis-aligned box. */
static int _zIntersectTriAABox3DEdge(const zVec3D *v1, const zVec3D *v2, const zAABox3D *box, zVec3D ip[], int n)
{
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zX, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zY, box, ip, n );
  n = _zIntersectTriAABox3DEdgeAx( v1, v2, zZ, box, ip, n );
  return n;
}

/* intersection of a triangle and edges of an axis-aligned box. */
static int _zIntersectTriAABox3DPlane(const zTri3D *t, const zAABox3D *box, zVec3D ip[], int n)
{
  zPlane3D p;
  zVec3D pip[6];
  int i, np;

  zTri3DToPlane3D( t, &p );
  np = zIntersectPlaneAABox3D( &p, box, pip );
  for( i=0; i<np; i++ )
    if( zTri3DPointIsInside( t, &pip[i], zTOL ) )
      zVec3DCopy( &pip[i], &ip[n++] );
  return n;
}

/* intersection of a triangle and an axis-aligned box. */
int zIntersectTriAABox3D(const zTri3D *t, const zAABox3D *box, zVec3D ip[])
{
  int n = 0;

  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,0), zTri3DVert(t,1), box, ip, n );
  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,1), zTri3DVert(t,2), box, ip, n );
  n = _zIntersectTriAABox3DEdge( zTri3DVert(t,2), zTri3DVert(t,0), box, ip, n );
  n = _zIntersectTriAABox3DPlane( t, box, ip, n );
  return n;
}

/* check if two axis-aligned boxes are in collision with each other.
 * The signed distance between them is the maximum of the gaps along the axes if all of them are
 * non-positive, and the norm of the positive gaps otherwise. */
bool zColChkAABox3D(const zAABox3D *b1, const zAABox3D *b2, double margin)
{
  double gap, gapmax = -HUGE_VAL, d2 = 0;
  int k;

  for( k=0; k<3; k++ ){
    gap = _zMax( b1->min.e[k] - b2->max.e[k], b2->min.e[k] - b1->max.e[k] );
    if( gap >= margin ) return false;
    if( gap > gapmax ) gapmax = gap;
    if( gap > 0 ) d2 += gap * gap;
  }
  return gapmax <= 0 ? gapmax < margin : d2 < margin * margin;
}

/* intersection of two axis-aligned boxes. */
zAABox3D *zIntersectAABox3D(zAABox3D *dst, const zAABox3D *src1, const zAABox3D *src2)
{
  if( !zColChkAABox3D( src1, src2, 0 ) ) return NULL;
  return zAABox3DCreate( dst,
    _zMax( src1->min.e[zX], src2->min.e[zX] ),
    _zMax( src1->min.e[zY], src2->min.e[zY] ),
    _zMax( src1->min.e[zZ], src2->min.e[zZ] ),
    _zMin( src1->max.e[zX], src2->max.e[zX] ),
    _zMin( src1->max.e[zY], src2->max.e[zY] ),
    _zMin( src1->max.e[zZ], src2->max.e[zZ] ) );
}

/* intersection of AABBs of two polyhedra. */
zAABox3D *zIntersectPH3DBox(const zPH3D *ph1, const zPH3D *ph2, zAABox3D *box)
{
  zAABox3D b1, b2;

  if( !zPH3DAABB( ph1, &b1 ) || !zPH3DAABB( ph2, &b2 ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  return zIntersectAABox3D( box, &b1, &b2 );
}

/* box vs box */

/* separation of two boxes along an axis of a box. */
static double _zColChkBox3DSepAlong(const zBox3D *b1, zAxis axis, const zBox3D *b2, const zVec3D *l)
{
  const zVec3D *a;
  double d;

  a = zBox3DAxis( b1, axis );
  d = 0.5*
     ( fabs( zVec3DInnerProd(zBox3DAxis(b2,zX),a) ) * zBox3DDepth(b2)
     + fabs( zVec3DInnerProd(zBox3DAxis(b2,zY),a) ) * zBox3DWidth(b2)
     + fabs( zVec3DInnerProd(zBox3DAxis(b2,zZ),a) ) * zBox3DHeight(b2)
     + zBox3DSpan(b1,axis) );
  return fabs( zVec3DInnerProd(l,a) ) - d;
}

/* separation of two boxes along an axis perpendicular to edges of them. */
static double _zColChkBox3DSepPerp(const zBox3D *b1, zAxis ax1, const zBox3D *b2, zAxis ax2, const zVec3D *l)
{
  zVec3D p;
  zAxis ax12, ax13, ax22, ax23;
  double d;

  zVec3DOuterProd( zBox3DAxis(b1,ax1), zBox3DAxis(b2,ax2), &p );
  if( zVec3DIsTiny( &p ) )
    return -HUGE_VAL; /* parallel edges, which have been already checked. */
  zVec3DNormalizeDRC( &p, 0 );
  ax12 = ( ax1 + 1 ) % 3;
  ax13 = ( ax1 + 2 ) % 3;
  ax22 = ( ax2 + 1 ) % 3;
  ax23 = ( ax2 + 2 ) % 3;
  d = 0.5*
    ( fabs( zVec3DInnerProd(zBox3DAxis(b1,ax12),&p) )*zBox3DSpan(b1,ax12)
    + fabs( zVec3DInnerProd(zBox3DAxis(b1,ax13),&p) )*zBox3DSpan(b1,ax13)
    + fabs( zVec3DInnerProd(zBox3DAxis(b2,ax22),&p) )*zBox3DSpan(b2,ax22)
    + fabs( zVec3DInnerProd(zBox3DAxis(b2,ax23),&p) )*zBox3DSpan(b2,ax23) );
  return fabs( zVec3DInnerProd(l,&p) ) - d;
}

/* check if two boxes apart from each other are within a margin. */
static bool _zColChkBox3DGJK(const zBox3D *b1, const zBox3D *b2, double margin)
{
  zVec3D v1[8], v2[8];
  int i;

  for( i=0; i<8; i++ ){
    zBox3DVert( b1, i, &v1[i] );
    zBox3DVert( b2, i, &v2[i] );
  }
  return _zColChkVec3DArray( v1, 8, v2, 8, margin );
}

/* check if two (oriented) boxes are in collision with each other.
 * The maximum separation of them along the fifteen axes is the signed distance between them if it is
 * non-positive (separating axis theorem), and is a lower bound of the distance otherwise. In the
 * latter case, the distance is computed by the GJK algorithm only if the lower bound is less than
 * the margin. */
bool zColChkBox3D(const zBox3D *b1, const zBox3D *b2, double margin)
{
  zVec3D l;
  double s, smax = -HUGE_VAL;
  int i, j;

  zVec3DSub( zBox3DCenter(b1), zBox3DCenter(b2), &l );
  for( i=zX; i<=zZ; i++ ){
    if( ( s = _zColChkBox3DSepAlong( b1, (zAxis)i, b2, &l ) ) >= margin ) return false;
    if( s > smax ) smax = s;
    if( ( s = _zColChkBox3DSepAlong( b2, (zAxis)i, b1, &l ) ) >= margin ) return false;
    if( s > smax ) smax = s;
  }
  for( j=zX; j<=zZ; j++ )
    for( i=zX; i<=zZ; i++ ){
      if( ( s = _zColChkBox3DSepPerp( b1, (zAxis)i, b2, (zAxis)j, &l ) ) >= margin ) return false;
      if( s > smax ) smax = s;
    }
  return smax <= 0 ? smax < margin : _zColChkBox3DGJK( b1, b2, margin );
}

/* box vs sphere */

/* check collision between a 3D sphere and a 3D axis-aligned box. */
bool zColChkSphereAABox3D(const zSphere3D *sphere, const zAABox3D *box, double margin)
{
  return zAABox3DPointIsInside( box, zSphere3DCenter(sphere), zSphere3DRadius(sphere) + margin );
}

/* check collision between a 3D sphere and a 3D box. */
bool zColChkSphereBox3D(const zSphere3D *sphere, const zBox3D *box, double margin)
{
  return zBox3DPointIsInside( box, zSphere3DCenter(sphere), zSphere3DRadius(sphere) + margin );
}

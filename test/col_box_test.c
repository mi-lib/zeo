#include <zeo/zeo.h>

#define N 1000

/* a random margin and a gap that is apart from the margin, where the shapes are in collision with
 * each other if the gap is less than the margin. A negative gap means that the shapes intersect. */
static bool gap_rand(double *margin, double *gap)
{
  double d;

  *margin = zRandI(0,2) == 0 ? 0 : zRandF(-0.5,0.5);
  d = zRandF(0.01,1);
  if( zRandI(0,1) == 0 ) d = -d;
  *gap = *margin + d;
  return d < 0;
}

/* a random box, which is thick enough for the negative gaps. */
static void aabox3D_rand(zAABox3D *box)
{
  int k;

  for( k=0; k<3; k++ ){
    box->min.e[k] = zRandF(-10,10);
    box->max.e[k] = box->min.e[k] + zRandF(3,10);
  }
}

/* a box beside another along the k-th axis with a gap, which overlaps with it along the other axes
 * more than the penetration along the k-th axis, so that the signed distance is the gap. */
static void aabox3D_beside(const zAABox3D *b1, int k, double gap, zAABox3D *b2)
{
  int j;

  for( j=0; j<3; j++ ){
    if( j == k ){
      b2->min.e[j] = b1->max.e[j] + gap;
      b2->max.e[j] = b2->min.e[j] + zRandF(2,5); /* beyond b1 */
    } else{
      /* overlap more than 2 */
      b2->min.e[j] = zRandF( b1->min.e[j]-5, b1->max.e[j]-2 );
      b2->max.e[j] = zRandF( zMax( b1->min.e[j], b2->min.e[j] ) + 2, b1->max.e[j]+5 );
    }
  }
}

void assert_colchk_aabox3D(void)
{
  zAABox3D b1, b2, b;
  double margin, gap;
  int i, j, k;
  bool expected, result = true, result_intersect = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &b1 );
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    aabox3D_beside( &b1, k, gap, &b2 );
    if( zColChkAABox3D( &b1, &b2, margin ) != expected ||
        zColChkAABox3D( &b2, &b1, margin ) != expected ) result = false;
    if( ( zIntersectAABox3D( &b, &b1, &b2 ) != NULL ) != ( gap < 0 ) ) result_intersect = false;
    if( gap >= 0 ) continue;
    /* the intersection spans the overlap of the boxes */
    if( !zEqual( b.min.e[k], b2.min.e[k], zTOL ) || !zEqual( b.max.e[k], b1.max.e[k], zTOL ) ) result_intersect = false;
    for( j=0; j<3; j++ ){
      if( j == k ) continue;
      if( !zEqual( b.min.e[j], zMax( b1.min.e[j], b2.min.e[j] ), zTOL ) ||
          !zEqual( b.max.e[j], zMin( b1.max.e[j], b2.max.e[j] ), zTOL ) ) result_intersect = false;
    }
  }
  zAssert( zColChkAABox3D, result );
  zAssert( zIntersectAABox3D, result_intersect );
}

/* a random margin around the distance g*sqrt(3) between a box and another shape apart from its
 * corner by g along every axis, which are in collision if the margin is larger than the distance. */
static bool diagonal_margin_rand(double *margin, double *g)
{
  bool expected;

  *g = zRandF(0.1,1);
  expected = zRandI(0,1) == 0;
  *margin = sqrt(3) * *g * ( expected ? 1.01 : 0.99 );
  return expected;
}

void assert_colchk_aabox3D_diagonal(void)
{
  zAABox3D b1, b2;
  double margin, g;
  int i, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &b1 );
    expected = diagonal_margin_rand( &margin, &g );
    for( k=0; k<3; k++ ){
      b2.min.e[k] = b1.max.e[k] + g;
      b2.max.e[k] = b2.min.e[k] + zRandF(1,5);
    }
    if( zColChkAABox3D( &b1, &b2, margin ) != expected ) result = false;
  }
  zAssert( zColChkAABox3D (diagonal case), result );
}

void assert_colchk_plane_aabox3D(void)
{
  zAABox3D box;
  zPlane3D plane;
  zVec3D p, n, v;
  double margin, d, dmin, dmax;
  int i, j;
  bool result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &box );
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &n, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    if( i % 10 == 0 ) n.e[i/10%3] = 0; /* a plane parallel to an axis */
    if( !zPlane3DCreate( &plane, &p, &n ) ) continue;
    margin = zRandI(0,2) == 0 ? 0 : zRandF(-2,2);
    dmin = HUGE_VAL;
    dmax = -HUGE_VAL;
    for( j=0; j<8; j++ ){
      d = zPlane3DDistFromPoint( &plane, zAABox3DVert( &box, j, &v ) );
      if( d < dmin ) dmin = d;
      if( d > dmax ) dmax = d;
    }
    /* the signed distance is max(dmin,-dmax) */
    if( zIsTiny( zMax( dmin, -dmax ) - margin ) ) continue; /* marginal case */
    if( zColChkPlaneAABox3D( &plane, &box, margin ) != ( zMax( dmin, -dmax ) < margin ) ) result = false;
  }
  zAssert( zColChkPlaneAABox3D, result );
}

void assert_colchk_tri_aabox3D(void)
{
  zAABox3D box;
  zTri3D tri;
  zVec3D v[3], c, ip[24];
  double margin, gap, w;
  int i, j, k, side, n;
  bool expected, result = true, result_intersect = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &box );
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    side = zRandI(0,1);
    /* a triangle on a plane parallel to a face of the box, which covers the face */
    zAABox3DCenter( &box, &c );
    w = 2 * ( zAABox3DDepth(&box) + zAABox3DWidth(&box) + zAABox3DHeight(&box) ) + 1;
    for( j=0; j<3; j++ ){
      zVec3DCopy( &c, &v[j] );
      v[j].e[(k+1)%3] += w * cos( zPIx2*j/3 );
      v[j].e[(k+2)%3] += w * sin( zPIx2*j/3 );
      v[j].e[k] = side == 0 ? box.min.e[k] - gap : box.max.e[k] + gap;
    }
    zTri3DCreate( &tri, &v[0], &v[1], &v[2] );
    if( zColChkTriAABox3D( &tri, &box, margin ) != expected ) result = false;
    /* the triangle crossing the box intersects with its four edges */
    if( gap >= 0 ) continue;
    if( ( n = zIntersectTriAABox3D( &tri, &box, ip ) ) != 4 ) result_intersect = false;
    for( j=0; j<n; j++ )
      if( !zIsTiny( ip[j].e[k] - v[0].e[k] ) || !zAABox3DPointIsInside( &box, &ip[j], zTOL ) ) result_intersect = false;
  }
  zAssert( zColChkTriAABox3D, result );
  zAssert( zIntersectTriAABox3D, result_intersect );
}

void assert_colchk_tri_aabox3D_diagonal(void)
{
  zAABox3D box;
  zTri3D tri;
  zVec3D v[3];
  double margin, g;
  int i, j, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &box );
    expected = diagonal_margin_rand( &margin, &g );
    /* the vertex v[0] is the closest to the corner of the box */
    for( k=0; k<3; k++ ) v[0].e[k] = box.max.e[k] + g;
    for( j=1; j<3; j++ )
      for( k=0; k<3; k++ ) v[j].e[k] = v[0].e[k] + zRandF(0.1,5);
    zTri3DCreate( &tri, &v[0], &v[1], &v[2] );
    if( zColChkTriAABox3D( &tri, &box, margin ) != expected ) result = false;
  }
  zAssert( zColChkTriAABox3D (diagonal case), result );
}

void assert_colchk_sphere_aabox3D(void)
{
  zAABox3D box;
  zSphere3D sphere;
  zVec3D c;
  double margin, gap, r;
  int i, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &box );
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    r = zRandF(2,5);
    zAABox3DCenter( &box, &c );
    c.e[k] = box.max.e[k] + r + gap;
    zSphere3DCreate( &sphere, &c, r, 0 );
    if( zColChkSphereAABox3D( &sphere, &box, margin ) != expected ) result = false;
  }
  zAssert( zColChkSphereAABox3D, result );
}

static void box3D_rand(zBox3D *box)
{
  zVec3D center, ax, ay, az, tmp;

  zVec3DCreate( &center, zRandF(-3,3), zRandF(-3,3), zRandF(-3,3) );
  zVec3DCreate( &ax, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DCreate( &tmp, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DOrthogonalize( &tmp, &ax, &ay );
  zVec3DOuterProd( &ax, &ay, &az );
  zVec3DNormalizeDRC( &ax, 0 );
  zVec3DNormalizeDRC( &ay, 0 );
  zVec3DNormalizeDRC( &az, 0 );
  zBox3DCreate( box, &center, &ax, &ay, &az, zRandF(3,5), zRandF(3,5), zRandF(3,5) );
}

void assert_colchk_sphere_box3D(void)
{
  zBox3D box;
  zSphere3D sphere;
  zVec3D c;
  double margin, gap, r;
  int i, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    box3D_rand( &box );
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    r = zRandF(2,5);
    zVec3DCat( zBox3DCenter(&box), 0.5*zBox3DSpan(&box,k)+r+gap, zBox3DAxis(&box,k), &c );
    zSphere3DCreate( &sphere, &c, r, 0 );
    if( zColChkSphereBox3D( &sphere, &box, margin ) != expected ) result = false;
  }
  zAssert( zColChkSphereBox3D, result );
}

void assert_colchk_box3D(void)
{
  zBox3D b1, b2;
  zVec3D c, a[3];
  double margin, gap, l[3], theta;
  int i, j, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    box3D_rand( &b1 );
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    /* another box beside a face of the box, which is rotated about the normal of the face */
    for( j=0; j<3; j++ ) l[j] = zRandF(3,5);
    zVec3DCopy( zBox3DAxis(&b1,k), &a[k] );
    theta = zRandF(-zPI,zPI);
    zVec3DMul( zBox3DAxis(&b1,(k+1)%3), cos(theta), &a[(k+1)%3] );
    zVec3DCatDRC( &a[(k+1)%3], sin(theta), zBox3DAxis(&b1,(k+2)%3) );
    zVec3DOuterProd( &a[k], &a[(k+1)%3], &a[(k+2)%3] );
    zVec3DCat( zBox3DCenter(&b1), 0.5*(zBox3DSpan(&b1,k)+l[k])+gap, zBox3DAxis(&b1,k), &c );
    zBox3DCreate( &b2, &c, &a[0], &a[1], &a[2], l[0], l[1], l[2] );
    if( zColChkBox3D( &b1, &b2, margin ) != expected ||
        zColChkBox3D( &b2, &b1, margin ) != expected ) result = false;
  }
  zAssert( zColChkBox3D, result );
}

void assert_colchk_box3D_diagonal(void)
{
  zBox3D b1, b2;
  zVec3D c;
  double margin, g, l[3];
  int i, k;
  bool expected, result = true;

  for( i=0; i<N; i++ ){
    box3D_rand( &b1 );
    expected = diagonal_margin_rand( &margin, &g );
    /* another box of the same attitude apart from a corner of the box */
    zVec3DCopy( zBox3DCenter(&b1), &c );
    for( k=0; k<3; k++ ){
      l[k] = zRandF(1,5);
      zVec3DCatDRC( &c, 0.5*(zBox3DSpan(&b1,k)+l[k])+g, zBox3DAxis(&b1,k) );
    }
    zBox3DCreate( &b2, &c, zBox3DAxis(&b1,zX), zBox3DAxis(&b1,zY), zBox3DAxis(&b1,zZ), l[0], l[1], l[2] );
    if( zColChkBox3D( &b1, &b2, margin ) != expected ||
        zColChkBox3D( &b2, &b1, margin ) != expected ) result = false;
  }
  zAssert( zColChkBox3D (diagonal case), result );
}

/* shapes touching each other are not in collision with a zero margin, while they are with a small
 * positive margin. */
void assert_colchk_box_touch(void)
{
  zAABox3D ab1, ab2;
  zBox3D b1, b2;
  zPlane3D plane;
  zTri3D tri;
  zSphere3D sphere;
  zVec3D v[3], c;
  int i, k;
  bool result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &ab1 );
    k = zRandI(0,2);
    aabox3D_beside( &ab1, k, 0, &ab2 );
    if( zColChkAABox3D( &ab1, &ab2, 0 ) || !zColChkAABox3D( &ab1, &ab2, zTOL ) ) result = false;
    zVec3DZero( &c );
    c.e[k] = 1;
    zPlane3DCreate( &plane, &ab1.max, &c );
    /* touching within the rounding error */
    if( zColChkPlaneAABox3D( &plane, &ab1, -zTOL ) || !zColChkPlaneAABox3D( &plane, &ab1, zTOL ) ) result = false;
    zVec3DCopy( &ab1.max, &v[0] ); /* a triangle touching a corner */
    zVec3DCreate( &v[1], ab1.max.c.x+1, ab1.max.c.y+2, ab1.max.c.z+3 );
    zVec3DCreate( &v[2], ab1.max.c.x+3, ab1.max.c.y+1, ab1.max.c.z+2 );
    zTri3DCreate( &tri, &v[0], &v[1], &v[2] );
    if( zColChkTriAABox3D( &tri, &ab1, -zTOL ) || !zColChkTriAABox3D( &tri, &ab1, zTOL ) ) result = false;
    box3D_rand( &b1 );
    zVec3DCat( zBox3DCenter(&b1), 0.5*zBox3DSpan(&b1,k)+2, zBox3DAxis(&b1,k), &c );
    zSphere3DCreate( &sphere, &c, 2, 0 );
    if( zColChkSphereBox3D( &sphere, &b1, -zTOL ) || !zColChkSphereBox3D( &sphere, &b1, zTOL ) ) result = false;
    zVec3DCat( zBox3DCenter(&b1), zBox3DSpan(&b1,k), zBox3DAxis(&b1,k), &c );
    zBox3DCreate( &b2, &c, zBox3DAxis(&b1,zX), zBox3DAxis(&b1,zY), zBox3DAxis(&b1,zZ), zBox3DDepth(&b1), zBox3DWidth(&b1), zBox3DHeight(&b1) );
    if( zColChkBox3D( &b1, &b2, -zTOL ) || !zColChkBox3D( &b1, &b2, zTOL ) ) result = false;
  }
  zAssert( zColChk...AABox3D + zColChk...Box3D (touching case), result );
}

/* the penetration depths of shapes at the centers of boxes are known. */
void assert_colchk_box_penetration(void)
{
  zAABox3D ab;
  zBox3D b;
  zPlane3D plane;
  zSphere3D sphere;
  zVec3D c, n;
  double d;
  int i, k;
  bool result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &ab );
    d = zMin3( zAABox3DDepth(&ab), zAABox3DWidth(&ab), zAABox3DHeight(&ab) );
    if( !zColChkAABox3D( &ab, &ab, -0.99*d ) || zColChkAABox3D( &ab, &ab, -1.01*d ) ) result = false;
    k = zRandI(0,2);
    zAABox3DCenter( &ab, &c );
    zVec3DZero( &n );
    n.e[k] = 1;
    zPlane3DCreate( &plane, &c, &n );
    d = 0.5 * ( ab.max.e[k] - ab.min.e[k] );
    if( !zColChkPlaneAABox3D( &plane, &ab, -0.99*d ) || zColChkPlaneAABox3D( &plane, &ab, -1.01*d ) ) result = false;
    zSphere3DCreate( &sphere, &c, 1, 0 );
    d = 0.5 * zMin3( zAABox3DDepth(&ab), zAABox3DWidth(&ab), zAABox3DHeight(&ab) ) + 1;
    if( !zColChkSphereAABox3D( &sphere, &ab, -0.99*d ) || zColChkSphereAABox3D( &sphere, &ab, -1.01*d ) ) result = false;
    box3D_rand( &b );
    d = zMin3( zBox3DDepth(&b), zBox3DWidth(&b), zBox3DHeight(&b) );
    if( !zColChkBox3D( &b, &b, -0.99*d ) || zColChkBox3D( &b, &b, -1.01*d ) ) result = false;
    zSphere3DCreate( &sphere, zBox3DCenter(&b), 1, 0 );
    d = 0.5 * d + 1;
    if( !zColChkSphereBox3D( &sphere, &b, -0.99*d ) || zColChkSphereBox3D( &sphere, &b, -1.01*d ) ) result = false;
  }
  zAssert( zColChk...AABox3D + zColChk...Box3D (penetration), result );
}

static void ph3D_box(const zAABox3D *box, zPH3D *ph)
{
  zVec3D v[8];
  zVec3DData data;
  int i;

  for( i=0; i<8; i++ ) zAABox3DVert( box, i, &v[i] );
  zVec3DDataAssignArrayDirect( &data, v, 8 );
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

void assert_intersect_ph3D_box(void)
{
  zAABox3D b1, b2, b, bans;
  zPH3D ph1, ph2;
  double margin, gap;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    aabox3D_rand( &b1 );
    gap_rand( &margin, &gap );
    aabox3D_beside( &b1, zRandI(0,2), gap, &b2 );
    ph3D_box( &b1, &ph1 );
    ph3D_box( &b2, &ph2 );
    if( ( zIntersectPH3DBox( &ph1, &ph2, &b ) != NULL ) != ( gap < 0 ) ) result = false;
    if( gap < 0 ){
      zIntersectAABox3D( &bans, &b1, &b2 );
      if( !zVec3DEqual( &b.min, &bans.min ) || !zVec3DEqual( &b.max, &bans.max ) ) result = false;
    }
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zPH3DInit( &ph1 );
  aabox3D_rand( &b1 );
  ph3D_box( &b1, &ph2 );
  if( zIntersectPH3DBox( &ph1, &ph2, &b ) || zIntersectPH3DBox( &ph2, &ph1, &b ) ) result = false;
  zPH3DDestroy( &ph2 );
  zAssert( zIntersectPH3DBox, result );
}

int main(void)
{
  zRandInit();
  assert_colchk_aabox3D();
  assert_colchk_aabox3D_diagonal();
  assert_colchk_plane_aabox3D();
  assert_colchk_tri_aabox3D();
  assert_colchk_tri_aabox3D_diagonal();
  assert_colchk_sphere_aabox3D();
  assert_colchk_sphere_box3D();
  assert_colchk_box3D();
  assert_colchk_box3D_diagonal();
  assert_colchk_box_touch();
  assert_colchk_box_penetration();
  assert_intersect_ph3D_box();
  return 0;
}

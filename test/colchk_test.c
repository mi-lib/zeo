#include <zeo/zeo_col.h>
#include <zeo/zeo_brep.h>

#define N 1000

void assert_intersect_plane(void)
{
  zPlane3D plane1, plane2;
  zVec3D p1, p2, d1, d2;
  zLine3D line;
  int i;
  bool result = true;

  for( i=0; i<N; i++ ){
    zVec3DCreate( &p1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &d1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zPlane3DCreate( &plane1, &p1, &d1 );
    zVec3DCreate( &p2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DCreate( &d2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zPlane3DCreate( &plane2, &p2, &d2 );
    zIntersectPlane3D( &plane1, &plane2, &line );
    if( !zPlane3DPointIsOn( &plane1, zLine3DOrg(&line), zTOL ) ||
        !zPlane3DPointIsOn( &plane2, zLine3DOrg(&line), zTOL ) ||
        !zIsTiny( zVec3DInnerProd( zPlane3DNorm(&plane1), zLine3DDir(&line) ) ) ||
        !zIsTiny( zVec3DInnerProd( zPlane3DNorm(&plane2), zLine3DDir(&line) ) ) ) result = false;
  }
  zAssert( zIntersectPlane3D (random case), result );

  zVec3DCreate( &p1, 0, 0, 0 );
  zVec3DCreate( &d1, 0, 0, 1 );
  zPlane3DCreate( &plane1, &p1, &d1 );
  zVec3DCreate( &p2, 1, 1, 0 );
  zVec3DCreate( &d2, 0, 0, 1 );
  zPlane3DCreate( &plane2, &p2, &d2 );
  zAssert( zIntersectPlane3D (identical case), zIntersectPlane3D( &plane1, &plane2, &line ) == -1 );
  zVec3DCreate( &p2, 1, 1, 1 );
  zPlane3DCreate( &plane2, &p2, &d2 );
  zAssert( zIntersectPlane3D (parallel case), zIntersectPlane3D( &plane1, &plane2, &line ) == 0 );
}

/* a box [lo,hi] rotated by r, translated by offset and scaled by scale. */
void create_box_ph(zPH3D *ph, zVec3D *lo, zVec3D *hi, zMat3D *r, double offset, double scale)
{
  zVec3D v[8];
  zVec3DData data;
  int i;

  for( i=0; i<8; i++ ){
    zVec3DCreate( &v[i], i & 1 ? hi->c.x : lo->c.x, i & 2 ? hi->c.y : lo->c.y, i & 4 ? hi->c.z : lo->c.z );
    zMulMat3DVec3DDRC( r, &v[i] );
    zVec3DCreate( &v[i], ( v[i].c.x + offset ) * scale, ( v[i].c.y - offset ) * scale, ( v[i].c.z + offset ) * scale );
  }
  zVec3DDataAssignArrayDirect( &data, v, 8 );
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

/* volume of a polyhedron, which is translated to the origin to avoid cancellation. */
double volume_ph(zPH3D *ph)
{
  zVec3D o;
  int i;

  zVec3DCopy( zPH3DVert(ph,0), &o );
  for( i=0; i<zPH3DVertNum(ph); i++ ) zVec3DSubDRC( zPH3DVert(ph,i), &o );
  return zPH3DVolume( ph );
}

/* intersection of a unit box and the one shifted, which are rotated, translated and scaled. The
 * volume of the intersection is the product of the overlaps along the axes. The cases of thin
 * intersections, shared face planes, identical boxes and touching boxes are tested. */
void assert_intersect_ph_box(void)
{
  zPH3D ph1, ph2, ph;
  zVec3D lo, hi, lo2, hi2, shift;
  zMat3D r;
  double scale, offset, v, v_expected;
  int i, k;
  bool result[5];

  zVec3DCreate( &lo, 0, 0, 0 );
  zVec3DCreate( &hi, 1, 1, 1 );
  for( k=0; k<5; k++ ){
    result[k] = true;
    for( i=0; i<N/10; i++ ){
      scale = pow( 10, zRandF(-6,6) );
      offset = i % 2 ? 1.0e3 : 0;
      zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
      zVec3DCreate( &shift, zRandF(-0.5,0.5), zRandF(-0.5,0.5), zRandF(-0.5,0.5) );
      switch( k ){
      case 1: shift.c.x = 1 - pow( 10, zRandF(-9,-3) ); break;
      case 2: shift.c.y = 0; break;
      case 3: zVec3DZero( &shift ); break;
      case 4: shift.c.x = 1; break;
      default: ;
      }
      zVec3DAdd( &lo, &shift, &lo2 );
      zVec3DAdd( &hi, &shift, &hi2 );
      create_box_ph( &ph1, &lo, &hi, &r, offset, scale );
      create_box_ph( &ph2, &lo2, &hi2, &r, offset, scale );
      v_expected = ( 1 - fabs( shift.c.x ) ) * ( 1 - fabs( shift.c.y ) ) * ( 1 - fabs( shift.c.z ) );
      if( !zIntersectPH3D( &ph1, &ph2, &ph ) ){
        if( k != 4 ) result[k] = false;
      } else{
        v = volume_ph( &ph ) / ( scale * scale * scale );
        /* the error is proportional to the rounding errors of the coordinates of the vertices */
        if( k == 4 || fabs( v - v_expected ) > 1.0e-12 * ( 1 + offset ) ) result[k] = false;
        zPH3DDestroy( &ph );
      }
      zPH3DDestroy( &ph1 );
      zPH3DDestroy( &ph2 );
    }
  }
  zAssert( zIntersectPH3D (boxes), result[0] );
  zAssert( zIntersectPH3D (thin intersection of boxes), result[1] );
  zAssert( zIntersectPH3D (boxes with shared face planes), result[2] );
  zAssert( zIntersectPH3D (identical boxes), result[3] );
  zAssert( zIntersectPH3D (touching boxes), result[4] );
}

/* intersection of random convex polyhedra compared with that computed by the boundary representation.
 * The tolerance of the volume is loose, since the vertices computed from three nearly coplanar planes
 * have errors up to about 1e-7 in rare cases. */
void assert_intersect_ph_random(void)
{
  zPH3D ph1, ph2, ph, ph_brep;
  zVec3D v[100];
  zVec3DData data;
  double offset;
  int i, k;
  bool result = true;

  for( k=0; k<N/10; k++ ){
    for( i=0; i<100; i++ )
      zVec3DCreatePolar( &v[i], zRandF(0.5,1), zRandF(0,zPI), zRandF(-zPI,zPI) );
    zVec3DDataAssignArrayDirect( &data, v, 100 );
    zVec3DDataConvexHull( &data, &ph1 );
    zVec3DDataDestroy( &data );
    offset = zRandF(0,0.3); /* so that the polyhedra intersect */
    for( i=0; i<100; i++ ){
      zVec3DCreatePolar( &v[i], zRandF(0.5,1), zRandF(0,zPI), zRandF(-zPI,zPI) );
      v[i].c.x += offset; v[i].c.y += offset; v[i].c.z += offset;
    }
    zVec3DDataAssignArrayDirect( &data, v, 100 );
    zVec3DDataConvexHull( &data, &ph2 );
    zVec3DDataDestroy( &data );
    if( !zIntersectPH3D( &ph1, &ph2, &ph ) || !zIntersectPH3DBREP( &ph1, &ph2, &ph_brep ) ){
      result = false;
    } else{
      if( !zIsTol( volume_ph( &ph ) - volume_ph( &ph_brep ), 1.0e-6 ) ) result = false;
      zPH3DDestroy( &ph );
      zPH3DDestroy( &ph_brep );
    }
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zAssert( zIntersectPH3D (random polyhedra), result );
}

/* a random margin and a gap that is apart from the margin, where the shapes are in collision with
 * each other if the gap is less than the margin. A negative gap means that the shapes intersect. */
bool gap_rand(double *margin, double *gap)
{
  double d;

  *margin = zRandI(0,2) == 0 ? 0 : zRandF(-0.5,0.5);
  d = zRandF(0.01,1);
  if( zRandI(0,1) == 0 ) d = -d;
  *gap = *margin + d;
  return d < 0;
}

/* a random orthonormal basis, where n is the normal vector of the plane spanned by u and w. */
void basis_rand(zVec3D *u, zVec3D *w, zVec3D *n)
{
  zVec3DCreate( n, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
  zVec3DNormalizeDRC( n, 0 );
  zVec3DOrthonormal( n, u );
  zVec3DOuterProd( n, u, w );
}

/* a point p + a*u + b*w + h*n. */
zVec3D *point_on_basis(const zVec3D *p, const zVec3D *u, double a, const zVec3D *w, double b, const zVec3D *n, double h, zVec3D *v)
{
  zVec3DCopy( p, v );
  zVec3DCatDRC( v, a, u );
  zVec3DCatDRC( v, b, w );
  return zVec3DCatDRC( v, h, n );
}

/* an edge and a triangle above a plane, the lowest points of which are at the height of the gap. The
 * highest points are high enough so that the signed distance is the gap. */
void assert_colchk_plane(void)
{
  zPlane3D plane;
  zEdge3D edge;
  zTri3D tri;
  zVec3D p, u, w, n, v[3];
  double margin, gap;
  int i, j;
  bool expected, result_edge = true, result_tri = true, result_touch = true;

  for( i=0; i<N; i++ ){
    basis_rand( &u, &w, &n );
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zPlane3DCreate( &plane, &p, &n );
    if( zRandI(0,1) == 0 ) zVec3DRevDRC( &n ); /* below the plane */
    expected = gap_rand( &margin, &gap );
    point_on_basis( &p, &u, zRandF(-5,5), &w, zRandF(-5,5), &n, gap, &v[0] );
    for( j=1; j<3; j++ )
      point_on_basis( &v[0], &u, zRandF(-5,5), &w, zRandF(-5,5), &n, zRandF(3.5,5), &v[j] );
    zEdge3DCreate( &edge, &v[0], &v[1] );
    if( zColChkEdgePlane3D( &edge, &plane, margin ) != expected ) result_edge = false;
    zEdge3DCreate( &edge, &v[1], &v[0] );
    if( zColChkEdgePlane3D( &edge, &plane, margin ) != expected ) result_edge = false;
    zTri3DCreate( &tri, &v[i%3], &v[(i+1)%3], &v[(i+2)%3] );
    if( zColChkTriPlane3D( &tri, &plane, margin ) != expected ) result_tri = false;
    /* touching within the rounding error */
    point_on_basis( &p, &u, zRandF(-5,5), &w, zRandF(-5,5), &n, 0, &v[0] );
    zEdge3DCreate( &edge, &v[0], &v[1] );
    zTri3DCreate( &tri, &v[0], &v[1], &v[2] );
    if( zColChkEdgePlane3D( &edge, &plane, -zTOL ) || !zColChkEdgePlane3D( &edge, &plane, zTOL ) ||
        zColChkTriPlane3D( &tri, &plane, -zTOL ) || !zColChkTriPlane3D( &tri, &plane, zTOL ) ) result_touch = false;
  }
  zAssert( zColChkEdgePlane3D, result_edge );
  zAssert( zColChkTriPlane3D, result_tri );
  zAssert( zColChkEdgePlane3D + zColChkTriPlane3D (touching case), result_touch );
}

/* an edge and a triangle above a triangle, the lowest vertices of which are above an inner point of
 * the latter at the height of the gap. The highest vertices are high enough and the inner point is
 * far enough from the edges of the latter so that the signed distance is the gap. */
void assert_colchk_tri(void)
{
  zEdge3D edge;
  zTri3D tri1, tri2;
  zVec3D p, u, w, n, v1[3], v2[3];
  double margin, gap;
  int i, j;
  bool expected, result_edge = true, result_tri = true, result_touch = true;

  for( i=0; i<N; i++ ){
    basis_rand( &u, &w, &n );
    zVec3DCreate( &p, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    for( j=0; j<3; j++ ) /* a triangle that includes the circle of radius 5 around p */
      point_on_basis( &p, &u, 10*cos(zPIx2*j/3), &w, 10*sin(zPIx2*j/3), &n, 0, &v1[j] );
    zTri3DCreate( &tri1, &v1[0], &v1[1], &v1[2] );
    expected = gap_rand( &margin, &gap );
    point_on_basis( &p, &u, zRandF(-1.5,1.5), &w, zRandF(-1.5,1.5), &n, gap, &v2[0] );
    point_on_basis( &v2[0], &u, 0, &w, 0, &n, zRandF(3.5,5), &v2[1] );
    zEdge3DCreate( &edge, &v2[0], &v2[1] );
    if( zColChkEdgeTri3D( &edge, &tri1, margin ) != expected ) result_edge = false;
    for( j=1; j<3; j++ )
      point_on_basis( &v2[0], &u, zRandF(-0.5,0.5), &w, zRandF(-0.5,0.5), &n, zRandF(3.5,5), &v2[j] );
    zTri3DCreate( &tri2, &v2[i%3], &v2[(i+1)%3], &v2[(i+2)%3] );
    if( zColChkTri3D( &tri1, &tri2, margin ) != expected ||
        zColChkTri3D( &tri2, &tri1, margin ) != expected ) result_tri = false;
    /* a coplanar triangle sharing an area, which has no volume, and an edge on the plane */
    for( j=0; j<3; j++ )
      point_on_basis( &p, &u, zRandF(-3,3), &w, zRandF(-3,3), &n, 0, &v2[j] );
    zTri3DCreate( &tri2, &v2[0], &v2[1], &v2[2] );
    zEdge3DCreate( &edge, &v2[0], &v2[1] );
    if( zColChkTri3D( &tri1, &tri2, -zTOL ) || !zColChkTri3D( &tri1, &tri2, zTOL ) ||
        zColChkEdgeTri3D( &edge, &tri1, -zTOL ) || !zColChkEdgeTri3D( &edge, &tri1, zTOL ) ) result_touch = false;
  }
  zAssert( zColChkEdgeTri3D, result_edge );
  zAssert( zColChkTri3D, result_tri );
  zAssert( zColChkEdgeTri3D + zColChkTri3D (coplanar case), result_touch );
}

/* boxes apart from each other along an axis or from a corner. */
void assert_colchk_ph(void)
{
  zPH3D ph1, ph2;
  zVec3D lo, hi, lo2, hi2, p1, p2;
  zMat3D r;
  double margin, gap, d;
  int i, k;
  bool expected, result = true, result_diagonal = true, result_negative = true;

  for( i=0; i<N/10; i++ ){
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI_2,zPI_2), zRandF(-zPI,zPI) );
    zVec3DCreate( &lo, zRandF(-5,-2), zRandF(-5,-2), zRandF(-5,-2) );
    zVec3DCreate( &hi, zRandF(2,5), zRandF(2,5), zRandF(2,5) );
    create_box_ph( &ph1, &lo, &hi, &r, 0, 1 );
    /* along an axis */
    expected = gap_rand( &margin, &gap );
    k = zRandI(0,2);
    zVec3DCopy( &lo, &lo2 );
    zVec3DCopy( &hi, &hi2 );
    lo2.e[k] = hi.e[k] + gap;
    hi2.e[k] = lo2.e[k] + zRandF(2,5);
    create_box_ph( &ph2, &lo2, &hi2, &r, 0, 1 );
    if( zColChkPH3D( &ph1, &ph2, margin, NULL, NULL ) != expected ) result = false;
    zPH3DDestroy( &ph2 );
    /* identical boxes, the penetration depth of which is the minimum size */
    d = zMin3( hi.c.x - lo.c.x, hi.c.y - lo.c.y, hi.c.z - lo.c.z );
    if( !zColChkPH3D( &ph1, &ph1, -0.99*d, NULL, NULL ) || zColChkPH3D( &ph1, &ph1, -1.01*d, NULL, NULL ) ) result_negative = false;
    /* from a corner */
    gap = zRandF(0.1,1);
    for( k=0; k<3; k++ ){
      lo2.e[k] = hi.e[k] + gap;
      hi2.e[k] = lo2.e[k] + zRandF(1,5);
    }
    create_box_ph( &ph2, &lo2, &hi2, &r, 0, 1 );
    d = sqrt(3) * gap;
    if( zColChkPH3D( &ph1, &ph2, 0.99*d, &p1, &p2 ) || !zColChkPH3D( &ph1, &ph2, 1.01*d, NULL, NULL ) ||
        !zEqual( zVec3DDist( &p1, &p2 ), d, zTOL*10 ) ) result_diagonal = false;
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zAssert( zColChkPH3D, result );
  zAssert( zColChkPH3D (diagonal case), result_diagonal );
  zAssert( zColChkPH3D (negative margin), result_negative );
}

/* convex polytopes for the reference of the signed distance */
typedef struct{
  int nv, ne, nn;
  zVec3D v[8];
  int e[12][2];
  zVec3D n[3]; /* normal vectors of faces */
} polytope_t;

static void polytope_edge(polytope_t *pt, const zVec3D *v0, const zVec3D *v1)
{
  pt->nv = 2; pt->ne = 1; pt->nn = 0;
  zVec3DCopy( v0, &pt->v[0] );
  zVec3DCopy( v1, &pt->v[1] );
  pt->e[0][0] = 0; pt->e[0][1] = 1;
}

static void polytope_tri(polytope_t *pt, const zVec3D *v0, const zVec3D *v1, const zVec3D *v2)
{
  zVec3D e1, e2;
  int i;

  pt->nv = 3; pt->ne = 3; pt->nn = 1;
  zVec3DCopy( v0, &pt->v[0] );
  zVec3DCopy( v1, &pt->v[1] );
  zVec3DCopy( v2, &pt->v[2] );
  for( i=0; i<3; i++ ){
    pt->e[i][0] = i; pt->e[i][1] = (i+1)%3;
  }
  zVec3DSub( v1, v0, &e1 );
  zVec3DSub( v2, v0, &e2 );
  zVec3DOuterProd( &e1, &e2, &pt->n[0] );
}

/* a box with the center c, the axes a[] and the half sizes h[]. */
static void polytope_box(polytope_t *pt, const zVec3D *c, const zVec3D a[], const double h[])
{
  int i, k, n = 0;

  pt->nv = 8; pt->ne = 12; pt->nn = 3;
  for( i=0; i<8; i++ ){
    zVec3DCopy( c, &pt->v[i] );
    for( k=0; k<3; k++ )
      zVec3DCatDRC( &pt->v[i], ( i>>k & 1 ) ? h[k] : -h[k], &a[k] );
  }
  for( k=0; k<3; k++ ){
    zVec3DCopy( &a[k], &pt->n[k] );
    for( i=0; i<8; i++ )
      if( !( i>>k & 1 ) ){
        pt->e[n][0] = i; pt->e[n][1] = i | 1<<k; n++;
      }
  }
}

/* separation of two polytopes along an axis, which is negative if their projections overlap. */
static double polytope_sep(const polytope_t *pt1, const polytope_t *pt2, const zVec3D *a)
{
  zVec3D u;
  double p, min1 = HUGE_VAL, max1 = -HUGE_VAL, min2 = HUGE_VAL, max2 = -HUGE_VAL;
  int i;

  if( zVec3DNorm( a ) < 1.0e-10 ) return -HUGE_VAL;
  zVec3DNormalize( a, 0, &u );
  for( i=0; i<pt1->nv; i++ ){
    p = zVec3DInnerProd( &u, &pt1->v[i] );
    if( p < min1 ) min1 = p;
    if( p > max1 ) max1 = p;
  }
  for( i=0; i<pt2->nv; i++ ){
    p = zVec3DInnerProd( &u, &pt2->v[i] );
    if( p < min2 ) min2 = p;
    if( p > max2 ) max2 = p;
  }
  return zMax( min2 - max1, min1 - max2 );
}

/* the maximum separation along the directions between all pairs of features of two polytopes,
 * which include the direction of the shortest translation to separate or to contact them. */
static double polytope_sep_feature(const polytope_t *pt1, const polytope_t *pt2)
{
  zVec3D a, d, w;
  double s, smax = -HUGE_VAL;
  int i, j;

  for( i=0; i<pt1->nv; i++ ) /* vertex-vertex */
    for( j=0; j<pt2->nv; j++ ){
      zVec3DSub( &pt2->v[j], &pt1->v[i], &a );
      if( ( s = polytope_sep( pt1, pt2, &a ) ) > smax ) smax = s;
    }
  for( i=0; i<pt1->ne; i++ ){ /* edge-vertex */
    zVec3DSub( &pt1->v[pt1->e[i][1]], &pt1->v[pt1->e[i][0]], &d );
    for( j=0; j<pt2->nv; j++ ){
      zVec3DSub( &pt2->v[j], &pt1->v[pt1->e[i][0]], &w );
      zVec3DOuterProd( &d, &w, &a );
      zVec3DOuterProd( &a, &d, &a );
      if( ( s = polytope_sep( pt1, pt2, &a ) ) > smax ) smax = s;
    }
    for( j=0; j<pt2->ne; j++ ){ /* edge-edge */
      zVec3DSub( &pt2->v[pt2->e[j][1]], &pt2->v[pt2->e[j][0]], &w );
      zVec3DOuterProd( &d, &w, &a );
      if( ( s = polytope_sep( pt1, pt2, &a ) ) > smax ) smax = s;
    }
    for( j=0; j<pt1->nn; j++ ){ /* in-plane normals of flat polytopes */
      zVec3DOuterProd( &pt1->n[j], &d, &a );
      if( ( s = polytope_sep( pt1, pt2, &a ) ) > smax ) smax = s;
    }
    for( j=0; j<pt2->nn; j++ ){
      zVec3DOuterProd( &pt2->n[j], &d, &a );
      if( ( s = polytope_sep( pt1, pt2, &a ) ) > smax ) smax = s;
    }
  }
  for( i=0; i<pt1->nn; i++ ) /* face normal */
    if( ( s = polytope_sep( pt1, pt2, &pt1->n[i] ) ) > smax ) smax = s;
  return smax;
}

/* the signed distance between two convex polytopes. */
static double polytope_signed_dist(const polytope_t *pt1, const polytope_t *pt2)
{
  return zMax( polytope_sep_feature( pt1, pt2 ), polytope_sep_feature( pt2, pt1 ) );
}

static void vec3D_rand(zVec3D *v, double r)
{
  zVec3DCreate( v, zRandF(-r,r), zRandF(-r,r), zRandF(-r,r) );
}

static void attitude_rand(zVec3D a[])
{
  zMat3D m;

  zMat3DFromZYX( &m, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zVec3DCopy( &m.v[0], &a[0] );
  zVec3DCopy( &m.v[1], &a[1] );
  zVec3DCopy( &m.v[2], &a[2] );
}

#define SD_TOL 1.0e-6
#define CHECK_COLCHK(func,s1,s2,d) ( func( s1, s2, (d) + SD_TOL ) && !func( s1, s2, (d) - SD_TOL ) )

/* random pairs of shapes are checked with margins slightly larger and smaller than the signed
 * distance between them computed from all pairs of features. */
void assert_colchk_signed_dist(void)
{
  polytope_t pt1, pt2;
  zEdge3D edge;
  zTri3D tri1, tri2;
  zAABox3D ab1, ab2;
  zBox3D b1, b2;
  zPH3D ph1, ph2;
  zVec3DData data;
  zVec3D v[6], c1, c2, a1[3], a2[3], xyz[3];
  double h1[3], h2[3], d;
  int i, k;
  bool result_edgetri = true, result_tri = true, result_triaabox = true, result_aabox = true, result_box = true, result_ph = true;

  zVec3DCopy( ZVEC3DX, &xyz[0] );
  zVec3DCopy( ZVEC3DY, &xyz[1] );
  zVec3DCopy( ZVEC3DZ, &xyz[2] );
  for( i=0; i<N; i++ ){
    for( k=0; k<6; k++ ) vec3D_rand( &v[k], 2 );
    zVec3DCatDRC( &v[3], 1, ZVEC3DX ); /* shifted so that they are apart in some cases */
    zVec3DCatDRC( &v[4], 1, ZVEC3DX );
    zVec3DCatDRC( &v[5], 1, ZVEC3DX );
    /* edge vs triangle */
    zEdge3DCreate( &edge, &v[3], &v[4] );
    zTri3DCreate( &tri1, &v[0], &v[1], &v[2] );
    polytope_edge( &pt1, &v[3], &v[4] );
    polytope_tri( &pt2, &v[0], &v[1], &v[2] );
    d = polytope_signed_dist( &pt1, &pt2 );
    if( !CHECK_COLCHK( zColChkEdgeTri3D, &edge, &tri1, d ) ) result_edgetri = false;
    /* triangle vs triangle */
    zTri3DCreate( &tri2, &v[3], &v[4], &v[5] );
    polytope_tri( &pt1, &v[3], &v[4], &v[5] );
    d = polytope_signed_dist( &pt1, &pt2 );
    if( !CHECK_COLCHK( zColChkTri3D, &tri1, &tri2, d ) ) result_tri = false;
    /* triangle vs axis-aligned box */
    vec3D_rand( &c1, 1 );
    for( k=0; k<3; k++ ) h1[k] = zRandF(0.5,2);
    zAABox3DCreate( &ab1, c1.c.x-h1[0], c1.c.y-h1[1], c1.c.z-h1[2], c1.c.x+h1[0], c1.c.y+h1[1], c1.c.z+h1[2] );
    polytope_box( &pt2, &c1, xyz, h1 );
    d = polytope_signed_dist( &pt1, &pt2 );
    if( !CHECK_COLCHK( zColChkTriAABox3D, &tri2, &ab1, d ) ) result_triaabox = false;
    /* axis-aligned boxes */
    vec3D_rand( &c2, 3 );
    for( k=0; k<3; k++ ) h2[k] = zRandF(0.5,2);
    zAABox3DCreate( &ab2, c2.c.x-h2[0], c2.c.y-h2[1], c2.c.z-h2[2], c2.c.x+h2[0], c2.c.y+h2[1], c2.c.z+h2[2] );
    polytope_box( &pt1, &c2, xyz, h2 );
    d = polytope_signed_dist( &pt1, &pt2 );
    if( !CHECK_COLCHK( zColChkAABox3D, &ab1, &ab2, d ) ) result_aabox = false;
    /* oriented boxes */
    attitude_rand( a1 );
    attitude_rand( a2 );
    zBox3DCreate( &b1, &c1, &a1[0], &a1[1], &a1[2], 2*h1[0], 2*h1[1], 2*h1[2] );
    zBox3DCreate( &b2, &c2, &a2[0], &a2[1], &a2[2], 2*h2[0], 2*h2[1], 2*h2[2] );
    polytope_box( &pt1, &c1, a1, h1 );
    polytope_box( &pt2, &c2, a2, h2 );
    d = polytope_signed_dist( &pt1, &pt2 );
    if( !CHECK_COLCHK( zColChkBox3D, &b1, &b2, d ) ) result_box = false;
    /* polyhedra */
    if( i % 10 == 0 ){
      zVec3DDataAssignArrayDirect( &data, pt1.v, 8 );
      zVec3DDataConvexHull( &data, &ph1 );
      zVec3DDataDestroy( &data );
      zVec3DDataAssignArrayDirect( &data, pt2.v, 8 );
      zVec3DDataConvexHull( &data, &ph2 );
      zVec3DDataDestroy( &data );
      if( !zColChkPH3D( &ph1, &ph2, d + SD_TOL, NULL, NULL ) || zColChkPH3D( &ph1, &ph2, d - SD_TOL, NULL, NULL ) ) result_ph = false;
      if( fabs( d ) > SD_TOL && zColChkPH3D( &ph1, &ph2, 0, NULL, NULL ) != ( d < 0 ) ) result_ph = false;
      zPH3DDestroy( &ph1 );
      zPH3DDestroy( &ph2 );
    }
  }
  zAssert( zColChkEdgeTri3D (signed distance), result_edgetri );
  zAssert( zColChkTri3D (signed distance), result_tri );
  zAssert( zColChkTriAABox3D (signed distance), result_triaabox );
  zAssert( zColChkAABox3D (signed distance), result_aabox );
  zAssert( zColChkBox3D (signed distance), result_box );
  zAssert( zColChkPH3D (signed distance), result_ph );
}

int main(void)
{
  zRandInit();
  assert_intersect_plane();
  assert_colchk_plane();
  assert_colchk_tri();
  assert_colchk_ph();
  assert_colchk_signed_dist();
  assert_intersect_ph_box();
  assert_intersect_ph_random();
  return 0;
}

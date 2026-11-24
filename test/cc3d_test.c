#include <zeo/zeo.h>

#define N 100

/* volume of a polyhedron, which is translated to the origin to avoid cancellation. */
double volume_ph(zPH3D *ph)
{
  zVec3D o;
  int i;

  zVec3DCopy( zPH3DVert(ph,0), &o );
  for( i=0; i<zPH3DVertNum(ph); i++ ) zVec3DSubDRC( zPH3DVert(ph,i), &o );
  return zPH3DVolume( ph );
}

/* Euler characteristic of a closed triangulated polyhedron. */
int ph_euler_characteristic(zPH3D *ph)
{
  return zPH3DVertNum(ph) - 3 * zPH3DFaceNum(ph) / 2 + zPH3DFaceNum(ph);
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

/* a cell complex whose universe includes two polyhedra, and the solid of which is the first one. */
zCC3D *create_cc(zCC3D *cc, zPH3D *ph1, zPH3D *ph2)
{
  zAABox3D b1, b2, box;
  double d;

  zPH3DAABB( ph1, &b1 );
  zPH3DAABB( ph2, &b2 );
  zVec3DCreate( &box.min, zMin(b1.min.c.x,b2.min.c.x), zMin(b1.min.c.y,b2.min.c.y), zMin(b1.min.c.z,b2.min.c.z) );
  zVec3DCreate( &box.max, zMax(b1.max.c.x,b2.max.c.x), zMax(b1.max.c.y,b2.max.c.y), zMax(b1.max.c.z,b2.max.c.z) );
  d = zMax3( box.max.c.x - box.min.c.x, box.max.c.y - box.min.c.y, box.max.c.z - box.min.c.z );
  box.min.c.x -= d; box.min.c.y -= d; box.min.c.z -= d;
  box.max.c.x += d; box.max.c.y += d; box.max.c.z += d;
  if( !zCC3DCreate( cc, &box ) ) return NULL;
  if( !zCC3DMerge( cc, ph1 ) ){
    zCC3DDestroy( cc );
    return NULL;
  }
  return cc;
}

/* check the result of an operation: the cell complex is consistent, the boundary is closed, and
 * the volume is as expected. */
bool check_result(zCC3D *cc, double v_expected, double tol)
{
  zPH3D ph;
  bool ret = true;

  if( !zCC3DCheck( cc ) ) return false;
  if( !zCC3DToPH3D( cc, &ph ) ) return zIsTol( v_expected, tol );
  if( !zPH3DIsClosed( &ph ) || !zIsTol( volume_ph( &ph ) - v_expected, tol ) ) ret = false;
  zPH3DDestroy( &ph );
  return ret;
}

void assert_cc3d_create(void)
{
  zCC3D cc;
  zPH3D ph;
  zAABox3D box;
  zVec3D lo, hi;
  zMat3D r;

  zAABox3DCreate( &box, -1, -2, -3, 1, 2, 3 );
  zCC3DCreate( &cc, &box );
  zAssert( zCC3DCreate, zCC3DCellNum(&cc) == 1 && !zCC3DCellIsSolid(&cc,0) && zCC3DCheck( &cc ) && !zCC3DToPH3D( &cc, &ph ) );
  zCC3DDestroy( &cc );

  zVec3DCreate( &lo, 0, 0, 0 );
  zVec3DCreate( &hi, 1, 2, 3 );
  zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  create_box_ph( &ph, &lo, &hi, &r, 0, 1 );
  zCC3DFromPH3D( &cc, &ph );
  zAssert( zCC3DFromPH3D + zCC3DToPH3D, check_result( &cc, 6, 1.0e-12 ) );
  zCC3DDestroy( &cc );
  zPH3DDestroy( &ph );
}

/* cut of a unit box rotated by r by a plane perpendicular to the rotated x-axis. */
void assert_cc3d_cut(void)
{
  zCC3D cc;
  zPH3D ph;
  zVec3D lo, hi, p, n;
  zMat3D r;
  zPlane3D plane;
  double x, scale;
  int i;
  bool result[3] = { true, true, true };

  zVec3DCreate( &lo, 0, 0, 0 );
  zVec3DCreate( &hi, 1, 1, 1 );
  for( i=0; i<N; i++ ){
    scale = pow( 10, zRandF(-6,6) );
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    create_box_ph( &ph, &lo, &hi, &r, 0, scale );
    zCC3DFromPH3D( &cc, &ph );
    x = zRandF(0.01,0.99);
    zVec3DCreate( &p, x * scale, 0, 0 );
    zMulMat3DVec3DDRC( &r, &p );
    zVec3DCopy( &r.v[0], &n );
    zPlane3DCreate( &plane, &p, &n );
    if( !zCC3DCutByPlane3D( &cc, &plane ) || !check_result( &cc, x * scale*scale*scale, 1.0e-12 * scale*scale*scale ) ) result[0] = false;
    zVec3DMul( &r.v[0], -2 * scale, &p );
    zPlane3DCreate( &plane, &p, &n );
    if( !zCC3DCutByPlane3D( &cc, &plane ) || check_result( &cc, 0, 0 ) == false ) result[2] = false;
    zCC3DDestroy( &cc );
    /* a plane apart from the box */
    zCC3DFromPH3D( &cc, &ph );
    zVec3DMul( &r.v[0], 2 * scale, &p );
    zPlane3DCreate( &plane, &p, &n );
    if( !zCC3DCutByPlane3D( &cc, &plane ) || !check_result( &cc, scale*scale*scale, 1.0e-12 * scale*scale*scale ) ) result[1] = false;
    zCC3DDestroy( &cc );
    zPH3DDestroy( &ph );
  }
  zAssert( zCC3DCutByPlane3D (box), result[0] );
  zAssert( zCC3DCutByPlane3D (plane apart from the solid), result[1] );
  zAssert( zCC3DCutByPlane3D (plane removing the solid), result[2] );
}

/* operations on a unit box and the one shifted, which are rotated, translated and scaled. The
 * volume of the intersection is the product of the overlaps along the axes. The cases of thin
 * intersections, shared face planes, identical boxes and touching boxes are tested. */
void assert_cc3d_box(void)
{
  zCC3D cc;
  zPH3D ph1, ph2;
  zVec3D lo, hi, lo2, hi2, shift;
  zMat3D r;
  double scale, offset, vi, v3, tol;
  int i, k;
  bool result[5][3];

  zVec3DCreate( &lo, 0, 0, 0 );
  zVec3DCreate( &hi, 1, 1, 1 );
  for( k=0; k<5; k++ ){
    result[k][0] = result[k][1] = result[k][2] = true;
    for( i=0; i<N; i++ ){
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
      vi = ( 1 - fabs( shift.c.x ) ) * ( 1 - fabs( shift.c.y ) ) * ( 1 - fabs( shift.c.z ) );
      v3 = scale * scale * scale;
      /* the error is proportional to the rounding errors of the coordinates of the vertices */
      tol = 1.0e-9 * ( 1 + offset ) * v3;
      if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DIntersect( &cc, &ph2 ) || !check_result( &cc, vi * v3, tol ) ) result[k][0] = false;
      zCC3DDestroy( &cc );
      if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DSubtract( &cc, &ph2 ) || !check_result( &cc, ( 1 - vi ) * v3, tol ) ) result[k][1] = false;
      zCC3DDestroy( &cc );
      if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DMerge( &cc, &ph2 ) || !check_result( &cc, ( 2 - vi ) * v3, tol ) ) result[k][2] = false;
      zCC3DDestroy( &cc );
      zPH3DDestroy( &ph1 );
      zPH3DDestroy( &ph2 );
    }
  }
  zAssert( zCC3DIntersect (boxes), result[0][0] );
  zAssert( zCC3DSubtract (boxes), result[0][1] );
  zAssert( zCC3DMerge (boxes), result[0][2] );
  zAssert( zCC3DIntersect (thin intersection of boxes), result[1][0] );
  zAssert( zCC3DSubtract (thin intersection of boxes), result[1][1] );
  zAssert( zCC3DMerge (thin intersection of boxes), result[1][2] );
  zAssert( zCC3DIntersect (boxes with shared face planes), result[2][0] );
  zAssert( zCC3DSubtract (boxes with shared face planes), result[2][1] );
  zAssert( zCC3DMerge (boxes with shared face planes), result[2][2] );
  zAssert( zCC3DIntersect (identical boxes), result[3][0] );
  zAssert( zCC3DSubtract (identical boxes), result[3][1] );
  zAssert( zCC3DMerge (identical boxes), result[3][2] );
  zAssert( zCC3DIntersect (touching boxes), result[4][0] );
  zAssert( zCC3DSubtract (touching boxes), result[4][1] );
  zAssert( zCC3DMerge (touching boxes), result[4][2] );
}

/* a convex hull of random points. */
void create_random_ph(zPH3D *ph, double offset, double scale)
{
  zVec3D v[100];
  zVec3DData data;
  int i;

  for( i=0; i<100; i++ ){
    zVec3DCreatePolar( &v[i], zRandF(0.5,1), zRandF(0,zPI), zRandF(-zPI,zPI) );
    v[i].c.x += offset; v[i].c.y += offset; v[i].c.z += offset;
    zVec3DMulDRC( &v[i], scale );
  }
  zVec3DDataAssignArrayDirect( &data, v, 100 );
  zVec3DDataConvexHull( &data, ph );
  zVec3DDataDestroy( &data );
}

/* operations on random convex polyhedra compared with the intersection computed by zIntersectPH3D.
 * The tolerance of the volume is loose for the same reason with the test of zIntersectPH3D. */
void assert_cc3d_random(void)
{
  zCC3D cc;
  zPH3D ph1, ph2, ph;
  double scale, v1, v2, vi, v3;
  int i;
  bool result[3] = { true, true, true };

  for( i=0; i<N/2; i++ ){
    scale = pow( 10, zRandF(-4,4) );
    create_random_ph( &ph1, 0, scale );
    create_random_ph( &ph2, zRandF(0,0.8), scale );
    v1 = volume_ph( &ph1 );
    v2 = volume_ph( &ph2 );
    if( zIntersectPH3D( &ph1, &ph2, &ph ) ){
      vi = volume_ph( &ph );
      zPH3DDestroy( &ph );
    } else
      vi = 0;
    v3 = scale * scale * scale;
    if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DIntersect( &cc, &ph2 ) || !check_result( &cc, vi, 1.0e-6 * v3 ) ) result[0] = false;
    zCC3DDestroy( &cc );
    if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DSubtract( &cc, &ph2 ) || !check_result( &cc, v1 - vi, 1.0e-6 * v3 ) ) result[1] = false;
    zCC3DDestroy( &cc );
    if( !create_cc( &cc, &ph1, &ph2 ) || !zCC3DMerge( &cc, &ph2 ) || !check_result( &cc, v1 + v2 - vi, 1.0e-6 * v3 ) ) result[2] = false;
    zCC3DDestroy( &cc );
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zAssert( zCC3DIntersect (random polyhedra), result[0] );
  zAssert( zCC3DSubtract (random polyhedra), result[1] );
  zAssert( zCC3DMerge (random polyhedra), result[2] );
}

/* subtraction that changes the topology: a rod through a box makes a torus, and a slab across a
 * box separates it into two components. */
void assert_cc3d_topology(void)
{
  zCC3D cc;
  zPH3D box, rod, slab, ph;
  zVec3D lo, hi;
  zMat3D r;
  double w;
  bool result[2] = { true, true };
  int i;

  for( i=0; i<N/10; i++ ){
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zVec3DCreate( &lo, 0, 0, 0 );
    zVec3DCreate( &hi, 1, 1, 1 );
    create_box_ph( &box, &lo, &hi, &r, 0, 1 );
    w = zRandF(0.1,0.8);
    zVec3DCreate( &lo, 0.5-0.5*w, 0.5-0.5*w, -1 );
    zVec3DCreate( &hi, 0.5+0.5*w, 0.5+0.5*w, 2 );
    create_box_ph( &rod, &lo, &hi, &r, 0, 1 );
    zVec3DCreate( &lo, -1, -1, 0.5-0.5*w );
    zVec3DCreate( &hi, 2, 2, 0.5+0.5*w );
    create_box_ph( &slab, &lo, &hi, &r, 0, 1 );

    create_cc( &cc, &box, &rod );
    if( !zCC3DSubtract( &cc, &rod ) || !check_result( &cc, 1 - w*w, 1.0e-12 ) ) result[0] = false;
    if( zCC3DToPH3D( &cc, &ph ) ){
      if( ph_euler_characteristic( &ph ) != 0 ) result[0] = false;
      zPH3DDestroy( &ph );
    } else
      result[0] = false;
    zCC3DDestroy( &cc );

    create_cc( &cc, &box, &slab );
    if( !zCC3DSubtract( &cc, &slab ) || !check_result( &cc, 1 - w, 1.0e-12 ) ) result[1] = false;
    if( zCC3DToPH3D( &cc, &ph ) ){
      if( ph_euler_characteristic( &ph ) != 4 ) result[1] = false;
      zPH3DDestroy( &ph );
    } else
      result[1] = false;
    zCC3DDestroy( &cc );

    zPH3DDestroy( &box );
    zPH3DDestroy( &rod );
    zPH3DDestroy( &slab );
  }
  zAssert( zCC3DSubtract (box with a hole), result[0] );
  zAssert( zCC3DSubtract (box separated into two), result[1] );
}

/* merge of polyhedra out of the universe, which is enlarged. The result is compared with that in a
 * universe large enough from the beginning. */
void assert_cc3d_enlarge(void)
{
  zCC3D cc, ref;
  zPH3D ph0, ph[4], ph_out;
  zAABox3D big;
  double scale, v, vr;
  int i, j, m;
  bool result = true;

  for( i=0; i<N/2; i++ ){
    scale = pow( 10, zRandF(-4,4) );
    create_random_ph( &ph0, 0, scale );
    zAABox3DCreate( &big, -20*scale, -20*scale, -20*scale, 20*scale, 20*scale, 20*scale );
    zCC3DFromPH3D( &cc, &ph0 );
    zCC3DCreate( &ref, &big );
    zCC3DMerge( &ref, &ph0 );
    m = 1 + i % 4;
    for( j=0; j<m; j++ ){
      create_random_ph( &ph[j], zRandF(-6,6), scale );
      if( !zCC3DMerge( &cc, &ph[j] ) || !zCC3DCheck( &cc ) ) result = false;
      zCC3DMerge( &ref, &ph[j] );
    }
    if( zCC3DToPH3D( &ref, &ph_out ) ){
      vr = volume_ph( &ph_out );
      zPH3DDestroy( &ph_out );
    } else
      vr = 0;
    if( zCC3DToPH3D( &cc, &ph_out ) ){
      v = volume_ph( &ph_out );
      if( !zPH3DIsClosed( &ph_out ) ) result = false;
      zPH3DDestroy( &ph_out );
    } else
      v = 0;
    if( !zIsTol( v - vr, 1.0e-9 * scale*scale*scale ) ) result = false;
    zCC3DDestroy( &cc );
    zCC3DDestroy( &ref );
    zPH3DDestroy( &ph0 );
    for( j=0; j<m; j++ ) zPH3DDestroy( &ph[j] );
  }
  zAssert( zCC3DMerge (enlargement of the universe), result );
}

/* union of unit cubes in a 3x3x3 grid occupied according to bits of occ, which are shifted by
 * offset/4, rotated by r and scaled by scale. The result is a non-convex polyhedron in general. */
bool create_grid_ph(zPH3D *ph, int occ, int offset[], zMat3D *r, double scale)
{
  zCC3D cc;
  zPH3D cube;
  zVec3D lo, hi;
  zAABox3D box;
  int i;
  bool ret;

  zAABox3DCreate( &box, -10*scale, -10*scale, -10*scale, 10*scale, 10*scale, 10*scale );
  zCC3DCreate( &cc, &box );
  for( i=0; i<27; i++ ){
    if( !( occ & 1 << i ) ) continue;
    zVec3DCreate( &lo, i%3 + 0.25*offset[0], (i/3)%3 + 0.25*offset[1], i/9 + 0.25*offset[2] );
    zVec3DCreate( &hi, lo.c.x + 1, lo.c.y + 1, lo.c.z + 1 );
    create_box_ph( &cube, &lo, &hi, r, 0, scale );
    zCC3DMerge( &cc, &cube );
    zPH3DDestroy( &cube );
  }
  ret = zCC3DToPH3D( &cc, ph ) != NULL;
  zCC3DDestroy( &cc );
  return ret;
}

/* check if a point of a fine grid with the interval 1/4 is in the union of cubes. */
bool grid_is_inside(int occ, int offset[], int x, int y, int z)
{
  x -= offset[0]; y -= offset[1]; z -= offset[2];
  if( x < 0 || y < 0 || z < 0 || x >= 12 || y >= 12 || z >= 12 ) return false;
  return ( occ & 1 << ( x/4 + 3*( y/4 + 3*( z/4 ) ) ) ) != 0;
}

/* volumes of the intersection, the difference and the union of two unions of cubes. */
void grid_volume(int occ1, int offset1[], int occ2, int offset2[], double v[])
{
  int x, y, z;
  bool in1, in2;

  v[0] = v[1] = v[2] = 0;
  for( x=0; x<16; x++ )
    for( y=0; y<16; y++ )
      for( z=0; z<16; z++ ){
        in1 = grid_is_inside( occ1, offset1, x, y, z );
        in2 = grid_is_inside( occ2, offset2, x, y, z );
        if( in1 && in2 ) v[0] += 1.0/64;
        if( in1 && !in2 ) v[1] += 1.0/64;
        if( in1 || in2 ) v[2] += 1.0/64;
      }
}

/* conversion of non-convex polyhedra, and operations on them. Unions of cubes in a grid are compared
 * with the volumes computed on a fine grid. They include edges shared by four faces, along which two
 * cubes touch each other. */
void assert_cc3d_nonconvex(void)
{
  zCC3D cc;
  zPH3D ph1, ph2;
  zMat3D r;
  int i, k, op, occ1, occ2, offset1[3] = { 0, 0, 0 }, offset2[3];
  double scale, v[3];
  bool result[4] = { true, true, true, true };

  for( i=0; i<N/2; i++ ){
    scale = pow( 10, zRandF(-3,3) );
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    occ1 = zRandI( 1, ( 1 << 27 ) - 1 );
    occ2 = zRandI( 1, ( 1 << 27 ) - 1 );
    for( k=0; k<3; k++ ) offset2[k] = i % 2 ? zRandI(0,3) : 0;
    if( !create_grid_ph( &ph1, occ1, offset1, &r, scale ) ||
        !create_grid_ph( &ph2, occ2, offset2, &r, scale ) ) continue;
    grid_volume( occ1, offset1, occ2, offset2, v );
    if( !zCC3DFromPH3D( &cc, &ph1 ) || !check_result( &cc, ( v[1] + v[0] ) * scale*scale*scale, 1.0e-9*scale*scale*scale ) ) result[0] = false;
    zCC3DDestroy( &cc );
    for( op=0; op<3; op++ ){
      if( !create_cc( &cc, &ph1, &ph2 ) ||
          !( op == 0 ? zCC3DIntersect( &cc, &ph2 ) : op == 1 ? zCC3DSubtract( &cc, &ph2 ) : zCC3DMerge( &cc, &ph2 ) ) ||
          !check_result( &cc, v[op] * scale*scale*scale, 1.0e-9*scale*scale*scale ) ) result[op+1] = false;
      zCC3DDestroy( &cc );
    }
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zAssert( zCC3DFromPH3D (non-convex polyhedra), result[0] );
  zAssert( zCC3DIntersect (non-convex polyhedra), result[1] );
  zAssert( zCC3DSubtract (non-convex polyhedra), result[2] );
  zAssert( zCC3DMerge (non-convex polyhedra), result[3] );
}

/* polyhedra not to be accepted: an open one and one with faces oriented inward. */
void assert_cc3d_invalid(void)
{
  zCC3D cc;
  zPH3D ph;
  zVec3D lo, hi;
  zMat3D r;
  bool result;

  zVec3DCreate( &lo, 0, 0, 0 );
  zVec3DCreate( &hi, 1, 1, 1 );
  zMat3DIdent( &r );
  create_box_ph( &ph, &lo, &hi, &r, 0, 1 );
  zPH3DFlipDRC( &ph );
  result = !zCC3DFromPH3D( &cc, &ph );
  zPH3DFlipDRC( &ph );
  zPH3DSetFaceNum( &ph, zPH3DFaceNum(&ph) - 1 );
  result = result && !zCC3DFromPH3D( &cc, &ph );
  zPH3DSetFaceNum( &ph, zPH3DFaceNum(&ph) + 1 );
  zPH3DDestroy( &ph );
  zAssert( zCC3DFromPH3D (invalid polyhedra), result );
}

/* area of a polyhedron. */
double area_ph(zPH3D *ph)
{
  double area = 0;
  int i;

  for( i=0; i<zPH3DFaceNum(ph); i++ ) area += zTri3DArea( zPH3DFace(ph,i) );
  return area;
}

/* cut of the surface of a unit cube by a box which removes a half of it, a rod through it, a box
 * apart from it and a box including it. */
void assert_cc3d_cut_by_ph3d(void)
{
  zCC3D cc;
  zPH3D cube, box, cut;
  zVec3D lo, hi;
  zMat3D r;
  double w;
  bool result[4] = { true, true, true, true };
  int i;

  for( i=0; i<N/10; i++ ){
    zMat3DFromZYX( &r, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
    zVec3DCreate( &lo, 0, 0, 0 );
    zVec3DCreate( &hi, 1, 1, 1 );
    create_box_ph( &cube, &lo, &hi, &r, 0, 1 );
    w = zRandF(0.1,0.9);
    /* a half */
    zVec3DCreate( &lo, w, -1, -1 );
    zVec3DCreate( &hi, 2, 2, 2 );
    create_box_ph( &box, &lo, &hi, &r, 0, 1 );
    zCC3DFromPH3D( &cc, &cube );
    if( !zCC3DCut( &cc, &box, &cut ) ) result[0] = false;
    else{
      if( zPH3DIsClosed( &cut ) || !zIsTol( area_ph( &cut ) - ( 1 + 4*w ), 1.0e-10 ) ) result[0] = false;
      zPH3DDestroy( &cut );
    }
    if( !check_result( &cc, w, 1.0e-12 ) ) result[0] = false;
    zCC3DDestroy( &cc );
    zPH3DDestroy( &box );
    /* a rod */
    zVec3DCreate( &lo, 0.5-0.5*w, 0.5-0.5*w, -1 );
    zVec3DCreate( &hi, 0.5+0.5*w, 0.5+0.5*w, 2 );
    create_box_ph( &box, &lo, &hi, &r, 0, 1 );
    zCC3DFromPH3D( &cc, &cube );
    if( !zCC3DCut( &cc, &box, &cut ) ) result[1] = false;
    else{
      if( zPH3DIsClosed( &cut ) || !zIsTol( area_ph( &cut ) - ( 6 - 2*w*w ), 1.0e-10 ) ) result[1] = false;
      zPH3DDestroy( &cut );
    }
    zCC3DDestroy( &cc );
    zPH3DDestroy( &box );
    /* a box apart from the cube */
    zVec3DCreate( &lo, 2, 2, 2 );
    zVec3DCreate( &hi, 3, 3, 3 );
    create_box_ph( &box, &lo, &hi, &r, 0, 1 );
    zCC3DFromPH3D( &cc, &cube );
    if( !zCC3DCut( &cc, &box, &cut ) ) result[2] = false;
    else{
      if( !zPH3DIsClosed( &cut ) || !zIsTol( area_ph( &cut ) - 6, 1.0e-10 ) ) result[2] = false;
      zPH3DDestroy( &cut );
    }
    zCC3DDestroy( &cc );
    zPH3DDestroy( &box );
    /* a box including the cube */
    zVec3DCreate( &lo, -1, -1, -1 );
    zVec3DCreate( &hi, 2, 2, 2 );
    create_box_ph( &box, &lo, &hi, &r, 0, 1 );
    zCC3DFromPH3D( &cc, &cube );
    if( zCC3DCut( &cc, &box, &cut ) ) result[3] = false;
    zCC3DDestroy( &cc );
    zPH3DDestroy( &box );
    zPH3DDestroy( &cube );
  }
  zAssert( zCC3DCut (half of a cube), result[0] );
  zAssert( zCC3DCut (rod through a cube), result[1] );
  zAssert( zCC3DCut (box apart from a cube), result[2] );
  zAssert( zCC3DCut (box including a cube), result[3] );
}

int main(void)
{
  zRandInit();
  assert_cc3d_create();
  assert_cc3d_cut();
  assert_cc3d_box();
  assert_cc3d_random();
  assert_cc3d_topology();
  assert_cc3d_enlarge();
  assert_cc3d_nonconvex();
  assert_cc3d_invalid();
  assert_cc3d_cut_by_ph3d();
  return 0;
}

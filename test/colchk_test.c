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

/* intersection of random convex polyhedra compared with that computed by the boundary representation. */
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
      if( !zIsTol( volume_ph( &ph ) - volume_ph( &ph_brep ), 1.0e-9 ) ) result = false;
      zPH3DDestroy( &ph );
      zPH3DDestroy( &ph_brep );
    }
    zPH3DDestroy( &ph1 );
    zPH3DDestroy( &ph2 );
  }
  zAssert( zIntersectPH3D (random polyhedra), result );
}

int main(void)
{
  zRandInit();
  assert_intersect_plane();
  assert_intersect_ph_box();
  assert_intersect_ph_random();
  return 0;
}

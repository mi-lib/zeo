#include <zeo/zeo_elem3d.h>

#define ZEO_WARN_ELEM_AABOX_NEGATIVE_MAGNITUDE "negative magnitude assigned for expansion, reversed"

zAABox3D *zAABox3DCreateFromSize(zAABox3D *box, zVec3D *center, double depth, double width, double height)
{
  zAABox3DXMin(box) = center->c.x - 0.5 * fabs(depth);
  zAABox3DYMin(box) = center->c.y - 0.5 * fabs(width);
  zAABox3DZMin(box) = center->c.z - 0.5 * fabs(height);
  zAABox3DXMax(box) = center->c.x + 0.5 * fabs(depth);
  zAABox3DYMax(box) = center->c.y + 0.5 * fabs(width);
  zAABox3DZMax(box) = center->c.z + 0.5 * fabs(height);
  return box;
}

bool zAABox3DEqual(const zAABox3D *box1, const zAABox3D *box2)
{
  return zVec3DEqual( &box1->min, &box2->min ) && zVec3DEqual( &box1->max, &box2->max );
}

zAABox3D *zAABox3DExpand(const zAABox3D *src, double magnitude, zAABox3D *dest)
{
  zVec3D center;

  if( magnitude < 0 ){
    ZRUNWARN( ZEO_WARN_ELEM_AABOX_NEGATIVE_MAGNITUDE );
    magnitude *= -1;
  }
  zAABox3DCenter( src, &center );
  return zAABox3DCreateFromSize( dest, &center,
    zAABox3DDepth(src) * magnitude, zAABox3DWidth(src) * magnitude, zAABox3DHeight(src) * magnitude );
}

#define zAABox3DExpandDRC(box,magnitude) zAABox3DExpand( box, magnitude, box )


void assert_aabox_create(void)
{
  zAABox3D box;
  int i;
  const int n = 10;
  bool result = true;

  for( i=0; i<n; i++ ){
    zAABox3DCreate( &box, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    if( zAABox3DXMin(&box) > zAABox3DXMax(&box) ||
        zAABox3DYMin(&box) > zAABox3DYMax(&box) ||
        zAABox3DZMin(&box) > zAABox3DZMax(&box) ) result = false;
  }
  zAssert( zAABox3DCreate, result );
}


void assert_aabox_equal(void)
{
  zAABox3D box1, box2;

  zAABox3DCreate( &box1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAABox3DCopy( &box1, &box2 );
  zAssert( zAABox3DCopy, zAABox3DEqual( &box1, &box2 ) );
}

void assert_aabox_expand(void)
{
  zAABox3D box1, box2, box3, box_test;
  zVec3D center;
  double depth, width, height;

  zVec3DCreate( &center, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  depth  = zRandF( 0.1, 10.0 );
  width  = zRandF( 0.1, 10.0 );
  height = zRandF( 0.1, 10.0 );
  zAABox3DCreateFromSize( &box1, &center, depth, width, height );
  zAABox3DExpand( &box1, 0.5, &box2 );
  zAABox3DExpand( &box1, 2.0, &box3 );
  zAssert( zAABox3DExpand,
    zAABox3DEqual( zAABox3DExpand( &box2, 2.0, &box_test ), &box1 ) &&
    zAABox3DEqual( zAABox3DExpand( &box3, 0.5, &box_test ), &box1 ) &&
    zAABox3DEqual( zAABox3DExpand( &box2, 4.0, &box_test ), &box3 ) );
  zAABox3DCopy( &box1, &box_test );
  zAssert( zAABox3DExpandDRC,
    zAABox3DEqual( zAABox3DExpandDRC( &box1, 0.5 ), &box2 ) &&
    zAABox3DEqual( zAABox3DExpandDRC( &box_test, 2.0 ), &box3 ) );
}

int main(void)
{
  zRandInit();
  assert_aabox_create();
  assert_aabox_equal();
  assert_aabox_expand();
  return 0;
}

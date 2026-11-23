#include <zeo/zeo.h>

/* exact values of orientations of points with integer coordinates. The coordinates are small
 * enough for the determinants not to overflow. */
long det3_int(const long a[], const long b[], const long c[], const long d[])
{
  long p[3], q[3], r[3];
  int i;

  for( i=0; i<3; i++ ){
    p[i] = b[i] - a[i];
    q[i] = c[i] - a[i];
    r[i] = d[i] - a[i];
  }
  return p[0]*(q[1]*r[2]-q[2]*r[1]) - p[1]*(q[0]*r[2]-q[2]*r[0]) + p[2]*(q[0]*r[1]-q[1]*r[0]);
}

long det2_int(const long a[], const long b[], const long c[], int i, int j)
{
  return (b[i]-a[i])*(c[j]-a[j]) - (b[j]-a[j])*(c[i]-a[i]);
}

int sgn(long x){ return x > 0 ? 1 : ( x < 0 ? -1 : 0 ); }

/* integer points: random, coplanar and perturbed from coplanar ones */
void create_points_int(long a[], long b[], long c[], long d[], int mode)
{
  long s1, s2;
  int i;

  for( i=0; i<3; i++ ){
    a[i] = zRandI( -10000, 10000 );
    b[i] = zRandI( -10000, 10000 );
    c[i] = zRandI( -10000, 10000 );
  }
  if( mode == 0 ){
    for( i=0; i<3; i++ ) d[i] = zRandI( -10000, 10000 );
    return;
  }
  s1 = zRandI( -3, 3 );
  s2 = zRandI( -3, 3 );
  for( i=0; i<3; i++ ) d[i] = a[i] + s1*(b[i]-a[i]) + s2*(c[i]-a[i]);
  if( mode == 2 ) d[zRandI(0,2)] += zRandI(0,1) ? 1 : -1;
  if( mode == 3 ) /* collinear */
    for( i=0; i<3; i++ ) c[i] = a[i] + s1*(b[i]-a[i]);
}

/* points scaled by a power of two and translated by an integer exactly */
void to_vec3d(const long x[], double scale, long offset, zVec3D *v)
{
  int i;

  for( i=0; i<3; i++ ) v->e[i] = (double)( x[i] + offset ) * scale;
}

void assert_orient(void)
{
  long a[3], b[3], c[3], d[3], offset;
  zVec3D va, vb, vc, vd;
  zVec2D wa, wb, wc;
  double scale;
  int i, k, mode, testnum = 100000;
  bool result3 = true, result2 = true, result2d = true, result_zero = true;

  for( i=0; i<testnum; i++ ){
    create_points_int( a, b, c, d, ( mode = i % 4 ) );
    scale = ldexp( 1.0, zRandI(-60,60) );
    offset = i % 3 == 0 ? (long)zRandI(-1000000,1000000) * 4096 : 0;
    to_vec3d( a, scale, offset, &va );
    to_vec3d( b, scale, offset, &vb );
    to_vec3d( c, scale, offset, &vc );
    to_vec3d( d, scale, offset, &vd );
    if( zVec3DOrient3D( &va, &vb, &vc, &vd ) != sgn( det3_int( a, b, c, d ) ) ) result3 = false;
    if( ( mode == 1 || mode == 3 ) && zVec3DOrient3D( &va, &vb, &vc, &vd ) != 0 ) result_zero = false;
    k = zRandI( 0, 2 );
    if( zVec3DOrient2D( &va, &vb, &vc, k ) != sgn( det2_int( a, b, c, (k+1)%3, (k+2)%3 ) ) ) result2 = false;
    zVec2DCreate( &wa, va.c.x, va.c.y );
    zVec2DCreate( &wb, vb.c.x, vb.c.y );
    zVec2DCreate( &wc, vc.c.x, vc.c.y );
    if( zVec2DOrient2D( &wa, &wb, &wc ) != sgn( det2_int( a, b, c, 0, 1 ) ) ) result2d = false;
  }
  zAssert( zVec3DOrient3D, result3 );
  zAssert( zVec3DOrient3D (coplanar points), result_zero );
  zAssert( zVec3DOrient2D, result2 );
  zAssert( zVec2DOrient2D, result2d );
}

int main(int argc, char *argv[])
{
  zRandInit();
  assert_orient();
  return EXIT_SUCCESS;
}

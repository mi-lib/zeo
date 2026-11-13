#include <zeo/zeo.h>

void assert_create(void)
{
  zVec3D v1, v2;
  double r, theta, phi;

  zVec3DCreate( &v1, 1, 2, 3 );
  zAssert( zVec3DCreate, v1.e[0] == 1 && v1.e[1] == 2 && v1.e[2] == 3 );
  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  r = zVec3DNorm( &v1 );
  theta = zVec3DAngle( &v1, ZVEC3DZ, NULL );
  phi = atan2( v1.e[1], v1.e[0] );
  zVec3DCreatePolar( &v2, r, theta, phi );
  zAssert( zVec3DCreatePolar, zVec3DEqual( &v1, &v2 ) );
  zVec3DCopy( &v1, &v2 );
  zAssert( zVec3DCopy, v1.e[0] == v2.e[0] && v1.e[1] == v2.e[1] && v1.e[2] == v2.e[2] );
  zAssert( zVec3DMatch, zVec3DMatch( &v1, &v2 ) );
  zVec3DZero( &v1 );
  zAssert( zVec3DIsTiny, zVec3DIsTiny( &v1 ) );
  zVec3DCreate( &v1, 1, 2, HUGE_VAL );
  zVec3DCreate( &v2, 1, 2, NAN );
  zAssert( zVec3DIsNan, zVec3DIsNan(&v1) && zVec3DIsNan(&v2) );
}

void assert_arith(void)
{
  zVec3D v1, v2, v3;
  double k;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  k = zRandF(-10,10);
  zVec3DAdd( &v1, &v2, &v3 );
  zAssert( zVec3DAdd, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2] );
  zVec3DSub( &v1, &v2, &v3 );
  zAssert( zVec3DSub, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2] );
  zVec3DRev( &v1, &v3 );
  zAssert( zVec3DRev, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0 );
  zVec3DMul( &v1, k, &v3 );
  zAssert( zVec3DMul, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2] );
  zVec3DDiv( &v1, k, &v3 );
  zAssert( zVec3DDiv, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2]) );
  zVec3DAmp( &v1, &v2, &v3 );
  zAssert( zVec3DAmp, v1.e[0]*v2.e[0]==v3.e[0] && v1.e[1]*v2.e[1]==v3.e[1] && v1.e[2]*v2.e[2]==v3.e[2] );
  zVec3DCat( &v1, k, &v2, &v3 );
  zAssert( zVec3DCat, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2] );

  zVec3DCopy( &v1, &v3 ); zVec3DAddDRC( &v3, &v2 );
  zAssert( zVec3DAddDRC, v1.e[0]+v2.e[0]==v3.e[0] && v1.e[1]+v2.e[1]==v3.e[1] && v1.e[2]+v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DSubDRC( &v3, &v2 );
  zAssert( zVec3DSubDRC, v1.e[0]-v2.e[0]==v3.e[0] && v1.e[1]-v2.e[1]==v3.e[1] && v1.e[2]-v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DRevDRC( &v3 );
  zAssert( zVec3DRevDRC, v1.e[0]+v3.e[0]==0 && v1.e[1]+v3.e[1]==0 && v1.e[2]+v3.e[2]==0 );
  zVec3DCopy( &v1, &v3 ); zVec3DMulDRC( &v3, k );
  zAssert( zVec3DMulDRC, v1.e[0]*k==v3.e[0] && v1.e[1]*k==v3.e[1] && v1.e[2]*k==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DDivDRC( &v3, k );
  zAssert( zVec3DDivDRC, zIsTiny(v1.e[0]/k-v3.e[0]) && zIsTiny(v1.e[1]/k-v3.e[1]) && zIsTiny(v1.e[2]/k-v3.e[2]) );
  zVec3DCopy( &v1, &v3 ); zVec3DAmpDRC( &v3, &v2 );
  zAssert( zVec3DAmpDRC, v1.e[0]*v2.e[0]==v3.e[0] && v1.e[1]*v2.e[1]==v3.e[1] && v1.e[2]*v2.e[2]==v3.e[2] );
  zVec3DCopy( &v1, &v3 ); zVec3DCatDRC( &v3, k, &v2 );
  zAssert( zVec3DCatDRC, v1.e[0]+k*v2.e[0]==v3.e[0] && v1.e[1]+k*v2.e[1]==v3.e[1] && v1.e[2]+k*v2.e[2]==v3.e[2] );
}

void assert_vecprod(void)
{
  zVec3D v1, v2, v3, v4, v5, v6, v7;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec3DInnerProd, v1.e[0]*v2.e[0]+v1.e[1]*v2.e[1]+v1.e[2]*v2.e[2] == zVec3DInnerProd(&v1,&v2) );
  zVec3DOuterProd( &v1, &v2, &v3 );
  zAssert( zVec3DOuterProd,
       v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1] == v3.e[0]
    && v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2] == v3.e[1]
    && v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0] == v3.e[2] );
  zVec3DCopy( &v1, &v4 ); zVec3DOuterProd( &v4, &v2, &v4 );
  zAssert( zVec3DOuterProd (v=vxu), zVec3DMatch( &v3, &v4 ) );
  zVec3DCopy( &v2, &v5 ); zVec3DOuterProd( &v1, &v5, &v5 );
  zAssert( zVec3DOuterProd (v=uxv), zVec3DMatch( &v3, &v5 ) );

  zAssert( zVec3DOuterProdNorm, zVec3DOuterProdNorm(&v1,&v2) == zVec3DNorm(&v3) );
  zVec3DCreate( &v3, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zAssert( zVec3DGrassmannProd, zIsTiny(zVec3DGrassmannProd(&v1,&v2,&v3)-((v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1])*v3.e[0]+(v1.e[2]*v2.e[0]-v1.e[0]*v2.e[2])*v3.e[1]+(v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0])*v3.e[2])) );
  zVec3DTripleProd( &v1, &v2, &v3, &v4 );
  zVec3DCopy( &v1, &v5 ); zVec3DTripleProd( &v5, &v2, &v3, &v5 );
  zVec3DCopy( &v2, &v6 ); zVec3DTripleProd( &v1, &v6, &v3, &v6 );
  zVec3DCopy( &v3, &v7 ); zVec3DTripleProd( &v1, &v2, &v7, &v7 );
  zAssert( zVec3DTripleProd,
       zIsTiny(v1.e[1]*v2.e[0]*v3.e[1]-v1.e[1]*v2.e[1]*v3.e[0]-v1.e[2]*v2.e[2]*v3.e[0]+v1.e[2]*v2.e[0]*v3.e[2]-v4.e[0])
    && zIsTiny(v1.e[2]*v2.e[1]*v3.e[2]-v1.e[2]*v2.e[2]*v3.e[1]-v1.e[0]*v2.e[0]*v3.e[1]+v1.e[0]*v2.e[1]*v3.e[0]-v4.e[1])
    && zIsTiny(v1.e[0]*v2.e[2]*v3.e[0]-v1.e[0]*v2.e[0]*v3.e[2]-v1.e[1]*v2.e[1]*v3.e[2]+v1.e[1]*v2.e[2]*v3.e[1]-v4.e[2])
    && v4.e[0]==v5.e[0] && v4.e[1]==v5.e[1] && v4.e[2]==v5.e[2]
    && v4.e[0]==v6.e[0] && v4.e[1]==v6.e[1] && v4.e[2]==v6.e[2]
    && v4.e[0]==v7.e[0] && v4.e[1]==v7.e[1] && v4.e[2]==v7.e[2] );

  zVec3DSub( &v1, &v2, &v3 );
  zAssert( zVec3DDist, zVec3DDist(&v1,&v2) == zVec3DNorm(&v3) &&
                       zVec3DDist(&v2,&v1) == zVec3DNorm(&v3) );
  zVec3DNormalize( &v1, 0, &v2 );
  zAssert( zVec3DNormalize, zIsTiny(v2.e[0]*v2.e[0]+v2.e[1]*v2.e[1]+v2.e[2]*v2.e[2]-1) );
  zVec3DNormalizeDRC( &v1, 0 );
  zAssert( zVec3DNormalizeDRC, zIsTiny(v1.e[0]*v1.e[0]+v1.e[1]*v1.e[1]+v1.e[2]*v1.e[2]-1) );
  zVec3DZero( &v1 );
  zAssert( zVec3DNormalize (zero vector), zVec3DNormalize( &v1, 0, &v2 ) == 0 );
  zVec3DZero( &v1 );
  zAssert( zVec3DNormalizeDRC (zero vector), zVec3DNormalizeDRC( &v1, 0 ) == 0 );
  /* tiny vector with the norm 5e-13 */
  zVec3DCreate( &v1, 3.0e-13, 0, 4.0e-13 );
  zVec3DCreate( &v2, 1, 2, 3 );
  zAssert( zVec3DNormalize (default tolerance), zVec3DNormalize( &v1, 0, &v2 ) == 0 && zVec3DIsTol( &v2, 0 ) );
  zAssert( zVec3DNormalize (specified tolerance),
    zIsTol( zVec3DNormalize( &v1, 1.0e-20, &v2 ) - 5.0e-13, 1.0e-28 ) &&
    zIsTol( v2.c.x - 0.6, 1.0e-15 ) && v2.c.y == 0 && zIsTol( v2.c.z - 0.8, 1.0e-15 ) &&
    zVec3DNormalize( &v1, 1.0e-12, &v2 ) == 0 );
  v2 = v1;
  zAssert( zVec3DNormalizeDRC (specified tolerance), zVec3DNormalizeDRC( &v2, 1.0e-20 ) > 0 && zIsTiny( zVec3DNorm( &v2 ) - 1 ) );
  /* not-a-number */
  zVec3DCreate( &v1, NAN, 1, 1 );
  zAssert( zVec3DNormalize (not-a-number), zVec3DNormalize( &v1, 0, &v2 ) == 0 );
}

void assert_geometry(void)
{
  zVec3D v1, v2, v3, v4;

  zVec3DCreate( &v1, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DCreate( &v2, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  zVec3DProj( &v1, &v2, &v3 );
  zVec3DOuterProd( &v2, &v3, &v4 );
  zAssert( zVec3DProj, zIsTiny(zVec3DInnerProd(&v1,&v3)-zVec3DSqrNorm(&v3)) && zVec3DIsTiny(&v4) );
  zVec3DOrthogonalize( &v1, &v2, &v3 );
  zAssert( zVec3DOrthogonalize, zIsTiny(zVec3DInnerProd(&v2,&v3)) );
  zVec3DOrthoSpace( &v1, &v2, &v3 );
  zAssert( zVec3DOrthoSpace, zIsTiny(zVec3DInnerProd(&v1,&v2)) && zIsTiny(zVec3DInnerProd(&v2,&v3)) && zIsTiny(zVec3DInnerProd(&v3,&v1)) );
  zVec3DRot( &v1, &v2, &v3 );
  zVec3DRevDRC( &v2 );
  zAssert( zVec3DRot + zVec3DAngle, zIsTiny(zVec3DAngle(&v1,&v3,&v2)+zVec3DNorm(&v2)) );
  zVec3DAAError( &v1, &v2, &v4 );
  zVec3DRot( &v1, &v4, &v3 );
  zAssert( zVec3DAAError + zVec3DRot, zVec3DIsTiny( zVec3DOuterProd( &v2, &v3, &v4 ) ) );
}

bool check_orthonormal_space(const zVec3D *v_org, const zVec3D *v, const zVec3D *sv1, const zVec3D *sv2)
{
  zVec3D d;

  zVec3DOuterProd( v_org, v, &d );
  return zIsTiny( zVec3DNorm(v) - 1 ) && zIsTiny( zVec3DNorm(sv1) - 1 ) && zIsTiny( zVec3DNorm(sv2) - 1 ) &&
         zIsTiny( zVec3DInnerProd(v,sv1) ) && zIsTiny( zVec3DInnerProd(sv1,sv2) ) && zIsTiny( zVec3DInnerProd(sv2,v) ) &&
         zIsTiny( zVec3DGrassmannProd(v,sv1,sv2) - 1 ) && /* right-handed */
         zVec3DInnerProd(v_org,v) > 0 && zIsTiny( zVec3DNorm(&d) / zVec3DNorm(v_org) ); /* same direction */
}

/* check zVec3DOrthoSpace() and zVec3DOrthonormalSpace() for a vector. */
bool check_ortho_space_one(const zVec3D *v_org, bool expected)
{
  zVec3D v, n, sv1, sv2;

  /* zVec3DOrthoSpace() */
  if( zVec3DOrthoSpace( v_org, &sv1, &sv2 ) != expected ) return false;
  if( expected ){
    zVec3DNormalize( v_org, 0, &n );
    if( !check_orthonormal_space( v_org, &n, &sv1, &sv2 ) ) return false;
  }
  /* zVec3DOrthonormalSpace() */
  zVec3DCopy( v_org, &v );
  if( zVec3DOrthonormalSpace( &v, &sv1, &sv2 ) != expected ) return false;
  return expected ? check_orthonormal_space( v_org, &v, &sv1, &sv2 ) : zVec3DMatch( &v, v_org );
}

#define N 1000
void assert_orthonormal_space(void)
{
  zVec3D v;
  int i;
  bool result_random = true, result_axis = true, result_small = true, result_zero;

  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );
    zVec3DMulDRC( &v, pow( 10, zRandF(-6,6) ) );
    if( !check_ortho_space_one( &v, true ) ) result_random = false;
  }
  for( i=zX; i<=zZ; i++ ){ /* vectors along the axes */
    zVec3DZero( &v );
    v.e[i] = 5;
    if( !check_ortho_space_one( &v, true ) ) result_axis = false;
  }
  /* a vector whose components are all tiny but whose norm is larger than the tolerance */
  zVec3DCreate( &v, 0, -1.0e-12, 1.0e-12 );
  if( !check_ortho_space_one( &v, true ) ) result_small = false;
  /* a vector whose norm is not larger than the tolerance, while the components of the vector
   * orthogonal to it are not tiny. */
  zVec3DCreate( &v, 0, -0.7e-12, 0.7e-12 );
  if( !check_ortho_space_one( &v, false ) ) result_small = false;
  zVec3DZero( &v );
  result_zero = check_ortho_space_one( &v, false );
  zAssert( zVec3DOrthoSpace + zVec3DOrthonormalSpace (random), result_random );
  zAssert( zVec3DOrthoSpace + zVec3DOrthonormalSpace (vectors along axes), result_axis );
  zAssert( zVec3DOrthoSpace + zVec3DOrthonormalSpace (small vectors), result_small );
  zAssert( zVec3DOrthoSpace + zVec3DOrthonormalSpace (zero vector), result_zero );
}

void assert_angvel_conv(void)
{
  zVec3D ea, eavel, angvel, test;

  zVec3DCreate( &ea, zRandF(-zPI,zPI), zRandF(-zPI,zPI), zRandF(-zPI,zPI) );
  zVec3DCreate( &angvel, zRandF(-1,1), zRandF(-1,1), zRandF(-1,1) );

  zAngVelToZYXVel( &angvel, &ea, &eavel );
  zZYXVelToAngVel( &eavel, &ea, &test );
  zAssert( zAngVelToZYXVel + zZYXVelToAngVel, zVec3DEqual(&angvel,&test) );

  zAngVelToZYZVel( &angvel, &ea, &eavel );
  zZYZVelToAngVel( &eavel, &ea, &angvel );
  zAssert( zAngVelToZYZVel + zZYZVelToAngVel, zVec3DEqual(&angvel,&test) );
}

#define V    (1.0e-4)
#define STEP 100
#define TOL  (1.0e-6)

void assert_angvel_zyx_intg(void)
{
  zVec3D zyxvel, angvel, zyx, err;
  zMat3D m1, m2, tmp;
  int i;
  bool res = true;

  zVec3DZero( &zyx );
  zMat3DIdent( &m2 );
  zVec3DCreate( &zyxvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  for( i=0; i<=STEP; i++ ){
    /* z-y-x Eulerian angle velocity -> angular velocity */
    zZYXVelToAngVel( &zyxvel, &zyx, &angvel );
    /* z-y-x Eulerian angle -> matrix */
    zVec3DAddDRC( &zyx, &zyxvel );
    zMat3DFromZYX( &m1, zyx.c.x, zyx.c.y, zyx.c.z );
    /* angular velocity vector -> matrix */
    zMat3DCopy( &m2, &tmp );
    zMat3DRot( &tmp, &angvel, &m2 );
    /* error */
    zMat3DError( &m1, &m2, &err );
    if( !zVec3DIsTol( &err, TOL ) ){
      res = false;
      break;
    }
  }
  zAssert( zZYXVelToAngVel (integral), res );
}

void assert_angvel_zyz_intg(void)
{
  zVec3D zyzvel, angvel, zyz, err;
  zMat3D m1, m2, tmp;
  int i;
  bool res = true;

  zVec3DZero( &zyz );
  zMat3DIdent( &m2 );
  zVec3DCreate( &zyzvel, zRandF(-V,V), zRandF(-V,V), zRandF(-V,V) );
  for( i=0; i<=STEP; i++ ){
    /* z-y-x Eulerian angle velocity -> angular velocity */
    zZYZVelToAngVel( &zyzvel, &zyz, &angvel );
    /* z-y-x Eulerian angle -> matrix */
    zVec3DAddDRC( &zyz, &zyzvel );
    zMat3DFromZYZ( &m1, zyz.e[zX], zyz.e[zY], zyz.e[zZ] );
    /* angular velocity vector -> matrix */
    zMat3DCopy( &m2, &tmp );
    zMat3DRot( &tmp, &angvel, &m2 );
    /* error */
    zMat3DError( &m1, &m2, &err );
    if( !zVec3DIsTol( &err, TOL ) ){
      res = false;
      break;
    }
  }
  zAssert( zZYZVelToAngVel (integral), res );
}

void assert_vec3d_ztk(void)
{
  ZTK ztk;
  zVec3D src, dest;

  zVec3DCreate( &src, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
  ZTKInit( &ztk );
  ZTKAddTag( &ztk, "" );
  ZTKAddKey( &ztk, "" );
  zVec3DToZTK( &src, &ztk );
  ZTKRewind( &ztk );
  zVec3DFromZTK( &dest, &ztk );
  ZTKDestroy( &ztk );
  zVec3DSubDRC( &src, &dest );
  zAssert( zVec3DFromZTK + zVec3DToZTK, zVec3DIsTol( &src, 1.0e-9 ) );
}

/* division by very small or large values, where each component is accessed as a double array */
bool check_div_array_vec3d(const double *a, double k, const double *q, int n)
{
  int i;

  for( i=0; i<n; i++ )
    if( !zIsFinite( q[i] ) || fabs( q[i] - a[i]/k ) > DBL_EPSILON * fabs( a[i]/k ) ) return false;
  return true;
}

void assert_div_range_vec3d(void)
{
  zVec3D a, q, d;
  double k[] = { 3.0, 1.0e-13, 4.0e-309, 1.0e308 }; /* including a subnormal divisor and a divisor with the subnormal reciprocal */
  const int n = sizeof(zVec3D) / sizeof(double);
  int i, j;
  bool result = true, result_drc = true, result_zero;

  for( j=0; j<sizeof(k)/sizeof(double); j++ ){
    for( i=0; i<n; i++ ) ((double *)&a)[i] = zRandF(-1,1) * ( k[j] < 1.0e-300 ? 1.0e-300 : 1 ); /* avoid overflow */
    if( !zVec3DDiv( &a, k[j], &q ) || !check_div_array_vec3d( (double *)&a, k[j], (double *)&q, n ) ) result = false;
    d = a;
    if( !zVec3DDivDRC( &d, k[j] ) ) result_drc = false;
    for( i=0; i<n; i++ )
      if( ((double *)&d)[i] != ((double *)&q)[i] ) result_drc = false;
  }
  result_zero = zVec3DDiv( &a, 0, &q ) == NULL;
  zAssert( zVec3DDiv (very small or large divisor), result );
  zAssert( zVec3DDivDRC (very small or large divisor), result_drc );
  zAssert( zVec3DDiv (division by zero), result_zero );
}

/* division where the divisor refers to a component of the destination */
void assert_div_alias_vec3d(void)
{
  zVec3D v;
  double k;

  zVec3DCreate( &v, 6, 4, 2 );
  _zVec3DDivDRC( &v, v.c.x );
  zAssert( _zVec3DDivDRC (divisor in the destination), v.c.x == 1 && v.c.y == 4.0/6 && v.c.z == 2.0/6 );
  zVec3DCreate( &v, 6.0e-300, 4.0e-300, 2.0e-300 );
  k = 6.0e-300 * 1.0e-10; /* subnormal divisor */
  _zVec3DDivDRC( &v, v.c.x * 1.0e-10 );
  zAssert( _zVec3DDivDRC (divisor in the destination, subnormal), v.c.x == 6.0e-300/k && v.c.y == 4.0e-300/k && v.c.z == 2.0e-300/k );
}

int main(void)
{
  zRandInit();
  assert_create();
  assert_arith();
  assert_vecprod();
  assert_geometry();
  assert_orthonormal_space();
  assert_angvel_conv();
  assert_angvel_zyx_intg();
  assert_angvel_zyz_intg();
  assert_vec3d_ztk();
  assert_div_range_vec3d();
  assert_div_alias_vec3d();
  return EXIT_SUCCESS;
}

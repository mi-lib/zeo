/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_ph - collision checking: polyhedron.
 */

#include <zeo/zeo_col.h>

/* check if two polyhedra intersect with each other. */
bool zColChkPH3D(zPH3D *ph1, zPH3D *ph2, zVec3D *p1, zVec3D *p2)
{
  zVec3D _p1, _p2;
  zVec3DData data1, data2;

  if( p1 == NULL ) p1 = &_p1;
  if( p2 == NULL ) p2 = &_p2;
  zVec3DDataAssignArray( &data1, zPH3DVertArray(ph1) );
  zVec3DDataAssignArray( &data2, zPH3DVertArray(ph2) );
  return zGJK( &data1, &data2, p1, p2 );
}

/* intersection of convex polyhedra based on Muller-Preparata's algorithm:
 *   D. E. Muller and F. P. Preparata, Finding the Intersection of Two Convex Polyhedra, Theoretical
 *   Computer Science, 7(2):217-236, 1978.
 * The face planes of the polyhedra are transformed to points in the dual space with respect to a base
 * point inside of the intersection, namely, a plane n.(x-p)=d with a unit normal vector n is
 * transformed to a point q=n/d, where p is the base point and d>0 is the distance from it. Each face
 * of the convex hull of the dual points corresponds to a vertex of the intersection.
 *
 * The transformation is singular at d=0, and the accuracy of the computation in the dual space is
 * lost as the ratio of the size of the polyhedra to the minimum distance d grows. Hence, the base
 * point is chosen at the center of the largest ball inscribed in the intersection (the Chebyshev
 * center), which maximizes the minimum distance. In addition, the vertex corresponding to a face
 * (q1,q2,q3) of the convex hull in the dual space is computed as the intersection of the three
 * original planes q_i.(x-p)=1, namely,
 *   x - p = ( q1 x q2 + q2 x q3 + q3 x q1 ) / q1.(q2 x q3),
 * where the outer products are directly computed without differences of the dual points, so that
 * the accuracy does not depend on the distances of the planes from the base point. Note that it is
 * algebraically identical to the inverse transformation of the face, but the latter, in which the
 * normal vector of the face is computed from the differences of the dual points, loses the accuracy
 * if the intersection is thin. */

/* a face plane n.(x-c)<=b with a unit normal vector n, where c is the reference point. */
typedef struct{
  zVec3D n;
  double b;
} zIntersectPH3DPlane;

/* solve a 4x4 linear equation a x = b by Gaussian elimination with partial pivoting. */
static bool _zIntersectPH3DSolve4(double a[4][4], double b[4], double x[4])
{
  double tmp, r;
  int i, j, k, p;

  for( k=0; k<4; k++ ){
    for( p=k, i=k+1; i<4; i++ )
      if( fabs( a[i][k] ) > fabs( a[p][k] ) ) p = i;
    if( a[p][k] == 0 ) return false;
    if( p != k ){
      for( j=0; j<4; j++ ){
        tmp = a[k][j]; a[k][j] = a[p][j]; a[p][j] = tmp;
      }
      tmp = b[k]; b[k] = b[p]; b[p] = tmp;
    }
    for( i=k+1; i<4; i++ ){
      r = a[i][k] / a[k][k];
      for( j=k; j<4; j++ ) a[i][j] -= r * a[k][j];
      b[i] -= r * b[k];
    }
  }
  for( k=3; k>=0; k-- ){
    for( tmp=b[k], j=k+1; j<4; j++ ) tmp -= a[k][j] * x[j];
    x[k] = tmp / a[k][k];
  }
  return true;
}

/* Chebyshev center of the intersection of half spaces, which is the solution x of the following
 * linear programming problem:
 *   t -> maximum, subject to n_i.x + t <= b_i.
 * It is solved via the dual problem:
 *   sum_i b_i y_i -> minimum, subject to sum_i n_i y_i = 0, sum_i y_i = 1, y_i >= 0,
 * which has only four equality constraints. The solution x and t are obtained from the final bases of
 * the simplex method, namely, the constraints of the original problem corresponding to the bases are
 * active. The minimum distance of x from the planes is put into rho, which is negative if x is
 * outside of the intersection. */
static bool _zIntersectPH3DCenter(zIntersectPH3DPlane *plane, int m, zVec3D *x, double *rho)
{
  zMat a;
  zVec b, c;
  zLPTableau tab;
  double a4[4][4], b4[4], x4[4], d;
  int i, j, k;
  bool ret = false;

  a = zMatAlloc( 4, m );
  b = zVecAlloc( 4 );
  c = zVecAlloc( m );
  if( !a || !b || !c ) goto TERMINATE;
  for( i=0; i<m; i++ ){
    for( k=0; k<3; k++ )
      zMatSetElemNC( a, k, i, plane[i].n.e[k] );
    zMatSetElemNC( a, 3, i, 1 );
    zVecSetElemNC( c, i, plane[i].b );
  }
  zVecSetElemNC( b, 3, 1 );
  if( !zLPTableauCreate( &tab, a, b ) ) goto TERMINATE2;
  if( !zLPTableauFindFeasibleBase( &tab ) ) goto TERMINATE2;
  zVecSetSize( tab.c, m );
  zVecCopyNC( c, tab.c );
  if( !zLPTableauSimplex( &tab ) ) goto TERMINATE2;
  if( zIndexSizeNC(tab.ib) != 4 ) goto TERMINATE2; /* never happens for bounded polyhedra */
  for( k=0; k<4; k++ ){
    if( ( j = zIndexElemNC(tab.ib,k) ) >= m ) goto TERMINATE2;
    zVec3DCopy( &plane[j].n, (zVec3D *)a4[k] );
    a4[k][3] = 1;
    b4[k] = plane[j].b;
  }
  if( !_zIntersectPH3DSolve4( a4, b4, x4 ) ) goto TERMINATE2;
  zVec3DCreate( x, x4[0], x4[1], x4[2] );
  /* the actual minimum distance of x from the planes */
  for( *rho=HUGE_VAL, i=0; i<m; i++ )
    if( ( d = plane[i].b - zVec3DInnerProd( &plane[i].n, x ) ) < *rho ) *rho = d;
  ret = true;
 TERMINATE2:
  zLPTableauDestroy( &tab );
 TERMINATE:
  zMatFree( a );
  zVecFree( b );
  zVecFree( c );
  return ret;
}

/* reference point and size of two polyhedra, namely, the centroid of the vertices and the maximum
 * distance of the vertices from it. */
static double _zIntersectPH3DRef(zPH3D *ph1, zPH3D *ph2, zVec3D *c)
{
  double r = 0, d;
  int i;

  zVec3DZero( c );
  for( i=0; i<zPH3DVertNum(ph1); i++ ) zVec3DAddDRC( c, zPH3DVert(ph1,i) );
  for( i=0; i<zPH3DVertNum(ph2); i++ ) zVec3DAddDRC( c, zPH3DVert(ph2,i) );
  zVec3DDivDRC( c, zPH3DVertNum(ph1) + zPH3DVertNum(ph2) );
  for( i=0; i<zPH3DVertNum(ph1); i++ )
    if( ( d = zVec3DDist( zPH3DVert(ph1,i), c ) ) > r ) r = d;
  for( i=0; i<zPH3DVertNum(ph2); i++ )
    if( ( d = zVec3DDist( zPH3DVert(ph2,i), c ) ) > r ) r = d;
  return r;
}

/* face planes of a polyhedron with respect to a reference point c. Degenerate faces, the normal
 * vectors of which are the zero vector, are skipped. */
static int _zIntersectPH3DPlanes(zPH3D *ph, zVec3D *c, zIntersectPH3DPlane *plane)
{
  zVec3D d;
  int i, m = 0;

  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    if( zVec3DIsTiny( zTri3DNorm(zPH3DFace(ph,i)) ) ) continue;
    zVec3DCopy( zTri3DNorm(zPH3DFace(ph,i)), &plane[m].n );
    zVec3DSub( zTri3DVert(zPH3DFace(ph,i),0), c, &d );
    plane[m].b = zVec3DInnerProd( &plane[m].n, &d );
    m++;
  }
  return m;
}

/* the vertex of the intersection corresponding to a face of the convex hull in the dual space with
 * respect to the base point, namely, the intersection of three planes q_i.x=1 for the vertices q_i of
 * the face. It returns the false value if the planes are degenerate within the tolerance. */
static bool _zIntersectPH3DVert(zTri3D *f, zVec3D *v)
{
  zVec3D *q1, *q2, *q3, p;
  double det;

  q1 = zTri3DVert(f,0); q2 = zTri3DVert(f,1); q3 = zTri3DVert(f,2);
  zVec3DOuterProd( q2, q3, &p );
  /* q1.(q2 x q3) / |q1||q2||q3| is the triple product of the unit normal vectors of the planes */
  if( fabs( ( det = zVec3DInnerProd( q1, &p ) ) ) <= zTOL * zVec3DNorm(q1) * zVec3DNorm(q2) * zVec3DNorm(q3) )
    return false;
  zVec3DCopy( &p, v );
  zVec3DAddDRC( v, zVec3DOuterProd( q1, q2, &p ) );
  zVec3DAddDRC( v, zVec3DOuterProd( q3, q1, &p ) );
  zVec3DDivDRC( v, det );
  return true;
}

/* intersection of convex polyhedra by Muller-Preparata's algorithm. */
zPH3D *zIntersectPH3D(zPH3D *ph1, zPH3D *ph2, zPH3D *phcol)
{
  zIntersectPH3DPlane *plane;
  zVec3DData data;
  zVec3D c, x, v;
  zPH3D ch;
  double r, rho;
  int i, m;
  zPH3D *ret = NULL;

  zPH3DInit( phcol );
  zPH3DInit( &ch );
  if( zPH3DVertNum(ph1) == 0 || zPH3DVertNum(ph2) == 0 ) return NULL;
  if( !zColChkPH3D( ph1, ph2, NULL, NULL ) ) return NULL; /* quick rejection */
  if( !( plane = zAlloc( zIntersectPH3DPlane, zPH3DFaceNum(ph1) + zPH3DFaceNum(ph2) ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  /* the base point, which is relative to the reference point c */
  r = _zIntersectPH3DRef( ph1, ph2, &c );
  m = _zIntersectPH3DPlanes( ph1, &c, plane );
  m += _zIntersectPH3DPlanes( ph2, &c, plane+m );
  if( !_zIntersectPH3DCenter( plane, m, &x, &rho ) ) goto TERMINATE;
  if( rho <= zTOL * r ) goto TERMINATE; /* the intersection is empty or degenerate */
  /* dual transformation and the convex hull in the dual space */
  if( !zVec3DDataInitArray( &data, m ) ) goto TERMINATE;
  for( i=0; i<m; i++ )
    zVec3DDataAdd( &data, zVec3DMul( &plane[i].n, 1.0/( plane[i].b - zVec3DInnerProd( &plane[i].n, &x ) ), &v ) );
  if( !zVec3DDataConvexHull( &data, &ch ) ) goto TERMINATE2;
  zVec3DDataDestroy( &data );
  /* vertices of the intersection */
  if( !zVec3DDataInitArray( &data, zPH3DFaceNum(&ch) ) ) goto TERMINATE;
  zVec3DAddDRC( &x, &c );
  for( i=0; i<zPH3DFaceNum(&ch); i++ )
    if( _zIntersectPH3DVert( zPH3DFace(&ch,i), &v ) )
      zVec3DDataAdd( &data, zVec3DAddDRC( &v, &x ) );
  ret = zVec3DDataConvexHull( &data, phcol );
 TERMINATE2:
  zVec3DDataDestroy( &data );
 TERMINATE:
  zPH3DDestroy( &ch );
  zFree( plane );
  return ret;
}

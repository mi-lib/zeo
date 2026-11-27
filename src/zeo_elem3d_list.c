/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_elem3d_list - 3D shape element list.
 */

#include <zeo/zeo_elem3d.h>
#include <zeo/zeo_elem2d.h>

/* ********************************************************** */
/* 3D triangle list.
 * ********************************************************** */

/* add a 3D triangle list cell. */
zTri3DListCell *zTri3DListAdd(zTri3DList *list, const zTri3D *t)
{
  zTri3DListCell *cell;

  if( !( cell = zAlloc( zTri3DListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zTri3DCopy( t, &cell->data );
  zListInsertHead( list, cell );
  return cell;
}

/* align triangles to a direction referred by a vector. */
void zTri3DListAlign(zTri3DList *list, const zVec3D *ref)
{
  zTri3DListCell *tp;

  zListForEach( list, tp )
    if( zVec3DInnerProd( ref, zTri3DNorm(&tp->data) ) < 0 )
      zTri3DFlipDRC( &tp->data );
}

/* copy triangles in a list to array. */
void zTri3DListCopyArray(const zTri3DList *list, zTri3D t[], int n)
{
  zTri3DListCell *tp;
  int i = 0;

  zListForEach( list, tp ){
    zTri3DCopy( &tp->data, &t[i] );
    if( ++i >= n ) break;
  }
}

/* ********************************************************** */
/* triangulation of a simple polygon.
 * The polygon is projected onto its plane, the normal vector of which is computed by Newell's
 * method, divided into triangles by cutting ears, and then the triangulation is made the
 * constrained Delaunay triangulation by flipping diagonals (Lawson's method), which maximizes the
 * minimum angle of the triangles among all triangulations of the polygon. An ear to be cut is the
 * one that has the largest minimum angle, so that the initial triangulation is already close to it.
 * ********************************************************** */

/* indices of three vertices of a triangle, or adjacent triangles of it. */
typedef int _zTriangulateIndex3[3];

/* workspace of the triangulation. */
typedef struct{
  int n;          /* number of vertices */
  const zVec3D **v; /* vertices in the order of the loop */
  zVec2D *q;      /* vertices projected onto the plane of the polygon */
  double margin;  /* tolerance of the distance */
  double eps;     /* tolerance of the area */
  int *prev, *next; /* neighbors in the remaining polygon */
  double *quality;  /* quality of the ear at each vertex (negative if not an ear) */
  _zTriangulateIndex3 *tri; /* triangles counterclockwise on the plane */
  _zTriangulateIndex3 *adj; /* adjacent triangles across the edge from tri[i][k] to tri[i][k+1] (-1 for none) */
  int nt;         /* number of triangles */
} _zTriangulateWS;

/* the k-th vertex of the t-th triangle and the triangle adjacent to it across the k-th edge,
 * where k is cyclic. */
#define _zTriangulateVert(ws,t,k)  (ws)->tri[t][(k)%3]
#define _zTriangulateAdj(ws,t,k)   (ws)->adj[t][(k)%3]
/* set the vertices and the adjacent triangles of the t-th triangle. */
#define _zTriangulateSetTri(ws,t,v0,v1,v2,a0,a1,a2) do{\
  (ws)->tri[t][0] = (v0); (ws)->tri[t][1] = (v1); (ws)->tri[t][2] = (v2);\
  (ws)->adj[t][0] = (a0); (ws)->adj[t][1] = (a1); (ws)->adj[t][2] = (a2);\
} while(0)

/* allocate the workspace of the triangulation. */
static bool _zTriangulateWSAlloc(_zTriangulateWS *ws, int n)
{
  ws->n = n;
  ws->nt = 0;
  ws->v = zAlloc( const zVec3D*, n );
  ws->q = zAlloc( zVec2D, n );
  ws->prev = zAlloc( int, n );
  ws->next = zAlloc( int, n );
  ws->quality = zAlloc( double, n );
  ws->tri = zAlloc( _zTriangulateIndex3, n );
  ws->adj = zAlloc( _zTriangulateIndex3, n );
  if( !ws->v || !ws->q || !ws->prev || !ws->next || !ws->quality || !ws->tri || !ws->adj ){
    ZALLOCERROR();
    return false;
  }
  return true;
}

/* free the workspace of the triangulation. */
static void _zTriangulateWSFree(_zTriangulateWS *ws)
{
  free( ws->v );
  free( ws->q );
  free( ws->prev );
  free( ws->next );
  free( ws->quality );
  free( ws->tri );
  free( ws->adj );
}

/* signed area of a triangle on the plane. */
static double _zTriangulateArea(const _zTriangulateWS *ws, int a, int b, int c)
{
  zTri2D tri;

  return zTri2DSignedArea( zTri2DCreate( &tri, &ws->q[a], &ws->q[b], &ws->q[c] ) );
}

/* project the vertices onto the plane of the polygon, where the normal vector is computed by
 * Newell's method, namely, as the sum of the outer products of consecutive vertices, which is
 * robust against collinear vertices. The projected polygon is counterclockwise. */
static bool _zTriangulateProject(_zTriangulateWS *ws)
{
  zVec3D n, u, w, d;
  double size = 0;
  int i;

  zVec3DZero( &n );
  for( i=0; i<ws->n; i++ ){
    zVec3DOuterProd( ws->v[i], ws->v[(i+1)%ws->n], &d );
    zVec3DAddDRC( &n, &d );
  }
  if( zVec3DIsTiny( &n ) ) return false; /* degenerate polygon */
  zVec3DOrthonormalSpace( &n, &u, &w ); /* (u, w, n) is a right-handed orthonormal frame */
  for( i=0; i<ws->n; i++ ){
    zVec3DSub( ws->v[i], ws->v[0], &d );
    zVec2DCreate( &ws->q[i], zVec3DInnerProd( &d, &u ), zVec3DInnerProd( &d, &w ) );
    size = zMax( size, zVec2DSqrNorm( &ws->q[i] ) );
  }
  ws->margin = zTOL * sqrt( size );
  ws->eps = zTOL * size;
  return true;
}

/* minimum of the squared sines of the angles of a triangle, which increases with the minimum angle
 * up to 60 degrees (negative if the triangle is not counterclockwise). */
static double _zTriangulateQuality(const _zTriangulateWS *ws, int a, int b, int c)
{
  double s, la, lb, lc;

  if( ( s = _zTriangulateArea( ws, a, b, c ) ) <= ws->eps ) return -1;
  la = zVec2DSqrDist( &ws->q[b], &ws->q[c] );
  lb = zVec2DSqrDist( &ws->q[c], &ws->q[a] );
  lc = zVec2DSqrDist( &ws->q[a], &ws->q[b] );
  /* sin^2 of the angle at a vertex is (2s)^2 / ( the product of the squared lengths of the adjacent edges ) */
  return 4 * s * s / ( zMax3( lb*lc, lc*la, la*lb ) );
}

/* quality of the ear at a vertex, which is negative if it is not an ear, namely, if the vertex is
 * not convex or another vertex is inside of or on the ear. Since only reflex vertices can be inside
 * of an ear, cutting an ear changes only the ears at its neighbors. */
static double _zTriangulateEar(const _zTriangulateWS *ws, int i)
{
  zTri2D ear;
  int a, c, j;
  double quality;

  a = ws->prev[i];
  c = ws->next[i];
  if( ( quality = _zTriangulateQuality( ws, a, i, c ) ) < 0 ) return -1;
  zTri2DCreate( &ear, &ws->q[a], &ws->q[i], &ws->q[c] );
  for( j=ws->next[c]; j!=a; j=ws->next[j] )
    if( _zTriangulateArea( ws, ws->prev[j], j, ws->next[j] ) <= ws->eps &&
        zTri2DPointIsInside( &ear, &ws->q[j], ws->margin ) ) return -1;
  return quality;
}

/* add a triangle. */
static void _zTriangulateAddTri(_zTriangulateWS *ws, int a, int b, int c)
{
  _zTriangulateSetTri( ws, ws->nt, a, b, c, -1, -1, -1 );
  ws->nt++;
}

/* divide the polygon into triangles by cutting the ear with the largest minimum angle one by one.
 * If no ear is found due to the degeneracy of the polygon, the most convex vertex is cut. */
static void _zTriangulateCutEar(_zTriangulateWS *ws)
{
  int i, ibest, m, start = 0;
  double qbest;

  for( i=0; i<ws->n; i++ ){
    ws->prev[i] = ( i + ws->n - 1 ) % ws->n;
    ws->next[i] = ( i + 1 ) % ws->n;
  }
  for( i=0; i<ws->n; i++ ) ws->quality[i] = _zTriangulateEar( ws, i );
  for( m=ws->n; m>3; m-- ){
    ibest = -1;
    qbest = -1;
    i = start;
    do{
      if( ws->quality[i] > qbest ){
        qbest = ws->quality[i];
        ibest = i;
      }
    } while( ( i = ws->next[i] ) != start );
    if( ibest < 0 ){ /* degenerate polygon */
      i = start;
      do{
        if( _zTriangulateArea( ws, ws->prev[i], i, ws->next[i] ) > qbest ){
          qbest = _zTriangulateArea( ws, ws->prev[i], i, ws->next[i] );
          ibest = i;
        }
      } while( ( i = ws->next[i] ) != start );
      if( qbest <= ws->eps ) return; /* all the remaining vertices are collinear */
    }
    _zTriangulateAddTri( ws, ws->prev[ibest], ibest, ws->next[ibest] );
    ws->next[ws->prev[ibest]] = ws->next[ibest];
    ws->prev[ws->next[ibest]] = ws->prev[ibest];
    start = ws->next[ibest];
    ws->quality[ws->prev[ibest]] = _zTriangulateEar( ws, ws->prev[ibest] );
    ws->quality[ws->next[ibest]] = _zTriangulateEar( ws, ws->next[ibest] );
  }
  if( _zTriangulateArea( ws, ws->prev[start], start, ws->next[start] ) > ws->eps )
    _zTriangulateAddTri( ws, ws->prev[start], start, ws->next[start] );
}

/* find adjacent triangles across edges. */
static void _zTriangulateFindAdj(_zTriangulateWS *ws)
{
  int i, j, k, l;

  for( i=0; i<ws->nt; i++ )
    for( k=0; k<3; k++ ){
      if( _zTriangulateAdj(ws,i,k) >= 0 ) continue;
      for( j=i+1; j<ws->nt; j++ )
        for( l=0; l<3; l++ )
          if( _zTriangulateVert(ws,j,l) == _zTriangulateVert(ws,i,k+1) &&
              _zTriangulateVert(ws,j,l+1) == _zTriangulateVert(ws,i,k) ){
            _zTriangulateAdj(ws,i,k) = j;
            _zTriangulateAdj(ws,j,l) = i;
          }
    }
}

/* check if a point d is strictly inside of the circumcircle of a counterclockwise triangle abc. */
static bool _zTriangulateInCircle(const _zTriangulateWS *ws, int a, int b, int c, int d)
{
  zVec2D pa, pb, pc;
  double la, lb, lc, det;

  zVec2DSub( &ws->q[a], &ws->q[d], &pa );
  zVec2DSub( &ws->q[b], &ws->q[d], &pb );
  zVec2DSub( &ws->q[c], &ws->q[d], &pc );
  la = zVec2DSqrNorm( &pa );
  lb = zVec2DSqrNorm( &pb );
  lc = zVec2DSqrNorm( &pc );
  det = la * zVec2DOuterProd( &pb, &pc ) + lb * zVec2DOuterProd( &pc, &pa ) + lc * zVec2DOuterProd( &pa, &pb );
  return det > ws->eps * zMax3( la, lb, lc );
}

/* index of a vertex in a triangle. */
static int _zTriangulateVertID(const _zTriangulateWS *ws, int t, int v)
{
  int k;

  for( k=0; k<3; k++ )
    if( _zTriangulateVert(ws,t,k) == v ) return k;
  return -1;
}

/* replace a reference to an adjacent triangle. */
static void _zTriangulateReplaceAdj(_zTriangulateWS *ws, int t, int from, int to)
{
  int k;

  if( t < 0 ) return;
  for( k=0; k<3; k++ )
    if( _zTriangulateAdj(ws,t,k) == from ){
      _zTriangulateAdj(ws,t,k) = to;
      return;
    }
}

/* flip the diagonal across the k-th edge of a triangle t if it is not locally Delaunay. Edges
 * of the polygon are never flipped, since they have no adjacent triangle.
 * Let t be a->b->c and the adjacent triangle s be b->a->d. The diagonal ab is replaced with dc,
 * and t and s become a->d->c and d->b->c, respectively.
 *
 *        c                c
 *       / \              /|\
 *      / t \            / | \
 *     a-----b   -->    a t|s b
 *      \ s /            \ | /
 *       \ /              \|/
 *        d                d
 *
 * The triangles adjacent across ad and bc are switched between t and s. */
static bool _zTriangulateFlip(_zTriangulateWS *ws, int t, int k)
{
  int s, l, a, b, c, d, adj_bc, adj_ca, adj_ad, adj_db;

  if( ( s = _zTriangulateAdj(ws,t,k) ) < 0 ) return false;
  a = _zTriangulateVert(ws,t,k);
  b = _zTriangulateVert(ws,t,k+1);
  c = _zTriangulateVert(ws,t,k+2);
  if( ( l = _zTriangulateVertID( ws, s, b ) ) < 0 ) return false; /* s is b->a->d from the l-th vertex */
  d = _zTriangulateVert(ws,s,l+2);
  if( !_zTriangulateInCircle( ws, a, b, c, d ) ) return false;
  /* the quadrilateral a-d-b-c has to be convex */
  if( _zTriangulateArea( ws, a, d, c ) <= ws->eps || _zTriangulateArea( ws, d, b, c ) <= ws->eps ) return false;
  adj_bc = _zTriangulateAdj(ws,t,k+1);
  adj_ca = _zTriangulateAdj(ws,t,k+2);
  adj_ad = _zTriangulateAdj(ws,s,l+1);
  adj_db = _zTriangulateAdj(ws,s,l+2);
  _zTriangulateSetTri( ws, t, a, d, c, adj_ad, s, adj_ca );
  _zTriangulateSetTri( ws, s, d, b, c, adj_db, adj_bc, t );
  _zTriangulateReplaceAdj( ws, adj_ad, s, t );
  _zTriangulateReplaceAdj( ws, adj_bc, t, s );
  return true;
}

/* flip diagonals until every diagonal is locally Delaunay. */
static void _zTriangulateDelaunay(_zTriangulateWS *ws)
{
  int i, k, iter;
  bool flipped;

  _zTriangulateFindAdj( ws );
  for( iter=0; iter<Z_MAX_ITER_NUM; iter++ ){
    for( flipped=false, i=0; i<ws->nt; i++ )
      for( k=0; k<3; k++ )
        if( _zTriangulateFlip( ws, i, k ) ) flipped = true;
    if( !flipped ) break;
  }
}


/* triangulate a simple polygon given by an array of pointers to vertices. */
static int _zTriangulate(_zTriangulateWS *ws, zTri3DList *tlist)
{
  zTri3D t;
  int i;

  if( !_zTriangulateProject( ws ) ) return 0;
  _zTriangulateCutEar( ws );
  _zTriangulateDelaunay( ws );
  for( i=0; i<ws->nt; i++ ){
    zTri3DCreate( &t, ws->v[ws->tri[i][0]], ws->v[ws->tri[i][1]], ws->v[ws->tri[i][2]] );
    if( !zTri3DListAdd( tlist, &t ) ) break;
  }
  return zListSize(tlist);
}

/* triangulate a simple loop of vertices. */
int zLoop3DTriangulate(const zLoop3D *loop, zTri3DList *tlist)
{
  _zTriangulateWS ws;
  zLoop3DCell *cp;
  int i = 0, ret = 0;

  zListInit( tlist );
  if( zListSize(loop) < 3 ) return 0;
  if( _zTriangulateWSAlloc( &ws, zListSize(loop) ) ){
    zListForEach( loop, cp ) ws.v[i++] = cp->data;
    ret = _zTriangulate( &ws, tlist );
  }
  _zTriangulateWSFree( &ws );
  return ret;
}

/* triangulate a simple polygon given by an array of vertices. */
int zTriangulate(const zVec3DArray *array, zTri3DList *tlist)
{
  _zTriangulateWS ws;
  int i, ret = 0;

  zListInit( tlist );
  if( zArraySize(array) < 3 ) return 0;
  if( _zTriangulateWSAlloc( &ws, zArraySize(array) ) ){
    for( i=0; i<zArraySize(array); i++ ) ws.v[i] = zArrayElemNC(array,i);
    ret = _zTriangulate( &ws, tlist );
  }
  _zTriangulateWSFree( &ws );
  return ret;
}

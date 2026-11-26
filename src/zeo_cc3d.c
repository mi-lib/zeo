/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_cc3d - 3D cell complex.
 */

#include <zeo/zeo_cc3d.h>
#include <zeo/zeo_col.h>

/* Data structure:
 * Each face has two sides. The half-edges on side 0 make a loop counterclockwise about the normal
 * vector of the face, which directs from cell[0] to cell[1], and those on side 1 make the reversed
 * loop. Namely, the half-edges on each side are ordered outward for the cell on that side. The
 * half-edges of a cell make a closed polyhedral surface, in which every half-edge has a twin on
 * the adjacent face. Every half-edge also has a mate on the other side of the same face unless the
 * face is on the boundary of the universe. Hence, the faces around an edge are traversed by
 * alternating twins and mates. A face on the boundary of the universe has the cell inside on side 0
 * and no cell on side 1.
 *
 * Cut of a cell by a plane:
 * It is based on the topology-oriented method (Sugihara et al., 2000). The vertices of the cell are
 * divided into those in front of the plane (V1) and those behind it (V2) so that both V1 and V2 are
 * connected. Then, the edges between V1 and V2 are divided, the faces across them are split, and
 * the cutting face is made by connecting the new vertices. Whatever the numerical values are, the
 * result is combinatorially consistent as far as V1 and V2 are connected.
 * The connectivity is examined on the graph of corners, namely, vertices that have three or more
 * neighbors in the cell, since vertices on edges divided by cuts of neighbor cells are not corners.
 * V1 is found by the breadth-first search from the farthest corner in front of the plane, which is
 * found by hill-climbing. The connectivity of V2 is confirmed by checking that the edges between V1
 * and V2 make a single loop. Hence, the computation is local around V1 in most cases.
 * Vertices on the plane, namely, those made by the cut of an adjacent cell by the same plane and
 * those within the tolerance from the plane, are reused as vertices of the cutting face. They are
 * regarded as behind the plane with the distance exactly zero, so that a duplicate vertex, an edge
 * of length zero and a cell of volume zero are not made. Such a vertex may be followed by two or
 * more vertices in front of the plane; the faces between them are left in front of the plane as
 * they are. If the reuse breaks the consistency in a numerically degenerate case, the vertices are
 * classified again by the distance computed numerically.
 */

/* ********************************************************** */
/* memory management
 * ********************************************************** */

/* accessors of vertices and half-edges. */
#define _zCC3DV(cc,i)     zArrayElemNC(&(cc)->vert,i)
#define _zCC3DHE(cc,i)    zArrayElemNC(&(cc)->he,i)
#define _zCC3DHENum(cc)   zArraySize(&(cc)->he)

/* append a cell to an array, which returns the new cell, or the null pointer if it fails. */
#define _zCC3DArrayPush(array,type,cell) do{ \
  int __n; \
  __n = zArraySize(array); \
  zArrayExpand( array, type, __n+1 ); \
  (cell) = zArraySize(array) == __n+1 ? zArrayElemNC(array,__n) : NULL; \
} while(0)

/* expand the capacities of the full arrays of a cell complex for one more cell, where the sizes
 * are unchanged. It is shared by the arrays, so that the functions to add a cell are inlined. */
#define _zCC3DArrayReserve(array,type) do{ \
  int __n; \
  if( ( __n = zArraySize(array) ) == zArrayCapacity(array) ){ \
    zArrayExpand( array, type, __n+1 ); \
    if( zArraySize(array) != __n+1 ) return false; \
    zArraySetSize( array, __n ); \
  } \
} while(0)
static bool _zCC3DReserve(zCC3D *cc)
{
  _zCC3DArrayReserve( &cc->vert, zCC3DVert );
  _zCC3DArrayReserve( &cc->he,   zCC3DHalfEdge );
  _zCC3DArrayReserve( &cc->face, zCC3DFace );
  _zCC3DArrayReserve( &cc->cell, zCC3DCell );
  return true;
}

/* append a cell to an array of a cell complex, which is frequently used. */
#define _zCC3DPush(cc,array,cell) do{ \
  if( zArraySize(array) == zArrayCapacity(array) && !_zCC3DReserve( cc ) ){ \
    (cell) = NULL; \
    break; \
  } \
  zArraySetSize( array, zArraySize(array)+1 ); \
  (cell) = zArrayHead(array); \
} while(0)

/* push an integer to an array, which is a macro to be expanded inline in frequent uses, where the
 * array and the value are evaluated more than once. The expansion of the array is separated, so that
 * the code expanded inline is small. */
static bool _zCC3DIndexPushExpand(zIndexStruct *v, int val)
{
  int *p;

  _zCC3DArrayPush( v, int, p );
  if( !p ) return false;
  *p = val;
  return true;
}
#define _zCC3DIndexPush(v,val) ( zArraySize(v) < zArrayCapacity(v) ? \
  ( zIndexSetElemNC( v, zArraySize(v), val ), zArraySetSize( v, zArraySize(v)+1 ), true ) : \
  _zCC3DIndexPushExpand( v, val ) )

/* workspaces in a cell complex */
enum{
  _ZEO_CC3D_WS_V1 = 0,
  _ZEO_CC3D_WS_IN,
  _ZEO_CC3D_WS_LOOP,
  _ZEO_CC3D_WS_BUF,
  _ZEO_CC3D_WS_X,
  _ZEO_CC3D_WS_K0,
  _ZEO_CC3D_WS_K1,
  _ZEO_CC3D_WS_QUEUE,
};

/* a workspace, which is emptied. */
static zIndexStruct *_zCC3DWS(zCC3D *cc, int i)
{
  zArraySetSize( &cc->_ws[i], 0 );
  return &cc->_ws[i];
}

/* add a vertex. */
static int _zCC3DAddVert(zCC3D *cc, const zVec3D *p)
{
  zCC3DVert *v;

  _zCC3DPush( cc, &cc->vert, v );
  if( !v ) return -1;
  zVec3DCopy( p, &v->p );
  v->_stamp = v->_visit = v->_on = 0;
  v->_side = 0;
  return zCC3DVertNum(cc) - 1;
}

/* add a half-edge. */
static int _zCC3DAddHalfEdge(zCC3D *cc, int org, int face, int side)
{
  zCC3DHalfEdge *h;

  _zCC3DPush( cc, &cc->he, h );
  if( !h ) return -1;
  h->org = org;
  h->next = h->prev = h->twin = h->mate = -1;
  h->face = face;
  h->side = side;
  h->_aux = 0;
  return _zCC3DHENum(cc) - 1;
}

/* add a face. */
static int _zCC3DAddFace(zCC3D *cc, int cell0, int cell1, int tag)
{
  zCC3DFace *f;

  _zCC3DPush( cc, &cc->face, f );
  if( !f ) return -1;
  f->cell[0] = cell0;
  f->cell[1] = cell1;
  f->he[0] = f->he[1] = -1;
  f->tag = tag;
  return zCC3DFaceNum(cc) - 1;
}

/* add a cell. */
static int _zCC3DAddCell(zCC3D *cc, bool solid)
{
  zCC3DCell *c;

  _zCC3DPush( cc, &cc->cell, c );
  if( !c ) return -1;
  c->he = -1;
  c->solid = solid;
  zAABox3DInit( &c->aabb );
  c->_cand = false;
  return zCC3DCellNum(cc) - 1;
}

/* initialize a 3D cell complex. */
zCC3D *zCC3DInit(zCC3D *cc)
{
  int i;

  zAABox3DInit( &cc->universe );
  cc->tol = 0;
  zArrayInit( &cc->vert );
  zArrayInit( &cc->he );
  zArrayInit( &cc->face );
  zArrayInit( &cc->cell );
  cc->_stamp = 0;
  cc->_tag = 0;
  cc->_plane = 0;
  cc->_useon = true;
  for( i=0; i<8; i++ ) cc->_corner[i] = -1;
  for( i=0; i<_ZEO_CC3D_WS_NUM; i++ ) zArrayInit( &cc->_ws[i] );
  return cc;
}

/* destroy a 3D cell complex. */
void zCC3DDestroy(zCC3D *cc)
{
  int i;

  for( i=0; i<_ZEO_CC3D_WS_NUM; i++ ) zArrayFree( &cc->_ws[i] );
  zArrayFree( &cc->vert );
  zArrayFree( &cc->he );
  zArrayFree( &cc->face );
  zArrayFree( &cc->cell );
  zCC3DInit( cc );
}

/* create a 3D cell complex of the universe. */
zCC3D *zCC3DCreate(zCC3D *cc, const zAABox3D *universe)
{
  /* vertex i is at (i&1 ? xmax : xmin, i&2 ? ymax : ymin, i&4 ? zmax : zmin) */
  static const int facevert[6][4] = {
    { 0, 4, 6, 2 }, { 1, 3, 7, 5 }, { 0, 1, 5, 4 }, { 2, 6, 7, 3 }, { 0, 2, 3, 1 }, { 4, 5, 7, 6 },
  };
  zVec3D p;
  double scale;
  int i, j, k, h;

  zCC3DInit( cc );
  if( !( universe->min.c.x < universe->max.c.x && universe->min.c.y < universe->max.c.y && universe->min.c.z < universe->max.c.z ) ){
    ZRUNERROR( ZEO_ERR_ELEM_DEG_BOX );
    return NULL;
  }
  zCopy( zAABox3D, universe, &cc->universe );
  for( scale=0, i=0; i<3; i++ ){
    scale = zMax( scale, fabs( universe->min.e[i] ) );
    scale = zMax( scale, fabs( universe->max.e[i] ) );
    scale = zMax( scale, universe->max.e[i] - universe->min.e[i] );
  }
  cc->tol = zTOL * scale;
  for( i=0; i<8; i++ ){
    zVec3DCreate( &p, i & 1 ? universe->max.c.x : universe->min.c.x,
                      i & 2 ? universe->max.c.y : universe->min.c.y,
                      i & 4 ? universe->max.c.z : universe->min.c.z );
    if( _zCC3DAddVert( cc, &p ) < 0 ) goto FAILURE;
  }
  if( _zCC3DAddCell( cc, false ) < 0 ) goto FAILURE;
  zCopy( zAABox3D, universe, &zCC3DCell(cc,0)->aabb );
  for( i=0; i<6; i++ ){
    if( _zCC3DAddFace( cc, 0, -1, 0 ) < 0 ) goto FAILURE;
    for( j=0; j<4; j++ )
      if( _zCC3DAddHalfEdge( cc, facevert[i][j], i, 0 ) < 0 ) goto FAILURE;
    for( j=0; j<4; j++ ){
      h = 4*i + j;
      _zCC3DHE(cc,h)->next = 4*i + (j+1)%4;
      _zCC3DHE(cc,h)->prev = 4*i + (j+3)%4;
    }
    zCC3DFace(cc,i)->he[0] = 4*i;
  }
  for( h=0; h<24; h++ ) /* twins */
    for( k=0; k<24; k++ )
      if( _zCC3DHE(cc,k)->org == _zCC3DHE(cc,_zCC3DHE(cc,h)->next)->org && _zCC3DHE(cc,_zCC3DHE(cc,k)->next)->org == _zCC3DHE(cc,h)->org ){
        _zCC3DHE(cc,h)->twin = k;
        break;
      }
  zCC3DCell(cc,0)->he = 0;
  for( i=0; i<8; i++ ) cc->_corner[i] = i;
  return cc;

 FAILURE:
  zCC3DDestroy( cc );
  return NULL;
}

/* ********************************************************** */
/* cut of a cell by a plane
 * ********************************************************** */

#define _zCC3DTarget(cc,h)   _zCC3DHE(cc,_zCC3DHE(cc,h)->next)->org
#define _zCC3DRotate(cc,h)   _zCC3DHE(cc,_zCC3DHE(cc,h)->prev)->twin /* next outgoing half-edge around the origin */

/* check if a vertex is regarded as on the current cutting plane. */
#define _zCC3DIsOn(cc,v) ( (cc)->_useon && _zCC3DV(cc,v)->_on == (cc)->_plane )

/* signed distance of a vertex from a plane. A vertex within the tolerance is marked as on the plane. */
static double _zCC3DDist(zCC3D *cc, int v, const zPlane3D *plane)
{
  zVec3D d;

  double s;

  if( _zCC3DIsOn( cc, v ) ) return 0;
  zVec3DSub( &_zCC3DV(cc,v)->p, zPlane3DOrg(plane), &d );
  s = zVec3DInnerProd( &d, zPlane3DNorm(plane) );
  if( cc->_useon && fabs( s ) <= cc->tol ){ /* regarded as on the plane */
    _zCC3DV(cc,v)->_on = cc->_plane;
    return 0;
  }
  return s;
}

/* check if the origin of a half-edge is a corner, namely, has three or more neighbors. */
static bool _zCC3DIsCorner(zCC3D *cc, int h)
{
  int g, n = 0;

  g = h;
  do{
    if( ++n >= 3 ) return true;
    g = _zCC3DRotate( cc, g );
  } while( g != h );
  return false;
}

/* the last half-edge of a chain from a corner, which ends at a corner. */
static int _zCC3DChainEnd(zCC3D *cc, int h)
{
  while( !_zCC3DIsCorner( cc, _zCC3DHE(cc,h)->next ) ) h = _zCC3DHE(cc,h)->next;
  return h;
}

/* find a half-edge whose origin is the farthest (sign=1) or the nearest (sign=-1) vertex of a cell
 * from a plane by hill-climbing. Since the cell is convex, a vertex none of whose neighbors is
 * farther is the farthest. Vertices at the same distance within the tolerance, which lie on a face
 * or an edge parallel to the plane or on tiny edges made by numerical errors, are searched
 * breadth-first to find a farther neighbor. */
static int _zCC3DClimb(zCC3D *cc, int h, const zPlane3D *plane, double sign, double *s, zIndexStruct *queue)
{
  int i, g, hp, best, w;
  double sv, sw, sb;

  cc->_stamp++;
  _zCC3DV(cc,_zCC3DHE(cc,h)->org)->_visit = cc->_stamp;
  sv = sign * _zCC3DDist( cc, _zCC3DHE(cc,h)->org, plane );
  zArraySetSize( queue, 0 );
  if( !_zCC3DIndexPush( queue, h ) ) return -1;
  for( i=0; i<zArraySize(queue); i++ ){
    hp = zIndexElemNC(queue,i);
    best = -1;
    sb = sv;
    g = hp;
    do{
      if( ( sw = sign * _zCC3DDist( cc, _zCC3DTarget( cc, g ), plane ) ) > sb ){
        sb = sw;
        best = g;
      }
      g = _zCC3DRotate( cc, g );
    } while( g != hp );
    if( best >= 0 ){ /* climb up, which never makes a loop since the distance strictly increases */
      h = _zCC3DHE(cc,best)->next;
      _zCC3DV(cc,_zCC3DHE(cc,h)->org)->_visit = ++cc->_stamp; /* plateau is searched again at the new level */
      sv = sb;
      zArraySetSize( queue, 0 );
      if( !_zCC3DIndexPush( queue, h ) ) return -1;
      i = -1;
      continue;
    }
    /* plateau including tiny edges made by numerical errors */
    g = hp;
    do{
      w = _zCC3DTarget( cc, g );
      if( _zCC3DV(cc,w)->_visit != cc->_stamp && sign * _zCC3DDist( cc, w, plane ) >= sv - cc->tol ){
        _zCC3DV(cc,w)->_visit = cc->_stamp;
        if( !_zCC3DIndexPush( queue, _zCC3DHE(cc,g)->next ) ) return -1;
      }
      g = _zCC3DRotate( cc, g );
    } while( g != hp );
  }
  *s = sign * sv;
  return h;
}

/* move from a vertex to the corner of the chain on which it lies with the farther (sign=1) or
 * nearer (sign=-1) distance from a plane. */
static int _zCC3DToCorner(zCC3D *cc, int h, const zPlane3D *plane, double sign, double *s)
{
  int h1, h2;
  double s1, s2;

  if( _zCC3DIsCorner( cc, h ) ){
    *s = _zCC3DDist( cc, _zCC3DHE(cc,h)->org, plane );
    return h;
  }
  h1 = _zCC3DHE(cc,_zCC3DChainEnd( cc, h ))->next;
  h2 = _zCC3DHE(cc,_zCC3DChainEnd( cc, _zCC3DRotate( cc, h ) ))->next;
  s1 = _zCC3DDist( cc, _zCC3DHE(cc,h1)->org, plane );
  s2 = _zCC3DDist( cc, _zCC3DHE(cc,h2)->org, plane );
  if( sign * s1 >= sign * s2 ){
    *s = s1;
    return h1;
  }
  *s = s2;
  return h2;
}

/* workspace for a cut. */
typedef struct{
  int stamp;          /* stamp of V1 */
  zIndexStruct *v1;   /* outgoing half-edges of corners in V1 */
  zIndexStruct *in;   /* half-edges entering V1 from V2 */
  zIndexStruct *loop; /* half-edges entering V1 in the order of the loop */
  zIndexStruct *buf;  /* buffer */
} _zCC3DCutWS;

static void _zCC3DCutWSInit(zCC3D *cc, _zCC3DCutWS *ws)
{
  ws->stamp = 0;
  ws->v1 = _zCC3DWS( cc, _ZEO_CC3D_WS_V1 );
  ws->in = _zCC3DWS( cc, _ZEO_CC3D_WS_IN );
  ws->loop = _zCC3DWS( cc, _ZEO_CC3D_WS_LOOP );
  zArrayBuf(ws) = _zCC3DWS( cc, _ZEO_CC3D_WS_BUF );
}

#define _zCC3DIsV1(cc,ws,v) ( _zCC3DV(cc,v)->_stamp == (ws)->stamp && _zCC3DV(cc,v)->_side == 1 )
#define _zCC3DSetV1(cc,ws,v) do{ _zCC3DV(cc,v)->_stamp = (ws)->stamp; _zCC3DV(cc,v)->_side = 1; } while(0)

/* find corners in V1 connected to a corner by the breadth-first search. */
static bool _zCC3DFindV1(zCC3D *cc, _zCC3DCutWS *ws, int h, const zPlane3D *plane)
{
  int i, g, e, b;

  _zCC3DSetV1( cc, ws, _zCC3DHE(cc,h)->org );
  if( !_zCC3DIndexPush( ws->v1, h ) ) return false;
  for( i=0; i<zArraySize(ws->v1); i++ ){
    g = h = zIndexElemNC(ws->v1,i);
    do{
      e = _zCC3DChainEnd( cc, g );
      b = _zCC3DTarget( cc, e );
      if( !_zCC3DIsV1( cc, ws, b ) && _zCC3DDist( cc, b, plane ) > cc->tol ){
        _zCC3DSetV1( cc, ws, b );
        if( !_zCC3DIndexPush( ws->v1, _zCC3DHE(cc,e)->next ) ) return false;
      }
      g = _zCC3DRotate( cc, g );
    } while( g != h );
  }
  return true;
}

/* classify vertices on chains from corners in V1, and find half-edges entering V1 from V2. */
static bool _zCC3DFindBoundary(zCC3D *cc, _zCC3DCutWS *ws, const zPlane3D *plane)
{
  int i, j, k, g, h, b;

  zArraySetSize( ws->in, 0 );
  for( i=0; i<zArraySize(ws->v1); i++ ){
    g = h = zIndexElemNC(ws->v1,i);
    do{
      /* half-edges on the chain */
      zArraySize(zArrayBuf(ws)) = 0;
      for( k=g; ; k=_zCC3DHE(cc,k)->next ){
        if( !_zCC3DIndexPush( zArrayBuf(ws), k ) ) return false;
        if( _zCC3DIsCorner( cc, _zCC3DHE(cc,k)->next ) ) break;
      }
      b = _zCC3DTarget( cc, k );
      if( _zCC3DIsV1( cc, ws, b ) ){
        for( j=0; j<zArraySize(zArrayBuf(ws))-1; j++ ) _zCC3DSetV1( cc, ws, _zCC3DTarget( cc, zIndexElemNC(ws->buf,j) ) );
      } else{
        /* the chain crosses the plane at the first vertex in front of it from V2 */
        for( k=zArraySize(zArrayBuf(ws))-1; k>0; k-- )
          if( _zCC3DDist( cc, _zCC3DHE(cc,zIndexElemNC(ws->buf,k))->org, plane ) > 0 ) break;
        for( j=0; j<k; j++ ) _zCC3DSetV1( cc, ws, _zCC3DTarget( cc, zIndexElemNC(ws->buf,j) ) );
        if( !_zCC3DIndexPush( ws->in, _zCC3DHE(cc,zIndexElemNC(ws->buf,k))->twin ) ) return false;
      }
      g = _zCC3DRotate( cc, g );
    } while( g != h );
  }
  return true;
}

/* the next half-edge entering V1 on the loop of the boundary between V1 and V2. */
static int _zCC3DNextEntering(zCC3D *cc, _zCC3DCutWS *ws, int e)
{
  int f;

  for( f=_zCC3DHE(cc,e)->next; _zCC3DIsV1( cc, ws, _zCC3DTarget( cc, f ) ); f=_zCC3DHE(cc,f)->next );
  return _zCC3DHE(cc,f)->twin;
}

/* trace the loop of the boundary between V1 and V2. */
static bool _zCC3DTraceLoop(zCC3D *cc, _zCC3DCutWS *ws, bool *single)
{
  int i, e, stamp;

  *single = false;
  stamp = ++cc->_stamp;
  for( i=0; i<zArraySize(ws->in); i++ ) _zCC3DHE(cc,zIndexElemNC(ws->in,i))->_aux = stamp;
  zArraySetSize( ws->loop, 0 );
  if( zArraySize(ws->in) < 3 ) return true;
  e = zIndexElemNC(ws->in,0);
  do{
    if( _zCC3DHE(cc,e)->_aux != stamp || zArraySize(ws->loop) >= zArraySize(ws->in) ) return true;
    if( !_zCC3DIndexPush( ws->loop, e ) ) return false;
    e = _zCC3DNextEntering( cc, ws, e );
  } while( e != zIndexElemNC(ws->in,0) );
  *single = zArraySize(ws->loop) == zArraySize(ws->in);
  return true;
}

/* make V2 connected by moving corners not connected to the nearest corner into V1. This is
 * required only in numerically inconsistent cases. */
static bool _zCC3DMakeV2Connected(zCC3D *cc, _zCC3DCutWS *ws, int hmin)
{
  zIndexStruct *queue;
  int i, g, h, e, b, visit;
  bool ret = false;

  queue = _zCC3DWS( cc, _ZEO_CC3D_WS_QUEUE );
  /* corners in V2 connected to the nearest corner */
  visit = ++cc->_stamp;
  _zCC3DV(cc,_zCC3DHE(cc,hmin)->org)->_visit = visit;
  if( !_zCC3DIndexPush( queue, hmin ) ) goto TERMINATE;
  for( i=0; i<zArraySize(queue); i++ ){
    g = h = zIndexElemNC(queue,i);
    do{
      e = _zCC3DChainEnd( cc, g );
      b = _zCC3DTarget( cc, e );
      if( !_zCC3DIsV1( cc, ws, b ) && _zCC3DV(cc,b)->_visit != visit ){
        _zCC3DV(cc,b)->_visit = visit;
        if( !_zCC3DIndexPush( queue, _zCC3DHE(cc,e)->next ) ) goto TERMINATE;
      }
      g = _zCC3DRotate( cc, g );
    } while( g != h );
  }
  /* the other corners are moved to V1 */
  zArraySetSize( queue, 0 );
  if( !_zCC3DIndexPush( queue, hmin ) ) goto TERMINATE;
  visit = ++cc->_stamp; /* visit of half-edges */
  _zCC3DHE(cc,hmin)->_aux = visit;
  for( i=0; i<zArraySize(queue); i++ ){
    h = zIndexElemNC(queue,i);
    if( !_zCC3DIsV1( cc, ws, _zCC3DHE(cc,h)->org ) && _zCC3DV(cc,_zCC3DHE(cc,h)->org)->_visit != visit - 1 &&
        _zCC3DIsCorner( cc, h ) ){
      _zCC3DSetV1( cc, ws, _zCC3DHE(cc,h)->org );
      if( !_zCC3DIndexPush( ws->v1, h ) ) goto TERMINATE;
    }
    g = _zCC3DHE(cc,h)->next;
    if( _zCC3DHE(cc,g)->_aux != visit ){
      _zCC3DHE(cc,g)->_aux = visit;
      if( !_zCC3DIndexPush( queue, g ) ) goto TERMINATE;
    }
    g = _zCC3DHE(cc,h)->twin;
    if( _zCC3DHE(cc,g)->_aux != visit ){
      _zCC3DHE(cc,g)->_aux = visit;
      if( !_zCC3DIndexPush( queue, g ) ) goto TERMINATE;
    }
  }
  ret = true;
 TERMINATE:
  return ret;
}

/* divide an edge by a vertex on all faces around the edge. */
static bool _zCC3DSplitEdge(zCC3D *cc, int e, int x, zIndexStruct *s)
{
  int i, h, t, n, tw, mt;

  /* half-edges around the edge */
  zArraySetSize( s, 0 );
  for( h=e; ; ){
    t = _zCC3DHE(cc,h)->twin;
    if( !_zCC3DIndexPush( s, h ) || !_zCC3DIndexPush( s, t ) ) return false;
    if( ( h = _zCC3DHE(cc,t)->mate ) < 0 || h == e ) break;
  }
  if( h < 0 )
    for( h=_zCC3DHE(cc,e)->mate; h>=0; h=_zCC3DHE(cc,t)->mate ){
      t = _zCC3DHE(cc,h)->twin;
      if( !_zCC3DIndexPush( s, h ) || !_zCC3DIndexPush( s, t ) ) return false;
    }
  /* divide half-edges */
  n = zArraySize(s);
  for( i=0; i<n; i++ ){
    h = zIndexElemNC(s,i);
    if( ( t = _zCC3DAddHalfEdge( cc, x, _zCC3DHE(cc,h)->face, _zCC3DHE(cc,h)->side ) ) < 0 ) return false;
    _zCC3DHE(cc,t)->next = _zCC3DHE(cc,h)->next;
    _zCC3DHE(cc,t)->prev = h;
    _zCC3DHE(cc,_zCC3DHE(cc,h)->next)->prev = t;
    _zCC3DHE(cc,h)->next = t;
    _zCC3DHE(cc,t)->twin = _zCC3DHE(cc,h)->twin; /* old twin and mate temporarily */
    _zCC3DHE(cc,t)->mate = _zCC3DHE(cc,h)->mate;
    _zCC3DHE(cc,h)->_aux = t;
  }
  for( i=0; i<n; i++ ){
    h = zIndexElemNC(s,i);
    t = _zCC3DHE(cc,h)->next;
    tw = _zCC3DHE(cc,t)->twin;
    mt = _zCC3DHE(cc,t)->mate;
    _zCC3DHE(cc,h)->twin = _zCC3DHE(cc,tw)->_aux;
    _zCC3DHE(cc,t)->twin = tw;
    if( mt >= 0 ){
      _zCC3DHE(cc,h)->mate = _zCC3DHE(cc,mt)->_aux;
      _zCC3DHE(cc,t)->mate = mt;
    }
  }
  for( i=0; i<n; i++ ) _zCC3DHE(cc,zIndexElemNC(s,i))->_aux = 0; /* not to be confused with stamps */
  return true;
}

/* update the cell of faces and the bounding box of a cell by traversing its half-edges. */
static bool _zCC3DUpdateCell(zCC3D *cc, int c)
{
  zIndexStruct *queue;
  zCC3DHalfEdge *h;
  zVec3D *p;
  int i, g, visit;
  bool ret = false;

  queue = _zCC3DWS( cc, _ZEO_CC3D_WS_QUEUE );
  visit = ++cc->_stamp;
  _zCC3DHE(cc,zCC3DCell(cc,c)->he)->_aux = visit;
  if( !_zCC3DIndexPush( queue, zCC3DCell(cc,c)->he ) ) goto TERMINATE;
  p = &_zCC3DV(cc,_zCC3DHE(cc,zCC3DCell(cc,c)->he)->org)->p;
  zVec3DCopy( p, &zCC3DCell(cc,c)->aabb.min );
  zVec3DCopy( p, &zCC3DCell(cc,c)->aabb.max );
  for( i=0; i<zArraySize(queue); i++ ){
    h = _zCC3DHE(cc,zIndexElemNC(queue,i));
    zCC3DFace(cc,h->face)->cell[h->side] = c;
    zAABox3DEnlarge( &zCC3DCell(cc,c)->aabb, &_zCC3DV(cc,h->org)->p );
    g = h->next;
    if( _zCC3DHE(cc,g)->_aux != visit ){
      _zCC3DHE(cc,g)->_aux = visit;
      if( !_zCC3DIndexPush( queue, g ) ) goto TERMINATE;
    }
    g = _zCC3DHE(cc,zIndexElemNC(queue,i))->twin;
    if( _zCC3DHE(cc,g)->_aux != visit ){
      _zCC3DHE(cc,g)->_aux = visit;
      if( !_zCC3DIndexPush( queue, g ) ) goto TERMINATE;
    }
  }
  ret = true;
 TERMINATE:
  return ret;
}

/* check if an entry of the loop and the next share a vertex on the cutting plane. */
static bool _zCC3DLoopIsDup(zCC3D *cc, _zCC3DCutWS *ws, int i)
{
  int u;

  u = _zCC3DHE(cc,zIndexElemNC(ws->loop,i))->org;
  return _zCC3DIsOn( cc, u ) && u == _zCC3DHE(cc,zIndexElemNC(ws->loop,(i+1)%zArraySize(ws->loop)))->org;
}

/* check if vertices on the cutting plane can be reused as vertices of the cutting face. The
 * entries of the loop from such a vertex have to be consecutive, the cutting face has to have three
 * or more vertices, and a face across an edge between two such vertices, which is an edge of the
 * cutting face, must be behind the plane. */
static bool _zCC3DCheckReuse(zCC3D *cc, _zCC3DCutWS *ws)
{
  int i, i0, k, m, n, u, h, g, visit;

  n = zArraySize(ws->loop);
  for( i0=0; i0<n && _zCC3DLoopIsDup( cc, ws, (i0+n-1)%n ); i0++ );
  if( i0 == n ) return false;
  visit = ++cc->_stamp;
  for( m=0, k=0; k<n; k++ ){
    i = ( i0 + k ) % n;
    if( !_zCC3DLoopIsDup( cc, ws, i ) ) m++;
    if( !_zCC3DIsOn( cc, ( u = _zCC3DHE(cc,zIndexElemNC(ws->loop,i))->org ) ) ) continue;
    if( k > 0 && _zCC3DLoopIsDup( cc, ws, (i+n-1)%n ) ) continue; /* in a run of the vertex */
    if( _zCC3DV(cc,u)->_visit == visit ) return false;
    _zCC3DV(cc,u)->_visit = visit;
  }
  if( m < 3 ) return false;
  for( i=0; i<n; i++ ){
    if( !_zCC3DIsOn( cc, _zCC3DHE(cc,zIndexElemNC(ws->loop,i))->org ) ||
        !_zCC3DIsOn( cc, _zCC3DHE(cc,zIndexElemNC(ws->loop,(i+1)%n))->org ) ||
        _zCC3DLoopIsDup( cc, ws, i ) ) continue;
    h = _zCC3DHE(cc,zIndexElemNC(ws->loop,i))->prev;
    if( _zCC3DHE(cc,_zCC3DHE(cc,zIndexElemNC(ws->loop,(i+1)%n))->twin)->next != h ) continue;
    g = _zCC3DHE(cc,h)->twin;
    do{
      if( _zCC3DIsV1( cc, ws, _zCC3DHE(cc,g)->org ) ) return false;
      g = _zCC3DHE(cc,g)->next;
    } while( g != _zCC3DHE(cc,h)->twin );
  }
  return true;
}

/* split a cell along the loop of the boundary between V1 and V2.
 * A vertex behind the plane on an edge entering V1 is reused as a vertex of the cutting face if it
 * was made on the same plane by a cut of a neighbor cell. Otherwise, the edge is divided. */
static int _zCC3DSplitCell(zCC3D *cc, int c, _zCC3DCutWS *ws, const zPlane3D *plane, int tag)
{
  zIndexStruct *x, *k0, *k1;
  zVec3D p;
  double su, sw, t;
  int n, ne, i, j, l, e, u, w, cf, cap, g, a, b, cb, cfh, nb, nf, sc, sn, f, m, q, mp, be, tb;
  bool ri, rj;
  int ret = -1;

  x = _zCC3DWS( cc, _ZEO_CC3D_WS_X );
  k0 = _zCC3DWS( cc, _ZEO_CC3D_WS_K0 );
  k1 = _zCC3DWS( cc, _ZEO_CC3D_WS_K1 );
  n = zArraySize(ws->loop);
  /* vertices of the cutting face on edges entering V1 */
  for( i=0; i<n; i++ ){
    e = zIndexElemNC(ws->loop,i);
    u = _zCC3DHE(cc,e)->org;
    if( _zCC3DIsOn( cc, u ) ){ /* reuse */
      if( !_zCC3DIndexPush( x, u ) ) goto TERMINATE;
      continue;
    }
    w = _zCC3DTarget( cc, e );
    su = _zCC3DDist( cc, u, plane );
    sw = _zCC3DDist( cc, w, plane );
    t = sw > su ? zLimit( -su / ( sw - su ), 0, 1 ) : 0.5;
    /* the coordinate shared by both ends is exactly kept, so that vertices on the boundary of the
     * universe are exactly on it */
    _zVec3DInterDiv( &_zCC3DV(cc,u)->p, &_zCC3DV(cc,w)->p, t, &p );
    if( ( j = _zCC3DAddVert( cc, &p ) ) < 0 || !_zCC3DIndexPush( x, j ) ||
        !_zCC3DSplitEdge( cc, e, j, zArrayBuf(ws) ) ) goto TERMINATE;
    _zCC3DV(cc,j)->_on = cc->_plane;
  }
  /* entries of the loop which make edges of the cutting face. An entry followed by the same
   * reused vertex is skipped, since the face across it is entirely in front of the plane except
   * the vertex. */
  zArraySetSize( ws->in, 0 );
  for( i=0; i<n; i++ )
    if( zIndexElemNC(x,i) != zIndexElemNC(x,(i+1)%n) && !_zCC3DIndexPush( ws->in, i ) ) goto TERMINATE;
  ne = zArraySize(ws->in);
  /* new cell and cutting face */
  if( ( cf = _zCC3DAddCell( cc, zCC3DCell(cc,c)->solid ) ) < 0 ||
      ( cap = _zCC3DAddFace( cc, c, cf, tag ) ) < 0 ) goto TERMINATE;
  for( l=0; l<ne; l++ ){
    i = zIndexElemNC(ws->in,l);
    if( ( j = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,(i+1)%n), cap, 0 ) ) < 0 || !_zCC3DIndexPush( k0, j ) ||
        ( j = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,i), cap, 1 ) ) < 0 || !_zCC3DIndexPush( k1, j ) ) goto TERMINATE;
  }
  for( l=0; l<ne; l++ ){
    /* side 0 of the cutting face: x[i+1] -> x[i], outward for the cell behind the plane */
    _zCC3DHE(cc,zIndexElemNC(k0,l))->next = zIndexElemNC(k0,(l+ne-1)%ne);
    _zCC3DHE(cc,zIndexElemNC(k0,l))->prev = zIndexElemNC(k0,(l+1)%ne);
    _zCC3DHE(cc,zIndexElemNC(k0,l))->mate = zIndexElemNC(k1,l);
    /* side 1 of the cutting face: x[i] -> x[i+1], outward for the cell in front of the plane */
    _zCC3DHE(cc,zIndexElemNC(k1,l))->next = zIndexElemNC(k1,(l+1)%ne);
    _zCC3DHE(cc,zIndexElemNC(k1,l))->prev = zIndexElemNC(k1,(l+ne-1)%ne);
    _zCC3DHE(cc,zIndexElemNC(k1,l))->mate = zIndexElemNC(k0,l);
  }
  zCC3DFace(cc,cap)->he[0] = zIndexElemNC(k0,0);
  zCC3DFace(cc,cap)->he[1] = zIndexElemNC(k1,0);
  /* split faces across the loop */
  for( l=0; l<ne; l++ ){
    i = zIndexElemNC(ws->in,l);
    e = zIndexElemNC(ws->loop,i);
    ri = zIndexElemNC(x,i) == _zCC3DHE(cc,e)->org;
    rj = zIndexElemNC(x,(i+1)%n) == _zCC3DHE(cc,zIndexElemNC(ws->loop,(i+1)%n))->org;
    if( ri ){
      a = e;                           /* x[i] -> w: the first half-edge in front of the plane */
      be = _zCC3DHE(cc,e)->prev;             /* -> x[i]: the last half-edge behind the plane */
    } else{
      be = e;
      a = _zCC3DHE(cc,e)->next;
    }
    tb = _zCC3DHE(cc,zIndexElemNC(ws->loop,(i+1)%n))->twin;
    if( rj ){
      b = tb;                          /* w' -> x[i+1]: the last half-edge in front of the plane */
      q = _zCC3DHE(cc,tb)->next;             /* x[i+1] -> : the first half-edge behind the plane */
    } else{
      q = tb;
      b = _zCC3DHE(cc,tb)->prev;
    }
    if( q == be ){ /* the edge x[i+1] -> x[i] becomes an edge of the cutting face */
      tb = _zCC3DHE(cc,be)->twin;
      _zCC3DHE(cc,tb)->twin = zIndexElemNC(k0,l); _zCC3DHE(cc,zIndexElemNC(k0,l))->twin = tb;
      _zCC3DHE(cc,be)->twin = zIndexElemNC(k1,l); _zCC3DHE(cc,zIndexElemNC(k1,l))->twin = be;
      continue;
    }
    f = _zCC3DHE(cc,e)->face;
    sc = _zCC3DHE(cc,e)->side;
    sn = 1 - sc;
    if( ( g = _zCC3DAddFace( cc, sc == 0 ? cf : zCC3DFace(cc,f)->cell[0], sc == 1 ? cf : zCC3DFace(cc,f)->cell[1], zCC3DFace(cc,f)->tag ) ) < 0 ) goto TERMINATE;
    /* chords on the side of the cell */
    if( ( cb = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,i), f, sc ) ) < 0 ||
        ( cfh = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,(i+1)%n), g, sc ) ) < 0 ) goto TERMINATE;
    _zCC3DHE(cc,be)->next = cb; _zCC3DHE(cc,cb)->prev = be; _zCC3DHE(cc,cb)->next = q; _zCC3DHE(cc,q)->prev = cb;
    _zCC3DHE(cc,b)->next = cfh; _zCC3DHE(cc,cfh)->prev = b; _zCC3DHE(cc,cfh)->next = a; _zCC3DHE(cc,a)->prev = cfh;
    for( m=a; m!=cfh; m=_zCC3DHE(cc,m)->next ) _zCC3DHE(cc,m)->face = g;
    _zCC3DHE(cc,cfh)->face = g;
    _zCC3DHE(cc,cb)->twin = zIndexElemNC(k0,l); _zCC3DHE(cc,zIndexElemNC(k0,l))->twin = cb;
    _zCC3DHE(cc,cfh)->twin = zIndexElemNC(k1,l); _zCC3DHE(cc,zIndexElemNC(k1,l))->twin = cfh;
    zCC3DFace(cc,f)->he[sc] = cb;
    zCC3DFace(cc,g)->he[sc] = cfh;
    /* chords on the other side of the face */
    if( zCC3DFace(cc,f)->cell[sn] >= 0 ){
      if( ( nb = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,(i+1)%n), f, sn ) ) < 0 ||
          ( nf = _zCC3DAddHalfEdge( cc, zIndexElemNC(x,i), g, sn ) ) < 0 ) goto TERMINATE;
      mp = _zCC3DHE(cc,q)->mate;             /* -> x[i+1] */
      m = _zCC3DHE(cc,be)->mate;             /* x[i] -> */
      _zCC3DHE(cc,mp)->next = nb; _zCC3DHE(cc,nb)->prev = mp; _zCC3DHE(cc,nb)->next = m; _zCC3DHE(cc,m)->prev = nb;
      _zCC3DHE(cc,_zCC3DHE(cc,a)->mate)->next = nf; _zCC3DHE(cc,nf)->prev = _zCC3DHE(cc,a)->mate;
      _zCC3DHE(cc,nf)->next = _zCC3DHE(cc,b)->mate; _zCC3DHE(cc,_zCC3DHE(cc,b)->mate)->prev = nf;
      for( m=nf; ; ){
        _zCC3DHE(cc,m)->face = g;
        if( ( m = _zCC3DHE(cc,m)->next ) == nf ) break;
      }
      _zCC3DHE(cc,nb)->twin = nf; _zCC3DHE(cc,nf)->twin = nb;
      _zCC3DHE(cc,nb)->mate = cb; _zCC3DHE(cc,cb)->mate = nb;
      _zCC3DHE(cc,nf)->mate = cfh; _zCC3DHE(cc,cfh)->mate = nf;
      zCC3DFace(cc,f)->he[sn] = nb;
      zCC3DFace(cc,g)->he[sn] = nf;
    }
  }
  /* cells */
  zCC3DCell(cc,c)->he = zIndexElemNC(k0,0);
  zCC3DCell(cc,cf)->he = zIndexElemNC(k1,0);
  if( !_zCC3DUpdateCell( cc, cf ) ) goto TERMINATE;
  for( i=0; i<n; i++ ) zAABox3DEnlarge( &zCC3DCell(cc,c)->aabb, &_zCC3DV(cc,zIndexElemNC(x,i))->p );
  ret = cf;
 TERMINATE:
  return ret;
}

/* classify vertices of a cell into V1 and V2, and find the loop of the boundary between them. */
static bool _zCC3DClassify(zCC3D *cc, int c, _zCC3DCutWS *ws, int hmax, const zPlane3D *plane)
{
  int hmin;
  double smin;
  bool single;

  ws->stamp = ++cc->_stamp;
  zArraySetSize( ws->v1, 0 );
  if( !_zCC3DFindV1( cc, ws, hmax, plane ) ||
      !_zCC3DFindBoundary( cc, ws, plane ) ||
      !_zCC3DTraceLoop( cc, ws, &single ) ) return false;
  if( !single ){
    if( ( hmin = _zCC3DClimb( cc, zCC3DCell(cc,c)->he, plane, -1, &smin, zArrayBuf(ws) ) ) < 0 ) return false;
    hmin = _zCC3DToCorner( cc, hmin, plane, -1, &smin );
    if( !_zCC3DMakeV2Connected( cc, ws, hmin ) ||
        !_zCC3DFindBoundary( cc, ws, plane ) ||
        !_zCC3DTraceLoop( cc, ws, &single ) ) return false;
    if( !single ){
      ZRUNERROR( ZEO_ERR_CC3D_INCONSISTENT );
      return false;
    }
  }
  return true;
}

/* cut a cell by a plane.
 * returns 0 if the cell is behind the plane, 1 if in front of it, 2 if it is split (the new cell
 * in front of the plane is stored in *cf), and -1 if failed. */
static int _zCC3DSplit(zCC3D *cc, int c, const zPlane3D *plane, int tag, int *cf)
{
  _zCC3DCutWS ws;
  int hmax, hmin, ret = -1;
  double smax, smin;

  zAABox3DDistRangeFromPlane( &zCC3DCell(cc,c)->aabb, plane, &smin, &smax );
  if( smax <= cc->tol ) return 0;
  if( smin >= -cc->tol ) return 1;
  _zCC3DCutWSInit( cc, &ws );
  if( ( hmax = _zCC3DClimb( cc, zCC3DCell(cc,c)->he, plane, 1, &smax, zArrayBuf(&ws) ) ) < 0 ) goto TERMINATE;
  hmax = _zCC3DToCorner( cc, hmax, plane, 1, &smax );
  if( smax <= cc->tol ){
    ret = 0;
    goto TERMINATE;
  }
  /* the nearest vertex is necessary only if the start vertex is not behind the plane */
  hmin = zCC3DCell(cc,c)->he;
  if( ( smin = _zCC3DDist( cc, _zCC3DHE(cc,hmin)->org, plane ) ) >= -cc->tol &&
      ( hmin = _zCC3DClimb( cc, hmin, plane, -1, &smin, zArrayBuf(&ws) ) ) < 0 ) goto TERMINATE;
  if( smin >= -cc->tol ){
    ret = 1;
    goto TERMINATE;
  }
  if( !_zCC3DClassify( cc, c, &ws, hmax, plane ) ) goto TERMINATE;
  if( !_zCC3DCheckReuse( cc, &ws ) ){
    /* numerically inconsistent case: classify again without vertices on the plane */
    cc->_useon = false;
    if( !_zCC3DClassify( cc, c, &ws, hmax, plane ) ) goto TERMINATE;
  }
  if( ( *cf = _zCC3DSplitCell( cc, c, &ws, plane, tag ) ) >= 0 ) ret = 2;
 TERMINATE:
  cc->_useon = true;
  return ret;
}

/* ********************************************************** */
/* operations
 * ********************************************************** */

/* order of faces of a polyhedron along the Z-order curve of their barycenters, so that the start
 * vertex of hill-climbing in a cut is close to the region cut by the next plane. */
typedef struct{
  unsigned long key;
  int id;
} _zCC3DFaceOrder;

static int _zCC3DFaceOrderCmp(const void *o1, const void *o2)
{
  unsigned long k1 = ((const _zCC3DFaceOrder *)o1)->key, k2 = ((const _zCC3DFaceOrder *)o2)->key;
  return k1 < k2 ? -1 : ( k1 > k2 ? 1 : 0 );
}

/* spread lower 21 bits of an integer to every three bits. */
static unsigned long _zCC3DSpreadBits(unsigned long v)
{
  unsigned long r = 0;
  int i;

  for( i=0; i<21; i++ ) r |= ( ( v >> i ) & 1UL ) << ( 3*i );
  return r;
}

static _zCC3DFaceOrder *_zCC3DFaceOrderCreate(const zPH3D *ph, const zAABox3D *box, const zIndexStruct *sel)
{
  _zCC3DFaceOrder *order;
  zVec3D c;
  double w;
  unsigned long q[3];
  int i, j, k;

  if( !( order = zAlloc( _zCC3DFaceOrder, zArraySize(sel) ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  for( j=0; j<zArraySize(sel); j++ ){
    i = zIndexElemNC(sel,j);
    zTri3DBarycenter( zPH3DFace(ph,i), &c );
    for( k=0; k<3; k++ ){
      w = box->max.e[k] - box->min.e[k];
      q[k] = (unsigned long)( ( w > 0 ? zLimit( ( c.e[k] - box->min.e[k] ) / w, 0, 1 ) : 0 ) * 0x1fffff );
    }
    order[j].key = _zCC3DSpreadBits( q[0] ) | _zCC3DSpreadBits( q[1] ) << 1 | _zCC3DSpreadBits( q[2] ) << 2;
    order[j].id = i;
  }
  qsort( order, zArraySize(sel), sizeof(_zCC3DFaceOrder), _zCC3DFaceOrderCmp );
  return order;
}

/* cut candidate cells by planes of selected faces of a convex polyhedron, and mark those inside of it. */
static bool _zCC3DSplitConvexPH3D(zCC3D *cc, const zPH3D *ph, const zAABox3D *box, const zIndexStruct *sel, zIndexStruct *cand)
{
  _zCC3DFaceOrder *order;
  zPlane3D plane;
  int i, j, n, c, cf, ret;
  bool result = false;

  if( !( order = _zCC3DFaceOrderCreate( ph, box, sel ) ) ) return false;
  for( j=0; j<zArraySize(sel) && zArraySize(cand)>0; j++ ){
    i = order[j].id;
    if( zTri3DIsDegenerate( zPH3DFace(ph,i), cc->tol ) ) continue;
    zPlane3DCreate( &plane, zTri3DVert(zPH3DFace(ph,i),0), zTri3DNorm(zPH3DFace(ph,i)) );
    cc->_plane++;
    for( n=0, c=0; c<zArraySize(cand); c++ ){
      if( ( ret = _zCC3DSplit( cc, zIndexElemNC(cand,c), &plane, cc->_tag, &cf ) ) < 0 ) goto TERMINATE;
      if( ret != 1 ) zIndexElemNC(cand,n++) = zIndexElemNC(cand,c);
    }
    zArraySetSize( cand, n );
  }
  for( j=0; j<zArraySize(cand); j++ ) zCC3DCell(cc,zIndexElemNC(cand,j))->_cand = true;
  result = true;
 TERMINATE:
  free( order );
  return result;
}

/* Non-convex polyhedron:
 * Candidate cells are cut along the solid BSP (binary space partitioning) tree of the polyhedron
 * built by the autopartition (Thibault and Naylor, 1987). The faces of the polyhedron are divided
 * into fragments by the plane of a fragment, which are passed to the subtrees behind and in front
 * of the plane, and the cells are cut by the same plane. A region without fragments is inside of
 * the polyhedron if it is behind the plane, and outside if in front of it. The tree is not stored;
 * a subtree is not built if no cell is in its region. The topology of the cell complex is changed
 * only by the cut of convex cells, so that it is kept consistent whatever the numerical errors in
 * the tree are.
 */

/* fragment of a face of a polyhedron, which is a convex polygon. */
typedef struct{
  int face; /* face of the polyhedron */
  int vs;   /* start of the vertices in the pool */
  int vn;   /* number of vertices */
} _zCC3DFrag;

/* pool of fragments. */
ZEDA_DEF_ARRAY_CLASS( _zCC3DFragArray, _zCC3DFrag );
ZEDA_DEF_ARRAY_CLASS( _zCC3DDoubleArray, double );
typedef struct{
  const zPH3D *ph;
  double tol;
  _zCC3DFragArray frag; /* fragments */
  zVec3DArray v;        /* vertices of fragments */
  _zCC3DDoubleArray s;  /* signed distances of vertices of a fragment from a plane */
} _zCC3DFragPool;

#define _zCC3DFragElem(pool,i) zArrayElemNC(&(pool)->frag,i)
#define _zCC3DFragVert(pool,i) zArrayElemNC(&(pool)->v,i)
#define _zCC3DFragDist(pool,i) zArrayBuf(&(pool)->s)[i]

static void _zCC3DFragPoolInit(_zCC3DFragPool *pool, const zPH3D *ph, double tol)
{
  pool->ph = ph;
  pool->tol = tol;
  zArrayInit( &pool->frag );
  zArrayInit( &pool->v );
  zArrayInit( &pool->s );
}

static void _zCC3DFragPoolDestroy(_zCC3DFragPool *pool)
{
  zArrayFree( &pool->frag );
  zArrayFree( &pool->v );
  zArrayFree( &pool->s );
  _zCC3DFragPoolInit( pool, NULL, 0 );
}

/* add a fragment of a face, whose vertices are to be added after it. */
static int _zCC3DFragAdd(_zCC3DFragPool *pool, int face)
{
  _zCC3DFrag *frag;

  _zCC3DArrayPush( &pool->frag, _zCC3DFrag, frag );
  if( !frag ) return -1;
  frag->face = face;
  frag->vs = zArraySize(&pool->v);
  frag->vn = 0;
  return zArraySize(&pool->frag) - 1;
}

/* add a vertex to the last fragment. */
static bool _zCC3DFragAddVert(_zCC3DFragPool *pool, const zVec3D *p)
{
  zVec3D *v;

  _zCC3DArrayPush( &pool->v, zVec3D, v );
  if( !v ) return false;
  zVec3DCopy( p, v );
  _zCC3DFragElem(pool,zArraySize(&pool->frag)-1)->vn++;
  return true;
}

/* plane of a fragment. */
static zPlane3D *_zCC3DFragPlane(_zCC3DFragPool *pool, int f, zPlane3D *plane)
{
  const zTri3D *tri = zPH3DFace(pool->ph,_zCC3DFragElem(pool,f)->face);
  return zPlane3DCreate( plane, zTri3DVert(tri,0), zTri3DNorm(tri) );
}

/* classify a fragment with respect to a plane: -1 behind, 1 in front, 0 on the plane, 2 across it.
 * the signed distances of the vertices are stored in pool->s, which are zero if within tolerance. */
static int _zCC3DFragClassify(_zCC3DFragPool *pool, int f, const zPlane3D *plane)
{
  zVec3D d;
  int i;
  bool back = false, front = false;

  if( zArrayCapacity(&pool->s) < _zCC3DFragElem(pool,f)->vn ){ /* used as a buffer regardless of the size */
    zArrayExpand( &pool->s, double, _zCC3DFragElem(pool,f)->vn );
    if( zArraySize(&pool->s) != _zCC3DFragElem(pool,f)->vn ) return -2;
  }
  for( i=0; i<_zCC3DFragElem(pool,f)->vn; i++ ){
    zVec3DSub( _zCC3DFragVert(pool,_zCC3DFragElem(pool,f)->vs+i), zPlane3DOrg(plane), &d );
    _zCC3DFragDist(pool,i) = zVec3DInnerProd( &d, zPlane3DNorm(plane) );
    if( _zCC3DFragDist(pool,i) < -pool->tol ) back = true;
    else if( _zCC3DFragDist(pool,i) > pool->tol ) front = true;
    else _zCC3DFragDist(pool,i) = 0;
  }
  return back ? ( front ? 2 : -1 ) : ( front ? 1 : 0 );
}

/* the part of a fragment across a plane behind it (sign=-1) or in front of it (sign=1), where the
 * signed distances of the vertices have to be stored in pool->s by _zCC3DFragClassify(). */
static int _zCC3DFragClip(_zCC3DFragPool *pool, int f, double sign)
{
  zVec3D p;
  int i, j, n, nf;
  double si, sj;

  if( ( nf = _zCC3DFragAdd( pool, _zCC3DFragElem(pool,f)->face ) ) < 0 ) return -1;
  n = _zCC3DFragElem(pool,f)->vn;
  for( i=0; i<n; i++ ){
    j = ( i + 1 ) % n;
    si = sign * _zCC3DFragDist(pool,i);
    sj = sign * _zCC3DFragDist(pool,j);
    if( si >= 0 ){ /* copied, since pool->v might be reallocated */
      zVec3DCopy( _zCC3DFragVert(pool,_zCC3DFragElem(pool,f)->vs+i), &p );
      if( !_zCC3DFragAddVert( pool, &p ) ) return -1;
    }
    if( ( si > 0 && sj < 0 ) || ( si < 0 && sj > 0 ) ){
      _zVec3DInterDiv( _zCC3DFragVert(pool,_zCC3DFragElem(pool,f)->vs+i), _zCC3DFragVert(pool,_zCC3DFragElem(pool,f)->vs+j), si/(si-sj), &p );
      if( !_zCC3DFragAddVert( pool, &p ) ) return -1;
    }
  }
  return nf;
}

/* choose a fragment as the splitter, which divides the fewest fragments among a few candidates. */
#define _ZEO_CC3D_SPLITTER_CAND_NUM   5
#define _ZEO_CC3D_SPLITTER_SAMPLE_NUM 64
static int _zCC3DFragChooseSplitter(_zCC3DFragPool *pool, const int *frag, int n)
{
  zPlane3D plane;
  int m, k, i, step, f, best = frag[0], score, bestscore = INT_MAX, nb, nf, ns;

  if( n <= 1 ) return best;
  m = zMin( n, _ZEO_CC3D_SPLITTER_CAND_NUM );
  step = zMax( 1, n / _ZEO_CC3D_SPLITTER_SAMPLE_NUM );
  for( k=0; k<m; k++ ){
    f = frag[( 2*k + 1 ) * n / ( 2*m )];
    _zCC3DFragPlane( pool, f, &plane );
    for( nb=nf=ns=0, i=0; i<n; i+=step )
      switch( _zCC3DFragClassify( pool, frag[i], &plane ) ){
      case -1: nb++; break;
      case  1: nf++; break;
      case  2: ns++; break;
      case -2: return -1;
      default: ;
      }
    if( ( score = 8 * ns + abs( nf - nb ) ) < bestscore ){
      bestscore = score;
      best = f;
    }
  }
  return best;
}

/* task of the autopartition: fragments and cells in a region. */
typedef struct{
  int fs, fn; /* fragments in the stack */
  int cs, cn; /* cells in the stack */
  bool in;    /* the region is inside of the polyhedron if no fragment is in it */
} _zCC3DBSPTask;

ZEDA_DEF_ARRAY_CLASS( _zCC3DBSPTaskStack, _zCC3DBSPTask );

/* push a task with fragments and cells in a region. */
static bool _zCC3DBSPTaskPush(_zCC3DBSPTaskStack *task, zIndexStruct *fstack, const zIndexStruct *frag, zIndexStruct *cstack, const zIndexStruct *cell, bool in)
{
  _zCC3DBSPTask *t;
  int i;

  if( zArraySize(cell) == 0 ) return true; /* no cell in the region */
  _zCC3DArrayPush( task, _zCC3DBSPTask, t );
  if( !t ) return false;
  t->fs = zArraySize(fstack); t->fn = zArraySize(frag);
  t->cs = zArraySize(cstack); t->cn = zArraySize(cell);
  t->in = in;
  for( i=0; i<zArraySize(frag); i++ )
    if( !_zCC3DIndexPush( fstack, zIndexElemNC(frag,i) ) ) return false;
  for( i=0; i<zArraySize(cell); i++ )
    if( !_zCC3DIndexPush( cstack, zIndexElemNC(cell,i) ) ) return false;
  return true;
}

/* add a fragment of a face of a polyhedron clipped by a region, which is given by the six planes of
 * its faces (NULL for no region). */
static bool _zCC3DFragAddFace(_zCC3DFragPool *pool, int face, const zPlane3D *region, zIndexStruct *frag)
{
  int f, k;

  if( ( f = _zCC3DFragAdd( pool, face ) ) < 0 ||
      !_zCC3DFragAddVert( pool, zTri3DVert(zPH3DFace(pool->ph,face),0) ) ||
      !_zCC3DFragAddVert( pool, zTri3DVert(zPH3DFace(pool->ph,face),1) ) ||
      !_zCC3DFragAddVert( pool, zTri3DVert(zPH3DFace(pool->ph,face),2) ) ) return false;
  if( region ){
    for( k=0; k<6; k++ ){ /* the region is behind its six planes */
      switch( _zCC3DFragClassify( pool, f, &region[k] ) ){
      case  1: return true; /* out of the region */
      case  2: if( ( f = _zCC3DFragClip( pool, f, -1 ) ) < 0 ) return false; break;
      case -2: return false;
      default: ;
      }
    }
  }
  return _zCC3DIndexPush( frag, f );
}

/* cut candidate cells along the solid BSP tree of selected faces of a polyhedron, and mark those
 * inside of it. If a region is given, the fragments are clipped by it, so that an empty region of
 * the tree is adjacent to the fragment of the last splitter within the region; the candidate cells
 * have to be in the region. */
static bool _zCC3DSplitBSPPH3D(zCC3D *cc, const zPH3D *ph, const zAABox3D *region, const zIndexStruct *sel, zIndexStruct *cand)
{
  _zCC3DFragPool pool;
  _zCC3DBSPTaskStack task;
  _zCC3DBSPTask t;
  zIndexStruct fstack, cstack, frag, fb, ff, cb, cf;
  zPlane3D plane, rplane[6], *rp = NULL;
  int i, f, c, cnew, ret, splitter;
  bool result = false;

  if( region ) rp = zAABox3DFaceToPlane3D6( region, rplane );
  _zCC3DFragPoolInit( &pool, ph, cc->tol );
  zArrayInit( &task );
  zArrayInit( &fstack ); zArrayInit( &cstack ); zArrayInit( &frag );
  zArrayInit( &fb ); zArrayInit( &ff ); zArrayInit( &cb ); zArrayInit( &cf );
  for( i=0; i<zArraySize(sel); i++ ){
    if( zTri3DIsDegenerate( zPH3DFace(ph,zIndexElemNC(sel,i)), cc->tol ) ) continue; /* not to be a splitter */
    if( !_zCC3DFragAddFace( &pool, zIndexElemNC(sel,i), rp, &frag ) ) goto TERMINATE;
  }
  if( zArraySize(&frag) == 0 ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    goto TERMINATE;
  }
  if( !_zCC3DBSPTaskPush( &task, &fstack, &frag, &cstack, cand, false ) ) goto TERMINATE;
  while( zArraySize(&task) > 0 ){
    t = *zArrayHead(&task);
    zArraySetSize( &task, zArraySize(&task)-1 );
    if( t.fn == 0 ){ /* leaf */
      if( t.in )
        for( i=0; i<t.cn; i++ ) zCC3DCell(cc,zIndexElemNC(&cstack,t.cs+i))->_cand = true;
      zArraySetSize( &fstack, t.fs );
      zArraySetSize( &cstack, t.cs );
      continue;
    }
    /* the fragments and cells of the task are on the top of the stacks */
    zArraySetSize( &frag, 0 );
    for( i=0; i<t.fn; i++ )
      if( !_zCC3DIndexPush( &frag, zIndexElemNC(&fstack,t.fs+i) ) ) goto TERMINATE;
    zArraySetSize( &fstack, t.fs );
    if( ( splitter = _zCC3DFragChooseSplitter( &pool, zArrayBuf(&frag), zArraySize(&frag) ) ) < 0 ) goto TERMINATE;
    _zCC3DFragPlane( &pool, splitter, &plane );
    cc->_plane++;
    zArraySetSize( &cb, 0 ); zArraySetSize( &cf, 0 );
    for( i=0; i<t.cn; i++ ){
      c = zIndexElemNC(&cstack,t.cs+i);
      if( ( ret = _zCC3DSplit( cc, c, &plane, cc->_tag, &cnew ) ) < 0 ) goto TERMINATE;
      if( !_zCC3DIndexPush( ret == 1 ? &cf : &cb, c ) ||
          ( ret == 2 && !_zCC3DIndexPush( &cf, cnew ) ) ) goto TERMINATE;
    }
    zArraySetSize( &cstack, t.cs );
    /* the splitter and fragments on the plane are consumed */
    zArraySetSize( &fb, 0 ); zArraySetSize( &ff, 0 );
    for( i=0; i<zArraySize(&frag); i++ ){
      if( ( f = zIndexElemNC(&frag,i) ) == splitter ) continue;
      switch( _zCC3DFragClassify( &pool, f, &plane ) ){
      case -1: if( zArraySize(&cb) > 0 && !_zCC3DIndexPush( &fb, f ) ) goto TERMINATE; break;
      case  1: if( zArraySize(&cf) > 0 && !_zCC3DIndexPush( &ff, f ) ) goto TERMINATE; break;
      case  2:
        if( zArraySize(&cb) > 0 && ( ( ret = _zCC3DFragClip( &pool, f, -1 ) ) < 0 || !_zCC3DIndexPush( &fb, ret ) ) ) goto TERMINATE;
        if( zArraySize(&cf) > 0 && ( ( ret = _zCC3DFragClip( &pool, f,  1 ) ) < 0 || !_zCC3DIndexPush( &ff, ret ) ) ) goto TERMINATE;
        break;
      case -2: goto TERMINATE;
      default: ;
      }
    }
    if( !_zCC3DBSPTaskPush( &task, &fstack, &ff, &cstack, &cf, false ) ||
        !_zCC3DBSPTaskPush( &task, &fstack, &fb, &cstack, &cb, true ) ) goto TERMINATE;
  }
  result = true;
 TERMINATE:
  zArrayFree( &task );
  zArrayFree( &fstack ); zArrayFree( &cstack ); zArrayFree( &frag );
  zArrayFree( &fb ); zArrayFree( &ff ); zArrayFree( &cb ); zArrayFree( &cf );
  _zCC3DFragPoolDestroy( &pool );
  return result;
}

/* select faces of a polyhedron whose axis-aligned bounding boxes overlap a region (all faces if the
 * region is NULL). It is also checked if any face actually intersects the region. */
static bool _zCC3DPH3DSelectFaces(const zPH3D *ph, const zAABox3D *region, double tol, zIndexStruct *sel, bool *hit)
{
  _zCC3DFragPool pool;
  zIndexStruct frag;
  zPlane3D rplane[6];
  const zTri3D *tri;
  double lo, hi;
  int i, j, k;
  bool ret = false;

  *hit = region == NULL;
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    if( region ){
      tri = zPH3DFace(ph,i);
      for( k=0; k<3; k++ ){
        lo = hi = zTri3DVert(tri,0)->e[k];
        for( j=1; j<3; j++ ){
          lo = zMin( lo, zTri3DVert(tri,j)->e[k] );
          hi = zMax( hi, zTri3DVert(tri,j)->e[k] );
        }
        if( hi < region->min.e[k] - tol || lo > region->max.e[k] + tol ) break;
      }
      if( k < 3 ) continue;
    }
    if( !_zCC3DIndexPush( sel, i ) ) return false;
  }
  if( *hit ) return true;
  /* a face that remains after being clipped by the region */
  zAABox3DFaceToPlane3D6( region, rplane );
  _zCC3DFragPoolInit( &pool, ph, tol );
  zArrayInit( &frag );
  for( i=0; i<zArraySize(sel) && zArraySize(&frag)==0; i++ ){
    if( zTri3DIsDegenerate( zPH3DFace(ph,zIndexElemNC(sel,i)), tol ) ) continue; /* not used for the cut */
    if( !_zCC3DFragAddFace( &pool, zIndexElemNC(sel,i), rplane, &frag ) ) goto TERMINATE;
  }
  *hit = zArraySize(&frag) > 0;
  ret = true;
 TERMINATE:
  zArrayFree( &frag );
  _zCC3DFragPoolDestroy( &pool );
  return ret;
}

/* mark cells of a given label inside of a polyhedron as candidates by cutting them. The polyhedron
 * is assumed to satisfy the condition, which is not checked here.
 * If a region is given, only the faces of the polyhedron in the region are used, where the candidate
 * cells have to be in the region. For a convex polyhedron, the part of the region inside of the
 * polyhedron is bounded by the planes of the faces in the region if any. For a general polyhedron,
 * see the explanation of the solid BSP tree. If no face is in the region, the region is entirely
 * inside or outside of the polyhedron. */
static bool _zCC3DSplitPH3D(zCC3D *cc, const zPH3D *ph, bool solid, bool convex, const zAABox3D *region)
{
  zIndexStruct cand, sel;
  zAABox3D box;
  zVec3D center;
  int c;
  bool hit, result = false;

  cc->_tag++; /* tag of faces made by the operation, which is renewed even if no face is made */
  for( c=0; c<zCC3DCellNum(cc); c++ ) zCC3DCell(cc,c)->_cand = false;
  if( !zPH3DAABB( ph, &box ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return false;
  }
  zArrayInit( &cand );
  zArrayInit( &sel );
  for( c=0; c<zCC3DCellNum(cc); c++ )
    if( zCC3DCell(cc,c)->solid == solid && zColChkAABox3D( &box, &zCC3DCell(cc,c)->aabb, cc->tol ) )
      if( !_zCC3DIndexPush( &cand, c ) ) goto TERMINATE;
  if( zArraySize(&cand) == 0 ){
    result = true;
    goto TERMINATE;
  }
  if( !_zCC3DPH3DSelectFaces( ph, region, cc->tol, &sel, &hit ) ) goto TERMINATE;
  if( !hit ){ /* no face in the region */
    zAABox3DCenter( region, &center );
    if( zPH3DPointIsInside( ph, &center, 0 ) )
      for( c=0; c<zArraySize(&cand); c++ ) zCC3DCell(cc,zIndexElemNC(&cand,c))->_cand = true;
    result = true;
    goto TERMINATE;
  }
  result = convex ?
    _zCC3DSplitConvexPH3D( cc, ph, &box, &sel, &cand ) : _zCC3DSplitBSPPH3D( cc, ph, region, &sel, &cand );
 TERMINATE:
  zArrayFree( &cand );
  zArrayFree( &sel );
  return result;
}

/* check if a polyhedron satisfies the condition for operations, and if it is convex. */
static bool _zCC3DPH3DCheck(const zPH3D *ph, bool *convex)
{
  if( !zPH3DIsClosed( ph ) ){
    ZRUNERROR( ZEO_ERR_CC3D_OPENPH );
    return false;
  }
  if( zPH3DVolume( ph ) <= 0 ){
    ZRUNERROR( ZEO_ERR_CC3D_INVERTEDPH );
    return false;
  }
  *convex = zPH3DIsConvex( ph );
  return true;
}

/* ********************************************************** */
/* enlargement of the universe
 * ********************************************************** */

/* The universe is enlarged by surrounding the old one with 26 boxes, which are the 3x3x3 grid boxes
 * except the center one. The faces on the boundary of the old universe are shared by the boxes
 * adjacent to them, and the vertices on the edges of the old universe are inserted into the edges
 * of the faces between the surrounding boxes. Since vertices on the boundary of the universe are
 * exactly on it (see the cut of a cell), they are identified by exact comparisons of coordinates. */

/* code of a coordinate with respect to the old universe: 1 on the minimum, 2 on the maximum. */
static int _zCC3DBoundCode(const zCC3D *cc, int v, int k)
{
  const zVec3D *p = &_zCC3DV(cc,v)->p;
  return p->e[k] == cc->universe.min.e[k] ? 1 : ( p->e[k] == cc->universe.max.e[k] ? 2 : 0 );
}

/* edge of the old universe on which a segment between two vertices lies (-1 if none).
 * The edge is identified by the free axis k and the codes of the other two axes. */
static int _zCC3DOldEdgeID(const zCC3D *cc, int u, int v)
{
  int k, k1, k2, c1, c2;

  for( k=0; k<3; k++ ){
    k1 = (k+1)%3; k2 = (k+2)%3;
    if( ( c1 = _zCC3DBoundCode( cc, u, k1 ) ) == 0 || c1 != _zCC3DBoundCode( cc, v, k1 ) ) continue;
    if( ( c2 = _zCC3DBoundCode( cc, u, k2 ) ) == 0 || c2 != _zCC3DBoundCode( cc, v, k2 ) ) continue;
    return k*4 + (c1-1) + 2*(c2-1);
  }
  return -1;
}

/* identifier of a grid point (a,b,c), each of which is from 0 to 3. */
#define _zCC3DGridID(a,b,c) ( (a) + 4*( (b) + 4*(c) ) )
/* identifier of a grid box (i,j,k), each of which is from 0 to 2 (13 for the old universe). */
#define _zCC3DBoxID(i,j,k)  ( (i) + 3*( (j) + 3*(k) ) )
#define _ZEO_CC3D_BOX_CENTER   13

/* add a side of a face as a loop of half-edges. */
static bool _zCC3DAddFaceSide(zCC3D *cc, int f, int side, const int *vert, int n)
{
  int i, h0;

  h0 = _zCC3DHENum(cc);
  for( i=0; i<n; i++ )
    if( _zCC3DAddHalfEdge( cc, vert[i], f, side ) < 0 ) return false;
  for( i=0; i<n; i++ ){
    _zCC3DHE(cc,h0+i)->next = h0 + (i+1)%n;
    _zCC3DHE(cc,h0+i)->prev = h0 + (i+n-1)%n;
  }
  zCC3DFace(cc,f)->he[side] = h0;
  return true;
}

/* pair twins of half-edges of a cell. */
typedef struct{
  int v1, v2; /* smaller and larger vertices */
  int he;
} _zCC3DEdgeKey;

static int _zCC3DEdgeKeyCmp(const void *k1, const void *k2)
{
  const _zCC3DEdgeKey *a = (const _zCC3DEdgeKey *)k1, *b = (const _zCC3DEdgeKey *)k2;
  if( a->v1 != b->v1 ) return a->v1 < b->v1 ? -1 : 1;
  return a->v2 < b->v2 ? -1 : ( a->v2 > b->v2 ? 1 : 0 );
}

static bool _zCC3DPairTwins(zCC3D *cc, zIndexStruct *hes)
{
  _zCC3DEdgeKey *key;
  int i, u, v;
  bool ret = false;

  if( !( key = zAlloc( _zCC3DEdgeKey, zArraySize(hes) ) ) ){
    ZALLOCERROR();
    return false;
  }
  for( i=0; i<zArraySize(hes); i++ ){
    u = _zCC3DHE(cc,zIndexElemNC(hes,i))->org;
    v = _zCC3DTarget( cc, zIndexElemNC(hes,i) );
    key[i].v1 = zMin( u, v );
    key[i].v2 = zMax( u, v );
    key[i].he = zIndexElemNC(hes,i);
  }
  qsort( key, zArraySize(hes), sizeof(_zCC3DEdgeKey), _zCC3DEdgeKeyCmp );
  for( i=0; i<zArraySize(hes); i+=2 ){
    if( i+1 >= zArraySize(hes) || _zCC3DEdgeKeyCmp( &key[i], &key[i+1] ) != 0 ||
        ( i+2 < zArraySize(hes) && _zCC3DEdgeKeyCmp( &key[i], &key[i+2] ) == 0 ) ||
        _zCC3DHE(cc,key[i].he)->org == _zCC3DHE(cc,key[i+1].he)->org ){
      ZRUNERROR( ZEO_ERR_CC3D_INCONSISTENT );
      goto TERMINATE;
    }
    _zCC3DHE(cc,key[i].he)->twin = key[i+1].he;
    _zCC3DHE(cc,key[i+1].he)->twin = key[i].he;
  }
  ret = true;
 TERMINATE:
  free( key );
  return ret;
}

/* enlarge the universe to include a box. */
static bool _zCC3DEnlargeUniverse(zCC3D *cc, const zAABox3D *box)
{
  zAABox3D uv;
  double grid[3][4], d, scale;
  zVec3D p;
  int gid[64], cell[27], *nb = NULL, chainstart[12], chainlen[12];
  zIndexStruct chain, poly, hes;
  int i, j, k, a, b, c, k1, k2, f, h, n, u, v, e, side, blk[2], idx[3], corner[4][3], nf0;
  bool ret = false;

  nf0 = zCC3DFaceNum(cc); /* faces of the old universe */
  zArrayInit( &chain );
  zArrayInit( &poly );
  zArrayInit( &hes );
  /* new universe */
  for( d=0, k=0; k<3; k++ ) d = zMax( d, box->max.e[k] - box->min.e[k] );
  if( d <= 0 )
    for( k=0; k<3; k++ ) d = zMax( d, cc->universe.max.e[k] - cc->universe.min.e[k] );
  for( k=0; k<3; k++ ){
    uv.min.e[k] = zMin( cc->universe.min.e[k], box->min.e[k] ) - d;
    uv.max.e[k] = zMax( cc->universe.max.e[k], box->max.e[k] ) + d;
    grid[k][0] = uv.min.e[k];
    grid[k][1] = cc->universe.min.e[k];
    grid[k][2] = cc->universe.max.e[k];
    grid[k][3] = uv.max.e[k];
  }
  /* vertices on the edges of the old universe */
  if( !( nb = zAlloc( int, 2*zCC3DVertNum(cc) ) ) ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  for( i=0; i<2*zCC3DVertNum(cc); i++ ) nb[i] = -1;
  for( f=0; f<zCC3DFaceNum(cc); f++ ){
    if( zCC3DFace(cc,f)->cell[1] >= 0 ) continue; /* not on the boundary of the universe */
    h = zCC3DFace(cc,f)->he[0];
    do{
      u = _zCC3DHE(cc,h)->org;
      v = _zCC3DTarget( cc, h );
      if( _zCC3DOldEdgeID( cc, u, v ) >= 0 ){
        /* neighbors on the edge (corners are excluded since they are on three edges) */
        if( nb[2*u] != v && nb[2*u+1] != v ) nb[2*u+( nb[2*u] < 0 ? 0 : 1 )] = v;
        if( nb[2*v] != u && nb[2*v+1] != u ) nb[2*v+( nb[2*v] < 0 ? 0 : 1 )] = u;
      }
      h = _zCC3DHE(cc,h)->next;
    } while( h != zCC3DFace(cc,f)->he[0] );
  }
  /* chains of vertices on the edges from the smaller corner to the larger one; corners themselves
   * are not stored. */
  for( e=0; e<12; e++ ){
    k = e / 4; k1 = (k+1)%3; k2 = (k+2)%3;
    idx[k] = 1; idx[k1] = 1 + (e%2); idx[k2] = 1 + (e/2)%2;
    u = cc->_corner[(idx[0]-1)|(idx[1]-1)<<1|(idx[2]-1)<<2];
    idx[k] = 2;
    v = cc->_corner[(idx[0]-1)|(idx[1]-1)<<1|(idx[2]-1)<<2];
    chainstart[e] = zArraySize(&chain);
    /* the first vertex next to the corner u on this edge */
    for( n=-1, f=0; f<zCC3DFaceNum(cc) && n<0; f++ ){
      if( zCC3DFace(cc,f)->cell[1] >= 0 ) continue;
      h = zCC3DFace(cc,f)->he[0];
      do{
        if( _zCC3DHE(cc,h)->org == u && _zCC3DOldEdgeID( cc, u, _zCC3DTarget( cc, h ) ) == e ){
          n = _zCC3DTarget( cc, h );
          break;
        }
        h = _zCC3DHE(cc,h)->next;
      } while( h != zCC3DFace(cc,f)->he[0] );
    }
    if( n < 0 ) goto INCONSISTENT;
    for( a=u; n!=v; ){
      if( !_zCC3DIndexPush( &chain, n ) || zArraySize(&chain) > zCC3DVertNum(cc) ) goto INCONSISTENT;
      b = nb[2*n] == a ? nb[2*n+1] : nb[2*n];
      if( b < 0 ) goto INCONSISTENT;
      a = n;
      n = b;
    }
    chainlen[e] = zArraySize(&chain) - chainstart[e];
  }
  /* grid points */
  for( c=0; c<4; c++ )
    for( b=0; b<4; b++ )
      for( a=0; a<4; a++ ){
        if( a >= 1 && a <= 2 && b >= 1 && b <= 2 && c >= 1 && c <= 2 ){
          gid[_zCC3DGridID(a,b,c)] = cc->_corner[(a-1)|(b-1)<<1|(c-1)<<2];
          continue;
        }
        zVec3DCreate( &p, grid[0][a], grid[1][b], grid[2][c] );
        if( ( gid[_zCC3DGridID(a,b,c)] = _zCC3DAddVert( cc, &p ) ) < 0 ) goto TERMINATE;
      }
  /* surrounding boxes */
  for( c=0; c<3; c++ )
    for( b=0; b<3; b++ )
      for( a=0; a<3; a++ ){
        if( _zCC3DBoxID(a,b,c) == _ZEO_CC3D_BOX_CENTER ){
          cell[_ZEO_CC3D_BOX_CENTER] = -1;
          continue;
        }
        if( ( i = _zCC3DAddCell( cc, false ) ) < 0 ) goto TERMINATE;
        cell[_zCC3DBoxID(a,b,c)] = i;
        zVec3DCreate( &zCC3DCell(cc,i)->aabb.min, grid[0][a], grid[1][b], grid[2][c] );
        zVec3DCreate( &zCC3DCell(cc,i)->aabb.max, grid[0][a+1], grid[1][b+1], grid[2][c+1] );
      }
  /* faces between grid boxes perpendicular to the k-axis at the grid plane a */
  for( k=0; k<3; k++ ){
    k1 = (k+1)%3; k2 = (k+2)%3;
    for( a=0; a<4; a++ )
      for( b=0; b<3; b++ )
        for( c=0; c<3; c++ ){
          for( side=0; side<2; side++ ){
            idx[k] = a - 1 + side; idx[k1] = b; idx[k2] = c;
            blk[side] = idx[k] < 0 || idx[k] > 2 ? -1 : cell[_zCC3DBoxID(idx[0],idx[1],idx[2])];
          }
          if( blk[0] < 0 && blk[1] < 0 ) continue;
          idx[k] = a - 1; idx[k1] = b; idx[k2] = c;
          if( ( idx[k] >= 0 && _zCC3DBoxID(idx[0],idx[1],idx[2]) == _ZEO_CC3D_BOX_CENTER ) ||
              ( ++idx[k] <= 2 && _zCC3DBoxID(idx[0],idx[1],idx[2]) == _ZEO_CC3D_BOX_CENTER ) )
            continue; /* the boundary of the old universe */
          /* corners counterclockwise about the k-axis */
          for( i=0; i<4; i++ ){
            corner[i][k] = a;
            corner[i][k1] = b + ( i == 1 || i == 2 );
            corner[i][k2] = c + ( i >= 2 );
          }
          zArraySetSize( &poly, 0 );
          for( i=0; i<4; i++ ){
            u = gid[_zCC3DGridID(corner[i][0],corner[i][1],corner[i][2])];
            if( !_zCC3DIndexPush( &poly, u ) ) goto TERMINATE;
            /* vertices on an edge of the old universe */
            v = gid[_zCC3DGridID(corner[(i+1)%4][0],corner[(i+1)%4][1],corner[(i+1)%4][2])];
            for( j=0; j<3; j++ )
              if( corner[i][j] < 1 || corner[i][j] > 2 || corner[(i+1)%4][j] < 1 || corner[(i+1)%4][j] > 2 ) break;
            if( j < 3 ) continue;
            for( e=0; e<12; e++ ){
              int ek = e / 4, ek1 = (ek+1)%3, ek2 = (ek+2)%3;
              if( corner[i][ek] == corner[(i+1)%4][ek] ) continue;
              if( corner[i][ek1] != 1 + (e%2) || corner[i][ek2] != 1 + (e/2)%2 ) continue;
              if( corner[i][ek] < corner[(i+1)%4][ek] ){
                for( j=0; j<chainlen[e]; j++ )
                  if( !_zCC3DIndexPush( &poly, zIndexElemNC(&chain,chainstart[e]+j) ) ) goto TERMINATE;
              } else{
                for( j=chainlen[e]-1; j>=0; j-- )
                  if( !_zCC3DIndexPush( &poly, zIndexElemNC(&chain,chainstart[e]+j) ) ) goto TERMINATE;
              }
              break;
            }
          }
          if( blk[0] < 0 ){ /* a face on the boundary of the universe is outward for the inner cell */
            for( i=0; i<zArraySize(&poly)/2; i++ ) zSwap( int, zIndexElemNC(&poly,i), zIndexElemNC(&poly,zArraySize(&poly)-1-i) );
            blk[0] = blk[1];
            blk[1] = -1;
          }
          if( ( f = _zCC3DAddFace( cc, blk[0], blk[1], 0 ) ) < 0 ) goto TERMINATE;
          if( !_zCC3DAddFaceSide( cc, f, 0, zArrayBuf(&poly), zArraySize(&poly) ) ) goto TERMINATE;
          if( blk[1] >= 0 ){
            for( i=0; i<zArraySize(&poly)/2; i++ ) zSwap( int, zIndexElemNC(&poly,i), zIndexElemNC(&poly,zArraySize(&poly)-1-i) );
            if( !_zCC3DAddFaceSide( cc, f, 1, zArrayBuf(&poly), zArraySize(&poly) ) ) goto TERMINATE;
          }
          if( blk[1] >= 0 ){ /* mates */
            h = zCC3DFace(cc,f)->he[0];
            do{
              /* the mate of u->v on side 1 starts at v */
              for( j=zCC3DFace(cc,f)->he[1]; _zCC3DHE(cc,j)->org != _zCC3DTarget( cc, h ); j=_zCC3DHE(cc,j)->next );
              _zCC3DHE(cc,h)->mate = j;
              _zCC3DHE(cc,j)->mate = h;
              h = _zCC3DHE(cc,h)->next;
            } while( h != zCC3DFace(cc,f)->he[0] );
          }
        }
  }
  /* faces on the boundary of the old universe are shared by the adjacent boxes */
  for( f=0; f<nf0; f++ ){
    if( zCC3DFace(cc,f)->cell[1] >= 0 ) continue;
    h = zCC3DFace(cc,f)->he[0];
    for( k=0; k<3; k++ ){ /* the side of the old universe on which the face lies */
      if( ( n = _zCC3DBoundCode( cc, _zCC3DHE(cc,h)->org, k ) ) == 0 ) continue;
      for( j=_zCC3DHE(cc,h)->next; j!=h; j=_zCC3DHE(cc,j)->next )
        if( _zCC3DBoundCode( cc, _zCC3DHE(cc,j)->org, k ) != n ) break;
      if( j == h ) break;
    }
    if( k == 3 ) goto INCONSISTENT;
    idx[0] = idx[1] = idx[2] = 1;
    idx[k] = n == 1 ? 0 : 2;
    zCC3DFace(cc,f)->cell[1] = cell[_zCC3DBoxID(idx[0],idx[1],idx[2])];
    i = _zCC3DHENum(cc);
    do{ /* side 1: mates in the reversed order */
      if( ( j = _zCC3DAddHalfEdge( cc, _zCC3DTarget( cc, h ), f, 1 ) ) < 0 ) goto TERMINATE;
      _zCC3DHE(cc,j)->mate = h;
      _zCC3DHE(cc,h)->mate = j;
      h = _zCC3DHE(cc,h)->next;
    } while( h != zCC3DFace(cc,f)->he[0] );
    for( j=i; j<_zCC3DHENum(cc); j++ ){
      _zCC3DHE(cc,j)->next = _zCC3DHE(cc,_zCC3DHE(cc,_zCC3DHE(cc,j)->mate)->prev)->mate;
      _zCC3DHE(cc,_zCC3DHE(cc,j)->next)->prev = j;
    }
    zCC3DFace(cc,f)->he[1] = i;
  }
  /* twins in each box, and the cell assignments */
  for( i=0; i<27; i++ ){
    if( cell[i] < 0 ) continue;
    zArraySetSize( &hes, 0 );
    for( f=0; f<zCC3DFaceNum(cc); f++ )
      for( side=0; side<2; side++ ){
        if( zCC3DFace(cc,f)->cell[side] != cell[i] ) continue;
        h = zCC3DFace(cc,f)->he[side];
        do{
          if( !_zCC3DIndexPush( &hes, h ) ) goto TERMINATE;
          h = _zCC3DHE(cc,h)->next;
        } while( h != zCC3DFace(cc,f)->he[side] );
      }
    if( !_zCC3DPairTwins( cc, &hes ) ) goto TERMINATE;
    zCC3DCell(cc,cell[i])->he = zIndexElemNC(&hes,0);
  }
  /* the new universe */
  for( c=0; c<2; c++ )
    for( b=0; b<2; b++ )
      for( a=0; a<2; a++ )
        cc->_corner[a|b<<1|c<<2] = gid[_zCC3DGridID(3*a,3*b,3*c)];
  zCopy( zAABox3D, &uv, &cc->universe );
  for( scale=0, k=0; k<3; k++ ){
    scale = zMax( scale, fabs( uv.min.e[k] ) );
    scale = zMax( scale, fabs( uv.max.e[k] ) );
    scale = zMax( scale, uv.max.e[k] - uv.min.e[k] );
  }
  cc->tol = zTOL * scale;
  ret = true;
  goto TERMINATE;

 INCONSISTENT:
  ZRUNERROR( ZEO_ERR_CC3D_INCONSISTENT );
 TERMINATE:
  free( nb );
  zArrayFree( &chain );
  zArrayFree( &poly );
  zArrayFree( &hes );
  return ret;
}

/* cut the solid by a plane. */
zCC3D *zCC3DCutByPlane3D(zCC3D *cc, const zPlane3D *plane)
{
  int c, n, cf, ret;

  cc->_tag++;
  cc->_plane++;
  for( n=zCC3DCellNum(cc), c=0; c<n; c++ ){
    if( !zCC3DCell(cc,c)->solid ) continue;
    if( ( ret = _zCC3DSplit( cc, c, plane, cc->_tag, &cf ) ) < 0 ) return NULL;
    if( ret == 1 ) zCC3DCell(cc,c)->solid = false;
    if( ret == 2 ) zCC3DCell(cc,cf)->solid = false;
  }
  return cc;
}

/* merge a polyhedron to the solid. */
static zCC3D *_zCC3DMerge(zCC3D *cc, const zPH3D *ph, bool convex)
{
  zAABox3D box;
  int c;

  if( zPH3DAABB( ph, &box ) &&
      ( box.min.c.x < cc->universe.min.c.x || box.min.c.y < cc->universe.min.c.y || box.min.c.z < cc->universe.min.c.z ||
        box.max.c.x > cc->universe.max.c.x || box.max.c.y > cc->universe.max.c.y || box.max.c.z > cc->universe.max.c.z ) &&
      !_zCC3DEnlargeUniverse( cc, &box ) ) return NULL;
  if( !_zCC3DSplitPH3D( cc, ph, false, convex, NULL ) ) return NULL;
  for( c=0; c<zCC3DCellNum(cc); c++ )
    if( zCC3DCell(cc,c)->_cand ) zCC3DCell(cc,c)->solid = true;
  return cc;
}

zCC3D *zCC3DMergeConvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DMerge( cc, ph, true );
}

zCC3D *zCC3DMergeNonconvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DMerge( cc, ph, false );
}

zCC3D *zCC3DMerge(zCC3D *cc, const zPH3D *ph)
{
  bool convex;

  return _zCC3DPH3DCheck( ph, &convex ) ? _zCC3DMerge( cc, ph, convex ) : NULL;
}

/* subtract a polyhedron from the solid. */
static zCC3D *_zCC3DSubtract(zCC3D *cc, const zPH3D *ph, bool convex)
{
  int c;

  if( !_zCC3DSplitPH3D( cc, ph, true, convex, NULL ) ) return NULL;
  for( c=0; c<zCC3DCellNum(cc); c++ )
    if( zCC3DCell(cc,c)->_cand ) zCC3DCell(cc,c)->solid = false;
  return cc;
}

zCC3D *zCC3DSubtractConvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DSubtract( cc, ph, true );
}

zCC3D *zCC3DSubtractNonconvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DSubtract( cc, ph, false );
}

zCC3D *zCC3DSubtract(zCC3D *cc, const zPH3D *ph)
{
  bool convex;

  return _zCC3DPH3DCheck( ph, &convex ) ? _zCC3DSubtract( cc, ph, convex ) : NULL;
}

/* intersection of the solid and a polyhedron.
 * Since the intersection is in the intersection of the axis-aligned bounding boxes of the solid and
 * the polyhedron, the solid is cut by the planes of the box in advance, and only the faces of the
 * polyhedron in the box are used. The box is slightly enlarged, so that its planes do not coincide
 * with those of faces of the polyhedron, which would be left with the tags of the planes of the box.
 * A plane of the box is not used if the solid is inside of it. */
#define _ZEO_CC3D_REGION_MARGIN 1.0e-3
static zCC3D *_zCC3DIntersect(zCC3D *cc, const zPH3D *ph, bool convex)
{
  zAABox3D solidbox, phbox, region;
  zPlane3D plane[6];
  double d, margin;
  int c, k, side;
  bool hassolid = false;

  for( c=0; c<zCC3DCellNum(cc); c++ ){
    if( !zCC3DCell(cc,c)->solid ) continue;
    /* the bounding box of a cell might be larger than the cell, which is made tight */
    if( !_zCC3DUpdateCell( cc, c ) ) return NULL;
    if( !hassolid ){
      zAABox3DCopy( &zCC3DCell(cc,c)->aabb, &solidbox );
      hassolid = true;
    } else
      zAABox3DMerge( &solidbox, &zCC3DCell(cc,c)->aabb, &solidbox );
  }
  if( !hassolid || !zPH3DAABB( ph, &phbox ) ) /* empty solid or polyhedron */
    return _zCC3DSplitPH3D( cc, ph, true, convex, NULL ) ? cc : NULL;
  if( !zColChkAABox3D( &solidbox, &phbox, cc->tol ) ){ /* apart from each other */
    cc->_tag++;
    for( c=0; c<zCC3DCellNum(cc); c++ ) zCC3DCell(cc,c)->solid = false;
    return cc;
  }
  /* the intersection of the bounding boxes, which spans the gap between them if apart within the tolerance */
  for( k=0; k<3; k++ ){
    region.min.e[k] = zMax( solidbox.min.e[k], phbox.min.e[k] );
    region.max.e[k] = zMin( solidbox.max.e[k], phbox.max.e[k] );
    if( region.min.e[k] > region.max.e[k] ) zSwap( double, region.min.e[k], region.max.e[k] );
  }
  d = zMax3( zAABox3DDepth(&region), zAABox3DWidth(&region), zAABox3DHeight(&region) );
  margin = zMax( _ZEO_CC3D_REGION_MARGIN * d, 10 * cc->tol );
  for( k=0; k<3; k++ ){
    region.min.e[k] -= margin;
    region.max.e[k] += margin;
  }
  zAABox3DFaceToPlane3D6( &region, plane );
  for( k=0; k<3; k++ )
    for( side=0; side<2; side++ ){
      if( side == 0 ? region.min.e[k] <= solidbox.min.e[k] + cc->tol : region.max.e[k] >= solidbox.max.e[k] - cc->tol )
        continue; /* the solid is inside of the plane */
      if( !zCC3DCutByPlane3D( cc, &plane[2*k+side] ) ) return NULL;
    }
  if( !_zCC3DSplitPH3D( cc, ph, true, convex, &region ) ) return NULL;
  for( c=0; c<zCC3DCellNum(cc); c++ )
    if( zCC3DCell(cc,c)->solid && !zCC3DCell(cc,c)->_cand ) zCC3DCell(cc,c)->solid = false;
  return cc;
}

zCC3D *zCC3DIntersectConvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DIntersect( cc, ph, true );
}

zCC3D *zCC3DIntersectNonconvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DIntersect( cc, ph, false );
}

zCC3D *zCC3DIntersect(zCC3D *cc, const zPH3D *ph)
{
  bool convex;

  return _zCC3DPH3DCheck( ph, &convex ) ? _zCC3DIntersect( cc, ph, convex ) : NULL;
}

/* create a 3D cell complex from a polyhedron. */
static zCC3D *_zCC3DFromPH3D(zCC3D *cc, const zPH3D *ph, bool convex)
{
  zAABox3D box;
  double d;

  zCC3DInit( cc );
  if( !zPH3DAABB( ph, &box ) ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  d = zMax3( box.max.c.x - box.min.c.x, box.max.c.y - box.min.c.y, box.max.c.z - box.min.c.z );
  box.min.c.x -= d; box.min.c.y -= d; box.min.c.z -= d;
  box.max.c.x += d; box.max.c.y += d; box.max.c.z += d;
  if( !zCC3DCreate( cc, &box ) ) return NULL;
  if( !_zCC3DMerge( cc, ph, convex ) ){
    zCC3DDestroy( cc );
    return NULL;
  }
  return cc;
}

zCC3D *zCC3DFromPH3DConvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DFromPH3D( cc, ph, true );
}

zCC3D *zCC3DFromPH3DNonconvex(zCC3D *cc, const zPH3D *ph)
{
  return _zCC3DFromPH3D( cc, ph, false );
}

zCC3D *zCC3DFromPH3D(zCC3D *cc, const zPH3D *ph)
{
  bool convex;

  zCC3DInit( cc );
  return _zCC3DPH3DCheck( ph, &convex ) ? _zCC3DFromPH3D( cc, ph, convex ) : NULL;
}

/* create a 3D cell complex whose solid is an axis-aligned box. */
zCC3D *zCC3DFromAABox3D(zCC3D *cc, const zAABox3D *box)
{
  zAABox3D universe;
  zPlane3D plane[6];
  double d;
  int k;

  zCC3DInit( cc );
  d = zMax3( box->max.c.x - box->min.c.x, box->max.c.y - box->min.c.y, box->max.c.z - box->min.c.z );
  if( !( d > 0 ) || box->min.c.x > box->max.c.x || box->min.c.y > box->max.c.y || box->min.c.z > box->max.c.z ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  for( k=0; k<3; k++ ){
    universe.min.e[k] = box->min.e[k] - d;
    universe.max.e[k] = box->max.e[k] + d;
  }
  if( !zCC3DCreate( cc, &universe ) ) return NULL;
  zCC3DCell(cc,0)->solid = true;
  zAABox3DFaceToPlane3D6( box, plane );
  for( k=0; k<6; k++ )
    if( !zCC3DCutByPlane3D( cc, &plane[k] ) ){
      zCC3DDestroy( cc );
      return NULL;
    }
  return cc;
}

/* ********************************************************** */
/* conversion to a polyhedron
 * ********************************************************** */

/* area of a triangle (twice). */
static double _zCC3DTriArea(zCC3D *cc, int a, int b, int c)
{
  zVec3D e1, e2, n;

  zVec3DSub( &_zCC3DV(cc,b)->p, &_zCC3DV(cc,a)->p, &e1 );
  zVec3DSub( &_zCC3DV(cc,c)->p, &_zCC3DV(cc,a)->p, &e2 );
  zVec3DOuterProd( &e1, &e2, &n );
  return zVec3DNorm( &n );
}

/* hash set of undirected edges between vertices of the output polyhedron, which prevents a
 * diagonal of a polygon from duplicating another edge, so that the output is closed. */
typedef struct{
  unsigned long *key; /* 0 for an empty slot */
  int cap;
} _zCC3DEdgeSet;

static bool _zCC3DEdgeSetInit(_zCC3DEdgeSet *set, int num)
{
  for( set->cap=16; set->cap<2*num; set->cap*=2 );
  if( !( set->key = zAlloc( unsigned long, set->cap ) ) ){
    ZALLOCERROR();
    return false;
  }
  return true;
}

static unsigned long *_zCC3DEdgeSetFind(_zCC3DEdgeSet *set, int u, int v)
{
  unsigned long key;
  int i;

  if( u > v ) zSwap( int, u, v );
  key = ( (unsigned long)u << 32 | (unsigned long)v ) + 1;
  for( i=(int)( ( key * 0x9e3779b97f4a7c15UL ) >> 40 ) & ( set->cap - 1 ); set->key[i] != 0 && set->key[i] != key; i=( i + 1 ) & ( set->cap - 1 ) );
  return &set->key[i];
}

#define _zCC3DEdgeSetHas(set,u,v) ( *_zCC3DEdgeSetFind( set, u, v ) != 0 )

static void _zCC3DEdgeSetAdd(_zCC3DEdgeSet *set, int u, int v)
{
  unsigned long *p;

  p = _zCC3DEdgeSetFind( set, u, v );
  *p = ( (unsigned long)zMin(u,v) << 32 | (unsigned long)zMax(u,v) ) + 1;
}

/* divide a convex polygon into triangles by cutting the largest ear one by one. An ear whose
 * diagonal is already an edge is not cut. If no ear can be cut, the rest is divided by the
 * centroid, which is a new vertex identified by c. */
static int _zCC3DTriangulate(zCC3D *cc, int *poly, int n, int *tri, _zCC3DEdgeSet *set, int c, bool *usec)
{
  int i, imax, nt = 0;
  double a, amax;

  *usec = false;
  while( n > 3 ){
    for( imax=-1, amax=-1, i=0; i<n; i++ ){
      if( _zCC3DEdgeSetHas( set, poly[(i+n-1)%n], poly[(i+1)%n] ) ) continue;
      if( ( a = _zCC3DTriArea( cc, poly[(i+n-1)%n], poly[i], poly[(i+1)%n] ) ) > amax ){
        amax = a;
        imax = i;
      }
    }
    if( imax < 0 ){
      for( i=0; i<n; i++ ){
        tri[3*nt] = poly[i]; tri[3*nt+1] = poly[(i+1)%n]; tri[3*nt+2] = c;
        nt++;
      }
      *usec = true;
      return nt;
    }
    tri[3*nt  ] = poly[(imax+n-1)%n];
    tri[3*nt+1] = poly[imax];
    tri[3*nt+2] = poly[(imax+1)%n];
    nt++;
    _zCC3DEdgeSetAdd( set, poly[(imax+n-1)%n], poly[(imax+1)%n] );
    memmove( &poly[imax], &poly[imax+1], sizeof(int)*( n - imax - 1 ) );
    n--;
  }
  if( n == 3 ){
    tri[3*nt] = poly[0]; tri[3*nt+1] = poly[1]; tri[3*nt+2] = poly[2];
    nt++;
  }
  return nt;
}

#define _zCC3DCellIsSolid(cc,c) ( (c) >= 0 && zCC3DCell(cc,c)->solid )

/* side of a face on the boundary of the solid (-1 if not on the boundary). */
static int _zCC3DBoundarySide(const zCC3D *cc, int f)
{
  bool s0, s1;

  s0 = _zCC3DCellIsSolid( cc, zCC3DFace(cc,f)->cell[0] );
  s1 = _zCC3DCellIsSolid( cc, zCC3DFace(cc,f)->cell[1] );
  return s0 == s1 ? -1 : ( s0 ? 0 : 1 );
}

/* side of a face on the boundary of the solid to be selected, namely, whose tag is equal to a given
 * tag (except=false) or not (except=true). -1 if not selected. */
static int _zCC3DSelectSide(const zCC3D *cc, int f, int tag, bool except)
{
  if( ( zCC3DFace(cc,f)->tag == tag ) == except ) return -1;
  return _zCC3DBoundarySide( cc, f );
}

/* polygon of a side of a face. */
static bool _zCC3DFacePolygon(zCC3D *cc, int f, int s, zIndexStruct *poly)
{
  int h;

  h = zCC3DFace(cc,f)->he[s];
  for( zArraySetSize( poly, 0 ); ; ){
    if( !_zCC3DIndexPush( poly, _zCC3DHE(cc,h)->org ) ) return false;
    if( ( h = _zCC3DHE(cc,h)->next ) == zCC3DFace(cc,f)->he[s] ) break;
  }
  return true;
}

/* convert faces on the boundary of the solid selected by a tag to a polyhedron. */
static zPH3D *_zCC3DToPH3DByTag(zCC3D *cc, int tag, bool except, zPH3D *ph)
{
  zIndexStruct poly, tri, cent;
  _zCC3DEdgeSet set = { NULL, 0 };
  zVec3D p;
  int i, j, s, nv = 0, nt = 0, ne = 0, n, stamp;
  bool usec;

  zPH3DInit( ph );
  zArrayInit( &poly );
  zArrayInit( &tri );
  zArrayInit( &cent );
  /* edges of the boundary */
  for( i=0; i<zCC3DFaceNum(cc); i++ ){
    if( ( s = _zCC3DSelectSide( cc, i, tag, except ) ) < 0 ) continue;
    if( !_zCC3DFacePolygon( cc, i, s, &poly ) ) goto FAILURE;
    ne += 2 * zArraySize(&poly);
  }
  if( ne == 0 ) goto FAILURE;
  if( !_zCC3DEdgeSetInit( &set, ne ) ) goto FAILURE;
  for( i=0; i<zCC3DFaceNum(cc); i++ ){
    if( ( s = _zCC3DSelectSide( cc, i, tag, except ) ) < 0 ) continue;
    _zCC3DFacePolygon( cc, i, s, &poly );
    for( j=0; j<zArraySize(&poly); j++ ) _zCC3DEdgeSetAdd( &set, zIndexElemNC(&poly,j), zIndexElemNC(&poly,(j+1)%zArraySize(&poly)) );
  }
  /* triangles, where centroids of polygons are identified by zCC3DVertNum(cc) and the followings */
  for( i=0; i<zCC3DFaceNum(cc); i++ ){
    if( ( s = _zCC3DSelectSide( cc, i, tag, except ) ) < 0 ) continue;
    if( !_zCC3DFacePolygon( cc, i, s, &poly ) ) goto FAILURE;
    if( zArraySize(&poly) < 3 ) continue;
    zArrayExpand( &tri, int, 3*( nt + zArraySize(&poly) ) );
    if( zArraySize(&tri) != 3*( nt + zArraySize(&poly) ) ) goto FAILURE;
    nt += _zCC3DTriangulate( cc, zArrayBuf(&poly), zArraySize(&poly), zArrayBuf(&tri)+3*nt, &set, zCC3DVertNum(cc) + zArraySize(&cent), &usec );
    if( usec && !_zCC3DIndexPush( &cent, i ) ) goto FAILURE;
  }
  if( nt == 0 ) goto FAILURE;
  stamp = ++cc->_stamp;
  for( i=0; i<3*nt; i++ )
    if( zIndexElemNC(&tri,i) < zCC3DVertNum(cc) && _zCC3DV(cc,zIndexElemNC(&tri,i))->_visit != stamp ){
      _zCC3DV(cc,zIndexElemNC(&tri,i))->_visit = stamp;
      _zCC3DV(cc,zIndexElemNC(&tri,i))->_side = nv++; /* temporarily used as an identifier */
    }
  if( !zPH3DAlloc( ph, nv + zArraySize(&cent), nt ) ) goto FAILURE;
  for( i=0; i<3*nt; i++ )
    if( zIndexElemNC(&tri,i) < zCC3DVertNum(cc) )
      zVec3DCopy( &_zCC3DV(cc,zIndexElemNC(&tri,i))->p, zPH3DVert(ph,_zCC3DV(cc,zIndexElemNC(&tri,i))->_side) );
  for( i=0; i<zArraySize(&cent); i++ ){
    _zCC3DFacePolygon( cc, zIndexElemNC(&cent,i), _zCC3DBoundarySide( cc, zIndexElemNC(&cent,i) ), &poly );
    zVec3DZero( &p );
    for( j=0; j<zArraySize(&poly); j++ ) zVec3DAddDRC( &p, &_zCC3DV(cc,zIndexElemNC(&poly,j))->p );
    zVec3DDiv( &p, zArraySize(&poly), zPH3DVert(ph,nv+i) );
  }
  for( i=0; i<nt; i++ ){
    for( j=0; j<3; j++ ){
      n = zIndexElemNC(&tri,3*i+j);
      n = n < zCC3DVertNum(cc) ? _zCC3DV(cc,n)->_side : nv + n - zCC3DVertNum(cc);
      zTri3DSetVert( zPH3DFace(ph,i), j, zPH3DVert(ph,n) );
    }
    zTri3DCalcNorm( zPH3DFace(ph,i) );
  }
  for( i=0; i<3*nt; i++ )
    if( zIndexElemNC(&tri,i) < zCC3DVertNum(cc) ) _zCC3DV(cc,zIndexElemNC(&tri,i))->_stamp = 0; /* reset stamps of sides */
  free( set.key );
  zArrayFree( &poly );
  zArrayFree( &tri );
  zArrayFree( &cent );
  return ph;

 FAILURE:
  free( set.key );
  zArrayFree( &poly );
  zArrayFree( &tri );
  zArrayFree( &cent );
  zPH3DDestroy( ph );
  return NULL;
}

/* convert a 3D cell complex to a polyhedron. */
zPH3D *zCC3DToPH3D(zCC3D *cc, zPH3D *ph)
{
  return _zCC3DToPH3DByTag( cc, -1, true, ph ); /* all faces, since tags are not negative */
}

/* cut the solid by a polyhedron, and convert the rest of the boundary of the solid to a polyhedron. */
static zPH3D *_zCC3DCut(zCC3D *cc, const zPH3D *ph, bool convex, zPH3D *cut)
{
  zPH3DInit( cut );
  if( !_zCC3DSubtract( cc, ph, convex ) ) return NULL;
  /* faces except those made by the subtraction */
  return _zCC3DToPH3DByTag( cc, cc->_tag, true, cut );
}

zPH3D *zCC3DCutConvex(zCC3D *cc, const zPH3D *ph, zPH3D *cut)
{
  return _zCC3DCut( cc, ph, true, cut );
}

zPH3D *zCC3DCutNonconvex(zCC3D *cc, const zPH3D *ph, zPH3D *cut)
{
  return _zCC3DCut( cc, ph, false, cut );
}

zPH3D *zCC3DCut(zCC3D *cc, const zPH3D *ph, zPH3D *cut)
{
  bool convex;

  if( !_zCC3DPH3DCheck( ph, &convex ) ){
    zPH3DInit( cut );
    return NULL;
  }
  return _zCC3DCut( cc, ph, convex, cut );
}

/* volume and first moment of the solid about a reference point, which are integrated over cones
 * from the reference point to the faces on the boundary of the solid. A side of a face is oriented
 * outward from the cell on that side. */
static double _zCC3DVolumeMoment(const zCC3D *cc, const zVec3D *ref, zVec3D *moment)
{
  zVec3D e0, e1, e2, n;
  double v, vol = 0;
  int f, s, h0, h1, h2;

  zVec3DZero( moment );
  for( f=0; f<zCC3DFaceNum(cc); f++ ){
    if( ( s = _zCC3DBoundarySide( cc, f ) ) < 0 ) continue;
    h0 = zCC3DFace(cc,f)->he[s];
    zVec3DSub( &_zCC3DV(cc,_zCC3DHE(cc,h0)->org)->p, ref, &e0 );
    for( h1=_zCC3DHE(cc,h0)->next; ( h2 = _zCC3DHE(cc,h1)->next ) != h0; h1=h2 ){
      zVec3DSub( &_zCC3DV(cc,_zCC3DHE(cc,h1)->org)->p, ref, &e1 );
      zVec3DSub( &_zCC3DV(cc,_zCC3DHE(cc,h2)->org)->p, ref, &e2 );
      zVec3DOuterProd( &e1, &e2, &n );
      vol += ( v = zVec3DInnerProd( &e0, &n ) / 6 );
      /* the barycenter of a cone relative to the reference point is (e0+e1+e2)/4 */
      zVec3DAdd( &e0, &e1, &n );
      zVec3DAddDRC( &n, &e2 );
      zVec3DCatDRC( moment, 0.25*v, &n );
    }
  }
  return vol;
}

/* volume of the solid. */
double zCC3DVolume(const zCC3D *cc)
{
  zVec3D ref, moment;

  zAABox3DCenter( &cc->universe, &ref );
  return _zCC3DVolumeMoment( cc, &ref, &moment );
}

/* barycenter of the solid. */
zVec3D *zCC3DBarycenter(const zCC3D *cc, zVec3D *c)
{
  zVec3D ref, moment;
  double vol;

  zAABox3DCenter( &cc->universe, &ref );
  if( ( vol = _zCC3DVolumeMoment( cc, &ref, &moment ) ) <= 0 ){
    zVec3DZero( c );
    return NULL;
  }
  zVec3DDivDRC( &moment, vol );
  return zVec3DAdd( &ref, &moment, c );
}

/* area normal vector of the faces on the boundary of the solid made by the last operation. */
zVec3D *zCC3DAreaNormalVec(const zCC3D *cc, zVec3D *norm)
{
  zVec3D e1, e2, n;
  const zVec3D *p0;
  int f, s, h0, h1, h2;
  bool found = false;

  zVec3DZero( norm );
  for( f=0; f<zCC3DFaceNum(cc); f++ ){
    if( ( s = _zCC3DSelectSide( cc, f, cc->_tag, false ) ) < 0 ) continue;
    found = true;
    h0 = zCC3DFace(cc,f)->he[s];
    p0 = &_zCC3DV(cc,_zCC3DHE(cc,h0)->org)->p;
    for( h1=_zCC3DHE(cc,h0)->next; ( h2 = _zCC3DHE(cc,h1)->next ) != h0; h1=h2 ){
      zVec3DSub( &_zCC3DV(cc,_zCC3DHE(cc,h1)->org)->p, p0, &e1 );
      zVec3DSub( &_zCC3DV(cc,_zCC3DHE(cc,h2)->org)->p, p0, &e2 );
      zVec3DOuterProd( &e1, &e2, &n );
      zVec3DAddDRC( norm, &n );
    }
  }
  zVec3DMulDRC( norm, 0.5 );
  return found ? norm : NULL;
}

/* ********************************************************** */
/* consistency check
 * ********************************************************** */

/* check the consistency of a 3D cell complex. */
bool zCC3DCheck(zCC3D *cc)
{
  zIndexStruct queue;
  zCC3DHalfEdge *h;
  int c, i, g, nv, nf, visit, vstamp, fstamp, *fmark = NULL;
  bool ret = false;

  zArrayInit( &queue );
  if( !( fmark = zAlloc( int, zCC3DFaceNum(cc)+1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  for( c=0; c<zCC3DCellNum(cc); c++ ){
    if( zCC3DCell(cc,c)->he < 0 || zCC3DCell(cc,c)->he >= _zCC3DHENum(cc) ) goto TERMINATE;
    visit = ++cc->_stamp;
    vstamp = ++cc->_stamp;
    fstamp = c + 1;
    zArraySetSize( &queue, 0 );
    _zCC3DHE(cc,zCC3DCell(cc,c)->he)->_aux = visit;
    if( !_zCC3DIndexPush( &queue, zCC3DCell(cc,c)->he ) ) goto TERMINATE;
    for( nv=nf=0, i=0; i<zArraySize(&queue); i++ ){
      h = _zCC3DHE(cc,zIndexElemNC(&queue,i));
      if( h->next < 0 || h->prev < 0 || h->twin < 0 ) goto TERMINATE;
      if( _zCC3DHE(cc,h->next)->prev != zIndexElemNC(&queue,i) || _zCC3DHE(cc,h->prev)->next != zIndexElemNC(&queue,i) ) goto TERMINATE;
      if( _zCC3DHE(cc,h->twin)->twin != zIndexElemNC(&queue,i) ) goto TERMINATE;
      if( _zCC3DHE(cc,h->twin)->org != _zCC3DHE(cc,h->next)->org || h->org == _zCC3DHE(cc,h->next)->org ) goto TERMINATE;
      if( _zCC3DHE(cc,h->next)->face != h->face || _zCC3DHE(cc,h->next)->side != h->side ) goto TERMINATE;
      if( zCC3DFace(cc,h->face)->cell[h->side] != c ) goto TERMINATE;
      if( h->mate >= 0 ){
        if( _zCC3DHE(cc,h->mate)->mate != zIndexElemNC(&queue,i) || _zCC3DHE(cc,h->mate)->face != h->face ||
            _zCC3DHE(cc,h->mate)->side == h->side || _zCC3DHE(cc,h->mate)->org != _zCC3DHE(cc,h->next)->org ) goto TERMINATE;
      } else
      if( zCC3DFace(cc,h->face)->cell[1-h->side] >= 0 ) goto TERMINATE;
      if( _zCC3DV(cc,h->org)->_visit != vstamp ){
        _zCC3DV(cc,h->org)->_visit = vstamp;
        nv++;
      }
      if( fmark[h->face] != fstamp ){
        fmark[h->face] = fstamp;
        nf++;
      }
      for( g=h->next; ; ){
        if( _zCC3DHE(cc,g)->_aux != visit ){
          _zCC3DHE(cc,g)->_aux = visit;
          if( !_zCC3DIndexPush( &queue, g ) ) goto TERMINATE;
        }
        if( g == h->twin ) break;
        g = h->twin;
      }
      h = _zCC3DHE(cc,zIndexElemNC(&queue,i));
    }
    if( nv - zArraySize(&queue)/2 + nf != 2 ) goto TERMINATE; /* Euler's formula */
  }
  for( i=0; i<zCC3DFaceNum(cc); i++ )
    for( c=0; c<2; c++ )
      if( ( zCC3DFace(cc,i)->cell[c] >= 0 ) != ( zCC3DFace(cc,i)->he[c] >= 0 ) ||
          ( zCC3DFace(cc,i)->he[c] >= 0 && ( _zCC3DHE(cc,zCC3DFace(cc,i)->he[c])->face != i || _zCC3DHE(cc,zCC3DFace(cc,i)->he[c])->side != c ) ) )
        goto TERMINATE;
  ret = true;
 TERMINATE:
  free( fmark );
  zArrayFree( &queue );
  return ret;
}

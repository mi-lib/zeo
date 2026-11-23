/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_convexhull - 3D bounding volume: 3D convex hull.
 */

#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* quickhull algorithm by C. Barber, D. Dobkin and H. Huhdanpaa (1996).
 * Every judgement of the side of a facet on which a point lies is done by the exact orientation
 * predicates, and a point coplanar with a facet is regarded as invisible from the facet, so that
 * no threshold is used and the topology of the convex hull is never broken by rounding errors.
 * Coplanar facets are not merged. If all points are on a plane, a virtual vertex at infinity is
 * added along an axis that is not parallel to the plane, and the same algorithm is applied; the
 * facets that do not include the virtual vertex constitute a triangulation of the planar hull.
 * A point on a face or an edge of the convex hull may remain as a vertex, since a point chosen as
 * the furthest one in an outside set is not necessarily an extreme point of the whole set. Such a
 * vertex is removed afterward by contracting an edge or by replacing facets around it, where the
 * validity is checked by the exact predicates.
 * With a positive tolerance, vertices close to the convex hull of the other vertices within the
 * tolerance are removed, and the exact convex hull of the remaining vertices is rebuilt, so that
 * thin triangles due to nearly coplanar points are avoided. Removed vertices farther than the
 * tolerance from the rebuilt convex hull are restored.
 * ********************************************************** */

typedef struct _zQHVert{
  zVec3D *v;                /* coordinates (the null pointer for the virtual vertex at infinity) */
  struct _zQHVert *next;    /* next point in an outside set */
  struct _zQHFacet *cone;   /* new facet starting from this vertex on the horizon (work) */
  struct _zQHFacet *facet;  /* a facet incident to this vertex */
  int id;                   /* identifier in the output polyhedron (work) */
  int mark;                 /* stamp for flags (work) */
  bool keep;                /* flag to keep as a vertex (work) */
  bool dirty;               /* flag to be checked if removable (work) */
} zQHVert;

typedef struct _zQHFacet{
  zQHVert *v[3];            /* vertices in counterclockwise order seen from outside */
  struct _zQHFacet *c[3];   /* c[i] is contiguous via the edge from v[i+1] to v[i+2] */
  zVec3D n;                 /* approximate unit normal vector, only to choose the furthest point */
  zVec3D *base;             /* base point to measure the distance */
  zQHVert *op;              /* outside set, the head of which is the furthest point */
  double d_max;             /* distance to the furthest point */
  int visible;              /* stamp of visibility (work) */
  int tested;               /* stamp of visibility test (work) */
  struct _zQHFacet *prev, *next; /* list of facets */
  struct _zQHFacet *link;   /* visible set or new facets (work) */
} zQHFacet;

typedef struct{
  zQHVert *vert;            /* array of points */
  int num;                  /* number of points */
  zQHVert omega;            /* virtual vertex at infinity */
  zAxis axis;               /* direction of the virtual vertex */
  bool planar;              /* flag for a planar point set */
  zQHVert *simplex[4];      /* vertices of the initial simplex */
  zQHFacet list;            /* sentinel of the list of facets */
  int stamp;                /* stamp for flags */
  zQHFacet **sf;            /* buffer of facets around a vertex (work) */
  zQHVert **sv;             /* buffer of vertices around a vertex (work) */
  int ssize;                /* size of the buffers */
} zQH;

/* virtual vertex at infinity */
#define _zQHVertIsVirtual(p) ( (p)->v == NULL )
/* check if a facet includes the virtual vertex. */
#define _zQHFacetIsVertical(f) ( _zQHVertIsVirtual( (f)->v[0] ) || _zQHVertIsVirtual( (f)->v[1] ) || _zQHVertIsVirtual( (f)->v[2] ) )

/* orientation of a facet and a point: positive if the point is visible from the facet. */
static int _zQHOrient(zQH *qh, zQHFacet *f, zQHVert *p)
{
  int i;

  for( i=0; i<3; i++ )
    if( _zQHVertIsVirtual( f->v[i] ) ) /* facet ( a, b, omega ) is vertical to the plane */
      return -zVec3DOrient2D( f->v[(i+1)%3]->v, f->v[(i+2)%3]->v, p->v, qh->axis );
  return zVec3DOrient3D( f->v[0]->v, f->v[1]->v, f->v[2]->v, p->v );
}

/* signed distance from a facet to a point, which is used only to choose the furthest point. */
static double _zQHDist(zQHFacet *f, zQHVert *p)
{
  zVec3D d;

  _zVec3DSub( p->v, f->base, &d );
  return _zVec3DInnerProd( &f->n, &d );
}

/* normalize a vector; a zero vector is kept zero. */
static void _zQHNormalize(zVec3D *v)
{
  double l;

  if( ( l = _zVec3DNorm( v ) ) > 0 ) _zVec3DDivDRC( v, l );
}

/* approximate unit normal vector and base point of a facet. */
static void _zQHFacetSetNorm(zQH *qh, zQHFacet *f)
{
  zVec3D e1, e2;
  int i;

  for( i=0; i<3; i++ )
    if( _zQHVertIsVirtual( f->v[i] ) ) break;
  if( i < 3 ){ /* vertical facet ( a, b, omega ) */
    f->base = f->v[(i+1)%3]->v;
    _zVec3DSub( f->v[(i+2)%3]->v, f->base, &e1 );
    zVec3DZero( &e2 );
    e2.e[(int)qh->axis] = 1;
  } else{
    f->base = f->v[0]->v;
    _zVec3DSub( f->v[1]->v, f->v[0]->v, &e1 );
    _zVec3DSub( f->v[2]->v, f->v[0]->v, &e2 );
  }
  _zVec3DOuterProd( &e1, &e2, &f->n );
  _zQHNormalize( &f->n );
}

/* create a new facet. */
static zQHFacet *_zQHFacetCreate(zQH *qh, zQHVert *v0, zQHVert *v1, zQHVert *v2)
{
  zQHFacet *f;

  if( !( f = zAlloc( zQHFacet, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  f->v[0] = v0; f->v[1] = v1; f->v[2] = v2;
  f->c[0] = f->c[1] = f->c[2] = NULL;
  _zQHFacetSetNorm( qh, f );
  f->op = NULL;
  f->d_max = 0;
  f->visible = f->tested = 0;
  f->prev = f->next = f->link = NULL;
  return f;
}

/* insert a facet to the list; facets with outside points come first. */
static void _zQHFacetInsert(zQH *qh, zQHFacet *f)
{
  zQHFacet *prev;

  prev = f->op ? &qh->list : qh->list.prev;
  f->prev = prev;
  f->next = prev->next;
  prev->next->prev = f;
  prev->next = f;
}

/* purge a facet from the list. */
static void _zQHFacetPurge(zQHFacet *f)
{
  f->prev->next = f->next;
  f->next->prev = f->prev;
}

/* destroy all facets in the list. */
static void _zQHFacetListDestroy(zQH *qh)
{
  zQHFacet *f;

  while( ( f = qh->list.next ) != &qh->list ){
    _zQHFacetPurge( f );
    zFree( f );
  }
}

/* destroy facets connected by links. */
static void _zQHFacetLinkDestroy(zQHFacet *f)
{
  zQHFacet *next;

  for( ; f; f=next ){
    next = f->link;
    zFree( f );
  }
}

/* assign a point to the outside set of the furthest facet that sees it among linked facets.
 * The point is discarded if it is visible from none of them. */
static void _zQHAssign(zQH *qh, zQHFacet *flist, zQHVert *p)
{
  zQHFacet *f, *fmax = NULL;
  double d, dmax = 0;

  for( f=flist; f; f=f->link )
    if( _zQHOrient( qh, f, p ) > 0 ){
      d = _zQHDist( f, p );
      if( !fmax || d > dmax ){
        fmax = f;
        dmax = d;
      }
    }
  if( !fmax ) return;
  if( !fmax->op || dmax > fmax->d_max ){
    p->next = fmax->op;
    fmax->op = p;
    fmax->d_max = dmax;
  } else{
    p->next = fmax->op->next;
    fmax->op->next = p;
  }
}

/* bind two facets contiguous via an edge. */
static bool _zQHFacetBind(zQHFacet *f, zQHFacet *g)
{
  int i, j;

  for( i=0; i<3; i++ )
    for( j=0; j<3; j++ )
      if( f->v[(i+1)%3] == g->v[(j+2)%3] && f->v[(i+2)%3] == g->v[(j+1)%3] ){
        f->c[i] = g;
        g->c[j] = f;
        return true;
      }
  return false;
}

/* initial simplex */

/* check if three points are collinear. */
static bool _zQHIsCollinear(zVec3D *a, zVec3D *b, zVec3D *c)
{
  return zVec3DOrient2D( a, b, c, zX ) == 0 &&
         zVec3DOrient2D( a, b, c, zY ) == 0 &&
         zVec3DOrient2D( a, b, c, zZ ) == 0;
}

/* find vertices of the initial simplex. The points are chosen as far as possible from each other
 * by floating-point arithmetics, while degeneracies are judged exactly.
 * It returns 4 for a non-degenerate set, 3 for a planar set, 2 for a collinear set, 1 for a set of
 * coincident points and 0 for an empty set. */
static int _zQHSimplexVert(zQH *qh, zQHVert *s[])
{
  zQHVert *vmin[3], *vmax[3];
  zVec3D d, e, n;
  double l, lmax;
  int i, k;

  if( qh->num == 0 ) return 0;
  /* the first two vertices: the farthest pair of extreme points along axes */
  for( k=0; k<3; k++ ) vmin[k] = vmax[k] = &qh->vert[0];
  for( i=1; i<qh->num; i++ )
    for( k=0; k<3; k++ ){
      if( qh->vert[i].v->e[k] < vmin[k]->v->e[k] ) vmin[k] = &qh->vert[i];
      if( qh->vert[i].v->e[k] > vmax[k]->v->e[k] ) vmax[k] = &qh->vert[i];
    }
  s[0] = s[1] = vmin[0];
  for( lmax=0, k=0; k<3; k++ )
    if( ( l = zVec3DSqrDist( vmin[k]->v, vmax[k]->v ) ) > lmax ){
      lmax = l;
      s[0] = vmin[k];
      s[1] = vmax[k];
    }
  if( lmax == 0 ) return 1; /* all points coincide */
  /* the third vertex: the farthest point from the line */
  _zVec3DSub( s[1]->v, s[0]->v, &e );
  for( s[2]=NULL, lmax=0, i=0; i<qh->num; i++ ){
    _zVec3DSub( qh->vert[i].v, s[0]->v, &d );
    _zVec3DOuterProd( &e, &d, &n );
    if( ( l = _zVec3DSqrNorm( &n ) ) > lmax ){
      lmax = l;
      s[2] = &qh->vert[i];
    }
  }
  if( !s[2] || _zQHIsCollinear( s[0]->v, s[1]->v, s[2]->v ) ){
    for( s[2]=NULL, i=0; i<qh->num; i++ )
      if( !_zQHIsCollinear( s[0]->v, s[1]->v, qh->vert[i].v ) ){
        s[2] = &qh->vert[i];
        break;
      }
    if( !s[2] ) return 2; /* all points are collinear */
  }
  /* the fourth vertex: the farthest point from the plane */
  _zVec3DSub( s[2]->v, s[0]->v, &d );
  _zVec3DOuterProd( &e, &d, &n );
  for( s[3]=NULL, lmax=0, i=0; i<qh->num; i++ ){
    _zVec3DSub( qh->vert[i].v, s[0]->v, &d );
    if( ( l = fabs( _zVec3DInnerProd( &n, &d ) ) ) > lmax ){
      lmax = l;
      s[3] = &qh->vert[i];
    }
  }
  if( !s[3] || zVec3DOrient3D( s[0]->v, s[1]->v, s[2]->v, s[3]->v ) == 0 ){
    for( s[3]=NULL, i=0; i<qh->num; i++ )
      if( zVec3DOrient3D( s[0]->v, s[1]->v, s[2]->v, qh->vert[i].v ) != 0 ){
        s[3] = &qh->vert[i];
        break;
      }
    if( !s[3] ){ /* all points are on a plane */
      /* the virtual vertex is put at infinity along the axis the most orthogonal to the plane,
         the projection along which keeps the triangle non-degenerate. */
      qh->axis = fabs( n.c.x ) > fabs( n.c.y ) ? ( fabs( n.c.x ) > fabs( n.c.z ) ? zX : zZ ) : ( fabs( n.c.y ) > fabs( n.c.z ) ? zY : zZ );
      for( k=0; k<3 && zVec3DOrient2D( s[0]->v, s[1]->v, s[2]->v, qh->axis ) == 0; k++ )
        qh->axis = ( qh->axis + 1 ) % 3;
      s[3] = &qh->omega;
      return 3;
    }
  }
  return 4;
}

/* create the initial simplex. */
static int _zQHSimplex(zQH *qh, zQHFacet **flist)
{
  zQHVert *s[4];
  zQHFacet *f[4] = { NULL, NULL, NULL, NULL };
  int dim, o, i, j;

  *flist = NULL;
  if( ( dim = _zQHSimplexVert( qh, s ) ) < 3 ) return dim;
  qh->planar = ( dim == 3 );
  /* orient the simplex so that s[3] is beneath the facet ( s[0], s[1], s[2] ) */
  o = qh->planar ? zVec3DOrient2D( s[0]->v, s[1]->v, s[2]->v, qh->axis ) : zVec3DOrient3D( s[0]->v, s[1]->v, s[2]->v, s[3]->v );
  if( o > 0 ) zSwap( zQHVert*, s[0], s[1] );
  for( i=0; i<4; i++ ) qh->simplex[i] = s[i];
  if( !( f[0] = _zQHFacetCreate( qh, s[0], s[1], s[2] ) ) ||
      !( f[1] = _zQHFacetCreate( qh, s[0], s[3], s[1] ) ) ||
      !( f[2] = _zQHFacetCreate( qh, s[1], s[3], s[2] ) ) ||
      !( f[3] = _zQHFacetCreate( qh, s[2], s[3], s[0] ) ) ){
    for( i=0; i<4; i++ ) zFree( f[i] );
    return -1;
  }
  for( i=0; i<4; i++ ){
    for( j=i+1; j<4; j++ ) _zQHFacetBind( f[i], f[j] );
    f[i]->link = i < 3 ? f[i+1] : NULL;
  }
  *flist = f[0];
  /* assign the other points to the outside sets */
  for( i=0; i<qh->num; i++ )
    if( &qh->vert[i] != s[0] && &qh->vert[i] != s[1] && &qh->vert[i] != s[2] && &qh->vert[i] != s[3] )
      _zQHAssign( qh, *flist, &qh->vert[i] );
  for( i=0; i<4; i++ ) _zQHFacetInsert( qh, f[i] );
  return dim;
}

/* incremental construction */

/* collect the visible set of facets from a point by the breadth-first search from a facet. */
static zQHFacet *_zQHVisibleSet(zQH *qh, zQHFacet *f, zQHVert *p)
{
  zQHFacet *g, *h, *tail;
  int i;

  f->visible = f->tested = ++qh->stamp;
  f->link = NULL;
  for( tail=g=f; g; g=g->link )
    for( i=0; i<3; i++ ){
      if( ( h = g->c[i] )->tested == qh->stamp ) continue;
      h->tested = qh->stamp;
      if( _zQHOrient( qh, h, p ) > 0 ){
        h->visible = qh->stamp;
        h->link = NULL;
        tail = tail->link = h;
      }
    }
  return f;
}

/* create a cone of new facets from a point and the horizon of the visible set. */
static zQHFacet *_zQHCone(zQH *qh, zQHFacet *vs, zQHVert *p)
{
  zQHFacet *g, *h, *nf, *cone = NULL;
  zQHVert *a;
  int i, j, n = 0;

  /* a new facet for each horizon ridge */
  for( g=vs; g; g=g->link )
    for( i=0; i<3; i++ ){
      if( ( h = g->c[i] )->visible == qh->stamp ) continue;
      a = g->v[(i+1)%3];
      if( a->cone ) goto FAILURE; /* the horizon is not a simple cycle */
      if( !( nf = _zQHFacetCreate( qh, p, a, g->v[(i+2)%3] ) ) ) goto FAILURE;
      nf->link = cone;
      cone = a->cone = nf;
      n++;
      /* bind with the invisible facet beyond the ridge */
      for( j=0; j<3; j++ )
        if( h->c[j] == g ) break;
      if( j == 3 ) goto FAILURE;
      nf->c[0] = h;
      h->c[j] = nf;
    }
  if( !cone ) goto FAILURE;
  /* bind new facets with each other */
  for( nf=cone; nf; nf=nf->link ){
    if( !( g = nf->v[2]->cone ) || g->c[2] ) goto FAILURE;
    nf->c[1] = g;
    g->c[2] = nf;
  }
  /* check if the horizon is a single cycle */
  for( i=0, g=cone; i<n; i++, g=g->c[1] )
    if( i > 0 && g == cone ) goto FAILURE;
  if( g != cone ) goto FAILURE;
  for( nf=cone; nf; nf=nf->link ) nf->v[1]->cone = NULL;
  return cone;

 FAILURE:
  ZRUNERROR( ZEO_ERR_FATAL );
  for( nf=cone; nf; nf=nf->link ) nf->v[1]->cone = NULL;
  _zQHFacetLinkDestroy( cone );
  return NULL;
}

/* add the furthest point of a facet to the convex hull. */
static bool _zQHAddPoint(zQH *qh, zQHFacet *f)
{
  zQHFacet *vs, *cone, *g, *next;
  zQHVert *p, *q, *qnext;

  p = f->op;
  f->op = p->next;
  vs = _zQHVisibleSet( qh, f, p );
  if( !( cone = _zQHCone( qh, vs, p ) ) ) return false;
  /* reassign outside points of the visible set to the new facets, and delete the visible set */
  for( g=vs; g; g=next ){
    next = g->link;
    for( q=g->op; q; q=qnext ){
      qnext = q->next;
      _zQHAssign( qh, cone, q );
    }
    _zQHFacetPurge( g );
    zFree( g );
  }
  for( g=cone; g; g=next ){
    next = g->link;
    _zQHFacetInsert( qh, g );
  }
  return true;
}

/* create the convex hull of a set of points. */
static int _zQHBuild(zQH *qh)
{
  zQHFacet *flist;
  int dim;

  qh->list.prev = qh->list.next = &qh->list;
  qh->list.op = NULL;
  qh->omega.v = NULL;
  qh->omega.cone = NULL;
  qh->stamp = 0;
  qh->planar = false;
  qh->sf = NULL;
  qh->sv = NULL;
  qh->ssize = 0;
  if( ( dim = _zQHSimplex( qh, &flist ) ) < 3 ) return dim;
  while( qh->list.next->op )
    if( !_zQHAddPoint( qh, qh->list.next ) ) return -1;
  return dim;
}

/* destroy the convex hull. */
static void _zQHDestroy(zQH *qh)
{
  _zQHFacetListDestroy( qh );
  zFree( qh->sf );
  zFree( qh->sv );
}

/* removal of redundant vertices */

/* index of a vertex in a facet. */
static int _zQHFacetVertIndex(zQHFacet *f, zQHVert *v)
{
  return f->v[0] == v ? 0 : ( f->v[1] == v ? 1 : ( f->v[2] == v ? 2 : -1 ) );
}

/* vertex of a facet opposite to an edge shared with another facet. */
static zQHVert *_zQHFacetOpposite(zQHFacet *f, zQHFacet *g)
{
  return f->c[0] == g ? f->v[0] : ( f->c[1] == g ? f->v[1] : ( f->c[2] == g ? f->v[2] : NULL ) );
}

/* replace a contiguous facet of a facet. */
static bool _zQHFacetReplaceContig(zQHFacet *f, zQHFacet *g_old, zQHFacet *g_new)
{
  int i;

  for( i=0; i<3; i++ )
    if( f->c[i] == g_old ){
      f->c[i] = g_new;
      return true;
    }
  return false;
}

/* collect facets and vertices around a vertex in counterclockwise order:
 * the j-th facet is ( v, the j-th vertex, the (j+1)-th vertex ). */
static int _zQHStar(zQH *qh, zQHVert *v)
{
  zQHFacet *f;
  zQHFacet **sf;
  zQHVert **sv;
  int i, n = 0;

  f = v->facet;
  do{
    if( ( i = _zQHFacetVertIndex( f, v ) ) < 0 ) return -1;
    if( n >= qh->ssize ){
      if( !( sf = zRealloc( qh->sf, zQHFacet*, qh->ssize*2 ) ) ) return -1;
      qh->sf = sf;
      if( !( sv = zRealloc( qh->sv, zQHVert*, qh->ssize*2 ) ) ) return -1;
      qh->sv = sv;
      qh->ssize *= 2;
    }
    qh->sf[n] = f;
    qh->sv[n++] = f->v[(i+1)%3];
    f = f->c[(i+1)%3];
  } while( f != v->facet );
  return n;
}

/* distance from a point to a triangle. */
static double _zQHDistTri(zQHFacet *f, zVec3D *p)
{
  zTri3D t;

  zTri3DSetVert( &t, 0, f->v[0]->v );
  zTri3DSetVert( &t, 1, f->v[1]->v );
  zTri3DSetVert( &t, 2, f->v[2]->v );
  zTri3DSetNorm( &t, &f->n );
  return zTri3DDistFromPoint( &t, p );
}

/* check if a point is beneath or on a patch of facets. */
static bool _zQHPatchIsBeneath(zQHFacet *patch, zQHVert *p)
{
  zQHFacet *f;

  for( f=patch; f; f=f->link )
    if( zVec3DOrient3D( f->v[0]->v, f->v[1]->v, f->v[2]->v, p->v ) > 0 ) return false;
  return true;
}


/* check if a vertex is not an extreme point, namely, if it is on a face or an edge of the convex
 * hull. The star of the vertex is assumed to be stored in the buffers of qh with m facets.
 * Contiguous facets around the vertex are coplanar or not, which is judged exactly. The vertex is
 * on a face if all facets are coplanar, and on an edge if the facets are on two planes, where the
 * two edges between them are opposite to each other on the intersection line of the planes. */
static bool _zQHIsRedundant(zQH *qh, zQHVert *v, int m)
{
  zQHVert *crease[2];
  zVec3D d1, d2;
  int j, n = 0;

  for( j=0; j<m; j++ )
    if( zVec3DOrient3D( v->v, qh->sv[j]->v, qh->sv[(j+1)%m]->v, qh->sv[(j+2)%m]->v ) != 0 ){
      if( n == 2 ) return false; /* more than two planes */
      crease[n++] = qh->sv[(j+1)%m];
    }
  if( n == 0 ) return true; /* on a face */
  if( n == 1 ) return false; /* not happen */
  _zVec3DSub( crease[0]->v, v->v, &d1 );
  _zVec3DSub( crease[1]->v, v->v, &d2 );
  return _zVec3DInnerProd( &d1, &d2 ) < 0; /* on an edge */
}

/* mark a vertex and vertices around it to be checked. */
static void _zQHDirtyAround(zQHVert *v)
{
  zQHFacet *f;
  int i;

  v->dirty = true;
  if( !( f = v->facet ) ) return;
  do{
    if( ( i = _zQHFacetVertIndex( f, v ) ) < 0 ) return;
    f->v[(i+1)%3]->dirty = true;
    f = f->c[(i+1)%3];
  } while( f != v->facet );
}

/* check if a point is beneath or on new facets ( u, l[j], l[j+1] ) for j = t+1, ..., t-2. */
static bool _zQHFanIsBeneath(zQH *qh, zQHVert *u, int m, int t, zQHVert *p)
{
  zQHVert **l = qh->sv;
  int j, k;

  for( k=1; k<m-1; k++ ){
    j = ( t + k ) % m;
    if( zVec3DOrient3D( u->v, l[j]->v, l[(j+1)%m]->v, p->v ) > 0 ) return false;
  }
  return true;
}

/* check if a point d is beyond the plane of a triangle ( a, b, c ). */
#define _zQHIsBeyond(a,b,c,d) ( zVec3DOrient3D( (a)->v, (b)->v, (c)->v, (d)->v ) > 0 )

/* check the link condition of the edge from a vertex v to the t-th vertex u around it, which is
 * necessary to contract the edge: vertices adjacent to both v and u have to be l[t-1] and l[t+1]
 * only. The star of v is assumed to be stored in the buffers of qh with m facets. */
static bool _zQHContractIsLinkValid(zQH *qh, int m, int t)
{
  zQHVert **l = qh->sv, *u = qh->sv[t];
  zQHFacet *g;
  int i, k;

  qh->stamp++;
  g = u->facet;
  do{
    if( ( i = _zQHFacetVertIndex( g, u ) ) < 0 ) return false;
    g->v[(i+1)%3]->mark = qh->stamp;
    g = g->c[(i+1)%3];
  } while( g != u->facet );
  for( k=2; k<m-1; k++ )
    if( l[(t+k)%m]->mark == qh->stamp ) return false;
  return true;
}

/* check if the new facets ( u, l[j], l[j+1] ) for j = t+1, ..., t-2, which replace the facets
 * around v by contracting the edge from v to u = l[t], are not flipped and are convex with the
 * facets beyond them and with each other. */
static bool _zQHContractFanIsValid(zQH *qh, zQHVert *v, int m, int t)
{
  zQHFacet *g, *h;
  zQHVert **l = qh->sv, *u = qh->sv[t], *w;
  int j, k, i, ax, o, iv;
  double nmax;

  for( k=1; k<m-1; k++ ){
    j = ( t + k ) % m;
    g = qh->sf[j];
    /* no flip: the orientation projected onto a plane is kept. */
    iv = _zQHFacetVertIndex( g, v );
    nmax = -1;
    for( ax=0, i=0; i<3; i++ )
      if( fabs( g->n.e[i] ) > nmax ){
        nmax = fabs( g->n.e[i] );
        ax = i;
      }
    if( ( o = zVec3DOrient2D( v->v, l[j]->v, l[(j+1)%m]->v, ax ) ) == 0 ) return false;
    if( zVec3DOrient2D( u->v, l[j]->v, l[(j+1)%m]->v, ax ) != o ) return false;
    /* convexity with the facet beyond the opposite edge */
    h = g->c[iv];
    if( _zQHFacetVertIndex( h, u ) >= 0 ) return false;
    if( !( w = _zQHFacetOpposite( h, g ) ) ) return false;
    if( _zQHIsBeyond( u, l[j], l[(j+1)%m], w ) ) return false;
    if( _zQHIsBeyond( h->v[0], h->v[1], h->v[2], u ) ) return false;
    /* convexity with the next new facet */
    if( k < m - 2 && _zQHIsBeyond( u, l[j], l[(j+1)%m], l[(j+2)%m] ) ) return false;
  }
  return true;
}

/* check if a new facet ( u, a, b ) is convex with the facet g beyond a contracted facet f, where c is
 * the vertex of the new facet not on the edge shared with g after the contraction. */
static bool _zQHContractSideIsConvex(zQHFacet *f, zQHFacet *g, zQHVert *u, zQHVert *a, zQHVert *b, zQHVert *c)
{
  zQHVert *w;

  if( !( w = _zQHFacetOpposite( g, f ) ) || w == c ) return false;
  if( _zQHIsBeyond( u, a, b, w ) ) return false;
  return !_zQHIsBeyond( g->v[0], g->v[1], g->v[2], c );
}

/* contract the edge from a vertex v to the t-th vertex u around it, where facets fa = ( v, l[t-1], u )
 * and fb = ( v, u, l[t+1] ) are removed, and ga and gb are the facets beyond them, respectively. */
static void _zQHContractExec(zQH *qh, zQHVert *v, int m, int t, zQHFacet *fa, zQHFacet *fb, zQHFacet *ga, zQHFacet *gb)
{
  zQHFacet **sf = qh->sf, *g;
  zQHVert **l = qh->sv, *u = qh->sv[t];
  int k;

  for( k=1; k<m-1; k++ ){
    g = sf[(t+k)%m];
    g->v[_zQHFacetVertIndex( g, v )] = u;
    _zQHFacetSetNorm( qh, g );
  }
  g = sf[(t+m-2)%m]; /* ( u, l[t-2], l[t-1] ) */
  _zQHFacetReplaceContig( g, fa, ga );
  _zQHFacetReplaceContig( ga, fa, g );
  g = sf[(t+1)%m];   /* ( u, l[t+1], l[t+2] ) */
  _zQHFacetReplaceContig( g, fb, gb );
  _zQHFacetReplaceContig( gb, fb, g );
  u->facet = l[(t+m-1)%m]->facet = sf[(t+m-2)%m];
  l[(t+1)%m]->facet = sf[(t+1)%m];
  v->facet = NULL;
  _zQHFacetPurge( fa ); zFree( fa );
  _zQHFacetPurge( fb ); zFree( fb );
  for( k=0; k<m; k++ ) _zQHDirtyAround( l[k] );
}

/* contract the edge from a vertex v to the t-th vertex around it, if valid. The star of v is assumed
 * to be stored in the buffers of qh with m facets. */
static bool _zQHContract(zQH *qh, zQHVert *v, int m, int t)
{
  zQHFacet *fa, *fb, *ga, *gb;
  zQHVert **l = qh->sv, *u = qh->sv[t];

  fa = qh->sf[(t+m-1)%m]; /* ( v, l[t-1], u ) */
  fb = qh->sf[t];         /* ( v, u, l[t+1] ) */
  ga = fa->c[_zQHFacetVertIndex( fa, v )]; /* beyond ( l[t-1], u ) */
  gb = fb->c[_zQHFacetVertIndex( fb, v )]; /* beyond ( u, l[t+1] ) */
  if( !_zQHContractIsLinkValid( qh, m, t ) ||
      !_zQHContractFanIsValid( qh, v, m, t ) ||
      !_zQHContractSideIsConvex( fa, ga, u, l[(t+m-2)%m], l[(t+m-1)%m], l[(t+m-2)%m] ) ||
      !_zQHContractSideIsConvex( fb, gb, u, l[(t+1)%m], l[(t+2)%m], l[(t+2)%m] ) ) return false;
  /* the removed vertex has to be on the new facets, so that the convex hull is not changed. */
  if( !_zQHFanIsBeneath( qh, u, m, t, v ) ) return false;
  _zQHContractExec( qh, v, m, t, fa, fb, ga, gb );
  return true;
}

/* add a point to a sub problem if not added yet. */
static void _zQHSubAdd(zQH *qh, zQHVert **q, int *nq, zQHVert *p)
{
  if( p->mark == qh->stamp ) return;
  p->mark = qh->stamp;
  q[(*nq)++] = p;
}

/* a region of facets to be replaced */
typedef struct{
  zQHFacet **f;     /* facets */
  int n;            /* number of facets */
  int size;         /* size of the buffer */
  int stamp;        /* stamp of facets in the region */
  zQHVert **q;      /* vertices of the region and points of the sub problem (work) */
  int nq;           /* number of the points */
  int nb;           /* number of boundary edges (work) */
  zQHFacet *patch;  /* facets to replace the region (work) */
} zQHRegion;

/* add a facet to a region. */
static bool _zQHRegionAdd(zQHRegion *reg, zQHFacet *f)
{
  zQHFacet **fp;

  if( f->visible == reg->stamp ) return true;
  if( reg->n >= reg->size ){
    if( !( fp = zRealloc( reg->f, zQHFacet*, reg->size*2 ) ) ){
      ZALLOCERROR();
      return false;
    }
    reg->f = fp;
    reg->size *= 2;
  }
  f->visible = reg->stamp;
  reg->f[reg->n++] = f;
  return true;
}

/* collect vertices of a region except v. The vertices are reset as id = 0 and without cone. */
static void _zQHRegionCollectVert(zQH *qh, zQHRegion *reg, zQHVert *v)
{
  zQHVert *w;
  int j, k, vstamp;

  vstamp = ++qh->stamp;
  v->mark = vstamp; /* v is excluded */
  v->id = 0;
  v->cone = NULL;
  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ ){
      w = reg->f[j]->v[k];
      if( w->mark != vstamp ){
        w->mark = vstamp;
        w->id = 0;
        w->cone = NULL;
        reg->q[reg->nq++] = w;
      }
    }
}

/* check if the boundary edges of a region form a single cycle that does not pass a vertex v.
 * A boundary vertex is marked with id = 1, and has the facet with the boundary edge starting from
 * it as cone. */
static bool _zQHRegionBoundaryIsCycle(zQHRegion *reg, zQHVert *v)
{
  zQHFacet *f;
  zQHVert *a, *w;
  int i, j, k;

  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ ){
      if( reg->f[j]->c[k]->visible == reg->stamp ) continue;
      a = reg->f[j]->v[(k+1)%3];
      if( a == v || a->cone ) return false;
      a->cone = reg->f[j];
      a->id = 1;
      reg->nb++;
    }
  for( a=NULL, j=0; j<reg->nq; j++ ) /* find a boundary vertex */
    if( reg->q[j]->id == 1 ){
      a = reg->q[j];
      break;
    }
  if( !a ) return false;
  for( w=a, i=0; i<reg->nb; i++ ){
    f = w->cone;
    k = _zQHFacetVertIndex( f, w );
    w = f->v[(k+1)%3];
    if( !w->cone || ( i < reg->nb - 1 && w == a ) ) return false;
  }
  return w == a;
}

/* build the exact convex hull of the vertices of a region except v and the vertices of the initial
 * simplex as the points beneath the region. It returns 1 if a non-degenerate convex hull is built,
 * 0 if not, and -1 if failed. */
static int _zQHRegionSubHull(zQH *qh, zQHRegion *reg, zQHVert *v, zQH *sub)
{
  int i;

  qh->stamp++;
  for( i=0; i<reg->nq; i++ ) reg->q[i]->mark = qh->stamp;
  v->mark = qh->stamp;
  for( i=0; i<4; i++ ) _zQHSubAdd( qh, reg->q, &reg->nq, qh->simplex[i] );
  if( !( sub->vert = zAlloc( zQHVert, reg->nq ) ) ){
    ZALLOCERROR();
    return -1;
  }
  for( sub->num=reg->nq, i=0; i<reg->nq; i++ ){
    sub->vert[i].v = reg->q[i]->v;
    sub->vert[i].next = NULL;
    sub->vert[i].cone = NULL;
  }
  return _zQHBuild( sub ) == 4 ? 1 : 0;
}

/* create the patch, namely, the facets of the sub convex hull that consist only of the vertices of
 * the region. It returns 1 if created, 0 if empty, and -1 if failed. */
static int _zQHRegionCreatePatch(zQH *qh, zQHRegion *reg, zQH *sub)
{
  zQHFacet *f, *g;
  zQHVert **q = reg->q;
  int i, j, k, vstamp;

  for( i=0; i<reg->nq; i++ ) q[i]->mark = 0;
  vstamp = ++qh->stamp;
  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ ) reg->f[j]->v[k]->mark = vstamp;
  for( f=sub->list.next; f!=&sub->list; f=f->next ){
    for( k=0; k<3; k++ )
      if( q[f->v[k]-sub->vert]->mark != vstamp ) break;
    if( k < 3 ) continue;
    if( !( g = _zQHFacetCreate( qh, q[f->v[0]-sub->vert], q[f->v[1]-sub->vert], q[f->v[2]-sub->vert] ) ) )
      return -1;
    g->visible = -1; /* mark of the patch */
    g->link = reg->patch;
    reg->patch = g;
  }
  return reg->patch ? 1 : 0;
}

/* bind facets of the patch to each other and to the facets beyond the boundary of the region, and
 * check if the patch is bounded by the boundary and is convex with the facets beyond it. */
static bool _zQHPatchBind(zQHRegion *reg)
{
  zQHFacet *f, *g, *h;
  zQHVert *a, *b, *w;
  int i, k, nbu = 0;

  for( g=reg->patch; g; g=g->link )
    for( k=0; k<3; k++ ){
      a = g->v[(k+1)%3];
      b = g->v[(k+2)%3];
      if( a->id == 1 && ( f = a->cone ) && f->v[(_zQHFacetVertIndex( f, a )+1)%3] == b ){ /* boundary edge */
        h = f->c[(_zQHFacetVertIndex( f, a )+2)%3];
        if( !( w = _zQHFacetOpposite( h, f ) ) ) return false;
        /* convexity with the facet beyond the boundary */
        if( zVec3DOrient3D( g->v[0]->v, g->v[1]->v, g->v[2]->v, w->v ) > 0 ||
            zVec3DOrient3D( h->v[0]->v, h->v[1]->v, h->v[2]->v, g->v[k]->v ) > 0 ) return false;
        g->c[k] = h;
        nbu++;
        continue;
      }
      for( h=reg->patch; h; h=h->link ){ /* internal edge */
        if( h == g ) continue;
        if( ( i = _zQHFacetVertIndex( h, b ) ) >= 0 && h->v[(i+1)%3] == a ){
          g->c[k] = h;
          break;
        }
      }
      if( !h ) return false;
    }
  return nbu == reg->nb;
}

/* check if the patch is a disk by Euler's formula, which excludes another component. The vertices
 * of the patch are marked with the current stamp of qh. */
static bool _zQHPatchIsDisk(zQH *qh, zQHRegion *reg)
{
  zQHFacet *g;
  int np, ni, k, vstamp;

  vstamp = ++qh->stamp;
  for( np=0, ni=0, g=reg->patch; g; g=g->link, np++ )
    for( k=0; k<3; k++ )
      if( g->v[k]->mark != vstamp ){
        g->v[k]->mark = vstamp;
        if( g->v[k]->id == 0 ) ni++;
      }
  return np == reg->nb + 2*ni - 2;
}

/* list vertices of a region not included in the patch, which are to be removed, from v via next.
 * They are marked with -1. */
static void _zQHRegionCollectRemoved(zQH *qh, zQHRegion *reg, zQHVert *v)
{
  zQHVert *w;
  int j, k;

  v->next = NULL;
  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ ){
      w = reg->f[j]->v[k];
      if( w->mark == qh->stamp || w->mark == -1 ) continue;
      w->mark = -1; /* removed */
      if( w != v ){
        w->next = v->next;
        v->next = w;
      }
    }
}

/* cancel the removal of vertices of a region. */
static void _zQHRegionCancelRemoved(zQHRegion *reg, zQHVert *v)
{
  int j, k;

  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ )
      if( reg->f[j]->v[k]->mark == -1 ) reg->f[j]->v[k]->mark = 0;
  v->next = NULL;
}

/* replace a region with the patch. */
static void _zQHRegionSwap(zQH *qh, zQHRegion *reg, zQHVert *v)
{
  zQHFacet *g, *h, *next;
  zQHVert *w;
  int j, k;

  for( g=reg->patch; g; g=g->link )
    for( k=0; k<3; k++ ){
      g->v[k]->facet = g;
      if( ( h = g->c[k] )->visible == -1 ) continue; /* internal */
      _zQHFacetReplaceContig( h, g->v[(k+1)%3]->cone, g );
    }
  for( w=v; w; w=w->next ) w->facet = NULL;
  for( j=0; j<reg->n; j++ ){
    _zQHFacetPurge( reg->f[j] );
    zFree( reg->f[j] );
  }
  reg->n = 0;
  for( g=reg->patch; g; g=g->link ) g->visible = 0;
  for( g=reg->patch; g; g=g->link )
    for( k=0; k<3; k++ ) _zQHDirtyAround( g->v[k] );
  for( g=reg->patch; g; g=next ){
    next = g->link;
    _zQHFacetInsert( qh, g );
  }
  reg->patch = NULL;
}

/* replace a region of facets including a vertex v with the facets of the exact convex hull of the
 * vertices of the region except v and some points beneath them, if the boundary of the region is
 * kept and the removed vertices are on the convex hull. It returns 1 if replaced, 0 if not, and -1
 * if failed. */
static int _zQHRegionReplace(zQH *qh, zQHRegion *reg, zQHVert *v)
{
  zQH sub;
  zQHVert *w;
  int i, j, k, ret;

  if( !( reg->q = zAlloc( zQHVert*, 3*reg->n+4 ) ) ){
    ZALLOCERROR();
    return -1;
  }
  reg->nq = reg->nb = 0;
  reg->patch = NULL;
  sub.vert = NULL;
  _zQHRegionCollectVert( qh, reg, v );
  if( !_zQHRegionBoundaryIsCycle( reg, v ) ){
    ret = 0;
    goto TERMINATE;
  }
  if( ( ret = _zQHRegionSubHull( qh, reg, v, &sub ) ) <= 0 ||
      ( ret = _zQHRegionCreatePatch( qh, reg, &sub ) ) <= 0 ) goto TERMINATE;
  ret = 0;
  if( !_zQHPatchBind( reg ) || !_zQHPatchIsDisk( qh, reg ) ) goto TERMINATE;
  _zQHRegionCollectRemoved( qh, reg, v );
  /* the removed vertices have to be on the patch, so that the convex hull is not changed. */
  for( w=v; w; w=w->next )
    if( !_zQHPatchIsBeneath( reg->patch, w ) ){
      _zQHRegionCancelRemoved( reg, v );
      goto TERMINATE;
    }
  _zQHRegionSwap( qh, reg, v );
  ret = 1;
 TERMINATE:
  for( i=0; i<reg->nq; i++ ) reg->q[i]->cone = NULL;
  for( j=0; j<reg->n; j++ )
    for( k=0; k<3; k++ ) reg->f[j]->v[k]->cone = NULL;
  _zQHFacetLinkDestroy( reg->patch );
  reg->patch = NULL;
  if( sub.vert ){
    _zQHDestroy( &sub );
    zFree( sub.vert );
  }
  zFree( reg->q );
  return ret;
}

/* maximum number of expansions of a region around a vertex to be removed */
#define ZEO_QH_REGION_EXPANSION 2

/* remove a vertex v, the star of which is stored in the buffers of qh with m facets, by replacing
 * a region around v. It returns 1 if v is removed, 0 if not, and -1 if failed. */
static int _zQHRemoveVert(zQH *qh, zQHRegion *reg, zQHVert *v, int m)
{
  int i, j, k, n, ret;

  /* contract an edge from v */
  for( i=0; i<m; i++ )
    if( _zQHContract( qh, v, m, i ) ) return 1;
  /* replace a region around v */
  reg->n = 0;
  reg->stamp = ++qh->stamp;
  for( j=0; j<m; j++ )
    if( !_zQHRegionAdd( reg, qh->sf[j] ) ) return -1;
  for( i=0; ; i++ ){
    if( ( ret = _zQHRegionReplace( qh, reg, v ) ) != 0 || i == ZEO_QH_REGION_EXPANSION ) break;
    /* expand the region by the facets contiguous to it */
    for( n=reg->n, j=0; j<n; j++ )
      for( k=0; k<3; k++ )
        if( !_zQHRegionAdd( reg, reg->f[j]->c[k] ) ) return -1;
  }
  /* reset the stamp of the region */
  for( j=0; j<reg->n; j++ ) reg->f[j]->visible = 0;
  return ret;
}

/* remove vertices on faces and edges of the convex hull. */
static bool _zQHSimplify(zQH *qh)
{
  zQHFacet *f;
  zQHVert *v;
  zQHRegion reg;
  int i, m, ret;
  bool changed, result = false;

  for( i=0; i<qh->num; i++ ){
    qh->vert[i].facet = NULL;
    qh->vert[i].next = NULL;
    qh->vert[i].mark = 0;
    qh->vert[i].cone = NULL;
    qh->vert[i].dirty = true;
  }
  for( f=qh->list.next; f!=&qh->list; f=f->next ){
    f->op = NULL;
    f->visible = 0;
    for( i=0; i<3; i++ ) f->v[i]->facet = f;
  }
  zFree( qh->sf );
  zFree( qh->sv );
  qh->ssize = reg.size = 16;
  qh->sf = zAlloc( zQHFacet*, qh->ssize );
  qh->sv = zAlloc( zQHVert*, qh->ssize );
  reg.f = zAlloc( zQHFacet*, reg.size );
  if( !qh->sf || !qh->sv || !reg.f ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  do{
    changed = false;
    for( i=0; i<qh->num; i++ ){
      if( !( v = &qh->vert[i] )->facet || !v->dirty ) continue;
      v->dirty = false;
      if( ( m = _zQHStar( qh, v ) ) < 0 ) goto TERMINATE;
      if( !_zQHIsRedundant( qh, v, m ) ) continue;
      if( ( ret = _zQHRemoveVert( qh, &reg, v, m ) ) < 0 ) goto TERMINATE;
      if( ret > 0 ) changed = true;
    }
  } while( changed );
  result = true;
 TERMINATE:
  zFree( reg.f );
  return result;
}

/* check if a point is inside of a convex hull or within the tolerance from it. */
static bool _zQHIsWithinTol(zQH *qh, zQHVert *p, double tol)
{
  zQHFacet *f;
  double d, dmin = HUGE_VAL;

  if( !qh->planar ){
    for( f=qh->list.next; f!=&qh->list; f=f->next )
      if( _zQHOrient( qh, f, p ) > 0 ) break;
    if( f == &qh->list ) return true;
  }
  for( f=qh->list.next; f!=&qh->list; f=f->next )
    if( !_zQHFacetIsVertical( f ) && ( d = _zQHDistTri( f, p->v ) ) < dmin ) dmin = d;
  return dmin <= tol;
}

/* push a vertex to a buffer. */
static bool _zQHPush(zQHVert ***buf, int *n, int *size, zQHVert *v)
{
  zQHVert **bp;

  if( *n >= *size ){
    if( !( bp = zRealloc( *buf, zQHVert*, *size*2 ) ) ){
      ZALLOCERROR();
      return false;
    }
    *buf = bp;
    *size *= 2;
  }
  (*buf)[(*n)++] = v;
  return true;
}

/* buffers to collect vertices in the vicinity of a vertex */
typedef struct{
  zQHVert **q;      /* vertices in the vicinity, the first of which is the center */
  int nq;           /* number of the vertices */
  int qsize;        /* size of the buffer */
  zQHVert **st;     /* stack to pass through marked vertices */
  int nst;          /* number of vertices in the stack */
  int stsize;       /* size of the stack */
  zQHVert *sub;     /* points of the sub problem */
  int subsize;      /* size of the buffer */
} zQHVicinity;

/* initialize buffers of the vicinity. */
static bool _zQHVicinityInit(zQHVicinity *vic)
{
  vic->qsize = vic->stsize = vic->subsize = 64;
  vic->q = zAlloc( zQHVert*, vic->qsize );
  vic->st = zAlloc( zQHVert*, vic->stsize );
  vic->sub = zAlloc( zQHVert, vic->subsize );
  if( !vic->q || !vic->st || !vic->sub ){
    ZALLOCERROR();
    return false;
  }
  return true;
}

/* destroy buffers of the vicinity. */
static void _zQHVicinityDestroy(zQHVicinity *vic)
{
  zFree( vic->q );
  zFree( vic->st );
  zFree( vic->sub );
}

/* approximate normal vector at a vertex, the star of which is stored in the buffers of qh with m
 * facets. */
static void _zQHVertNorm(zQH *qh, int m, zVec3D *n)
{
  int j;

  zVec3DZero( n );
  for( j=0; j<m; j++ ) _zVec3DAddDRC( n, &qh->sf[j]->n );
  _zQHNormalize( n );
}

/* collect unmarked vertices within two edges from a vertex v, passing through marked vertices.
 * The mark is represented by keep. */
static bool _zQHVicinityCollect(zQH *qh, zQHVicinity *vic, zQHVert *v)
{
  zQHFacet *f;
  zQHVert *w, *x;
  int j, hop, from, to;

  qh->stamp++;
  v->mark = qh->stamp;
  vic->nq = 0;
  if( !_zQHPush( &vic->q, &vic->nq, &vic->qsize, v ) ) return false;
  for( from=0, hop=0; hop<2; hop++, from=to ){
    for( to=vic->nq, j=from; j<to; j++ ){
      vic->nst = 0;
      if( !_zQHPush( &vic->st, &vic->nst, &vic->stsize, vic->q[j] ) ) return false;
      while( vic->nst > 0 ){
        x = vic->st[--vic->nst];
        f = x->facet;
        do{
          w = f->v[(_zQHFacetVertIndex( f, x )+1)%3];
          if( w->mark != qh->stamp ){
            w->mark = qh->stamp;
            if( !( w->keep ? _zQHPush( &vic->st, &vic->nst, &vic->stsize, w ) :
                             _zQHPush( &vic->q, &vic->nq, &vic->qsize, w ) ) ) return false;
          }
          f = f->c[(_zQHFacetVertIndex( f, x )+1)%3];
        } while( f != x->facet );
      }
    }
  }
  return true;
}

/* check if a vertex v is within the tolerance from the exact convex hull of the unmarked vertices
 * in the vicinity, where n is the approximate normal vector at v. It returns 1 if so, 0 if not,
 * and -1 if failed. */
static int _zQHVertIsRemovable(zQHVicinity *vic, zQHVert *v, zVec3D *n, double tol)
{
  zQH sub;
  zQHVert *sp;
  zVec3D d;
  double h, hmax;
  int j, num, dim, ret;

  if( ( num = vic->nq - 1 ) < 3 ) return 0; /* q[0] is v */
  /* v is farther than the tolerance from the convex hull if all the vertices are lower than it by
     more than the tolerance along a direction. */
  for( hmax=-HUGE_VAL, j=1; j<=num; j++ ){
    _zVec3DSub( vic->q[j]->v, v->v, &d );
    if( ( h = _zVec3DInnerProd( n, &d ) ) > hmax ) hmax = h;
  }
  if( !zVec3DIsTiny( n ) && hmax < -tol ) return 0;
  if( num > vic->subsize ){
    if( !( sp = zRealloc( vic->sub, zQHVert, num*2 ) ) ){
      ZALLOCERROR();
      return -1;
    }
    vic->sub = sp;
    vic->subsize = num * 2;
  }
  sub.vert = vic->sub;
  for( sub.num=num, j=0; j<num; j++ ){
    sub.vert[j].v = vic->q[j+1]->v;
    sub.vert[j].next = NULL;
    sub.vert[j].cone = NULL;
  }
  ret = ( dim = _zQHBuild( &sub ) ) >= 3 && _zQHIsWithinTol( &sub, v, tol ) ? 1 : 0;
  _zQHDestroy( &sub );
  return dim < 0 ? -1 : ret;
}

/* mark vertices to be removed with a positive tolerance. A vertex is marked if it is within the
 * tolerance from the exact convex hull of the unmarked vertices within two edges from it, where
 * marked vertices are passed through, which is a subset of the convex hull of all the unmarked
 * vertices. The mark is represented by keep. */
static bool _zQHMarkRemovable(zQH *qh, double tol)
{
  zQHVicinity vic;
  zQHVert *v;
  zVec3D n;
  int i, m, ret;
  bool result = false;

  if( !_zQHVicinityInit( &vic ) ) goto TERMINATE;
  for( i=0; i<qh->num; i++ ){
    if( !( v = &qh->vert[i] )->facet ) continue;
    if( ( m = _zQHStar( qh, v ) ) < 0 ) goto TERMINATE;
    _zQHVertNorm( qh, m, &n );
    if( !_zQHVicinityCollect( qh, &vic, v ) ) goto TERMINATE;
    if( ( ret = _zQHVertIsRemovable( &vic, v, &n, tol ) ) < 0 ) goto TERMINATE;
    if( ret > 0 ) v->keep = true; /* to be removed */
  }
  result = true;
 TERMINATE:
  _zQHVicinityDestroy( &vic );
  return result;
}

/* distance from a point p to a segment ( a, b ). */
static double _zQHDistSegment(zVec3D *a, zVec3D *b, zVec3D *p)
{
  zVec3D e, d;
  double l, t;

  _zVec3DSub( b, a, &e );
  _zVec3DSub( p, a, &d );
  if( ( l = _zVec3DSqrNorm( &e ) ) > 0 && ( t = _zVec3DInnerProd( &e, &d ) / l ) > 0 ){
    if( t > 1 ) t = 1;
    _zVec3DCatDRC( &d, -t, &e );
  }
  return _zVec3DNorm( &d );
}

/* check if a vertex v and marked vertices between unmarked vertices a and b on the boundary of a
 * planar convex hull are within the tolerance from the segment ( a, b ), where vn is the array of
 * the next vertices. */
static bool _zQHSegmentIsWithinTol(zQH *qh, zQHVert **vn, zQHVert *a, zQHVert *b, zQHVert *v, double tol)
{
  zQHVert *w;

  if( _zQHDistSegment( a->v, b->v, v->v ) > tol ) return false;
  for( w=vn[a-qh->vert]; w!=b; w=vn[w-qh->vert] )
    if( w != v && _zQHDistSegment( a->v, b->v, w->v ) > tol ) return false;
  return true;
}

/* mark vertices of a planar convex hull to be removed with a positive tolerance. A vertex is marked
 * if it and the marked vertices around it are within the tolerance from the segment between the
 * previous and the next unmarked vertices on the boundary, which is a subset of the convex hull of
 * all the unmarked vertices. The mark is represented by keep. */
static bool _zQHMarkRemovablePlanar(zQH *qh, double tol)
{
  zQHFacet *f;
  zQHVert **vn, **vp, *v = NULL, *a, *b;
  int i, k, n = 0;
  bool result = false;

  vn = zAlloc( zQHVert*, qh->num );
  vp = zAlloc( zQHVert*, qh->num );
  if( !vn || !vp ){
    ZALLOCERROR();
    goto TERMINATE;
  }
  /* the boundary of the planar convex hull consists of the edges of vertical facets. */
  for( f=qh->list.next; f!=&qh->list; f=f->next ){
    for( i=0; i<3; i++ )
      if( _zQHVertIsVirtual( f->v[i] ) ) break;
    if( i == 3 ) continue;
    a = f->v[(i+1)%3];
    b = f->v[(i+2)%3];
    vn[a-qh->vert] = b;
    vp[b-qh->vert] = a;
    v = a;
    n++;
  }
  for( k=0; k<n; k++, v=vn[v-qh->vert] ){
    for( a=vp[v-qh->vert]; a->keep; a=vp[a-qh->vert] );
    for( b=vn[v-qh->vert]; b->keep; b=vn[b-qh->vert] );
    if( a == b ) continue; /* less than three vertices are left */
    if( _zQHSegmentIsWithinTol( qh, vn, a, b, v, tol ) ) v->keep = true; /* to be removed */
  }
  result = true;
 TERMINATE:
  zFree( vn );
  zFree( vp );
  return result;
}

/* build the exact convex hull qh2 of the vertices of qh, which have non-null facets. It returns the
 * dimension of the convex hull, or -1 if it fails. */
static int _zQHRebuildSub(zQH *qh, zQH *qh2)
{
  int i, n;

  for( n=0, i=0; i<qh->num; i++ )
    if( qh->vert[i].facet ) n++;
  zFree( qh2->vert );
  if( !( qh2->vert = zAlloc( zQHVert, n ) ) ){
    ZALLOCERROR();
    return -1;
  }
  for( qh2->num=0, i=0; i<qh->num; i++ )
    if( qh->vert[i].facet ){
      qh2->vert[qh2->num].v = qh->vert[i].v;
      qh2->vert[qh2->num].next = NULL;
      qh2->vert[qh2->num++].cone = NULL;
    }
  return _zQHBuild( qh2 );
}

/* restore removed vertices of qh that are farther than the tolerance from qh2, or all removed
 * vertices if qh2 is the null pointer. It returns the true value if any vertex is restored. */
static bool _zQHRestore(zQH *qh, zQH *qh2, double tol)
{
  int i;
  bool restored = false;

  for( i=0; i<qh->num; i++ )
    if( qh->vert[i].keep && !qh->vert[i].facet && ( !qh2 || !_zQHIsWithinTol( qh2, &qh->vert[i], tol ) ) ){
      qh->vert[i].facet = qh->list.next; /* dummy to mark as a vertex */
      restored = true;
    }
  return restored;
}

/* rebuild the exact convex hull of vertices left after the removal with a positive tolerance, and
 * restore removed vertices that are farther than the tolerance from it. It returns the dimension
 * of the rebuilt convex hull, or -1 if it fails. */
static int _zQHRebuild(zQH *qh, zQH *qh2, double tol)
{
  int i, dim;

  /* mark removable vertices of the exact convex hull (keep is used as the mark) */
  for( i=0; i<qh->num; i++ ) qh->vert[i].keep = false;
  if( !( qh->planar ? _zQHMarkRemovablePlanar( qh, tol ) : _zQHMarkRemovable( qh, tol ) ) ) return -1;
  /* after this, keep is true for vertices of the exact convex hull, and facet is the null pointer
     for removed vertices */
  for( i=0; i<qh->num; i++ )
    if( qh->vert[i].keep )
      qh->vert[i].facet = NULL;
    else if( qh->vert[i].facet )
      qh->vert[i].keep = true;
  qh2->vert = NULL;
  while( 1 ){
    if( ( dim = _zQHRebuildSub( qh, qh2 ) ) < 0 || ( dim == 4 && !_zQHSimplify( qh2 ) ) ) break;
    if( dim < 3 ){ /* too many vertices were removed; restore all */
      _zQHDestroy( qh2 );
      if( !_zQHRestore( qh, NULL, tol ) ) break;
      continue;
    }
    if( !_zQHRestore( qh, qh2, tol ) ) return dim;
    _zQHDestroy( qh2 );
  }
  if( qh2->vert ){
    _zQHDestroy( qh2 );
    zFree( qh2->vert );
  }
  return -1;
}

/* set a facet incident to each vertex, and the null pointer to the other points. */
static void _zQHSetVertFacet(zQH *qh)
{
  zQHFacet *f;
  int i;

  for( i=0; i<qh->num; i++ ) qh->vert[i].facet = NULL;
  for( f=qh->list.next; f!=&qh->list; f=f->next )
    for( i=0; i<3; i++ )
      if( !_zQHVertIsVirtual( f->v[i] ) ) f->v[i]->facet = f;
}

/* convert the convex hull to a polyhedron. */
static zPH3D *_zQH2PH3D(zQH *qh, zPH3D *ph)
{
  zQHFacet *f;
  zTri3D *t;
  int vn = 0, fn = 0, i;

  for( i=0; i<qh->num; i++ ) qh->vert[i].id = -1;
  for( f=qh->list.next; f!=&qh->list; f=f->next ){
    if( qh->planar && _zQHFacetIsVertical( f ) )
      continue;
    for( i=0; i<3; i++ )
      if( f->v[i]->id < 0 ) f->v[i]->id = vn++;
    fn++;
  }
  if( qh->planar ) fn *= 2; /* both sides of a planar convex hull */
  if( !zPH3DAlloc( ph, vn, fn ) ) return NULL;
  for( i=0; i<qh->num; i++ )
    if( qh->vert[i].id >= 0 )
      zVec3DCopy( qh->vert[i].v, zPH3DVert(ph,qh->vert[i].id) );
  t = zPH3DFaceBuf(ph);
  for( f=qh->list.next; f!=&qh->list; f=f->next ){
    if( qh->planar && _zQHFacetIsVertical( f ) )
      continue;
    /* the normal vector of a degenerate triangle is the zero vector (see zTri3DCalcNorm()). */
    zTri3DCreate( t++, zPH3DVert(ph,f->v[0]->id), zPH3DVert(ph,f->v[1]->id), zPH3DVert(ph,f->v[2]->id) );
    if( qh->planar ) /* the other side of a planar convex hull */
      zTri3DCreateFlip( t++, zPH3DVert(ph,f->v[0]->id), zPH3DVert(ph,f->v[1]->id), zPH3DVert(ph,f->v[2]->id) );
  }
  return ph;
}

/* convex hull of 3D points, simplified within a tolerance if it is positive. */
static zPH3D *_zVec3DDataConvexHull(zVec3DData *data, zPH3D *ch, double tol)
{
  zQH qh, qh2;
  zVec3D *v;
  int dim, dim2;
  zPH3D *ret = NULL;

  zPH3DInit( ch );
  qh.num = zVec3DDataSize( data );
  if( qh.num == 0 ){
    ZRUNERROR( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  if( !( qh.vert = zAlloc( zQHVert, qh.num ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zVec3DDataRewind( data );
  for( qh.num=0; ( v = zVec3DDataFetch( data ) ); qh.num++ ){
    if( zVec3DIsNan( v ) ) goto TERMINATE;
    qh.vert[qh.num].v = v;
    qh.vert[qh.num].next = NULL;
    qh.vert[qh.num].cone = NULL;
  }
  if( ( dim = _zQHBuild( &qh ) ) == 4 && !_zQHSimplify( &qh ) ) dim = -1;
  if( dim >= 3 && tol > 0 ){
    if( dim == 3 ) _zQHSetVertFacet( &qh );
    if( ( dim2 = _zQHRebuild( &qh, &qh2, tol ) ) >= 3 ){
      ret = _zQH2PH3D( &qh2, ch );
      _zQHDestroy( &qh2 );
      zFree( qh2.vert );
      goto DESTROY;
    }
    dim = -1;
  }
  switch( dim ){
  case 1: ZRUNERROR( ZEO_ERR_CH_DEG1 ); break;
  case 2: ZRUNERROR( ZEO_ERR_CH_DEG2 ); break;
  case 3:
  case 4: ret = _zQH2PH3D( &qh, ch ); break;
  default: ;
  }
 DESTROY:
  _zQHDestroy( &qh );
 TERMINATE:
  zFree( qh.vert );
  return ret;
}

/* exact convex hull of 3D points. */
zPH3D *zVec3DDataConvexHull(zVec3DData *data, zPH3D *ch)
{
  return _zVec3DDataConvexHull( data, ch, 0 );
}

/* convex hull of 3D points simplified within a tolerance. */
zPH3D *zVec3DDataConvexHullSimplify(zVec3DData *data, zPH3D *ch, double tol)
{
  return _zVec3DDataConvexHull( data, ch, tol );
}

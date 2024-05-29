/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bv3d_ch - 3D bounding volume: 3D convex hull.
 */

#include <zeo/zeo_bv2d.h>
#include <zeo/zeo_bv3d.h>

/* ********************************************************** */
/* CLASS: zQHPoint, zQHPointList, zQHFacet, zQHFacetList, zQH
 * facet/point class for quickhull algorithm
 * ********************************************************** */

typedef struct _zQHPoint{
  zVec3D *v;    /* vertex coordinates */
  bool discard; /* discarding flag */
} zQHPoint;
zListClass(zQHPointList, zQHPointListCell, zQHPoint);

typedef struct _zQHFacet{
  zQHPoint *p[3];  /* vertices */
  struct _zQHFacet *c[3]; /* contiguous facets */
  bool merge[3];  /* flag to check if merged with contiguous facets */
  zQHPointList op; /* outside point set */
  zVec3D n;        /* normal vector */
  double d_max;    /* furthest point distance */
  bool visible;    /* visible set flag */
  bool check;      /* merge check flag */
} zQHFacet;
zListClass(zQHFacetList, zQHFacetListCell, zQHFacet);

typedef struct{
  zQHPointList vl; /* vertices */
  zQHFacetList fl; /* facets */
} zQH;

/* facet/point operation */

/* create a point. */
static zQHPoint *_zQHPointCreate(zQHPoint *p, zVec3D *v)
{
  p->v = v;
  p->discard = false;
  return p;
}

/* create a new facet. */
static zQHFacet *_zQHFacetCreate(zQHFacet *f, zQHPoint *p0, zQHPoint *p1, zQHPoint *p2)
{
  zVec3D e1, e2;

  zVec3DSub( p1->v, p0->v, &e1 );
  zVec3DSub( p2->v, p0->v, &e2 );
  zVec3DOuterProd( &e1, &e2, &f->n );
  zVec3DNormalizeNCDRC( &f->n );
  f->p[0] = p0;
  f->p[1] = p1;
  f->p[2] = p2;
  f->c[0] = f->c[1] = f->c[2] = NULL;
  f->merge[0] = f->merge[1] = f->merge[2] = false;
  zListInit( &f->op );
  f->d_max = -zTOL;
  f->visible = false;
  f->check = false;
  return f;
}

/* destroy a facet. */
static void _zQHFacetDestroy(zQHFacet *f)
{
  f->p[0] = f->p[1] = f->p[2] = NULL;
  f->c[0] = f->c[1] = f->c[2] = NULL;
  zListDestroy( zQHPointListCell, &f->op );
}

/* bind with contiguous facets. */
static void _zQHFacetContig(zQHFacet *f, zQHFacet *f0, zQHFacet *f1, zQHFacet *f2)
{
  f->c[0] = f0;
  f->c[1] = f1;
  f->c[2] = f2;
}

/* convert a facet to a triangle element. */
static zTri3D *_zQHFacet2Tri3D(zQHFacet *f, zTri3D *t)
{
  zTri3DSetVert( t, 0, f->p[0]->v );
  zTri3DSetVert( t, 1, f->p[1]->v );
  zTri3DSetVert( t, 2, f->p[2]->v );
  zTri3DSetNorm( t, &f->n );
  return t;
}

/* check if a point is on a facet. */
static bool _zQHFacetOn(zQHFacet *f, zVec3D *v)
{
  zTri3D t;

  _zQHFacet2Tri3D( f, &t );
  return zTri3DPointIsInside( &t, v, zTOL );
}

/* destroy facet list. */
static void _zQHFacetListDestroy(zQHFacetList *fl)
{
  zQHFacetListCell *fc;

  while( !zListIsEmpty( fl ) ){
    zListDeleteHead( fl, &fc );
    _zQHFacetDestroy( &fc->data );
    zFree( fc );
  }
}

/* initial simplex */

/* find vertices of the initial simplex. */
static int _zQHSimplexVert(zQHPointList *pl, zQHPointListCell *v[])
{
  zQHPointList reserve;
  zVec3D e[3], d[2], n;
  double h;
  int dim = 4;

  zListInit( &reserve );
  if( zListIsEmpty( pl ) ){
    ZRUNWARN( ZEO_ERR_EMPTYSET );
    return 0;
  }
  zListDeleteTail( pl, &v[0] ); /* first vertex */
  while( !zListIsEmpty( pl ) ){ /* second vertex */
    zListDeleteTail( pl, &v[1] ); /* candidate */
    zVec3DSub( v[1]->data.v, v[0]->data.v, &e[0] );
    if( !zVec3DIsTiny( &e[0] ) ) goto THIRD;
    zFree( v[1] );
  }
  ZRUNWARN( ZEO_ERR_CH_DEG1 );
  return 1;
 THIRD:
  zVec3DNormalize( &e[0], &d[0] );
  while( !zListIsEmpty( pl ) ){ /* third vertex */
    zListDeleteTail( pl, &v[2] ); /* candidate */
    zVec3DSub( v[2]->data.v, v[0]->data.v, &e[1] );
    if( !zVec3DIsTiny( &e[1] ) ){
      zVec3DNormalize( &e[1], &d[1] );
      zVec3DOuterProd( &d[0], &d[1], &n );
      if( !zVec3DIsTiny( &n ) ){
        zVec3DNormalizeDRC( &n );
        goto FOURTH;
      }
    }
    zListInsertTail( &reserve, v[2] );
  }
  ZRUNERROR( ZEO_ERR_CH_DEG2 );
  dim = 2;
  goto TERMINATE;
 FOURTH:
  while( !zListIsEmpty( pl ) ){ /* fourth vertex */
    zListDeleteTail( pl, &v[3] ); /* candidate */
    zVec3DSub( v[3]->data.v, v[0]->data.v, &e[2] );
    h = zVec3DInnerProd( &e[2], &n );
    if( !zIsTiny( h ) ){
      if( h > 0 )
        zSwap( zQHPointListCell*, v[0], v[1] );
      goto TERMINATE;
    }
    zListInsertTail( &reserve, v[3] );
  }
  ZRUNERROR( ZEO_ERR_CH_DEG3 );
  dim = 3;
 TERMINATE:
  if( !zListIsEmpty( &reserve ) )
    zListAppend( pl, &reserve );
  return dim;
}

/* initial simplex. */
static int _zQHSimplex(zQH *qh, zQHPointList *pl)
{
  zQHPointListCell *v[4];
  zQHFacetListCell *fc[4];
  int ret, i;

  if( ( ret = _zQHSimplexVert( pl, v ) ) < 4 ) return ret;
  for( i=0; i<4; i++ ){
    zListInsertHead( &qh->vl, v[i] );
    if( !( fc[i] = zAlloc( zQHFacetListCell, 1 ) ) ){
      ZALLOCERROR();
      return 0;
    }
    zListInsertHead( &qh->fl, fc[i] );
  }
  _zQHFacetCreate( &fc[0]->data, &v[0]->data, &v[1]->data, &v[2]->data );
  _zQHFacetCreate( &fc[1]->data, &v[0]->data, &v[3]->data, &v[1]->data );
  _zQHFacetCreate( &fc[2]->data, &v[1]->data, &v[3]->data, &v[2]->data );
  _zQHFacetCreate( &fc[3]->data, &v[2]->data, &v[3]->data, &v[0]->data );
  _zQHFacetContig( &fc[0]->data, &fc[2]->data, &fc[3]->data, &fc[1]->data );
  _zQHFacetContig( &fc[1]->data, &fc[2]->data, &fc[0]->data, &fc[3]->data );
  _zQHFacetContig( &fc[2]->data, &fc[3]->data, &fc[0]->data, &fc[1]->data );
  _zQHFacetContig( &fc[3]->data, &fc[1]->data, &fc[0]->data, &fc[2]->data );
  return ret;
}

/* beneath-beyond test. */
static int _zQHFacetBB(zQHFacet *f, zQHPoint *p, zVec3D *e)
{
  double l;

  zVec3DSub( p->v, f->p[0]->v, e );
  if( zVec3DIsTiny(e) ) return 0; /* coincide with one vertex */
  l = zVec3DInnerProd( &f->n, e );
  if( l < -zTOL ) return -1; /* beneath case */
  if( l > zTOL ) return 1;   /* beyond case */
  return _zQHFacetOn( f, p->v ) ? 0 : 1;
}

/* assign points to the outside sets of facets. */
static void _zQHFacetAssign(zQHFacetList *fl, zQHPointList *pl)
{
  zQHFacetListCell *fc;
  zQHPointListCell *pc, *pc_tmp;
  zVec3D e;
  double d;
  int flag;

  zListForEach( fl, fc )
    zListForEach( pl, pc ){
      flag = _zQHFacetBB( &fc->data, &pc->data, &e );
      if( flag == -1 ) continue; /* invisible from the current facet */
      pc_tmp = zListCellPrev( pc );
      zListPurge( pl, pc );
      if( flag == 0 ){
        /* discard a superficial point on a facet */
        zFree( pc );
      } else{ /* visible point */
        d = zVec3DInnerProd( &fc->data.n, &e );
        if( d > fc->data.d_max ){
          fc->data.d_max = d;
          zListInsertHead( &fc->data.op, pc );
        } else
          zListInsertTail( &fc->data.op, pc );
      }
      pc = pc_tmp;
    }
  /* discard points inside of the convex hull */
  zListDestroy( zQHPointListCell, pl );
}

/* merge coplanar facets. */
static void _zQHMerge(zQHFacet *f)
{
  f->visible = true;
  f->check = true;
  if( f->merge[0] && !f->c[0]->check ) _zQHMerge( f->c[0] );
  if( f->merge[1] && !f->c[1]->check ) _zQHMerge( f->c[1] );
  if( f->merge[2] && !f->c[2]->check ) _zQHMerge( f->c[2] );
}

/* reset visibility flags of all facets. */
static void _zQHFacetResetFlag(zQH *qh)
{
  zQHFacetListCell *fc;

  zListForEach( &qh->fl, fc ){
    fc->data.visible = false;
    fc->data.check = false;
  }
}

/* purge visible set from facet list and collect outside points. */
static bool _zQHVisibleSet(zQH *qh, zQHPoint *p, zQHFacetList *vs, zQHPointList *pl)
{
  zQHFacetListCell *fc, *fc_tmp;
  zVec3D e;
  int flag;

  zListInit( vs );
  zListInit( pl );
  zListForEach( &qh->fl, fc ){
    flag = _zQHFacetBB( &fc->data, p, &e );
    if( flag == 0 ){
      _zQHFacetResetFlag( qh );
      return false;
    }
    if( flag == 1 && !fc->data.check ) _zQHMerge( &fc->data );
  }
  zListForEach( &qh->fl, fc )
    if( fc->data.visible ){ /* if visible, the facet will be deleted. */
      fc_tmp = zListCellPrev( fc );
      zListPurge( &qh->fl, fc ); /* delete from facet list */
      zListInsertTail( vs, fc ); /* insert to visible set */
      zListAppendZ( pl, &fc->data.op ); /* points to be reassigned */
      fc->data.p[0]->discard = fc->data.p[1]->discard = fc->data.p[2]->discard = true;
      fc = fc_tmp;
    }
  return true;
}

/* find counter identifier of the contiguous facet. */
static int _zQHFacetContigID(zQHFacet *f, int s)
{
  if( f->c[s]->c[0] == f ) return 0;
  if( f->c[s]->c[1] == f ) return 1;
  if( f->c[s]->c[2] == f ) return 2;
  ZRUNERROR( ZEO_ERR_FATAL );
  return -1;
}

/* bind two contiguous facets with each other. */
static void _zQHFacetBind(zQHFacet *f1, int s1, zQHFacet *f2, int s2)
{
  zVec3D e, d;
  bool flag = false;

  f1->c[s1] = f2;
  f2->c[s2] = f1;
  /* merge if coplanar */
  if( zVec3DIsTiny( zVec3DSub( &f1->n, &f2->n, &e ) ) )
    flag = true; /* normal vectors almost coincide */
  else{
    if( zVec3DIsTiny( zVec3DOuterProd( &f1->n, &f2->n, &d ) ) )
      flag = true; /* normal vectors almost parallel */
    else{
      zVec3DSub( f1->p[(s1+2)%3]->v, f1->p[(s1+1)%3]->v, &e );
      if( zVec3DInnerProd( &e, &d ) < 0 )
        flag = true; /* non-convex contiguous facets (abnormal) */
    }
  }
  f1->merge[s1] = f2->merge[s2] = flag;
}

/* create a new facet from a new vertex and a horizon ridge. */
static zQHFacetListCell *_zQHHorizonNewFacet(zQHPoint *p, zQHFacet *f, int s)
{
  zQHFacetListCell *fc;
  int s1, s2;

  if( !( fc = zAlloc( zQHFacetListCell, 1 ) ) ) return NULL;
  s1 = ( s + 1 ) % 3;
  s2 = ( s + 2 ) % 3;
  f->p[s1]->discard = f->p[s2]->discard = false;
  if( _zQHFacetCreate( &fc->data, p, f->p[s1], f->p[s2] ) &&
      ( s1 = _zQHFacetContigID( f, s ) ) != -1 ){
    _zQHFacetBind( &fc->data, 0, f->c[s], s1 );
    return fc;
  }
  ZRUNERROR( ZEO_ERR_FATAL );
  zFree( fc );
  return NULL;
}

/* find the horizon ridges and create new facets. */
static bool _zQHHorizon(zQHFacetList *vs, zQHPoint *p, zQHFacetList *cone)
{
  zQHFacetListCell *fc;
  zQHFacet *f, *f_prev;
  zQHPoint *p0;
  int i = 0, s, n;

  zListInit( cone );
  /* find first ridge */
  zListForEach( vs, fc )
    for( s=0; s<3; s++ )
      if( !fc->data.c[s]->visible ) goto START;
  ZRUNERROR( ZEO_ERR_FATAL );
  return false;

 START:
  /* first facet in the cone */
  f = &fc->data;
  if( !( fc = _zQHHorizonNewFacet( p, f, s ) ) )
    return false;
  zListInsertHead( cone, fc );
  s = ( s + 1 ) % 3;
  p0 = f->p[s];
  n = zListSize( vs ) * 3;
  do{
    if( i++ > n ){ /* probably a circulation occurs */
      ZRUNERROR( ZEO_ERR_FATAL );
      return false;
    }
    if( f->c[s]->visible ){
      f_prev = f;
      f = f->c[s];
      if( ( s = _zQHFacetContigID( f_prev, s ) ) == -1 ){
        ZRUNERROR( ZEO_ERR_FATAL );
        return false;
      }
      s = ( s + 1 ) % 3;
    } else{
      if( !( fc = _zQHHorizonNewFacet( p, f, s ) ) )
        return false;
      _zQHFacetBind( &fc->data, 2, &zListHead(cone)->data, 1 );
      zListInsertHead( cone, fc );
      s = ( s + 1 ) % 3;
    }
  } while( f->p[(s+1)%3] != p0 );
  /* bind head and tail of the cone */
  _zQHFacetBind( &zListHead(cone)->data, 1, &zListTail(cone)->data, 2 );
  return true;
}

/* incrementally create new vertices and facets. */
static bool _zQHInc(zQH *qh, zQHFacet *f)
{
  zQHPointListCell *pc; /* furthest point */
  zQHPointList pl; /* list of points to be reassigned */
  zQHFacetList vs; /* visible facet set */
  zQHFacetList cone; /* cone consisting of new facets */

  /* pick up a new vertex */
  zListDeleteHead( &f->op, &pc );
  /* create visible set and collect points to be reassigned */
  if( !_zQHVisibleSet( qh, &pc->data, &vs, &pl ) ){
    zFree( pc );
    return true; /* retry another facet */
  }
  zListInsertHead( &qh->vl, pc );
  /* create a cone of the new vertex and horizon ridges */
  if( !_zQHHorizon( &vs, &pc->data, &cone ) ){
    _zQHFacetListDestroy( &cone );
    _zQHFacetListDestroy( &vs );
    return false;
  }
  _zQHFacetAssign( &cone, &pl ); /* reassign points outside of the cone */
  zListAppendA( &qh->fl, &cone ); /* merge the cone to the convex hull */
  _zQHFacetListDestroy( &vs ); /* destroy visible set */
  return true;
}

/* discard old vertices inside of the convex hull. */
static void _zQHDiscard(zQH *qh, zQHPointList *pl)
{
  zQHPointListCell *pc;

  zListAppend( pl, &qh->vl );
  while( !zListIsEmpty(pl) ){
    zListDeleteTail( pl, &pc );
    if( pc->data.discard )
      zFree( pc );
    else
      zListInsertHead( &qh->vl, pc );
  }
}

/* create the convex hull of a point set. */
static int _zQHCreate(zQH *qh, zQHPointList *pl)
{
  zQHFacetListCell *fc;
  int dim;

  /* initialize vertex & facet list */
  zListInit( &qh->vl );
  zListInit( &qh->fl );
  /* create simplex */
  if( ( dim = _zQHSimplex( qh, pl ) ) < 4 ) return dim;
  _zQHFacetAssign( &qh->fl, pl ); /* initial beneath-beyond test */

  /* incrementally generate new facets */
  zListForEach( &qh->fl, fc )
    if( !zListIsEmpty( &fc->data.op ) ){
      if( !_zQHInc( qh, &fc->data ) ) break;
      fc = zListRoot( &qh->fl ); /* retry from the tail */
    }

  _zQHDiscard( qh, pl ); /* discard old vertices */
  return dim;
}

/* destroy the convex hull. */
static void _zQHDestroy(zQH *qh)
{
  zListDestroy( zQHPointListCell, &qh->vl );
  _zQHFacetListDestroy( &qh->fl );
}

/* convert the convex hull to a polyhedron. */
static zPH3D *_zQH2PH3D(zQH *qh, zPH3D *ph)
{
  zVec3D *vp;
  zTri3D *fp;
  zQHPointListCell *pc;
  zQHFacetListCell *fc;

  if( !zPH3DAlloc( ph, zListSize(&qh->vl), zListSize(&qh->fl) ) )
    return NULL;
  vp = zPH3DVertBuf(ph);
  zListForEach( &qh->vl, pc ){
    zVec3DCopy( pc->data.v, vp );
    pc->data.v = vp++;
  }
  fp = zPH3DFaceBuf(ph);
  zListForEach( &qh->fl, fc )
    _zQHFacet2Tri3D( &fc->data, fp++ );
  return ph;
}

/* convex hull from list of 3D points. */
zPH3D *zConvexHull3DPL(zPH3D *ch, zVec3DList *vl)
{
  zVec3DListCell *vc;
  zQHPointListCell *pc;
  zQHPointList pl;
  zQH qh;
  int ret;

  /* create point list */
  zListInit( &pl );
  zListForEach( vl, vc ){
    if( !( pc = zAlloc( zQHPointListCell, 1 ) ) ) break;
    _zQHPointCreate( &pc->data, vc->data );
    zListInsertHead( &pl, pc );
  }
  /* quickhull */
  if( ( ret = _zQHCreate( &qh, &pl ) ) < 4 ){
    zListDestroy( zQHPointListCell, &pl );
    if( ret == 3 ) /* planar convex hull */
      return zConvexHull2DPL2PH3D( ch, vl );
    return NULL;
  }
  /* convert to a polyhedron */
  _zQH2PH3D( &qh, ch );
  _zQHDestroy( &qh );
  return ch;
}

/* convex hull of 3D points. */
zPH3D *zConvexHull3D(zPH3D *ch, zVec3D p[], int num)
{
  zQHPointListCell *pc;
  zQHPointList pl;
  zQH qh;
  int i, ret;

  zPH3DInit( ch );
  /* create point list */
  zListInit( &pl );
  for( i=0; i<num; i++ ){
    if( !( pc = zAlloc( zQHPointListCell, 1 ) ) ) break;
    _zQHPointCreate( &pc->data, &p[i] );
    zListInsertHead( &pl, pc );
  }
  /* quickhull */
  if( ( ret = _zQHCreate( &qh, &pl ) ) < 4 ){
    zListDestroy( zQHPointListCell, &pl );
    if( ret == 3 ) /* planar convex hull */
      return zConvexHull2D2PH3D( ch, p, num );
    return NULL;
  }
  /* convert to a polyhedron */
  _zQH2PH3D( &qh, ch );
  _zQHDestroy( &qh );
  return ch;
}

/* for debug */
#ifdef DEBUG
static void _zQHFacetInfo(zQHFacet *f)
{
  printf( ">> facet:%p\n", f );
  printf( "[vert#0] " ); zVec3DPrint( f->p[0]->v );
  printf( "[vert#1] " ); zVec3DPrint( f->p[1]->v );
  printf( "[vert#2] " ); zVec3DPrint( f->p[2]->v );
  printf( "[contig#0] %p %g %s\n", f->c[0], zVec3DInnerProd(&f->n,&f->c[0]->n)-1, f->merge[0] ? " :merged" : "" );
  printf( "[contig#1] %p %g %s\n", f->c[1], zVec3DInnerProd(&f->n,&f->c[1]->n)-1, f->merge[1] ? " :merged" : "" );
  printf( "[contig#2] %p %g %s\n", f->c[2], zVec3DInnerProd(&f->n,&f->c[2]->n)-1, f->merge[2] ? " :merged" : "" );
  printf( "normal vector: " ); zVec3DPrint( &f->n );
}

static void _zQHFacetListInfo(zQHFacetList *fl)
{
  zQHFacetListCell *fc;

  zListForEach( fl, fc )
    _zQHFacetInfo( &fc->data );
}

static void _zQHPointListPrint(zQHPointList *pl, char filename[])
{
  zQHPointListCell *pc;
  FILE *fp;

  fp = fopen( filename, "w" );
  zListForEach( pl, pc )
    zVec3DDataNLFPrint( fp, pc->data.v );
  fclose( fp );
}

static void _zQHFacetListPrint(zQHFacetList *fl, char filename[])
{
  zQHFacetListCell *fc;
  FILE *fp;

  fp = fopen( filename, "w" );
  zListForEach( fl, fc ){
    zVec3DDataNLFPrint( fp, fc->data.p[1]->v );
    zVec3DDataNLFPrint( fp, fc->data.p[2]->v );
    fprintf( fp, "\n" );
    zVec3DDataNLFPrint( fp, fc->data.p[0]->v );
    zVec3DDataNLFPrint( fp, fc->data.p[0]->v );
    fprintf( fp, "\n\n" );
  }
  fclose(fp);
}

static void _zQHPrint(zQH *qh)
{
  zQHFacetListCell *fc;
  zQHPointListCell *pc;
  FILE *fp;

  _zQHFacetListPrint( &qh->fl, "ch" );
  _zQHPointListPrint( &qh->vl, "chv" );
  fp = fopen( "op", "w" );
  zListForEach( &qh->fl, fc )
    zListForEach( &fc->data.op, pc )
      zVec3DDataNLFPrint( fp, pc->data.v );
  fclose( fp );
}

static void _zQHPrint2(zQH *qh)
{
  zQHFacetListCell *fc;
  zQHPointListCell *pc;
  FILE *fp;
  int i=0;
  char filename[BUFSIZ];

  _zQHFacetListPrint( &qh->fl, "ch" );
  _zQHPointListPrint( &qh->vl, "chv" );
  zListForEach( &qh->fl, fc ){
    sprintf( filename, "op%03d", i );
    fp = fopen( filename, "w" );
    zListForEach( &fc->data.op, pc )
      zVec3DDataNLFPrint( fp, pc->data.v );
    fclose( fp );
    i++;
  }
}
#endif /* DEBUG */

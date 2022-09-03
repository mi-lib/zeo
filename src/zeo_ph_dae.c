/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_dae - polyhedron I/O on DAE format
 */

#include <zeo/zeo_ph.h>

/* source geometry in DAE */
typedef enum{ ZEO_DAE_COORD_INVALID=-1, ZEO_DAE_XYZ, ZEO_DAE_TEXTURE } zDAECoordType;
typedef struct{
  char *id;
  int count;
  int vcount;
  int stride;
  char *content;
  int accessor[3];
  zDAECoordType type;
} zDAESrc;

static zDAESrc *_zDAESrcInit(zDAESrc *s)
{
  s->id = NULL;
  s->count = s->vcount = s->stride = 0;
  s->content = NULL;
  s->accessor[0] = s->accessor[1] = s->accessor[2] = 0;
  s->type = ZEO_DAE_COORD_INVALID;
  return s;
}

zListClass( zDAESrcList, zDAESrcCell, zDAESrc );

static zDAESrc *_zDAESrcFind(zDAESrcList *slist, char *str)
{
  zDAESrcCell *sc;

  if( str )
    zListForEach( slist, sc )
      if( strcmp( sc->data.id, str ) == 0 ) return &sc->data;
  return NULL;
}

/* primitive of mesh in DAE */
typedef struct{
  char *vcount;
  int pcount;
  int ni;
  int voffset;
  char *p;
} zDAEPrimitive;

zDAEPrimitive *_zDAEPrimitiveInit(zDAEPrimitive *p)
{
  p->vcount = p->p = NULL;
  p->pcount = 0;
  p->ni = 0;
  p->voffset = 0;
  return p;
}

zListClass( zDAEPrimitiveList, zDAEPrimitiveCell, zDAEPrimitive );

/* mesh geometry in DAE */
typedef struct{
  zDAESrcList slist;
  zDAESrc *vert;
  char *id;
  char *vid;
  zDAEPrimitiveList plist;
  zFrame3D f;
} zDAEMesh;

static zDAEMesh *_zDAEMeshInit(zDAEMesh *mesh)
{
  zListInit( &mesh->slist );
  mesh->vert = NULL;
  mesh->id = mesh->vid = NULL;
  zListInit( &mesh->plist );
  zFrame3DIdent( &mesh->f );
  return mesh;
}

static void _zDAEMeshDestroy(zDAEMesh *mesh)
{
  zListDestroy( zDAESrcCell, &mesh->slist );
  zListDestroy( zDAEPrimitiveCell, &mesh->plist );
}

zListClass( zDAEMeshList, zDAEMeshCell, zDAEMesh );

static void _zDAEMeshListDestroy(zDAEMeshList *mlist)
{
  zDAEMeshCell *mc;

  while( !zListIsEmpty( mlist ) ){
    zListDeleteHead( mlist, &mc );
    _zDAEMeshDestroy( &mc->data );
    free( mc );
  }
}

static zDAEMesh *_zDAEMeshListFind(zDAEMeshList *mlist, char *id)
{
  zDAEMeshCell *mc;

  if( id )
    zListForEach( mlist, mc )
      if( strcmp( mc->data.id, id ) == 0 ) return &mc->data;
  return NULL;
}

/* evaluate source geometries */

static bool _zDAESrcEvalFloatArray(xmlNode *node, zDAESrc *s)
{
  xmlAttr *attr;

  zXMLForEachAttr( node, attr ){
    zXMLCheckAttrAndExec( attr, "count", s->count = zXMLGetAttrInt( attr ) );
  }
  s->content = zXMLGetContent( node );
  return true;
}

static bool _zDAESrcSetAccessor(xmlNode *node, zDAESrc *s, int i)
{
  xmlAttr *attr;

  zXMLForEachAttr( node, attr ){
    if( zXMLCheckStr( attr->name, "name" ) ){
      switch( zXMLGetAttrVal( attr )[0] ){
      case 'x': case 'X': s->accessor[i] = zX; s->type = ZEO_DAE_XYZ;     break;
      case 'y': case 'Y': s->accessor[i] = zY; s->type = ZEO_DAE_XYZ;     break;
      case 'z': case 'Z': s->accessor[i] = zZ; s->type = ZEO_DAE_XYZ;     break;
      case 's': case 'S': s->accessor[i] = zX; s->type = ZEO_DAE_TEXTURE; break;
      case 't': case 'T': s->accessor[i] = zY; s->type = ZEO_DAE_TEXTURE; break;
      }
    }
  }
  return true;
}

static bool _zDAESrcEvalAccessor(xmlNode *node, zDAESrc *s)
{
  xmlNode *np;
  xmlAttr *attr;
  int i;

  if( !( node = zXMLFindNodeElement( node, "accessor" ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_ACCESSOR_UNASSIGNED, s->id );
    return false;
  }
  zXMLForEachAttr( node, attr ){
    zXMLCheckAttrAndExec( attr, "count",  s->vcount = zXMLGetAttrInt( attr ) ) else;
    zXMLCheckAttrAndExec( attr, "stride", s->stride = zXMLGetAttrInt( attr ) );
  }
  if( s->vcount * s->stride != s->count ){
    ZRUNERROR( ZEO_ERR_DAE_ARRSIZMIS, s->vcount * s->stride, s->count );
    return false;
  }
  i = 0;
  zXMLForEachNode( node->children, np ){
    zXMLCheckElementAndExec( np, "param", _zDAESrcSetAccessor( np, s, i++ ) );
    if( i > s->stride ){
      ZRUNERROR( ZEO_ERR_DAE_STRIDEMIS, i, s->stride );
      return false;
    }
  }
  return true;
}

static bool _zDAESrcEval(xmlNode *node, zDAESrcList *slist)
{
  xmlNode *np;
  zDAESrcCell *sc;

  if( !( sc = zAlloc( zDAESrcCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  _zDAESrcInit( &sc->data );
  zListInsertHead( slist, sc );
  sc->data.id = zXMLFindNodeAttr( node, "id" );
  zXMLForEachNode( node->children, np ){
    zXMLCheckElementAndExec( np, "float_array",      _zDAESrcEvalFloatArray( np, &sc->data ) ) else;
    zXMLCheckElementAndExec( np, "technique_common", _zDAESrcEvalAccessor( np->children, &sc->data ) );
  }
  return true;
}

static char *_zDAEEvalURI(xmlNode *node, char *name)
{
  char *id;

  if( !( id = zXMLFindNodeAttr( node, name ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_URI_EMPTY );
    return NULL;
  }
  if( id[0] != '#' ){ /* only local URI is accepted. */
    ZRUNERROR( ZEO_ERR_DAE_URI_INVALID, id );
    return NULL;
  }
  return &id[1];
}

static bool _zDAESrcEvalVert(xmlNode *node, zDAEMesh *mesh)
{
  xmlNode *np;
  xmlAttr *attr;
  char *source_id;

  mesh->vid = zXMLFindNodeAttr( node, "id" );
  zXMLForEachNode( node->children, np ){
    if( !zXMLCheckElement( np, "input" ) ) continue;
    zXMLForEachAttr( np, attr ){
      if( !zXMLCheckAttrVal( attr, "semantic", "POSITION" ) ) continue;
      if( !( source_id = _zDAEEvalURI( np, "source" ) ) ) break;
      if( !( mesh->vert = _zDAESrcFind( &mesh->slist, source_id ) ) ){
        ZRUNERROR( ZEO_ERR_DAE_UNKNOWN_SRC, source_id );
        return false;
      }
      if( mesh->vert->type != ZEO_DAE_XYZ ){
        ZRUNERROR( ZEO_ERR_DAE_INVALID_SRC );
        return false;
      }
      return true;
    }
  }
  ZRUNERROR( ZEO_ERR_DAE_VERT_UNASSIGNED );
  return false;
}

static bool _zDAEPrimitiveEvalFace(xmlNode *node, zDAEMesh *mesh)
{
  xmlNode *np;
  xmlAttr *attr;
  char *source_id;
  int offset;
  zDAEPrimitiveCell *pc;

  if( !( pc = zAlloc( zDAEPrimitiveCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  _zDAEPrimitiveInit( &pc->data );
  zListInsertHead( &mesh->plist, pc );

  pc->data.pcount = atoi( zXMLFindNodeAttr( node, "count" ) );
  zXMLForEachNode( node->children, np ){
    if( zXMLCheckElement( np, "input" ) ){
      if( ( offset = atoi( zXMLFindNodeAttr( np, "offset" ) ) ) > pc->data.ni )
        pc->data.ni = offset;
      zXMLForEachAttr( np, attr ){
        if( !zXMLCheckAttrVal( attr, "semantic", "VERTEX" ) ) continue;
        if( !( source_id = _zDAEEvalURI( np, "source" ) ) ) return false;
        pc->data.voffset = offset;
        if( strcmp( mesh->vid, source_id ) != 0 ){
          ZRUNERROR( ZEO_ERR_DAE_IDENTMIS, mesh->vid, source_id );
          return false;
        }
      }
    } else
    if( zXMLCheckElement( np, "vcount" ) )
      pc->data.vcount = zXMLGetContent( np );
    else
    if( zXMLCheckElement( np, "p" ) )
      pc->data.p = zXMLGetContent( np );
  }
  pc->data.ni++;
  return true;
}

static bool _zDAEMeshEval(xmlNode *node, char *id, zDAEMeshList *mlist)
{
  xmlNode *np;
  zDAEMeshCell *mc;

  if( !( mc = zAlloc( zDAEMeshCell, 1 ) ) ){
    ZALLOCERROR();
    return false;
  }
  _zDAEMeshInit( &mc->data );
  zListInsertHead( mlist, mc );
  mc->data.id = id;
  zXMLForEachNode( node, np ){ /* create source list */
    zXMLCheckElementAndExec( np, "source", _zDAESrcEval( np, &mc->data.slist ) );
  }
  zXMLForEachNode( node, np ){ /* evaluate vertices */
    zXMLCheckElementAndExec( np, "vertices", _zDAESrcEvalVert( np, &mc->data ) );
  }
  zXMLForEachNode( node, np ){ /* evaluate faces */
    zXMLCheckElementAndExec( np, "polylist",  _zDAEPrimitiveEvalFace( np, &mc->data ) ) else
    zXMLCheckElementAndExec( np, "triangles", _zDAEPrimitiveEvalFace( np, &mc->data ) );
    /* other primitives than polylist (lines, linestrips, polygons, trifans,
       tristrips) are ignored. */
  }
  return true;
}

static bool _zDAEMeshEvalGeometry(xmlNode *node, zDAEMeshList *mlist)
{
  xmlNode *np;
  char *id;

  id = zXMLFindNodeAttr( node, "id" );
  zXMLForEachNode( node->children, np ){
    zXMLCheckElementAndExec( np, "mesh", _zDAEMeshEval( np->children, id, mlist ) );
  }
  return true;
}

static bool _zDAEEvalLibraryGeometries(xmlNode *node, zDAEMeshList *mlist)
{
  xmlNode *np;

  zListInit( mlist );
  zXMLForEachNode( node, np ){
    zXMLCheckElementAndExec( np, "geometry", _zDAEMeshEvalGeometry( np, mlist ) );
    /* attributes (id, name) are ignored. */
  }
  return true;
}

static bool _zDAEGetXform(xmlNode *node, zFrame3D *f)
{
  char *str;
  double p, q, r, s;

  if( !( str = zXMLGetContent( node ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_EMPTYNODE );
    return false;
  }
  sscanf( str, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
    &zFrame3DAtt(f)->c.xx, &zFrame3DAtt(f)->c.xy, &zFrame3DAtt(f)->c.xz, &zFrame3DPos(f)->c.x,
    &zFrame3DAtt(f)->c.yx, &zFrame3DAtt(f)->c.yy, &zFrame3DAtt(f)->c.yz, &zFrame3DPos(f)->c.y,
    &zFrame3DAtt(f)->c.zx, &zFrame3DAtt(f)->c.zy, &zFrame3DAtt(f)->c.zz, &zFrame3DPos(f)->c.z,
    &p, &q, &r, &s );
  zVec3DDivDRC( zFrame3DPos(f), s );
  zMat3DDivDRC( zFrame3DAtt(f), s );
  return true;
}

static bool _zDAEGetTrans(xmlNode *node, zFrame3D *f)
{
  char *str;
  zVec3D p;

  if( !( str = zXMLGetContent( node ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_EMPTYNODE );
    return false;
  }
  sscanf( str, "%lf %lf %lf", &p.c.x, &p.c.y, &p.c.z );
  zVec3DAddDRC( zFrame3DPos(f), &p );
  return true;
}

static bool _zDAEGetRot(xmlNode *node, zFrame3D *f)
{
  char *str;
  double q;
  zVec3D aa;

  if( !( str = zXMLGetContent( node ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_EMPTYNODE );
    return false;
  }
  sscanf( str, "%lf %lf %lf %lf", &aa.c.x, &aa.c.y, &aa.c.z, &q );
  zVec3DMulDRC( &aa, zDeg2Rad(q) );
  zMat3DRotDRC( zFrame3DAtt(f), &aa );
  return true;
}

static bool _zDAEGetScale(xmlNode *node, zFrame3D *f)
{
  char *str;
  double rx, ry, rz;

  if( !( str = zXMLGetContent( node ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_EMPTYNODE );
    return false;
  }
  sscanf( str, "%lf %lf %lf", &rx, &ry, &rz );
  zFrame3DAtt(f)->c.xx *= rx;
  zFrame3DAtt(f)->c.yx *= rx;
  zFrame3DAtt(f)->c.zx *= rx;
  zFrame3DAtt(f)->c.xy *= ry;
  zFrame3DAtt(f)->c.yy *= ry;
  zFrame3DAtt(f)->c.zy *= ry;
  zFrame3DAtt(f)->c.xz *= rz;
  zFrame3DAtt(f)->c.yz *= rz;
  zFrame3DAtt(f)->c.zz *= rz;
  return true;
}

static bool _zDAEEvalXform(xmlNode *node, zDAEMeshList *mlist)
{
  xmlNode *np;
  zFrame3D f;
  zDAEMesh *mesh = NULL;
  char *id;

  zFrame3DIdent( &f );
  zXMLForEachNode( node, np ){
    zXMLCheckElementAndExec( np, "matrix",    _zDAEGetXform( np, &f ) ) else
    zXMLCheckElementAndExec( np, "translate", _zDAEGetTrans( np, &f ) ) else
    zXMLCheckElementAndExec( np, "rotate",    _zDAEGetRot( np, &f ) ) else
    zXMLCheckElementAndExec( np, "scale",     _zDAEGetScale( np, &f ) ) else
    if( zXMLCheckElement( np, "instance_geometry" ) ){
      if( !( id = _zDAEEvalURI( np, "url" ) ) ) return false;
      mesh = _zDAEMeshListFind( mlist, id );
    }
  }
  if( mesh )
    zFrame3DCopy( &f, &mesh->f );
  return true;
}

static bool _zDAEEvalLibraryVisualScenes(xmlNode *node, zDAEMeshList *mlist)
{
  xmlNode *np;

  if( !( node = zXMLFindNodeElement( node, "visual_scene" ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_SCENE_UNDEF );
    return false;
  }
  zXMLForEachNode( node->children, np ){
    zXMLCheckElementAndExec( np, "node", _zDAEEvalXform( np->children, mlist ) );
  }
  return true;
}

static bool _zDAEEval(xmlNode *node, zDAEMeshList *mlist)
{
  xmlNode *np;

  if( !( np = zXMLFindNodeElement( node, "COLLADA" ) ) ){
    ZRUNERROR( ZEO_ERR_DAE_NOT_COLLADA );
    return false;
  }
  zXMLForEachNode( node->children, np ){
    zXMLCheckElementAndExec( np, "library_geometries", _zDAEEvalLibraryGeometries( np->children, mlist ) ) else
    zXMLCheckElementAndExec( np, "library_visual_scenes", _zDAEEvalLibraryVisualScenes( np->children, mlist ) );
  }
  /* other element nodes than library_geometries (asset, library_images,
     library_effects, library_materials, library_controllers, library_visual_scenes,
     and scene) are ignored. */
  if( zListIsEmpty( mlist ) ){
    ZRUNERROR( ZEO_ERR_DAE_NO_GEOMETRY );
    return false;
  }
  return true;
}

static zPH3D *_zDAEMeshList2PH3D(zPH3D *ph, zDAEMeshList *mlist)
{
  zDAEMeshCell *mc;
  zDAEPrimitiveCell *pc;
  int nv, nf, ns, vi[3];
  int i, j, k, vo, fo;
  char *sp, *vp, buf[BUFSIZ];

  zPH3DInit( ph );
  /* count vertices and faces */
  nv = nf = 0;
  zListForEach( mlist, mc ){
    nv += mc->data.vert->vcount;
    zListForEach( &mc->data.plist, pc )
      nf += pc->data.pcount;
  }
  zPH3DAlloc( ph, nv, nf );
  /* read vertices */
  vo = 0;
  zListForEach( mlist, mc ){
    sp = mc->data.vert->content;
    for( i=0; i<mc->data.vert->vcount; i++ ){
      for( j=0; j<mc->data.vert->stride; j++ ){
        sp = zSTokenSkim( sp, buf, BUFSIZ );
        zPH3DVert(ph,vo+i)->e[mc->data.vert->accessor[j]] = atof( buf );
      }
      zXform3DDRC( &mc->data.f, zPH3DVert(ph,vo+i) );
    }
    vo += mc->data.vert->vcount;
  }
  /* read faces */
  vo = fo = 0;
  zListForEach( mlist, mc ){
    zListForEach( &mc->data.plist, pc ){
      sp = pc->data.p;
      vp = pc->data.vcount;
      for( i=0; i<pc->data.pcount; i++ ){
        if( vp ){ /* polylist */
          vp = zSTokenSkim( vp, buf, BUFSIZ );
          if( ( ns = atoi( buf ) ) != 3 ){
            ZRUNERROR( ZEO_ERR_DAE_PRIM_UNSUPPORTED );
            break;
          }
        } else /* triangles */
          ns = 3;
        for( j=0; j<ns; j++ ){
          for( k=0; k<pc->data.ni; k++ ){
            sp = zSTokenSkim( sp, buf, BUFSIZ );
            if( k == pc->data.voffset ) vi[j] = atoi( buf );
          }
        }
        zTri3DCreate( zPH3DFace(ph,fo+i), zPH3DVert(ph,vo+vi[0]), zPH3DVert(ph,vo+vi[1]), zPH3DVert(ph,vo+vi[2]) );
      }
      fo += pc->data.pcount;
    }
    vo += mc->data.vert->vcount;
  }
  return ph;
}

/* read a 3D polyhedron in DAE format. */
zPH3D *zPH3DFReadDAE(zPH3D *ph, char *filename)
{
  xmlDoc *doc;
  zDAEMeshList mlist;

  zXMLInit();
  if( !( doc = xmlReadFile( filename, NULL, XML_PARSE_RECOVER | XML_PARSE_COMPACT ) ) ){
    ZOPENERROR( filename );
    return NULL;
  }
  if( !_zDAEEval( xmlDocGetRootElement( doc ), &mlist ) ){
    ZRUNERROR( ZEO_ERR_DAE_PRIM_INVALID, filename );
    ph = NULL;
  } else
  if( !_zDAEMeshList2PH3D( ph, &mlist ) ){
    ZRUNERROR( ZEO_ERR_DAE_FAILCONV, filename );
    ph = NULL;
  }
  _zDAEMeshListDestroy( &mlist );
  xmlFreeDoc( doc );
  xmlCleanupParser();
  return ph;
}

/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape - 3D shape
 */

#include <zeo/zeo_shape.h>
#include <zeo/zeo_bv.h>

/* ********************************************************** */
/* CLASS: zShapeType
 * shape type identifier
 * ********************************************************** */

static char *__zshapetypename[] = {
  "none", "polyhedron", "box", "sphere", "ellipsoid", "cylinder", "ellipticcylinder", "cone", "nurbs",
  NULL,
};

/* expression for the type of 3D shapes. */
char *zShapeTypeExpr(zShapeType type)
{
  return __zshapetypename[zLimit(type,ZSHAPE_NONE,ZSHAPE_NURBS)];
}

/* convert a string to a type of 3D shapes. */
zShapeType zShapeTypeByStr(char str[])
{
  char **jp;
  zShapeType type;

  for( type=ZSHAPE_NONE, jp=__zshapetypename; *jp; jp++, type++ )
    if( !strcmp( str, *jp ) ) return type;
  return ZSHAPE_NONE;
}

/* ********************************************************** */
/* CLASS: zShape3D
 * 3D unit shape class
 * ********************************************************** */

/* initialize a 3D shape. */
zShape3D *zShape3DInit(zShape3D *shape)
{
  zNameSetPtr( shape, NULL );
  zShape3DType(shape) = ZSHAPE_NONE;
  shape->com = &zprim_none_com;
  zBox3DInit( zShape3DBB(shape) );
  zShape3DSetOptic( shape, NULL );
  return shape;
}

/* create a 3D shape as a box. */
zShape3D *zShape3DCreateBox(zShape3D *shape, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double d, double w, double h)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_BOX;
  zBox3DCreate( zShape3DBox(shape), c, ax, ay, az, d, w, h );
  shape->com = &zprim_box3d_com;
  return shape;
}

/* create a 3D shape as an axis-aligned box. */
zShape3D *zShape3DCreateBoxAlign(zShape3D *shape, zVec3D *c, double d, double w, double h)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_BOX;
  zBox3DCreateAlign( zShape3DBox(shape), c, d, w, h );
  shape->com = &zprim_box3d_com;
  return shape;
}

/* create a 3D shape as a sphere. */
zShape3D *zShape3DCreateSphere(zShape3D *shape, zVec3D *c, double r, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_SPHERE;
  zSphere3DCreate( zShape3DSphere(shape), c, r, div );
  shape->com = &zprim_sphere3d_com;
  return shape;
}

/* create a 3D shape as an ellipsoid. */
zShape3D *zShape3DCreateEllips(zShape3D *shape, zVec3D *c, zVec3D *ax, zVec3D *ay, zVec3D *az, double rx, double ry, double rz, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_ELLIPS;
  zEllips3DCreate( zShape3DEllips(shape), c, ax, ay, az, rx, ry, rz, div );
  shape->com = &zprim_ellips3d_com;
  return shape;
}

/* create a 3D shape as an axis-aligned ellipsoid. */
zShape3D *zShape3DCreateEllipsAlign(zShape3D *shape, zVec3D *c, double rx, double ry, double rz, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_ELLIPS;
  zEllips3DCreateAlign( zShape3DEllips(shape), c, rx, ry, rz, div );
  shape->com = &zprim_ellips3d_com;
  return shape;
}

/* create a 3D shape as a cylinder. */
zShape3D *zShape3DCreateCyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_CYLINDER;
  zCyl3DCreate( zShape3DCyl(shape), c1, c2, r, div );
  shape->com = &zprim_cyl3d_com;
  return shape;
}

/* create a 3D shape as an elliptic cylinder. */
zShape3D *zShape3DCreateECyl(zShape3D *shape, zVec3D *c1, zVec3D *c2, double r1, double r2, zVec3D *ref, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_ELLIPTICCYLINDER;
  zECyl3DCreate( zShape3DECyl(shape), c1, c2, r1, r2, ref, div );
  shape->com = &zprim_ecyl3d_com;
  return shape;
}

/* create a 3D shape as a cone. */
zShape3D *zShape3DCreateCone(zShape3D *shape, zVec3D *c, zVec3D *v, double r, int div)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_CONE;
  zCone3DCreate( zShape3DCone(shape), c, v, r, div );
  shape->com = &zprim_cone3d_com;
  return shape;
}

/* allocate memory for NURBS stored in a 3D shape. */
zShape3D *zShape3DAllocNURBS(zShape3D *shape, int size1, int size2, int dim1, int dim2)
{
  zShape3DInit( shape );
  zShape3DType(shape) = ZSHAPE_NURBS;
  zNURBS3DAlloc( zShape3DNURBS(shape), size1, size2, dim1, dim2 );
  shape->com = &zprim_nurbs_com;
  return shape;
}

/* destroy a 3D shape. */
void zShape3DDestroy(zShape3D *shape)
{
  if( !shape ) return;
  zNameDestroy( shape );
  shape->com->_destroy( &shape->body );
  zShape3DSetOptic( shape, NULL );
}

/* clone a 3D shape. */
zShape3D *zShape3DClone(zShape3D *org, zShape3D *cln, zOpticalInfo *oi)
{
  if( !zNameSet( cln, zName(org) ) ){
    ZALLOCERROR();
    return NULL;
  }
  zShape3DType(cln) = zShape3DType(org);
  if( !( cln->com = org->com )->_clone( &org->body, &cln->body ) ){
    ZALLOCERROR();
    return NULL;
  }
  zBox3DCopy( zShape3DBB(org), zShape3DBB(cln) );
  zShape3DSetOptic( cln, oi );
  return cln;
}

/* mirror a 3D shape. */
zShape3D *zShape3DMirror(zShape3D *src, zShape3D *dest, zAxis axis)
{
  zShape3DType(dest) = zShape3DType(src);
  if( !( dest->com = src->com )->_mirror( &src->body, &dest->body, axis ) ){
    ZALLOCERROR();
    return NULL;
  }
  zBox3DMirror( zShape3DBB(src), zShape3DBB(dest), axis );
  zShape3DSetOptic( dest, zShape3DOptic(src) );
  return dest;
}

/* transform coordinates of a 3D shape. */
zShape3D *zShape3DXfer(zShape3D *src, zFrame3D *f, zShape3D *dest)
{
  src->com->_xfer( &src->body, f, &dest->body );
  zBox3DXfer( zShape3DBB(src), f, zShape3DBB(dest) );
  return dest;
}

/* inversely transform coordinates of a 3D shape. */
zShape3D *zShape3DXferInv(zShape3D *src, zFrame3D *f, zShape3D *dest)
{
  src->com->_xferinv( &src->body, f, &dest->body );
  zBox3DXferInv( zShape3DBB(src), f, zShape3DBB(dest) );
  return dest;
}

/* closest point to a 3D shape. */
double zShape3DClosest(zShape3D *shape, zVec3D *p, zVec3D *cp)
{
  return shape->com->_closest( &shape->body, p, cp );
}

/* distance from a point to a 3D shape. */
double zShape3DPointDist(zShape3D *shape, zVec3D *p)
{
  return shape->com->_pointdist( &shape->body, p );
}

/* check if a point is inside of a 3D shape. */
bool zShape3DPointIsInside(zShape3D *shape, zVec3D *p, bool rim)
{
  return shape->com->_pointisinside( &shape->body, p, rim );
}

/* convert a shape to a polyhedron. */
zShape3D *zShape3DToPH(zShape3D *shape)
{
  zShapeBody tmp;
  void *ret = NULL;

  if( zShape3DType(shape) != ZSHAPE_PH ){
    shape->com->_clone( &shape->body, &tmp );
    zShape3DType(shape) = ZSHAPE_PH;
  }
  ret = shape->com->_toph( &tmp, zShape3DPH(shape) );
  shape->com = &zprim_ph3d_com;
  return ret ? shape : NULL;
}

typedef enum{ ZSHAPE_BB_NONE=0, ZSHAPE_BB_AABB, ZSHAPE_BB_OBB } _zShape3DBBType;

typedef struct{
  zShape3D *shape;
  zShape3D *sarray;
  int ns;
  zOpticalInfo *oarray;
  int no;
  _zShape3DBBType bb_type;
  bool referred;
} _zShape3DParam;

static bool _zShape3DFScan(FILE *fp, void *instance, char *buf, bool *success);

/* (static)
 * scan a 3D shape (internal function). */
bool _zShape3DFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  _zShape3DParam *prm;
  zShape3D *ref = NULL;

  prm = instance;
  if( strcmp( buf, "type" ) == 0 ){
    if( !( zShape3DType(prm->shape) = zShapeTypeByStr(zFToken(fp,buf,BUFSIZ) ) ) )
      return ( *success = false );
  } else if( strcmp( buf, "name" ) == 0 ){
    if( !( zNameSet( prm->shape, zFToken(fp,buf,BUFSIZ) ) ) )
      return ( *success = false );
  } else if( strcmp( buf, "optic" ) == 0 ){
    zFToken( fp, buf, BUFSIZ );
    zNameFind( prm->oarray, prm->no, buf, zShape3DOptic(prm->shape) );
  } else if( strcmp( buf, "bb" ) == 0 ){
    zFToken( fp, buf, BUFSIZ );
    if( strcmp( buf, "AABB" ) == 0 )
      prm->bb_type = ZSHAPE_BB_AABB;
    else
    if( strcmp( buf, "OBB" ) == 0 )
      prm->bb_type = ZSHAPE_BB_OBB;
  } else if( strcmp( buf, "mirror" ) == 0 ){
    zFToken( fp, buf, BUFSIZ );
    zNameFind( prm->sarray, prm->ns, buf, ref );
    if( !ref ){
      ZRUNWARN( ZEO_ERR_SHAPE_UNDEF, buf );
    } else{
      zFToken( fp, buf, BUFSIZ );
      if( !zShape3DMirror( ref, prm->shape, zAxisByStr(buf) ) )
        return ( *success = false );
      prm->referred = true;
    }
  } else
    return false;
  return true;
}

/* scan a 3D shape from a file. */
zShape3D *zShape3DFScan(FILE *fp, zShape3D *shape, zShape3D *sarray, int ns, zOpticalInfo *oarray, int no)
{
  zPrimCom *com[] = {
    &zprim_none_com,
    &zprim_ph3d_com, &zprim_box3d_com,
    &zprim_sphere3d_com, &zprim_ellips3d_com,
    &zprim_cyl3d_com, &zprim_ecyl3d_com, &zprim_cone3d_com,
    &zprim_nurbs_com,
  };
  _zShape3DParam prm;
  int cur;
  zAABox3D aabb;

  prm.shape = shape;
  prm.sarray = sarray;
  prm.ns = ns;
  prm.oarray = oarray;
  prm.no = no;
  prm.bb_type = ZSHAPE_BB_NONE;
  prm.referred = false;
  zShape3DInit( shape );
  cur = ftell( fp );
  if( !zFieldFScan( fp, _zShape3DFScan, &prm ) ) goto ERROR;
  if( prm.referred ) return shape;
  if( !zNamePtr( shape ) ){
    ZRUNERROR( ZEO_ERR_SHAPE_UNNAME );
    goto ERROR;
  }
  fseek( fp, cur, SEEK_SET );
  shape->com = com[zShape3DType(shape)];
  if( !shape->com->_fscan( fp, &shape->body ) ) goto ERROR;
  /* bounding box */
  switch( prm.bb_type ){
  case ZSHAPE_BB_AABB:
    zAABB( &aabb, zShape3DVertBuf(shape), zShape3DVertNum(shape), NULL );
    zAABox3DToBox3D( &aabb, zShape3DBB(shape) );
    break;
  case ZSHAPE_BB_OBB:
    zOBB( zShape3DBB(shape), zShape3DVertBuf(shape), zShape3DVertNum(shape) );
    break;
  default:
    zBox3DInit( zShape3DBB(shape) );
  }
  return shape;

 ERROR:
  zShape3DDestroy( shape );
  return NULL;
}

/* print a 3D shape to a file. */
void zShape3DFPrint(FILE *fp, zShape3D *shape)
{
  if( !shape ) return;
  fprintf( fp, "type : %s\n", zShapeTypeExpr(zShape3DType(shape)) );
  fprintf( fp, "name : %s\n", zName(shape) );
  if( zShape3DOptic(shape) )
    fprintf( fp, "optic: %s\n", zName( zShape3DOptic(shape) ) );
  shape->com->_fprint( fp, &shape->body );
  fprintf( fp, "\n" );
}

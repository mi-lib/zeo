/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map_terra - map class: terrain elevation map.
 */

#include <zeo/zeo_map.h>

/* ********************************************************** */
/* cell type of elevation map.
 * ********************************************************** */

/* initialize a terrain cell before identification. */
static zTerraCell *_zTerraCellInit(zTerraCell *cell)
{
  cell->z = 0;
  zVec3DCopy( ZVEC3DZ, &cell->norm );
  cell->var = 1;
  cell->travs = false;
  zVec3DZero( &cell->_prm );
  zMat3DIdent( &cell->_s );
  cell->_np = 0;
  return cell;
}

/* compute the residual of the height of a point from the estimate elevation. */
static double _zTerraCellResidual(zTerraCell *cell, zVec3D *p)
{
  return zTerraCellZ( cell, p->e[zX], p->e[zY] ) - p->e[zZ];
}

/* update the parameters of a terrain cell in identification process
 * based on recursive least square minimization. */
static void _zTerraCellUpdate(zTerraCell *cell, zVec3D *p)
{
  double e, d;
  zVec3D w, q, qd;

  e = _zTerraCellResidual( cell, p );
  zVec3DCreate( &w, p->e[zX], p->e[zY], 1 );
  zMulMat3DVec3D( &cell->_s, &w, &q );
  d = 1 + zVec3DInnerProd(&w,&q); /* (>=1) */
  zVec3DDiv( &q, d, &qd );
  zVec3DCatDRC( &cell->_prm, -e, &qd );
  zMat3DSubDyad( &cell->_s, &qd, &q );
  e = _zTerraCellResidual( cell, p );
  cell->var += ( e*e - cell->var ) / ( ++cell->_np );
}

/* form a terrain cell based on estimated parameters. */
static void _zTerraCellForm(zTerraCell *cell, double x, double y)
{
  cell->z = zTerraCellZ( cell, x, y );
  zVec3DCreate( &cell->norm, -cell->_prm.e[zX], -cell->_prm.e[zY], 1 );
  zVec3DNormalizeDRC( &cell->norm );
}

/* ********************************************************** */
/* elevation map.
 * ********************************************************** */

/* retrieve a grid of an elevation map. */
zTerraCell *zTerraGrid(zTerra *terra, int i, int j)
{
  return zArray2PosIsValid(&terra->gridmap,i,j) ? zTerraGridNC(terra,i,j) : NULL;
}

/* initialize an elevation map. */
zTerra *zTerraInit(zTerra *terra)
{
  terra->xmin = terra->xmax = 0;
  terra->ymin = terra->ymax = 0;
  terra->zmin = terra->zmax = 0;
  terra->dx = terra->dy = 0;
  zTerraSetTravsThreshold( terra, 0, 0, 0 );
  zArray2Init( &terra->gridmap );
  return terra;
}

/* level an elevation map into flat. */
zTerra *zTerraLevel(zTerra *terra)
{
  register int i, n;

  n = zTerraXSize(terra) * zTerraYSize(terra);
  for( i=0; i<n; i++ )
    _zTerraCellInit( &terra->gridmap.buf[i] );
  return terra;
}

/* allocate the internal grid array of an elevation map. */
zTerra *zTerraAlloc(zTerra *terra, int xsize, int ysize)
{
  if( xsize == 0 || ysize == 0 ){
    ZRUNERROR( ZEO_ERR_TERRA_INVSIZ );
    return NULL;
  }
  zArray2Alloc( &terra->gridmap, zTerraCell, xsize, ysize );
  if( zTerraXSize(terra) < xsize || zTerraYSize(terra) < ysize ){
    ZALLOCERROR();
    return NULL;
  }
  return zTerraLevel( terra );
}

/* set the region of an elevation map based on resolution. */
void zTerraSetRegion(zTerra *terra, double xmin, double ymin, double zmin, double zmax, double dx, double dy)
{
  terra->xmin = xmin;
  terra->ymin = ymin;
  terra->zmin = zmin;
  terra->zmax = zmax;
  terra->dx = dx;
  terra->dy = dy;
  zTerraAdjustMax( terra );
  zTerraSetTravsThreshold( terra, ZTERRA_TRAVS_TH_VAR, ZTERRA_TRAVS_TH_GRD, ZTERRA_TRAVS_TH_RES );
}

/* allocate the internal grid array of an elevation map based on resolution. */
zTerra *zTerraAllocRegion(zTerra *terra, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double dx, double dy)
{
  int xsize, ysize;

  if( dx < zTOL || dy < zTOL ){
    ZRUNERROR( ZEO_ERR_TERRA_INVRSL );
    return NULL;
  }
  xsize = floor( ( xmax - xmin ) / dx ) + 1;
  ysize = floor( ( ymax - ymin ) / dy ) + 1;
  return zTerraAllocGrid( terra, xmin, ymin, dx, dy, xsize, ysize, zmin, zmax );
}

/* allocate the internal grid array of an elevation map based on size. */
zTerra *zTerraAllocGrid(zTerra *terra, double xmin, double ymin, double dx, double dy, int xsize, int ysize, double zmin, double zmax)
{
  if( zTerraAlloc( terra, xsize, ysize ) ){
    zTerraSetRegion( terra, xmin, ymin, zmin, zmax, dx, dy );
    return terra;
  }
  return NULL;
}

/* destroy the internal grid array of an elevation map. */
void zTerraDestroy(zTerra *terra)
{
  zArray2Free( &terra->gridmap );
  zTerraInit( terra );
}

/* get the base grid a square with which contains a given point. */
static zTerraCell *_zTerraGetBaseGrid(zTerra *terra, zVec3D *p, int *i, int *j)
{
  if( p->e[zZ] < terra->zmin || p->e[zZ] > terra->zmax ) return NULL;
  *i = floor( ( p->e[zX] - terra->xmin ) / terra->dx );
  *j = floor( ( p->e[zY] - terra->ymin ) / terra->dy );
  return zTerraGrid(terra,*i,*j);
}

/* update an elevation map from a point. */
#define _zTerraUpdate(terra,p,g,i,j) do{\
  if( ( g = _zTerraGetBaseGrid( terra, p, &i, &j ) ) )\
    _zTerraCellUpdate( grid, p );\
  if( ( g = zTerraGrid(terra,i+1,j) ) )\
    _zTerraCellUpdate( grid, p );\
  if( ( g = zTerraGrid(terra,i  ,j+1) ) )\
    _zTerraCellUpdate( grid, p );\
  if( ( g = zTerraGrid(terra,i+1,j+1) ) )\
    _zTerraCellUpdate( grid, p );\
} while(0)
zTerra *zTerraUpdate(zTerra *terra, zVec3D *p)
{
  zTerraCell *grid;
  int ic = 0, jc = 0;

  _zTerraUpdate( terra, p, grid, ic, jc );
  return terra;
}

/* form terrain profile of an elevation map. */
zTerra *zTerraForm(zTerra *terra)
{
  register int i, j;

  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ )
      _zTerraCellForm( zTerraGridNC(terra,i,j), zTerraX(terra,i), zTerraY(terra,j) );
  return terra;
}

/* identify an elevation map from point cloud. */
zTerra *zTerraIdent(zTerra *terra, zVec3DList *pl)
{
  zVec3DListCell *pc;
  zTerraCell *grid;
  int ic = 0, jc = 0;

  zListForEach( pl, pc ){
    _zTerraUpdate( terra, pc->data, grid, ic, jc );
  }
  zTerraForm( terra );
  zTerraCheckTravs( terra );
  return terra;
}

/* check traversability of a grid of an elevation map based on estimate residual. */
static bool _zTerraCheckGridTravsRes(zTerra *terra, int i, int j, zVec3D *p)
{
  zTerraCell *cs;

  if( !( cs = zTerraGrid(terra,i,j) ) ) return true;
  return fabs( _zTerraCellResidual( cs, p ) ) > terra->travs_th_res ?
    false : true;
}

/* check traversability of a grid of an elevation map. */
static bool _zTerraCheckGridTravs(zTerra *terra, int i, int j)
{
  zTerraCell *grid;
  zVec3D p;

  grid = zTerraGridNC(terra,i,j);
  if( grid->var > terra->travs_th_var ) return false;
  if( zVec3DInnerProd(&grid->norm,ZVEC3DZ) < terra->travs_th_grd ) return false;
  zVec3DCreate( &p, zTerraX(terra,i), zTerraY(terra,j), grid->z );
  if( !_zTerraCheckGridTravsRes( terra, i-1, j-1, &p ) ||
      !_zTerraCheckGridTravsRes( terra, i-1, j  , &p ) ||
      !_zTerraCheckGridTravsRes( terra, i-1, j+1, &p ) ||
      !_zTerraCheckGridTravsRes( terra, i  , j-1, &p ) ||
      !_zTerraCheckGridTravsRes( terra, i  , j+1, &p ) ||
      !_zTerraCheckGridTravsRes( terra, i+1, j-1, &p ) ||
      !_zTerraCheckGridTravsRes( terra, i+1, j  , &p ) ||
      !_zTerraCheckGridTravsRes( terra, i+1, j+1, &p ) ) return false;
  return true;
}

/* check traversability of each grid of an elevation map. */
void zTerraCheckTravs(zTerra *terra)
{
  register int i, j;

  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ )
      zTerraGridNC(terra,i,j)->travs = _zTerraCheckGridTravs( terra, i, j );
}

/* acquire the actual z-range of an elevation map. */
void zTerraZRange(zTerra *terra, double *zmin, double *zmax)
{
  register int i, j;
  double z;

  *zmin = HUGE_VAL;
  *zmax =-HUGE_VAL;
  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ ){
      z = zTerraGridNC(terra,i,j)->z;
      if( z < *zmin ) *zmin = z;
      if( z > *zmax ) *zmax = z;
    }
}

/* compute contribution of a grid for height estimation on an elevation map. */
static void _zTerraZGrid(zTerra *terra, double x, double y, int i, int j, double *z, double *d)
{
  zTerraCell *grid;

  grid = zTerraGridNC(terra,i,j);
  *z = zTerraCellZ( grid, x, y );
  *d = ( zSqr( x - zTerraX(terra,i) ) + zSqr( y - zTerraY(terra,j) ) ) * grid->var;
}

/* estimate z-value at a given horizontal place of an elevation map. */
double zTerraZ(zTerra *terra, double x, double y)
{
  double z[4], d[4], w[4];
  zVec3D p;
  int i, j;

  zVec3DCreate( &p, x, y, 0.5*(terra->zmin+terra->zmax) );
  if( !_zTerraGetBaseGrid( terra, &p, &i, &j ) ){
    ZRUNERROR( ZEO_ERR_TERRA_OOREG, x, y );
    return NAN;
  }
  _zTerraZGrid( terra, x, y, i  , j  , &z[0], &d[0] );
  _zTerraZGrid( terra, x, y, i  , j+1, &z[1], &d[1] );
  _zTerraZGrid( terra, x, y, i+1, j  , &z[2], &d[2] );
  _zTerraZGrid( terra, x, y, i+1, j+1, &z[3], &d[3] );
  w[0] = d[1] * d[2] * d[3];
  w[1] = d[2] * d[3] * d[0];
  w[2] = d[3] * d[0] * d[1];
  w[3] = d[0] * d[1] * d[2];
  return ( w[0]*z[0] + w[1]*z[1] + w[2]*z[2] + w[3]*z[3] ) / ( w[0] + w[1] + w[2] + w[3] );
}

/* parse ZTK format */

static void *_zTerraOriginFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zTerra*)obj)->xmin = ZTKDouble( ztk );
  ((zTerra*)obj)->ymin = ZTKDouble( ztk );
  return obj;
}
static void *_zTerraResolFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zTerra*)obj)->dx = ZTKDouble( ztk );
  ((zTerra*)obj)->dy = ZTKDouble( ztk );
  if( zIsTiny(((zTerra*)obj)->dx) || zIsTiny(((zTerra*)obj)->dy) ){
    ZRUNERROR( ZEO_ERR_TERRA_INVRSL );
    return NULL;
  }
  return obj;
}
static void *_zTerraSizeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  int xsize, ysize;
  xsize = ZTKInt( ztk );
  ysize = ZTKInt( ztk );
  if( !zTerraAlloc( (zTerra*)obj, xsize, ysize ) ) return NULL;
  zTerraAdjustMax( (zTerra*)obj );
  return obj;
}
static void *_zTerraZRangeFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  double z1, z2;
  z1 = ZTKDouble( ztk );
  z2 = ZTKDouble( ztk );
  ((zTerra*)obj)->zmin = _zMin( z1, z2 );
  ((zTerra*)obj)->zmax = _zMax( z1, z2 );
  return obj;
}
static void *_zTerraVarThrsdFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zTerra*)obj)->travs_th_var = ZTKDouble( ztk );
  return obj;
}
static void *_zTerraGrdThrsdFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zTerra*)obj)->travs_th_grd = cos( zDeg2Rad( ZTKDouble( ztk ) ) );
  return obj;
}
static void *_zTerraResThrsdFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  ((zTerra*)obj)->travs_th_res = ZTKDouble( ztk );
  return obj;
}
static void *_zTerraGridFromZTK(void *obj, int i, void *arg, ZTK *ztk){
  zTerraCell *grid;
  int j, k;
  j = ZTKInt( ztk );
  k = ZTKInt( ztk );
  if( !( grid = zTerraGrid((zTerra*)obj,j,k) ) ){
    ZRUNERROR( ZEO_ERR_TERRA_OORAN );
    return NULL;
  }
  grid->z = ZTKDouble( ztk );
  grid->norm.e[zX] = ZTKDouble( ztk );
  grid->norm.e[zY] = ZTKDouble( ztk );
  grid->norm.e[zZ] = ZTKDouble( ztk );
  grid->var = ZTKDouble( ztk );
  grid->travs = ZTKInt( ztk );
  return obj;
}

static void _zTerraOriginFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g %.10g\n", ((zTerra*)obj)->xmin, ((zTerra*)obj)->ymin );
}
static void _zTerraResolFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g %.10g\n", ((zTerra*)obj)->dx, ((zTerra*)obj)->dy );
}
static void _zTerraSizeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%d %d\n", zTerraXSize((zTerra*)obj), zTerraYSize((zTerra*)obj) );
}
static void _zTerraZRangeFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g %.10g\n", ((zTerra*)obj)->zmin, ((zTerra*)obj)->zmax );
}
static void _zTerraVarThrsdFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zTerra*)obj)->travs_th_var );
}
static void _zTerraGrdThrsdFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", zRad2Deg( acos( ((zTerra*)obj)->travs_th_grd ) ) );
}
static void _zTerraResThrsdFPrint(FILE *fp, int i, void *obj){
  fprintf( fp, "%.10g\n", ((zTerra*)obj)->travs_th_res );
}

static ZTKPrp __ztk_prp_terra[] = {
  { "origin", 1, _zTerraOriginFromZTK, _zTerraOriginFPrint },
  { "resolution", 1, _zTerraResolFromZTK, _zTerraResolFPrint },
  { "size", 1, _zTerraSizeFromZTK, _zTerraSizeFPrint },
  { "zrange", 1, _zTerraZRangeFromZTK, _zTerraZRangeFPrint },
  { "th_var", 1, _zTerraVarThrsdFromZTK, _zTerraVarThrsdFPrint },
  { "th_grd", 1, _zTerraGrdThrsdFromZTK, _zTerraGrdThrsdFPrint },
  { "th_res", 1, _zTerraResThrsdFromZTK, _zTerraResThrsdFPrint },
  { "grid", -1, _zTerraGridFromZTK, NULL },
};

/* read a terrain elevation map from a ZTK format processor. */
zTerra *zTerraFromZTK(zTerra *terra, ZTK *ztk)
{
  zTerraInit( terra );
  return ZTKEvalKey( terra, NULL, ztk, __ztk_prp_terra );
}

/* print an elevation map out to a file. */
void zTerraFPrintZTK(FILE *fp, zTerra *terra)
{
  register int i, j;
  zTerraCell *grid;

  ZTKPrpKeyFPrint( fp, terra, __ztk_prp_terra );
  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ ){
      if( !( grid = zTerraGridNC(terra,i,j) ) ) continue;
      fprintf( fp, "grid: %d %d %.10g %.10g %.10g %.10g %.10g %d\n", i, j, grid->z, grid->norm.e[zX], grid->norm.e[zY], grid->norm.e[zZ], grid->var, grid->travs ? 1 : 0 );
    }
}

/* print an elevation map out to a file in a plot-friendly format. */
void zTerraDataFPrint(FILE *fp, zTerra *terra)
{
  register int i, j;
  zTerraCell *grid;

  for( i=0; i<zTerraXSize(terra); i++ )
    for( j=0; j<zTerraYSize(terra); j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g %.10g %.10g %.10g %.10g %d\n", zTerraX(terra,i), zTerraY(terra,j), grid->z, grid->norm.e[zX], grid->norm.e[zY], grid->norm.e[zZ], grid->var, grid->travs ? 1 : 0 );
    }
}

/* methods for abstraction */

static void *_zMapTerraAlloc(void){
  return zAlloc( zTerra, 1 );
}

static void _zMapTerraDestroy(void *terra){
  zTerraDestroy( terra );
}

static void *_zMapTerraParseZTK(void *terra, ZTK *ztk){
  return zTerraFromZTK( terra, ztk );
}

static void _zMapTerraFPrintZTK(FILE *fp, void *terra){
  zTerraFPrintZTK( fp, terra );
}

zMapCom zeo_map_terra_com = {
  typestr: "terra",
  _alloc: _zMapTerraAlloc,
  _destroy: _zMapTerraDestroy,
  _fromZTK: _zMapTerraParseZTK,
  _fprintZTK: _zMapTerraFPrintZTK,
};

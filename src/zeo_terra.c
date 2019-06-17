/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_terra - terrain elevation map.
 */

#include <zeo/zeo_terra.h>

/* ********************************************************** */
/* cell type of elevation map.
 * ********************************************************** */

static zTerraCell *_zTerraCellInit(zTerraCell *cell);
static double _zTerraCellResidual(zTerraCell *cell, zVec3D *p);
static void _zTerraCellUpdate(zTerraCell *cell, zVec3D *p);
static void _zTerraCellForm(zTerraCell *cell, double x, double y);

/* (static)
 * initialize a terrain cell before identification. */
zTerraCell *_zTerraCellInit(zTerraCell *cell)
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

/* (static)
 * compute the residual of the height of a point from the estimate elevation. */
double _zTerraCellResidual(zTerraCell *cell, zVec3D *p)
{
  return zTerraCellZ( cell, p->e[zX], p->e[zY] ) - p->e[zZ];
}

/* (static)
 * update the parameters of a terrain cell in identification process
 * based on recursive least square minimization. */
void _zTerraCellUpdate(zTerraCell *cell, zVec3D *p)
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

/* (static)
 * form a terrain cell based on estimated parameters. */
void _zTerraCellForm(zTerraCell *cell, double x, double y)
{
  cell->z = zTerraCellZ( cell, x, y );
  zVec3DCreate( &cell->norm, -cell->_prm.e[zX], -cell->_prm.e[zY], 1 );
  zVec3DNormalizeDRC( &cell->norm );
}

/* ********************************************************** */
/* elevation map.
 * ********************************************************** */

static zTerraCell *_zTerraGetBaseGrid(zTerra *terra, zVec3D *p, int *i, int *j);
static bool _zTerraCheckGridTravsRes(zTerra *terra, int i, int j, zVec3D *p);
static bool _zTerraCheckGridTravs(zTerra *terra, int i, int j);
static void _zTerraZGrid(zTerra *terra, double x, double y, int i, int j, double *z, double *d);
static bool _zTerraFScan(FILE *fp, void *instance, char *buf, bool *success);

/* retrieve a grid of an elevation map. */
zTerraCell *zTerraGrid(zTerra *terra, int i, int j)
{
  return i < 0 || i >= terra->_nx || j < 0 || j >= terra->_ny ?
    NULL : zTerraGridNC(terra,i,j);
}

/* initialize an elevation map. */
zTerra *zTerraInit(zTerra *terra)
{
  terra->xmin = terra->xmax = 0;
  terra->ymin = terra->ymax = 0;
  terra->zmin = terra->zmax = 0;
  terra->dx = terra->dy = 0;
  terra->_nx = terra->_ny = 0;
  zTerraSetTravsThreshold( terra, 0, 0, 0 );
  terra->grid = NULL;
  return terra;
}

/* level an elevation map into flat. */
zTerra *zTerraLevel(zTerra *terra)
{
  register int i, j;

  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ )
      _zTerraCellInit( zTerraGridNC(terra,i,j) );
  return terra;
}

/* allocate the internal grid array of an elevation map. */
zTerra *zTerraAlloc(zTerra *terra)
{
  if( terra->_nx == 0 || terra->_ny == 0 ){
    ZRUNERROR( ZEO_ERR_TERRA_INVSIZ );
    return NULL;
  }
  if( !( terra->grid = zAlloc( zTerraCell, terra->_nx * terra->_ny ) ) ){
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
  if( dx < zTOL || dy < zTOL ){
    ZRUNERROR( ZEO_ERR_TERRA_INVRSL );
    return NULL;
  }
  terra->_nx = floor( ( xmax - xmin ) / dx ) + 1;
  terra->_ny = floor( ( ymax - ymin ) / dy ) + 1;
  zTerraSetRegion( terra, xmin, ymin, zmin, zmax, dx, dy );
  return zTerraAlloc( terra );
}

/* allocate the internal grid array of an elevation map based on size. */
zTerra *zTerraAllocGrid(zTerra *terra, double xmin, double ymin, double dx, double dy, int nx, int ny, double zmin, double zmax)
{
  terra->_nx = nx;
  terra->_ny = ny;
  zTerraSetRegion( terra, xmin, ymin, zmin, zmax, dx, dy );
  return zTerraAlloc( terra );
}

/* free the internal grid array of an elevation map. */
void zTerraFree(zTerra *terra)
{
  free( terra->grid );
  zTerraInit( terra );
}

/* (static)
 * get the base grid a square with which contains a given point. */
zTerraCell *_zTerraGetBaseGrid(zTerra *terra, zVec3D *p, int *i, int *j)
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

  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ )
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

/* (static)
 * check traversability of a grid of an elevation map based on estimate residual. */
bool _zTerraCheckGridTravsRes(zTerra *terra, int i, int j, zVec3D *p)
{
  zTerraCell *cs;

  if( !( cs = zTerraGrid(terra,i,j) ) ) return true;
  return fabs( _zTerraCellResidual( cs, p ) ) > terra->travs_th_res ?
    false : true;
}

/* (static)
 * check traversability of a grid of an elevation map. */
bool _zTerraCheckGridTravs(zTerra *terra, int i, int j)
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

  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ )
      zTerraGridNC(terra,i,j)->travs = _zTerraCheckGridTravs( terra, i, j );
}

/* acquire the actual z-range of an elevation map. */
void zTerraZRange(zTerra *terra, double *zmin, double *zmax)
{
  register int i, j;
  double z;

  *zmin = HUGE_VAL;
  *zmax =-HUGE_VAL;
  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ ){
      z = zTerraGridNC(terra,i,j)->z;
      if( z < *zmin ) *zmin = z;
      if( z > *zmax ) *zmax = z;
    }
}

/* (static)
 * compute contribution of a grid for height estimation on an elevation map. */
void _zTerraZGrid(zTerra *terra, double x, double y, int i, int j, double *z, double *d)
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

/* (static)
 * scan an elevation map from a file (internal function). */
bool _zTerraFScan(FILE *fp, void *instance, char *buf, bool *success)
{
  zTerra *terra;
  zTerraCell *grid;

  *success = true;
  terra = instance;
  if( strcmp( buf, "origin" ) == 0 ){
    terra->xmin = zFDouble( fp );
    terra->ymin = zFDouble( fp );
  } else
  if( strcmp( buf, "resolution" ) == 0 ){
    terra->dx = zFDouble( fp );
    terra->dy = zFDouble( fp );
  } else
  if( strcmp( buf, "size" ) == 0 ){
    terra->_nx = zFInt( fp );
    terra->_ny = zFInt( fp );
  } else
  if( strcmp( buf, "th_var" ) == 0 ){
    terra->travs_th_var = zFDouble( fp );
  } else
  if( strcmp( buf, "th_grd" ) == 0 ){
    terra->travs_th_grd = cos( zDeg2Rad( zFDouble( fp ) ) );
  } else
  if( strcmp( buf, "th_res" ) == 0 ){
    terra->travs_th_res = zFDouble( fp );
  } else
  if( strcmp( buf, "grid" ) == 0 ){
    int i, j;
    if( !terra->grid ){
      zTerraAdjustMax( terra );
      if( !zTerraAlloc( terra ) )
        return ( *success = false );
    }
    i = zFInt( fp );
    j = zFInt( fp );
    if( !( grid = zTerraGrid(terra,i,j) ) ){
      ZRUNERROR( ZEO_ERR_TERRA_OORAN );
      return ( *success = false );
    }
    grid->z = zFDouble( fp );
    grid->norm.e[zX] = zFDouble( fp );
    grid->norm.e[zY] = zFDouble( fp );
    grid->norm.e[zZ] = zFDouble( fp );
    grid->var = zFDouble( fp );
    grid->travs = zFInt( fp );
  } else
    return false;
  return true;
}

/* scan an elevation map from a file. */
zTerra *zTerraFScan(FILE *fp, zTerra *terra)
{
  zTerraInit( terra );
  if( zFieldFScan( fp, _zTerraFScan, terra ) ) return terra;
  zTerraFree( terra );
  return NULL;
}

/* print an elevation map out to a file. */
void zTerraFPrint(FILE *fp, zTerra *terra)
{
  register int i, j;
  zTerraCell *grid;

  fprintf( fp, "origin: %.10g %.10g\n", terra->xmin, terra->ymin );
  fprintf( fp, "resolution: %.10g %.10g\n", terra->dx, terra->dy );
  fprintf( fp, "size: %d %d\n", terra->_nx, terra->_ny );
  fprintf( fp, "zrange: %.10g %.10g\n", terra->zmin, terra->zmax );
  fprintf( fp, "th_var: %.10g\n", terra->travs_th_var );
  fprintf( fp, "th_grd: %.10g\n", zRad2Deg( acos( terra->travs_th_grd ) ) );
  fprintf( fp, "th_res: %.10g\n", terra->travs_th_res );
  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "grid: %d %d %.10g %.10g %.10g %.10g %.10g %d\n", i, j, grid->z, grid->norm.e[zX], grid->norm.e[zY], grid->norm.e[zZ], grid->var, grid->travs ? 1 : 0 );
    }
}

/* print an elevation map out to a file in a plot-friendly format. */
void zTerraDataFPrint(FILE *fp, zTerra *terra)
{
  register int i, j;
  zTerraCell *grid;

  for( i=0; i<terra->_nx; i++ )
    for( j=0; j<terra->_ny; j++ ){
      grid = zTerraGridNC(terra,i,j);
      fprintf( fp, "%.10g %.10g %.10g %.10g %.10g %.10g %.10g %d\n", zTerraX(terra,i), zTerraY(terra,j), grid->z, grid->norm.e[zX], grid->norm.e[zY], grid->norm.e[zZ], grid->var, grid->travs ? 1 : 0 );
    }
}

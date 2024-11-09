/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_map_terra - map class: terrain elevation map.
 */

#ifndef __ZEO_MAP_TERRA_H__
#define __ZEO_MAP_TERRA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/*! \brief cell type of elevation map.
 *//* ******************************************************* */
typedef struct{
  double z;    /*!< \brief elevation */
  zVec3D norm; /*!< \brief normal vector */
  double var;  /*!< \brief variance */
  bool travs;  /*!< \brief traversability */
  /*! \cond */
  zVec3D _prm;  /* plane parameters */
  zMat3D _s;    /* covariant matrix */
  int _np;      /* number of assigned points */
  /*! \endcond */
} zTerraCell;

#define zTerraCellZ(c,x,y) \
  ( (c)->_prm.e[zX] * (x) + (c)->_prm.e[zY] * (y) + (c)->_prm.e[zZ] )

zArray2Class( zTerraCellArray, zTerraCell );

/* ********************************************************** */
/*! \brief elevation map.
 *//* ******************************************************* */
typedef struct{
  double xmin; /*!< \brief minimum boundary in x */
  double xmax; /*!< \brief maximum boundary in x */
  double ymin; /*!< \brief minimum boundary in y */
  double ymax; /*!< \brief maximum boundary in y */
  double zmin; /*!< \brief minimum boundary in z */
  double zmax; /*!< \brief maximum boundary in z */
  double dx;   /*!< \brief spatial interval of grids in x */
  double dy;   /*!< \brief spatial interval of grids in y */
  double travs_th_var; /*!< \brief traversability threshold of variance */
  double travs_th_grd; /*!< \brief traversability threshold of gradient */
  double travs_th_res; /*!< \brief traversability threshold of residual */
  zTerraCellArray gridmap; /*!< \brief grid map */
} zTerra;

/* \brief a grid at (\a i, \a j) of a map in \a terra. */
#define zTerraGridNC(terra,i,j) zArray2ElemNC(&(terra)->gridmap,i,j)

#define zTerraXSize(terra) zArray2RowSize(&(terra)->gridmap)
#define zTerraYSize(terra) zArray2ColSize(&(terra)->gridmap)

#define zTerraX(terra,i)   ( (terra)->xmin + (i)*(terra)->dx )
#define zTerraY(terra,j)   ( (terra)->ymin + (j)*(terra)->dy )

#define ZTERRA_TRAVS_TH_VAR ( 1.0e-1 )
#define ZTERRA_TRAVS_TH_GRD zDeg2Rad( 45 )
#define ZTERRA_TRAVS_TH_RES ( 1.0e-1 )

/*! \brief retrieve a grid of an elevation map. */
__ZEO_EXPORT zTerraCell *zTerraGrid(zTerra *terra, int i, int j);

/*! \brief initialize an elevation map. */
__ZEO_EXPORT zTerra *zTerraInit(zTerra *terra);

/*! \brief level an elevation map into flat. */
__ZEO_EXPORT zTerra *zTerraLevel(zTerra *terra);

/*! \brief allocate the internal grid array of an elevation map. */
__ZEO_EXPORT zTerra *zTerraAlloc(zTerra *terra, int xsize, int ysize);

/*! \brief set thresholds of an elevation map for traversability check. */
#define zTerraSetTravsThreshold(terra,tv,tg,tr) do{\
  (terra)->travs_th_var = (tv);\
  (terra)->travs_th_grd = cos(tg);\
  (terra)->travs_th_res = (tr);\
} while(0)

/*! \brief adjust the maximum horizontal boundary based on resolution and size of an elevation map. */
#define zTerraAdjustMax(terra) do{\
  (terra)->xmax = (terra)->xmin + ( zTerraXSize(terra) - 1 ) * (terra)->dx;\
  (terra)->ymax = (terra)->ymin + ( zTerraYSize(terra) - 1 ) * (terra)->dy;\
} while(0)

/*! \brief set the region of an elevation map based on resolution. */
__ZEO_EXPORT void zTerraSetRegion(zTerra *terra, double xmin, double ymin, double zmin, double zmax, double dx, double dy);

/*! \brief allocate the internal grid array of an elevation map based on resolution. */
__ZEO_EXPORT zTerra *zTerraAllocRegion(zTerra *terra, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, double dx, double dy);

/*! \brief allocate the internal grid array of an elevation map based on size. */
__ZEO_EXPORT zTerra *zTerraAllocGrid(zTerra *terra, double xmin, double ymin, double dx, double dy, int xsize, int ysize, double zmin, double zmax);

/*! \brief destroy the internal grid array of an elevation map. */
__ZEO_EXPORT void zTerraDestroy(zTerra *terra);

/*! \brief update an elevation map from a point. */
__ZEO_EXPORT zTerra *zTerraUpdate(zTerra *terra, zVec3D *p);

/*! \brief form terrain profile of an elevation map. */
__ZEO_EXPORT zTerra *zTerraForm(zTerra *terra);

/*! \brief identify an elevation map from point cloud. */
__ZEO_EXPORT zTerra *zTerraIdent(zTerra *terra, zVec3DList *pl);

/*! \brief check traversability of each grid of an elevation map. */
__ZEO_EXPORT void zTerraCheckTravs(zTerra *terra);

/*! \brief acquire the actual z-range of an elevation map. */
__ZEO_EXPORT void zTerraZRange(zTerra *terra, double *zmin, double *zmax);

/*! \brief adjust z-range of an elevation map. */
#define zTerraAdjustZRange(terra) zTerraZRange( (terra), &(terra)->zmin, &(terra)->zmax )

/*! \brief estimate z-value at a given horizontal place of an elevation map. */
__ZEO_EXPORT double zTerraZ(zTerra *terra, double x, double y);

/* ZTK */

#define ZTK_KEY_ZEO_MAP_TERRA_ORIGIN     "origin"
#define ZTK_KEY_ZEO_MAP_TERRA_RESOLUTION "resolution"
#define ZTK_KEY_ZEO_MAP_TERRA_SIZE       "size"
#define ZTK_KEY_ZEO_MAP_TERRA_ZRANGE     "zrange"
#define ZTK_KEY_ZEO_MAP_TERRA_TH_VAR     "th_var"
#define ZTK_KEY_ZEO_MAP_TERRA_TH_GRID    "th_grd"
#define ZTK_KEY_ZEO_MAP_TERRA_TH_RES     "th_res"
#define ZTK_KEY_ZEO_MAP_TERRA_GRID       "grid"

/*! \brief read a terrain elevation map from a ZTK format processor. */
__ZEO_EXPORT zTerra *zTerraFromZTK(zTerra *terra, ZTK *ztk);

/*! \brief print an elevation map out to a file in ZTK format. */
__ZEO_EXPORT void zTerraFPrintZTK(FILE *fp, zTerra *terra);

/*! \brief print an elevation map out to a file in a plot-friendly format. */
__ZEO_EXPORT void zTerraDataFPrint(FILE *fp, zTerra *terra);

/* methods for abstraction */
__ZEO_EXPORT zMapCom zeo_map_terra_com;

#define zMapTerra(m) ((zTerra*)(m)->body)

__END_DECLS

#endif /* __ZEO_MAP_TERRA_H__ */

#include <zeo/zeo.h>

typedef struct{
  zPH3D ph;
  zBox3D obb;
  zAABox3D aabb;
} shape_t;

void output_ph(FILE *fp, zPH3D *ph, int i)
{
  zOpticalInfo oi;
  char name[BUFSIZ];

  sprintf( name, "color%d", i );
  zOpticalInfoCreateSimple( &oi, zRandF(0.3,1), zRandF(0.3,1),zRandF(0.3,1), name );
  fprintf( fp, "[optic]\n" );
  zOpticalInfoFPrintZTK( fp, &oi );
  fprintf( fp, "[shape]\n" );
  fprintf( fp, "name: shape%d\n", i );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "BB: AABB\n" );
  fprintf( fp, "optic: color%d\n", i );
  zPH3DFPrintZTK( fp, ph );
  zOpticalInfoDestroy( &oi );
}

void create_ph(shape_t *shape, int ns, int nv)
{
  zVec3D *v;
  int i, j;
  double x0, y0, z0;
  FILE *fp;

  v = zAlloc( zVec3D, nv );
  fp = fopen( "vast_src", "w" );
  for( i=0; i<ns; i++ ){
    x0 = zRandF(-0.4,0.4);
    y0 = zRandF(-0.4,0.4);
    z0 = zRandF(-0.4,0.4);
    for( j=0; j<nv; j++ )
      zVec3DCreate( &v[j], x0+zRandF(-0.05,0.05), y0+zRandF(-0.05,0.05), z0+zRandF(-0.05,0.05) );
    zConvexHull3D( &shape[i].ph, v, nv );
    zOBB3D( &shape[i].obb, zPH3DVertBuf(&shape[i].ph), zPH3DVertNum(&shape[i].ph) );
    zAABB3D( &shape[i].aabb, zPH3DVertBuf(&shape[i].ph), zPH3DVertNum(&shape[i].ph), NULL );
    output_ph( fp, &shape[i].ph, i );
  }
  fclose( fp );
  zFree( v );
}

/* GJK */

void colchk_bruteforce_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zPH3D *ph1, *ph2;
  FILE *fp;

  fp = fopen( "vast_GJK", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zGJK( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), &ca, &cb ) ){
        fprintf( fp, "%d-%d\n", i, j );
      }
    }
  }
  fclose( fp );
}

void colchk_obb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zPH3D *ph1, *ph2;
  FILE *fp;

  fp = fopen( "vast_OBB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
        if( zGJK( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), &ca, &cb ) ){
          fprintf( fp, "%d-%d\n", i, j );
        }
      }
    }
  }
  fclose( fp );
}

void colchk_aabb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zPH3D *ph1, *ph2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zAABB3D( &shape[i].aabb, zPH3DVertBuf(&shape[i].ph), zPH3DVertNum(&shape[i].ph), NULL );

  fp = fopen( "vast_AABB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zGJK( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), &ca, &cb ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
    }
  }
  fclose( fp );
}

void colchk_obb_aabb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zPH3D *ph1, *ph2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zBox3DToAABox3D( &shape[i].obb, &shape[i].aabb );

  fp = fopen( "vast_OBB_AABB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zGJK( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2), &ca, &cb ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
    }
  }
  fclose( fp );
}

/* MPR */

void colchk_bruteforce_mpr(shape_t *shape, int ns)
{
  int i, j;
  zPH3D *ph1, *ph2;
  FILE *fp;

  fp = fopen( "vast_MPR", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zMPR( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2) ) ){
        fprintf( fp, "%d-%d\n", i, j );
      }
    }
  }
  fclose( fp );
}

void colchk_obb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zPH3D *ph1, *ph2;
  FILE *fp;

  fp = fopen( "vast_OBB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
        if( zMPR( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2) ) ){
          fprintf( fp, "%d-%d\n", i, j );
        }
      }
    }
  }
  fclose( fp );
}

void colchk_aabb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zPH3D *ph1, *ph2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zAABB3D( &shape[i].aabb, zPH3DVertBuf(&shape[i].ph), zPH3DVertNum(&shape[i].ph), NULL );

  fp = fopen( "vast_AABB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zMPR( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2) ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
    }
  }
  fclose( fp );
}

void colchk_obb_aabb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zPH3D *ph1, *ph2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zBox3DToAABox3D( &shape[i].obb, &shape[i].aabb );

  fp = fopen( "vast_OBB_AABB", "w" );
  for( i=0; i<ns; i++ ){
    ph1 = &shape[i].ph;
    for( j=i+1; j<ns; j++ ){
      ph2 = &shape[j].ph;
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zMPR( zPH3DVertBuf(ph1), zPH3DVertNum(ph1), zPH3DVertBuf(ph2), zPH3DVertNum(ph2) ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
    }
  }
  fclose( fp );
}


#define NS 100
#define NV 100

int main(int argc, char *argv[])
{
  shape_t *shape;
  int ns, nv, i;
  clock_t t1, t2;

  zRandInit();
  ns = argc > 1 ? atoi(argv[1]) : NS; /* number of shapes */
  nv = argc > 2 ? atoi(argv[2]) : NV; /* number of vertices of each shape */
  shape = zAlloc( shape_t, ns );
  create_ph( shape, ns, nv );

  t1 = clock();
  colchk_bruteforce_gjk( shape, ns );
  t2 = clock();
  printf( "GJK:      time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_obb_gjk( shape, ns );
  t2 = clock();
  printf( "OBB+GJK:  time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_aabb_gjk( shape, ns );
  t2 = clock();
  printf( "AABB+GJK: time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_obb_aabb_gjk( shape, ns );
  t2 = clock();
  printf( "OBB-AABB: time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_bruteforce_mpr( shape, ns );
  t2 = clock();
  printf( "MPR:      time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_obb_mpr( shape, ns );
  t2 = clock();
  printf( "OBB+MPR:  time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_aabb_mpr( shape, ns );
  t2 = clock();
  printf( "AABB+MPR: time=%d\n", (int)(t2-t1) );

  t1 = clock();
  colchk_obb_aabb_mpr( shape, ns );
  t2 = clock();
  printf( "OBB-AABB: time=%d\n", (int)(t2-t1) );

  for( i=0; i<ns; i++ ){
    zPH3DDestroy( &shape[i].ph );
  }
  free( shape );
  return 0;
}

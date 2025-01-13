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
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_OPTIC );
  zOpticalInfoFPrintZTK( fp, &oi );
  fprintf( fp, "[%s]\n", ZTK_TAG_ZEO_SHAPE );
  fprintf( fp, "name: shape%d\n", i );
  fprintf( fp, "type: polyhedron\n" );
  fprintf( fp, "optic: color%d\n", i );
  zPH3DFPrintZTK( fp, ph );
  zOpticalInfoDestroy( &oi );
}

void create_ph(shape_t *shape, int ns, int nv)
{
  zVec3DData data_src, data_ph;
  zVec3D v;
  int i, j;
  double x0, y0, z0;
  FILE *fp;

  zVec3DDataInitArray( &data_src, nv );
  fp = fopen( "vast_src", "w" );
  for( i=0; i<ns; i++ ){
    x0 = zRandF(-0.4,0.4);
    y0 = zRandF(-0.4,0.4);
    z0 = zRandF(-0.4,0.4);
    for( j=0; j<nv; j++ ){
      zVec3DCreate( &v, x0+zRandF(-0.05,0.05), y0+zRandF(-0.05,0.05), z0+zRandF(-0.05,0.05) );
      zVec3DDataAdd( &data_src, &v );
    }
    zVec3DDataConvexHull( &data_src, &shape[i].ph );
    zVec3DDataAssignArray( &data_ph, &shape[i].ph.vert );
    zVec3DDataOBB( &data_ph, &shape[i].obb );
    zVec3DDataAABB( &data_ph, &shape[i].aabb, NULL );
    output_ph( fp, &shape[i].ph, i );
    zVec3DDataDestroy( &data_ph );
  }
  fclose( fp );
  zVec3DDataDestroy( &data_src );
}

/* GJK */

void colchk_bruteforce_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zVec3DData data1, data2;
  FILE *fp;

  fp = fopen( "vast_GJK", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zGJK( &data1, &data2, &ca, &cb ) ){
        fprintf( fp, "%d-%d\n", i, j );
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_obb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zVec3DData data1, data2;
  FILE *fp;

  fp = fopen( "vast_OBB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
        if( zGJK( &data1, &data2, &ca, &cb ) ){
          fprintf( fp, "%d-%d\n", i, j );
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_aabb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zVec3DData data1, data2;
  FILE *fp;

  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    zVec3DDataAABB( &data1, &shape[i].aabb, NULL );
  }

  fp = fopen( "vast_AABB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zGJK( &data1, &data2, &ca, &cb ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_obb_aabb_gjk(shape_t *shape, int ns)
{
  int i, j;
  zVec3D ca, cb;
  zVec3DData data1, data2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zBox3DToAABox3D( &shape[i].obb, &shape[i].aabb );

  fp = fopen( "vast_OBB_AABB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zGJK( &data1, &data2, &ca, &cb ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

/* MPR */

void colchk_bruteforce_mpr(shape_t *shape, int ns)
{
  int i, j;
  zVec3DData data1, data2;
  FILE *fp;

  fp = fopen( "vast_MPR", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zMPR( &data1, &data2 ) ){
        fprintf( fp, "%d-%d\n", i, j );
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_obb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zVec3DData data1, data2;
  FILE *fp;

  fp = fopen( "vast_OBB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
        if( zMPR( &data1, &data2 ) ){
          fprintf( fp, "%d-%d\n", i, j );
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_aabb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zVec3DData data1, data2;
  FILE *fp;

  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    zVec3DDataAABB( &data1, &shape[i].aabb, NULL );
  }

  fp = fopen( "vast_AABB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zMPR( &data1, &data2 ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
  }
  fclose( fp );
}

void colchk_obb_aabb_mpr(shape_t *shape, int ns)
{
  int i, j;
  zVec3DData data1, data2;
  FILE *fp;

  for( i=0; i<ns; i++ )
    zBox3DToAABox3D( &shape[i].obb, &shape[i].aabb );

  fp = fopen( "vast_OBB_AABB", "w" );
  for( i=0; i<ns; i++ ){
    zVec3DDataAssignArray( &data1, &shape[i].ph.vert );
    for( j=i+1; j<ns; j++ ){
      zVec3DDataAssignArray( &data2, &shape[j].ph.vert );
      if( zColChkAABox3D( &shape[i].aabb, &shape[j].aabb ) ){
        if( zColChkBox3D( &shape[i].obb, &shape[j].obb ) ){
          if( zMPR( &data1, &data2 ) ){
            fprintf( fp, "%d-%d\n", i, j );
          }
        }
      }
      zVec3DDataDestroy( &data2 );
    }
    zVec3DDataDestroy( &data1 );
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

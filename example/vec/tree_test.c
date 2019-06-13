#include <zeo/zeo_vec3d.h>
#include <sys/time.h>

void output_node(FILE *fp, zVecTree3D *node)
{
  /* face 1 */
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmin.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  /* face 2 */
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmin.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmax.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmax.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmin.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmin.e[zX], node->vmin.e[zY], node->vmax.e[zZ] );
  /* pole x 4 */
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmin.e[zX], node->vmin.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmin.e[zX], node->vmax.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmax.e[zX], node->vmax.e[zY], node->vmax.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmax.e[zX], node->vmin.e[zY], node->vmax.e[zZ] );
}

void output(FILE *fp, zVecTree3D *tree)
{
  output_node( fp, tree );
  if( tree->s[0] )
    output( fp, tree->s[0] );
  if( tree->s[1] )
    output( fp, tree->s[1] );
}

int deltatime(struct timeval *tv1, struct timeval *tv2)
{
  return (int)( (tv2->tv_sec-tv1->tv_sec)*1000000+tv2->tv_usec-tv1->tv_usec );
}

#define N 1000

int main(int argc, char *argv[])
{
  zVec3DList list; /* for comparison */
  zVecTree3D tree, *node;
  zVec3D v, *nn;
  int i, n;
  double dmin1, dmin2;
  FILE *fp;
  struct timeval tv1, tv2;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  zListInit( &list );
  zVecTree3DInit( &tree );
  zVec3DCreate( &tree.vmin,-10,-10,-10 );
  zVec3DCreate( &tree.vmax, 10, 10, 10 );

  fp = fopen( "src", "w" );
  for( i=0; i<n; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );
    zVec3DListInsert( &list, &v );
    zVecTree3DAdd( &tree, &v );
    zVec3DDataNLFPrint( fp, &v );
  }
  fclose( fp );

  fp = fopen( "par", "w" );
  output( fp, &tree );
  fclose( fp );

  zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), zRandF(-10,10) );

  fp = fopen( "tst", "w" );
  node = zVecTree3DPart( &tree, &v );
  zVec3DDataNLFPrint( fp, &v );
  fprintf( fp, "\n" );
  output_node( fp, node );
  fclose( fp );

  gettimeofday( &tv1, NULL );
  dmin1 = zVecTree3DNN( &tree, &v, &node );
  gettimeofday( &tv2, NULL );
  eprintf( "kd-tree: %g - ", dmin1 ); zVec3DFPrint( stderr, &node->v );
  printf( "%d %d ", n, deltatime(&tv1,&tv2) );
  fp = fopen( "nn", "w" );
  zVec3DDataNLFPrint( fp, &v );
  zVec3DDataNLFPrint( fp, &node->v );
  fclose( fp );
  /* for comparison */
  gettimeofday( &tv1, NULL );
  nn = zVec3DListNN( &list, &v, &dmin2 );
  gettimeofday( &tv2, NULL );
  eprintf( "naive  : %g - ", dmin2 ); zVec3DFPrint( stderr, nn );
  printf( "%d\n", deltatime(&tv1,&tv2) );
  fp = fopen( "nnn", "w" );
  zVec3DDataNLFPrint( fp, &v );
  zVec3DDataNLFPrint( fp, nn );
  fclose( fp );

  if( !zIsTiny( dmin1-dmin2 ) )
    eprintf( "FAILED! %g/%g (err.=%g)\n", dmin1, dmin2, dmin1-dmin2 );

  zVec3DListDestroy( &list );
  zVecTree3DDestroy( &tree );
  return 0;
}

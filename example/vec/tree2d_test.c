#include <zeo/zeo_vec3d.h>

void output_node(FILE *fp, zVecTree3D *node) /* dummy */
{
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmin.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmax.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n",   node->vmax.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
  fprintf( fp, "%g %g %g\n\n", node->vmin.e[zX], node->vmin.e[zY], node->vmin.e[zZ] );
}

void output(FILE *fp, zVecTree3D *tree)
{
  output_node( fp, tree );
  if( tree->s[0] )
    output( fp, tree->s[0] );
  if( tree->s[1] )
    output( fp, tree->s[1] );
}

#define N 100

int main(int argc, char *argv[])
{
  zVec3DList list; /* for comparison */
  zVecTree3D tree, *node;
  zVec3D v, *nn;
  double dmin;
  int i;
  FILE *fp;

  zRandInit();
  zListInit( &list );
  zVecTree3DInit( &tree );
  zVec3DCreate( &tree.vmin,-10,-10, 0 );
  zVec3DCreate( &tree.vmax, 10, 10, 0 );
  fp = fopen( "src", "w" );
  for( i=0; i<N; i++ ){
    zVec3DCreate( &v, zRandF(-10,10), zRandF(-10,10), 0 );
    zVec3DListInsert( &list, &v );
    zVecTree3DAdd( &tree, &v );
    zVec3DDataNLFWrite( fp, &v );
  }
  fclose( fp );

  fp = fopen( "par", "w" );
  output( fp, &tree );
  fclose( fp );

  zVec3DCreate( &v, zRandF(-5,5), zRandF(-5,5), 0 );
  fp = fopen( "tst", "w" );
  node = zVecTree3DPart( &tree, &v );
  zVec3DDataNLFWrite( fp, &v );
  fprintf( fp, "\n" );
  output_node( fp, node );
  fclose( fp );

  zVecTree3DNN( &tree, &v, &node );
  printf( "kd-tree: " ); zVec3DWrite( &node->v );
  fp = fopen( "nn", "w" );
  zVec3DDataNLFWrite( fp, &v );
  zVec3DDataNLFWrite( fp, &node->v );
  fclose( fp );
  /* for comparison */
  nn = zVec3DListNN( &list, &v, &dmin );
  printf( "naive  : " ); zVec3DWrite( nn );
  fp = fopen( "nnn", "w" );
  zVec3DDataNLFWrite( fp, &v );
  zVec3DDataNLFWrite( fp, nn );
  fclose( fp );

  zVec3DListDestroy( &list );
  zVecTree3DDestroy( &tree );
  return 0;
}

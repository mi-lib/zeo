#include <zeo/zeo_col.h>

static bool _zAABoxTree3DIsLeaf(zAABoxTree3D *node);
bool _zAABoxTree3DIsLeaf(zAABoxTree3D *node)
{
  return node->bp[0] == NULL && node->bp[1] == NULL ? true : false;
}


void _colchk_aabox(FILE *fp, zAABoxTree3D *tree, zAABox3D *box)
{
  eprintf( "check!\n" );
  if( zColChkAABox3D( &tree->box, box ) ){
    if( _zAABoxTree3DIsLeaf( tree ) ){
      zAABox3DDataFPrint( fp, &tree->box );
    } else{
      _colchk_aabox( fp, tree->bp[0], box );
      _colchk_aabox( fp, tree->bp[1], box );
    }
  }
}

void colchk_aabox(zAABoxTree3D *tree, zAABox3D *box)
{
  FILE *fp;

  fp = fopen( "b", "w" );
  zAABox3DDataFPrint( fp, box );
  fclose( fp );

  fp = fopen( "c", "w" );
  _colchk_aabox( fp, tree->bp[0], box );
  fclose( fp );
}

void output(zAABoxTree3D *tree, int layer)
{
  FILE *fp;
  char filename[BUFSIZ];

  if( layer == 0 )
    return output( tree->bp[0], 1 );
  sprintf( filename, "%d", layer );
  fp = fopen( filename, "a" );
  zAABox3DDataFPrint( fp, &tree->box );
  fclose( fp );
  if( tree->bp[0] )
    output( tree->bp[0], layer+1 );
  if( tree->bp[1] )
    output( tree->bp[1], layer+1 );
}

void gen_aabox(zAABox3D *box)
{
  double x1, y1, z1, x2, y2, z2;

  x1 = zRandF(-0.5,0.5);
  x2 = zRandF(-0.5,0.5);
  y1 = zRandF(-0.5,0.5);
  y2 = zRandF(-0.5,0.5);
  z1 = zRandF(-0.5,0.5);
  z2 = zRandF(-0.5,0.5);
  zAABox3DCreate( box, zMin(x1,x2), zMin(y1,y2), zMin(z1,z2), zMax(x1,x2), zMax(y1,y2), zMax(z1,z2) );
}

#define N 10

int main(int argc, char *argv[])
{
  zAABoxTree3D tree;
  zAABox3D box;
  int n;
  register int i;

  n = argc > 1 ? atoi( argv[1] ) : N;
  zRandInit();
  zAABoxTree3DInit( &tree );
  for( i=0; i<n; i++ ){
    gen_aabox( &box );
    zAABoxTree3DAdd( &tree, &box );
  }
  zAABoxTree3DFPrint( stdout, &tree );
  output( &tree, 0 );

  gen_aabox( &box );
  colchk_aabox( &tree, &box );

  zAABoxTree3DDestroy( &tree );
  return 0;
}

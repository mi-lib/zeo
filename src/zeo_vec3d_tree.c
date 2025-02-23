/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree (kd-tree with k=3).
 */

#include <zeo/zeo_vec3d_data.h>

/* initialize node data of a 3D vector tree. */
zVec3DTreeData *zVec3DTreeDataInit(zVec3DTreeData *data)
{
  data->id = -1; /* invalid identifier */
  data->split = zAxisInvalid; /* invalid split axis */
  zVec3DCreate( &data->vmin,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  zVec3DCreate( &data->vmax, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  return data;
}

zTreeClassMethod( zVec3DTree, zVec3DTreeData, zVec3DTreeDataInit, NULL )

/* create a leaf of a 3D vector tree. */
static zVec3DTree *_zVec3DTreeCreateLeaf(zAxis split, const zVec3D *point, int id)
{
  zVec3DTree *leaf;

  if( !( leaf = zAlloc( zVec3DTree, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  leaf->size = 1;
  leaf->data.id = id;
  leaf->data.split = split;
  zVec3DCopy( point, &leaf->data.v );
  leaf->child[0] = leaf->child[1] = NULL;
  return leaf;
}

/* return an index of a node which contains a given 3D vector. */
static int _zVec3DTreeChooseBranch(const zVec3DTree *node, const zVec3D *point)
{
  return point->e[(int)node->data.split] >= node->data.v.e[(int)node->data.split] ? 0 : 1;
}

/* add a new 3D vector to a tree. */
static zVec3DTree *_zVec3DTreeAddPoint(zVec3DTree *node, const zVec3D *point, int id)
{
  int b;
  zVec3DTree *leaf;

  node->size++;
  if( node->child[( b = _zVec3DTreeChooseBranch( node, point ) )] )
    return _zVec3DTreeAddPoint( node->child[b], point, id );
  if( !( leaf = _zVec3DTreeCreateLeaf( ( node->data.split + 1 ) % 3, point, id ) ) )
    return NULL;
  node->child[b] = leaf;
  zVec3DCopy( &node->data.vmin, &leaf->data.vmin );
  zVec3DCopy( &node->data.vmax, &leaf->data.vmax );
  if( b == 0 )
    leaf->data.vmin.e[(int)node->data.split] = node->data.v.e[(int)node->data.split];
  else /* b == 1 */
    leaf->data.vmax.e[(int)node->data.split] = node->data.v.e[(int)node->data.split];
  return leaf;
}

/* add a new 3D vector to a tree. */
zVec3DTree *zVec3DTreeAddPoint(zVec3DTree *tree, const zVec3D *point)
{
  if( tree->data.split == zAxisInvalid ){
    tree->size = 1;
    tree->data.id = 0;
    tree->data.split = zX;
    zVec3DCopy( point, &tree->data.v );
    return tree;
  }
  return _zVec3DTreeAddPoint( tree, point, tree->size );
}

/* find the partition in which a 3D vector is contained (for debug). */
const zVec3DTree *zVec3DTreePart(const zVec3DTree *node, const zVec3D *point)
{
  int b;

  if( node->child[( b = _zVec3DTreeChooseBranch( node, point ) )] )
    return zVec3DTreePart( node->child[b], point );
  return node;
}

/* nearest neighbor search */

/* check if a sphere is overlapped with a bounding box of a node. */
static bool _zVec3DTreeIsOverlap(const zVec3DTree *node, const zVec3D *point, double r)
{
  int i;
  double d;

  for( d=0, i=zX; i<=zZ; i++ ){
    if( point->e[i] < node->data.vmin.e[i] )
      d += zSqr( point->e[i] - node->data.vmin.e[i] );
    if( point->e[i] > node->data.vmax.e[i] )
      d += zSqr( point->e[i] - node->data.vmax.e[i] );
  }
  return d <= r*r + zTOL ? true : false;
}

/* test if a node is the current nearest neighbor to a 3D vector. */
static void _zVec3DTreeNNTest(const zVec3DTree *node, const zVec3D *point, zVec3DTree **nn, double *dmin)
{
  double d;

  if( ( d = zVec3DDist( &node->data.v, point ) ) < *dmin ){
    *nn = (zVec3DTree *)node;
    *dmin = d;
  }
}

/* an internal recursive call of the nearest neighbor search; check the opposite side of branch. */
static double _zVec3DTreeNNOpp(const zVec3DTree *node, const zVec3D *point, zVec3DTree **nn, double *dmin)
{
  _zVec3DTreeNNTest( node, point, nn, dmin );
  if( node->child[0] && _zVec3DTreeIsOverlap( node->child[0], point, *dmin ) )
    _zVec3DTreeNNOpp( node->child[0], point, nn, dmin );
  if( node->child[1] && _zVec3DTreeIsOverlap( node->child[1], point, *dmin ) )
    _zVec3DTreeNNOpp( node->child[1], point, nn, dmin );
  return *dmin;
}

/* an internal recursive call of the nearest neighbor search. */
static double _zVec3DTreeNN(const zVec3DTree *node, const zVec3D *point, zVec3DTree **nn, double *dmin)
{
  int b;
  zVec3DTree *ob; /* opposite branch */

  if( node->child[( b = _zVec3DTreeChooseBranch( node, point ) )] )
    _zVec3DTreeNN( node->child[b], point, nn, dmin );
  _zVec3DTreeNNTest( node, point, nn, dmin );
  ob = node->child[1-b];
  if( ob && _zVec3DTreeIsOverlap( ob, point, *dmin ) )
    _zVec3DTreeNNOpp( ob, point, nn, dmin );
  return *dmin;
}

/* find the nearest neighbor to a 3D vector in a tree. */
double zVec3DTreeNN(const zVec3DTree *tree, const zVec3D *point, zVec3DTree **nn)
{
  double dmin = HUGE_VAL;

  *nn = NULL;
  return _zVec3DTreeNN( tree, point, nn, &dmin );
}

/* test if a node is in the vicinity of a 3D vector. */
static bool _zVec3DTreeVicinityTest(const zVec3DTree *node, const zVec3D *p, double radius, zVec3DData *vicinity)
{
  if( zVec3DDist( &node->data.v, p ) < radius ){
    if( !zVec3DDataAdd( vicinity, &node->data.v ) ) return false;
  }
  return true;
}

/* check the opposite side of the branch to find vicinity of a 3D point in a 3D vector tree. */
static zVec3DData *_zVec3DTreeVicinityOpp(zVec3DTree *node, const zVec3D *point, double radius, zVec3DData *vicinity)
{
  if( !_zVec3DTreeVicinityTest( node, point, radius, vicinity ) ) return NULL;
  if( node->child[0] && _zVec3DTreeIsOverlap( node->child[0], point, radius ) )
    if( !_zVec3DTreeVicinityOpp( node->child[0], point, radius, vicinity ) ) return NULL;
  if( node->child[1] && _zVec3DTreeIsOverlap( node->child[1], point, radius ) )
    if( !_zVec3DTreeVicinityOpp( node->child[1], point, radius, vicinity ) ) return NULL;
  return vicinity;
}

/* find vicinity of a 3D point in a 3D vector tree. */
zVec3DData *zVec3DTreeVicinity(const zVec3DTree *tree, const zVec3D *point, double radius, zVec3DData *vicinity)
{
  int b;
  zVec3DTree *ob; /* opposite branch */

  if( tree->child[( b = _zVec3DTreeChooseBranch( tree, point ) )] )
    if( !zVec3DTreeVicinity( tree->child[b], point, radius, vicinity ) ) return NULL;
  if( !_zVec3DTreeVicinityTest( tree, point, radius, vicinity ) ) return NULL;
  ob = tree->child[1-b];
  if( ob && _zVec3DTreeIsOverlap( ob, point, radius ) )
    if( !_zVec3DTreeVicinityOpp( ob, point, radius, vicinity ) ) return NULL;
  return vicinity;
}

/* convert an array of 3D vectors to a 3D vector tree. */
zVec3DTree *zVec3DArrayToTree(const zVec3DArray *array, zVec3DTree *tree)
{
  int i;

  zVec3DTreeInit( tree );
  for( i=0; i<zArraySize(array); i++ )
    if( !zVec3DTreeAddPoint( tree, zArrayElem(array,i) ) ) return NULL;
  return tree;
}

/* recursively convert a 3D vector node to a cell of an array of 3D vectors. */
static zVec3DArray *_zVec3DTreeToArray(const zVec3DTree *tree, zVec3DArray *array)
{
  bool ret0, ret1;

  ret0 = ret1 = true;
  if( !zArraySetElem( array, tree->data.id, &tree->data.v ) ) return NULL;
  if( tree->child[0] )
    ret0 = _zVec3DTreeToArray( tree->child[0], array ) ? true : false;
  if( tree->child[1] )
    ret1 = _zVec3DTreeToArray( tree->child[1], array ) ? true : false;
  return ret0 && ret1 ? array : NULL;
}

/* convert a 3D vector tree to an array of 3D vectors. */
zVec3DArray *zVec3DTreeToArray(const zVec3DTree *tree, zVec3DArray *array)
{
  zArrayAlloc( array, zVec3D, tree->size );
  return _zVec3DTreeToArray( tree, array );
}

/* recursively convert a 3D vector node to a 3D vector list. */
static bool _zVec3DTreeNodeToList(const zVec3DTree *tree, zVec3DList *list)
{
  if( !zVec3DListAdd( list, &tree->data.v ) ) return false;
  if( tree->child[0] )
    if( !_zVec3DTreeNodeToList( tree->child[0], list ) ) return false;
  if( tree->child[1] )
    if( !_zVec3DTreeNodeToList( tree->child[1], list ) ) return false;
  return true;
}

/* convert a 3D vector tree to a 3D vector list. */
zVec3DList *zVec3DTreeToList(const zVec3DTree *tree, zVec3DList *list)
{
  zListInit( list );
  if( !_zVec3DTreeNodeToList( tree, list ) ) return NULL;
  return list;
}

/* convert a 3D vector list to a 3D vector tree. */
zVec3DTree *zVec3DListToTree(const zVec3DList *list, zVec3DTree *tree)
{
  zVec3DListCell *vc;

  zVec3DTreeInit( tree );
  zListForEach( list, vc ){
    if( !zVec3DTreeAddPoint( tree, &vc->data ) ) return NULL;
  }
  return tree;
}

/* convert a set of 3D vectors to a 3D vector tree. */
zVec3DTree *zVec3DTreeAddData(zVec3DTree *tree, zVec3DData *data)
{
  zVec3D *v;

  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    if( !zVec3DTreeAddPoint( tree, v ) ) return NULL;
  }
  return tree;
}

/* recursively convert a 3D vector node to a set of 3D vectors. */
static zVec3DData *_zVec3DTreeToData(const zVec3DTree *tree, zVec3DData *data)
{
  bool ret0, ret1;

  ret0 = ret1 = true;
  if( !zVec3DDataAdd( data, &tree->data.v ) ) return NULL;
  if( tree->child[0] )
    ret0 = _zVec3DTreeToData( tree->child[0], data ) ? true : false;
  if( tree->child[1] )
    ret1 = _zVec3DTreeToData( tree->child[1], data ) ? true : false;
  return ret0 && ret1 ? data : NULL;
}

/* convert a 3D vector tree to a set of 3D vectors. */
zVec3DData *zVec3DTreeToData(const zVec3DTree *tree, zVec3DData *data)
{
  return _zVec3DTreeToData( tree, data );
}

/* print out a 3D vector tree (for debug). */
static void _zVec3DTreeFPrint(FILE *fp, const zVec3DTree *tree, int level)
{
  zFIndent( fp, level );
  fprintf( fp, "(%d) #%d ", tree->size, tree->data.id );
  zVec3DFPrint( fp, &tree->data.v );
  if( tree->child[0] )
    _zVec3DTreeFPrint( fp, tree->child[0], level+2 );
  if( tree->child[1] )
    _zVec3DTreeFPrint( fp, tree->child[1], level+2 );
}
void zVec3DTreeFPrint(FILE *fp, const zVec3DTree *tree)
{
  _zVec3DTreeFPrint( fp, tree, 0 );
}

/* print out values of a 3D vector tree. */
void zVec3DTreeValueFPrint(FILE *fp, const zVec3DTree *tree)
{
  zVec3DValueNLFPrint( fp, &tree->data.v );
  if( tree->child[0] )
    zVec3DTreeValueFPrint( fp, tree->child[0] );
  if( tree->child[1] )
    zVec3DTreeValueFPrint( fp, tree->child[1] );
}

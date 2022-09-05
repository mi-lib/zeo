/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree.
 */

#include <zeo/zeo_vec3d.h>

/* initialize node data of a 3D vector tree. */
zVec3DTreeData *zVec3DTreeDataInit(zVec3DTreeData *data)
{
  data->id = -1; /* invalid identifier */
  data->split = -1; /* invalid split axis */
  zVec3DCreate( &data->vmin,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  zVec3DCreate( &data->vmax, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  return data;
}

zTreeClassMethod( zVec3DTree, zVec3DTreeData, zVec3DTreeDataInit, NULL )

/* create a leaf of a 3D vector tree. */
static zVec3DTree *_zVec3DTreeCreateLeaf(zAxis split, zVec3D *v, int id)
{
  zVec3DTree *leaf;

  if( !( leaf = zAlloc( zVec3DTree, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  leaf->size = 1;
  leaf->data.id = id;
  leaf->data.split = split;
  zVec3DCopy( v, &leaf->data.v );
  leaf->child[0] = leaf->child[1] = NULL;
  return leaf;
}

/* return an index of a node which contains a given 3D vector. */
static int _zVec3DTreeChooseBranch(zVec3DTree *node, zVec3D *v)
{
  return v->e[node->data.split] >= node->data.v.e[node->data.split] ? 0 : 1;
}

/* add a new 3D vector to a tree. */
static zVec3DTree *_zVec3DTreeAdd(zVec3DTree *node, zVec3D *v, int id)
{
  int b;
  zVec3DTree *leaf;

  node->size++;
  if( node->child[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    return _zVec3DTreeAdd( node->child[b], v, id );
  if( !( leaf = _zVec3DTreeCreateLeaf( ( node->data.split + 1 ) % 3, v, id ) ) )
    return NULL;
  node->child[b] = leaf;
  zVec3DCopy( &node->data.vmin, &leaf->data.vmin );
  zVec3DCopy( &node->data.vmax, &leaf->data.vmax );
  if( b == 0 )
    leaf->data.vmin.e[node->data.split] = node->data.v.e[node->data.split];
  else /* b == 1 */
    leaf->data.vmax.e[node->data.split] = node->data.v.e[node->data.split];
  return leaf;
}

/* add a new 3D vector to a tree with an identifier. */
zVec3DTree *zVec3DTreeAddID(zVec3DTree *tree, zVec3D *v, int id)
{
  if( tree->data.split == -1 ){
    tree->size = 1;
    tree->data.id = id;
    tree->data.split = zX;
    zVec3DCopy( v, &tree->data.v );
    return tree;
  }
  return _zVec3DTreeAdd( tree, v, id );
}

/* add a new 3D vector to a tree. */
zVec3DTree *zVec3DTreeAdd(zVec3DTree *tree, zVec3D *v)
{
  return zVec3DTreeAddID( tree, v, tree->size );
}

/* find the partition in which a 3D vector is contained (for debug). */
zVec3DTree *zVec3DTreePart(zVec3DTree *node, zVec3D *v)
{
  int b;

  if( node->child[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    return zVec3DTreePart( node->child[b], v );
  return node;
}

/* nearest neighbor search */

/* check if a sphere is overlapped with a bounding box of a node. */
static bool _zVec3DTreeIsOverlap(zVec3DTree *node, zVec3D *c, double r)
{
  int i;
  double d, vd;

  for( d=0, i=zX; i<=zZ; i++ ){
    vd = c->e[i];
    if( vd < node->data.vmin.e[i] )
      d += zSqr( vd - node->data.vmin.e[i] );
    if( vd > node->data.vmax.e[i] )
      d += zSqr( vd - node->data.vmax.e[i] );
  }
  return d <= r*r + zTOL ? true : false;
}

/* test if a node is the current nearest neighbor to a 3D vector. */
static void _zVec3DTreeNNTest(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  double d;

  if( ( d = zVec3DDist( &node->data.v, v ) ) < *dmin ){
    *nn = node;
    *dmin = d;
  }
}

/* an internal recursive call of the nearest neighbor search; check the opposite side of branch. */
static double _zVec3DTreeNNOpp(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  _zVec3DTreeNNTest( node, v, nn, dmin );
  if( node->child[0] && _zVec3DTreeIsOverlap( node->child[0], v, *dmin ) )
    _zVec3DTreeNNOpp( node->child[0], v, nn, dmin );
  if( node->child[1] && _zVec3DTreeIsOverlap( node->child[1], v, *dmin ) )
    _zVec3DTreeNNOpp( node->child[1], v, nn, dmin );
  return *dmin;
}

/* an internal recursive call of the nearest neighbor search. */
static double _zVec3DTreeNN(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  int b;
  zVec3DTree *ob; /* opposite branch */

  if( node->child[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    _zVec3DTreeNN( node->child[b], v, nn, dmin );
  _zVec3DTreeNNTest( node, v, nn, dmin );
  ob = node->child[1-b];
  if( ob && _zVec3DTreeIsOverlap( ob, v, *dmin ) )
    _zVec3DTreeNNOpp( ob, v, nn, dmin );
  return *dmin;
}

/* find the nearest neighbor to a 3D vector in a tree. */
double zVec3DTreeNN(zVec3DTree *tree, zVec3D *v, zVec3DTree **nn)
{
  double dmin = HUGE_VAL;

  *nn = NULL;
  return _zVec3DTreeNN( tree, v, nn, &dmin );
}

/* convert an array of 3D vectors to a 3D vector tree. */
zVec3DTree *zVec3DArray2Tree(zVec3DArray *array, zVec3DTree *tree)
{
  uint i;

  zVec3DTreeInit( tree );
  for( i=0; i<zArraySize(array); i++ )
    if( !zVec3DTreeAdd( tree, zArrayElem(array,i) ) ) return NULL;
  return tree;
}

/* recursively convert a 3D vector node to a cell of an array of 3D vectors. */
static zVec3DArray *_zVec3DTree2Array(zVec3DTree *tree, zVec3DArray *array)
{
  bool ret0, ret1;

  ret0 = ret1 = true;
  if( !zArraySetElem( array, (uint)tree->data.id, &tree->data.v ) ) return NULL;
  if( tree->child[0] )
    ret0 = _zVec3DTree2Array( tree->child[0], array ) ? true : false;
  if( tree->child[1] )
    ret1 = _zVec3DTree2Array( tree->child[1], array ) ? true : false;
  return ret0 && ret1 ? array : NULL;
}

/* convert a 3D vector tree to an array of 3D vectors. */
zVec3DArray *zVec3DTree2Array(zVec3DTree *tree, zVec3DArray *array)
{
  zArrayAlloc( array, zVec3D, tree->size );
  return _zVec3DTree2Array( tree, array );
}

/* recursively convert a 3D vector node to a 3D vector list. */
static bool _zVec3DTreeNode2List(zVec3DTree *tree, zVec3DList *list)
{
  if( !zVec3DListAdd( list, &tree->data.v ) ) return false;
  if( tree->child[0] )
    if( !_zVec3DTreeNode2List( tree->child[0], list ) ) return false;
  if( tree->child[1] )
    if( !_zVec3DTreeNode2List( tree->child[1], list ) ) return false;
  return true;
}

/* convert a 3D vector tree to a 3D vector list. */
zVec3DList *zVec3DTree2List(zVec3DTree *tree, zVec3DList *list)
{
  zListInit( list );
  if( !_zVec3DTreeNode2List( tree, list ) ) return NULL;
  return list;
}

/* convert a 3D vector list to a 3D vector tree. */
zVec3DTree *zVec3DList2Tree(zVec3DList *list, zVec3DTree *tree)
{
  zVec3DListCell *vc;

  zVec3DTreeInit( tree );
  zListForEach( list, vc ){
    if( !zVec3DTreeAdd( tree, vc->data ) ) return NULL;
  }
  return tree;
}

/* print out a 3D vector tree (for debug). */
static void _zVec3DTreeFPrint(FILE *fp, zVec3DTree *tree, int level)
{
  zFIndent( fp, level );
  fprintf( fp, "(%d) #%d ", tree->size, tree->data.id );
  zVec3DFPrint( fp, &tree->data.v );
  if( tree->child[0] )
    _zVec3DTreeFPrint( fp, tree->child[0], level+2 );
  if( tree->child[1] )
    _zVec3DTreeFPrint( fp, tree->child[1], level+2 );
}
void zVec3DTreeFPrint(FILE *fp, zVec3DTree *tree)
{
  _zVec3DTreeFPrint( fp, tree, 0 );
}

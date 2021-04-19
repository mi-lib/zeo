/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree.
 */

#include <zeo/zeo_vec3d.h>

/* initialize a 3D vector tree. */
zVec3DTree *zVec3DTreeInit(zVec3DTree *tree)
{
  tree->split = -1; /* invalid split axis */
  tree->s[0] = tree->s[1] = NULL;
  zVec3DCreate( &tree->vmin,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  zVec3DCreate( &tree->vmax, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  return tree;
}

/* destroy a 3D vector tree. */
void zVec3DTreeDestroy(zVec3DTree *tree)
{
  if( tree->s[0] ){
    zVec3DTreeDestroy( tree->s[0] );
    free( tree->s[0] );
  }
  if( tree->s[1] ){
    zVec3DTreeDestroy( tree->s[1] );
    free( tree->s[1] );
  }
}

/* create a leaf of a 3D vector tree. */
static zVec3DTree *_zVec3DTreeCreateLeaf(zAxis split, zVec3D *v)
{
  zVec3DTree *leaf;

  if( !( leaf = zAlloc( zVec3DTree, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  leaf->split = split;
  zVec3DCopy( v, &leaf->v );
  leaf->s[0] = leaf->s[1] = NULL;
  return leaf;
}

/* return an index of a node which contains a given 3D vector. */
static int _zVec3DTreeChooseBranch(zVec3DTree *node, zVec3D *v)
{
  return v->e[node->split] >= node->v.e[node->split] ? 0 : 1;
}

/* add a new 3D vector to a tree. */
static zVec3DTree *_zVec3DTreeAdd(zVec3DTree *node, zVec3D *v)
{
  int b;
  zVec3DTree *leaf;

  if( node->s[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    return _zVec3DTreeAdd( node->s[b], v );
  if( !( leaf = _zVec3DTreeCreateLeaf( ( node->split + 1 ) % 3, v ) ) )
    return NULL;
  node->s[b] = leaf;
  zVec3DCopy( &node->vmin, &leaf->vmin );
  zVec3DCopy( &node->vmax, &leaf->vmax );
  if( b == 0 )
    leaf->vmin.e[node->split] = node->v.e[node->split];
  else /* b == 1 */
    leaf->vmax.e[node->split] = node->v.e[node->split];
  return leaf;
}

/* add a new 3D vector to a tree. */
zVec3DTree *zVec3DTreeAdd(zVec3DTree *tree, zVec3D *v)
{
  if( tree->split == -1 ){
    tree->split = zX;
    zVec3DCopy( v, &tree->v );
    return tree;
  }
  return _zVec3DTreeAdd( tree, v );
}

/* find the partition in which a 3D vector is contained (for debug). */
zVec3DTree *zVec3DTreePart(zVec3DTree *node, zVec3D *v)
{
  int b;

  if( node->s[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    return zVec3DTreePart( node->s[b], v );
  return node;
}

/* nearest neighbor search */

/* check if a sphere is overlapped with a bounding box of a node. */
static bool _zVec3DTreeIsOverlap(zVec3DTree *node, zVec3D *c, double r)
{
  register int i;
  double d, vd;

  for( d=0, i=zX; i<=zZ; i++ ){
    vd = c->e[i];
    if( vd < node->vmin.e[i] )
      d += zSqr( vd - node->vmin.e[i] );
    if( vd > node->vmax.e[i] )
      d += zSqr( vd - node->vmax.e[i] );
  }
  return d <= r*r+zTOL ? true : false;
}

/* test if a node is the current nearest neighbor to a 3D vector. */
static void _zVec3DTreeNNTest(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  double d;

  if( ( d = zVec3DDist( &node->v, v ) ) < *dmin ){
    *nn = node;
    *dmin = d;
  }
}

/* an internal recursive call of the nearest neighbor search; check the opposite side of branch. */
static double _zVec3DTreeNNOpp(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  _zVec3DTreeNNTest( node, v, nn, dmin );
  if( node->s[0] && _zVec3DTreeIsOverlap( node->s[0], v, *dmin ) )
    _zVec3DTreeNNOpp( node->s[0], v, nn, dmin );
  if( node->s[1] && _zVec3DTreeIsOverlap( node->s[1], v, *dmin ) )
    _zVec3DTreeNNOpp( node->s[1], v, nn, dmin );
  return *dmin;
}

/* an internal recursive call of the nearest neighbor search. */
static double _zVec3DTreeNN(zVec3DTree *node, zVec3D *v, zVec3DTree **nn, double *dmin)
{
  int b;
  zVec3DTree *ob; /* opposite branch */

  if( node->s[( b = _zVec3DTreeChooseBranch( node, v ) )] )
    _zVec3DTreeNN( node->s[b], v, nn, dmin );
  _zVec3DTreeNNTest( node, v, nn, dmin );
  ob = node->s[1-b];
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

/* recursively convert a 3D vector node to a 3D vector list. */
static bool _zVec3DTreeNode2List(zVec3DTree *tree, zVec3DList *list)
{
  if( !zVec3DListAdd( list, &tree->v ) ) return false;
  if( tree->s[0] )
    if( !_zVec3DTreeNode2List( tree->s[0], list ) ) return false;
  if( tree->s[1] )
    if( !_zVec3DTreeNode2List( tree->s[1], list ) ) return false;
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

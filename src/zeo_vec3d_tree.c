/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_tree - 3D vector tree.
 */

#include <zeo/zeo_vec3d.h>

static zVecTree3D *_zVecTree3DCreateLeaf(zAxis split, zVec3D *v);
static int _zVecTree3DChooseBranch(zVecTree3D *node, zVec3D *v);
static zVecTree3D *_zVecTree3DAdd(zVecTree3D *node, zVec3D *v);

static bool _zVecTree3DIsOverlap(zVecTree3D *node, zVec3D *c, double r);
static void _zVecTree3DNNTest(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin);
static double _zVecTree3DNN(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin);
static double _zVecTree3DNNOpp(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin);

/* zVecTree3DInit
 * - initialize a 3D vector tree.
 */
zVecTree3D *zVecTree3DInit(zVecTree3D *tree)
{
  tree->split = -1; /* invalid split axis */
  tree->s[0] = tree->s[1] = NULL;
  zVec3DCreate( &tree->vmin,-HUGE_VAL,-HUGE_VAL,-HUGE_VAL );
  zVec3DCreate( &tree->vmax, HUGE_VAL, HUGE_VAL, HUGE_VAL );
  return tree;
}

/* zVecTree3DDestroy
 * - destroy a 3D vector tree.
 */
void zVecTree3DDestroy(zVecTree3D *tree)
{
  if( tree->s[0] ){
    zVecTree3DDestroy( tree->s[0] );
    free( tree->s[0] );
  }
  if( tree->s[1] ){
    zVecTree3DDestroy( tree->s[1] );
    free( tree->s[1] );
  }
}

/* (static)
 * _zVecTree3DCreateLeaf
 * - create a leaf of a 3D vector tree.
 */
zVecTree3D *_zVecTree3DCreateLeaf(zAxis split, zVec3D *v)
{
  zVecTree3D *leaf;

  if( !( leaf = zAlloc( zVecTree3D, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  leaf->split = split;
  zVec3DCopy( v, &leaf->v );
  leaf->s[0] = leaf->s[1] = NULL;
  return leaf;
}

/* (static)
 * _zVecTree3DChooseBranch
 * - return an index of a node which contains a given 3D vector.
 */
int _zVecTree3DChooseBranch(zVecTree3D *node, zVec3D *v)
{
  return v->e[node->split] >= node->v.e[node->split] ? 0 : 1;
}

/* (static)
 * _zVecTree3DAdd
 * - add a new 3D vector to a tree.
 */
zVecTree3D *_zVecTree3DAdd(zVecTree3D *node, zVec3D *v)
{
  int b;
  zVecTree3D *leaf;

  if( node->s[( b = _zVecTree3DChooseBranch( node, v ) )] )
    return _zVecTree3DAdd( node->s[b], v );
  if( !( leaf = _zVecTree3DCreateLeaf( ( node->split + 1 ) % 3, v ) ) )
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

/* zVecTree3DAdd
 * - add a new 3D vector to a tree.
 */
zVecTree3D *zVecTree3DAdd(zVecTree3D *tree, zVec3D *v)
{
  if( tree->split == -1 ){
    tree->split = zX;
    zVec3DCopy( v, &tree->v );
    return tree;
  }
  return _zVecTree3DAdd( tree, v );
}

/* zVecTree3DPart
 * - find the partition in which a 3D vector is contained (for debug).
 */
zVecTree3D *zVecTree3DPart(zVecTree3D *node, zVec3D *v)
{
  int b;

  if( node->s[( b = _zVecTree3DChooseBranch( node, v ) )] )
    return zVecTree3DPart( node->s[b], v );
  return node;
}

/* nearest neighbor search */

/* (static)
 * _zVecTree3DIsOverlap
 * - check if a sphere is overlapped with a bounding box of a node.
 */
bool _zVecTree3DIsOverlap(zVecTree3D *node, zVec3D *c, double r)
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

/* (static)
 * _zVecTree3DNNTest
 * - test if a node is the current nearest neighbor to a 3D vector.
 */
void _zVecTree3DNNTest(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin)
{
  double d;

  if( ( d = zVec3DDist( &node->v, v ) ) < *dmin ){
    *nn = node;
    *dmin = d;
  }
}

/* (static)
 * _zVecTree3DNN
 * - an internal recursive call of the nearest neighbor search.
 */
double _zVecTree3DNN(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin)
{
  int b;
  zVecTree3D *ob; /* opposite branch */

  if( node->s[( b = _zVecTree3DChooseBranch( node, v ) )] )
    _zVecTree3DNN( node->s[b], v, nn, dmin );
  _zVecTree3DNNTest( node, v, nn, dmin );
  ob = node->s[1-b];
  if( ob && _zVecTree3DIsOverlap( ob, v, *dmin ) )
    _zVecTree3DNNOpp( ob, v, nn, dmin );
  return *dmin;
}

/* (static)
 * _zVecTree3DNNOpp
 * - an internal recursive call of the nearest neighbor search;
 *   check the opposite side of branch.
 */
double _zVecTree3DNNOpp(zVecTree3D *node, zVec3D *v, zVecTree3D **nn, double *dmin)
{
  _zVecTree3DNNTest( node, v, nn, dmin );
  if( node->s[0] && _zVecTree3DIsOverlap( node->s[0], v, *dmin ) )
    _zVecTree3DNNOpp( node->s[0], v, nn, dmin );
  if( node->s[1] && _zVecTree3DIsOverlap( node->s[1], v, *dmin ) )
    _zVecTree3DNNOpp( node->s[1], v, nn, dmin );
  return *dmin;
}

/* zVecTree3DNN
 * - find the nearest neighbor to a 3D vector in a tree.
 */
double zVecTree3DNN(zVecTree3D *tree, zVec3D *v, zVecTree3D **nn)
{
  double dmin = HUGE_VAL;

  *nn = NULL;
  return _zVecTree3DNN( tree, v, nn, &dmin );
}

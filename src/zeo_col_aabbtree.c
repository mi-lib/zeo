/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_aabbtree - collision checking: AABB tree.
 */

#include <zeo/zeo_col.h>

static bool _zAABoxTree3DIsLeaf(zAABoxTree3D *node);
static zAABoxTree3D *_zAABoxTree3DNodeCreate(zAABox3D *box);
static void _zAABoxTree3DSetBranch(zAABoxTree3D *node, zAABoxTree3D *b1, zAABoxTree3D *b2);

static bool _zAABoxTree3DAdd(zAABoxTree3D *tree, zAABoxTree3D *parent, int bidx, zAABox3D *box);
static void _zAABoxTree3DFWrite(FILE *fp, zAABoxTree3D *tree, int indent);

/* zAABoxTree3DInit
 * - initialize an AABB tree.
 */
void zAABoxTree3DInit(zAABoxTree3D *node)
{
  zAABox3DInit( &node->box );
  node->bp[0] = node->bp[1] = NULL;
}

/* zAABoxTree3DDestroy
 * - destroy an AABB tree.
 */
void zAABoxTree3DDestroy(zAABoxTree3D *tree)
{
  if( tree->bp[0] ){
    zAABoxTree3DDestroy( tree->bp[0] );
    zFree( tree->bp[0] );
  }
  if( tree->bp[1] ){
    zAABoxTree3DDestroy( tree->bp[1] );
    zFree( tree->bp[1] );
  }
}

/* (static)
 * _zAABoxTree3DIsLeaf
 * - check if the given node of an AABB tree is a leaf.
 */
bool _zAABoxTree3DIsLeaf(zAABoxTree3D *node)
{
  return node->bp[0] == NULL && node->bp[1] == NULL ? true : false;
}

/* (static)
 * _zAABoxTree3DNodeCreate
 * - create a node of an AABB tree from a given axis-aligned box.
 */
zAABoxTree3D *_zAABoxTree3DNodeCreate(zAABox3D *box)
{
  zAABoxTree3D *node;

  if( ( node = zAlloc( zAABoxTree3D, 1 ) ) == NULL ){
    ZALLOCERROR();
    return NULL;
  }
  zAABox3DCopy( box, &node->box );
  node->bp[0] = node->bp[1] = NULL;
  return node;
}

/* (static)
 * _zAABoxTree3DSetBranch
 * - set branches of a node of an AABB tree for given two nodes.
 */
void _zAABoxTree3DSetBranch(zAABoxTree3D *node, zAABoxTree3D *b1, zAABoxTree3D *b2)
{
  node->bp[0] = b1;
  node->bp[1] = b2;
  zAABox3DMerge( &node->box, &b1->box, &b2->box );
}

/* (static)
 * _zAABoxTree3DAdd
 * - add an axis-aligned box to an AABB tree (internal operation).
 */
bool _zAABoxTree3DAdd(zAABoxTree3D *tree, zAABoxTree3D *parent, int bidx, zAABox3D *box)
{
  zAABoxTree3D *node;
  zAABox3D box_tmp0, box_tmp1;

  if( tree == NULL ){
    parent->bp[0] = _zAABoxTree3DNodeCreate( box );
    return true;
  }
  if( _zAABoxTree3DIsLeaf( tree ) || !zColChkAABox3D( &tree->box, box ) ){
    if( ( node = zAlloc( zAABoxTree3D, 1 ) ) == NULL ){
      ZALLOCERROR();
      return false;
    }
    parent->bp[bidx] = node;
    if( ( node = _zAABoxTree3DNodeCreate( box ) ) == NULL )
      return false;
    _zAABoxTree3DSetBranch( parent->bp[bidx], tree, node );
    return true;
  }

  if( tree->bp[0] == NULL || tree->bp[1] == NULL ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return false;
  }
  zAABox3DMerge( &box_tmp0, &tree->bp[0]->box, box );
  zAABox3DMerge( &box_tmp1, &tree->bp[1]->box, box );
  if( zAABox3DVolume( &box_tmp0 ) <= zAABox3DVolume( &box_tmp1 ) ){
    _zAABoxTree3DAdd( tree->bp[0], tree, 0, box );
  } else{
    _zAABoxTree3DAdd( tree->bp[1], tree, 1, box );
  }
  zAABox3DMerge( &tree->box, &tree->bp[0]->box, &tree->bp[1]->box );
  return true;
}

/* zAABoxTree3DAdd
 * - add an axis-aligned box to an AABB tree.
 */
bool zAABoxTree3DAdd(zAABoxTree3D *tree, zAABox3D *box)
{
  return _zAABoxTree3DAdd( tree->bp[0], tree, 0, box );
}

/* (static)
 * _zAABoxTree3DFWrite
 * - output an AABB tree to a file (internal operation).
 */
void _zAABoxTree3DFWrite(FILE *fp, zAABoxTree3D *tree, int indent)
{
  zIndentF( fp, indent );
  fprintf( fp, "min: " ); zVec3DFWrite( fp, &tree->box.pmin );
  zIndentF( fp, indent );
  fprintf( fp, "max: " ); zVec3DFWrite( fp, &tree->box.pmax );
  if( tree->bp[0] ){
    zIndentF( fp, indent+2 );
    fprintf( fp, "[branch0]\n" );
    _zAABoxTree3DFWrite( fp, tree->bp[0], indent+2 );
  }
  if( tree->bp[1] ){
    zIndentF( fp, indent+2 );
    fprintf( fp, "[branch1]\n" );
    _zAABoxTree3DFWrite( fp, tree->bp[1], indent+2 );
  }
}

/* zAABoxTree3DFWrite
 * - output an AABB tree to a file.
 */
void zAABoxTree3DFWrite(FILE *fp, zAABoxTree3D *tree)
{
  _zAABoxTree3DFWrite( fp, tree, 0 );
}

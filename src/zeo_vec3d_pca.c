/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_pca - principal component analysis for 3D vectors.
 */

#include <zeo/zeo_mat3d.h>

/* zVec3DBarycenterPL
 * - barycenter of a point cloud given by a list.
 */
zVec3D *zVec3DBarycenterPL(zVec3DList *pl, zVec3D *c)
{
  zVec3DListCell *pc;

  zVec3DClear( c );
  zListForEach( pl, pc )
    zVec3DAddDRC( c, pc->data );
  return zVec3DDivDRC( c, zListNum(pl) );
}

/* zVec3DBarycenter
 * - barycenter of a point cloud given by an array.
 */
zVec3D *zVec3DBarycenter(zVec3D p[], int num, zVec3D *c)
{
  register int i;

  zVec3DClear( c );
  for( i=0; i<num; i++ )
    zVec3DAddDRC( c, &p[i] );
  return zVec3DDivDRC( c, num );
}

/* zVec3DPCA_PL
 * - PCA against vector cloud given by a list.
 */
zVec3D *zVec3DPCA_PL(zVec3DList *pl, zVec3D evec[])
{
  zMat3D vm;
  double eval[3];
  zVec3DListCell *pc;

  zMat3DClear( &vm );
  zListForEach( pl, pc )
    zMat3DAddDyad( &vm, pc->data, pc->data );
  zMat3DSymEig( &vm, eval, evec );
  return evec;
}

/* zVec3DPCA
 * - PCA against vector cloud given by an array.
 */
zVec3D *zVec3DPCA(zVec3D p[], int num, zVec3D evec[])
{
  zMat3D vm;
  double eval[3];
  register int i;

  zMat3DClear( &vm );
  for( i=0; i<num; i++ )
    zMat3DAddDyad( &vm, &p[i], &p[i] );
  zMat3DSymEig( &vm, eval, evec );
  return evec;
}

/* zVec3DBaryPCA_PL
 * - barycenter of and PCA against vector cloud given by a list.
 */
zVec3D *zVec3DBaryPCA_PL(zVec3DList *pl, zVec3D *c, zVec3D evec[])
{
  zMat3D vm;
  double eval[3];
  zVec3DListCell *pc;
  zVec3D dp;

  zVec3DBarycenterPL( pl, c );
  zMat3DClear( &vm );
  zListForEach( pl, pc ){
    zVec3DSub( pc->data, c, &dp );
    zMat3DAddDyad( &vm, &dp, &dp );
  }
  zMat3DSymEig( &vm, eval, evec );
  return c;
}

/* zVec3DBaryPCA
 * - barycenter of and PCA against vector cloud given by an array.
 */
zVec3D *zVec3DBaryPCA(zVec3D p[], int num, zVec3D *c, zVec3D evec[])
{
  zMat3D vm;
  double eval[3];
  zVec3D dp;
  register int i;

  zVec3DBarycenter( p, num, c );
  zMat3DClear( &vm );
  for( i=0; i<num; i++ ){
    zVec3DSub( &p[i], c, &dp );
    zMat3DAddDyad( &vm, &dp, &dp );
  }
  zMat3DSymEig( &vm, eval, evec );
  return c;
}

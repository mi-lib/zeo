/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_col_minkowski - collision checking: Minkowski sum
 */

#include <zeo/zeo_col.h>

/* Minkowski subtract. */
zPH3D *zPH3DMinkowskiSub(zPH3D *ph, zVec3D p1[], int n1, zVec3D p2[], int n2)
{
  zVec3D *v;
  register int i, j;

  if( ( v = zAlloc( zVec3D, n1*n2 ) ) == NULL ){
    ZALLOCERROR();
    return NULL;
  }
  for( i=0; i<n1; i++ )
    for( j=0; j<n2; j++ )
      zVec3DSub( &p1[i], &p2[j], &v[n2*i+j] );
  zCH3D( ph, v, n1*n2 );
  return ph;
}

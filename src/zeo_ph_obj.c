/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_obj - polyhedron I/O on OBJ format
 */

#include <zeo/zeo_ph.h>

static int _zOBJCountKey(FILE *fp, const char *key)
{
  int len, n = 0;
  char buf[BUFSIZ];

  len = strlen( key );
  rewind( fp );
  while( fgets( buf, BUFSIZ, fp ) ){
    if( strncmp( buf, key, len ) == 0 ) n++;
  }
  return n;
}

static zPH3D *_zOBJFReadPH3D(FILE *fp, zPH3D *ph)
{
  char buf[BUFSIZ], tkn[BUFSIZ], *sp;
  int vi, fi, i[3], j;

  vi = fi = 0;
  rewind( fp );
  while( fgets( buf, BUFSIZ, fp ) ){
    if( strncmp( buf, "v ", 2 ) == 0 ){ /* vertex */
      sp = buf + 2;
      for( j=0; j<3; j++ ){
        sp = zSTokenSkim( sp, tkn, BUFSIZ );
        zPH3DVert(ph,vi)->e[j] = atof( tkn );
      }
      if( ++vi > zPH3DVertNum(ph) ){
        ZRUNERROR( "fatal error" );
        ph = NULL;
        break;
      }
    } else
    if( strncmp( buf, "f ", 2 ) == 0 ){ /* face */
      sp = buf + 2;
      for( j=0; j<3; j++ ){
        sp = zSTokenSkim( sp, tkn, BUFSIZ );
        *strchr( tkn, '/' ) = '\0';
        i[j] = atoi( tkn ) - 1; /* indices of vertices start from 1 in OBJ format. */
        /* normal and texture coordinates are ignored */
      }
      zTri3DCreate( zPH3DFace(ph,fi), zPH3DVert(ph,i[0]), zPH3DVert(ph,i[1]), zPH3DVert(ph,i[2]) );
      if( ++fi > zPH3DFaceNum(ph) ){
        ZRUNERROR( "fatal error" );
        ph = NULL;
        break;
      }
    }
    /* only vertices and faces are picked up. */
  }
  return ph;
}

/* read a 3D polyhedron from OBJ format */
zPH3D *zPH3DFReadOBJ(FILE *fp, zPH3D *ph)
{
  int nv, nf;

  nv = _zOBJCountKey( fp, "v " );
  nf = _zOBJCountKey( fp, "f " );
  zPH3DAlloc( ph, nv, nf );
  _zOBJFReadPH3D( fp, ph );
  return ph;
}

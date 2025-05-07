/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph3d_obj - polyhedron I/O on OBJ format.
 */

#include <zeo/zeo_ph3d.h>

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
  char buf[BUFSIZ], tkn[BUFSIZ], *sp, *slashmark;
  int vi, fi, fvi[3], j;

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
      if( zeo_ph_echo_while_reading ){
        eprintf( "\r%d/%d", vi, zPH3DVertNum(ph) );
        if( vi == zPH3DVertNum(ph) ) eprintf( "\n" );
      }
    } else
    if( strncmp( buf, "f ", 2 ) == 0 ){ /* face */
      sp = buf + 2;
      for( j=0; j<3; j++ ){
        sp = zSTokenSkim( sp, tkn, BUFSIZ );
        if( ( slashmark = strchr( tkn, '/' ) ) ) *slashmark = '\0';
        fvi[j] = atoi( tkn ) - 1; /* indices of vertices start from 1 in OBJ format. */
        /* normal and texture coordinates are ignored */
      }
      zTri3DCreate( zPH3DFace(ph,fi), zPH3DVert(ph,fvi[0]), zPH3DVert(ph,fvi[1]), zPH3DVert(ph,fvi[2]) );
      if( ++fi > zPH3DFaceNum(ph) ){
        ZRUNERROR( "fatal error" );
        ph = NULL;
        break;
      }
      if( zeo_ph_echo_while_reading ){
        eprintf( "\r%d/%d", fi, zPH3DFaceNum(ph) );
        if( fi == zPH3DFaceNum(ph) ) eprintf( "\n" );
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

  if( ( nv = _zOBJCountKey( fp, "v " ) ) == 0 ){
    ZRUNWARN( ZEO_ERR_NOVERT );
  }
  if( ( nf = _zOBJCountKey( fp, "f " ) ) == 0 ){
    ZRUNWARN( ZEO_ERR_NOFACE );
  }
  zPH3DAlloc( ph, nv, nf );
  _zOBJFReadPH3D( fp, ph );
  return ph;
}

/* read a 3D polyhedron from OBJ format */
zPH3D *zPH3DReadFileOBJ(zPH3D *ph, const char *filename)
{
  FILE *fp;

  if( !( fp = fopen( filename, "rt" ) ) ){
    ZOPENERROR( filename );
    return NULL;
  }
  ph = zPH3DFReadOBJ( fp, ph );
  fclose( fp );
  return ph;
}

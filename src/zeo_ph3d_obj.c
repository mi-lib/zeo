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

static zPH3D *_zOBJFReadVert(char *buf, zPH3D *ph, int *vi)
{
  char tkn[BUFSIZ];
  int j;

  for( j=0; j<3; j++ ){
    buf = zSTokenSkim( buf, tkn, BUFSIZ );
    zPH3DVert(ph,*vi)->e[j] = atof( tkn );
  }
  if( ++*vi > zPH3DVertNum(ph) ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return NULL;
  }
  return ph;
}

static int _zOBJFReadFaceIndex(char *tkn, int *fvi, int *fti, int *fni)
{
  char *slash1, *slash2, *fts, *fns;
  int retval = 0;

  if( !( slash1 = strchr( tkn, '/' ) ) ){ /* only vert index */
    fts = fns = NULL;
    retval = 1;
  } else{
    *slash1 = '\0';
    fts = slash1 + 1;
    if( !( slash2 = strrchr( tkn, '/' ) ) ){ /* vert/texture coord indices */
      fns = NULL;
      retval = 2;
    } else{ /* vert/texture coord/normal indices */
      *slash2 = '\0';
      fns = slash2 + 1;
      retval = 3;
    }
  }
   /* indices start from 1 in OBJ format. */
  *fvi = *tkn ? atoi( tkn ) - 1 : 0; /* vertex */
  *fti = fts  ? atoi( fts ) - 1 : 0; /* texture */
  *fni = fns  ? atoi( fns ) - 1 : 0; /* normal vector */
  return retval;
}

static zPH3D *_zOBJFReadFace(char *buf, zPH3D *ph, int *fi)
{
  char tkn[BUFSIZ];
  int j, fvi[3], fti, fni;

  for( j=0; j<3; j++ ){
    buf = zSTokenSkim( buf, tkn, BUFSIZ );
    _zOBJFReadFaceIndex( tkn, &fvi[j], &fti, &fni );
    /* normal and texture coordinates are discarded */
  }
  zTri3DCreate( zPH3DFace(ph,*fi), zPH3DVert(ph,fvi[0]), zPH3DVert(ph,fvi[1]), zPH3DVert(ph,fvi[2]) );
  if( ++*fi > zPH3DFaceNum(ph) ){
    ZRUNERROR( ZEO_ERR_FATAL );
    return NULL;
  }
  return ph;
}

static zPH3D *_zOBJFReadPH3D(FILE *fp, zPH3D *ph)
{
  char buf[BUFSIZ];
  int vi, fi;

  vi = fi = 0;
  rewind( fp );
  while( fgets( buf, BUFSIZ, fp ) ){
    if( strncmp( buf, "v ", 2 ) == 0 ){ /* vertex */
      if( !( ph = _zOBJFReadVert( buf+2, ph, &vi ) ) ) break;
      if( zeo_ph_echo_while_reading ){
        eprintf( "\r%d/%d", vi, zPH3DVertNum(ph) );
        if( vi == zPH3DVertNum(ph) ) eprintf( "\n" );
      }
    } else
    if( strncmp( buf, "f ", 2 ) == 0 ){ /* face */
      if( !( ph = _zOBJFReadFace( buf+2, ph, &fi ) ) ) break;
      if( zeo_ph_echo_while_reading ){
        eprintf( "\r%d/%d", fi, zPH3DFaceNum(ph) );
        if( fi == zPH3DFaceNum(ph) ) eprintf( "\n" );
      }
    } else{
      /* only vertices and faces are evaluated. */
    }
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

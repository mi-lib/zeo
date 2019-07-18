/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_stl - polyhedron I/O on STL format
 */

#include <zeo/zeo_ph.h>

/* STL vertex list */
typedef struct{
  int id;
  zVec3D v;
} _zSTLVert;
zListClass( _zSTLVertList, _zSTLVertListCell, _zSTLVert );

static void _zSTLVertListInit(_zSTLVertList *list);
static _zSTLVertListCell *_zSTLVertListReg(_zSTLVertList *list, zVec3D *v);
static zVec3DArray *_zSTLVertListToVec3DArray(_zSTLVertList *list, zVec3DArray *array);

/* initialize a STL vertex list. */
void _zSTLVertListInit(_zSTLVertList *list)
{
  zListInit( list );
  zListRoot(list)->data.id = -1;
}

/* register a vector to a STL vertex list. */
_zSTLVertListCell *_zSTLVertListReg(_zSTLVertList *list, zVec3D *v)
{
  _zSTLVertListCell *cp;

  zListForEach( list, cp )
    if( zVec3DEqual( &cp->data.v, v ) ) return cp;
  if( !( cp = zAlloc( _zSTLVertListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.id = zListHead(list)->data.id + 1;
  zVec3DCopy( v, &cp->data.v );
  zListInsertHead( list, cp );
  return cp;
}

/* convert a STL vertex list to an array of vertices. */
zVec3DArray *_zSTLVertListToVec3DArray(_zSTLVertList *list, zVec3DArray *array)
{
  _zSTLVertListCell *cp;
  register int i = 0;

  zArrayAlloc( array, zVec3D, zListNum(list) );
  if( zArraySize(array) < zListNum(list) ){
    zArrayFree( array );
    return NULL;
  }
  zListForEach( list, cp )
    zVec3DCopy( &cp->data.v, zArrayElemNC(array,i++) );
  return array;
}

/* STL facet list */
typedef struct{
  _zSTLVert *v1, *v2, *v3;
  zVec3D normal;
} _zSTLFacet;
zListClass( _zSTLFacetList, _zSTLFacetListCell, _zSTLFacet );

static _zSTLFacetListCell *_zSTLFacetListReg(_zSTLFacetList *flist, _zSTLVertList *vlist, zVec3D *normal, zVec3D v[]);
static zTri3DArray *_zSTLFacetListToTri3DArray(_zSTLFacetList *list, zVec3DArray *varray, zTri3DArray *array);

/* register a facet to a STL facet list. */
_zSTLFacetListCell *_zSTLFacetListReg(_zSTLFacetList *flist, _zSTLVertList *vlist, zVec3D *normal, zVec3D v[])
{
  zTri3D facet;
  _zSTLVertListCell *vc1, *vc2, *vc3;
  _zSTLFacetListCell *cp;

  if( !( vc1 = _zSTLVertListReg( vlist, &v[0] ) ) ||
      !( vc2 = _zSTLVertListReg( vlist, &v[1] ) ) ||
      !( vc3 = _zSTLVertListReg( vlist, &v[2] ) ) ) return NULL;
  if( !( cp = zAlloc( _zSTLFacetListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.v1 = &vc1->data;
  cp->data.v2 = &vc2->data;
  cp->data.v3 = &vc3->data;
  zTri3DCreate( &facet, &vc1->data.v, &vc2->data.v, &vc3->data.v );
  if( !zVec3DEqual( zTri3DNorm(&facet), normal ) ){
    ZRUNWARN( ZEO_WARN_STL_WRONGNORMAL );
  }
  zVec3DCopy( zTri3DNorm(&facet), &cp->data.normal );
  zListInsertHead( flist, cp );
  return cp;
}

/* convert a STL facet list to an array of faces. */
zTri3DArray *_zSTLFacetListToTri3DArray(_zSTLFacetList *list, zVec3DArray *varray, zTri3DArray *array)
{
  _zSTLFacetListCell *cp;
  register int i = 0;

  zArrayAlloc( array, zTri3D, zListNum(list) );
  if( zArraySize(array) < zListNum(list) ){
    zArrayFree( array );
    return NULL;
  }
  zListForEach( list, cp ){
    zTri3DSetVert( zArrayElemNC(array,i), 0, zArrayElemNC(varray,cp->data.v1->id) );
    zTri3DSetVert( zArrayElemNC(array,i), 1, zArrayElemNC(varray,cp->data.v2->id) );
    zTri3DSetVert( zArrayElemNC(array,i), 2, zArrayElemNC(varray,cp->data.v3->id) );
    zTri3DSetNorm( zArrayElemNC(array,i), &cp->data.normal );
    i++;
  }
  return array;
}

/* read/write in STL format */

static bool _zPH3DFReadSTL_ASCIIloop(FILE *fp, char buf[], zVec3D v[]);
static zPH3D *_zPH3DFReadSTL_ASCIIfacet(FILE *fp, char buf[], zPH3D *ph, _zSTLVertList *vert_list, _zSTLFacetList *facet_list);
static zPH3D *_zPH3DFReadSTL_ASCIIsolid(FILE *fp, zPH3D *ph, char name[], size_t namesize);

static zVec3D *_zPH3DFReadSTL_BinVec(FILE *fp, zVec3D *v);
static void _zPH3DFWriteSTL_BinVec(FILE *fp, zVec3D *v);

/* scan an outer loop of a face of a 3D polyhedron from ASCII STL format */
bool _zPH3DFReadSTL_ASCIIloop(FILE *fp, char buf[], zVec3D v[])
{
  int i = 0;

  if( !zFToken( fp, buf, BUFSIZ ) ){
    ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
    return false;
  }
  if( strcmp( buf, "loop" ) != 0 ){
    ZRUNWARN( ZEO_WARN_STL_MIS_LOOP );
    return false;
  }
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      return false;
    }
    if( strcmp( buf, "vertex" ) == 0 ){
      if( i > 2 ){
        ZRUNWARN( ZEO_WARN_STL_TOOMANYVERT );
        continue;
      }
      zVec3DFScan( fp, &v[i++] );
    } else
    if( strcmp( buf, "endloop" ) == 0 ){
      if( i < 3 ){
        ZRUNWARN( ZEO_WARN_STL_TOOFEWVERT );
        break;
      }
      return true;
    }
  }
  return false;
}

/* scan a facet of a 3D polyhedron from ASCII STL format */
zPH3D *_zPH3DFReadSTL_ASCIIfacet(FILE *fp, char buf[], zPH3D *ph, _zSTLVertList *vert_list, _zSTLFacetList *facet_list)
{
  zVec3D normal, v[3];

  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      break;
    }
    if( strcmp( buf, "normal" ) == 0 ){
      zVec3DFScan( fp, &normal );
    } else
    if( strcmp( buf, "outer" ) == 0 ){
      _zPH3DFReadSTL_ASCIIloop( fp, buf, v );
    } else
    if( strcmp( buf, "endfacet" ) == 0 ){
      if( !_zSTLFacetListReg( facet_list, vert_list, &normal, v ) ) break;
      return ph;
    }
  }
  return NULL;
}

/* scan a solid object of a 3D polyhedron from ASCII STL format */
zPH3D *_zPH3DFReadSTL_ASCIIsolid(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[BUFSIZ];
  _zSTLVertList vert_list;
  _zSTLFacetList facet_list;

  _zSTLVertListInit( &vert_list );
  zListInit( &facet_list );
  if( !zFToken( fp, name, namesize ) ) return NULL; /* no name */
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      break;
    }
    if( strcmp( buf, "endsolid" ) == 0 ){
      if( !_zSTLVertListToVec3DArray( &vert_list, &ph->vert ) ||
          !_zSTLFacetListToTri3DArray( &facet_list, &ph->vert, &ph->face ) )
        ph = NULL;
      zListDestroy( _zSTLVertListCell, &vert_list );
      zListDestroy( _zSTLFacetListCell, &facet_list );
      return ph;
    } else
    if( strcmp( buf, "facet" ) != 0 ) continue; /* skip as a comment */
    if( !_zPH3DFReadSTL_ASCIIfacet( fp, buf, ph, &vert_list, &facet_list ) ) break; /* read a facet */
  }
  return NULL;
}

/* scan a 3D polyhedron from ASCII STL format */
zPH3D *zPH3DFReadSTL_ASCII(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[BUFSIZ];

  zPH3DInit( ph );
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ) break; /* early EOF */
    if( strcmp( buf, "solid" ) != 0 ) continue; /* skip as a comment */
    return _zPH3DFReadSTL_ASCIIsolid( fp, ph, name, namesize ); /* succeed to scan */
  }
  return NULL; /* fail to scan */
}

/* print a 3D polyhedron to ASCII STL format */
void zPH3DFWriteSTL_ASCII(FILE *fp, zPH3D *ph, char name[])
{
  register int i;

  fprintf( fp, "solid %s\n", name );
  for( i=0; i<zPH3DFaceNum(ph); i++ ){
    fprintf( fp, "\tfacet normal\t" );
    zVec3DDataFPrint( fp, zTri3DNorm(zPH3DFace(ph,i)) );
    fprintf( fp, "\n\t\touter loop" );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),0) );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),1) );
    fprintf( fp, "\n\t\t\tvertex\t" );
    zVec3DDataFPrint( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    fprintf( fp, "\n\t\tendloop\n\tendfacet\n" );
  }
  fprintf( fp, "endsolid\n" );
}

/* read a 3D vector from binary STL format */
zVec3D *_zPH3DFReadSTL_BinVec(FILE *fp, zVec3D *v)
{
  float val1, val2, val3;

  if( fread( &val1, sizeof(float), 1, fp ) < 1 );
  if( fread( &val2, sizeof(float), 1, fp ) < 1 );
  if( fread( &val3, sizeof(float), 1, fp ) < 1 );
  return zVec3DCreate( v, val1, val2, val3 );
}

/* write a 3D vector to binary STL format */
void _zPH3DFWriteSTL_BinVec(FILE *fp, zVec3D *v)
{
  float val1, val2, val3;

  val1 = v->e[0];
  val2 = v->e[1];
  val3 = v->e[2];
  if( fwrite( &val1, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fwrite( &val2, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fwrite( &val3, sizeof(float), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
}

#define ZEO_STL_HEADSIZ 80

/* read a 3D polyhedron from binary STL format */
zPH3D *zPH3DFReadSTL_Bin(FILE *fp, zPH3D *ph, char name[])
{
  uint32_t nf; /* number of facets */
  uint16_t dummy;
  zVec3D normal, v[3];
  _zSTLVertList vert_list;
  _zSTLFacetList facet_list;
  register int i;

  _zSTLVertListInit( &vert_list );
  zListInit( &facet_list );
  zPH3DInit( ph );
  if( fread( name, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ )
    ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fread( &nf, sizeof(uint32_t), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( nf <= 0 ) return NULL;
  for( i=0; i<nf; i++ ){
    _zPH3DFReadSTL_BinVec( fp, &normal );
    _zPH3DFReadSTL_BinVec( fp, &v[0] );
    _zPH3DFReadSTL_BinVec( fp, &v[1] );
    _zPH3DFReadSTL_BinVec( fp, &v[2] );
    if( fread( &dummy, sizeof(uint16_t), 1, fp ) < 1 )
      ZRUNWARN( ZEO_WARN_STL_MISSINGDATA ); /* two empty bytes */
    if( !_zSTLFacetListReg( &facet_list, &vert_list, &normal, v ) ) break;
  }
  if( !_zSTLVertListToVec3DArray( &vert_list, &ph->vert ) ||
      !_zSTLFacetListToTri3DArray( &facet_list, &ph->vert, &ph->face ) )
    ph = NULL;
  zListDestroy( _zSTLVertListCell, &vert_list );
  zListDestroy( _zSTLFacetListCell, &facet_list );
  return ph;
}

/* write a 3D polyhedron to binary STL format */
void zPH3DFWriteSTL_Bin(FILE *fp, zPH3D *ph, char name[])
{
  uint32_t nf;
  uint16_t dummy = 0;
  register int i;

  if( fwrite( name, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ );
  nf = zPH3DFaceNum(ph);
  if( fwrite( &nf, sizeof(uint32_t), 1, fp ) < 1 );
  for( i=0; i<nf; i++ ){
    _zPH3DFWriteSTL_BinVec( fp, zTri3DNorm(zPH3DFace(ph,i)) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),0) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),1) );
    _zPH3DFWriteSTL_BinVec( fp, zTri3DVert(zPH3DFace(ph,i),2) );
    if( fwrite( &dummy, sizeof(uint16_t), 1, fp ) < 1 ); /* two empty bytes */
  }
}

#define ZEO_ERR_STL_UNREADABLE "File unreadable. Probably not a STL file."

/* read a 3D polyhedron from either ASCII/binary STL format */
zPH3D *zPH3DFReadSTL(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[6];
  long int pos;

  pos = ftell( fp );
  if( fread( buf, sizeof(char), 6, fp ) < 6 ){
    ZRUNERROR( ZEO_ERR_STL_UNREADABLE );
    return NULL;
  }
  fseek( fp, pos, SEEK_SET );
  return strncmp( buf, "solid ", 6 ) == 0 ?
    zPH3DFReadSTL_ASCII( fp, ph, name, namesize ) :
    zPH3DFReadSTL_Bin( fp, ph, name );
}

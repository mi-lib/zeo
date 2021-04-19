/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_ph_stl - polyhedron I/O on STL format
 */

#include <zeo/zeo_ph.h>

/* register a vector to a STL vertex tree. */
static zVec3DTree *_zSTLVertReg(zVec3DTree *tree, zVec3D *v)
{
  zVec3DTree *node;

  zVec3DTreeNN( tree, v, &node );
  if( zVec3DEqual( v, &node->v ) ) return node;
  return zVec3DTreeAdd( tree, v );
}

/* STL facet list */
typedef struct{
  zVec3DTree *v1, *v2, *v3;
  zVec3D normal;
} _zSTLFacet;
zListClass( _zSTLFacetList, _zSTLFacetListCell, _zSTLFacet );

/* register a facet to a STL facet list. */
static _zSTLFacetListCell *_zSTLFacetListReg(_zSTLFacetList *flist, zVec3DTree *vtree, zVec3D *normal, zVec3D v[])
{
  zTri3D facet;
  zVec3DTree *vc1, *vc2, *vc3;
  _zSTLFacetListCell *cp;

  if( !( vc1 = _zSTLVertReg( vtree, &v[0] ) ) ||
      !( vc2 = _zSTLVertReg( vtree, &v[1] ) ) ||
      !( vc3 = _zSTLVertReg( vtree, &v[2] ) ) ) return NULL;
  if( !( cp = zAlloc( _zSTLFacetListCell, 1 ) ) ){
    ZALLOCERROR();
    return NULL;
  }
  cp->data.v1 = vc1;
  cp->data.v2 = vc2;
  cp->data.v3 = vc3;
  zTri3DCreate( &facet, &vc1->v, &vc2->v, &vc3->v );
  zVec3DCopy( zTri3DNorm(&facet), &cp->data.normal );
  zListInsertHead( flist, cp );
  return cp;
}

/* convert a STL facet list to an array of faces. */
static zTri3DArray *_zSTLFacetListToTri3DArray(_zSTLFacetList *list, zVec3DArray *varray, zTri3DArray *array)
{
  _zSTLFacetListCell *cp;
  register int i = 0;

  zArrayAlloc( array, zTri3D, zListSize(list) );
  if( zArraySize(array) < zListSize(list) ){
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

/* convert a vertex tree and a facet list to a polyhedron (and destroy the tree and the list). */
static zPH3D *_zSTLToPH3D(zVec3DTree *vert_tree, _zSTLFacetList *facet_list, zPH3D *ph)
{
  if( !zVec3DTree2Array( vert_tree, &ph->vert ) ||
      !_zSTLFacetListToTri3DArray( facet_list, &ph->vert, &ph->face ) )
    ph = NULL;
  zVec3DTreeDestroy( vert_tree );
  zListDestroy( _zSTLFacetListCell, facet_list );
  return ph;
}

/* read/write in STL format */

/* scan an outer loop of a face of a 3D polyhedron from ASCII STL format */
static bool _zPH3DFReadSTL_ASCIIloop(FILE *fp, char buf[], zVec3D v[])
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
static zPH3D *_zPH3DFReadSTL_ASCIIfacet(FILE *fp, char buf[], zPH3D *ph, zVec3DTree *vert_tree, _zSTLFacetList *facet_list)
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
      if( !_zSTLFacetListReg( facet_list, vert_tree, &normal, v ) ) break;
      return ph;
    }
  }
  return NULL;
}

/* scan a solid object of a 3D polyhedron from ASCII STL format */
static zPH3D *_zPH3DFReadSTL_ASCIIsolid(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  char buf[BUFSIZ];
  zVec3DTree vert_tree;
  _zSTLFacetList facet_list;

  zVec3DTreeInit( &vert_tree );
  zListInit( &facet_list );
  if( !zFToken( fp, name, namesize ) ) return NULL; /* no name */
  while( !feof( fp ) ){
    if( !zFToken( fp, buf, BUFSIZ ) ){
      ZRUNERROR( ZEO_ERR_STL_INCOMPLETE );
      break;
    }
    if( strcmp( buf, "endsolid" ) == 0 )
      return _zSTLToPH3D( &vert_tree, &facet_list, ph );
    if( strcmp( buf, "facet" ) != 0 ) continue; /* skip as a comment */
    if( !_zPH3DFReadSTL_ASCIIfacet( fp, buf, ph, &vert_tree, &facet_list ) ) break; /* read a facet */
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
static zVec3D *_zPH3DFReadSTL_BinVec(FILE *fp, zVec3D *v)
{
  float val1, val2, val3;

  if( fread( &val1, sizeof(float), 1, fp ) < 1 );
  if( fread( &val2, sizeof(float), 1, fp ) < 1 );
  if( fread( &val3, sizeof(float), 1, fp ) < 1 );
  return zVec3DCreate( v, val1, val2, val3 );
}

/* write a 3D vector to binary STL format */
static void _zPH3DFWriteSTL_BinVec(FILE *fp, zVec3D *v)
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

/* check if STL format is binary. */
bool zSTLIsBin(FILE *fp)
{
  size_t filesize;
  uint32_t nf;
  char buf[BUFSIZ];
  bool ret = false;

  filesize = zFileSize( fp );
  if( fread( buf, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ ) goto TERMINATE;
  if( fread( &nf, sizeof(uint32_t), 1, fp ) < 1 ) goto TERMINATE;
  ret = filesize == ZEO_STL_HEADSIZ + sizeof(uint32_t)
                  + nf * ( sizeof(float)*12 + sizeof(uint16_t) ) ? true : false;
 TERMINATE:
  rewind( fp );
  return ret;
}

/* read a 3D polyhedron from binary STL format */
zPH3D *zPH3DFReadSTL_Bin(FILE *fp, zPH3D *ph, char name[])
{
  uint32_t nf; /* number of facets */
  uint16_t dummy;
  zVec3D normal, v[3];
  zVec3DTree vert_tree;
  _zSTLFacetList facet_list;
  register int i;

  zVec3DTreeInit( &vert_tree );
  zListInit( &facet_list );
  zPH3DInit( ph );
  if( fread( name, sizeof(char), ZEO_STL_HEADSIZ, fp ) < ZEO_STL_HEADSIZ )
    ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( fread( &nf, sizeof(uint32_t), 1, fp ) < 1 ) ZRUNWARN( ZEO_WARN_STL_MISSINGDATA );
  if( nf <= 0 ) return NULL;
  for( i=0; i<nf; i++ ){
    eprintf( "\r%d/%d", i, nf );
    _zPH3DFReadSTL_BinVec( fp, &normal );
    _zPH3DFReadSTL_BinVec( fp, &v[0] );
    _zPH3DFReadSTL_BinVec( fp, &v[1] );
    _zPH3DFReadSTL_BinVec( fp, &v[2] );
    if( fread( &dummy, sizeof(uint16_t), 1, fp ) < 1 )
      ZRUNWARN( ZEO_WARN_STL_MISSINGDATA ); /* two empty bytes */
    if( !_zSTLFacetListReg( &facet_list, &vert_tree, &normal, v ) ) break;
  }
  eprintf( "\n" );
  return _zSTLToPH3D( &vert_tree, &facet_list, ph );
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

/* read a 3D polyhedron from either ASCII/binary STL format */
zPH3D *zPH3DFReadSTL(FILE *fp, zPH3D *ph, char name[], size_t namesize)
{
  return zSTLIsBin( fp ) ?
    zPH3DFReadSTL_Bin( fp, ph, name ) :
    zPH3DFReadSTL_ASCII( fp, ph, name, namesize );
}

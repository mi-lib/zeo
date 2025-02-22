/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vecxd_list - list of 2D/3D vectors.
 */

#ifndef __ZEO_VECXD_LIST_H__
#define __ZEO_VECXD_LIST_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* CLASS: zVec2D/3DList
 * list of 2D/3D vectors
 * ********************************************************** */

#define ZEO_VECXD_LIST_DEF_STRUCT(XD) \
  zListClass( zVec##XD##List, zVec##XD##ListCell, zVec##XD )

/* allocate a 2D/3D vector list cell and copy a vector. */
#define ZEO_VECXD_LIST_NEW_PROTOTYPE(XD) \
  zVec##XD##ListCell *zVec##XD##ListNew(const zVec##XD *v)
#define ZEO_VECXD_LIST_NEW(XD) \
  ZEO_VECXD_LIST_NEW_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cell; \
    if( ( cell = zAlloc( zVec##XD##ListCell, 1 ) ) ){ \
      zVec##XD##Copy( v, &cell->data ); \
      return cell; \
    } \
    ZALLOCERROR(); \
    return NULL; \
  }

/* add a 2D/3D vector to a vector list. */
#define ZEO_VECXD_LIST_ADD_PROTOTYPE(XD) \
  zVec##XD##ListCell *zVec##XD##ListAdd(zVec##XD##List *list, const zVec##XD *v)
#define ZEO_VECXD_LIST_ADD(XD) \
  ZEO_VECXD_LIST_ADD_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cell; \
    if( ( cell = zVec##XD##ListNew( v ) ) ) \
      zListInsertHead( list, cell ); \
    return cell; \
  }

/* append an array of 2D/3D vectors to a list. */
#define ZEO_VECXD_LIST_APPEND_ARRAY_PROTOTYPE(XD) \
  zVec##XD##List *zVec##XD##ListAppendArray(zVec##XD##List *list, const zVec##XD##Array *array)
#define ZEO_VECXD_LIST_APPEND_ARRAY(XD) \
  ZEO_VECXD_LIST_APPEND_ARRAY_PROTOTYPE( XD ){ \
    int i; \
    for( i=0; i<zArraySize(array); i++ ) \
      if( !zVec##XD##ListAdd( list, zArrayElemNC(array,i) ) ) break; \
    return list; \
  }

/* clone an array of 2D/3D vectors. */
#define ZEO_VECXD_LIST_CLONE_PROTOTYPE(XD) \
  zVec##XD##List *zVec##XD##ListClone(const zVec##XD##List *src, zVec##XD##List *dest)
#define ZEO_VECXD_LIST_CLONE(XD) \
  ZEO_VECXD_LIST_CLONE_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cp; \
    zListInit( dest ); \
    zListForEach( src, cp ){ \
      if( !zVec##XD##ListAdd( dest, &cp->data ) ){ \
        zVec##XD##ListDestroy( dest ); \
        return NULL; \
      } \
    } \
    return dest; \
  }

/* converts an array of 2D/3D vectors to a list. */
#define ZEO_VECXD_LIST_ARRAY_TO_LIST_PROTOTYPE(XD) \
  zVec##XD##List *zVec##XD##ArrayToList(const zVec##XD##Array *array, zVec##XD##List *list)
#define ZEO_VECXD_LIST_ARRAY_TO_LIST(XD) \
  ZEO_VECXD_LIST_ARRAY_TO_LIST_PROTOTYPE( XD ){ \
    zListInit( list ); \
    return zVec##XD##ListAppendArray( list, array ); \
  }

/* destroy a 2D/3D vector list. */
#define zVecXDListDestroy(XD,list) zListDestroy( zVec##XD##ListCell, list )

/* find an identical 3D vector in a list. */
#define ZEO_VECXD_LIST_FIND_PROTOTYPE(XD) \
  zVec##XD##ListCell *zVec##XD##ListFind(const zVec##XD##List *list, const zVec##XD *v)
#define ZEO_VECXD_LIST_FIND(XD) \
  ZEO_VECXD_LIST_FIND_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cp; \
    zListForEach( list, cp ) \
      if( zVec##XD##Equal( &cp->data, v ) ) return cp; \
    return NULL; \
  }

/* a quick sort routine for 2D/3D vector list class. */
#define ZEO_VECXD_LIST_QUICKSORT_PROTOTYPE(XD) \
  zVec##XD##List *zVec##XD##ListQuickSort(zVec##XD##List *list, int (*cmp)(void*,void*,void*), void *util)
#define ZEO_VECXD_LIST_QUICKSORT(XD) zListQuickSortDef( zVec##XD##List, zVec##XD##ListCell )

#define ZEO_VECXD_LIST_QUICKSORT_DEFAULT_PROTOTYPE(XD) \
  zVec##XD##List *zVec##XD##ListQuickSortDefault(zVec##XD##List *list)
#define ZEO_VECXD_LIST_QUICKSORT_DEFAULT(XD) \
  ZEO_VECXD_LIST_QUICKSORT_DEFAULT_PROTOTYPE(XD){\
    return zVec##XD##ListQuickSort( list, _zVec##XD##ListQuickSortDefaultCmp, NULL );\
  }

/* print a list of 2D/3D vectors. */
#define ZEO_VECXD_LIST_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##ListFPrint(FILE *fp, const zVec##XD##List *list)
#define ZEO_VECXD_LIST_FPRINT(XD) \
  ZEO_VECXD_LIST_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cp; \
    fprintf( fp, "%d\n", zListSize(list) ); \
    zListForEach( list, cp ) \
      zVec##XD##FPrint( fp, &cp->data ); \
  }

#define zVecXDListPrint(XD,list) zVec##XD##ListFPrint( stdout, (list) )

/* print a list of 2D/3D vectors in a plain form. */
#define ZEO_VECXD_LIST_VALUE_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##ListValueFPrint(FILE *fp, const zVec##XD##List *list)
#define ZEO_VECXD_LIST_VALUE_FPRINT(XD) \
  ZEO_VECXD_LIST_VALUE_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cp; \
    zListForEach( list, cp ) \
      zVec##XD##ValueNLFPrint( fp, &cp->data ); \
  }

#define zVecXDListValuePrint(XD,list) zVec##XD##ListValueFPrint( stdout, (list) )

/* ********************************************************** */
/* CLASS: zVec2D/3DAddrList
 * list of addresses of 2D/3D vectors
 * ********************************************************** */

#define ZEO_VECXD_ADDRLIST_DEF_STRUCT(XD) \
  zListClass( zVec##XD##AddrList, zVec##XD##AddrListCell, zVec##XD * )

/* add a pointer to a 2D/3D vector into a list of vectors. */
#define ZEO_VECXD_ADDRLIST_ADD_PROTOTYPE(XD) \
  zVec##XD##AddrListCell *zVec##XD##AddrListAdd(zVec##XD##AddrList *list, const zVec##XD *v)
#define ZEO_VECXD_ADDRLIST_ADD(XD) \
  ZEO_VECXD_ADDRLIST_ADD_PROTOTYPE( XD ){ \
    zVec##XD##AddrListCell *cell; \
    if( !( cell = zAlloc( zVec##XD##AddrListCell, 1 ) ) ){ \
      ZALLOCERROR(); \
      return NULL; \
    } \
    cell->data = (zVec##XD *)v; \
    zListInsertHead( list, cell ); \
    return cell; \
  }

/* append an array of 2D/3D vectors to a list of pointers to 3D vectors. */
#define ZEO_VECXD_ADDRLIST_APPEND_ARRAY_PROTOTYPE(XD) \
  zVec##XD##AddrList *zVec##XD##AddrListAppendArray(zVec##XD##AddrList *list, const zVec##XD##Array *array)
#define ZEO_VECXD_ADDRLIST_APPEND_ARRAY(XD) \
  ZEO_VECXD_ADDRLIST_APPEND_ARRAY_PROTOTYPE( XD ){ \
    int i; \
    for( i=0; i<zArraySize(array); i++ ) \
      if( !zVec##XD##AddrListAdd( list, zArrayElemNC(array,i) ) ) return NULL; \
    return list; \
  }

/* create a list of pointers to 2D/3D vectors from an array. */
#define ZEO_VECXD_ADDRLIST_CREATE_PROTOTYPE(XD) \
  zVec##XD##AddrList *zVec##XD##AddrListCreate(zVec##XD##AddrList *list, const zVec##XD##Array *array)
#define ZEO_VECXD_ADDRLIST_CREATE(XD) \
  ZEO_VECXD_ADDRLIST_CREATE_PROTOTYPE( XD ){ \
    zListInit( list ); \
    return zVec##XD##AddrListAppendArray( list, array ); \
  }

/* create a list of pointers to 2D/3D vectors from a list of 3D vectors. */
#define ZEO_VECXD_ADDRLIST_FROM_LIST_PROTOTYPE(XD) \
  zVec##XD##AddrList *zVec##XD##AddrListFromList(zVec##XD##AddrList *addrlist, const zVec##XD##List *list)
#define ZEO_VECXD_ADDRLIST_FROM_LIST(XD) \
  ZEO_VECXD_ADDRLIST_FROM_LIST_PROTOTYPE( XD ){ \
    zVec##XD##ListCell *cp; \
    zListInit( addrlist ); \
    zListForEach( list, cp ) \
      if( !zVec##XD##AddrListAdd( addrlist, &cp->data ) ) return NULL; \
    return addrlist; \
  }

/* clone a list of pointers to 2D/3D vectors. */
#define ZEO_VECXD_ADDRLIST_CLONE_PROTOTYPE(XD) \
  zVec##XD##AddrList *zVec##XD##AddrListClone(const zVec##XD##AddrList *src, zVec##XD##AddrList *dest)
#define ZEO_VECXD_ADDRLIST_CLONE(XD) \
  ZEO_VECXD_ADDRLIST_CLONE_PROTOTYPE( XD ){ \
    zVec##XD##AddrListCell *scp; \
    zListInit( dest ); \
    zListForEach( src, scp ){ \
      if( !zVec##XD##AddrListAdd( dest, scp->data ) ){ \
        zVec##XD##AddrListDestroy( dest ); \
        return NULL; \
      } \
    } \
    return dest; \
  }

/* destroy a list of pointers to 2D/3D vectors. */
#define zVecXDAddrListDestroy(XD,list) zListDestroy( zVec##XD##AddrListCell, list )

/* a quick sort routine for vector list class. */
#define ZEO_VECXD_ADDRLIST_QUICKSORT_PROTOTYPE(XD) \
  zVec##XD##AddrList *zVec##XD##AddrListQuickSort(zVec##XD##AddrList *list, int (*cmp)(void*,void*,void*), void *utilp)
#define ZEO_VECXD_ADDRLIST_QUICKSORT(XD) zListQuickSortDef( zVec##XD##AddrList, zVec##XD##AddrListCell )

/* print a list of pointers to 2D/3D vectors. */
#define ZEO_VECXD_ADDRLIST_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##AddrListFPrint(FILE *fp, const zVec##XD##AddrList *list)
#define ZEO_VECXD_ADDRLIST_FPRINT(XD) \
  ZEO_VECXD_ADDRLIST_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD##AddrListCell *cp; \
    fprintf( fp, "%d\n", zListSize(list) ); \
    zListForEach( list, cp ) \
      zVec##XD##FPrint( fp, cp->data ); \
  }

#define zVecXDAddrListPrint(XD,list) zVec##XD##AddrListFPrint( stdout, (list) )

/* print a list of pointers to 3D vectors in a plain form. */
#define ZEO_VECXD_ADDRLIST_VALUE_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##AddrListValueFPrint(FILE *fp, const zVec##XD##AddrList *list)
#define ZEO_VECXD_ADDRLIST_VALUE_FPRINT(XD) \
  ZEO_VECXD_ADDRLIST_VALUE_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD##AddrListCell *cp; \
    zListForEach( list, cp ) \
      zVec##XD##ValueNLFPrint( fp, cp->data ); \
  }

#define zVecXDAddrListValuePrint(XD,list) zVec##XD##AddrListValueFPrint( stdout, (list) )

__END_DECLS

#endif /* __ZEO_VECXD_LIST_H__ */

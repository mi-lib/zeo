/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vecxd_data - abstract class of a set of 2D/3D vectors.
 */

#ifndef __ZEO_VECXD_DATA_H__
#define __ZEO_VECXD_DATA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* ********************************************************** */
/* commonized class of a set of 2D/3D vectors.
 * ********************************************************** */

#define ZEO_VECXD_DATA_METHOD_DEF_STRUCT(XD) \
  ZDECL_STRUCT( zVec##XD##Data ); \
  ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec##XD##DataMethod ){ \
    void (* destroy)(zVec##XD##Data *); \
    int (* size)(zVec##XD##Data *); \
    bool (* is_empty)(zVec##XD##Data *); \
    bool (* add)(zVec##XD##Data *, const zVec##XD *); \
    void (* rewind)(zVec##XD##Data *); \
    zVec##XD *(* peek)(zVec##XD##Data *); \
    zVec##XD *(* fetch)(zVec##XD##Data *); \
  }

#define ZEO_VECXD_DATA_DEF_UNION_MEMBER(XD) \
  zVec##XD##Array *array; \
  zVec##XD##List *list; \
  zVec##XD##AddrList *addrlist; \
  const zVec##XD##Array *array_ref; \
  const zVec##XD##List *list_ref; \
  const zVec##XD##AddrList *addrlist_ref

#define ZEO_VECXD_DATA_DEF_POINTER_MEMBER(XD) \
  int i; \
  zVec##XD##ListCell *cp; \
  zVec##XD##AddrListCell *ap

#ifdef __cplusplus
#define ZEO_VECXD_DATA_DEF_STRUCT(XD) \
  ZEO_VECXD_DATA_METHOD_DEF_STRUCT( XD ); \
  ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec##XD##Data ){ \
    union __vec##XD##data { \
      ZEO_VECXD_DATA_DEF_UNION_MEMBER( XD ); \
      __vec##XD##data(){} \
    } data; \
    union __vec##XD##data_pointer{ \
      ZEO_VECXD_DATA_DEF_POINTER_MEMBER( XD ); \
      __vec##XD##data_pointer(){} \
    } pointer; \
    const zVec##XD##DataMethod *method; \
    zVec##XD##Data(){} \
  }
#else
#define ZEO_VECXD_DATA_DEF_STRUCT(XD) \
  ZEO_VECXD_DATA_METHOD_DEF_STRUCT( XD ); \
  ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec##XD##Data ){ \
    union{ \
      ZEO_VECXD_DATA_DEF_UNION_MEMBER( XD ); \
    } data; \
    union{ \
      ZEO_VECXD_DATA_DEF_POINTER_MEMBER( XD ); \
    } pointer; \
    const zVec##XD##DataMethod *method; \
  }
#endif

#define zVecXDDataDestroy(data)  (data)->method->destroy( data )
#define zVecXDDataSize(data)     (data)->method->size( data )
#define zVecXDDataIsEmpty(data)  (data)->method->is_empty( data )
#define zVecXDDataAdd(data,v)    (data)->method->add( data, v )
#define zVecXDDataRewind(data)   (data)->method->rewind( data )
#define zVecXDDataPeek(data)     (data)->method->peek( data )
#define zVecXDDataFetch(data)    (data)->method->fetch( data )

/* methods for individual dataset representations */

#define ZEO_VECXD_DATA_DEF_STATIC_METHOD(XD) \
  static void _zVec##XD##DataDummyDestroy(zVec##XD##Data *data){} \
  static int _zVec##XD##DataDummySize(zVec##XD##Data *data){ return 0; } \
  static bool _zVec##XD##DataDummyIsEmpty(zVec##XD##Data *data){ return true; } \
  static bool _zVec##XD##DataDummyAdd(zVec##XD##Data *data, const zVec##XD *vec){ return false; } \
  static void _zVec##XD##DataDummyRewind(zVec##XD##Data *data){} \
  static zVec##XD *_zVec##XD##DataDummyPeek(zVec##XD##Data *data){ return NULL; } \
  static zVec##XD *_zVec##XD##DataDummyFetch(zVec##XD##Data *data){ return NULL; } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_dummy = { \
    destroy:  _zVec##XD##DataDummyDestroy, \
    size:     _zVec##XD##DataDummySize, \
    is_empty: _zVec##XD##DataDummyIsEmpty, \
    add:      _zVec##XD##DataDummyAdd, \
    rewind:   _zVec##XD##DataDummyRewind, \
    peek:     _zVec##XD##DataDummyPeek, \
    fetch:    _zVec##XD##DataDummyFetch, \
  }; \
  \
  static void _zVec##XD##DataArrayDestroy(zVec##XD##Data *data){ \
    zVec##XD##ArrayFree( data->data.array ); \
    zFree( data->data.array ); \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataArraySize(zVec##XD##Data *data){ return zArraySize( data->data.array ); } \
  static bool _zVec##XD##DataArrayIsEmpty(zVec##XD##Data *data){ return zArraySize( data->data.array ) == 0; } \
  static bool _zVec##XD##DataArrayAdd(zVec##XD##Data *data, const zVec##XD *vec){ \
    if( data->pointer.i >= zArraySize(data->data.array) ) return false; \
    zVec##XD##Copy( vec, zArrayElemNC(data->data.array,data->pointer.i) ); \
    data->pointer.i++; \
    return true; \
  } \
  static void _zVec##XD##DataArrayRewind(zVec##XD##Data *data){ data->pointer.i = 0; } \
  static zVec##XD *_zVec##XD##DataArrayPeek(zVec##XD##Data *data){ \
    if( data->pointer.i >= zArraySize(data->data.array) ) return NULL; \
    return zArrayElemNC(data->data.array,data->pointer.i); \
  } \
  static zVec##XD *_zVec##XD##DataArrayFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataArrayPeek( data ); \
    data->pointer.i++; \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_array = { \
    destroy:  _zVec##XD##DataArrayDestroy, \
    size:     _zVec##XD##DataArraySize, \
    is_empty: _zVec##XD##DataArrayIsEmpty, \
    add:      _zVec##XD##DataArrayAdd, \
    rewind:   _zVec##XD##DataArrayRewind, \
    peek:     _zVec##XD##DataArrayPeek, \
    fetch:    _zVec##XD##DataArrayFetch, \
  }; \
  \
  static void _zVec##XD##DataArrayDirectDestroy(zVec##XD##Data *data){ \
    zArrayInit( data->data.array ); \
    zFree( data->data.array ); \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_array_direct = { \
    destroy:  _zVec##XD##DataArrayDirectDestroy, \
    size:     _zVec##XD##DataArraySize, \
    is_empty: _zVec##XD##DataArrayIsEmpty, \
    add:      _zVec##XD##DataDummyAdd, \
    rewind:   _zVec##XD##DataArrayRewind, \
    peek:     _zVec##XD##DataArrayPeek, \
    fetch:    _zVec##XD##DataArrayFetch, \
  }; \
  \
  static void _zVec##XD##DataListDestroy(zVec##XD##Data *data){ \
    zVec##XD##ListDestroy( data->data.list ); \
    zFree( data->data.list ); \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataListSize(zVec##XD##Data *data){ return zListSize( data->data.list ); } \
  static bool _zVec##XD##DataListIsEmpty(zVec##XD##Data *data){ return zListIsEmpty( data->data.list ); } \
  static bool _zVec##XD##DataListAdd(zVec##XD##Data *data, const zVec##XD *vec){ return zVec##XD##ListAdd( data->data.list, vec ) ? true: false; } \
  static void _zVec##XD##DataListRewind(zVec##XD##Data *data){ data->pointer.cp = zListTail(data->data.list); } \
  static zVec##XD *_zVec##XD##DataListPeek(zVec##XD##Data *data){ \
    if( data->pointer.cp == zListRoot(data->data.list) ) return NULL; \
    return &data->pointer.cp->data; \
  } \
  static zVec##XD *_zVec##XD##DataListFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataListPeek( data ); \
    data->pointer.cp = zListCellNext(data->pointer.cp); \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_list = { \
    destroy:  _zVec##XD##DataListDestroy, \
    size:     _zVec##XD##DataListSize, \
    is_empty: _zVec##XD##DataListIsEmpty, \
    add:      _zVec##XD##DataListAdd, \
    rewind:   _zVec##XD##DataListRewind, \
    peek:     _zVec##XD##DataListPeek, \
    fetch:    _zVec##XD##DataListFetch, \
  }; \
  \
  static void _zVec##XD##DataAddrListDestroy(zVec##XD##Data *data){ \
    zVec##XD##AddrListDestroy( data->data.addrlist ); \
    zFree( data->data.addrlist ); \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataAddrListSize(zVec##XD##Data *data){ return zListSize( data->data.addrlist ); } \
  static bool _zVec##XD##DataAddrListIsEmpty(zVec##XD##Data *data){ return zListIsEmpty( data->data.addrlist ); } \
  static bool _zVec##XD##DataAddrListAdd(zVec##XD##Data *data, const zVec##XD *vec){ return zVec##XD##AddrListAdd( data->data.addrlist, vec ) ? true: false; } \
  static void _zVec##XD##DataAddrListRewind(zVec##XD##Data *data){ data->pointer.ap = zListTail(data->data.addrlist); } \
  static zVec##XD *_zVec##XD##DataAddrListPeek(zVec##XD##Data *data){ \
    if( data->pointer.ap == zListRoot(data->data.addrlist) ) return NULL; \
    return data->pointer.ap->data; \
  } \
  static zVec##XD *_zVec##XD##DataAddrListFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataAddrListPeek( data ); \
    data->pointer.ap = zListCellNext(data->pointer.ap); \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_addrlist = { \
    destroy:  _zVec##XD##DataAddrListDestroy, \
    size:     _zVec##XD##DataAddrListSize, \
    is_empty: _zVec##XD##DataAddrListIsEmpty, \
    add:      _zVec##XD##DataAddrListAdd, \
    rewind:   _zVec##XD##DataAddrListRewind, \
    peek:     _zVec##XD##DataAddrListPeek, \
    fetch:    _zVec##XD##DataAddrListFetch, \
  }; \
  \
  static void _zVec##XD##DataArrayPtrDestroy(zVec##XD##Data *data){ \
    data->data.array_ref = NULL; \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataArrayPtrSize(zVec##XD##Data *data){ return zArraySize( data->data.array_ref ); } \
  static bool _zVec##XD##DataArrayPtrIsEmpty(zVec##XD##Data *data){ return zArraySize( data->data.array_ref ) == 0; } \
  static bool _zVec##XD##DataArrayPtrAdd(zVec##XD##Data *data, const zVec##XD *vec){ return false; } \
  static zVec##XD *_zVec##XD##DataArrayPtrPeek(zVec##XD##Data *data){ \
    if( data->pointer.i >= zArraySize(data->data.array_ref) ) return NULL; \
    return zArrayElemNC(data->data.array_ref,data->pointer.i); \
  } \
  static zVec##XD *_zVec##XD##DataArrayPtrFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataArrayPtrPeek( data ); \
    data->pointer.i++; \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_array_ptr = { \
    destroy:  _zVec##XD##DataArrayPtrDestroy, \
    size:     _zVec##XD##DataArrayPtrSize, \
    is_empty: _zVec##XD##DataArrayPtrIsEmpty, \
    add:      _zVec##XD##DataArrayPtrAdd, \
    rewind:   _zVec##XD##DataArrayRewind, \
    peek:     _zVec##XD##DataArrayPtrPeek, \
    fetch:    _zVec##XD##DataArrayPtrFetch, \
  }; \
  \
  static void _zVec##XD##DataListPtrDestroy(zVec##XD##Data *data){ \
    data->data.list_ref = NULL; \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataListPtrSize(zVec##XD##Data *data){ return zListSize( data->data.list_ref ); } \
  static bool _zVec##XD##DataListPtrIsEmpty(zVec##XD##Data *data){ return zListIsEmpty( data->data.list_ref ); } \
  static void _zVec##XD##DataListPtrRewind(zVec##XD##Data *data){ data->pointer.cp = zListTail(data->data.list_ref); } \
  static zVec##XD *_zVec##XD##DataListPtrPeek(zVec##XD##Data *data){ \
    if( data->pointer.cp == zListRoot(data->data.list_ref) ) return NULL; \
    return &data->pointer.cp->data; \
  } \
  static zVec##XD *_zVec##XD##DataListPtrFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataListPtrPeek( data ); \
    data->pointer.cp = zListCellNext(data->pointer.cp); \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_list_ptr = { \
    destroy:  _zVec##XD##DataListPtrDestroy, \
    size:     _zVec##XD##DataListPtrSize, \
    is_empty: _zVec##XD##DataListPtrIsEmpty, \
    add:      _zVec##XD##DataDummyAdd, \
    rewind:   _zVec##XD##DataListPtrRewind, \
    peek:     _zVec##XD##DataListPtrPeek, \
    fetch:    _zVec##XD##DataListPtrFetch, \
  }; \
  \
  static void _zVec##XD##DataAddrListPtrDestroy(zVec##XD##Data *data){ \
    data->data.addrlist_ref = NULL; \
    data->method = &zeo_vec##XD##_data_method_dummy; \
  } \
  static int  _zVec##XD##DataAddrListPtrSize(zVec##XD##Data *data){ return zListSize( data->data.addrlist_ref ); } \
  static bool _zVec##XD##DataAddrListPtrIsEmpty(zVec##XD##Data *data){ return zListIsEmpty( data->data.addrlist_ref ); } \
  static void _zVec##XD##DataAddrListPtrRewind(zVec##XD##Data *data){ data->pointer.ap = zListTail(data->data.addrlist_ref); } \
  static zVec##XD *_zVec##XD##DataAddrListPtrPeek(zVec##XD##Data *data){ \
    if( data->pointer.ap == zListRoot(data->data.addrlist_ref) ) return NULL; \
    return data->pointer.ap->data; \
  } \
  static zVec##XD *_zVec##XD##DataAddrListPtrFetch(zVec##XD##Data *data){ \
    zVec##XD *ret; \
    ret = _zVec##XD##DataAddrListPtrPeek( data ); \
    data->pointer.ap = zListCellNext(data->pointer.ap); \
    return ret; \
  } \
  static const zVec##XD##DataMethod zeo_vec##XD##_data_method_addrlist_ptr = { \
    destroy:  _zVec##XD##DataAddrListPtrDestroy, \
    size:     _zVec##XD##DataAddrListPtrSize, \
    is_empty: _zVec##XD##DataAddrListPtrIsEmpty, \
    add:      _zVec##XD##DataDummyAdd, \
    rewind:   _zVec##XD##DataAddrListPtrRewind, \
    peek:     _zVec##XD##DataAddrListPtrPeek, \
    fetch:    _zVec##XD##DataAddrListPtrFetch, \
  };

/* initialize 2D/3D vector data as an array. */
#define ZEO_VECXD_DATA_INIT_ARRAY_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataInitArray(zVec##XD##Data *data, int size)
#define ZEO_VECXD_DATA_INIT_ARRAY(XD) \
  ZEO_VECXD_DATA_INIT_ARRAY_PROTOTYPE( XD ){ \
    if( !( data->data.array = zAlloc( zVec##XD##Array, 1 ) ) ){ \
      ZALLOCERROR(); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    zVec##XD##ArrayAlloc( data->data.array, size ); \
    if( zArraySize(data->data.array) != size ){ \
      zFree( data->data.array ); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    data->method = &zeo_vec##XD##_data_method_array; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* initialize 2D/3D vector data as a list. */
#define ZEO_VECXD_DATA_INIT_LIST_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataInitList(zVec##XD##Data *data)
#define ZEO_VECXD_DATA_INIT_LIST(XD) \
  ZEO_VECXD_DATA_INIT_LIST_PROTOTYPE( XD ){ \
    if( !( data->data.list = zAlloc( zVec##XD##List, 1 ) ) ){ \
      ZALLOCERROR(); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    zListInit( data->data.list ); \
    data->method = &zeo_vec##XD##_data_method_list; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* initialize 2D/3D vector data as a list of addresses. */
#define ZEO_VECXD_DATA_INIT_ADDRLIST_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataInitAddrList(zVec##XD##Data *data)
#define ZEO_VECXD_DATA_INIT_ADDRLIST(XD) \
  ZEO_VECXD_DATA_INIT_ADDRLIST_PROTOTYPE( XD ){ \
    if( !( data->data.addrlist = zAlloc( zVec##XD##AddrList, 1 ) ) ){ \
      ZALLOCERROR(); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    zListInit( data->data.addrlist ); \
    data->method = &zeo_vec##XD##_data_method_addrlist; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* directly assign an array to 2D/3D vector data. */
#define ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataAssignArrayDirect(zVec##XD##Data *data, zVec##XD *vec, int size)
#define ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT(XD) \
  ZEO_VECXD_DATA_ASSIGN_ARRAY_DIRECT_PROTOTYPE( XD ){ \
    if( !vec || size <= 0 ){ \
      ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "vector array" ); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    if( !( data->data.array = zAlloc( zVec##XD##Array, 1 ) ) ){ \
      ZALLOCERROR(); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    zArrayAssign( data->data.array, vec, size ); \
    data->method = &zeo_vec##XD##_data_method_array_direct; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* assign an array to 2D/3D vector data. */
#define ZEO_VECXD_DATA_ASSIGN_ARRAY_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataAssignArray(zVec##XD##Data *data, const zVec##XD##Array *array)
#define ZEO_VECXD_DATA_ASSIGN_ARRAY(XD) \
  ZEO_VECXD_DATA_ASSIGN_ARRAY_PROTOTYPE( XD ){ \
    if( !( data->data.array_ref = array ) ){ \
      ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "array" ); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    data->method = &zeo_vec##XD##_data_method_array_ptr; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* assign a list to 2D/3D vector data. */
#define ZEO_VECXD_DATA_ASSIGN_LIST_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataAssignList(zVec##XD##Data *data, const zVec##XD##List *list)
#define ZEO_VECXD_DATA_ASSIGN_LIST(XD) \
  ZEO_VECXD_DATA_ASSIGN_LIST_PROTOTYPE( XD ){ \
    if( !( data->data.list_ref = list ) ){ \
      ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "list" ); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    data->method = &zeo_vec##XD##_data_method_list_ptr; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* assign a list of addresses of 2D/3D vector data. */
#define ZEO_VECXD_DATA_ASSIGN_ADDRLIST_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataAssignAddrList(zVec##XD##Data *data, const zVec##XD##AddrList *list)
#define ZEO_VECXD_DATA_ASSIGN_ADDRLIST(XD) \
  ZEO_VECXD_DATA_ASSIGN_ADDRLIST_PROTOTYPE( XD ){ \
    if( !( data->data.addrlist_ref = list ) ){ \
      ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "address list" ); \
      data->method = &zeo_vec##XD##_data_method_dummy; \
      return NULL; \
    } \
    data->method = &zeo_vec##XD##_data_method_addrlist_ptr; \
    zVec##XD##DataRewind( data ); \
    return data; \
  }

/* create a list of addresses of 2D/3D vectors of data. */
#define ZEO_VECXD_DATA_TO_ADDRLIST_PROTOTYPE(XD) \
  bool zVec##XD##DataToAddrList(zVec##XD##Data *data, zVec##XD##AddrList *addrlist)
#define ZEO_VECXD_DATA_TO_ADDRLIST(XD) \
  ZEO_VECXD_DATA_TO_ADDRLIST_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    zListInit( addrlist ); \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ){ \
      if( !zVec##XD##AddrListAdd( addrlist, v ) ){ \
        zVec##XD##AddrListDestroy( addrlist ); \
        return false; \
      } \
    } \
    return true; \
  }

/* find an identical vector in a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_FIND_PROTOTYPE(XD) \
  zVec##XD *zVec##XD##DataFind(zVec##XD##Data *data, const zVec##XD *point)
#define ZEO_VECXD_DATA_FIND(XD) \
  ZEO_VECXD_DATA_FIND_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ) \
      if( zVec##XD##Equal( point, v ) ) return v; \
    return NULL; \
  }

/* transform points in a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_XFORM_PROTOTYPE(XD) \
  bool zVec##XD##DataXform(zVec##XD##Data *src, const zFrame##XD *frame, double scale, zVec##XD##Data *dest)
#define ZEO_VECXD_DATA_XFORM(XD) \
  ZEO_VECXD_DATA_XFORM_PROTOTYPE( XD ){ \
    zVec##XD *v, p; \
    zVec##XD##DataInitList( dest ); \
    zVec##XD##DataRewind( src ); \
    while( ( v = zVec##XD##DataFetch( src ) ) ){ \
      zXform##XD( frame, v, &p ); \
      zVec##XD##MulDRC( &p, scale ); \
      if( !zVec##XD##DataAdd( dest, &p ) ) return false; \
    }\
    return true; \
  }

/* a naive algorithm to find the nearest neighbor in a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_NN_PROTOTYPE(XD) \
  double zVec##XD##DataNN(zVec##XD##Data *data, const zVec##XD *point, zVec##XD **nn)
#define ZEO_VECXD_DATA_NN(XD) \
  ZEO_VECXD_DATA_NN_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    double d, dmin = HUGE_VAL; \
    zVec##XD nn_tmp, *nnp_tmp; \
    if( !nn ){ \
      nnp_tmp = &nn_tmp; \
      nn = &nnp_tmp; \
    } \
    if( zVec##XD##DataIsEmpty( data ) ){ \
      ZRUNERROR( ZEO_ERR_EMPTYSET ); \
      *nn = NULL; \
      return HUGE_VAL; \
    } \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ){ \
      if( ( d = zVec##XD##SqrDist( v, point ) ) < dmin ){ \
        dmin = d; \
        *nn = v; \
      } \
    } \
    return sqrt( dmin ); \
  }

/* a naive algorithm to find vicinity of a point in a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_VICINITY_PROTOTYPE(XD) \
  zVec##XD##Data *zVec##XD##DataVicinity(zVec##XD##Data *data, const zVec##XD *point, double radius, zVec##XD##Data *vicinity)
#define ZEO_VECXD_DATA_VICINITY(XD) \
  ZEO_VECXD_DATA_VICINITY_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    double r2; \
    r2 = _zSqr( radius ); \
    zVec##XD##DataInitAddrList( vicinity ); \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ){ \
      if( zVec##XD##SqrDist( v, point ) < r2 ) zVec##XD##DataAdd( vicinity, v ); \
    } \
    return vicinity; \
  }

/* support map of a set of 2D/3D points with respect to a direction vector. */
#define ZEO_VECXD_DATA_SUPPORTMAP_PROTOTYPE(XD) \
  const zVec##XD *zVec##XD##DataSupportMap(zVec##XD##Data *data, const zVec##XD *direction)
#define ZEO_VECXD_DATA_SUPPORTMAP(XD) \
  ZEO_VECXD_DATA_SUPPORTMAP_PROTOTYPE( XD ){ \
    const zVec##XD *sp; \
    zVec##XD *v; \
    double d, d_max; \
    if( zVec##XD##DataIsEmpty( data ) ){ \
      ZRUNERROR( ZEO_ERR_EMPTYSET ); \
      return NULL; \
    } \
    zVec##XD##DataRewind( data ); \
    sp = zVec##XD##DataFetch( data ); \
    d_max = zVec##XD##InnerProd( sp, direction ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ){ \
      if( ( d = zVec##XD##InnerProd( v, direction ) ) > d_max ){ \
        sp = v; \
        d_max = d; \
      } \
    } \
    return sp; \
  }

/* print out a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##DataFPrint(FILE *fp, zVec##XD##Data *data)
#define ZEO_VECXD_DATA_FPRINT(XD) \
  ZEO_VECXD_DATA_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ) \
      zVec##XD##FPrint( fp, v ); \
  }

#define zVecXDDataPrint(XD,data) zVec##XD##DataFPrint( stdout, data )

/* print out values of a set of 2D/3D vectors. */
#define ZEO_VECXD_DATA_VALUE_FPRINT_PROTOTYPE(XD) \
  void zVec##XD##DataValueFPrint(FILE *fp, zVec##XD##Data *data)
#define ZEO_VECXD_DATA_VALUE_FPRINT(XD) \
  ZEO_VECXD_DATA_VALUE_FPRINT_PROTOTYPE( XD ){ \
    zVec##XD *v; \
    zVec##XD##DataRewind( data ); \
    while( ( v = zVec##XD##DataFetch( data ) ) ) \
      zVec##XD##ValueNLFPrint( fp, v ); \
  }

#define zVecXDDataValuePrint(XD,data) zVec##XD##DataValueFPrint( stdout, data )

__END_DECLS

#endif /* __ZEO_VECXD_DATA_H__ */

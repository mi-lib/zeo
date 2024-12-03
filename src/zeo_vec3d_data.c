/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_data - abstract class of a set of 3D vectors.
 */

#include <zeo/zeo_vec3d.h>

/* dummy */

static void _zVec3DDataDummyDestroy(zVec3DData *data){}
static size_t _zVec3DDataDummySize(zVec3DData *data){ return 0; }
static bool _zVec3DDataDummyIsEmpty(zVec3DData *data){ return true; }
static bool _zVec3DDataDummyAdd(zVec3DData *data, const zVec3D *vec){ return false; }
static void _zVec3DDataDummyRewind(zVec3DData *data){}
static zVec3D *_zVec3DDataDummyFetch(zVec3DData *data){ return NULL; }

static const zVec3DDataMethod zeo_vec3d_data_method_dummy = {
  destroy:  _zVec3DDataDummyDestroy,
  size:     _zVec3DDataDummySize,
  is_empty: _zVec3DDataDummyIsEmpty,
  add:      _zVec3DDataDummyAdd,
  rewind:   _zVec3DDataDummyRewind,
  fetch:    _zVec3DDataDummyFetch,
};

/* array */

static void _zVec3DDataArrayDestroy(zVec3DData *data){
  zVec3DArrayFree( &data->data.array );
}
static size_t _zVec3DDataArraySize(zVec3DData *data){
  return zArraySize( &data->data.array );
}
static bool _zVec3DDataArrayIsEmpty(zVec3DData *data){
  return zArraySize( &data->data.array ) == 0;
}
static bool _zVec3DDataArrayAdd(zVec3DData *data, const zVec3D *vec){
  if( data->pointer.i >= zArraySize(&data->data.array) ) return false;
  zVec3DCopy( vec, zArrayElemNC(&data->data.array,data->pointer.i) );
  data->pointer.i++;
  return true;
}
static void _zVec3DDataArrayRewind(zVec3DData *data){
  data->pointer.i = 0;
}
static zVec3D *_zVec3DDataArrayFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.i >= zArraySize(&data->data.array) ) return NULL;
  ret = zArrayElemNC(&data->data.array,data->pointer.i);
  data->pointer.i++;
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_array = {
  destroy:  _zVec3DDataArrayDestroy,
  size:     _zVec3DDataArraySize,
  is_empty: _zVec3DDataArrayIsEmpty,
  add:      _zVec3DDataArrayAdd,
  rewind:   _zVec3DDataArrayRewind,
  fetch:    _zVec3DDataArrayFetch,
};

/* initialize 3D vector data as an array. */
zVec3DData *zVec3DDataInitArray(zVec3DData *data, size_t size)
{
  zVec3DArrayAlloc( &data->data.array, size );
  if( zArraySize(&data->data.array) != size ){
    data->method = &zeo_vec3d_data_method_dummy;
    return NULL;
  }
  data->method = &zeo_vec3d_data_method_array;
  zVec3DDataRewind( data );
  return data;
}

/* list */

static void _zVec3DDataListDestroy(zVec3DData *data){
  zVec3DListDestroy( &data->data.list );
}
static size_t _zVec3DDataListSize(zVec3DData *data){
  return zListSize( &data->data.list );
}
static bool _zVec3DDataListIsEmpty(zVec3DData *data){
  return zListIsEmpty( &data->data.list );
}
static bool _zVec3DDataListAdd(zVec3DData *data, const zVec3D *vec){
  return zVec3DListAdd( &data->data.list, vec ) ? true: false;
}
static void _zVec3DDataListRewind(zVec3DData *data){
  data->pointer.cp = zListTail(&data->data.list);
}
static zVec3D *_zVec3DDataListFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.cp == zListRoot(&data->data.list) ) return NULL;
  ret = &data->pointer.cp->data;
  data->pointer.cp = zListCellNext(data->pointer.cp);
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_list = {
  destroy:  _zVec3DDataListDestroy,
  size:     _zVec3DDataListSize,
  is_empty: _zVec3DDataListIsEmpty,
  add:      _zVec3DDataListAdd,
  rewind:   _zVec3DDataListRewind,
  fetch:    _zVec3DDataListFetch,
};

/* initialize 3D vector data as a list. */
zVec3DData *zVec3DDataInitList(zVec3DData *data)
{
  zListInit( &data->data.list );
  data->method = &zeo_vec3d_data_method_list;
  zVec3DDataRewind( data );
  return data;
}

/* address list */

static void _zVec3DDataAddrListDestroy(zVec3DData *data){
  zVec3DAddrListDestroy( &data->data.addrlist );
}
static size_t _zVec3DDataAddrListSize(zVec3DData *data){
  return zListSize( &data->data.addrlist );
}
static bool _zVec3DDataAddrListIsEmpty(zVec3DData *data){
  return zListIsEmpty( &data->data.addrlist );
}
static bool _zVec3DDataAddrListAdd(zVec3DData *data, const zVec3D *vec){
  return zVec3DAddrListAdd( &data->data.addrlist, vec ) ? true: false;
}
static void _zVec3DDataAddrListRewind(zVec3DData *data){
  data->pointer.ap = zListTail(&data->data.addrlist);
}
static zVec3D *_zVec3DDataAddrListFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.ap == zListRoot(&data->data.addrlist) ) return NULL;
  ret = data->pointer.ap->data;
  data->pointer.ap = zListCellNext(data->pointer.ap);
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_addrlist = {
  destroy:  _zVec3DDataAddrListDestroy,
  size:     _zVec3DDataAddrListSize,
  is_empty: _zVec3DDataAddrListIsEmpty,
  add:      _zVec3DDataAddrListAdd,
  rewind:   _zVec3DDataAddrListRewind,
  fetch:    _zVec3DDataAddrListFetch,
};

/* initialize 3D vector data as a list of addresses. */
zVec3DData *zVec3DDataInitAddrList(zVec3DData *data)
{
  zListInit( &data->data.addrlist );
  data->method = &zeo_vec3d_data_method_addrlist;
  zVec3DDataRewind( data );
  return data;
}

/* array pointer */

static void _zVec3DDataArrayPtrDestroy(zVec3DData *data){
  data->data.array_ptr = NULL;
}
static size_t _zVec3DDataArrayPtrSize(zVec3DData *data){
  return zArraySize( data->data.array_ptr );
}
static bool _zVec3DDataArrayPtrIsEmpty(zVec3DData *data){
  return zArraySize( data->data.array_ptr ) == 0;
}
static bool _zVec3DDataArrayPtrAdd(zVec3DData *data, const zVec3D *vec){
  return false;
}
static zVec3D *_zVec3DDataArrayPtrFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.i >= zArraySize(data->data.array_ptr) ) return NULL;
  ret = zArrayElemNC(data->data.array_ptr,data->pointer.i);
  data->pointer.i++;
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_array_ptr = {
  destroy:  _zVec3DDataArrayPtrDestroy,
  size:     _zVec3DDataArrayPtrSize,
  is_empty: _zVec3DDataArrayPtrIsEmpty,
  add:      _zVec3DDataArrayPtrAdd,
  rewind:   _zVec3DDataArrayRewind,
  fetch:    _zVec3DDataArrayPtrFetch,
};

/* assign an array to 3D vector data. */
zVec3DData *zVec3DDataAssignArray(zVec3DData *data, const zVec3DArray *array)
{
  if( !( data->data.array_ptr = array ) ){
    ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "array" );
    data->method = &zeo_vec3d_data_method_dummy;
    return NULL;
  }
  data->method = &zeo_vec3d_data_method_array_ptr;
  zVec3DDataRewind( data );
  return data;
}

/* list pointer */

static void _zVec3DDataListPtrDestroy(zVec3DData *data){
  data->data.list_ptr = NULL;
}
static size_t _zVec3DDataListPtrSize(zVec3DData *data){
  return zListSize( data->data.list_ptr );
}
static bool _zVec3DDataListPtrIsEmpty(zVec3DData *data){
  return zListIsEmpty( data->data.list_ptr );
}
static bool _zVec3DDataListPtrAdd(zVec3DData *data, const zVec3D *vec){
  return false;
}
static void _zVec3DDataListPtrRewind(zVec3DData *data){
  data->pointer.cp = zListTail(data->data.list_ptr);
}
static zVec3D *_zVec3DDataListPtrFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.cp == zListRoot(data->data.list_ptr) ) return NULL;
  ret = &data->pointer.cp->data;
  data->pointer.cp = zListCellNext(data->pointer.cp);
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_list_ptr = {
  destroy:  _zVec3DDataListPtrDestroy,
  size:     _zVec3DDataListPtrSize,
  is_empty: _zVec3DDataListPtrIsEmpty,
  add:      _zVec3DDataListPtrAdd,
  rewind:   _zVec3DDataListPtrRewind,
  fetch:    _zVec3DDataListPtrFetch,
};

/* assign a list to 3D vector data. */
zVec3DData *zVec3DDataAssignList(zVec3DData *data, const zVec3DList *list)
{
  if( !( data->data.list_ptr = list ) ){
    ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "list" );
    data->method = &zeo_vec3d_data_method_dummy;
    return NULL;
  }
  data->method = &zeo_vec3d_data_method_list_ptr;
  zVec3DDataRewind( data );
  return data;
}

/* address list pointer */

static void _zVec3DDataAddrListPtrDestroy(zVec3DData *data){
  data->data.addrlist_ptr = NULL;
}
static size_t _zVec3DDataAddrListPtrSize(zVec3DData *data){
  return zListSize( data->data.addrlist_ptr );
}
static bool _zVec3DDataAddrListPtrIsEmpty(zVec3DData *data){
  return zListIsEmpty( data->data.addrlist_ptr );
}
static bool _zVec3DDataAddrListPtrAdd(zVec3DData *data, const zVec3D *vec){
  return false;
}
static void _zVec3DDataAddrListPtrRewind(zVec3DData *data){
  data->pointer.ap = zListTail(data->data.addrlist_ptr);
}
static zVec3D *_zVec3DDataAddrListPtrFetch(zVec3DData *data){
  zVec3D *ret;
  if( data->pointer.ap == zListRoot(data->data.addrlist_ptr) ) return NULL;
  ret = data->pointer.ap->data;
  data->pointer.ap = zListCellNext(data->pointer.ap);
  return ret;
}

static const zVec3DDataMethod zeo_vec3d_data_method_addrlist_ptr = {
  destroy:  _zVec3DDataAddrListPtrDestroy,
  size:     _zVec3DDataAddrListPtrSize,
  is_empty: _zVec3DDataAddrListPtrIsEmpty,
  add:      _zVec3DDataAddrListPtrAdd,
  rewind:   _zVec3DDataAddrListPtrRewind,
  fetch:    _zVec3DDataAddrListPtrFetch,
};

/* assign a list of addresses to 3D vector data. */
zVec3DData *zVec3DDataAssignAddrList(zVec3DData *data, const zVec3DAddrList *list)
{
  if( !( data->data.addrlist_ptr = list ) ){
    ZRUNERROR( ZEO_ERR_NULLPOINTERASSIGNED, "address list" );
    data->method = &zeo_vec3d_data_method_dummy;
    return NULL;
  }
  data->method = &zeo_vec3d_data_method_addrlist_ptr;
  zVec3DDataRewind( data );
  return data;
}

/* a naive algorithm to find the nearest neighbor in 3D vector pool. */
double zVec3DDataNN(zVec3DData *data, const zVec3D *point, zVec3D **nn)
{
  zVec3D *v;
  double d, dmin = HUGE_VAL;
  zVec3D nn_tmp, *nnp_tmp;

  if( !nn ){
    nnp_tmp = &nn_tmp;
    nn = &nnp_tmp;
  }
  if( zVec3DDataIsEmpty( data ) ){
    ZRUNWARN( ZEO_ERR_EMPTYSET );
    *nn = NULL;
    return HUGE_VAL;
  }
  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) ){
    if( ( d = zVec3DSqrDist( v, point ) ) < dmin ){
      dmin = d;
      *nn = v;
    }
  }
  return sqrt( dmin );
}

/* support map of a set of points with respect to a direction vector. */
const zVec3D *zVec3DDataSupportMap(zVec3DData *data, zVec3D *direction)
{
  const zVec3D *sp;
  zVec3D *v;
  double d, d_max;

  if( zVec3DDataIsEmpty( data ) ){
    ZRUNWARN( ZEO_ERR_EMPTYSET );
    return NULL;
  }
  zVec3DDataRewind( data );
  sp = zVec3DDataFetch( data );
  d_max = zVec3DInnerProd( sp, direction );
  while( ( v = zVec3DDataFetch( data ) ) ){
    if( ( d = zVec3DInnerProd( v, direction ) ) > d_max ){
      sp = v;
      d_max = d;
    }
  }
  return sp;
}

/* print out a set of 3D vectors. */
void zVec3DDataFPrint(FILE *fp, zVec3DData *data)
{
  zVec3D *v;

  zVec3DDataRewind( data );
  while( ( v = zVec3DDataFetch( data ) ) )
    zVec3DFPrint( fp, v );
}

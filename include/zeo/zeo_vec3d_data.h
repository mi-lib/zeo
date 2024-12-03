/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_vec3d_data - abstract class of a set of 3D vectors.
 */

#ifndef __ZEO_VEC3D_DATA_H__
#define __ZEO_VEC3D_DATA_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

struct _zVec3DDataMethod;

/*! \struct zVec3DData
 * \brief an abstract class of a set of 3D vectors.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DData ){
  union{
    zVec3DArray array;
    zVec3DList list;
    zVec3DAddrList addrlist;
    const zVec3DArray *array_ptr;
    const zVec3DList *list_ptr;
    const zVec3DAddrList *addrlist_ptr;
  } data;
  union{
    int i;
    zVec3DListCell *cp;
    zVec3DAddrListCell *ap;
  } pointer;
  const struct _zVec3DDataMethod *method;
};

/*! \struct zVec3DDataMethod
 * \brief a set of pointers to methods for an abstract class of a set of 3D vectors.
 */
ZDEF_STRUCT( __ZEO_CLASS_EXPORT, zVec3DDataMethod ){
  void (* destroy)(zVec3DData *);
  size_t (* size)(zVec3DData *);
  bool (* is_empty)(zVec3DData *);
  bool (* add)(zVec3DData *, const zVec3D *);
  void (* rewind)(zVec3DData *);
  zVec3D *(* fetch)(zVec3DData *);
};

#define zVec3DDataDestroy(data)    (data)->method->destroy( data )
#define zVec3DDataSize(data)       (data)->method->size( data )
#define zVec3DDataIsEmpty(data)    (data)->method->is_empty( data )
#define zVec3DDataAdd(data,v)      (data)->method->add( data, v )
#define zVec3DDataRewind(data)     (data)->method->rewind( data )
#define zVec3DDataFetch(data)      (data)->method->fetch( data )

/* initialize 3D vector data as an array. */
__ZEO_EXPORT zVec3DData *zVec3DDataInitArray(zVec3DData *data, size_t size);
/* initialize 3D vector data as a list. */
__ZEO_EXPORT zVec3DData *zVec3DDataInitList(zVec3DData *data);
/* initialize 3D vector data as a list of addresses. */
__ZEO_EXPORT zVec3DData *zVec3DDataInitAddrList(zVec3DData *data);

/* assign an array to 3D vector data. */
__ZEO_EXPORT zVec3DData *zVec3DDataAssignArray(zVec3DData *data, const zVec3DArray *array);
/* assign a list to 3D vector data. */
__ZEO_EXPORT zVec3DData *zVec3DDataAssignList(zVec3DData *data, const zVec3DList *list);
/* assign a list of addresses to 3D vector data. */
__ZEO_EXPORT zVec3DData *zVec3DDataAssignAddrList(zVec3DData *data, const zVec3DAddrList *list);

/* a naive algorithm to find the nearest neighbor in 3D vector pool. */
__ZEO_EXPORT double zVec3DDataNN(zVec3DData *data, const zVec3D *point, zVec3D **nn);
/* support map of a set of points with respect to a direction vector. */
__ZEO_EXPORT const zVec3D *zVec3DDataSupportMap(zVec3DData *data, zVec3D *direction);

/* print out a set of 3D vectors. */
__ZEO_EXPORT void zVec3DDataFPrint(FILE *fp, zVec3DData *data);

__END_DECLS

#endif /* __ZEO_VEC3D_DATA_H__ */

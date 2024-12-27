/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_bvxd_boundingball - 2D/3D bounding ball.
 */

#ifndef __ZEO_BVXD_BOUNDINGBALL_H__
#define __ZEO_BVXD_BOUNDINGBALL_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* A recursive implementation of the bounding ball algorithm based on the original algorithm
 * proposed by Welzl (1991). This is possibly violated due to stack-overflow. */

#define ZEO_BOUNDINGBALL_RECURSIVE_DEF_METHOD(XD,DIM,BALLTYPE) \
static int _zBoundingBall##XD##Recursive(BALLTYPE *bb, zVec##XD##AddrList *pl, zVec##XD##AddrList *shell, zVec##XD **vp) \
{ \
  zVec##XD##AddrListCell *cp; \
  int num; \
\
  if( zListIsEmpty(pl) || zListSize(shell) == DIM ) \
    return _zBoundingBall##XD##Direct( bb, shell, vp ); \
  zListDeleteTail( pl, &cp ); \
  num = _zBoundingBall##XD##Recursive( bb, pl, shell, vp ); \
  if( !BALLTYPE##PointIsInside( bb, cp->data, zTOL ) ){ \
    zListInsertTail( shell, cp ); \
    num = _zBoundingBall##XD##Recursive( bb, pl, shell, vp ); \
    zListPurge( shell, cp ); \
  } \
  zListInsertTail( pl, cp ); \
  return num; \
} \
\
int zVec##XD##DataBoundingBallRecursive(zVec##XD##Data *data, BALLTYPE *bb, zVec##XD **vp) \
{ \
  zVec##XD##AddrList addrlist; \
  zVec##XD##AddrList shell; \
  int num; \
\
  zListInit( &shell ); \
  if( !zVec##XD##DataToAddrList( data, &addrlist ) ) return 0; \
  num = _zBoundingBall##XD##Recursive( bb, &addrlist, &shell, vp ); \
  zVec##XD##AddrListDestroy( &shell ); \
  zVec##XD##AddrListDestroy( &addrlist ); \
  return num; \
}

/* A non-recursive implementation of the bounding ball algorithm. This is also based on the original
 * algorithm by Welzl (1991), but uses an action stack to avoid recursive function calls. */

#define ZEO_BOUNDINGBALL_MAX_RETRY_NUM 100

typedef enum{
  ZEO_BOUNDINGBALL_ACTION_NONE = -1,
  ZEO_BOUNDINGBALL_ACTION_SAVE = 0,
  ZEO_BOUNDINGBALL_ACTION_CHECK,
  ZEO_BOUNDINGBALL_ACTION_REVERT,
} zBoundingBallAction;
zListClass( zBoundingBallActionStack, zBoundingBallActionStackCell, zBoundingBallAction );

#define ZEO_BOUNDINGBALL_DEF_METHOD(XD,DIM,BALLTYPE) \
typedef struct{ \
  zVec##XD##AddrList pointstack; \
  zVec##XD##AddrList pointsavestack; \
  zVec##XD##AddrList shellstack; \
  zVec##XD##AddrList shellsavestack; \
  zBoundingBallActionStack actionstack; \
  zBoundingBallActionStack actionsavestack; \
} _zBoundingBall##XD##Data; \
\
static bool _zBoundingBall##XD##DataAlloc(_zBoundingBall##XD##Data *data, zVec##XD##Data *pointdata) \
{ \
  zVec##XD##AddrListCell *cp; \
  zBoundingBallActionStackCell *sp; \
  int i; \
\
  zVec##XD##DataToAddrList( pointdata, &data->pointstack ); \
  zListInit( &data->pointsavestack ); \
  zListInit( &data->shellstack ); \
  zListInit( &data->shellsavestack ); \
  for( i=0; i<DIM; i++ ){ \
    if( !( cp = zAlloc( zVec##XD##AddrListCell, 1 ) ) ){ \
      ZALLOCERROR(); \
      return false; \
    } \
    zListInsertHead( &data->shellsavestack, cp ); \
  } \
  zListInit( &data->actionstack ); \
  for( i=0; i<zListSize(&data->pointstack); i++ ){ \
    if( !( sp = zAlloc( zBoundingBallActionStackCell, 1 ) ) ){ \
      ZALLOCERROR(); \
      return false; \
    } \
    sp->data = ZEO_BOUNDINGBALL_ACTION_SAVE; \
    zListInsertHead( &data->actionstack, sp ); \
  } \
  zListInit( &data->actionsavestack ); \
  return true; \
} \
\
static void _zBoundingBall##XD##DataDestroy(_zBoundingBall##XD##Data *data) \
{ \
  zListDestroy( zVec##XD##AddrListCell, &data->pointstack ); \
  zListDestroy( zVec##XD##AddrListCell, &data->pointsavestack ); \
  zListDestroy( zVec##XD##AddrListCell, &data->shellstack ); \
  zListDestroy( zVec##XD##AddrListCell, &data->shellsavestack ); \
  zListDestroy( zBoundingBallActionStackCell, &data->actionstack ); \
  zListDestroy( zBoundingBallActionStackCell, &data->actionsavestack ); \
} \
\
static bool _zBoundingBall##XD##DataSavePoint(_zBoundingBall##XD##Data *data, zBoundingBallAction nextaction){ \
  if( zListIsEmpty( &data->pointstack ) ) return false; \
  zListHead(&data->actionstack)->data = nextaction; \
  zStackMoveCell( zVec##XD##AddrListCell, &data->pointstack, &data->pointsavestack ); \
  zStackMoveCell( zBoundingBallActionStackCell, &data->actionstack, &data->actionsavestack ); \
  return true; \
} \
\
static bool _zBoundingBall##XD##DataRevertPoint(_zBoundingBall##XD##Data *data){ \
  if( zListIsEmpty( &data->pointsavestack ) ) return false; \
  zStackMoveCell( zVec##XD##AddrListCell, &data->pointsavestack, &data->pointstack ); \
  zStackMoveCell( zBoundingBallActionStackCell, &data->actionsavestack, &data->actionstack ); \
  return true; \
} \
\
static void _zBoundingBall##XD##DataAddShell(_zBoundingBall##XD##Data *data){ \
  zStackMoveCell( zVec##XD##AddrListCell, &data->shellsavestack, &data->shellstack ); \
  zListHead(&data->shellstack)->data = zListHead(&data->pointstack)->data; \
} \
\
static void _zBoundingBall##XD##DataRevertShell(_zBoundingBall##XD##Data *data){ \
  zStackMoveCell( zVec##XD##AddrListCell, &data->shellstack, &data->shellsavestack ); \
} \
\
static zBoundingBallAction _zBoundingBallNextAction(_zBoundingBall##XD##Data *data){ \
  return !zListIsEmpty(&data->actionstack) ? zListHead(&data->actionstack)->data : ZEO_BOUNDINGBALL_ACTION_NONE; \
} \
\
static void _zBoundingBallSetAction(_zBoundingBall##XD##Data *data, zBoundingBallAction action){ \
  zListHead(&data->actionstack)->data = action; \
} \
\
static bool _zBoundingBall##XD##DataCheck(BALLTYPE *bb, _zBoundingBall##XD##Data *data){ \
  return BALLTYPE##PointIsInside( bb, zListHead(&data->pointstack)->data, zTOL ); \
} \
\
static bool _zBoundingBall##XD##DataReorder(_zBoundingBall##XD##Data *data, BALLTYPE *bb) \
{ \
  zVec##XD##AddrListCell *cp; \
  zBoundingBallActionStackCell *ap; \
  bool reordered = false; \
  zListForEach( &data->pointstack, cp ){ /* check if an outlier exists. */ \
    if( !BALLTYPE##PointIsInside( bb, cp->data, zTOL ) ){ \
      reordered = true; \
      break; \
    } \
  } \
  if( !reordered ) return false; /* no outliers */ \
  zListPurge( &data->pointstack, cp ); \
  zListInsertHead( &data->pointstack, cp ); \
  zListForEach( &data->actionstack, ap ) /* reset action stack */ \
    ap->data = ZEO_BOUNDINGBALL_ACTION_SAVE; \
  return true; \
} \
\
int zVec##XD##DataBoundingBall(zVec##XD##Data *pointdata, BALLTYPE *bb, zVec##XD **vp) \
{ \
  _zBoundingBall##XD##Data data; \
  int i, num = 0; \
  if( !_zBoundingBall##XD##DataAlloc( &data, pointdata ) ) goto TERMINATE; \
  for( i=0; i<ZEO_BOUNDINGBALL_MAX_RETRY_NUM; i++ ){ /* try until outliers vanish. */ \
    while( 1 ){ \
      if( _zBoundingBallNextAction( &data ) == ZEO_BOUNDINGBALL_ACTION_REVERT ){ \
        _zBoundingBall##XD##DataRevertShell( &data ); \
        _zBoundingBallSetAction( &data, ZEO_BOUNDINGBALL_ACTION_SAVE ); \
        if( !_zBoundingBall##XD##DataRevertPoint( &data ) ) goto OUTOFLOOP; \
      } \
      if( zListIsEmpty(&data.pointstack) || zListSize(&data.shellstack) == DIM ){ \
        num = _zBoundingBall##XD##Direct( bb, &data.shellstack, vp ); \
        if( !_zBoundingBall##XD##DataRevertPoint( &data ) ) goto OUTOFLOOP; \
      } else \
      switch( _zBoundingBallNextAction( &data ) ){ \
      case ZEO_BOUNDINGBALL_ACTION_SAVE: \
        _zBoundingBall##XD##DataSavePoint( &data, ZEO_BOUNDINGBALL_ACTION_CHECK ); \
        break; \
      case ZEO_BOUNDINGBALL_ACTION_CHECK: \
        if( _zBoundingBall##XD##DataCheck( bb, &data ) ){ \
          _zBoundingBallSetAction( &data, ZEO_BOUNDINGBALL_ACTION_SAVE ); \
          if( !_zBoundingBall##XD##DataRevertPoint( &data ) ) goto OUTOFLOOP; \
        } else{ \
          _zBoundingBall##XD##DataAddShell( &data ); \
          _zBoundingBall##XD##DataSavePoint( &data, ZEO_BOUNDINGBALL_ACTION_REVERT ); \
        } \
        break; \
      default: ; \
      } \
    } \
   OUTOFLOOP: \
    if( !_zBoundingBall##XD##DataReorder( &data, bb ) ) break; \
  } \
  if( i == ZEO_BOUNDINGBALL_MAX_RETRY_NUM ) \
    ZITERWARN( ZEO_BOUNDINGBALL_MAX_RETRY_NUM ); \
 TERMINATE: \
  _zBoundingBall##XD##DataDestroy( &data ); \
  return num; \
}

__END_DECLS

#endif /* __ZEO_BVXD_BOUNDINGBALL_H__ */

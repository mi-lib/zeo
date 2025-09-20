/* Zeo - Z/Geometry and optics computation library.
 * Copyright (C) 2005 Tomomichi Sugihara (Zhidao)
 *
 * zeo_shape3d_ph - 3D shapes: polyhedron (for class abstraction).
 */

#ifndef __ZEO_SHAPE3D_PH_H__
#define __ZEO_SHAPE3D_PH_H__

/* NOTE: never include this header file in user programs. */

__BEGIN_DECLS

/* methods for abstraction */
__ZEO_EXPORT zShape3DCom zeo_shape3d_ph_com;

#define zShape3DPH(shape) ( (zPH3D*)(shape)->body )

#define zShape3DVertNum(shape)      zPH3DVertNum(zShape3DPH(shape))
#define zShape3DVertBuf(shape)      zPH3DVertBuf(zShape3DPH(shape))
#define zShape3DVert(shape,i)       zPH3DVert(zShape3DPH(shape),i)
#define zShape3DFaceNum(shape)      zPH3DFaceNum(zShape3DPH(shape))
#define zShape3DFaceBuf(shape)      zPH3DFaceBuf(zShape3DPH(shape))
#define zShape3DFace(shape,i)       zPH3DFace(zShape3DPH(shape),i)

#define zShape3DFaceVert(shape,i,j) zPH3DFaceVert(zShape3DPH(shape),i,j)
#define zShape3DFaceNorm(shape,i)   zPH3DFaceNorm(zShape3DPH(shape),i)

__END_DECLS

#endif /* __ZEO_SHAPE3D_PH_H__ */

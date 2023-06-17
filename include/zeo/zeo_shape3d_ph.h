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

#define zShape3DPH(s) ( (zPH3D*)(s)->body )

#define zShape3DVertNum(s)      zPH3DVertNum(zShape3DPH(s))
#define zShape3DVertBuf(s)      zPH3DVertBuf(zShape3DPH(s))
#define zShape3DVert(s,i)       zPH3DVert(zShape3DPH(s),i)
#define zShape3DFaceNum(s)      zPH3DFaceNum(zShape3DPH(s))
#define zShape3DFaceBuf(s)      zPH3DFaceBuf(zShape3DPH(s))
#define zShape3DFace(s,i)       zPH3DFace(zShape3DPH(s),i)

#define zShape3DFaceVert(s,i,j) zPH3DFaceVert(zShape3DPH(s),i,j)
#define zShape3DFaceNorm(s,i)   zPH3DFaceNorm(zShape3DPH(s),i)

__END_DECLS

#endif /* __ZEO_SHAPE3D_PH_H__ */

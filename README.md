Zeo - Z/Geometry and optics computation library.
=================================================================
Copyright (C) Tomomichi Sugihara (Zhidao) since 2005

-----------------------------------------------------------------
## [What is this?]

Zeo is a handy library for computational geometry and optics
including:
- color and optics computations
- 2D/3D vectors and matrices
- 6D spatial vectors and matrices
- 3D frame and coordinate transportation
- Euler parameter (unit quaternion)
- 3D shape elements (plane, edge and triangle)
- 3D shape primitives (box, sphere, cylinder, cone, ellipsoid,
  elliptic cylinder, polyhedron)
- multiple 3D shapes
- point cloud
- trianglation of non-convex
- bounding volume (AABB, OBB, boundin ball, convex-hull)
- collision checking (AABB tree, GJK, Muller-Preparata)
- principal component analysis of point cloud
- B-Rep (boundary representation) and boolean operations
- elevation map

ZEDA and ZM are required to be installed.

-----------------------------------------------------------------
## [Installation / Uninstallation]

### install

Install ZEDA and ZM in advance.

Move to a directly under which you want to install Zeo, and run:

   ```
   % git clone https://github.com/zhidao/zeo.git
   % cd zeo
   ```

Edit **PREFIX** in *config* file if necessary in order to specify
a directory where the header files, the library and some utilities
are installed. (default: ~/usr)

   - header files: $PREFIX/include/zeo
   - library file: $PREFIX/lib
   - utilities: $PREFIX/bin

Then, make and install.

   ```
   % make && make install
   ```

### uninstall

Do:

   ```
   % make uninstall
   ```

which removes $PREFIX/lib/libzm.so and $PREFIX/include/zeo.

-----------------------------------------------------------------
## [How to use]

When you want to compile your code *test.c*, for example, the following line will work.

   ```
   % gcc `zeo-config -L` `zeo-config -I` test.c `zeo-config -l`
   ```

-----------------------------------------------------------------
## [Contact]

zhidao@ieee.org

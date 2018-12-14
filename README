Zeo - Z/Geometry and optics computation library.
Copyright (C) 2005 Tomomichi Sugihara (Zhidao)

-----------------------------------------------------------------
[What is this?]

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

ZEDA and ZM are required.

-----------------------------------------------------------------
[Installation / Uninstallation]

<install>
0. Install ZEDA and ZM in advance.

1. Unpack the distributed archive where you want.

% zcat zeo-X.Y.Z.tgz | tar xvf
or
% tar xzvf zeo-X.Y.Z.tgz (GNU tar)

X.Y.Z is for the revision.

2. Enter the directory.

% cd zeo-X.Y.Z

3. Edit config file if necessary.
  PREFIX   directory where the library is installed.
           ~/usr as a default. In this case, header files
           and library are installed under ~/usr/include
           and ~/usr/lib, respectively.

4. Make it.

% make

5. Install it.

% make install

Or,

% cp -a lib/libzeo.so $PREFIX/lib/
% cp -a include/zeo $PREFIX/include/
% cp -a bin/* $PREFIX/bin/

<uninstall>
Delete $PREFIX/lib/libzeo.so and $PREFIX/include/zeo.

-----------------------------------------------------------------
[How to use]

You may need to set your PATH and LD_LIBRARY_PATH environment
variables. This is done by:
 export PATH=$PATH:$PREFIX/bin
 export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PREFIX/lib
if your working shell is Bourne shell (bash, zsh, etc.), or by:
 set path = ( $path $PREFIX/bin )
 setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:$PREFIX/lib
if your working shell is C shell (csh, tcsh, etc.).

When you want to compile your code test.c, for example, the following
line will work.

% gcc `zeo-config -L` `zeo-config -I` test.c `zeo-config -l`

-----------------------------------------------------------------
[Contact]

zhidao@ieee.org

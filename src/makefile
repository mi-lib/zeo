include ../config

ROOTDIR=..
LIBDIR=$(ROOTDIR)/lib
INCLUDE=-I$(ROOTDIR)/include -I$(PREFIX)/include

CC=gcc
CFLAGS=-ansi -Wall -fPIC -O3 $(INCLUDE) -funroll-loops
LD=gcc
LDFLAGS=-shared
SIGNUP=echo "Zeo ver."$(VERSION)" Copyright (C) 2005 Tomomichi Sugihara (Zhidao)" >>

OBJ=zeo_color.o zeo_optic.o\
	zeo_misc.o\
	zeo_vec2d.o zeo_mat2d.o\
	zeo_vec3d.o zeo_vec6d.o zeo_mat3d.o zeo_mat6d.o\
	zeo_vec3d_list.o zeo_vec3d_tree.o zeo_vec3d_pca.o\
	zeo_ep.o zeo_frame.o\
	zeo_pointcloud.o\
	zeo_elem.o zeo_elem_list.o zeo_ph.o zeo_ph_stl.o\
	zeo_nurbs.o\
	zeo_prim.o zeo_prim_box.o zeo_prim_sphere.o zeo_prim_ellips.o zeo_prim_cyl.o zeo_prim_ecyl.o zeo_prim_cone.o zeo_prim_ph.o zeo_prim_nurbs.o\
	zeo_shape.o zeo_shape_list.o zeo_mshape.o\
	zeo_brep.o zeo_brep_trunc.o zeo_brep_bool.o\
	zeo_bv_ch2.o zeo_bv_aabb.o zeo_bv_obb.o zeo_bv_bball.o zeo_bv_qhull.o\
	zeo_col.o zeo_col_box.o zeo_col_aabbtree.o zeo_col_minkowski.o zeo_col_gjk.o zeo_col_mpr.o zeo_col_ph.o\
	zeo_terra.o\
	zeo_map.o
DLIB=libzeo.so

TARGET=$(DLIB)

all: $(TARGET)

$(DLIB): $(OBJ)
	@echo " LD	" $^
	@$(LD) $(LDFLAGS) -o $@ $^ > /dev/null; $(SIGNUP) $@
	-@mv $@ $(LIBDIR)
%.o: %.c
	@echo " CC	" $<
	@$(CC) $(CFLAGS) -c $< > /dev/null
clean:
	@echo " CLEAN"
	-@rm -f *.o *~ \#* $(TARGET)

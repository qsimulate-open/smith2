SUBDIRS = dg ig storage .
bin_PROGRAMS = CCGEN
CCGEN_SOURCES = binarycontraction.cc cost.cc equation.cc index.cc main.cc pretensor.cc smartindex.cc tensor.cc tree.cc vectensor.cc \
                binarycontraction_impl.cc indexspace.cc spaces.cc smartindexlist.cc block.cc tensor_impl.cc
LDADD = dg/libdg.a ig/libig.a storage/libstorage.a -lblas -llapack

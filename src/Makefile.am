SUBDIRS = dg ig .
bin_PROGRAMS = SMITH2
SMITH2_SOURCES = binarycontraction.cc cost.cc equation.cc index.cc main.cc pretensor.cc smartindex.cc tensor.cc tree.cc vectensor.cc \
                smartindexlist.cc #block.cc tensor_impl.cc binarycontraction_impl.cc
LDADD = dg/libdg.a ig/libig.a -lblas -llapack -L/opt/local/lib -lboost_regex
AM_CXXFLAGS=-I$(top_srcdir)

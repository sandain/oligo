
bin_PROGRAMS = oligo

noinst_LTLIBRARIES = \
    liboligo_cluster.la \
    liboligo_fasta.la \
    liboligo_newick.la \
    liboligo_sequence.la \
    liboligo_tools.la

oligo_SOURCES = oligo.c
oligo_LDADD =  \
    -lm \
    liboligo_cluster.la \
    liboligo_fasta.la \
    liboligo_newick.la \
    liboligo_sequence.la \
    liboligo_tools.la

liboligo_cluster_la_SOURCES = cluster.h cluster.c
liboligo_cluster_la_LIBADD = -lvl

liboligo_fasta_la_SOURCES = fasta.h fasta.c

liboligo_newick_la_SOURCES = newick.h newick.c

liboligo_sequence_la_SOURCES = sequence.h sequence.c

liboligo_tools_la_SOURCES = tools.h tools.c


TESTS = test_fasta

check_PROGRAMS = $(TESTS)

test_fasta_SOURCES = test_fasta.c
test_fasta_CFLAGS = @CHECK_CFLAGS@
test_fasta_LDADD = \
    $(top_builddir)/src/liboligo_fasta.la \
    $(top_builddir)/src/liboligo_sequence.la \
    $(top_builddir)/src/liboligo_tools.la \
    @CHECK_LIBS@

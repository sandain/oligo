#include <check.h>

#include "../src/fasta.h"
#include "../src/sequence.h"


START_TEST (size) {
  Fasta * fasta = newFasta ("test_fasta.nwk");
  ck_assert_msg (
    fasta->size == 2,
    "Expected a fasta size of 2.\n"
  );
} END_TEST



Suite * fasta_suite (void) {
  Suite *s = suite_create ("Fasta");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_test (tc_core, size);



  suite_add_tcase (s, tc_core);

  return s;
}

int main (void) {
  int number_failed;
  Suite *s = fasta_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

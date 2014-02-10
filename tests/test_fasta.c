#include <check.h>

#include "../src/fasta.h"
#include "../src/sequence.h"

Fasta * fasta;

void setup (void) {
  fasta = newFasta ("test_fasta.nwk");
}

void teardown (void) {
  freeFasta (fasta);
}

START_TEST (test_fasta_size) {
  ck_assert_int_eq (
    fasta->size,
    2
  );
} END_TEST

START_TEST (test_fasta_length) {
  ck_assert_int_eq (
    fasta->lengths[0],
    2268
  );
  ck_assert_int_eq (
    fasta->lengths[1],
    2268
  );
} END_TEST

START_TEST (test_fasta_identifiers) {
  ck_assert_str_eq (
    fasta->ids[0],
    "gb|CP000239.1|:2536947-2539214"
  );
  ck_assert_str_eq (
    fasta->ids[1],
    "gi|86607503:c28351-26084"
  );
} END_TEST

Suite * fasta_suite (void) {
  Suite *s = suite_create ("Fasta");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_fasta_size);
  tcase_add_test (tc_core, test_fasta_length);
  tcase_add_test (tc_core, test_fasta_identifiers);

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

/*
 *  Copyright (c) 2014, Jason M. Wood <sandain@hotmail.com>
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the
 *     distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * 
 *
 * @file test_fasta.c
 */

#include <check.h>

#include "../src/fasta.h"
#include "../src/sequence.h"

Fasta * fasta;

void setup (void) {
  fasta = newFasta ("test_fasta.fa");
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
    "gb|CP000240.1|:c28351-26084"
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
  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

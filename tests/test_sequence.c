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
 * @file test_sequence.c
 */

#include <check.h>

#include "../src/sequence.h"

Sequence * seq;

char * testIdentifier = "test_sequence";
char * testDescription = "Test description";
char * testSequence = "acgttgca";

void setup (void) {
  seq = newSequence ();
  setIdentifier (seq, testIdentifier);
  setDescription (seq, testDescription);
  setSequence (seq, testSequence);
}

void teardown (void) {
  freeSequence (seq);
}

START_TEST (test_seq_identifier) {
  ck_assert_str_eq (
    getIdentifier (seq),
    testIdentifier
  );
} END_TEST

START_TEST (test_seq_identifier_length) {
  ck_assert_int_eq (
    getIdentifierLength (seq),
    strlen (testIdentifier)
  );
} END_TEST

START_TEST (test_seq_description) {
  ck_assert_str_eq (
    getDescription (seq),
    testDescription
  );
} END_TEST

START_TEST (test_seq_description_length) {
  ck_assert_int_eq (
    getDescriptionLength (seq),
    strlen (testDescription)
  );
} END_TEST

START_TEST (test_seq_sequence) {
  ck_assert_str_eq (
    getSequence (seq),
    testSequence
  );
} END_TEST

START_TEST (test_seq_sequence_length) {
  ck_assert_int_eq (
    getSequenceLength (seq),
    strlen (testSequence)
  );
} END_TEST

Suite * sequence_suite (void) {
  Suite *s = suite_create ("Sequence");
  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_seq_identifier);
  tcase_add_test (tc_core, test_seq_identifier_length);
  tcase_add_test (tc_core, test_seq_description);
  tcase_add_test (tc_core, test_seq_description_length);
  tcase_add_test (tc_core, test_seq_sequence);
  tcase_add_test (tc_core, test_seq_sequence_length);
  suite_add_tcase (s, tc_core);
  return s;
}

int main (void) {
  int number_failed;
  Suite *s = sequence_suite ();
  SRunner *sr = srunner_create (s);
  srunner_set_fork_status (sr, CK_NOFORK);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

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
 * Loads a file containing sequence data in the fasta format.
 *
 * @file fasta.c
 */

#include "fasta.h"

/**
 * Creates a new Fasta object from the given fasta formatted file.
 *
 * @memberof Fasta
 * @public
 * @param fileName The fasta formatted file to create this object from.
 * @return The new Fasta object.
 */
Fasta * newFasta (
  char * fileName
) {
  Fasta * fasta = malloc (sizeof (Fasta));
  size_t i;
  /* Open the fasta file with read access. */
  fasta->file = fopen (fileName, "r");
  /* Verify that the fasta file is open. */
  if (fasta->file == NULL) {
    free (fasta);
    return NULL;
  }
  /* Count the number of sequences in this fasta file. */
  char * buffer = malloc (LINE_MAX * sizeof (char));
  fasta->size = 0;
  while (fgets (buffer, LINE_MAX, fasta->file) != NULL) {
    if (buffer[0] == '>') {
      fasta->size ++;
    }
  }
  free (buffer);
  /* Verify that the fasta has at least one sequence. */
  if (fasta->size == 0) {
    free (fasta);
    return NULL;
  }
  /* Grab the identifiers and the file offset for each sequence. */
  fasta->ids = malloc (fasta->size * sizeof (char *));
  fasta->lengths = malloc (fasta->size * sizeof (size_t));
  fasta->offsets = malloc (fasta->size * sizeof (size_t));
  fseek (fasta->file, 0, SEEK_SET);
  for (i = 0; i < fasta->size; i ++) {
    size_t offset = ftell (fasta->file);
    Sequence * seq = parseSequence (fasta->file);
    fasta->ids[i] = strdup (getIdentifier (seq));
    fasta->lengths[i] = getSequenceLength (seq);
    fasta->offsets[i] = offset;
    freeSequence (seq);
  }
  fseek (fasta->file, 0, SEEK_SET);
  /* Default to a minimum sequence length of 1. */
  fasta->minimumLength = 1;
  /* Set the current sequence as the first found in the file. */
  fasta->current = 0;
  return fasta;
}

/**
 * Retrieves the next sequence from the fasta file.
 *
 * @memberof Fasta
 * @public
 * @param fasta This Fasta object.
 * @param seq The sequence to be returned.
 * @return True while there are still sequences in the buffer.
 */
int nextSequence (
  Fasta * fasta,
  Sequence ** seq
) {
  /* Skip sequences that are not long enough. */
  while (
    fasta->current < fasta->size &&
    fasta->lengths[fasta->current] < fasta->minimumLength
  ) {
    fasta->current ++;
  }
  /* Make sure there are sequences available. */
  if (fasta->size <= fasta->current) {
    return 0;
  }
  /* Seek to the location of the current sequence in the file. */
  fseek (fasta->file, fasta->offsets[fasta->current], SEEK_SET);
  /* Parse the current sequence. */
  *seq = parseSequence (fasta->file);
  /* Increment the current sequence. */
  fasta->current ++;
  return 1;
}

/**
 * Retrieves the number of sequences in this object.
 *
 * @memberof Fasta
 * @public
 * @param fasta This Fasta object.
 * @return The number of sequences longer than the minimum length.
 */
size_t numberSequences (
  Fasta * fasta
) {
  size_t i;
  size_t number = 0;
  for (i = 0; i < fasta->size; i ++) {
    if (fasta->lengths[i] >= fasta->minimumLength) {
      number ++;
    }
  }
  return number;
}

/**
 * Retrieves the identifiers of the sequences in this object.
 *
 * @memberof Fasta
 * @public
 * @param fasta This Fasta object.
 * @return The number of sequences longer than length.
 */
extern char ** getIdentifiers (
  Fasta * fasta
) {
  size_t i, j;
  size_t number = numberSequences (fasta);
  char ** ids = malloc (number * sizeof (char *));
  j = 0;
  for (i = 0; i < fasta->size; i ++) {
    if (fasta->lengths[i] >= fasta->minimumLength) {
      ids[j] = fasta->ids[i];
      j ++;
    }
  }
  return ids;
}

/**
 * Set the minimum sequence length.
 *
 * @memberof Fasta
 * @public
 * @param fasta This Fasta object.
 * @param length The minimum sequence length.
 */
void setMinimumLength (
  Fasta * fasta,
  size_t length
) {
  fasta->minimumLength = length;
}

/**
 * Close the file and free the memory reserved for this Fasta object.
 *
 * @memberof Fasta
 * @public
 * @param fasta The Fasta object to free.
 */
void freeFasta (
  Fasta * fasta
) {
  size_t i;
  /* Close the fasta file. */
  fclose (fasta->file);
  /* Free the memory allocated for the Fasta object. */
  for (i = 0; i < fasta->size; i ++) {
    free (fasta->ids[i]);
  }
  free (fasta->ids);
  free (fasta->lengths);
  free (fasta->offsets);
  free (fasta);
}

/**
 * Parse a sequence from the fasta file.
 *
 * @private
 * @param file The file with the sequence to parse.
 * @return The sequence.
 */
static Sequence * parseSequence (
  FILE * file
) {
  char * buffer = malloc (LINE_MAX * sizeof (char));
  char * seqBuffer = malloc (FASTA_BUFFER_SIZE * sizeof (char));
  Sequence * seq = newSequence();
  /* Make sure there is a sequence at the current location. */
  if (fgets (buffer, LINE_MAX, file) == NULL) {
    printf ("No sequence found.\n");
    return 0;
  }
  if (buffer[0] != '>') {
    printf ("Not a fasta formated sequence.\n%s\n", buffer);
    return 0;
  }
  /* Remove line-feed and carriage-return characters from the buffer. */
  chomp (buffer);
  /* Tokenize the buffer. */
  char * token = strdup (buffer + 1);
  /* Grab the sequence identifier from the fasta buffer. */
  char * id = strtok (token, " \t\r\n");
  setIdentifier (seq, id);
  /* Grab the sequence description from the fasta buffer. */
  char * desc = strdup(buffer + strlen (id) + 2);
  setDescription (seq, desc);
  /* Initialize the sequence buffer. */
  seqBuffer[0] = '\0';
  /* Grab the sequence data. */
  while (fgets (buffer, LINE_MAX, file)) {
    /* Stop when the next sequence is found. */
    if (buffer[0] == '>') {
      fseek (file, ftell (file) - strlen (buffer), SEEK_SET);
      break;
    }
    /* Remove line-feed and carriage-return characters from the buffer. */
    chomp (buffer);
    /* Store the sequence data. */
    seqBuffer = strcat (seqBuffer, buffer);
  }
  /* Grab the sequence from the sequence buffer. */
  setSequence (seq, seqBuffer);
  /* Free reserved memory. */
  free (buffer);
  free (seqBuffer);
  free (token);
  free (desc);
  return seq;
}

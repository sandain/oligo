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
 * Stores sequence data contained in fasta formatted files.
 *
 * @file sequence.c
 */

#include "sequence.h"

/**
 * Creates a new Sequence object.
 *
 * @memberof Sequence
 * @public
 * @return The new Sequence object.
 */
Sequence * newSequence (void) {
  Sequence * seq;
  /* Allocate memory for the sequence. */
  seq = malloc (sizeof (Sequence));
  seq->identifier = malloc (sizeof (char));
  seq->description = malloc (sizeof (char));
  seq->sequence = malloc (sizeof (char));
  /* Initialize the sequence. */
  seq->identifier[0] = '\0';
  seq->description[0] = '\0';
  seq->sequence[0] = '\0';
  seq->identifierLength = 0;
  seq->descriptionLength = 0;
  seq->sequenceLength = 0;
  return seq;
}

/**
 * Store sequence identifier in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the identifier in.
 * @param identifier The sequence description.
 */
void setIdentifier (Sequence * seq, char * identifier) {
  seq->identifierLength = strlen (identifier);
  seq->identifier = realloc (
    seq->identifier, (seq->identifierLength + 1) * sizeof (char)
  );
  strcpy (seq->identifier, identifier);
  seq->identifier[seq->identifierLength] = '\0';
}

/**
 * Store sequence description in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the description in.
 * @param description The sequence description.
 */
void setDescription (Sequence * seq, char * description) {
  seq->descriptionLength = strlen(description);
  seq->description = realloc (
    seq->description, (seq->descriptionLength + 1) * sizeof (char)
  );
  strcpy (seq->description, description);
  seq->description[seq->descriptionLength] = '\0';
}

/**
 * Store sequence data in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the sequence data in.
 * @param sequence The sequence data.
 */
void setSequence (Sequence * seq, char * sequence) {
  seq->sequenceLength = strlen (sequence);
  seq->sequence = realloc (
    seq->sequence, (seq->sequenceLength + 1) * sizeof (char)
  );
  strcpy (seq->sequence, sequence);
  seq->sequence[seq->sequenceLength] = '\0';
}

/**
 * Get the sequence identifier.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The sequence identifier.
 */
char * getIdentifier (Sequence * seq) {
  return seq->identifier;
}

/**
 * Get the length of the sequence identifier.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The length of the sequence identifier.
 */
size_t getIdentifierLength (Sequence * seq) {
  return seq->identifierLength;
}

/**
 * Get the sequence description.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The sequence description.
 */
char * getDescription (Sequence * seq) {
  return seq->description;
}

/**
 * Get the length of the sequence description.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The length of the sequence description.
 */
size_t getDescriptionLength (Sequence * seq) {
  return seq->descriptionLength;
}

/**
 * Get the sequence.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The sequence.
 */
char * getSequence (Sequence * seq) {
  return seq->sequence;
}

/**
 * Get the length of the sequence.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object.
 * @return The length of the sequence.
 */
size_t getSequenceLength (Sequence * seq) {
  return seq->sequenceLength;
}

/**
 * Free the memory reserved for this sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to free.
 */
void freeSequence (Sequence * seq) {
  free (seq->identifier);
  free (seq->description);
  free (seq->sequence);
  free (seq);
}

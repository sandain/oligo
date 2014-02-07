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
 * @file fasta.h
 */

#ifndef _OLIGO_FASTA_H
#define _OLIGO_FASTA_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sequence.h"
#include "tools.h"

/**
 * @def FASTA_BUFFER_SIZE
 *   The size of the buffer.
 */
#define FASTA_BUFFER_SIZE 4294967295

/**
 * The structure to hold a Fasta object.
 * 
 * @public
 */
typedef struct Fasta {
  FILE * file;                     /**< A pointer to the fasta file. */
  size_t size;                     /**< The number of sequences. */
  char ** ids;                     /**< An array of sequence identifiers. */
  size_t * lengths;                /**< An array of sequence lengths. */
  size_t * offsets;                /**< An array of file offsets. */
  size_t current;                  /**< The current sequence. */
} Fasta;

/**
 * Creates a new Fasta object from the given fasta formatted file.
 *
 * @memberof Fasta
 * @public
 * @param fileName The fasta formatted file to create this object from.
 * @return The new Fasta object.
 */
extern Fasta * newFasta (
  char * fileName
);

/**
 * Retrieves the next sequence from the fasta file.
 *
 * @memberof Fasta
 * @public
 * @param fasta This Fasta object.
 * @param seq The sequence to be returned.
 * @return True while there are still sequences in the buffer.
 */
extern int nextSequence (
  Fasta * fasta,
  Sequence ** seq
);

/**
 * Retrieves all of the sequences larger than length from the fasta file.
 *
 * @param fasta This fasta object.
 * @param length The minimum length of sequences to return.
 * @param sequences The array of sequences larger than length.
 * @return The number of sequences larger than length.
 */
extern size_t getSequences (
  Fasta * fasta,
  size_t length,
  Sequence ** sequences 
);

/**
 * Close the file and free the memory reserved for this Fasta object.
 *
 * @memberof Fasta
 * @public
 * @param fasta The Fasta object to free.
 */
extern void freeFasta (
  Fasta * fasta
);

/**
 * Parse a sequence from the fasta file.
 *
 * @private
 * @param file The file with the sequence to parse.
 * @param offset The file offset of the sequence to parse.
 * @return The sequence.
 */
extern Sequence * parseSequence (
  FILE * file
);

#endif

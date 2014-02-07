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
 * @file sequence.h
 */

#ifndef _OLIGO_SEQUENCE_H
#define _OLIGO_SEQUENCE_H

#include <stdlib.h>
#include <string.h>

/**
 * The structure to hold a Sequence object.
 *
 * @public
 */
typedef struct Sequence {
  char * identifier;               /**< The identifier. */
  char * description;              /**< The description. */
  char * sequence;                 /**< The sequence data. */
  size_t identifierLength;         /**< The length of the identifier. */ 
  size_t descriptionLength;        /**< The length of the description. */
  size_t sequenceLength;           /**< The length of the sequence data. */
} Sequence;

/**
 * Creates a new Sequence object.
 *
 * @memberof Sequence
 * @public
 * @return The new Sequence object.
 */
extern Sequence * newSequence (void);

/**
 * Store sequence identifier in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the identifier in.
 * @param identifier The sequence description.
 */
extern void setIdentifier (Sequence * seq, char * identifier);

/**
 * Store sequence description in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the description in.
 * @param description The sequence description.
 */
extern void setDescription (Sequence * seq, char * description);

/**
 * Store sequence data in the sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq The sequence object to store the sequence data in.
 * @param sequence The sequence data.
 */
extern void setSequence (Sequence * seq, char * sequence);

/**
 * Free the memory reserved for this sequence object.
 *
 * @memberof Sequence
 * @public
 * @param seq This sequence object to free.
 */
extern void freeSequence (Sequence * seq);

#endif

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
 * Various tools used by other methods.
 *
 * @file tools.h
 */

#ifndef _OLIGO_TOOLS_H
#define _OLIGO_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Removes line-feed and carriage-return characters from the end of a string.
 *
 * @param string - The string to chomp.
 */
extern void chomp (char * string);

/**
 * Reverse a string.
 *
 * ie: "This is a string" -> "gnirts a si sihT"
 *
 * @param string The string to reverse.
 * @return The reverse of the provided string.
 */
extern char * reverse (char * string);

/**
 * Complement a strand of DNA.
 *
 * ie. "ATCGC" -> "TAGCG"
 *
 * @param string The string to complement.
 * @return The complement of the given string.
 */
extern char * complement (char * string);

/**
 * Reverse complement a strand of DNA.
 *
 * ie. "ATCGC" -> "GCGAT"
 *
 * @param string The string to reverse complement.
 * @return The reverse complement of the given string.
 */
extern char * reverseComplement (char * string);

/**
 * Test whether or not two nucleotides are equal, taking into account
 * the numerous IUPAC codes that could come into play.
 *
 * @param nucleotideA The first of the nucleotides to compare.
 * @param nucleotideB The second of the nucleotides to compare.
 * @return Returns a 0 for false, 1 for true.
 */
extern char nucleotideIsEqual (char nucleotideA, char nucleotideB);

/**
 * Test whether or not two sequences are equal, taking into account
 * the numerous IUPAC codes that could come into play.
 *
 * @param sequenceA The first of the sequences to compare.
 * @param sequenceB The second of the sequences to compare.
 * @return Returns a 0 for false, 1 for true.
 */
extern char sequenceIsEqual (char * sequenceA, char * sequenceB);


/**
 * Calculates base ^ exp.
 *
 * @param base The base.
 * @param exp The exponent.
 * @return The value of base ^ exp.
 */
extern size_t power (size_t base, size_t exp);

/**
 * Calculates the number of digits in a number.
 *
 * @param i The number to use in the calculation.
 * @return The number of digits in the provided number.
 */
extern size_t numberOfDigits (size_t i);

#endif

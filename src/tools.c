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
 * @file tools.c
 */

#include "tools.h"

/**
 * Removes line-feed and carriage-return characters from the end of a string.
 *
 * @public
 * @param string - The string to chomp.
 */
void chomp (char * string) {
  while (*string && *string != '\n' && *string != '\r') string ++;
  *string = 0;
}

/**
 * Reverse a string.
 *
 * ie: "This is a string" -> "gnirts a si sihT"
 *
 * @public
 * @param string The string to reverse.
 * @return The reverse of the provided string.
 */
char * reverse (char * string) {
  size_t length = strlen (string);
  char * reverse = malloc ((length + 1) * sizeof (char));
  size_t i;
  for (i = 0; i < length; i ++) {
    reverse[length - 1 - i] = string[i];
  }
  reverse[length] = '\0';
  return reverse;
}

/**
 * Complement a strand of DNA.
 *
 * ie. "ATCGC" -> "TAGCG"
 *
 * @public
 * @param string The string to complement.
 * @return The complement of the given string.
 */
char * complement (char * string) {
  size_t length = strlen (string);
  char * complement = malloc ((length + 1) * sizeof (char));
  size_t i;
  for (i = 0; i < length; i ++) {
    switch (string[i]) {
      /* Adenine (A) -> Thymine (T) */
      case 'a' : complement[i] = 't';
                 break;
      case 'A' : complement[i] = 'T';
                 break;
      /* Cytosine (C) -> Guanine (G) */
      case 'c' : complement[i] = 'g';
                 break;
      case 'C' : complement[i] = 'G';
                 break;
      /* Guanine (G) -> Cytosine (C) */
      case 'g' : complement[i] = 'c';
                 break;
      case 'G' : complement[i] = 'C';
                 break;
      /* Thymine (T) -> Adenine (A) */
      case 't' : complement[i] = 'a';
                 break;
      case 'T' : complement[i] = 'A';
                 break;
      /* A or G -> C or T */
      case 'r' : complement[i] = 'y';
                 break;
      case 'R' : complement[i] = 'Y';
                 break;
      /* C or T -> A or G */
      case 'y' : complement[i] = 'r';
                 break;
      case 'Y' : complement[i] = 'R';
                 break;
      /* G or T -> A or C */
      case 'k' : complement[i] = 'm';
                 break;
      case 'K' : complement[i] = 'M';
                 break;
      /* A or C -> G or T */
      case 'm' : complement[i] = 'k';
                 break;
      case 'M' : complement[i] = 'K';
                 break;
      /* C or G or T (not A) -> A or C or G (not T) */
      case 'b' : complement[i] = 'v';
                 break;
      case 'B' : complement[i] = 'V';
                 break;
      /* A or G or T (not C) -> A or C or T (not G) */
      case 'd' : complement[i] = 'h';
                 break;
      case 'D' : complement[i] = 'H';
                 break;
      /* A or C or T (not G) -> A or G or T (not C) */
      case 'h' : complement[i] = 'd';
                 break;
      case 'H' : complement[i] = 'D';
                 break;
      /* A or C or G (not T) -> C or G or T (not A) */
      case 'v' : complement[i] = 'b';
                 break;
      case 'V' : complement[i] = 'B';
                 break;
      /** G or C -> no change */
      case 's' :
      case 'S' :
      /** A or T -> no change */
      case 'w' :
      case 'W' :
      /** A or C or G or T -> no change */
      case 'n' :
      case 'N' :
      /* Gap -> no change */
      case '.' :
      case '-' :
      /* Unknown -> no change */
      case '?' :
                 complement[i] = string[i];
                 break;
      /** Unknown IUPAC character. */
      default:   complement[i] = string[i];
                 fprintf (
                   stderr, "Unrecognized nucleotide code: %c.\n", string[i]
                 );
                 break;
    }
  }
  complement[length] = '\0';
  return complement;
}

/**
 * Reverse complement a strand of DNA.
 *
 * ie. "ATCGC" -> "GCGAT"
 *
 * @public
 * @param string The string to reverse complement.
 * @return The reverse complement of the given string.
 */
char * reverseComplement (char * string) {
  char * complementString = complement (string);
  char * reverseComplement = reverse (complementString);
  free (complementString);
  return reverseComplement;
}

/**
 * Test whether or not two nucleotides are equal, taking into account
 * the numerous IUPAC codes that could come into play.
 *
 * @param nucleotideA The first of the nucleotides to compare.
 * @param nucleotideB The second of the nucleotides to compare.
 * @return Returns a 0 for false, 1 for true.
 */
char nucleotideIsEqual (char nucleotideA, char nucleotideB) {
  char isEqual = 0;
  char * testString;
  size_t i;
  /* Determine which string to use to test for equality by looking at
     nucleotide A. */
  switch (tolower (nucleotideA)) {
    /* Adenine (A) */
    case 'a' : testString = "arwmdhvn";
               break;
    /* Cytosine (C) */
    case 'c' : testString = "cysmbhvn";
               break;
    /* Guanine (G) */
    case 'g' : testString = "grskbdvn";
               break;
    /* Thymine (T) */
    case 't' : testString = "tywkbdhn";
               break;
    /* A or G */
    case 'r' : testString = "agrn";
               break;
    /* C or T */
    case 'y' : testString = "ctyn";
               break;
    /** G or C */
    case 's' : testString = "gcsn";
               break;
    /** A or T */
    case 'w' : testString = "atwn";
               break;
    /* G or T  */
    case 'k' : testString = "gtkn";
               break;
    /* A or C */
    case 'm' : testString = "acmn";
               break;
    /* C or G or T (not A) */
    case 'b' : testString = "cgtbn";
               break;
    /* A or G or T (not C) */
    case 'd' : testString = "agtdn";
               break;
    /* A or C or T (not G) */
    case 'h' : testString = "acthn";
               break;
    /* A or C or G (not T) */
    case 'v' : testString = "acgvn";
               break;
    /** A or C or G or T */
    case 'n' : testString = "acgtryswkmbdhvn"; 
               break;
    /* Gap */
    case '.' :
    case '-' : testString = ".-";
               break;
    /* Unknown */
    case '?' : testString = "?";
               break;
    /** Unknown IUPAC character. */
    default  : fprintf (
                 stderr, "Unrecognized nucleotide code: %c.\n", nucleotideA
               );
               return 0;
               break;
  }
  /* Test for equality between the nucleotide B and each character of the
     test string. */
  for (i = 0; i < strlen (testString); i ++) {
    if (testString[i] == tolower (nucleotideB)) {
      isEqual = 1;
      break;
    }
  }
  return isEqual;
}

/**
 * Test whether or not two sequences are equal, taking into account
 * the numerous IUPAC codes that could come into play.
 *
 * @param sequenceA The first of the sequences to compare.
 * @param sequenceB The second of the sequences to compare.
 * @return Returns a 0 for false, 1 for true.
 */
char sequenceIsEqual (char * sequenceA, char * sequenceB) {
  size_t i;
  size_t length = strlen (sequenceA);
  size_t nucCounter = 0;
  char isEqual = 0;
  if (length == strlen (sequenceB)) {
    for (i = 0; i < length; i ++) {
      if (nucleotideIsEqual (sequenceA[i], sequenceB[i])) {
        nucCounter ++;
      }
    }
    if (nucCounter == length) {
      isEqual = 1;
    }
  }
  return isEqual;
}


/**
 * Calculates base ^ exp.
 *
 * @public
 * @param base The base.
 * @param exp The exponent.
 * @return The value of base ^ exp.
 */
size_t power (size_t base, size_t exp) {
  size_t result = 1;
  while (exp) {
    if (exp & 1) {
      result *= base;
    }
    exp >>= 1;
    base *= base;
  }
  return result;
}

/**
 * Calculates the number of digits in a number.
 *
 * @param i The number to use in the calculation.
 * @return The number of digits in the provided number.
 */
size_t numberOfDigits (size_t i) {
  size_t num = 1;
  while (i > 9) {
    num ++;
    i /= 10;
  }
  return num;
}

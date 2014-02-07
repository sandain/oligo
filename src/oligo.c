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
 * Oligo categorizes sequence data based on oligonucleotide usage frequency.
 *
 * @file oligo.c
 * @mainpage Oligo
 *
 * Oligo categorizes sequence data based on oligonucleotide usage frequency.
 *
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "cluster.h"
#include "fasta.h"
#include "sequence.h"
#include "tools.h"

/**
 * @def DEBUG
 *   The default debug level to use.  Set to 1 to turn debugging output on.
 */
#define DEBUG 1

/**
 * @def DEFAULT_OLIGO_LENGTH
 *   The default length oligo to use.  This becomes very computationally
 *   intensive with larger numbers.
 */
#define DEFAULT_OLIGO_LENGTH 4

/**
 * @def DEFAULT_FRAGMENT_LENGTH
 *   The default length fragment to use.  Smaller fragments retain less
 *   fidelity.
 */
#define DEFAULT_FRAGMENT_LENGTH 5000

void generateOligonucleotides (
  size_t oligoLength,
  char ** oligonucleotides,
  char * oligo,
  size_t index
);

void oligoFrequency (
  Sequence ** sequences,
  size_t numSequences,
  size_t oligoLength,
  size_t fragmentLength,
  size_t numCombinations,
  double * frequency
);

/**
 * The main entry point for the Oligo program.
 *
 * @param argc The number of arguments passed to Oligo.
 * @param argv The array of arguments passed to Oligo.
 * @return The error level, 0 for no error.
 */
int main (
  int argc,
  char * argv[]
) {
  size_t oligoLength;
  size_t fragmentLength;
  size_t numSequences;
  size_t numCombinations;
  size_t i;
  double * frequency;
  char * fastaFile;
  Sequence ** sequences;
  /* Grab the fasta file from the command line, or produce an error. */
  if (argc < 2) {
    printf ("Error, fasta formatted sequence file not provided!\n");
    return 1;
  }
  fastaFile = strdup (argv[1]);
  /* Grab the oligo length from the command line, or use the default value if
     not provided. */
  if (argc >= 3) {
    oligoLength = atoi (argv[2]);
  }
  else {
    printf (
      "Oligo length parameter not supplied, using default value of %d.\n",
      DEFAULT_OLIGO_LENGTH
    );
    oligoLength = DEFAULT_OLIGO_LENGTH;
  }
  /* Grab the fragment length from the command line, or use the default value
     if not provided. */
  if (argc >= 4) {
    fragmentLength = atoi (argv[3]);
  }
  else {
    printf (
      "Fragment length parameter not supplied, using default value of %d.\n",
      DEFAULT_FRAGMENT_LENGTH
    );
    fragmentLength = DEFAULT_FRAGMENT_LENGTH;
  }
  /* Load the fasta file. */
  Fasta * fasta = newFasta (fastaFile);


  sequences = malloc (fasta->size * sizeof (Sequence *));
  numSequences = getSequences (fasta, fragmentLength, sequences);
  freeFasta (fasta);

  // XXX Use the fasta object throughout.  Requires the fasta object to be smarter.

  /* Determine the number of nucleotide combinations. */
  numCombinations = power (4, oligoLength);
  /* Initialize the frequency matrix. */
  frequency = malloc (numSequences * numCombinations * sizeof (double *));
  for (i = 0; i < numSequences * numCombinations; i ++) {
    frequency[i] = 0.0;
  }
  /* Generate the oligonucleotide usage frequency matrix. */
  printf ("Generating the oligo usage frequency matrix.\n");
  oligoFrequency (
    sequences, numSequences, oligoLength, fragmentLength, numCombinations,
    frequency
  );
  /* Display the oligonucleotide usage frequency matrix if debug is on. */
  if (DEBUG > 0) {
    size_t s, c;
    for (s = 0; s < numSequences; s ++) {
      printf ("%s: ", sequences[s]->identifier);
      for (c = 0; c < numCombinations; c ++) {
        printf ("%.4f ", frequency[s + c * numCombinations]);
      }
      printf ("\n");
    }
  }

  /* Run the Kmeans algorithm. */
  printf ("Running the Kmeans algorithm.\n");
  runKmeans (sequences, numSequences, numCombinations, frequency, 10, DEBUG);

  /* Run the AIB algorithm. */
  printf ("Running the AIB algorithm.\n");
  runAIB (sequences, numSequences, numCombinations, frequency, DEBUG);



  /* Free reserved memory. */
  for (i = 0; i < numSequences; i++) {
    freeSequence (sequences[i]);
  }
  free (sequences);
  free (frequency);
  return 0;
}

/**
 * Generate all of the nucleotide combinations for the given length using a
 * recursive method.
 * 
 * @param oligoLength The length of oligonucleotides to generate.
 * @param oligonucleotides The final list of oligonucleotides generated.
 * @param oligo The oligo being generated via recursion.
 * @param index The index of oligonucleotides to store the resulting
 *        oligonucleotide in.
 */
void generateOligonucleotides (
  size_t oligoLength,
  char ** oligonucleotides,
  char * oligo,
  size_t index
) {
  char nucs[4] = {'a', 'c', 'g', 't'};
  size_t i;
  size_t length = strlen(oligo);
  /* If oligo is not long enough, append the four nucleotides to oligo and
     recurse. If oligo is long enough, push the oligo onto the
     oligonucleotides array and return. */
  if (oligoLength > length) {
    /* Make a local copy of the oligo. */
    char buffer[oligoLength + 1];
    strcpy (buffer, oligo);
    for (i = 0; i < 4; i ++) {
      buffer[length] = nucs[i];
      buffer[length + 1] = '\0';
      generateOligonucleotides (
        oligoLength, oligonucleotides, buffer, index + i * pow(4, length)
      );
    }
  }
  else {
    oligonucleotides[index] = strdup (oligo);
  }
}

/**
 * Calculate the oligo usage frequency for each sequence in a fasta file.
 *
 * @param sequences An array of sequences to analyze.
 * @param numSequences The number of sequences.
 * @param oligoLength - The length of the oligos.
 * @param fragmentLength - The minimum length of sequences to use.
 * @param numCombinations - The number of possible oligo combinations.
 * @param frequency - The oligo frequency matrix to be generated.
 */
void oligoFrequency (
  Sequence ** sequences,
  size_t numSequences,
  size_t oligoLength,
  size_t fragmentLength,
  size_t numCombinations,
  double * frequency
) {
  size_t i, j, k, l;
  size_t numSamples;
  size_t numOligos;
  size_t stepSize;
  char ** oligonucleotides;
  char * sample;
  char * oligo;
  int r;
  /* Initialize the random number generator. */
  srand (time (NULL));
  /* Generate all of the nucleotide combinations. */
  oligonucleotides = malloc (numCombinations * sizeof (char *));
  generateOligonucleotides (oligoLength, oligonucleotides, "", 0);

  /* Calculate the number of oligonucleotides that will be tested. */
  numOligos = floor (fragmentLength / oligoLength);

/*
  #pragma omp parallel shared ( \
    sequences, numSequences, oligoLength, numCombinations, frequency, \
    oligonucleotides, numOligos \
  )
  #pragma omp for
*/

  /* Count the number of times each oligonucleotide appears in a sequence. */
  for (i = 0; i < numSequences; i ++) {
    /* Take samples from the sequence, and average the nucleotide usage of
       the samples. */
    numSamples = rint (
      (1.5 * sequences[i]->sequenceLength) / (1.0 * fragmentLength)
    );
    stepSize = rint (
      (sequences[i]->sequenceLength - fragmentLength) / (1.0 * numSamples)
    );
    for (j = 0; j < numSamples; j ++) {
      /* Take a random sample of a section of the sequence. */
      r = rand() % stepSize + j * stepSize;
      sample = strndup (sequences[i]->sequence + r, fragmentLength);
      for (k = 0; k < numOligos; k ++) {
        /* Compare the oligo generated from the sequence with each possible
           oligo. */
        oligo = strndup (sample + k * oligoLength, oligoLength);
        for (l = 0; l < numCombinations; l ++) {
          /* Increment the frequency counter if a match is found in the
             sequence. */
          if (sequenceIsEqual (oligo, oligonucleotides[l])) {
            #pragma omp critical
            frequency[i * numCombinations + l] ++;
          }
        }
        free (oligo);
      }
      free (sample);
    }
  }
  /* Normalize the frequency values based on the number of samples and
     length of the sequence. */
  for (i = 0; i < numSequences; i ++) {
    for (j = 0; j < numCombinations; j ++) {
      frequency[i * numCombinations + j] /= 
        numSamples * (fragmentLength - oligoLength + 1);
    }
  }
  /* Free the memory used by the oligonucleotides array. */
  for (i = 0; i < numCombinations; i ++) {
    free (oligonucleotides[i]);
  }
  free (oligonucleotides);
}

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
 * @file cluster.h
 */

#ifndef _OLIGO_CLUSTER_H
#define _OLIGO_CLUSTER_H

#include <stdio.h>

#include "vl/aib.h"
#include "vl/kmeans.h"

#include "sequence.h"
#include "newick.h"

/**
 * The structure to hold a Cluster object.
 * 
 * @public
 */
typedef struct Cluster {
  size_t size;                     /**< The size of the cluster. */
  char ** array;                   /**< An array of sequence identifiers. */
} Cluster;


/**
 * Creates a new Cluster object.
 *
 * @memberof Cluster
 * @public
 * @return The new Cluster object.
 */
extern Cluster * newCluster (void);

/**
 * Run the Kmeans algorithm provided by the VLFeat library.
 *
 * @param ids The sequence identifiers.
 * @param numSequences The number of sequences in the sequences array.
 * @param numCombinations The number of possible oligo combinations.
 * @param frequency The oligo frequency matrix.
 * @param numCenters The number of centers to search for.
 * @param debug Print debugging information with values > 0.
 */
extern void runKmeans (
  char ** ids,
  vl_uint32 numSequences,
  vl_uint32 numCombinations,
  double * frequency,
  vl_uint32 numCenters,
  vl_uint32 debug
);

/**
 * Run the Agglomerative Information Bottleneck (AIB) method provided
 * by the VLFeat library.
 *
 * @param ids The sequence identifiers.
 * @param numSequences The number of sequences in the sequences array.
 * @param numCombinations The number of possible oligo combinations.
 * @param frequency The oligo frequency matrix.
 * @param debug Print debugging information with values > 0.
 */
extern void runAIB (
  char ** ids,
  vl_uint32 numSequences,
  vl_uint32 numCombinations,
  double * frequency,
  vl_uint32 debug
);

#endif

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
 * @file cluster.c
 */

#include "cluster.h"

/**
 * Creates a new Cluster object.
 *
 * @memberof Cluster
 * @public
 * @return The new Cluster object.
 */
Cluster * newCluster (void) {
  Cluster * cluster;
  /* Allocate memory for the Cluster. */
  cluster = malloc (sizeof (Cluster));
  cluster->size = 0;
  cluster->array = malloc (sizeof (char*));
  return cluster;
}

//void addMember(Cluster cluster, char* identifier) {
//  cluster->array = realloc (cluster->array, (cluster->size + 1) * sizeof (char*));
//  cluster->array[cluster->size] = strdup (identifier);
//  cluster->size ++;
//}

/**
 * Run the Kmeans algorithm provided by the VLFeat library.
 *
 * @param sequences The sequences to run through the Kmeans algorithm.
 * @param numSequences The number of sequences in the sequences array.
 * @param numCombinations The number of possible oligo combinations.
 * @param frequency The oligo frequency matrix.
 * @param numCenters The number of centers to search for.
 * @param debug Print debugging information with values > 0.
 */
void runKmeans (
  char ** ids,
  vl_uint32 numSequences,
  vl_uint32 numCombinations,
  double * frequency,
  vl_uint32 numCenters,
  vl_uint32 debug
) {
  VlKMeans * kmeans;
  const double * centers;
  size_t i, j;

  kmeans = vl_kmeans_new (VL_TYPE_DOUBLE, VlDistanceL2);

  vl_kmeans_set_verbosity (kmeans, debug);
  vl_kmeans_set_max_num_iterations (kmeans, 100) ;
  vl_kmeans_set_algorithm (kmeans, VlKMeansLloyd);
//  vl_kmeans_set_algorithm (kmeans, VlKMeansANN);
//  vl_kmeans_set_algorithm (kmeans, VlKMeansElkan);


  // Initialize the centers.
  vl_kmeans_init_centers_plus_plus (
    kmeans, frequency, numCombinations, numSequences, numCenters
  );

  // Print the cluster centers if debug is on.
  if (debug > 0) {
    centers = vl_kmeans_get_centers (kmeans);
    for (i = 0; i < numCenters; i ++) {
      printf ("center %lu: ", i);
      for (j = 0; j < numCombinations; j ++) {
        printf ("%g ", centers[i + j * numCombinations]);
      }
      printf ("\n");
    }
  }

  // Refine the centers using Kmeans.
  vl_kmeans_refine_centers (kmeans, frequency, numSequences);

  // Print the cluster centers if debug is on.
  if (debug > 0) {
    centers = vl_kmeans_get_centers (kmeans);
    for (i = 0; i < numCenters; i ++) {
      printf("center %lu: ", i);
      for (j = 0; j < numCombinations; j ++) {
        printf ("%g ", centers[i + j * numCombinations]);
      }
      printf ("\n");
    }
  }

  vl_uint32 * assignments = vl_malloc (sizeof (vl_uint32) * numSequences);
  float * distances = vl_malloc (sizeof (double) * numSequences);
  vl_kmeans_quantize (
    kmeans, assignments, distances, frequency, numSequences
  );


/*  Cluster ** clusters;*/

/*  clusters = malloc (numCenters * sizeof (Cluster*));*/

/*  for (i = 0; i < numCenters; i ++) {*/
/*    clusters[i] = newCluster ();*/

/*    for (j = 0; j < numSequences; j ++) {*/


/*    }*/
/*  }*/
  

  for (i = 0; i < numSequences; i ++) {
    printf (
      "%23s: %d\t%f\n",
      ids[i], assignments[i], distances[i]
    );
  }


  free (assignments);
  free (distances);
  vl_kmeans_delete (kmeans);
}

/**
 * Run the Agglomerative Information Bottleneck (AIB) method provided
 * by the VLFeat library.
 *
 * @param sequences The sequences to run through the AIB algorithm.
 * @param numSequences The number of sequences in the sequences array.
 * @param numCombinations The number of possible oligo combinations.
 * @param frequency The oligo frequency matrix.
 * @param debug Print debugging information with values > 0.
 */
void runAIB (
  char ** ids,
  vl_uint32 numSequences,
  vl_uint32 numCombinations,
  double * frequency,
  vl_uint32 debug
)  {
  VlAIB * aib;
  size_t i, j;
  double * costs;
  vl_uint * parents;
  char * newick;
  /* Create a new AIB object. */
  aib = vl_aib_new (frequency, numSequences, numCombinations);
  /* Set the debug output level for the AIB algorithm. */
  vl_aib_set_verbosity (aib, debug);
  /* Run the AIB algorithm. */
  vl_aib_process (aib);
  /* Get the costs and parents vectors. */
  costs = vl_aib_get_costs (aib);
  parents = vl_aib_get_parents (aib);
  /* Display the costs and parents vectors if debug is on. */
  if (debug > 0) {
    printf ("Costs:\n");
    for (i = 0; i < numSequences; i ++) {
      printf ("%zu => %f\n", i, costs[i]);
    }
    printf ("Parents:\n");
    for (i = 0; i < 2 * numSequences - 1; i ++) {
      printf ("%zu => %d\n", i, parents[i]);
    }
  }
  /* Build a Newick tree from the parents vector. */
  Node ** nodes = malloc ((2 * numSequences - 1) * sizeof (Node *));
  Node * root = NULL;
  for (i = 0; i < 2 * numSequences - 1; i ++) {
    /* Create the new node. */
    nodes[i] = newNode();
    /* If the node being built for the current index was not the result of a
       merge, use the sequence identifier as the name. */
    if (i < numSequences) {
      setNodeName (nodes[i], ids[i]);
    }
  }


  /* Create relationships between parents and children. */
  for (i = 0; i < 2 * numSequences - 1; i ++) {
    if (parents[i] == 0) {
      root = nodes[i];
    }
    else {
      setNodeParent (nodes[i], nodes[parents[i]]);
      addNodeChild (nodes[parents[i]], nodes[i]);
    }
  }



  if (root == NULL) {
    printf ("Root node not found!\n");
    exit (1);
  }
  /* Set the difference in merge costs as the distance to each nodes
     parent. */
  for (i = numSequences; i < 2 * numSequences - 1; i ++) {
    for (j = 0; j < nodes[i]->numChildren; j ++) {
      setNodeDistance (
        nodes[i]->children[j],
        costs[i - numSequences] - costs[i - numSequences + 1]
      );
    }
  }
  newick = toString(root);
  printf ("newick:\n%s\n", newick);

  /* Free memory. */
  freeNode (root);
  free (nodes);
  free (newick);
  vl_aib_delete (aib);
}


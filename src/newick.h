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
 * Stores a tree in a linked node data structure, with output in newick format.
 *
 * @file newick.h
 */

#ifndef _OLIGO_NEWICK_H
#define _OLIGO_NEWICK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @def NEWICK_BUFFER_SIZE
 *   The size of the buffer.
 */
#define NEWICK_BUFFER_SIZE 65536

/**
 * The structure to hold a Node object.
 * 
 * @public
 */
typedef struct Node {
  char * name;              /**< The name of this Node. */
  double distance;          /**< The distance of this Node to its parent. */
  size_t numChildren;       /**< The number of children that this Node has. */
  struct Node * parent;     /**< A pointer to the parent of this Node. */
  struct Node ** children;  /**< An array containing the children of this Node. */
} Node;

/**
 * Creates a new Node object.
 *
 * @memberof Node
 * @public
 * @return The new Node object.
 */
extern Node * newNode (void);

/**
 * Changes the name of the provided Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param name The new name of the Node.
 */
extern void setNodeName (Node * node, char * name);

/**
 * Changes the distance of this Node from its parent.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param distance The distance from the parent Node.
 */
extern void setNodeDistance (Node * node, double distance);

/**
 * Changes the parent of this Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param parent The new parent of the Node.
 */
extern void setNodeParent (Node * node, Node * parent);

/**
 * Add a child to this Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param child The child Node to add.
 */
extern void addNodeChild (Node * node, Node * child);

/**
 * Returns true if this Node has no children, false if it has at least one child.
 *
 * @memberof Node
 * @public
 * @param node The Node object to check.
 * @return True if this Node has no children, false if it has at least one child.
 */
extern int isLeafNode (Node * node);

/**
 * Returns true if this Node has no parent, false if it does.
 *
 * @memberof Node
 * @public
 * @param node The Node object to check.
 * @return True if this Node has no parent, false if it does.
 */
extern int isRootNode (Node * node);

/**
 * Converts a the tree stored in the Node into Newick format.
 *
 * @memberof Node
 * @public
 * @param node The Node object to convert.
 * @return The Node and its children formatted in Newick format.
 */
extern char * toString (Node * node);

/**
 * Free the memory reserved for this Node object.
 *
 * @memberof Node
 * @public
 * @param node This Node object.
 */
extern void freeNode (Node * node);

#endif

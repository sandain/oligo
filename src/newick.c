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
 * @file newick.c
 */

#include "newick.h"

/**
 * Creates a new Node object.
 *
 * @memberof Node
 * @public
 * @return The new Node object.
 */
Node * newNode (void) {
  Node * node = malloc (sizeof(Node));
  node->name = malloc (sizeof(char));
  node->children = malloc (sizeof(Node *));
  node->parent = NULL;
  node->name[0] = '\0';
  node->distance = 0.0;
  node->numChildren = 0;
  return node;
};

/**
 * Changes the name of the provided Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param name The new name of the Node.
 */
void setNodeName (Node * node, char * name) {
  size_t length = strlen (name);
  node->name = realloc (node->name, (length + 1) * sizeof (char));
  strcpy (node->name, name);
  node->name[length] = '\0';
}

/**
 * Changes the distance of this Node from its parent.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param distance The distance from the parent Node.
 */
void setNodeDistance (Node * node, double distance) {
  node->distance = distance;
}

/**
 * Changes the parent of this Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param parent The new parent of the Node.
 */
void setNodeParent (Node * node, Node * parent) {
  node->parent = parent;
}

/**
 * Add a child to this Node.
 *
 * @memberof Node
 * @public
 * @param node The Node object to change.
 * @param child The child Node to add.
 */
void addNodeChild (Node * node, Node * child) {
  /* Allocate memory for the child. */
  if (node->numChildren > 0) {
    node->children = realloc (node->children, (node->numChildren + 1) * sizeof (Node *));
  }
  /* Add the child to this node. */
  node->children[node->numChildren] = child;
  node->numChildren ++;
}

/**
 * Returns true if this Node has no children, false if it has at least one child.
 *
 * @memberof Node
 * @public
 * @param node The Node object to check.
 * @return True if this Node has no children, false if it has at least one child.
 */
int isLeafNode (Node * node) {
  return node->numChildren == 0;
}

/**
 * Returns true if this Node has no parent, false if it does.
 *
 * @memberof Node
 * @public
 * @param node The Node object to check.
 * @return True if this Node has no parent, false if it does.
 */
int isRootNode (Node * node) {
  return node->parent == NULL;
}

/**
 * Converts a the tree stored in the Node into Newick format.
 *
 * @memberof Node
 * @public
 * @param node The Node object to convert.
 * @return The Node and its children formatted in Newick format.
 */
char * toString (Node * node) {
  char * newick;
  char * children;
  char * distance;
  char * buffer = malloc (NEWICK_BUFFER_SIZE * sizeof (char));
  /* Convert the distance attribute of this node to a string. */
  sprintf (buffer, "%f", node->distance);
  distance = strdup (buffer);
  /* Erase the buffer. */
  buffer[0] = '\0';
  /* Process this nodes children. */
  if (node->numChildren > 0) {
    size_t i;
    for (i = 0; i < node->numChildren; i++) {
      if (i != 0) {
        buffer = strcat (buffer, ",");
      }
      /* Process this child's subtree. */
      char * childTree = toString (node->children[i]);
      buffer = strcat (buffer, childTree);
      free (childTree);
    }
    children = malloc ((strlen (buffer) + 3) * sizeof (char));
    sprintf (children, "(%s)", buffer);
  }
  else {
    /* Node has no children, fill with an empty string (copy the empty buffer). */
    children = strdup (buffer);
  }
  /* Append a semi-colon to the end of the string if this is the root node. */
  size_t newickLength = strlen (children) + strlen (node->name) + strlen (distance);
  char * isRoot;
  if (isRootNode (node)) {
    isRoot = ";";
    newickLength ++;
  }
  else {
    isRoot = "";
  }
  /* Allocate memory for the Newick formatted tree. */
  newick = malloc ((newickLength + strlen (isRoot) + 2) * sizeof (char));
  sprintf (newick, "%s%s:%s%s", children, node->name, distance, isRoot);
  /* Free memory used by the buffer, children and distance strings. */
  free (buffer);
  free (children);
  free (distance);
  return newick;
}

/**
 * Free the memory reserved for this Node object.
 *
 * @memberof Node
 * @public
 * @param node This Node object.
 */
void freeNode (Node * node) {
  size_t i;
  for (i = 0; i < node->numChildren; i ++) {
      freeNode (node->children[i]);
  }
  free (node->name);
  free (node->children);
  free (node);
}

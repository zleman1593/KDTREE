/* 

Laura Toma, based on code by John Visentin

*/

#include "kdtree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>




/* returns the root node of the given tree */
treeNode* kdtree_getRoot(kdtree *tree) {

  assert(tree); 
  return tree->root; 
}

/* returns the point stored by the node */
point2D treeNode_getPoint(treeNode *node) {

  assert(node); 
  return node->p; 
}





/* create a new empty tree */
kdtree* kdtree_init() {
  
  kdtree* tree = new kdtree();
  assert(tree); 

  tree->root = NULL; 
  tree->count = tree->height = 0; 
  return tree; 
}

/* private function to recursively print a node and its subtree in
   order */
static void treeNode_print(treeNode * node) {

  if (node == NULL) return; 

  //if we are here, node must be valid

  //recursively print left child 
  treeNode_print(node->left); 

  //print node 
  switch (node->type) {
  case 'h':
    printf("HORIZONTAL: (y=%d)\n", node->p.y);
    break; 
  case 'v':
    printf("VERTICAL: (x=%d)\n", node->p.x); 
    break; 
  case 'l': 
    printf("LEAF: (p=(%d,%d))\n", node->p.x, node->p.y); 
    break;
  default: 
    printf("Improper tree node type\n"); 
    exit(1); 
  };

  //recursively print right child
  treeNode_print(node->right); 

} 


/* print out information about the tree including height, number of
 nodes, and each node in an in-order traversal */
void kdtree_print(kdtree *tree) {

  if (tree) {
    printf("--- kdtree Info ---\n");
    printf("Height: %lu, Node Count: %lu\n", tree->height, tree->count);
    printf("Nodes in order:\n\n");
    treeNode_print(tree->root);
    printf("---------------------\n");
  }
}

//private function to recursively free the subtree rooted at node
static void treeNode_free(treeNode* node) {
  //fill in 

}


/* free all memory allocated for the tree, including the tree
   itself */
void kdtree_free(kdtree *tree) {

  if (!tree) return; 
  treeNode_free(tree->root); 
  free(tree); 
}




/* create a new tree representing the given array of points */
kdtree* kdtree_build(point2D *points, int n) {

  //fill in

  return NULL;
}


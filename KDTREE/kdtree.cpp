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


struct xCompare
{
    inline bool operator() (const point2D& struct1, const point2D& struct2)
    {
        return (struct1.x < struct2.x);
    }
};

struct yCompare
{
    inline bool operator() (const point2D& struct1, const point2D& struct2)
    {
        return (struct1.y < struct2.y);
    }
};



kdtree* kdtree_build_rec(std::vector<point2D> xSortedPointsVector, std::vector<point2D> ySortedPointsVector,bounds xAndYBounds, int depth){
    
    //If are contains only one point
    if (xSortedPointsVector.size() == 1){
         treeNode leafNode = treeNode();
        leafNode.type = 'l';
        leafNode.nodeBounds = xAndYBounds;
        leafNode.p = xSortedPointsVector[0];
        kdtree* tree = kdtree_init();
        tree->count = 1;
        tree->height = 1;
        tree->root = &leafNode;
        return tree;
    } else if (depth % 2 == 0){
       
        
        
    } else{
        
        
    }
    
    
    kdtree* VLeft = kdtree_build_rec();
    kdtree* VRight = kdtree_build_rec();
    
    treeNode node = treeNode();
    
    if (depth % 2 == 0) {
          node.type = 'h';
    } else{
        node.type = 'v';
    }
    //node.p = ; ----------------------------------------------------- PUT MEDIAN POINT HERE
    node.nodeBounds = xAndYBounds;
    node.left = VLeft->root;
    node.right = VRight->root;
    
    kdtree* tree = kdtree_init();
    tree->count = VLeft->count + VRight->count;
    tree->height = std::max(VLeft->height, VRight->height) + 1;
    tree->root = &node;
    return tree;
    
}

/* create a new tree representing the given array of points */
kdtree* kdtree_build(std::vector<point2D> points) {
    // Create a vector sorted by x
    std::vector<point2D>  xSortedPointsVector(points);
    //Sort asscending
    std::sort(xSortedPointsVector.begin(),xSortedPointsVector.end(),xCompare());
    //Create a vecotr sorted by y
    std::vector<point2D>  ySortedPointsVector(points);
    //Sort asscending
    std::sort(ySortedPointsVector.begin(),ySortedPointsVector.end(),yCompare());
    //Start Recursion
    bounds boundingBox = bounds();
    boundingBox.x_lower = xSortedPointsVector.at(0).x;
    boundingBox.x_upper = xSortedPointsVector.at(xSortedPointsVector.size() - 1).x;
    boundingBox.y_lower = ySortedPointsVector.at(0).y;
    boundingBox.y_upper = ySortedPointsVector.at(ySortedPointsVector.size() - 1).y;
    return kdtree_build_rec(xSortedPointsVector,ySortedPointsVector,boundingBox,1);

}





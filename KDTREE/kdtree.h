
/* 
   LT based on code from John Visentin 
*/ 

#ifndef __kdtree_h
#define __kdtree_h


#include "geom.h"
#include <stddef.h> //for size_t
#include <vector>
#include <map>
#include <string>
#include <sstream>

/* global variables */
const int WINDOWSIZE = 500;
const int POINT_SIZE  = 6.0f;

typedef struct _treeNode treeNode;

//Struct that holds the bounds that will allow
//the lines to terminate at the correct locations
typedef struct _bounds {
    int x_upper;
    int x_lower;
    int y_upper;
    int y_lower;
} bounds;

struct _treeNode {
    
 bounds  nodeBounds;//Bounds fare used to draw the line for a nonleaf node
    
  point2D p; /* If this is a leaf node, p represents the point
		stored in this leaf.  If this is not a leaf node, p
		represents the horizontal or vertical line stored in
		this node. For a vertical line, p.y is ignored. For
		a horizontal line, p.x is ignored
	     */
  char type; /* this can be 'h' (horizontal) or 'v' (vertical), or
		'l' (leaf) depending whether the node splits with
		a horizontal line or vertical line.
	     */
  treeNode  *left, *right; /* left/below and right/above children. */
  
};


typedef struct _kdtree {
  treeNode *root;  //root of the tree
  size_t count; //number of nodes in the tree
  size_t height; //height of the tree
} kdtree;



/* create a new empty tree */
kdtree* kdtree_init();

/* create a new tree representing the given array of points */
kdtree* kdtree_build(std::vector<point2D> points);

//Main recursive method
kdtree* kdtree_build_rec(std::vector<point2D> xSortedPointsVector, std::vector<point2D> ySortedPointsVector,bounds xAndYBounds, int depth, point2D recursionLimit);

/* Free memory for the tree itself and resets tree */
void kdtree_free(kdtree *tree);

/* print out information about the tree including height, number of
 nodes, and each node in an in-order traversal */
void kdtree_print(kdtree *tree);

#endif

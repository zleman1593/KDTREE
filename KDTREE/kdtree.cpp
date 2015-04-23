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
    
    
    
    kdtree* VLeft;
    kdtree* VRight;
    bounds xAndYBoundsForLeft;//Bottom
    bounds xAndYBoundsRight;//Top
    point2D median;
    
    
    //If are contains only one point
    if (xSortedPointsVector.size() == 1){
        treeNode* leafNode = new treeNode();
        leafNode->type = 'l';
        leafNode->nodeBounds = xAndYBounds;
        leafNode->p = xSortedPointsVector[0];
        kdtree* tree = kdtree_init();
        tree->count = 1;
        tree->height = 1;
        tree->root = leafNode;
        return tree;
    } else if (depth % 2 == 0){
        
        int numberOfPoints = xSortedPointsVector.size();
        int medianIndex = numberOfPoints/2;
        median = xSortedPointsVector.at(medianIndex);
        int medianXValue = median.x;
        int i;
        
        //iterate over all values that have
        for (i = medianIndex; xSortedPointsVector.at(i).x == medianXValue; i--){
            if (i == 0) {
                i--;// Causes error
                break;
            }
        }
        
        //Set pointer to the leftmost value that has the same x value as the median
        i = i + 1;
        
        //Update Median so line will draw through proper point
        median = xSortedPointsVector.at(i);
        
        //Get all points in sorted order that will be part of the left part
        std::vector<point2D>  xSortedPointsVectorForLeft(xSortedPointsVector.begin(),xSortedPointsVector.begin() + i);
        //Copy all thesw points over to the array that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForLeft(xSortedPointsVectorForLeft);
        //Sort the y array by y values
        std::sort(ySortedPointsVectorForLeft.begin(),ySortedPointsVectorForLeft.end(),yCompare());
        
        
        
        //Get all points in sorted order that will be part of the right part
        std::vector<point2D>  xSortedPointsVectorForRight(xSortedPointsVector.begin() + i,xSortedPointsVector.end());
        //Copy all thesw points over to the array that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForRight(xSortedPointsVectorForRight);
        //Sort the y array by y values
        std::sort(ySortedPointsVectorForRight.begin(),ySortedPointsVectorForRight.end(),yCompare());
        
        
        //BOUNDS
        
        xAndYBoundsForLeft = xAndYBounds;
        xAndYBoundsRight = xAndYBounds;
        //Shoudl these be set to the same number?   ok?
        xAndYBoundsForLeft.x_upper = medianXValue;
        xAndYBoundsRight.x_lower = medianXValue;
        
        if (xSortedPointsVectorForLeft.size() != 0) {
            VLeft = kdtree_build_rec(xSortedPointsVectorForLeft,ySortedPointsVectorForLeft,xAndYBoundsForLeft,depth + 1);
        }else{
            VLeft = NULL;
        }
        if(xSortedPointsVectorForRight.size() != 0){
            VRight = kdtree_build_rec(xSortedPointsVectorForRight,ySortedPointsVectorForRight,xAndYBoundsRight, depth + 1);
        } else{
            VRight = NULL;
        }
        
    } else{
        
        int numberOfPoints = ySortedPointsVector.size();
        int medianIndex = numberOfPoints/2;
        median = ySortedPointsVector.at(medianIndex);
        int medianYValue = median.y;
        int i;
        
        //iterate over all values that have
        for (i = medianIndex; ySortedPointsVector.at(i).y == medianYValue; i--){
            if (i == 0) {
                i--;// Causes error
                break;
            }
        }
        //Set pointer to the leftmost value that has the same y value as the median
        i = i + 1;
        
        //Update Median so line will draw through proper point
        median = ySortedPointsVector.at(i);
        
        
        //Get all points in sorted order that will be part of the left part
        std::vector<point2D>  ySortedPointsVectorForLeft(ySortedPointsVector.begin(),ySortedPointsVector.begin() + i);
        //Cop all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForLeft(ySortedPointsVectorForLeft);
        //Sort the x arrax bx x values
        std::sort(xSortedPointsVectorForLeft.begin(),xSortedPointsVectorForLeft.end(),xCompare());
        
        
        
        //Get all points in sorted order that will be part of the right part
        std::vector<point2D>  ySortedPointsVectorForRight(ySortedPointsVector.begin() + i,ySortedPointsVector.end());
        //Copx all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForRight(ySortedPointsVectorForRight);
        //Sort the x arrax bx x values
        std::sort(xSortedPointsVectorForRight.begin(),xSortedPointsVectorForRight.end(),xCompare());
        
        
        //BOUNDS
        
        xAndYBoundsForLeft = xAndYBounds;
        xAndYBoundsRight = xAndYBounds;
        //Shoudl these be set to the same number?   ok?
        xAndYBoundsForLeft.y_upper = medianYValue;
        xAndYBoundsRight.y_lower = medianYValue;
        
        if(xSortedPointsVectorForLeft.size() != 0){
            VLeft = kdtree_build_rec(ySortedPointsVectorForLeft,xSortedPointsVectorForLeft,xAndYBoundsForLeft,depth + 1);
        }else{
            VLeft = NULL;
        }
        if(xSortedPointsVectorForRight.size() != 0){
            VRight = kdtree_build_rec(ySortedPointsVectorForRight,xSortedPointsVectorForRight,xAndYBoundsRight, depth + 1);
        }else{
            VLeft = NULL;
        }
        
        
    }
    
    
    
    
    treeNode* node = new treeNode();
    
    if (depth % 2 == 0) {
        node->type = 'v';
    } else{
        node->type = 'h';
    }
    node->p = median; //----------------------------------------------------- PUT MEDIAN POINT HERE
    node->nodeBounds = xAndYBounds;
    if(VLeft != NULL){
        node->left = VLeft->root;
    } else {
        node->left  = NULL;
    }
    if(VLeft != NULL){
        node->right = VRight->root;
    } else {
        node->right  = NULL;
    }
    
    kdtree* tree = kdtree_init();
    tree->count = VLeft->count + VRight->count;
    tree->height = std::max(VLeft->height, VRight->height) + 1;
    tree->root = node;
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
    boundingBox.x_lower = 0;
    boundingBox.x_upper = WINDOWSIZE;
    boundingBox.y_lower = 0;
    boundingBox.y_upper = WINDOWSIZE;
    return kdtree_build_rec(xSortedPointsVector,ySortedPointsVector,boundingBox,1);
    
}





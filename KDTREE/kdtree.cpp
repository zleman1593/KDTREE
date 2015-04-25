/*
 
 Laura Toma, based on code by John Visentin
 
 */

#include "kdtree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>



/* create a new empty tree */
kdtree* kdtree_init() {
    
    kdtree* tree = new kdtree();
    assert(tree);
    
    tree->root = NULL;
    tree->count = tree->height = 0;
    return tree;
}

/* Private function to recursively print a node and its subtree in
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
    
    
}


/* free all memory allocated for the tree, including the tree
 itself */
void kdtree_free(kdtree *tree) {
    
    if (!tree) return;
    treeNode_free(tree->root);
    free(tree);
}

//Custom Comparators
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


/* Recursively called method to create a new treefor the points */
kdtree* kdtree_build_rec(std::vector<point2D> xSortedPointsVector, std::vector<point2D> ySortedPointsVector,bounds xAndYBounds, int depth, point2D recursionLimit){
    
    //BASE CASE: If area contains no points return NULL to stop recursion
    if (xSortedPointsVector.size() == 0){
        return NULL;
    }
    
    kdtree* VLeft;//Left Subtree
    kdtree* VRight;//Right Subtree
    bounds xAndYBoundsForLeft = xAndYBounds;//Bottom
    bounds xAndYBoundsRight = xAndYBounds;//Top
    point2D median;//Median Point for the split
    
    int numberOfPoints = ySortedPointsVector.size();
    int medianIndex = numberOfPoints/2;
    
    //BASE CASE: If area contains only one point
    if (xSortedPointsVector.size() == 1){
        //Create a noe for this point
        treeNode* leafNode = new treeNode();
        leafNode->type = 'l';
        leafNode->nodeBounds = xAndYBounds;
        leafNode->p = xSortedPointsVector[0];
        //Create a tree "wrapper" for the node so it can be passed back
        kdtree* tree = kdtree_init();
        tree->count = 1;
        tree->height = 1;
        tree->root = leafNode;
        return tree;
        
    } else if (depth % 2 == 0){//Use depth to determine to split by X or Y
        //Split By X
        median = xSortedPointsVector.at(medianIndex);
        int medianXValue = median.x;
        int i;
        
        //iterate over all values that have the same X coord
        for (i = medianIndex; xSortedPointsVector.at(i).x == medianXValue; i--){
            if (i == 0) {
                i--;
                break;
            }
        }
        //Set pointer to the leftmost value that has the same x value as the median
        //Because for loop will leave i at one position below coorect value
        i = i + 1;
        
        //Update Median so line will draw through proper point
        median = xSortedPointsVector.at(i);
        
//        
//        //Check if the case that causes infinite recursion is present
//      
//            //Update Median so line will draw through proper point
//            median = xSortedPointsVector.at(2);
//            medianXValue = median.x;
//            i = 2;
        
        
        //Get all points in sorted order that are to the left . This includes the point along the median line.
        std::vector<point2D>  xSortedPointsVectorForLeft(xSortedPointsVector.begin(),xSortedPointsVector.begin() + i);
        //Copy all these points over to the array that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForLeft(xSortedPointsVectorForLeft);
        //Sort the y vector by y values
        std::sort(ySortedPointsVectorForLeft.begin(),ySortedPointsVectorForLeft.end(),yCompare());
        
        
        
        //Get all points in sorted order that are to the right . This excludes the point along the median line.
        std::vector<point2D>  xSortedPointsVectorForRight(xSortedPointsVector.begin() + i,xSortedPointsVector.end());
        //Copy all these points over to the vector that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForRight(xSortedPointsVectorForRight);
        //Sort the y vector by y values
        std::sort(ySortedPointsVectorForRight.begin(),ySortedPointsVectorForRight.end(),yCompare());
        
        
        //BOUNDS
        xAndYBoundsForLeft.x_upper = medianXValue;
        xAndYBoundsRight.x_lower = medianXValue;
        
       
        //Indicate how many points are being passed to each side for recursion
        point2D* limit = new point2D;
        limit->x = xSortedPointsVectorForLeft.size();
        limit->y = xSortedPointsVectorForRight.size();
        
         //Recursive Checks prevents infinite recursion when all points are in an L shape along those two intersecting lines
        if (recursionLimit.x == 0 || recursionLimit.y == 0 ) {
            if (recursionLimit.x == limit->x && recursionLimit.y == limit->y ) {
                return NULL;
            }
        }
        
        //Recursive Calls
        VLeft =  kdtree_build_rec(xSortedPointsVectorForLeft,ySortedPointsVectorForLeft,xAndYBoundsForLeft,depth + 1, *limit);
        VRight = kdtree_build_rec(xSortedPointsVectorForRight,ySortedPointsVectorForRight,xAndYBoundsRight, depth + 1, *limit);
        
        
    } else{
        
        median = ySortedPointsVector.at(medianIndex);
        int medianYValue = median.y;
        int i;
        
        //Set pointer to the leftmost value that has the same x value as the median
        //Because for loop will leave i at one position below coorect value
        for (i = medianIndex; ySortedPointsVector.at(i).y == medianYValue; i--){
            if (i == 0) {
                i--;
                break;
            }
        }
        //Set pointer to the leftmost value that has the same y value as the median
        i = i + 1;
        
        //Update Median so line will draw through proper point
        median = ySortedPointsVector.at(i);
        
//        //Check if the case that causes infinite recursion is present
//      
//            //Update Median so line will draw through proper point
//            median = ySortedPointsVector.at(2);
//            i = 2;
//            medianYValue = median.y;
//        }
        
        //Get all points in sorted order that are to the left . This includes the point along the median line.
        std::vector<point2D>  ySortedPointsVectorForLeft(ySortedPointsVector.begin(),ySortedPointsVector.begin() + i);
        //Cop all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForLeft(ySortedPointsVectorForLeft);
        //Sort the x arrax bx x values
        std::sort(xSortedPointsVectorForLeft.begin(),xSortedPointsVectorForLeft.end(),xCompare());
        
        
        
        //Get all points in sorted order that are to the right . This excludes the point along the median line.
        std::vector<point2D>  ySortedPointsVectorForRight(ySortedPointsVector.begin() + i,ySortedPointsVector.end());
        //Copx all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForRight(ySortedPointsVectorForRight);
        //Sort the x arrax bx x values
        std::sort(xSortedPointsVectorForRight.begin(),xSortedPointsVectorForRight.end(),xCompare());
        
        
        //BOUNDS
        xAndYBoundsForLeft.y_upper = medianYValue;
        xAndYBoundsRight.y_lower = medianYValue;
        
        
        //Indicate how many points are being passed to each side for recursion
        point2D* limit = new point2D;
        limit->x = xSortedPointsVectorForLeft.size();
        limit->y = xSortedPointsVectorForRight.size();
        
        //Recursive Checks prevents infinite recursion when all points are in an L shape along those two intersecting lines
        if (recursionLimit.x == 0 || recursionLimit.y == 0 ) {
            if (recursionLimit.x == limit->x && recursionLimit.y == limit->y ) {
                return NULL;
            }
        }
        
        //Recursive Calls
        VLeft = kdtree_build_rec(xSortedPointsVectorForLeft, ySortedPointsVectorForLeft,xAndYBoundsForLeft,depth + 1,*limit);
        VRight = kdtree_build_rec(xSortedPointsVectorForRight, ySortedPointsVectorForRight,xAndYBoundsRight, depth + 1,*limit);
        
    }
    
    
    
    //Create a node for this median split
    treeNode* node = new treeNode();
    
    //Determine type of point
    depth % 2 == 0 ? node->type = 'v' : node->type = 'h';
    //Assign median point to node to be used as split info
    node->p = median;
    //Add the bouding box for this split
    node->nodeBounds = xAndYBounds;
    
    //Attach subtrees
    node->left =  VLeft != NULL ? VLeft->root : NULL;
    
    node->right = VRight != NULL ? VRight->root :  NULL;
    
    //Create tree to be returned
    kdtree* tree = kdtree_init();
    
    //Calculate the number of nodes in tree
    tree->count = (VLeft != NULL ? VLeft->count : 0 ) + (VRight != NULL ? VRight->count : 0 );
    //Calculate the height of the tree
    tree->height = std::max((VLeft != NULL ? VLeft->height : 0 ), (VRight != NULL ? VRight->height : 0 )) + 1;
    //Assign node as the root of the tree
    tree->root = node;
    kdtree_free(VRight);
    kdtree_free(VLeft);
    
    return tree;
    
}

/*Create a new tree representing the given array of points */
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
    
    //Prevents infinite recursion when all points are in an L shape along those two intersecting lines
    //Reuse Point2D to hold the information needed to determine this. Not actually a point.
    point2D* recursionLimit = new point2D;
    recursionLimit->x = 1;
    recursionLimit->y = 1;
    
    //Return Tree
    return kdtree_build_rec(xSortedPointsVector,ySortedPointsVector,boundingBox,1,*recursionLimit);
    
}



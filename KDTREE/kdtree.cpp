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



point2D split(std::vector<point2D> xSortedPointsVector, std::vector<point2D> ySortedPointsVector,bounds xAndYBounds, point2D infiniteRecursionCheck, kdtree** VLeft, kdtree** VRight,int depth, bool vertLine){
    std::vector<point2D>  ySortedPointsVectorForLeft;
    //To prevent infinite recursion
    point2D* recursionCheck = new point2D;
    
    bounds xAndYBoundsForLeft = xAndYBounds;//Bottom
    bounds xAndYBoundsRight = xAndYBounds;//Top
    
    int numberOfPoints = ySortedPointsVector.size();
    int medianIndex = numberOfPoints/2;
    point2D median;
    
    median = vertLine ? xSortedPointsVector.at(medianIndex): ySortedPointsVector.at(medianIndex);//Median Point for the split
    
    int medianCoordValue = vertLine ? median.x : median.y;
    
    int i;
    
    //Set pointer to the leftmost value that has the same x value as the median
    //Because for loop will leave i at one position below coorect value
    for (i = medianIndex;  (vertLine ? xSortedPointsVector.at(i).x : ySortedPointsVector.at(i).y) == medianCoordValue; i--){
        if (i == 0) {
            i--;
            break;
        }
    }
    //Set pointer to the leftmost value that has the same y value as the median
    i = i + 1;
    
    
    //Update Median so line will draw through proper point
    median = vertLine ? xSortedPointsVector.at(medianIndex): ySortedPointsVector.at(medianIndex);//Median Point for the split
    
    do {
        std::vector<point2D>  ySortedPointsVectorForLeft;
        std::vector<point2D>  xSortedPointsVectorForLeft;
        std::vector<point2D>  ySortedPointsVectorForRight;
        std::vector<point2D>  xSortedPointsVectorForRight;
        
        //addPointsToSplitPartts(xSortedPointsVector,ySortedPointsVector,i);
        
        
        if(!vertLine) {
        //For Horizontal Line Split
        
        //Get all points in sorted order that are to the left . This includes the point along the median line.
        std::vector<point2D>  ySortedPointsVectorForLeft2(ySortedPointsVector.begin(),ySortedPointsVector.begin() + i);
        //Cop all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForLeft2(ySortedPointsVectorForLeft2);
        //Sort the x arrax bx x values
     
        
        
        
        //Get all points in sorted order that are to the right . This excludes the point along the median line.
        std::vector<point2D>  ySortedPointsVectorForRight2(ySortedPointsVector.begin() + i,ySortedPointsVector.end());
        //Copx all thesw points over to the arrax that will be sorted bx x
        std::vector<point2D>  xSortedPointsVectorForRight2(ySortedPointsVectorForRight2);
        //Sort the x arrax bx x values
            ySortedPointsVectorForLeft = ySortedPointsVectorForLeft2;
            ySortedPointsVectorForRight = ySortedPointsVectorForRight2;
            xSortedPointsVectorForLeft = xSortedPointsVectorForLeft2;
            xSortedPointsVectorForRight = xSortedPointsVectorForRight2;
            
            
        }else{
        
        //For Veritical Line Split
        
        
        //Get all points in sorted order that are to the left . This includes the point along the median line.
        std::vector<point2D>  xSortedPointsVectorForLeft2(xSortedPointsVector.begin(),xSortedPointsVector.begin() + i);
        //Copy all these points over to the array that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForLeft2(xSortedPointsVectorForLeft2);
        //Sort the y vector by y values
     
        
        
        
        //Get all points in sorted order that are to the right . This excludes the point along the median line.
        std::vector<point2D>  xSortedPointsVectorForRight2(xSortedPointsVector.begin() + i,xSortedPointsVector.end());
        //Copy all these points over to the vector that will be sorted by y
        std::vector<point2D>  ySortedPointsVectorForRight2(xSortedPointsVectorForRight2);
        //Sort the y vector by y values
            
            ySortedPointsVectorForLeft = ySortedPointsVectorForLeft2;
            ySortedPointsVectorForRight = ySortedPointsVectorForRight2;
            xSortedPointsVectorForLeft = xSortedPointsVectorForLeft2;
            xSortedPointsVectorForRight = xSortedPointsVectorForRight2;
        
        }
        
        if(vertLine){
            std::sort(ySortedPointsVectorForLeft.begin(),ySortedPointsVectorForLeft.end(),yCompare());
            std::sort(ySortedPointsVectorForRight.begin(),ySortedPointsVectorForRight.end(),yCompare());
        }else{
            std::sort(xSortedPointsVectorForLeft.begin(),xSortedPointsVectorForLeft.end(),xCompare());
            std::sort(xSortedPointsVectorForRight.begin(),xSortedPointsVectorForRight.end(),xCompare());
        }
        
        
        //BOUNDS
        if (vertLine){
            xAndYBoundsForLeft.x_upper = medianCoordValue;
            xAndYBoundsRight.x_lower = medianCoordValue;
        }else{
            xAndYBoundsForLeft.y_upper = medianCoordValue;
            xAndYBoundsRight.y_lower = medianCoordValue;
        }
        
        
        //Indicate how many points are being passed to each side for recursion
        recursionCheck->x = xSortedPointsVectorForLeft.size();
        recursionCheck->y = xSortedPointsVectorForRight.size();
        
        if (!(infiniteRecursionCheck.x == recursionCheck->x && infiniteRecursionCheck.y == recursionCheck->y) ) {
            //Recursive Calls
            *VLeft = kdtree_build_rec(xSortedPointsVectorForLeft, ySortedPointsVectorForLeft,xAndYBoundsForLeft,depth + 1,*recursionCheck);
            *VRight = kdtree_build_rec(xSortedPointsVectorForRight, ySortedPointsVectorForRight,xAndYBoundsRight, depth + 1,*recursionCheck);
        }else{
            //Prepare for second pass through while loop
            //Update Median so line will draw through proper point
            int index;
            if(vertLine){
                for (index = 0;  xSortedPointsVector.at(index).x <= xSortedPointsVector.at(0).x; index++) {}
                
            }else{
                for (index = 0;  ySortedPointsVector.at(index).y <= ySortedPointsVector.at(0).y; index++) {}
            }
            
            median = vertLine ? xSortedPointsVector.at(medianIndex): ySortedPointsVector.at(medianIndex);
            i = index;
          
            medianCoordValue = vertLine ? median.x : median.y;
            
            
        }
        
    } while(infiniteRecursionCheck.x == recursionCheck->x && infiniteRecursionCheck.y == recursionCheck->y);
    //Recursive Checks prevents infinite recursion when all points are in an L shape along those two intersecting lines
    
    return median;
}



/* Recursively called method to create a new tree for the points */
kdtree* kdtree_build_rec(std::vector<point2D> xSortedPointsVector, std::vector<point2D> ySortedPointsVector,bounds xAndYBounds, int depth, point2D infiniteRecursionCheck){
    
    //BASE CASE: If area contains no points return NULL to stop recursion
    if (xSortedPointsVector.size() == 0){
        return NULL;
    }
    
    kdtree* VLeft;//Left Subtree
    kdtree* VRight;//Right Subtree
    
    point2D median;//Median Point for split
    
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
        
        //Use depth to determine to split by X or Y
    } else if (depth % 2 == 0){
        //Returns Median point and gets VRight and VLeft
        //splitWithVerticalLine
        median = split(xSortedPointsVector, ySortedPointsVector, xAndYBounds, infiniteRecursionCheck, &VLeft, &VRight,depth,false);
        
    } else{
        //Returns Median point and gets VRight and VLeft
        //splitWithHorizontalLine
        median = split(xSortedPointsVector, ySortedPointsVector, xAndYBounds, infiniteRecursionCheck, &VLeft, &VRight,depth,false);
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
    point2D* infiniteRecursionCheck = new point2D;
    infiniteRecursionCheck->x = 1;
    infiniteRecursionCheck->y = 1;
    
    //Return Tree
    return kdtree_build_rec(xSortedPointsVector,ySortedPointsVector,boundingBox,1,*infiniteRecursionCheck);
    
}



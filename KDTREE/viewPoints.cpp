/*
 Base code From: Laura Toma, Rob Visentin
 Complete KD-Tree Algorithm and custom drawing By Zackery Leman and Ivy Xing
 
 What it does (See Readme for more details):
 
 - generates a set of random points in 2D, builds a kd-tree on it and
 renders it in 2D with default orthogonal projection.
 
 - program is run as:  ./viewpoints <nbpoints>
 
 - when the user presses the space bar, it regenarates the point set of same size
 
 */
#include <iostream>
#include <vector>
#include "rtimer.h"
#include "kdtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <map>
#include <string>
#include <sstream>
#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h> // Header File For The OpenGL32 Library
#include <OpenGL/glu.h> // Header File For The GLu32 Library
#include <GLUT/glut.h> // Header File For The GLut Library


//Colors
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};




/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);

//Draw points or not
bool drawPoints = false;

//The array of n points
int n;
std::vector<point2D> points;

//Initial random case
int caseToUse = 0;

// The kd-tree created with the points
kdtree *tree = NULL;


/*Method to remove all identical points. Done in linear time with hashmap*/
int remove_coincident_points() {
    std::map<std::string, std::string> duplicateMap;
    std::vector<point2D> nonDuplicatedPoints;
    
    for (int i=0; i<points.size(); i++) {
        point2D temp = points.at(i);
        std::ostringstream oss;
        oss << temp.x << " " << temp.y;
        std::string tempString = oss.str();
        
        // check if key is present
        if (duplicateMap.find(tempString) != duplicateMap.end()){
         //Map Already Contains Point
           // std::cout << "map already contains the point!\n";
        } else{
            duplicateMap[tempString] = "";
            nonDuplicatedPoints.push_back(temp);
            //Add element to new vector
        }
    }
    
    points = nonDuplicatedPoints;
    return nonDuplicatedPoints.size();
}


/* Initialize the vector of points stored in global variable points with random points */
void initialize_points_random() {
    
    for (int i=0; i<n; i++) {
        point2D point = point2D();
        point.x = (int)(.1*WINDOWSIZE)/2 + rand() % ((int)(.9*WINDOWSIZE));
        point.y =  (int)(.1*WINDOWSIZE)/2 + rand() % ((int)(.9*WINDOWSIZE));
        points.push_back(point);
    }
    n = remove_coincident_points();
}

/* Initialize the vector of points stored in global variable points with certain points for oth test case */
void initialize_points_case1() {
    
    point2D pointA = point2D();
    pointA.x = 100;
    pointA.y = 400;
    points.push_back(pointA);
    point2D pointB = point2D();
    pointB.x = 370;
    pointB.y = 400;
    points.push_back(pointB);
    point2D pointC = point2D();
    pointC.x = 20;
    pointC.y = 400;
    points.push_back(pointC);
    
    n = remove_coincident_points();
}

/* Initialize the vector of points stored in global variable points with certain points for first test case */
void initialize_points_case2() {
    
    point2D pointA = point2D();
    pointA.x = 370;
    pointA.y = 73;
    points.push_back(pointA);
    point2D pointB = point2D();
    pointB.x = 370;
    pointB.y = 132;
    points.push_back(pointB);
    point2D pointC = point2D();
    pointC.x = 370;
    pointC.y = 297;
    points.push_back(pointC);
    point2D pointD = point2D();
    pointD.x = 370;
    pointD.y = 415;
    points.push_back(pointD);
    n = remove_coincident_points();
}


/* initialize the vector of points stored in global variable points with certain points for second test case */
void initialize_points_case3() {
    
    point2D pointA = point2D();
    pointA.x = 73;
    pointA.y = 200;
    points.push_back(pointA);
    point2D pointB = point2D();
    pointB.x = 200;
    pointB.y = 200;
    points.push_back(pointB);
    point2D pointC = point2D();
    pointC.x = 200;
    pointC.y = 20;
    points.push_back(pointC);

    n = remove_coincident_points();
}

/* initialize the vector of points stored in global variable points with certain points for third test case */
void initialize_points_case4() {
    point2D pointA = point2D();
    pointA.x = 3;
    pointA.y = 5;
    points.push_back(pointA);
    point2D pointB = point2D();
    pointB.x = 36;
    pointB.y = 370;
    points.push_back(pointB);
    point2D pointC = point2D();
    pointC.x = 2;
    pointC.y = 6;
    points.push_back(pointC);

    n = remove_coincident_points();
}



/* initialize the vector of points stored in global variable points with certain points for third test case */
void initialize_points_case5() {
    point2D pointA = point2D();
    pointA.x = 50;
    pointA.y = 232;
    points.push_back(pointA);
    point2D pointB = point2D();
    pointB.x = 50;
    pointB.y = 400;
    points.push_back(pointB);
    point2D pointC = point2D();
    pointC.x = 68;
    pointC.y = 232;
    points.push_back(pointC);
    
    n = remove_coincident_points();

}






/* Print the array of points stored in global variable points*/
void print_points() {
    int i;
    printf("points: ");
    for (i=0; i<n; i++) {
        printf("[%d,%df] ", points.at(i).x, points.at(i).y);
    }
    printf("\n");
    fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}


void reset() {
    
    //re-initialize points
    
    switch ( caseToUse )
    {
        case 0:
            initialize_points_random();
            break;
        case 1:
            initialize_points_case1();
            break;
        case 2:
            initialize_points_case2();
            break;
        case 3:
            initialize_points_case3();
            break;
        case 4:
            initialize_points_case4();
            break;
    }
    
    
    //free current tree
    if (tree) kdtree_free(tree);
    //tree = NULL;
    
    //Build the tree
    Rtimer rt1;
    rt_start(rt1);
    tree = kdtree_build(points);
    rt_stop(rt1);
    char buf [1024];
    rt_sprint(buf,rt1);
    printf("time to generate kd-tree:  %s\n\n", buf);
    fflush(stdout);
    
    // print the tree
    kdtree_print(tree);
}


/* MAIN METHOD */
int main(int argc, char * argv[]) {
    
    // read number of points from user
    if (argc!=2) {
        printf("usage: viewPoints <nbPoints>\n");
        exit(1);
    }
    
    n = atoi(argv[1]);
    printf("you entered n=%d\n", n);
    assert(n > 0);
    
    srand(time(NULL));
    //initialize the points and build kdtree
    reset();
    
    /* initialize GLUT  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    /* register callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(keypress);
    
    
    /* init GL */
    /* set background color black*/
    glClearColor(1, 1, 1, 1);
    
    /* circular points */
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPointSize(POINT_SIZE);
    
    /* give control to event handler */
    glutMainLoop();
    return 0;
}


/* draw a line between two points */
void draw_line(lineSegment2D line){
    glColor3fv(black);
    glLineWidth(40);
    glBegin(GL_LINES);
    glVertex2f(line.p1.x, line.p1.y);
    glVertex2f(line.p2.x, line.p2.y);
    glEnd();
}


/* Draw the array of points stored in global variable points vector.*/
void draw_points(){
    for (int i=0; i<n; i++) {
        point2D point = points.at(i);
        glColor3fv(green);
        glBegin(GL_POINTS);
        glVertex2f(point.x, point.y);
        glEnd();
    }
    
}

/*Takes a bounding box for a node and colors it in randomly with one of four colors */
void fillRec(bounds nodeBounds){
    point2D a, b, c, d;
    a.x = nodeBounds.x_lower;
    a.y = nodeBounds.y_upper;
    b.x = nodeBounds.x_upper;
    b.y = nodeBounds.y_upper;
    c.x = nodeBounds.x_lower;
    c.y = nodeBounds.y_lower;
    d.x = nodeBounds.x_upper;
    d.y = nodeBounds.y_lower;
    
    int color = rand() % 4 + 1;
    
    switch ( color )
    {
        case 1:
            glColor3fv(red);
            break;
        case 2:
            glColor3fv(blue);
            break;
        case 3:
            glColor3fv(yellow);
            break;
        case 4:
            glColor3fv(white);
            break;
    }
    
    glBegin(GL_POLYGON);
    glVertex2f(a.x, a.y);
    glVertex2f(c.x, c.y);
    glVertex2f(d.x, d.y);
    glVertex2f(b.x, b.y);
    glEnd();
}


/* Recursive draw function for drawing a tree rooted at the given node*/
void draw_node(treeNode *node)
{
    if (node == NULL) return;
    
    //if we are here, node must be valid
    
    //recursively print left child
    draw_node(node->left);
    lineSegment2D line = lineSegment2D();
    point2D pointA = point2D();
    point2D pointB = point2D();
    
    
    switch (node->type) {
        case 'h':
            //Draw line for node
            printf("HORIZONTAL: (y=%d)\n", node->p.y);
            
            //Y coords are same
            pointA.y = node->p.y;
            pointB.y =  pointA.y;
            
            pointA.x = node->nodeBounds.x_lower;
            pointB.x =  node->nodeBounds.x_upper;
            
            break;
        case 'v':
            //Draw line for node
            printf("VERTICAL: (x=%d)\n", node->p.x);
            
            //X coords are same
            pointA.x = node->p.x;
            pointB.x =  pointA.x;
            
            pointA.y = node->nodeBounds.y_lower;
            pointB.y =  node->nodeBounds.y_upper;
            break;
        case 'l':
            //Fill rectangle for node
            fillRec( node->nodeBounds);
            break;
        default:
            printf("Improper tree node type\n");
            exit(1);
    };
    
    
    line.p1 = pointA;
    line.p2 = pointB;
    draw_line(line);
    
    //recursively print right child
    draw_node(node->right);
    
}


/* Draw the kd-tree stored in the global variable kdTree
 */
void draw_kdtree(){
    assert(tree);
    draw_node(tree->root);
}



/*Display Function that calls the draw tree and points method */
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //clear the matrix
    
    
    /* the default GL window is [-1,1]x[-1,1] with the origin in the
     center the points are in the range (0,0) to (WINSIZE,WINSIZE), so
     they need to be mapped to [-1,1]x [-1,1] */
    glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);
    glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0);
    
    //eventually we'll want to call the function that draws the kdtree
    draw_kdtree();
    
    //Draw points
    if (drawPoints) {
        draw_points();
    }
    
    
    /* execute the drawing commands */
    glFlush();
}




/* Updates the display when keys are pressed */
void keypress(unsigned char key, int x, int y) {
    switch(key)
    {
        case ' ':
            caseToUse = 0;
            reset();
            glutPostRedisplay();
            break;
        case 'p':
            drawPoints = drawPoints ? false : true;
            glutPostRedisplay();
            break;
        case '1':
            caseToUse = 1;
            reset();
            glutPostRedisplay();
            break;
        case '2':
            caseToUse = 2;
            reset();
            glutPostRedisplay();
            break;
        case '3':
            caseToUse = 3;
            reset();
            glutPostRedisplay();
            break;
        case '4':
            caseToUse = 4;
            reset();
            glutPostRedisplay();
            break;
        case 'q':
            exit(0);
            break;
    }
}


/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height);
}



/*
 Laura Toma, Rob Visentin
 
 What it does:
 
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


#include <GLUT/glut.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h> // Header File For The OpenGL32 Library
#include <OpenGL/glu.h> // Header File For The GLu32 Library
#include <GLUT/glut.h> // Header File For The GLut Library



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


/* global variables */
const int WINDOWSIZE = 500;
const int POINT_SIZE  = 6.0f;

//the array of n points
int n;
std::vector<point2D> points;


// the kd-tree created with the points
kdtree *tree = NULL;




/* ****************************** */
/* initialize  the array of points stored in global variable points with random points */
void initialize_points_random() {
    
    for (int i=0; i<n; i++) {
        point2D point = point2D();
        point.x = (int)(.1*WINDOWSIZE)/2 + rand() % ((int)(.9*WINDOWSIZE));
        point.y =  (int)(.1*WINDOWSIZE)/2 + rand() % ((int)(.9*WINDOWSIZE));
        points.push_back(point);
    }
}


/* ****************************** */
/* print the array of points stored in global variable points*/
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
    
    initialize_points_random();
    
    //free current tree
    if (tree) kdtree_free(tree);
    
    
    Rtimer rt1;
    rt_start(rt1);
    tree = kdtree_build(points, n);
    rt_stop(rt1);
    char buf [1024];
    rt_sprint(buf,rt1);
    printf("time to generate kd-tree:  %s\n\n", buf);
    fflush(stdout);
    
    // print the tree
   kdtree_print(tree);
}


/* ****************************** */
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
    glClearColor(0, 0, 0, 0);
    
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




/* ****************************** */
/* draw a single point */
void draw_point(point2D point)
{
    glColor3fv(yellow);
    
    glBegin(GL_POINTS);
    glVertex2f(point.x, point.y);
    glEnd();
}


/* ****************************** */
/* draw a line between two points */
void draw_line(lineSegment2D line)
{
    glColor3fv(cyan);
    
    glBegin(GL_LINES);
    glVertex2f(line.p1.x, line.p1.y);
    glVertex2f(line.p2.x, line.p2.y);
    glEnd();
}





/* ****************************** */
/* Draw the array of points stored in global variable points vector.
 Each point is drawn as a small square
 
 */
void draw_points(){
    
    const int R= 1;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //set color
    glColor3fv(yellow);
    
       int i;
    for (i=0; i<n; i++) {
        draw_point(points.at(i));
    }
    
}


/* ****************************** */
/* recursive draw function for drawing a tree rooted at the given node
 */
void draw_node(treeNode *node)
{
    //fill in
}

/* ****************************** */
/* draw the kd-tree stored in the global variable kdTree
 */
void draw_kdtree()
{
    assert(tree);
    draw_node(tree->root);
}



/* ****************************** */
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
    //draw_kdtree();
    
    //for now we just draw the input points
    draw_points();
    
    /* execute the drawing commands */
    glFlush();
}




/* ****************************** */
void keypress(unsigned char key, int x, int y) {
    switch(key)
    {
        case ' ':
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



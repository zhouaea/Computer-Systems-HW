/*
 * Program to add partial OOP functionality (encapsulation) to C.
 * Author: Neo Zhou - zhouaea@bc.edu
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_SIZE 64

// Define the PointData struct in Point.c so main can not access it.
typedef struct PointData {
    double x;
    double y;
    char to_string[MAX_STRING_SIZE];
} PointData;

// Declare a PointData object in the heap and initialize it.
PointData * PointData_new(double x, double y) {
    PointData * newPointData = (PointData *) malloc(sizeof(PointData));
    newPointData->x = x;
    newPointData->y = y;
    sprintf(newPointData->to_string, "(%f, %f)", x, y);
    return newPointData;
}

typedef struct Point {
    double (*x)(struct Point *);
    double (*y)(struct Point *);
    double (*magnitude)(struct Point *);
    double (*distance)(struct Point *, struct Point *);
    char * (*to_string)(struct Point *);
    void (*delete)(struct Point *);
    struct PointData * data;
} Point;

// Extract the x coordinate of a Point.
double Point_x(Point * this) {
    return this->data->x;
}

// Extract the y coordinate of a Point.
double Point_y(Point * this) {
    return this->data->y;
}

// Obtain the length of a line from the origin to a Point.
double Point_magnitude(Point * this) {
    double x = this->data->x;
    double y = this->data->y;
    
    // Calculate the absolute value of the hypotenuse of the triangle formed by the line from the
    // origin to the x coordinate and the line from the origin to the y coordinate.
    return fabs(sqrt(x * x + y * y));
}

// Obtain the length of a line from one point to another.
double Point_distance(struct Point * this, struct Point * that){
    // Extract the x and y coordinates of the two points.
    double x1 = this->data->x;
    double y1 = this->data->y;
    double x2 = that->data->x;
    double y2 = that->data->y;
    
    // Calculate the difference between the x and y coordinates.
    double delta_x = x2 - x1;
    double delta_y = y2 - y1;
    
    // Calculate the relative distance of the hypotenuse of the triangle formed by the line from one
    // x coordinate to another and the line from one y coordinate to the other.
    return sqrt(delta_x * delta_x + delta_y * delta_y);
}

// Obtain the string format of a Point.
char * Point_to_string(struct Point * this) {
    return this->data->to_string;
}

// Free memory in the heap associated with a Point.
void Point_delete(struct Point * this) {
    free(this->data);
    free(this);
}

// Declare a Point object in the heap and initialize it
Point * Point_new(double x, double y) {
    Point * newPoint = (Point *) malloc(sizeof(Point));
    newPoint->data = PointData_new(x,y);
    newPoint->x = &Point_x;
    newPoint->y = &Point_y;
    newPoint->magnitude = &Point_magnitude;
    newPoint->distance = &Point_distance;
    newPoint->to_string = &Point_to_string;
    newPoint->delete = &Point_delete;
    return newPoint;
}

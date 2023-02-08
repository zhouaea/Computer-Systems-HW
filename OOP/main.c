#include <stdio.h>
#include <stdlib.h>
#include "Point.h"

int main(void) {
    Point * p1 = Point_new(3.0, 3.0);
    Point * p2 = Point_new(4.0, 4.0);
    printf("Point p1: %s\n", p1->to_string(p1));
    printf("Point p2: %s\n", p2->to_string(p2));
    printf("Magnitude of p1: %.2f\n", p1->magnitude(p1));
    printf("Magnitude of p1: %.2f\n", p2->magnitude(p2));
    printf("Distance from p1 to p2: %.2f\n", p1->distance(p1, p2));
    printf("Distance from p2 to p1: %.2f\n", p2->distance(p2, p1));
    printf("Distance from p1 to itself: %.2f\n", p1->distance(p1, p1));
    p2->delete(p2);
    p1->delete(p1);
    return EXIT_SUCCESS;
}

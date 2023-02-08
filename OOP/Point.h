typedef struct Point {
    double (*x)(struct Point *);
    double (*y)(struct Point *);
    double (*magnitude)(struct Point *);
    double (*distance)(struct Point *, struct Point *);
    char * (*to_string)(struct Point *);
    void (*delete)(struct Point *);
    struct PointData * data;
} Point;

Point * Point_new(double x, double y);


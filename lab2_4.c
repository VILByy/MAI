#include "stdio.h"
#include "stdarg.h"
#include "math.h"

#define epsilon 0.0000001

typedef enum{
    OK = 1,
    INVALID_SIDE,
    TOO_FEW,
    NO_CONVEX,
    CONVEX,
} CODE_RESULT;

typedef struct {
    double x;
    double y;
} Point;

void status(CODE_RESULT flag){
    switch (flag) {
        case OK:
            printf("Done\n"); break;
        case INVALID_SIDE:
            printf("Wrong type or number <= 0 given!\n"); break;
        case TOO_FEW:
            printf("Too few sides for polygon!\n"); break;
        case CONVEX:
            printf("Given polygon is convex\n"); break;
        case NO_CONVEX:
            printf("Given polygon is not convex\n"); break;
    }
}

int sign(double x){
    return (x > 0) - (x < 0);
}

double straight(Point p1, Point p2, Point p3){
    double x = p2.x;
    double x1 = p1.x;
    double x2 = p3.x;
    double y = p2.y;
    double y1 = p1.y;
    double y2 = p3.y;
    return ((x - x1) * (y2 - y) - (x2 - x) * (y - y1));
}

CODE_RESULT polygon(int cnt, ...){
    if (cnt < 3){
        return TOO_FEW;
    }
    Point arr[cnt];
    int length = cnt, prev_sign;
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = 0; i < length; i++) {
        arr[i] = va_arg(ptr, Point);
    }
    va_end(ptr);
    int cur_sign = sign(straight(arr[0], arr[1], arr[2]));
    for (int i = 0; i < length; ++i) {
        double temp = straight(arr[i], arr[(i + 1) % length], arr[(i + 2) % length]);
        if (fabs(temp) < epsilon){
            return NO_CONVEX;
        }
        prev_sign = cur_sign;
        cur_sign = sign(temp);
        if (cur_sign != prev_sign){
            return NO_CONVEX;
        }
    }
    return CONVEX;
}

CODE_RESULT polynomial(double *res, double x, int n, int cnt, ...){
    int length = cnt;
    double arr[cnt];
    va_list ptr;
    va_start(ptr, cnt);
    for (int i = length - 1; i >= 0; --i) {
        arr[i] = va_arg(ptr, double);
    }
    va_end(ptr);
    for (int i = n; i >= 0 ; --i) {
        *res += pow(x, i) * arr[i];
    }
    return OK;
}

int main(){
    double res = 0;
    Point p1 = {-5.60, 0.0};
    Point p3 = {-15.7, 4.9};
    Point p2 = {-15.7, 0.0};
    Point p5 = {-5.60, 4.9};
    Point p4 = {-8.0, 4.9};
    printf("1st part res: ");
    status(polygon(5, p1, p2, p3, p4, p5));
    status(polynomial(&res, 2.0, 3, 4, 6.0, 56.9, 0.0, 7.9));
    printf("2nd part res: %f", res);
    return 0;
}
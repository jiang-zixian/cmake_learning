#include "MathFunctions.h"
#include <cmath>

// 实现 mysqrt 函数
double mysqrt(double x) {
    if (x < 0.0) {
        return 0.0; // 对于负数，返回 0.0
    } else {
        return sqrt(x); // 调用编译器的 sqrt 函数计算平方根
    }
}

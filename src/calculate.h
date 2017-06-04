#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
void FunCount(char const* output, double* stack, int j, error_t* lastError);
double Count(char* output, error_t* lastError);
double Calculate(char const* expression, error_t* lastError);
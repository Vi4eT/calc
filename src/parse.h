#pragma once
#define SQRT 1
#define SIN 2
#define COS 3
#define TG 4
#define CTG 5
#define ARCSIN 6
#define ARCCOS 7
#define ARCTG 8
#define LN 9
#define FLOOR 10
#define CEIL 11
int isfunc(char const* s);
void FunToStack(char* out, int fun);
void Shift(int *i, int fun);
int isoper(char s);
int FunCheck(int s);
int FindUnary(char const* expression, int* i, int const* previous, error_t* lastError);
void DigitCase(char const *expression, char *output, int const* i, int* j, int* previous, int* digit, error_t* lastError);
void PointCase(char const *expression, char *output, int const* i, int* j, int* previous, int* digit, error_t* lastError);
void ECase(char const *expression, char *output, int* i, int* j, int* previous, error_t* lastError);
void OperCase(char const *expression, char *output, char* stack, int* i, int* j, int* k, int previous, error_t* lastError);
void Parse(char const *expression, char *output, error_t* lastError);
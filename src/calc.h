#pragma once
#pragma warning(disable:4996)
#define _USE_MATH_DEFINES
#ifndef _CRTDBG_MAP_ALLOC
  #define _CRTDBG_MAP_ALLOC
#endif // !_CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#define MEM_BLOCK 8
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  ERR_CANNOT_OPEN,
  ERR_ARGUMENTS,
  ERR_OPERANDS,
  ERR_BRACKETS,
  ERR_EXPRESSION,
  ERR_DOMAIN,
  ERR_SIGNS,
  ERR_E,
  ERR_POINT,
  ERR_MINUS
} error_t;
//parse.c
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
//calculate.c
void FunCount(char const* output, double* stack, int j, error_t* lastError);
double Count(char* output, error_t* lastError);
double Calculate(char const* expression, error_t* lastError);
//ui.c
char const* GetErrorString(error_t error);
error_t ReportError(error_t error);
int iscomment(char const* line);
char* ReadLine(FILE* in, error_t* lastError);
void ProcessLine(char const* line, FILE* in, error_t* lastError);
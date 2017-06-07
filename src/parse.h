#pragma once
typedef enum
{
  SQRT = 1,
  SIN,
  COS,
  TG,
  CTG,
  ARCSIN,
  ARCCOS,
  ARCTG,
  LN,
  FLOOR,
  CEIL
} func_t;
typedef enum
{
  EMPTY,
  NUMBER,
  FUNCTION,
  OPEN_BRACKET,
  CLOSE_BRACKET,
  OPERATION,
  DOT,
  CONST_PI,
  CONST_E,
  SCI_NUMBER
} prev_t;
int isfunc(char const* s);
void FunToStack(char* out, int fun);
void Shift(int *i, int fun);
int isoper(char s);
int FindUnary(char const* expression, int* i, prev_t const* previous, error_t* lastError);
void DigitCase(char const *expression, char *output, int const* i, int* j, prev_t* previous, int* digit, error_t* lastError);
void PointCase(char const *expression, char *output, int const* i, int* j, prev_t* previous, const int* digit, error_t* lastError);
void ECase(char const *expression, char *output, int* i, int* j, prev_t* previous, error_t* lastError);
void OperCase(char const *expression, char *output, char* stack, int* i, int* j, int* k, prev_t previous, error_t* lastError);
void Parse(char const *expression, char *output, error_t* lastError);
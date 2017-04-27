#pragma warning(disable:4996)
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <crtdbg.h>
#define MEM_BLOCK 8
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
//////////////////////////////////////////////////////////////////////////////
// Dummy calc module
// TODO: Move to a separate module 'calc'
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  ERR_CANNOT_OPEN,
  ERR_ARGUMENTS,
  ERR_OPERANDS,
  ERR_DIV_ZERO,
  ERR_BRACKETS,
  ERR_EXPRESSION,
  ERR_DOMAIN,
  ERR_SIGNS
} error_t;
// TODO: Move to a separate module 'calc'
char const* GetErrorString(error_t error)
{
  if (error == ERR_NOT_ENOUGH_MEMORY)
    return "Not enough memory.";
  else if (error == ERR_CANNOT_OPEN)
    return "Cannot open file.";
  else if (error == ERR_ARGUMENTS)
    return "Too many input arguments.";
  else if (error == ERR_OPERANDS)
    return "Not enough operands.";
  else if (error == ERR_DIV_ZERO)
    return "Division by zero!";
  else if (error == ERR_BRACKETS)
    return "Brackets.";
  else if (error == ERR_EXPRESSION)
    return "Wrong expression.";
  else if (error == ERR_DOMAIN)
    return "Out of domain.";
  else if (error == ERR_SIGNS)
    return "Not enough arithmetic signs.";
  else if (error == ERR_OK)
    return "Redundant error report.";
  else
    return "No error code!";
}
error_t ReportError(error_t error)
{
  printf("ERROR: %s\n", GetErrorString(error));
  return error;
}
// TODO: Move to a separate module 'calc'
int isfunc(char const *s)
{
  if (strncmp(s, "sqrt", 4) == 0)
    return SQRT;
  else if (strncmp(s, "sin", 3) == 0)
    return SIN;
  else if (strncmp(s, "cos", 3) == 0)
    return COS;
  else if (strncmp(s, "tg", 2) == 0)
    return TG;
  else if (strncmp(s, "ctg", 3) == 0)
    return CTG;
  else if (strncmp(s, "arcsin", 6) == 0)
    return ARCSIN;
  else if (strncmp(s, "arccos", 6) == 0)
    return ARCCOS;
  else if (strncmp(s, "arctg", 5) == 0)
    return ARCTG;
  else if (strncmp(s, "ln", 2) == 0)
    return LN;
  else if (strncmp(s, "floor", 5) == 0)
    return FLOOR;
  else if (strncmp(s, "ceil", 4) == 0)
    return CEIL;
  else
    return 0;
}
void FuncToStack(char out, int func)
{
  if (func == SQRT)
    out = 'q';
  else if (func == SIN)
    out = 's';
  else if (func == COS)
    out = 'c';
  else if (func == TG)
    out = 't';
  else if (func == CTG)
    out = 'g';
  else if (func == ARCSIN)
    out = 'i';
  else if (func == ARCCOS)
    out = 'o';
  else if (func == ARCTG)
    out = 'a';
  else if (func == LN)
    out = 'n';
  else if (func == FLOOR)
    out = 'f';
  else if (func == CEIL)
    out = 'l';
}
int isoper(char s)
{
  if (s == '^')
    return 1;
  else if (s == '*' || s == '/')
    return 2;
  else if (s == '+' || s == '-')
    return 3;
  else if (s == '(' || s == ')')
    return 4;
  else
    return 0;
}
void Parse(char const *expression, char *output, error_t* lastError)
{
  int i = 0, j = 0, k = 0, n, previous = 0;
  char *stack, *realltmp;
  n = 8 * MEM_BLOCK;
  stack = (char*)malloc(sizeof(char)*n);
  if (stack == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return;
  }
  while (expression[i])
  {
    if (isdigit(expression[i]))
    {
      if (previous == 1)
      {
        *lastError = ERR_SIGNS;
        return;
      }
      output[j] = expression[i];
      j++;
      if (!isdigit(expression[i+1]) || isspace(expression[i+1]))
      {
        output[j] = ' ';
        j++;
        previous = 1;
      }
    }
    else if (isfunc(expression + i))
    {
      FuncToStack(stack[k], isfunc(expression + i));
      k++;
      previous = 2;
    }
    else if (expression[i] == '(')
    {
      stack[k] = expression[i];
      k++;
      previous = 3;
    }
    else if (expression[i] == ')')
    {
      k--;
      if (k < 1)
      {
        *lastError = ERR_BRACKETS;
        return;
      }
      for (; stack[k] != '(' && k >= 0; k--, j++)
      {
        output[j] = stack[k];
        output[j + 1] = ' ';
        j++;
      }
      if (stack[k] != '(')
      {
        *lastError = ERR_BRACKETS;
        return;
      }
      previous = 4;
    }
    else if (isoper(expression[i]))
    {
      if (k && stack[k - 1] != '(')
      {
        while ((isoper(expression[i]) >= isoper(stack[k - 1])) && k > 0)
        {
          k--;
          output[j] = stack[k];
          j++;
          output[j] = ' ';
          j++;
        }
      }
      stack[k] = expression[i];
      k++;
      previous = 5;
    }
    else if (isspace(expression[i]));
    else
    {
      *lastError = ERR_EXPRESSION;
      return;
    }
    i++;
    if (k >= n)
    {
      n += MEM_BLOCK;
      if ((realltmp = (char*)realloc(stack, n)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        return;
      }
      else
        stack = realltmp;
    }
  }
  k--;
  if (isoper(stack[0])||(isdigit(output[j-2])&&!isoper(stack[0])))
    for (; k > -1; k--, j++)
    {
      if (stack[k] == '(')
      {
        *lastError = ERR_BRACKETS;
        return;
      }
      output[j] = stack[k];
      output[j + 1] = ' ';
      j++;
    }
  else
  {
    *lastError = ERR_OPERANDS;
    return;
  }
  free(stack);
  output[j] = '\0';
}
double Count(char* output, error_t* lastError)
{
  int j = 0, n;
  double *stack, *realltmp, number;
  char *endptr;
  n = 8 * MEM_BLOCK;
  stack = (double*)malloc(sizeof(double)*n);
  while (*output)
  {
    if (isdigit(*output))
    {
      stack[j] = strtod(output, &endptr);
      output = endptr;
      j++;
    }
    else if (isoper(*output))
    {
      j -= 2;
      if (j < 0)
      {
        *lastError = ERR_OPERANDS;
        return 1;
      }
      switch (*output)
      {
        case '+':
          stack[j] += stack[j + 1];
          break;
        case '-':
          stack[j] -= stack[j + 1];
          break;
        case '*':
          stack[j] *= stack[j + 1];
          break;
        case '/':
          if (stack[j + 1] == 0)
          {
            *lastError = ERR_DIV_ZERO;
            return 1;
          }
          stack[j] /= stack[j + 1];
          break;
        case '^':
          if ((stack[j] == 0 && stack[j + 1] <= 0) || (stack[j] < 0 && stack[j + 1] != (int)stack[j + 1]))
          {
            *lastError = ERR_DOMAIN;
            return 1;
          }
          stack[j] = pow(stack[j], stack[j + 1]);
          break;
      }
      j++;
      *output++;
    }
    if (isspace(*output))
      *output++;
    if (j >= n)
    {
      n += MEM_BLOCK;
      if ((realltmp = (double*)realloc(stack, n)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        return 1;
      }
      else
        stack = realltmp;
    }
  }
  number = stack[0];
  free(stack);
  return number;
}
double Calculate(char const* expression, error_t* lastError)
{
  double result;
  char *output;
  output = (char*)malloc(sizeof(char)*strlen(expression) * 2 + 1);
  if (output == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return 1;
  }
  Parse(expression, output, lastError);
  if (*lastError != ERR_OK)
    return 1;
  result = Count(output, lastError);
  if (*lastError != ERR_OK)
    return 1;
  free(output);
  return result;
}
//////////////////////////////////////////////////////////////////////////////
// UI functions
int isempty(char* line)
{
  int i = 0;
  for (i = 0;line[i] != 0;i++)
  {
    if (isspace(line[i]) == 0)
      return 0;
  }
  return 1;
}
int iscomment(char* line)
{
  int i;
  for (i = 0;line[i] != 0;i++)
  {
    if (line[i] == '/'&&line[i + 1] == '/')
      return 1;
  }
  return 0;
}
char* ReadLine(FILE* in, error_t* lastError)
{
  int i = 0, n = MEM_BLOCK;
  char* line = NULL, *realltmp = NULL;
  char c = 0;
  line = (char*)malloc(sizeof(char)*n);
  if (line == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    ReportError(*lastError);
  }
  while (c != '\n' && c != EOF)
  {
    for (; i < n; i++)
    {
      c = (char)getc(in);
      line[i] = c;
      if (c == '\n' || c == EOF)
        break;
    }
    if (c != '\n' && c != EOF)
    {
      n += MEM_BLOCK;
      if ((realltmp = (char*)realloc(line, n)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        ReportError(*lastError);
      }
      else
        line = realltmp;
    }
  }
  line[i] = 0;
  return line;
}
void ProcessLine(char const* line, FILE* in, error_t* lastError)
{
  printf("%s == ", line);
  double result = Calculate(line, lastError);
  if (*lastError == ERR_OK)
  {
    printf("%g", result);
    if (!feof(in))
      printf("\n");
  }
  else
    ReportError(*lastError);
}
void print(char* line, FILE* in, error_t* lastError)
{
  if (iscomment(line))
    if (feof(in))
      printf("%s", line);
    else
      printf("%s\n", line);
  else if (isempty(line))
    if (feof(in))
      printf("%s", line);
    else
      printf("%s\n", line);
  else
    ProcessLine(line, in, lastError);
}
int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  char* line = NULL;
  error_t lastError = ERR_OK;
  if (argc > 1 && (in = fopen(argv[1], "r")) == NULL)// Choose an input source
  {
    lastError = ERR_CANNOT_OPEN;
    ReportError(lastError);
    exit(ERR_CANNOT_OPEN);
  }
  if (argc > 2)
  {
    lastError = ERR_ARGUMENTS;
    ReportError(lastError);
    exit(ERR_ARGUMENTS);
  }
  while ((line = ReadLine(in, &lastError)) != NULL)// Process the data line by line
  {
    if (lastError != ERR_OK)
      lastError = ERR_OK;
    print(line, in, &lastError);
    free(line);
    if (feof(in))
      break;
  }
  if (in != stdin)// Clean up
    fclose(in);
  _CrtDumpMemoryLeaks();
  return 0;
}
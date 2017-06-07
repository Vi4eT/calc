#include "main.h"
#include "calculate.h"
#include "parse.h"
int FunCheck(int s)
{
  if (s == 'q' || s == 's' || s == 'c' || s == 't' || s == 'g' || s == 'i' || s == 'o' || s == 'a' || s == 'n' || s == 'f' || s == 'l')
    return 1;
  else
    return 0;
}
void FunCount(char const* output, double* stack, int j, error_t* lastError)
{
  if (*output == 'q')
  {
    if (stack[j] < 0)
    {
      *lastError = ERR_DOMAIN;
      return;
    }
    stack[j] = sqrt(stack[j]);
  }
  else if (*output == 's')
    stack[j] = sin(stack[j]);
  else if (*output == 'c')
    stack[j] = cos(stack[j]);
  else if (*output == 't')
    stack[j] = tan(stack[j]);
  else if (*output == 'g')
    if (sin(stack[j]) == 0)
    {
      *lastError = ERR_DOMAIN;
      return;
    }
    else
      stack[j] = 1 / tan(stack[j]);
  else if (*output == 'i')
  {
    if (stack[j] > 1 || stack[j] < -1)
    {
      *lastError = ERR_DOMAIN;
      return;
    }
    stack[j] = asin(stack[j]);
  }
  else if (*output == 'o')
  {
    if (stack[j] > 1 || stack[j] < -1)
    {
      *lastError = ERR_DOMAIN;
      return;
    }
    stack[j] = acos(stack[j]);
  }
  else if (*output == 'a')
    stack[j] = atan(stack[j]);
  else if (*output == 'n')
  {
    if (stack[j] <= 0)
    {
      *lastError = ERR_DOMAIN;
      return;
    }
    stack[j] = log(stack[j]);
  }
  else if (*output == 'f')
    stack[j] = floor(stack[j]);
  else if (*output == 'l')
    stack[j] = ceil(stack[j]);
}
double Count(char* output, error_t* lastError)
{
  int j = 0, size = 2 * MEM_BLOCK;
  double *stack = NULL, *reallptr = NULL, number;
  char *endptr;
  stack = (double*)malloc(sizeof(double)*size);
  if (stack == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return 1;
  }
  while (*output)
  {
    if (isdigit((unsigned char)*output) || *output == '.')
    {
      stack[j] = strtod(output, &endptr);
      output = endptr;
      j++;
    }
    else if (*output == 'E')
    {
      j--;
      *(output++);
      if (*output == '-')
      {
        *(output++);
        stack[j + 1] = strtod(output, &endptr);
        output = endptr;
        while (stack[j + 1]>0)
        {
          stack[j] /= 10;
          stack[j + 1]--;
        }
      }
      else
      {
        if (*output == '+')
          *(output++);
        stack[j + 1] = strtod(output, &endptr);
        output = endptr;
        while (stack[j + 1]>0)
        {
          stack[j] *= 10;
          stack[j + 1]--;
        }
      }
      j++;
    }
    else if (*output == 'p')
    {
      stack[j] = M_PI;
      j++;
      *(output++);
    }
    else if (*output == 'e')
    {
      stack[j] = M_E;
      j++;
      *(output++);
    }
    else if (FunCheck(*output))
    {
      j--;
      if (j < 0)
      {
        *lastError = ERR_OPERANDS;
        free(stack);
        return 1;
      }
      FunCount(output, stack, j, lastError);
      if (*lastError != ERR_OK)
      {
        free(stack);
        return 1;
      }
      j++;
      *(output++);
    }
    else if (isoper(*output))
    {
      j -= 2;
      if (*output == '!')
        j++;
      if (j < 0)
      {
        *lastError = ERR_OPERANDS;
        free(stack);
        return 1;
      }
      switch (*output)
      {
        case '!':
          stack[j] = -stack[j];
          break;
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
            *lastError = ERR_DOMAIN;
            free(stack);
            return 1;
          }
          stack[j] /= stack[j + 1];
          break;
        case '^':
          if ((stack[j] == 0 && stack[j + 1] <= 0) || (stack[j] < 0 && stack[j + 1] != (int)stack[j + 1])) //rational exponent: NaN || complex
          {
            *lastError = ERR_DOMAIN;
            free(stack);
            return 1;
          }
          stack[j] = pow(stack[j], stack[j + 1]);
          break;
      }
      j++;
      *(output++);
    }
    if (isspace((unsigned char)*output))
      *(output++);
    if (j >= size)
    {
      size += MEM_BLOCK;
      if ((reallptr = (double*)realloc(stack, size)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        free(stack);
        return 1;
      }
      else
        stack = reallptr;
    }
  }
  number = stack[0];
  free(stack);
  return number;
}
double Calculate(char const* expression, error_t* lastError)
{
  double result;
  char *output = NULL;
  output = (char*)malloc(sizeof(char)*strlen(expression) * 2 + 1);
  if (output == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return 1;
  }
  Parse(expression, output, lastError);
  if (*output == '\0')
    *lastError = ERR_EXPRESSION;
  if (*lastError != ERR_OK)
  {
    free(output);
    return 1;
  }
  result = Count(output, lastError);
  if (result == -0)
    result = 0;
  if (*lastError != ERR_OK)
  {
    free(output);
    return 1;
  }
  free(output);
  return result;
}
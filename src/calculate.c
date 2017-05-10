#include "calc.h"
#include <math.h>
void FunCount(char const* output, double* stack, int j, error_t* lastError)
{
  switch (*output)
  {
    case 'q':
      if (stack[j] < 0)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      stack[j] = sqrt(stack[j]);
      break;
    case 's':
      stack[j] = sin(stack[j]);
      break;
    case 'c':
      stack[j] = cos(stack[j]);
      break;
    case 't':
      if (cos(stack[j]) == 0)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      stack[j] = tan(stack[j]);
      break;
    case 'g':
      if (sin(stack[j]) == 0)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      else if (cos(stack[j]) == 0)
        stack[j] = 0;
      else
        stack[j] = 1 / tan(stack[j]);
      break;
    case 'i':
      if (stack[j] > 1 || stack[j] < -1)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      stack[j] = asin(stack[j]);
      break;
    case 'o':
      if (stack[j] > 1 || stack[j] < -1)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      stack[j] = acos(stack[j]);
      break;
    case 'a':
      stack[j] = atan(stack[j]);
      break;
    case 'n':
      if (stack[j] <= 0)
      {
        *lastError = ERR_DOMAIN;
        return;
      }
      stack[j] = log(stack[j]);
      break;
    case 'f':
      stack[j] = floor(stack[j]);
      break;
    case 'l':
      stack[j] = ceil(stack[j]);
      break;
  }
}
double Count(char* output, error_t* lastError)
{
  int j = 0, n;
  double *stack, *realltmp, number;
  char *endptr;
  n = 2 * MEM_BLOCK;
  stack = (double*)malloc(sizeof(double)*n);
  if (stack == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return 1;
  }
  while (*output)
  {
    if (isdigit(*output) || *output == '.')
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
    if (isspace(*output))
      *(output++);
    if (j >= n)
    {
      n += MEM_BLOCK;
      if ((realltmp = (double*)realloc(stack, n)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        free(stack);
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
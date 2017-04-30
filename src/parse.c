#include "calc.h"
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
int isfunc(char const* s)
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
void FunToStack(char* out, int fun)
{
  if (fun == SQRT)
    *out = 'q';
  else if (fun == SIN)
    *out = 's';
  else if (fun == COS)
    *out = 'c';
  else if (fun == TG)
    *out = 't';
  else if (fun == CTG)
    *out = 'g';
  else if (fun == ARCSIN)
    *out = 'i';
  else if (fun == ARCCOS)
    *out = 'o';
  else if (fun == ARCTG)
    *out = 'a';
  else if (fun == LN)
    *out = 'n';
  else if (fun == FLOOR)
    *out = 'f';
  else if (fun == CEIL)
    *out = 'l';
}
void Shift(int *i, int fun)
{
  if (fun == TG || fun == LN)
    *i += 1;
  else if (fun == SIN || fun == COS || fun == CTG)
    *i += 2;
  else if (fun == SQRT || fun == CEIL)
    *i += 3;
  else if (fun == ARCTG || fun == FLOOR)
    *i += 4;
  else if (fun == ARCSIN || fun == ARCCOS)
    *i += 5;
}
int isoper(char s)
{
  if (s == '^')
    return 1;
  else if (s == '!')
    return 2;
  else if (s == '*' || s == '/')
    return 3;
  else if (s == '+' || s == '-')
    return 4;
  else if (s == '(' || s == ')')
    return 5;
  else
    return 0;
}
int FunCheck(int s)
{
  if (s == 'q' || s == 's' || s == 'c' || s == 't' || s == 'g' || s == 'i' || s == 'o' || s == 'a' || s == 'n' || s == 'f' || s == 'l')
    return 1;
  else
    return 0;
}
int FindUnary(char const* expression, int* i, int* previous, error_t* lastError)
{
  if (*previous == 6 || *previous == 2 || *previous == 5 || *previous == 0 || *previous == 3 || ((*previous == 1 || *previous == 7 || *previous == 8 || *previous == 9) && expression[*i + 1] == '-'))
  {
    if (expression[*i + 1] == ')' || isoper(expression[*i + 1]) == 1 || isoper(expression[*i + 1]) == 3 || expression[*i + 1] == '+')
    {
      *lastError = ERR_MINUS;
      return 0;
    }
    else
    {
      int amount = 1;
      if (expression[*i + 1] == '-')
      {
        int j = *i;
        j++;
        while (expression[j] == '-')
        {
          amount++;
          j++;
        }
        *i = j - 1;
        if (amount % 2 == 0)
          return 2;
      }
      if ((*previous == 1 || *previous == 6 || *previous == 7 || *previous == 8 || *previous == 9) && amount % 2)
        return 0;
      return 1;
    }
  }
  else
    return 0;
}
void Parse(char const *expression, char *output, error_t* lastError)
{
  int i = 0, j = 0, k = 0, n, previous = 0;
  char *stack, *realltmp = NULL;
  n = 2 * MEM_BLOCK;
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
      if (previous == 1 || previous == 4 || previous == 7 || previous == 8 || previous == 9)
      {
        *lastError = ERR_SIGNS;
        free(stack);
        return;
      }
      if (previous == 6 && expression[i - 1] != '.' && !isdigit(expression[i - 1]))
      {
        *lastError = ERR_POINT;
        free(stack);
        return;
      }
      output[j] = expression[i];
      j++;
      if ((!isdigit(expression[i + 1]) && expression[i + 1] != '.') || isspace(expression[i + 1]))
      {
        output[j] = ' ';
        j++;
        previous = 1;
      }
    }
    else if (expression[i] == '.')
    {
      if (previous == 1)
      {
        *lastError = ERR_SIGNS;
        free(stack);
        return;
      }
      if (previous == 6 || previous == 8 || previous == 9 || previous == 7 || previous == 4 || previous == 6)
      {
        *lastError = ERR_POINT;
        free(stack);
        return;
      }
      if (!isdigit(expression[i + 1]))
        if (i == 0)
        {
          *lastError = ERR_POINT;
          free(stack);
          return;
        }
        else if (!isdigit(expression[i - 1]))
        {
          *lastError = ERR_POINT;
          free(stack);
          return;
        }
      output[j] = expression[i];
      j++;
      if ((expression[i + 1] != '(' && isoper(expression[i + 1]) != 0) || isspace(expression[i + 1]))
      {
        output[j] = ' ';
        j++;
        previous = 1;
      }
      previous = 6;
    }
    else if (expression[i] == 'p' && expression[i + 1] == 'i')
    {
      if (previous == 1 || previous == 4 || previous == 6 || previous == 7 || previous == 8 || previous == 9)
      {
        *lastError = ERR_SIGNS;
        free(stack);
        return;
      }
      output[j] = 'p';
      j++;
      output[j] = ' ';
      j++;
      i++;
      previous = 7;
    }
    else if (expression[i] == 'e' || expression[i] == 'E')
    {
      if ((previous == 0 || previous == 3 || previous == 2 || previous == 5) && !isdigit(expression[i + 1]))
      {
        output[j] = 'e';
        j++;
        output[j] = ' ';
        j++;
        previous = 8;
      }
      else if ((previous == 6 || previous == 1) && !isspace(expression[i + 1]) && !isspace(expression[i - 1]))
      {
        output[j] = 'E';
        j++;
        if (!isdigit(expression[i + 1]) && (expression[i + 1] != '+' && expression[i + 1] != '-'))
        {
          *lastError = ERR_E;
          free(stack);
          return;
        }
        if (expression[i + 1] == '+' || expression[i + 1] == '-')
        {
          i++;
          output[j] = expression[i];
          j++;
        }
        if (isoper(expression[i + 1]) || isspace(expression[i + 1]) || expression[i + 1] == 0)
        {
          *lastError = ERR_E;
          free(stack);
          return;
        }
        while (isdigit(expression[i + 1]))
        {
          i++;
          output[j] = expression[i];
          j++;
        }
        if ((!isoper(expression[i + 1]) || expression[i + 1] == '(') && !isspace(expression[i + 1]) && expression[i + 1] != 0)
        {
          *lastError = ERR_E;
          free(stack);
          return;
        }
        output[j] = ' ';
        j++;
        previous = 9;
      }
      else
      {
        *lastError = ERR_E;
        free(stack);
        return;
      }
    }
    else if (isfunc(expression + i))
    {
      if (previous == 1 || previous == 2 || previous == 4 || previous == 6 || previous == 7 || previous == 8 || previous == 9)
      {
        *lastError = ERR_EXPRESSION;
        free(stack);
        return;
      }
      FunToStack(&stack[k], isfunc(expression + i));
      k++;
      Shift(&i, isfunc(expression + i));
      previous = 2;
    }
    else if (expression[i] == '(')
    {
      if (previous == 1 || previous == 4 || previous == 6 || previous == 7 || previous == 8 || previous == 9)
      {
        *lastError = ERR_SIGNS;
        free(stack);
        return;
      }
      stack[k] = expression[i];
      k++;
      previous = 3;
    }
    else if (expression[i] == ')')
    {
      k--;
      if (k < 0 || previous == 0 || previous == 2 || previous == 5)
      {
        *lastError = ERR_BRACKETS;
        free(stack);
        return;
      }
      if (previous == 3)
      {
        *lastError = ERR_OPERANDS;
        free(stack);
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
        free(stack);
        return;
      }
      previous = 4;
    }
    else if (isoper(expression[i]))
    {
      int priority = 0, unarycase = 0;
      if (expression[i] == '-')
      {
        unarycase = FindUnary(expression, &i, &previous, lastError);
        if (unarycase == 1)
          priority = 2;
        if (*lastError != ERR_OK)
        {
          free(stack);
          return;
        }
      }
      if (k && stack[k - 1] != '(')
      {
        if (priority == 2 || isoper(expression[i]) == 1)
        {
          if (priority != 2)
            priority = 1;
          while ((priority > isoper(stack[k - 1])) && k > 0 && (previous != 2 && isoper(stack[k - 1]) != 1))
          {
            k--;
            output[j] = stack[k];
            j++;
            output[j] = ' ';
            j++;
          }
        }
        else
        {
          if (unarycase == 2)
            i++;
          int oper = isoper(expression[i]);
          if (unarycase == 2 && oper == 5)
            oper = 0;
          if (unarycase == 2 && (previous == 1 || previous == 7 || previous == 8 || previous == 9 || previous == 4))
            oper = 4;
          while ((oper >= isoper(stack[k - 1])) && k > 0)
          {
            k--;
            output[j] = stack[k];
            j++;
            output[j] = ' ';
            j++;
          }
          if (unarycase == 2)
            i--;
        }
      }
      if (priority == 2)
      {
        stack[k] = '!';
        k++;
      }
      else if (unarycase == 2 && (previous == 1 || previous == 7 || previous == 8 || previous == 9 || previous == 4))
      {
        stack[k] = '+';
        k++;
      }
      else if (unarycase != 2)
      {
        stack[k] = expression[i];
        k++;
      }
      previous = 5;
    }
    else if (isspace(expression[i]));
    else
    {
      *lastError = ERR_EXPRESSION;
      free(stack);
      return;
    }
    i++;
    if (k >= n)
    {
      n += MEM_BLOCK;
      if ((realltmp = (char*)realloc(stack, n)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        free(stack);
        return;
      }
      else
        stack = realltmp;
    }
  }
  k--;
  //if (isoper(stack[0]) || (isdigit(output[j - 2]) && !isoper(stack[0])) || FunCheck(stack[0]) || ((previous == 7 || previous == 8) && !isoper(stack[0])))
    for (; k > -1; k--, j++)
    {
      if (stack[k] == '(')
      {
        *lastError = ERR_BRACKETS;
        free(stack);
        return;
      }
      output[j] = stack[k];
      output[j + 1] = ' ';
      j++;
    }
  /*else
  {
    *lastError = ERR_OPERANDS;
    free(stack);
    return;
  }*/
  free(stack);
  output[j] = '\0';
}
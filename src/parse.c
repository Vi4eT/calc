#include "main.h"
#include "parse.h"
int isfunc(char const* s)
{
  if (!strncmp(s, "sqrt", 4) || !strncmp(s, "SQRT", 4))
    return SQRT;
  else if (!strncmp(s, "sin", 3) || !strncmp(s, "SIN", 3))
    return SIN;
  else if (!strncmp(s, "cos", 3) || !strncmp(s, "COS", 3))
    return COS;
  else if (!strncmp(s, "tg", 2) || !strncmp(s, "TG", 2))
    return TG;
  else if (!strncmp(s, "ctg", 3) || !strncmp(s, "CTG", 3))
    return CTG;
  else if (!strncmp(s, "arcsin", 6) || !strncmp(s, "ARCSIN", 6))
    return ARCSIN;
  else if (!strncmp(s, "arccos", 6) || !strncmp(s, "ARCCOS", 6))
    return ARCCOS;
  else if (!strncmp(s, "arctg", 5) || !strncmp(s, "ARCTG", 5))
    return ARCTG;
  else if (!strncmp(s, "ln", 2) || !strncmp(s, "LN", 2))
    return LN;
  else if (!strncmp(s, "floor", 5) || !strncmp(s, "FLOOR", 5))
    return FLOOR;
  else if (!strncmp(s, "ceil", 4) || !strncmp(s, "CEIL", 4))
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
int FindUnary(char const* expression, int* i, prev_t const* previous, error_t* lastError)
{
  if (*previous == DOT || *previous == FUNCTION || *previous == OPERATION || *previous == EMPTY || *previous == OPEN_BRACKET || ((*previous == NUMBER || *previous == CONST_PI || *previous == CONST_E || *previous == SCI_NUMBER) && expression[*i + 1] == '-'))
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
      if ((*previous == NUMBER || *previous == DOT || *previous == CONST_PI || *previous == CONST_E || *previous == SCI_NUMBER) && amount % 2)
        return 0;
      return 1;
    }
  }
  else
    return 0;
}
void DigitCase(char const *expression, char *output, int const* i, int* j, prev_t* previous, int* digit, error_t* lastError)
{
  if (*previous == NUMBER || *previous == CLOSE_BRACKET || *previous == CONST_PI || *previous == CONST_E || *previous == SCI_NUMBER)
  {
    *lastError = ERR_SIGNS;
    return;
  }
  if (*previous == DOT && expression[*i - 1] != '.' && !isdigit((unsigned char)expression[*i - 1]))
  {
    *lastError = ERR_POINT;
    return;
  }
  if (*previous == FUNCTION) //&& expression[*i - 1] != ' ')
  {
    *lastError = ERR_EXPRESSION;
    return;
  }
  output[*j] = expression[*i];
  (*j)++;
  if ((!isdigit((unsigned char)expression[*i + 1]) && expression[*i + 1] != '.') || isspace((unsigned char)expression[*i + 1]))
  {
    output[*j] = ' ';
    (*j)++;
    *previous = NUMBER;
    *digit = 0;
  }
  else
    *digit = 1;
}
void PointCase(char const *expression, char *output, int const* i, int* j, prev_t* previous, const int* digit, error_t* lastError)
{
  if (*previous == NUMBER)
  {
    *lastError = ERR_SIGNS;
    return;
  }
  if (*previous == DOT || *previous == CONST_E || *previous == SCI_NUMBER || *previous == CONST_PI || *previous == CLOSE_BRACKET || (*previous == OPERATION && *digit == 0) || (*previous == OPEN_BRACKET && *digit == 0)) //is OPERATION, OPEN_BRACKET correct?
  {
    *lastError = ERR_POINT;
    return;
  }
  if (*previous == FUNCTION) //&& expression[*i - 1] != ' ')
  {
    *lastError = ERR_EXPRESSION;
    return;
  }
  if (!isdigit((unsigned char)expression[*i + 1]))
    if (*i == 0)
    {
      *lastError = ERR_POINT;
      return;
    }
    else if (!isdigit((unsigned char)expression[*i - 1]))
    {
      *lastError = ERR_POINT;
      return;
    }
  output[*j] = expression[*i];
  (*j)++;
  if ((expression[*i + 1] != '(' && isoper(expression[*i + 1]) != 0) || isspace((unsigned char)expression[*i + 1]))
  {
    output[*j] = ' ';
    (*j)++;
    *previous = NUMBER;
  }
  else
    *previous = DOT;
}
void ECase(char const *expression, char *output, int* i, int* j, prev_t* previous, error_t* lastError)
{
  if (*previous == FUNCTION) //&& expression[*i - 1] != ' ')
  {
    *lastError = ERR_EXPRESSION;
    return;
  }
  if ((*previous == EMPTY || *previous == OPEN_BRACKET || *previous == FUNCTION || *previous == OPERATION) && !isdigit((unsigned char)expression[*i + 1]))
  {
    output[*j] = 'e';
    (*j)++;
    output[*j] = ' ';
    (*j)++;
    *previous = CONST_E;
  }
  else if ((*previous == DOT || *previous == NUMBER) && !isspace((unsigned char)expression[*i + 1]) && !isspace((unsigned char)expression[*i - 1]))
  {
    output[*j] = 'E';
    (*j)++;
    if (!isdigit((unsigned char)expression[*i + 1]) && (expression[*i + 1] != '+' && expression[*i + 1] != '-'))
    {
      *lastError = ERR_E;
      return;
    }
    if (expression[*i + 1] == '+' || expression[*i + 1] == '-')
    {
      (*i)++;
      output[*j] = expression[*i];
      (*j)++;
    }
    if (isoper(expression[*i + 1]) || isspace((unsigned char)expression[*i + 1]) || expression[*i + 1] == '\0')
    {
      *lastError = ERR_E;
      return;
    }
    while (isdigit((unsigned char)expression[*i + 1]))
    {
      (*i)++;
      output[*j] = expression[*i];
      (*j)++;
    }
    if ((!isoper(expression[*i + 1]) || expression[*i + 1] == '(') && !isspace((unsigned char)expression[*i + 1]) && expression[*i + 1] != '\0')
    {
      *lastError = ERR_E;
      return;
    }
    output[*j] = ' ';
    (*j)++;
    *previous = SCI_NUMBER;
  }
  else
  {
    *lastError = ERR_E;
    return;
  }
}
void OperCase(char const *expression, char *output, char* stack, int* i, int* j, int* k, prev_t previous, error_t* lastError)
{
  int priority = 0, unarycase = 0;
  if (expression[*i] == '-')
  {
    unarycase = FindUnary(expression, i, &previous, lastError);
    if (unarycase == 1)
      priority = 2;
    if (*lastError != ERR_OK)
      return;
  }
  if (*k && stack[*k - 1] != '(')
  {
    if (priority == 2 || isoper(expression[*i]) == 1)
    {
      if (priority != 2)
        priority = 1;
      while ((priority > isoper(stack[*k - 1])) && *k > 0 && (previous != FUNCTION && isoper(stack[*k - 1]) != 1))
      {
        (*k)--;
        output[*j] = stack[*k];
        (*j)++;
        output[*j] = ' ';
        (*j)++;
      }
    }
    else
    {
      if (unarycase == 2)
        (*i)++;
      int oper = isoper(expression[*i]);
      if (unarycase == 2 && oper == 5)
        oper = 0;
      if (unarycase == 2 && (previous == NUMBER || previous == CONST_PI || previous == CONST_E || previous == SCI_NUMBER || previous == CLOSE_BRACKET))
        oper = 4;
      while ((oper >= isoper(stack[*k - 1])) && *k > 0)
      {
        (*k)--;
        output[*j] = stack[*k];
        (*j)++;
        output[*j] = ' ';
        (*j)++;
      }
      if (unarycase == 2)
        (*i)--;
    }
  }
  if (priority == 2)
  {
    stack[*k] = '!';
    (*k)++;
  }
  else if (unarycase == 2 && (previous == NUMBER || previous == CONST_PI || previous == CONST_E || previous == SCI_NUMBER || previous == CLOSE_BRACKET))
  {
    stack[*k] = '+';
    (*k)++;
  }
  else if (unarycase != 2)
  {
    stack[*k] = expression[*i];
    (*k)++;
  }
}
void Parse(char const *expression, char *output, error_t* lastError)
{
  int i = 0, j = 0, k = 0, size = 2 * MEM_BLOCK, digit = 0;
  prev_t previous = EMPTY;
  char *stack = NULL, *reallptr = NULL;
  stack = (char*)malloc(sizeof(char)*size);
  if (stack == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    return;
  }
  while (expression[i])
  {
    if (isdigit((unsigned char)expression[i]))
    {
      DigitCase(expression, output, &i, &j, &previous, &digit, lastError);
      if (*lastError != ERR_OK)
      {
        free(stack);
        return;
      }
    }
    else if (expression[i] == '.')
    {
      PointCase(expression, output, &i, &j, &previous, &digit, lastError);
      if (*lastError != ERR_OK)
      {
        free(stack);
        return;
      }
    }
    else if (isalpha((unsigned char)expression[i]))
    {
      if (expression[i] == 'p' && expression[i + 1] == 'i') //|| (expression[i] == 'P' && (expression[i + 1] == 'I' || expression[i + 1] == 'i')))
      {
        if (previous == NUMBER || previous == CLOSE_BRACKET || previous == DOT || previous == CONST_PI || previous == CONST_E || previous == SCI_NUMBER)
        {
          *lastError = ERR_SIGNS;
          free(stack);
          return;
        }
        if (previous == FUNCTION) //&& expression[i - 1] != ' ')
        {
          *lastError = ERR_EXPRESSION;
          free(stack);
          return;
        }
        output[j] = 'p';
        j++;
        output[j] = ' ';
        j++;
        i++;
        previous = CONST_PI;
      }
      else if (expression[i] == 'e' || expression[i] == 'E')
      {
        ECase(expression, output, &i, &j, &previous, lastError);
        if (*lastError != ERR_OK)
        {
          free(stack);
          return;
        }
      }
      else if (isfunc(expression + i))
      {
        if (previous == NUMBER || previous == FUNCTION /*&& expression[i - 1] != ' ')*/ || previous == CLOSE_BRACKET || previous == DOT || previous == CONST_PI || previous == CONST_E || previous == SCI_NUMBER)
        {
          *lastError = ERR_EXPRESSION;
          free(stack);
          return;
        }
        FunToStack(&stack[k], isfunc(expression + i));
        k++;
        Shift(&i, isfunc(expression + i));
        previous = FUNCTION;
      }
      else
      {
        *lastError = ERR_EXPRESSION;
        free(stack);
        return;
      }
    }
    else if (expression[i] == '(')
    {
      if (previous == NUMBER || previous == CLOSE_BRACKET || previous == DOT || previous == CONST_PI || previous == CONST_E || previous == SCI_NUMBER)
      {
        *lastError = ERR_SIGNS;
        free(stack);
        return;
      }
      stack[k] = expression[i];
      k++;
      previous = OPEN_BRACKET;
    }
    else if (expression[i] == ')')
    {
      k--;
      if (k < 0 || previous == EMPTY || previous == FUNCTION || previous == OPERATION)
      {
        *lastError = ERR_BRACKETS;
        free(stack);
        return;
      }
      if (previous == OPEN_BRACKET)
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
      previous = CLOSE_BRACKET;
    }
    else if (isoper(expression[i]) && expression[i] != '!')
    {
      if (previous == DOT || previous == FUNCTION) //&& expression[i - 1] != ' ')
      {
        *lastError = ERR_EXPRESSION;
        free(stack);
        return;
      }
      OperCase(expression, output, stack, &i, &j, &k, previous, lastError);
      if (*lastError != ERR_OK)
      {
        free(stack);
        return;
      }
      previous = OPERATION;
    }
    else if (isspace((unsigned char)expression[i]));
    else
    {
      *lastError = ERR_EXPRESSION;
      free(stack);
      return;
    }
    i++;
    if (k >= size)
    {
      size += MEM_BLOCK;
      if ((reallptr = (char*)realloc(stack, size)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        free(stack);
        return;
      }
      else
        stack = reallptr;
    }
  }
  k--;
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
  free(stack);
  output[j] = '\0';
}
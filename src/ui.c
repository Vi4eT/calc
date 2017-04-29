#include "calc.h"
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
  else if (error == ERR_BRACKETS)
    return "Brackets.";
  else if (error == ERR_EXPRESSION)
    return "Wrong expression.";
  else if (error == ERR_DOMAIN)
    return "Out of domain.";
  else if (error == ERR_SIGNS)
    return "Not enough arithmetic signs.";
  else if (error == ERR_E)
    return "Incorrect usage of \"e\" sign.";
  else if (error == ERR_POINT)
    return "Incorrect point usage.";
  else if (error == ERR_MINUS)
    return "Incorrect minus usage.";
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
int isempty(char* line)
{
  int i = 0;
  for (i = 0; line[i] != 0; i++)
  {
    if (isspace(line[i]) == 0)
      return 0;
  }
  return 1;
}
int iscomment(char* line)
{
  int i;
  for (i = 0; line[i] != 0; i++)
  {
    if (line[i] == '/'&&line[i + 1] == '/')
      return 1;
  }
  return 0;
}
char* ReadLine(FILE* in, error_t* lastError)
{
  int i = 0, n = MEM_BLOCK;
  char* line = NULL, *realltmp = NULL, *err = (char*)1;
  char c = 0;
  line = (char*)malloc(sizeof(char)*n);
  if (line == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    ReportError(*lastError);
    return err;
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
        return err;
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
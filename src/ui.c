#include "main.h"
#include "ui.h"
#include "calculate.h"
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
  else
    return "No error code!";
}
error_t ReportError(error_t error)
{
  printf("ERROR: %s\n", GetErrorString(error));
  return error;
}
int iscomment(char const* line)
{
  int i;
  for (i = 0; line[i] != '\0'; i++)
  {
    if (!isspace((unsigned char)line[i]))
      if (line[i] == '/' && line[i + 1] == '/')
        return 1;
      else
        return 0;
  }
  return 2;
}
char* ReadLine(FILE* in, error_t* lastError)
{
  int i = 0, size = MEM_BLOCK, c = 0;
  char* line = NULL, *reallptr = NULL, *err = (char*)1;
  line = (char*)malloc(sizeof(char) * size);
  if (line == NULL)
  {
    *lastError = ERR_NOT_ENOUGH_MEMORY;
    do
      c = fgetc(in);
    while (c != '\n' && c != EOF);
    if (c == EOF)
      return NULL;
    return err;
  }
  while (c != '\n' && c != EOF)
  {
    for (; i < size; i++)
    {
      c = fgetc(in);
      line[i] = (char)c;
      if (c == '\n' || c == EOF)
        break;
    }
    if (c != '\n' && c != EOF)
    {
      size += MEM_BLOCK;
      if ((reallptr = (char*)realloc(line, size)) == NULL)
      {
        *lastError = ERR_NOT_ENOUGH_MEMORY;
        free(line);
        do
          c = fgetc(in);
        while (c != '\n' && c != EOF);
        if (c == EOF)
          return NULL;
        return err;
      }
      else
        line = reallptr;
    }
  }
  line[i] = '\0';
  if (c == EOF && i == 0)
  {
    free(line);
    return NULL;
  }
  return line;
}
void ProcessLine(char const* line, error_t* lastError)
{
  if (iscomment(line))
    puts(line);
  else
  {
    printf("%s == ", line);
    double result = Calculate(line, lastError);
    if (*lastError == ERR_OK)
      printf("%g\n", result);
    else
      ReportError(*lastError);
  }
}
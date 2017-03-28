#pragma warning(disable:4996)
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <crtdbg.h>
#define MEM_BLOCK 8
//////////////////////////////////////////////////////////////////////////////
// Dummy calc module
// TODO: Move to a separate module 'calc'
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  ERR_CANNOT_OPEN,
  ERR_ARGUMENTS,
  
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
  else
    return "Success!";
}
error_t ReportError(error_t error)
{
  printf("ERROR: %s\n", GetErrorString(error));
  return error;
}
// TODO: Move to a separate module 'calc'
double Calculate(char const* expression)
{
  error_t lastError = ERR_OK;
  double result;
  if(expression);//temp
  // TODO: Replace with a computational algorithm subdivided into modules/functions
  result = 4.0;
  if(lastError==ERR_OK)
    return result;
  else
  {
    ReportError(lastError);
    return lastError;
  }
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
char* ReadLine(FILE* in, error_t lastError)
{
  int i = 0, n = MEM_BLOCK;
  char* line = NULL, *realltmp = NULL;
  char c = 0;
  line = (char*)malloc(sizeof(char)*n);
  if (line == NULL)
  {
    lastError = ERR_NOT_ENOUGH_MEMORY;
    ReportError(lastError);
  }
  while (c != '\n' && c != EOF)
  {
    for (;i < n;i++)
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
        lastError = ERR_NOT_ENOUGH_MEMORY;
        ReportError(lastError);
      }
      else
        line = realltmp;
    }
  }
  line[i] = 0;
  return line;
}
void ProcessLine(char const* line, FILE* in)
{
  error_t lastError = ERR_OK;
  printf("%s == ", line);
  double result = Calculate(line);
  if (lastError == ERR_OK)
    printf("%lg", result);
  else
    ReportError(lastError);
  if (!feof(in))
    printf("\n");
}
void print(char* line, FILE* in)
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
    ProcessLine(line, in);
}
/*int NeedCalculate(char const* line)
{
  // TODO: Determine if the line contains an expression
  UNUSED_PARAMETER(line);
  return 1;
}*/
//need it?
int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  char* line = NULL;
  error_t lastError = ERR_OK;
  if (argc > 1 && (in = fopen(argv[1], "r")) == NULL)// Choose an input source
  {
    lastError = ERR_CANNOT_OPEN;
    ReportError(lastError);
  }
  if (argc > 2)
  {
    lastError = ERR_ARGUMENTS;
    ReportError(lastError);
  }
  while ((line = ReadLine(in, lastError)) != NULL)// Process the data line by line
  {
    if (lastError != ERR_OK)
    {
      lastError = ERR_OK;
      continue;
    }
    print(line, in);
    free(line);
    if (feof(in))
      break;
  }
  if (in != stdin)// Clean up
    fclose(in);
  _CrtDumpMemoryLeaks();
  return 0;
}
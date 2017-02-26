#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MEM_BLOCK 8
// TODO: Include local header files

// TODO: Use all unused parameters and remove this temporary definition
// Helps eliminating warning C4100: 'error': unreferenced formal parameter
#define UNUSED_PARAMETER(name) (void)name

//////////////////////////////////////////////////////////////////////////////
// Dummy calc module

// TODO: Move to a separate module 'calc'
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  // TODO: Add your own error codes
} error_t;

// TODO: Move to a separate module 'calc'
char const* GetErrorString(error_t error)
{
  // TODO: Find the corresponding error description
  UNUSED_PARAMETER(error);
  return "";
}

// TODO: Move to a separate module 'calc'
double Calculate(char const* expression, error_t* error)
{
  double result;

  // TODO: Replace with a computational algorithm subdivided into modules/functions
  UNUSED_PARAMETER(expression);
  result = 4.0;
  if (error != NULL)
    *error = ERR_OK;

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
char* ReadLine(FILE* in)
{
  int i = 0, n = MEM_BLOCK;
  char* line = NULL, *realltmp = NULL;
  char c = 0;
  // TODO: Read a line of text into a dynamic memory block
  //UNUSED_PARAMETER(in);
  line = (char*)malloc(sizeof(char)*n);
  if (line == NULL)
  {
    printf("ERROR: Not enough memory.");
    exit(1);
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
        printf("ERROR: Not enough memory.");
        exit(2);
      }
      else
        line = realltmp;
    }
  }
  line[i] = 0;
  return line;
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
    if (feof(in))
      printf("%s == 0", line);
    else
      printf("%s == 0\n", line);
}
int NeedCalculate(char const* line)
{
  // TODO: Determine if the line contains an expression
  UNUSED_PARAMETER(line);
  return 1;
}

error_t ReportError(error_t error)
{
  printf("ERROR: %s\n", GetErrorString(error));
  return error;
}

void ProcessLine(char const* line)
{
  error_t lastError = ERR_OK;
  if (!NeedCalculate(line))
  {
    puts(line);
    return;
  }

  printf("%s == ", line);
  double result = Calculate(line, &lastError);
  if (lastError == ERR_OK)
    printf("%lg\n", result);
  else
    ReportError(lastError);
}

int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  char* line = NULL;
  if (argc > 1 && (in = fopen(argv[1], "r")) == NULL)// Choose an input source
  {
    printf("ERROR: Cannot open file '%s'.\n", argv[1]);
    exit(-1);
  }
  if (argc > 2)
  {
    printf("ERROR: Too many input arguments.");
    exit(3);
  }
  while ((line = ReadLine(in)) != NULL)// Process the data line by line
  {
    print(line, in);
    //ProcessLine(line);
    free(line);
    if (feof(in))
      break;
  }
  if (in != stdin)// Clean up
    fclose(in);
  return 0;
}
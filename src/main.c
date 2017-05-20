#include "calc.h"
int main(int argc, char const* argv[])
{
  FILE* in = stdin;
  char* line = NULL;
  error_t lastError = ERR_OK;
  system("chcp 1251 > nul");
  if (argc > 1 && (in = fopen(argv[1], "r")) == NULL)// Choose an input source
  {
    lastError = ERR_CANNOT_OPEN;
    ReportError(lastError);
    exit(lastError);
  }
  if (argc > 2)
  {
    lastError = ERR_ARGUMENTS;
    ReportError(lastError);
    exit(lastError);
  }
  while ((line = ReadLine(in, &lastError)) != NULL)// Process the data line by line
  {
    if (line == (char*)1)//ReadLine error
      continue;
    if (lastError != ERR_OK)
      lastError = ERR_OK;
    ProcessLine(line, in, &lastError);
    free(line);
    if (feof(in))//always != NULL
      break;
  }
  if (in != stdin)// Clean up
    fclose(in);
  _CrtDumpMemoryLeaks();
  return 0;
}
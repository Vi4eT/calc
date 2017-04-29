#include "calc.h"
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
    if (line == (char*)1)
      continue;
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
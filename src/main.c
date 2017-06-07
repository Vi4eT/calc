#include "main.h"
#include "ui.h"
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
    return 0;
  }
  if (argc > 2)
  {
    lastError = ERR_ARGUMENTS;
    ReportError(lastError);
    return 0;
  }
  while ((line = ReadLine(in, &lastError)) != NULL)// Process the data line by line
  {
    if (lastError != ERR_OK)
      ReportError(lastError);
    else
    {
      ProcessLine(line, &lastError);
      free(line);
    }
    lastError = ERR_OK;
  }
  if (in != stdin)// Clean up
    fclose(in);//in != NULL
  return 0;
}
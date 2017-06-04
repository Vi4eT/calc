#pragma once
char const* GetErrorString(error_t error);
error_t ReportError(error_t error);
int iscomment(char const* line);
char* ReadLine(FILE* in, error_t* lastError);
void ProcessLine(char const* line, error_t* lastError);
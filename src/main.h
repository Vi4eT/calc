#pragma once
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MEM_BLOCK 8
typedef enum
{
  ERR_OK,
  ERR_NOT_ENOUGH_MEMORY,
  ERR_CANNOT_OPEN,
  ERR_ARGUMENTS,
  ERR_OPERANDS,
  ERR_BRACKETS,
  ERR_EXPRESSION,
  ERR_DOMAIN,
  ERR_SIGNS,
  ERR_E,
  ERR_POINT,
  ERR_MINUS
} error_t;
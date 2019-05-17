#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <main.h>

char mdFileName[32];
char htmlFileName[32];

int main()
{
  GREEN("Input file name");
  scanf("%s", mdFileName);
  green("md filename is ");
  CYAN("%s", mdFileName);
  return 0;
}

#include <main.h>

char mdFileName[32];
char htmlFileName[32];

int main()
{
  readFileName();
  openFile();
  return 0;
}

void readFileName()
{
  YELLOW("Your current path: [%s]", getenv("PWD"));
  GREEN("Input file name");
  scanf("%s", mdFileName);
  green("md filename is ");
  CYAN("%s", mdFileName);
}

void openFile()
{
  FILE *fp = fopen(mdFileName, "r");
  if (fp == NULL) {
    RED("[%s] doesn't exist", mdFileName);
  } else {
    GREEN("open [%s] successfully", mdFileName);
  }
}

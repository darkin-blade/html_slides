#include <main.h>

char mdFileName[32];
char htmlFileName[32];
FILE *md;
FILE *html;

int main()
{
  openFile();
  readFile();
  return 0;
}

void openFile()
{
  YELLOW("Your current path: [%s]", getenv("PWD"));

  GREEN("Input markdown file name");
  scanf("%s", mdFileName);
  green("md filename is ");
  CYAN("%s", mdFileName);
  md = fopen(mdFileName, "r");
  if (md == NULL) {
    RED("[%s] doesn't exist", mdFileName);
  } else {
    GREEN("open [%s] successfully", mdFileName);
  }

  GREEN("Input html file name");
  scanf("%s", htmlFileName);
  green("html filename is ");
  CYAN("%s", htmlFileName);
  html = fopen(htmlFileName, "w+");
  if (html == NULL) {
    RED("[%s] open error", htmlFileName);
  } else {
    GREEN("open [%s] successfully", htmlFileName);
  }
}

void readFile()
{
  int i = 0;
  char line[512];// 每一行
  while (fgets(line, 500, md)) {// 读取一行
    WHITE("%s", line);
    for (i = 0; i < (int)strlen(line); i ++) {
      ;
    }
  }
  return;
}

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

  return;// TODO
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
  char render[1024];// 解析后的字符串
  int title = 0;// 标题级数
  int tag = 0;// 缩进级数
  int length = 0;

  while (fgets(line, 500, md)) {// 读取一行
    WHITE("%s", line);
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      RED("%c", line[length - 1]);
      line[length - 1] = '\0';
      length --;
    }
    if (line[0] != ' ') {// 非缩进
      title = 0;
      for (i = 0; line[i] == '#'; i ++) {// 标题
        title ++;// 标题级数增加
      }
      if (line[i] != ' ') {// 判断语法,#后接空格,TODO \n
        title = 0;
      } else {
        i ++;
      }
    } else {
      for (i = 0; line[i] == ' '; i ++) {// 缩进
        tag ++;// 缩进级数增加
      }
    }
    if (title) {// 是标题
      memset(render, 0, sizeof(render));
      sprintf(render, "<h%d>%s</h%d>\n", title, line + i, title);
      MAGENTA("%s", render);
      // fputs(render, html);
    }
  }

  return;
}

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
  int tagStack[8]; // 最多8种缩进
  int tagStackTop = 0;// 栈顶
  int length = 0;

  for (i = 0; i < 8; i ++) {// 初始化tag栈,TODO 有序,无序
    tagStack[i] = -1;
  }

  while (fgets(line, 500, md)) {// 读取一行
    WHITE("%s", line);
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      RED("%c", line[length - 1]);
      line[length - 1] = '\0';
      length --;
    }

    if (length == 0) continue;// 空行

    if (line[0] != ' ') {// 非缩进
      title = 0;
      for (i = 0; line[i] == '#'; i ++) {// 标题
        title ++;// 标题级数增加,TODO 最大标题级数
      }
      if (line[i] != ' ') {// 判断语法,#后接空格,TODO
        title = 0;// 不合语法
      } else {
        i ++;// 空格
      }
    } else {// 缩进
      for (i = 0; line[i] == ' '; i ++) {
        tag ++;// 缩进级数增加
      }
      if (line[i] != ' ') {// 语法判断
        tag = 0;// 不合语法
      } else {
        i ++;// 空格
        if (tag > tagStack[tagStackTop]) {// 更小一级
          assert(tagStackTop < 8);
          tagStack[tagStackTop] = tag;
          tagStackTop ++;
          assert(tagStack[tagStackTop] == -1);
        } else if (tag == tagStack[tagStackTop]) {// 同级
          assert(tag != -1);
        } else {// 向前回溯
          ;
        }
      }
    }
    if (title) {// 是标题
      tag = 0;// 清空tag
      memset(render, 0, sizeof(render));
      sprintf(render, "<h%d>%s</h%d>\n", title, line + i, title);
      MAGENTA("%s", render);
      // fputs(render, html);
    } else if (tag) {// 有缩进
      ;
    }
  }

  return;
}

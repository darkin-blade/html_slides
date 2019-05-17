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
  // scanf("%s", htmlFileName);// TODO
  sprintf(htmlFileName, "test.html");// TODO
  green("html filename is ");
  CYAN("%s", htmlFileName);
  html = fopen(htmlFileName, "w+");
  if (html == NULL) {
    RED("[%s] open error", htmlFileName);
  } else {
    GREEN("open [%s] successfully", htmlFileName);
  }
}

char line[512];// 每一行
int length = 0;// 每一行的长度
char render[1024];// 解析后的字符串
int tag = 0;// 缩进级数
int tagStack[8]; // 最多8种缩进
int tagStackTop = 0;// 栈顶
int typeStack[8]; // 1 for ul, 2 for ol
char clear[64];// 存放</ol> </ul>

void readFile()
{
  int i = 0;

  header();
  while (fgets(line, 500, md)) {// 读取一行
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      line[length - 1] = '\0';
      length --;
    }
    if (length == 0) continue;// 空行

    if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } else {
      for (i = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }

      if (line[i] == '-' && line[i + 1] == ' ') {// 无序表
        isUL();
      } else if (line[i] >= '1' && line[i] <= '9' && line[i + 1] == '.' 
          && line[i + 2] == ' ') {// 有序表
        isOL();
      }
    }
    WHITE("%s", line);
    MAGENTA("%s", render);
    fputs(render, html);
  }
  footer();

  return;
}

void isTitle()
{
  int i = 0;
  int title = 0;// 标题级数
  tag = 0;// 清空tag
  clearTag();// 清空之前的ul,ol

  for (i = 0; line[i] == '#'; i ++) {
    title ++;// 标题级数增加,TODO 最大标题级数
  }
  assert(i == title);
  while (line[i] == ' ') i ++;// 除去空格
  length = strlen(line + i);

  if (length == 0 || i == title) {// 不合语法
    title = 0;
    isPlain();
  } else {
    sprintf(render, "%s<h%d>%s</h%d>\n", clear, title, line + i, title);
  }
}

void isUL()
{
  int i = tag + 1;// 跳过 "-"
  assert(tag != -1);

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == tag) {// 不合语法
    isPlain();
  }

  if (tag > tagStack[tagStackTop]) {// 更小一级
    assert(tagStackTop < 8);
    tagStack[tagStackTop] = tag;
    typeStack[tagStackTop] = 1;// ul
    tagStackTop ++;
    assert(tagStack[tagStackTop] == -1);
    assert(typeStack[tagStackTop] == 0);

    sprintf(render, "<ul>\n<li>\n%s\n</li>\n", line + i);// TODO
  } else if (tag == tagStack[tagStackTop]) {// 同级
    sprintf(render, "<li>\n%s</li>\n", line + i);
  } else {// 向前回溯
    clearTag();
    sprintf(render, "%s<li>\n%s\n</li>\n", clear, line + i);
  }
}

void isOL()
{
  assert(0);// TODO
}

void isPlain()
{
  sprintf(render, "%s\n", line + tag);// TODO
}

void clearTag()
{
  if (tagStackTop == 0) return;// 没有tag
  for (memset(clear, 0, sizeof(clear)); 
      tagStackTop >= 1 && tag < tagStack[tagStackTop - 1]; tagStackTop --) 
  {
    if (typeStack[tagStackTop - 1] == 1) {// ul
      strcat(clear, "</ul>\n");
    } else {// ol
      assert(typeStack[tagStackTop - 1] == 2);
      strcat(clear, "</ol>\n");
    }
    tagStack[tagStackTop - 1] = -1;// 复原
    typeStack[tagStackTop - 1] = 0;// 复原
  }
  assert(tagStackTop >= 0);
}

void header()
{
  int i = 0;
  for (i = 0; i < 8; i ++) {// 初始化tag栈,TODO 有序,无序
    tagStack[i] = -1;
    typeStack[i] = 0;
  }

  sprintf(render, "<html>\n<head>\n</head>\n<body>\n");
  MAGENTA("%s", render);
  fputs(render, html);
}

void footer()
{
  tag = 0;
  clearTag();
  sprintf(render, "%s</body>\n</html>\n", clear);
  MAGENTA("%s", render);
  fputs(render, html);
}

#include <main.h>

#define MAX_LINE 1024
// 原文件一行的长度
#define MAX_REND 2048
// 渲染之后一行的长度
#define MAX_TAG 8
// 无序/有序表缩进级数
#define MAX_NAME 32
// 文件名长度
#define MAX_CLEAR 64
// 清除<ul><ol><p>的最大长度

char mdFileName[32];
char htmlFileName[32];
FILE *md;
FILE *html;

char line[1024];// 每一行
int length = 0;// 每一行的长度
char render[2048];// 解析后的字符串

int tag = -1;// 缩进级数(空格数)
int tagStack[8]; // 最多8种不同缩进
int evnStack[8]; // 语言环境栈
// 0: default, 1: title, 2: paragraph, 3: blockquote, 4: ul, 5: ol
int stackTop = 0;// 语言环境栈顶
char clear_text[64];// 清除语言环境
int textEvn = 0;// 当前语言环境 
// 0: plain, 1: title, 2: paragraph, 3: blockquote, 4: ul, 5: ol
int paraEvn = 0;// 每一段的语言环境
// 0: normal, 1: em, 2: strong
int latexEvn = 0;// latex语言环境
// 0: none, 1: $latex$, 2: $$latex$$
int codeEvn = 0;
// 0: none, 1: `code`, 2: ``code``

int escape = 0;// 转义'\'
char escp_char[32] = "*_\\";
int paragraph = 0;// 0: 没有新的段落, 1: 有段落没有结束
int slide_num = 0;// 当前slide序号,初始0

void isTitle()
{
  int i = 0;
  int title = 0;// 标题级数
  tag = -1;// 清空tag
  clearTag();// 清空之前的ul,ol

  for (i = 0; line[i] == '#'; i ++) {
    title ++;// 标题级数增加,TODO 最大标题级数
  }
  assert(i == title);
  while (line[i] == ' ') i ++;// 除去空格
  length = strlen(line + i);

  if (length == 0 || i == title) {// 不合语法
    assert(tag == -1);// 注意初始化-1
    textRend();
  } else {
    endEvn();// 结束之前的段落
    sprintf(render, "%s<h%d>%s</h%d>\n", clear_text, title, line + i, title);
  }
}

void isUL()
{
  int i = tag + 1;// 跳过 "-"
  assert(tag != -1);

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == tag + 1) {// 不合语法
    textRend();
    return;// TODO
  } else {
    if (textEvn != 4 && textEvn != 5) {// 之前不是表环境
      endEvn();// 结束之前的段落
    }
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n%s</li>\n", line + i);
  } else {
    clearTag();// 向前回溯
    if ((stackTop == 0)||(tag > tagStack[stackTop - 1]))
    {// 没有找到符合之前级数的缩进/更小一级
      assert(stackTop < 8);
      tagStack[stackTop] = tag;
      evnStack[stackTop] = 1;// ul
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);
      sprintf(render, "%s<ul>\n<li>\n%s\n</li>\n", clear_text, line + i);// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "%s<li>\n%s\n</li>\n", clear_text, line + i);
    }
  }
}

void isOL()
{
  int i = tag;// 不跳过
  int j = 0;
  int num = 0;
  assert(tag != -1);
  while (line[i] >= '1' && line[i] <= '9') {
    num = num * 10 + line[i] - 48;
    i ++;
  }
  if (line[i] != '.' || line[i + 1] != ' ') {
    textRend();// 不合语法
    return;// TODO
  } else {
    i ++;// 跳转至空格处
    j = i;
  }

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == j) {// 不合语法,TODO
    textRend();
    return;// TODO
  } else {
    endEvn();// 结束之前的段落
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    YELLOW("%s", line);
    assert(stackTop != 0);
    sprintf(render, "<li>\n%s</li>\n", line + i);
  } else {
    clearTag();// 向前回溯
    assert(stackTop >= 0);

    if ((stackTop == 0)||(tag > tagStack[stackTop - 1]))
    {// 没有找到符合之前级数的缩进/更小一级
      assert(stackTop < 8);
      tagStack[stackTop] = tag;
      evnStack[stackTop] = 2;// ol
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);
      sprintf(render, "%s<ol start=\"%d\">\n<li>\n%s\n</li>\n",
          clear_text, num, line + i);// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "%s<li>\n%s\n</li>\n", clear_text, line + i);
    }
  }
}

void clearTag()
{
  memset(clear_text, 0, sizeof(clear_text));
  if (stackTop == 0) return;// 没有tag
  for (; stackTop >= 1 && tag < tagStack[stackTop - 1]; stackTop --) {
    if (evnStack[stackTop - 1] == 1) {// ul
      strcat(clear_text, "</ul>\n");
    } else {// ol
      assert(evnStack[stackTop - 1] == 2);
      strcat(clear_text, "</ol>\n");
    }
    tagStack[stackTop - 1] = -1;// 复原
    evnStack[stackTop - 1] = 0;// 复原
  }
  if (tag == 0) {// 没有缩进时不能多li
    assert(stackTop <= 1);
    for (tag = -1; 
        stackTop >= 1 && tag < tagStack[stackTop - 1]; stackTop --) 
    {
      if (evnStack[stackTop - 1] == 1) {// ul
        strcat(clear_text, "</ul>\n");
      } else {// ol
        assert(evnStack[stackTop - 1] == 2);
        strcat(clear_text, "</ol>\n");
      }
      tagStack[stackTop - 1] = -1;// 复原
      evnStack[stackTop - 1] = 0;// 复原
    }
    tag = 0;
  }
  assert(stackTop >= 0);
}

void isCodeblock()
{
  int i = tag + 3;// 跳过```
  char language[16];// 语言类型
  clearTag();
  endEvn();// 结束之前的段落

  while (line[i] == ' ') i ++;// 跳过空格
  sprintf(language, "%s", line + i);// 有可能为空
  sprintf(render, "%s<figure class=\"highlight %s\">\n<pre>\n<code>\n",
      clear_text, language);
  WHITE("%s", line);
  MAGENTA("%s", render);
  fputs(render, html);

  while (fgets(line, 1000, md)) {// 读取一行
    if (line[0] == '`' && line[1] == '`' && line[2] == '`' && line[3] != '`') {
      sprintf(render, "</code>\n</pre>\n</figure>\n");
      return;// TODO
    } else {// 继续留在代码块
      sprintf(render, "%s", line);// 包括回车,TODO 转义
      WHITE("%s", line);
      MAGENTA("%s", render);
      fputs(render, html);
    }
  }
  assert(0);
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

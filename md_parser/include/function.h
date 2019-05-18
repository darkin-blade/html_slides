#include <main.h>

char mdFileName[32];
char htmlFileName[32];
FILE *md;
FILE *html;

char line[1024];// 每一行
int length = 0;// 每一行的长度
char render[2048];// 解析后的字符串
int tag = -1;// 缩进级数
int tagStack[8]; // 最多8种缩进
int tagStackTop = 0;// 栈顶
int typeStack[8]; // 1 for ul, 2 for ol
char clear[64];// 存放</ol> </ul>
int textEvn;// 当前语言环境 0: normal, 1: code`, 2: code``, 3: latex$, 4: latex$$
int paragraph;// 0: 没有新的段落, 1: 有段落没有结束
struct {
  int id, len;// 序号,长度
  char left[4];
  char right[4];
} latexPar[4];// latex公式环境

void endPara()
{
  if (paragraph == 1) {// 有段落未结束
    sprintf(render, "\n</p>\n");// TODO
    MAGENTA("%s", render);
    fputs(render, html);
    paragraph = 0;// 之后的文字属于新的段落
  } else {
    assert(paragraph == 0);// 什么也不做
  }
}

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
    isText();
  } else {
    endPara();// 结束之前的段落
    sprintf(render, "%s<h%d>%s</h%d>\n", clear, title, line + i, title);
  }
}

void isUL()
{
  int i = tag + 1;// 跳过 "-"
  assert(tag != -1);

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == tag + 1) {// 不合语法
    isText();
    return;// TODO
  } else {
    endPara();// 结束之前的段落
  }

  if ((tagStackTop >= 1)&&(tag == tagStack[tagStackTop - 1])) {// 同级
    assert(tagStackTop != 0);
    sprintf(render, "<li>\n%s</li>\n", line + i);
  } else {
    clearTag();// 向前回溯
    if ((tagStackTop == 0)||(tag > tagStack[tagStackTop - 1]))
    {// 没有找到符合之前级数的缩进/更小一级
      assert(tagStackTop < 8);
      tagStack[tagStackTop] = tag;
      typeStack[tagStackTop] = 1;// ul
      tagStackTop ++;
      assert(tagStack[tagStackTop] == -1);
      assert(typeStack[tagStackTop] == 0);
      sprintf(render, "%s<ul>\n<li>\n%s\n</li>\n", clear, line + i);// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[tagStackTop - 1]);
      sprintf(render, "%s<li>\n%s\n</li>\n", clear, line + i);
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
    isText();// 不合语法
    return;// TODO
  } else {
    i ++;// 跳转至空格处
    j = i;
  }

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == j) {// 不合语法,TODO
    isText();
    return;// TODO
  } else {
    endPara();// 结束之前的段落
  }

  if ((tagStackTop >= 1)&&(tag == tagStack[tagStackTop - 1])) {// 同级
    YELLOW("%s", line);
    assert(tagStackTop != 0);
    sprintf(render, "<li>\n%s</li>\n", line + i);
  } else {
    clearTag();// 向前回溯
    assert(tagStackTop >= 0);

    if ((tagStackTop == 0)||(tag > tagStack[tagStackTop - 1]))
    {// 没有找到符合之前级数的缩进/更小一级
      assert(tagStackTop < 8);
      tagStack[tagStackTop] = tag;
      typeStack[tagStackTop] = 2;// ol
      tagStackTop ++;
      assert(tagStack[tagStackTop] == -1);
      assert(typeStack[tagStackTop] == 0);
      sprintf(render, "%s<ol start=\"%d\">\n<li>\n%s\n</li>\n",
          clear, num, line + i);// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[tagStackTop - 1]);
      sprintf(render, "%s<li>\n%s\n</li>\n", clear, line + i);
    }
  }
}

void clearTag()
{
  memset(clear, 0, sizeof(clear));
  if (tagStackTop == 0) return;// 没有tag
  for (; tagStackTop >= 1 && tag < tagStack[tagStackTop - 1]; tagStackTop --) {
    if (typeStack[tagStackTop - 1] == 1) {// ul
      strcat(clear, "</ul>\n");
    } else {// ol
      assert(typeStack[tagStackTop - 1] == 2);
      strcat(clear, "</ol>\n");
    }
    tagStack[tagStackTop - 1] = -1;// 复原
    typeStack[tagStackTop - 1] = 0;// 复原
  }
  if (tag == 0) {// 没有缩进时不能多li
    assert(tagStackTop <= 1);
    for (tag = -1; 
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
    tag = 0;
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
  if (0) {// 初始化latex环境
    latexPar[0].id = 0;
    latexPar[0].len = 1;
    sprintf(latexPar[0].left, "$");
    sprintf(latexPar[0].right, "$");
    latexPar[1].id = 1;
    latexPar[1].len = 2;
    sprintf(latexPar[1].left, "$$");
    sprintf(latexPar[1].right, "$$");
    // latexPar[2].id = 2;
    // latexPar[2].len = 2;
    // sprintf(latexPar[2].left, "\\[");
    // sprintf(latexPar[2].right, "\\]");
    // latexPar[3].id = 3;
    // latexPar[3].len = 2;
    // sprintf(latexPar[3].left, "\\(");
    // sprintf(latexPar[3].right, "\\)");
  }
  paragraph = 0;// 默认一开始就是新的段落

  char partial[8][16] = {"html", "style", "head", ""};
  char filename[32];
  FILE *fp = NULL;
  int flag = 1;// 所有文件是否成功打开
  for (i = 0; partial[i][0] != 0; i ++) {
    sprintf(filename, "./partial/%s.html", partial[i]);
    fp = fopen(filename, "r");
    if (fp == NULL) {// 打开失败
      flag = 0;
    }
  }

  if (flag == 1) {// 所有文件均存在
    for (i = 0; i < sizeof(partial) / sizeof(char *); i ++) {
      sprintf(filename, "./partial/%s.html", partial[i]);
      fp = fopen(filename, "r");
      assert(fp != NULL);
      while (fgets(line, 1000, fp)) {// 读取一行
        sprintf(render, "%s", line);
        // MAGENTA("%s", render);
        fputs(render, html);
      }
    }
  } else {
    YELLOW("No header module");
    sprintf(render, "<html>\n<head>\n</head>\n<body>\n");
    MAGENTA("%s", render);
    fputs(render, html);
  }
}

void footer()
{
  tag = -1;
  clearTag();
  if (strlen(clear) != 0) {// 如果有表,那么不应该处于段落之中
    assert(paragraph == 0);
  }
  endPara();// 注意endPara会直接写入文件

  char partial[8][16] = {"js", "katex", "body"};
  char filename[32];
  FILE *fp = NULL;
  int flag = 1;// 所有文件是否成功打开
  for (i = 0; partial[i][0] != 0; i ++) {
    sprintf(filename, "./partial/%s.html", partial[i]);
    fp = fopen(filename, "r");
    if (fp == NULL) {// 打开失败
      flag = 0;
    }
  }

  if (flag == 1) {// 所有文件均存在
    for (i = 0; i < sizeof(partial) / sizeof(char *); i ++) {
      sprintf(filename, "./partial/%s.html", partial[i]);
      fp = fopen(filename, "r");
      assert(fp != NULL);
      while (fgets(line, 1000, fp)) {// 读取一行
        sprintf(render, "%s", line);
        // MAGENTA("%s", render);
        fputs(render, html);
      }
    }
  } else {
    YELLOW("No footer module");
    sprintf(render, "%s</body>\n</html>\n", clear);
    MAGENTA("%s", render);
    fputs(render, html);
  }
}

void isCodeblock()
{
  int i = tag + 3;// 跳过```
  char language[16];// 语言类型
  clearTag();
  endPara();// 结束之前的段落

  while (line[i] == ' ') i ++;// 跳过空格
  sprintf(language, "%s", line + i);// 有可能为空
  sprintf(render, "%s<figure class=\"highlight %s\">\n<pre>\n<code>\n",
      clear, language);
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

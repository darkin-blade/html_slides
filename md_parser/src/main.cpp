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

char line[1024];// 每一行
int length = 0;// 每一行的长度
char render[2048];// 解析后的字符串
int tag = -1;// 缩进级数
int tagStack[8]; // 最多8种缩进
int tagStackTop = 0;// 栈顶
int typeStack[8]; // 1 for ul, 2 for ol
char clear[64];// 存放</ol> </ul>

void readFile()
{
  int i = 0;

  header();
  while (fgets(line, 1000, md)) {// 读取一行
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      line[length - 1] = '\0';
      length --;
    }
    if (length == 0) continue;// 空行

    if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } else {
      for (i = 0, tag = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }
      assert(i == tag);

      if (line[i] == '-' && line[i + 1] != '\0') 
      {// 无序表,TODO 条件
        isUL();
      } 
      else if (line[i] >= '1' && line[i] <= '9' && line[i + 1] != '\0') 
      {// 有序表
        isOL();
      } 
      else if (line[i] == '`' && line[i + 1] == '`'
          && line[i + 2] == '`' && line[i + 3] != '`') 
      {// 大代码块
        isCode();
        continue;
      } 
      else 
      {// 正文
        tag = -1;
        isPlain();
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

  if (length == 0 || i == tag + 1) {// 不合语法
    isPlain();
    return;// TODO
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
    isPlain();// 不合语法
    return;// TODO
  } else {
    i ++;// 跳转至空格处
    j = i;
  }

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == j) {// 不合语法,TODO
    isPlain();
    return;// TODO
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

void isPlain()
{
  int i = my_max(tag, 0);
  tag = -1;
  clearTag();
  sprintf(render, "%s<p>%s</p>\n", clear, line + i);// TODO
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

  sprintf(render, "<html>\n<head>\n</head>\n<body>\n");
  MAGENTA("%s", render);
  fputs(render, html);
}

void footer()
{
  tag = -1;
  clearTag();
  sprintf(render, "%s</body>\n</html>\n", clear);
  MAGENTA("%s", render);
  fputs(render, html);
}

void isCode()
{
  int i = tag + 3;// 跳过```
  char language[16];// 语言类型
  clearTag();

  while (line[i] == ' ') i ++;// 跳过空格
  sprintf(language, "%s", line + i);// 有可能为空
  sprintf(render, "%s<figure class=\"highlight %s\">\n<pre>\n<code>\n",
      clear, language);
  WHITE("%s", line);
  MAGENTA("%s", render);
  fputs(render, html);

  while (fgets(line, 1000, md)) {// 读取一行
    if (line[0] == '`' && line[1] == '`' && line[2] == '`' && line[3] != '`') {
      ;
    }
  }
}

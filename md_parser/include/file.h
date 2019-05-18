#ifndef MAIN_H

#include <main.h>

#endif

void header()
{
  int i = 0;
  for (i = 0; i < 8; i ++) {// 初始化tag栈,TODO 有序,无序
    tagStack[i] = -1;
    evnStack[i] = 0;
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
    for (i = 0; partial[i][0] != 0; i ++) {
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
  endTag();
  if (strlen(clear_text) != 0) {// 如果有表,那么不应该处于段落之中
    assert(paragraph == 0);
  }
  endText();// 注意endText会直接写入文件
  sprintf(render, "</div>\n</div>\n");// 结束slide
  MAGENTA("%s", render);// TODO
  fputs(render, html);

  char partial[8][16] = {"js", "katex", "body"};
  char filename[32];
  FILE *fp = NULL;
  int flag = 1;// 所有文件是否成功打开
  int i = 0;
  for (i = 0; partial[i][0] != 0; i ++) {
    sprintf(filename, "./partial/%s.html", partial[i]);
    fp = fopen(filename, "r");
    if (fp == NULL) {// 打开失败
      flag = 0;
    }
  }

  if (flag == 1) {// 所有文件均存在
    for (i = 0; partial[i][0] != 0; i ++) {
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
    sprintf(render, "%s</body>\n</html>\n", clear_text);
    MAGENTA("%s", render);
    fputs(render, html);
  }
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

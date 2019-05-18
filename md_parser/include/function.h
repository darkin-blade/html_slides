#ifndef MAIN_H

#include <main.h>

#endif

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
    endText();// 结束之前的段落
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
    textRend();// TODO
    return;
  } else {
    if (textEvn != 4 && textEvn != 5) {// 之前不是表环境
      endText();// 结束之前的所有环境,TODO
      textEvn = 4;// 进入ul环境
    }
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n%s</li>\n", line + i);// TODO
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
    endText();// 结束之前的段落
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
  endText();// 结束之前的段落

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

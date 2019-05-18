#ifndef MAIN_H

#include <main.h>

#endif

void isTitle()
{
  int i = 0;
  int rend_tail = 0;
  int title = 0;// 标题级数

  for (i = 0; line[i] == '#'; i ++) {
    title ++;// 标题级数增加,TODO 最大标题级数
  }
  assert(i == title);
  while (line[i] == ' ') i ++;// 除去空格
  length = strlen(line + i);

  if (length == 0 || i == title) {// 不合语法
    isPara();
  } else {// TODO,清除多余的环境
    endText();// 清除之前的所有环境
    sprintf(render, "<h%d>", title);
    sprintf(line, "%s", line + i);// 裁剪字符串,TODO
    textRend();
    rend_tail = strlen(render);
    sprintf(render + rend_tail, "</h%d>\n", title);
  }
}

void isUL()
{
  int i = tag + 2;// 跳过 "- ",TODO
  int rend_tail = 0;
  assert(tag != -1);

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0) {// 不合语法;注意如果-之后全是空格也是符合语法的
    assert(i == tag + 2);// TODO
    isPara();// 即使之前有无序表也会被清除
    return;
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    if (textEvn != 4 && textEvn != 5) {// 之前不是表环境
      endText();// 结束之前的所有环境,TODO
      textEvn = 4;// 进入ul环境
    }
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n");
    textRend();
    rend_tail = strlen(render);
    sprintf(render + rend_tail, "</li>\n");// TODO
  } else {
    endTag();// 向前回溯
    if ((stackTop == 0)||(tag > tagStack[stackTop - 1]))
    {// 没有找到符合之前级数的缩进,将tag增加
      assert(stackTop < MAX_TAG);
      tagStack[stackTop] = tag;
      evnStack[stackTop] = 4;// ul
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);

      sprintf(render, "<ul>\n<li>\n");
      textRend();
      rend_tail = strlen(render);
      sprintf(render + rend_tail, "\n</li>\n");// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "<li>\n");
      textRend();
      rend_tail = strlen(render);
      sprintf(render + rend_tail, "\n</li>\n");// TODO
    }
  }
}

void isOL()
{
  int i = tag;// 不跳过
  int j = 0;
  int rend_tail = 0;
  int num = 0;
  assert(tag != -1);
  while (line[i] >= '1' && line[i] <= '9') {// 字符串转数字
    num = num * 10 + line[i] - 48;
    i ++;
  }
  if (line[i] != '.' || line[i + 1] != ' ') {
    isPara();// 不合语法
    return;// TODO
  } else {
    i ++;// 跳转至空格处
    j = i;// 为后面判断空格做准备
  }

  while (line[i] == ' ') i ++;// 清除空格
  length = strlen(line + i);

  if (length == 0 || i == j) {// 不合语法
    isPara();// 不要裁剪字符串,强制进入段落环境(isPara内部实现),TODO
    return;// TODO
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    endText();// 结束之前的段落
    textEvn = 5;// ol
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n");
    textRend();
    rend_tail = strlen(render);
    sprintf(render + rend_tail, "</li>\n");// TODO
  } else {
    endTag();// 向前回溯
    assert(stackTop >= 0);
    if ((stackTop == 0)||(tag > tagStack[stackTop - 1]))
    {// 没有找到符合之前级数的缩进->增加tag级数
      assert(stackTop < MAX_TAG);
      tagStack[stackTop] = tag;
      evnStack[stackTop] = 5;// ol
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);

      sprintf(render, "<ol start=\"%d\">\n<li>\n", num);
      textRend();
      rend_tail = strlen(render);
      sprintf(render + rend_tail, "\n</li>\n");// TODO
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "<li>\n", num);
      textRend();
      rend_tail = strlen(render);
      sprintf(render + rend_tail, "\n</li>\n");// TODO
    }
  }
}

void isCodeblock()
{
  int i = tag + 3;// 跳过```
  char language[16];// 语言类型
  endTag();
  endText();// 结束之前的段落

  while (line[i] == ' ') i ++;// 跳过空格
  sprintf(language, "%s", line + i);// 有可能为空
  sprintf(render, "%s<figure class=\"highlight %s\">\n<pre>\n<code>\n",
      clear_tag, language);
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

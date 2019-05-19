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
    isPara();// TODO,可以进行衔接
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
  assert(tag != -1);

  length = strlen(line + i);// 不要清除空格,TODO
  if (length == 0) {// 不合语法;注意如果-之后全是空格也是符合语法的
    assert(i == tag + 2);// TODO
    isPara();// TODO,可以进行衔接
    return;
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    if (textEvn != 4 && textEvn != 5) {// 之前不是表环境
      endText();// 结束之前的所有环境,TODO
    } else {
      endTag();// 向前回溯,TODO
    }
    textEvn = 4;// 进入ul环境
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n");
    textRend();
  } else {
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
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "<li>\n");
      textRend();
    }
  }
}

void isOL()
{
  int i = tag;// 不跳过
  int num = 0;
  assert(tag != -1);
  while (line[i] >= '0' && line[i] <= '9') {// 字符串转数字
    num = num * 10 + line[i] - 48;
    i ++;
  }
  if (line[i] != '.' || line[i + 1] != ' ') {
    isPara();// TODO,可以进行衔接
    return;// TODO
  } else {
    i += 2;// 跳转至空格后,注意{num}.后只有空格也是符合语法的
  }

  length = strlen(line + i);// 不要清除空格,TODO
  if (length == 0) {// 不合语法
    isPara();// TODO,可以进行衔接
    return;// TODO
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    if (textEvn != 4 && textEvn != 5) {// 之前不是表环境
      endText();// 结束之前的段落
    } else {
      endTag();// 向前回溯,TODO
    }
    textEvn = 5;// ol
  }

  if ((stackTop >= 1)&&(tag == tagStack[stackTop - 1])) {// 同级
    assert(stackTop != 0);
    sprintf(render, "<li>\n");
    textRend();// TODO
  } else {
    assert(stackTop >= 0);
    if ((stackTop == 0)||(tag > tagStack[stackTop - 1]))
    {// 没有找到符合之前级数的缩进->增加tag级数
      RED("%d", tag);
      assert(stackTop < MAX_TAG);
      tagStack[stackTop] = tag;
      evnStack[stackTop] = 5;// ol
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);

      sprintf(render, "<ol start=\"%d\">\n<li>\n", num);
      textRend();
    } else {// 找到之前的同级
      assert(tag == tagStack[stackTop - 1]);
      sprintf(render, "<li>\n");
      textRend();
    }
  }
}

void isCodeblock()
{
  int i = tag + 3;// 跳过```
  char language[16];// 语言类型
  endText();// 结束之前的所有环境

  while (line[i] == ' ') i ++;// 跳过空格
  sprintf(language, "%s", line + i);// 有可能为空
  sprintf(render, "<figure class=\"highlight %s\">\n<pre>\n<code>\n", language);
  WHITE("[%d] %s", line_num, line);
  MAGENTA("%s", render);
  fputs(render, html);

  while (fgets(line, 1000, md)) {// 读取一行
    line_num ++;// debug
    if (line[0] == '`' && line[1] == '`' && line[2] == '`' && line[3] != '`') {
      sprintf(render, "</code>\n</pre>\n</figure>\n");
      return;// TODO
    } else {// 继续留在代码块
      sprintf(render, "%s", line);// 包括回车,TODO 转义
      WHITE("[%d] %s", line_num, line);
      MAGENTA("%s", render);
      fputs(render, html);
    }
  }
  assert(0);
}

void isSlide()
{
  if (slide_num == 0) {// 文章开头
    sprintf(render, "<div class=\"slide\">\n<div class=\"content\">\n");
    MAGENTA("%s", render);
    fputs(render, html);
  } else {// 需要结束之前的slide
    int i = 0;
    while (line[i] == '-') i ++;// 跳过-
    while (line[i] == ' ') i ++;// 跳过空格
    length = strlen(line + i);

    if (length != 0) {// 不合语法
      isPara();// TODO,可以进行衔接
      return;// TODO
    }

    endText();// 除去之前任何环境
    sprintf(render,
        "</div>\n</div>\n<div class=\"slide\">\n<div class=\"content\">\n");
    // 注意如果不是开头,不能够直接把html写进文件
  }
  slide_num ++;
}

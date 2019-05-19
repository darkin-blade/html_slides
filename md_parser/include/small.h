#ifndef MAIN_H

#include <main.h>

#endif

void isTitle()
{
  int i = 0;
  int title = 0;// 标题级数
  rend_tail = 0;

  for (i = 0; line[i] == '#'; i ++) {
    title ++;// 标题级数增加,TODO 最大标题级数
  }
  assert(i == title);
  while (line[i] == ' ') i ++;// 除去空格
  line_len = strlen(line + i);

  if (line_len == 0 || i == title) {// 不合语法
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

  line_len = strlen(line + i);// 不要清除空格,TODO
  if (line_len == 0) {// 不合语法;注意如果-之后全是空格也是符合语法的
    assert(i == tag + 2);// TODO
    isPara();// TODO,可以进行衔接
    return;
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    if (textEvn != 4 && textEvn != 5 && textEvn != 3) {// 之前不是表环境
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

  line_len = strlen(line + i);// 不要清除空格,TODO
  if (line_len == 0) {// 不合语法
    isPara();// TODO,可以进行衔接
    return;// TODO
  } else {
    sprintf(line, "%s", line + i);// 缩短字符串,TODO
    if (textEvn != 4 && textEvn != 5 && textEvn != 3) {// 之前不是表环境,TODO
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
    line_len = strlen(line + i);

    if (line_len != 0) {// 不合语法
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

void isPara()
{
  if (textEvn == 0) {// 新的段落
    sprintf(render, "<p>\n");
    textEvn = 2;// 进入段落环境
  } else if (textEvn == 2) {// 段落中,TODO
    assert(stackTop == 0);// 处于段落之中不应该有表
    sprintf(render, " ");// 将上一行与此行之间加上空格(用于垃圾katex渲染)
  } else {
    if (empty_line == 1) {// 之前有空行
      endText();// 清除所有环境
      assert(stackTop == 0);
      sprintf(render, "<p>\n");
      textEvn = 2;// 强制新段落
    } else {// 继承之前的环境
      sprintf(render, " ");// 将上一行与此行之间加上空格(用于垃圾katex渲染)
    }
  }
  textRend();
}

void isQuote()
{
  int i = tag;
  int quote = 0;// 引用级数
  for (; line[i] == '>'; i ++) {// 跳过'>'
    quote ++;
  }

  line_len = strlen(line + i);
  if (line_len == 0 || line[i] != ' ') {// 不合语法(>后没有空格)
    isPara();// TODO
  } else {
    i ++;// 跳过1个空格
    sprintf(line, "%s", line + i);// 缩短字符串
  }

  if (textEvn == 3) {// 需要考虑级数
    if ((stackTop >= 1)&&(quote <= tagStack[stackTop - 1])) {// 视作同级blockquote
      isPara();// TODO,那么引用符就没有用了
    } else {// 为空或者级数增加
add_quote:
      tagStack[stackTop] = quote;
      evnStack[stackTop] = 3;// blockquote
      stackTop ++;
      assert(tagStack[stackTop] == -1);
      assert(evnStack[stackTop] == 0);

      sprintf(render, "<blockquote>\n");
      textRend();
    }
  } else if (textEvn == 4 || textEvn == 3 || textEvn == 0) {// 无条件继承环境
    textEvn = 3;
    goto add_quote;// 级数增加
  } else {
    if (textEvn != 2) {// 只有可能是段落了,且没有有序表和无序表
      YELLOW("%d", textEvn);
      assert(0);
    }
    endText();// 清除所有环境
    textEvn = 3;
    goto add_quote;// 级数增加
  }
}


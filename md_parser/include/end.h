#ifndef MAIN_H

#include <main.h>

#endif

void endTag()
{
  assert(paraEvn == 0 && lineEvn == 0);// 表不可能做到换行环境
  if (textEvn == 4 || textEvn == 5) {// 结束上一个ol/ul的li
    sprintf(render, "\n</li>\n");
  }
  MAGENTA("%s", render);
  fputs(render, html);

  memset(render, 0, sizeof(render));
  if (stackTop == 0) return;// 没有tag
  for (; stackTop >= 1 && tag < tagStack[stackTop - 1]; stackTop --) {
    if (evnStack[stackTop - 1] == 4) {// ul
      strcat(render, "</ul>\n");
    } else if (evnStack[stackTop - 1] == 5) {// ol
      strcat(render, "</ol>\n");
    } else {
      assert(evnStack[stackTop - 1] == 3);// blockquote
      assert(tagStack[stackTop - 1] == -2);// TODO
      strcat(render, "</blockquote>");
    }
    tagStack[stackTop - 1] = -1;// 复原
    evnStack[stackTop - 1] = 0;// 复原
  }
  MAGENTA("%s", render);
  fputs(render, html);
  assert(stackTop >= 0);
}

void endText()
{
  endPara();// 先处理行内环境和强调环境
  if (textEvn == 0) {
    assert(stackTop == 0);
    return;// 什么事也不做
  }
  if (textEvn == 2) {// 有段落未结束
    sprintf(render, "\n</p>\n");// TODO
    MAGENTA("%s", render);
    fputs(render, html);
  } else if (textEvn == 3) {// 引用块
    assert(0);// TODO
  } else {// 清除ul,ol,blockquote
    assert(textEvn == 4 || textEvn == 5 || textEvn == 3);// TODO
    int old_tag = tag;// TODO
    tag = -1;
    endTag();
    tag = old_tag;
  }
  assert(stackTop == 0);// 不应有ul,ol
  textEvn = 0;
}

void endPara()
{
  endLine();// 先处理行内环境
  if (textEvn == 3) {// 如果blockquote遭到中断,会触发清除所有格式
    int old_tag = tag;// TODO
    tag = -1;
    endTag();
    tag = old_tag;
    textEvn = 0;// TODO
  }
  if (paraEvn == 0) return;
  RED("should not reach here");// 正常情况下不应该调用此函数
  if (paraEvn == 1) {// em
    sprintf(render, "</em>");
    MAGENTA("%s", render);
    fputs(render, html);// TODO
  } else if (paraEvn == 2) {// strong
    sprintf(render, "</strong>");
    MAGENTA("%s", render);
    fputs(render, html);
  } else {
    assert(paraEvn == 3);
    sprintf(render, "</del>");
    MAGENTA("%s", render);
    fputs(render, html);
  }
}

void endLine()
{
  if (lineEvn == 1 || lineEvn == 2) {// code
    RED("forget code ?");// 正常情况下不应该调用此函数
    sprintf(render, "\n</code>\n");// TODO,换行
  } else if (lineEvn == 3 || lineEvn == 4) {
    assert(0);// TODO,说明公式写的有问题
    assert(lineEvn == 3 || lineEvn == 4);// latex TODO
    sprintf(render, "\n");// TODO,换行
  }
  if (lineEvn != 0) {// lineEvn == 0 不能够直接 return
    assert(0);// TODO
    MAGENTA("%s", render);
    fputs(render, html);
    lineEvn = 0;
  }

  if (textEvn == 2) {// TODO,终止段落
    sprintf(render, "\n</p>\n");
    MAGENTA("%s", render);
    fputs(render, html);
  } else if (textEvn == 3) {// 终止引用
    return;// TODO
  }
}


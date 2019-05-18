#ifndef MAIN_H

#include <main.h>

#endif

void endPara()
{
  if (paraEvn == 0) return;
  RED("forget em / strong ?");// 正常情况下不应该调用此函数
  if (paraEvn == 1) {// em
    sprintf(render, "</em>");
    MAGENTA("%s", render);
    fputs(render, html);// TODO
  } else {// strong
    assert(paraEvn == 2);
    sprintf(render, "</strong>");
    MAGENTA("%s", render);
    fputs(render, html);// TODO
  }
}

void endTag()
{
  assert(paraEvn == 0 && lineEvn == 0);// 表不可能做到换行环境
  memset(render, 0, sizeof(render));// TODO
  if (stackTop == 0) return;// 没有tag
  for (; stackTop >= 1 && tag < tagStack[stackTop - 1]; stackTop --) {
    if (evnStack[stackTop - 1] == 4) {// ul
      strcat(render, "</ul>\n");
    } else {// ol
      assert(evnStack[stackTop - 1] == 5);
      strcat(render, "</ol>\n");
    }
    tagStack[stackTop - 1] = -1;// 复原
    evnStack[stackTop - 1] = 0;// 复原
  }
  if (tag == 0) {// 没有缩进时不能多个li并列
    assert(stackTop <= 1);
    for (tag = -1; stackTop >= 1 && tag < tagStack[stackTop - 1]; stackTop --) {
      if (evnStack[stackTop - 1] == 4) {// ul
        strcat(render, "</ul>\n");
      } else {// ol
        assert(evnStack[stackTop - 1] == 5);
        strcat(render, "</ol>\n");
      }
      tagStack[stackTop - 1] = -1;// 复原
      evnStack[stackTop - 1] = 0;// 复原
    }
    tag = 0;
  }
  MAGENTA("%s", render);
  fputs(render, html);
  assert(stackTop >= 0);
}

void endText()
{
  endLine();// 先处理行内环境
  endPara();// 再处理强调环境
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
  } else {// 清除ul,ol
    assert(textEvn == 4 || textEvn == 5);// 暂未实现title
    int old_tag = tag;
    tag = -1;
    endTag();
    tag = old_tag;
  }
  assert(stackTop == 0);// 不应有ul,ol
  textEvn = 0;
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
    MAGENTA("%s", render);
    fputs(render, html);
    lineEvn = 0;
  }

  if (textEvn == 2) {// TODO
    sprintf(render, "\n</p>\n");// 终止当前段落
    MAGENTA("%s", render);
    fputs(render, html);
    textEvn = 0;
  } else if (textEvn == 3) {
    assert(0);// TODO
    textEvn = 0;
  }
}


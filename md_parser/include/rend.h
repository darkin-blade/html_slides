#ifndef MAIN_H

#include <main.h>

#endif

void charRend(int &i)// 单个 强调 字符
{
  int j = 0;// 不要使用i
  assert(render[rend_tail] == '\0');// TODO
  if (escape == 1) {// 是转义环境
    escape = 0;// 取消转义
    for (j = 0; escp_char[j] != '\0'; j ++) {
      if (line[i] == escp_char[j]) {// 需要进行转义
        render[rend_tail] = line[i];
        rend_tail ++;
        return;
      }
    }
    // 后接非转义字符
    render[rend_tail] = '\\';// 把'\'补上
    rend_tail ++;
    render[rend_tail] = line[i];
    rend_tail ++;
    return;
  } else if (line[i] == '\\') {// 进入转义
    escape = 1;
  } else {
    if (line[i] == '*') {
      if (line[i + 1] == '*') {// strong
        if (paraEvn == 0) {// 进入strong
          strcat(render, "<strong>");
          paraEvn = 2;
        } else if (paraEvn == 2) {// 跳出strong
          strcat(render, "</strong>");
          paraEvn = 0;
        } else {// 多个环境嵌套,未定义行为
          goto normal_char;// TODO
        }
        i ++;// 跳过**
      } else {// em
        if (paraEvn == 0) {// 进入em
          strcat(render, "<em>");
          paraEvn = 1;
        } else if (paraEvn == 1) {// 跳出em
          strcat(render, "</em>");
          paraEvn = 0;
        } else {
          goto normal_char;
        }
      }
      rend_tail += strlen(render + rend_tail);// 刷新长度,TODO
    } else if (line[i] == '_') {
      if (line[i + 1] == '_') {// strong
        if (paraEvn == 0) {// 进入strong
          RED("%d %s", paraEvn, render);
          strcat(render, "<strong>");
          paraEvn = 2;
          YELLOW("%d %s", paraEvn, render);
        } else if (paraEvn == 2) {// 跳出strong
          strcat(render, "</strong>");
          paraEvn = 0;
        } else {
          goto normal_char;
        }
        i ++;// 跳过__
      } else {// em
        if (paraEvn == 0) {// 进入em
          strcat(render, "<em>");
          paraEvn = 1;
        } else if (paraEvn == 1) {// 跳出em
          strcat(render, "</em>");
          paraEvn = 0;
        } else {
          goto normal_char;
        }
      }
      rend_tail += strlen(render + rend_tail);// 刷新长度,TODO
    } else if (line[i] == '~' && line[i + 1] == '~') {// 删除线
      if (paraEvn == 0) {// 进入del
        strcat(render, "<del>");
        paraEvn = 3;
      } else if (paraEvn == 3) {// 跳出del
        strcat(render, "</del>");
        paraEvn = 0;
      } else {// 未定义行为
        goto normal_char;
      }
      i ++;// 跳过~~
      rend_tail += strlen(render + rend_tail);// 刷新长度,TODO
    } else if (line[i] == '<' && isLetter(line[i + 1])) {
      assert(0);// TODO,html内嵌代码
    } else {// 正常字符
normal_char:// 多个环境嵌套,未定义行为
      render[rend_tail] = line[i];
      rend_tail ++;
    }
  }// 末尾填上'\0'在textRend中完成
}

void textRend()// code和latex环境
{
  int i = 0;// 在这个函数里面不会对i的位置进行修正,在调用之间要把字符串缩减,TODO
  escape = 0;// 转义:'\', 0: 之前一个字符不是'\', 1: 之前一个字符是'\'
  // 每新的一行,转义进行刷新
  rend_tail = strlen(render);
  assert(render[rend_tail] == '\0');

  for (; line[i] != '\0'; i ++)
  {
    if (lineEvn != 0) {// code和latex中严禁有链接/图片
      lineRend(i);
    } else if (line[i] == '!' && line[i + 1] == '[') {// 可能是图片
      imgRend(i);
    } else if (line[i] == '[') {
      linkRend(i);
    } else {// TODO
      lineRend(i);
    }
  }
  render[rend_tail] = '\0';
}

void imgRend(int &i)
{
  int j = 0;
  int origin = i;// 原来的i
  memset(link_con, 0, sizeof(link_con));
  for (i += 2;// 跳过'!['
      line[i] != '\0'; i ++)
  {
    if (line[i] == ']' && line[i + 1] == '(') {
      link_con_end = i;// 不包括i
      link_con[j] = '\0';
      goto img_content_found;
    }
    link_con[j] = line[i];
    j ++;
  }
  goto img_content_fail;

img_content_found:// 内容能够匹配
  j = 0;
  memset(link_rel, 0, sizeof(link_rel));
  for (i += 2;// 先跳过']('
      line[i] != '\0'; i ++) {
    if (line[i] == ')') {
      link_rel_end = i;// 不包括i
      link_rel[j] = '\0';// TODO
      goto img_content_succ;// 成功
    }
    link_rel[j] = line[i];// TODO
    j ++;
  }

img_content_fail:// 判定失败
  link_rel_end = 0;// 判定失败,TODO
  for (i = origin; line[i] != '\0'; i ++) {// 要证明嵌套的正确性,TODO
    lineRend(i);
  }
  return;
  assert(0);// should not reach there

img_content_succ:// 成功
  sprintf(render + rend_tail, "<img src=\"%s\" alt=\"%s\">",
    link_rel, link_con);// TODO
  rend_tail += strlen(render + rend_tail);// TODO
  i = link_rel_end;// TODO
  link_rel_end = 0;// TODO
  return;
}

void linkRend(int &i)// 链接
{
  int j = 0;
  int origin = i;// 原来的i
  for (; line[i] != '\0'; i ++)
  {
    if (line[i] == ']' && line[i + 1] == '(') {
      link_con_end = i;// 不包括i
      goto link_content_found;
    }
  }
  goto link_content_fail;

link_content_found:// 内容能够匹配
  memset(link_rel, 0, sizeof(link_rel));
  for (i += 2;// 先跳过']('
      line[i] != '\0'; i ++) {
    if (line[i] == ')') {
      link_rel_end = i;// 不包括i
      link_rel[j] = '\0';// TODO
      goto link_content_succ;// 成功
    }
    link_rel[j] = line[i];// TODO
    j ++;
  }

link_content_fail:// 判定失败
  link_con_end = link_rel_end = 0;// 判定失败
  RED("%s %d %d", line, lineEvn, paraEvn);
  for (i = origin; line[i] != '\0'; i ++) {// 要证明嵌套的正确性,TODO
    lineRend(i);
  }
  return;
  assert(0);// should not reach there

link_content_succ:// 成功
  sprintf(render + rend_tail, "<a href=\"%s\">", link_rel);// TODO
  rend_tail += strlen(render + rend_tail);// TODO
  for (i = origin + 1;// 跳过'['
      i < link_con_end; i ++) {// 渲染内容
    lineRend(i);// TODO
  }
  sprintf(render + rend_tail, "</a>");
  rend_tail += strlen(render + rend_tail);// TODO
  i = link_rel_end;// TODO
  link_con_end = link_rel_end = 0;// TODO
  return;
}

void lineRend(int &i)// 单个code/latex字符
{
  if (lineEvn == 0) {// 正常文本
    if (line[i] == '`') {// 行间代码
      if (line[i + 1] == '`') {
        strcat(render, "<code class=\"code\">");
        rend_tail += strlen(render + rend_tail);// TODO
        i ++;
        lineEvn = 2;// ``code``
      } else {
        strcat(render, "<code class=\"code\">");
        rend_tail += strlen(render + rend_tail);// TODO
        lineEvn = 1;// `code`
      }
    }
    else if (line[i] == '$') {// latex公式
      if (line[i + 1] == '$') {// $$ latex $$
        render[rend_tail] = render[rend_tail + 1] = '$';
        rend_tail += 2;
        i ++;
        lineEvn = 4;
      } else {
        render[rend_tail] = '$';
        rend_tail ++;
        lineEvn = 3;
      }
    }
    else {// TODO
      charRend(i);
    }
  }
  else {// 非正常文本
    if (lineEvn == 1) {
      if (line[i] == '`') {// 解除`code`
        strcat(render, "</code>");
        rend_tail += strlen(render + rend_tail);// TODO
        lineEvn = 0;
      }
    }
    else if (lineEvn == 2) {
      if (line[i] == '`' && line[i + 1] == '`') {// 解除``code``
        strcat(render, "</code>");
        rend_tail += strlen(render + rend_tail);// TODO
        i ++;
        lineEvn = 0;
      }
    } 
    else if (lineEvn == 3) {
      if (line[i] == '$') {// 解除$latex$
        render[rend_tail] = '$';
        rend_tail += 1;
        lineEvn = 0;
      }
    } 
    else if (lineEvn == 4) {
      if (line[i] == '$' && line[i + 1] == '$') {// 解除$$ latex $$
        render[rend_tail] = render[rend_tail + 1] = '$';
        rend_tail += 2;
        i ++;
        lineEvn = 0;
      }
    }
    else {// 不可能有其他环境
      assert(0);// TODO
    }

    if (lineEvn != 0) {
      if (lineEvn == 1 || lineEvn == 2) {// 无视一切其他字符
        render[rend_tail] = line[i];
        rend_tail ++;
      } else {// latex公式需要注意转义
        assert(lineEvn == 3 || lineEvn == 4);
        charRend(i);
      }
    }
  }
  render[rend_tail] = '\0';// TODO
  assert((int)strlen(render) == rend_tail);
}


#include <function.h>

int main()
{
  openFile();
  readFile();
  return 0;
}

void readFile()
{
  int i = 0;

  header();
  isSlide();
  while (fgets(line, 1000, md)) {// 读取一行
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      line[length - 1] = '\0';
      length --;
    }
    if (length == 0) {// 空行 
      endPara();
      continue;
    }

    if (textEvn != 0) {// 有code/latex环境未结束
      isText();
    } else if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } else {
      for (i = 0, tag = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }
      assert(i == tag);


      if (line[i] == '-' && line[i + 1] == '-' && line[i + 2] == '-') {// 分割线
        isSlide();
      } else if (line[i] == '-' && line[i + 1] != '\0') 
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
        isCodeblock();// TODO
      } 
      else 
      {// 正文
        isText();
      }
    }
    WHITE("%s", line);
    MAGENTA("%s", render);
    fputs(render, html);
  }
  footer();

  return;
}

void isText()
{
  int i = my_max(tag, 0);
  if (paragraph == 0) {// 新的段落
    assert(textEvn == 0);
    tag = -1;
    clearTag();// 清空之前的表
    sprintf(render, "%s<p>\n", clear);
    paragraph = 1;
  } else {
    sprintf(render, " ");// TODO
    assert(tagStackTop == 0);// 处于段落之中不应该有表
  }
  int rend_tail = strlen(render);
  assert(render[rend_tail] == '\0');

  for (; line[i] != '\0'; i ++) {
    if (textEvn == 0)
    {// 正常文本
      if (line[i] == '`')
      {// 行间代码
        if (line[i + 1] == '`') {
          strcat(render, "<code class=\"code\">");
          rend_tail += strlen(render + rend_tail);// TODO
          i ++;
          textEvn = 2;// ``code``
        } else {
          strcat(render, "<code class=\"code\">");
          rend_tail += strlen(render + rend_tail);// TODO
          textEvn = 1;// `code`
        }
      }
      else if (line[i] == '$')
      {// latex公式
        if (line[i + 1] == '$') {// $$ latex $$
          render[rend_tail] = render[rend_tail + 1] = '$';
          rend_tail += 2;
          i ++;
          textEvn = 4;
        } else {
          render[rend_tail] = '$';
          rend_tail ++;
          textEvn = 3;
        }
      }
      else
      {// TODO
        render[rend_tail] = line[i];
        rend_tail ++;
      }
    }
    else
    {// 非正常文本
      if (textEvn == 1)
      {
        if (line[i] == '`') {// 解除`code`
          strcat(render, "</code>");
          rend_tail += strlen(render + rend_tail);// TODO
          textEvn = 0;
        }
      }
      else if (textEvn == 2)
      {
        if (line[i] == '`' && line[i + 1] == '`') {// 解除``code``
          strcat(render, "</code>");
          rend_tail += strlen(render + rend_tail);// TODO
          i ++;
          textEvn = 0;
        }
      } 
      else if (textEvn == 3) 
      {
        if (line[i] == '$') {// 解除$latex$
          render[rend_tail] = '$';
          rend_tail += 1;
          textEvn = 0;
        }
      } 
      else if (textEvn == 4)
      {
        if (line[i] == '$' && line[i + 1] == '$') {// 解除$$ latex $$
          render[rend_tail] = render[rend_tail + 1] = '$';
          rend_tail += 2;
          i ++;
          textEvn = 0;
        }
      }
      else
      {// 不可能有其他环境
        assert(0);// TODO
      }

      if (textEvn != 0)
      {// 无视一切其他字符
        render[rend_tail] = line[i];
        rend_tail ++;
      }
    }
    render[rend_tail] = '\0';// TODO
    assert((int)strlen(render) == rend_tail);
  }

  render[rend_tail] = '\0';
}

void isSlide()
{
  assert(textEvn == 0);
  if (slide_num == 0) {// 文章开头
    sprintf(render, "<div class=\"slide\">\n<div class=\"content\">\n");
    MAGENTA("%s", render);
    fputs(render, html);
  } else {// 需要结束之前的段落
    int i = tag + 3;// 跳过---
    while (i == ' ') i ++;// 跳过空格
    length = strlen(line + i);
    if (length != 0) {// 不合语法
      isText();
      return;// TODO
    }

    tag = -1;
    clearTag();
    if (strlen(clear) != 0) {
      assert(paragraph == 0);
    }
    endPara();
    sprintf(render,
        "</div>\n</div>\n<div class=\"slide\">\n<div class=\"content\">\n");
    // 注意如果不是开头,不能够直接把html写进文件
  }
  slide_num ++;
}

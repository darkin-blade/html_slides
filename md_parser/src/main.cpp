#include <function.h>
#include <file.h>

int main()
{
  openFile();
  readFile();
  return 0;
}

void endText()
{
  endLine();// 先处理行内环境
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
    sprintf(render, "\n</code>\n");// TODO,换行
  } else {
    assert(0);// TODO,说明公式写的有问题
    assert(lineEvn == 3 || lineEvn == 4);// latex TODO
    sprintf(render, "\n");// TODO,换行
  }
  MAGENTA("%s", render);
  fputs(render, html);
  lineEvn = 0;
  if (textEvn == 2) {// TODO
    textEvn = 0;// 终止当前段落
  }
}

void doEscape(int &i, int &rend_tail)// TODO
{
  int j = 0;// 不要使用i
  if (escape == 1) {
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
  } else {// 正常环境,且不是转义字符
    render[rend_tail] = line[i];
    rend_tail ++;
  }
}

void isPara()
{
  if (textEvn != 2) {// 新的段落
    endText();// 清除之前的所有环境,TODO
    sprintf(render, "%s<p>\n", clear_text);
    textEvn = 2;// 进入段落环境
  } else {// 段落中
    assert(stackTop == 0);// 处于段落之中不应该有表
    sprintf(render, " ");// 将上一行与此行之间加上空格(用于垃圾katex渲染)
  }
  textRend();
}

void textRend()
{
  int i = my_max(tag, 0);
  escape = 0;// 转义:'\', 0: 之前一个字符不是'\', 1: 之前一个字符是'\'
  // 每新的一行,转义进行刷新

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
        doEscape(i, rend_tail);
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
      {
        if (textEvn == 1 || textEvn == 2) {// 无视一切其他字符
          render[rend_tail] = line[i];
          rend_tail ++;
        } else {// latex公式需要注意转义
          assert(textEvn == 3 || textEvn == 4);
          doEscape(i, rend_tail);
        }
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
  } else {// 需要结束之前的slide
    int i = 0;
    while (line[i] == '-') i ++;// 跳过-
    while (line[i] == ' ') i ++;// 跳过空格
    length = strlen(line + i);

    if (length != 0) {// 不合语法
      isPara();// 视作段落
      return;// TODO
    }

    endText();// 除去之前任何环境
    sprintf(render,
        "</div>\n</div>\n<div class=\"slide\">\n<div class=\"content\">\n");
    // 注意如果不是开头,不能够直接把html写进文件
  }
  slide_num ++;
}

void readFile()
{
  int i = 0;

  header();
  isSlide();// 第一张slide
  while (fgets(line, 1000, md)) {// 读取一行
    length = strlen(line);
    if (line[length - 1] == '\n') {// 过滤换行
      line[length - 1] = '\0';
      length --;
    }
    if (length == 0) {// 空行 
      endLine();// 终止段落,终止行内环境
      continue;
    }

    if (lineEvn != 0) {// 有code/latex环境未结束,TODO[默认行间公式的优先度大于标题等]
      textRend();// TODO,不能够新开段落?
    } else if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } 
    else if (line[0] == '-' && line[i + 1] == '-' && line[i + 2] == '-') 
    {// 分割线
        isSlide();
    } else {
      for (i = 0, tag = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }
      assert(i == tag);

      if (line[i] == '`' && line[i + 1] == '`'
          && line[i + 2] == '`' && line[i + 3] != '`') 
      {// 大代码块
        isCodeblock();// TODO
      } 
      /// else if (line[i] == '-' && line[i + 1] != '\0') 
      /// {// 无序表,TODO 条件
      ///   isUL();
      /// } 
      /// else if (line[i] >= '1' && line[i] <= '9' && line[i + 1] != '\0') 
      /// {// 有序表
      ///   isOL();
      /// } 
      else 
      {// 正文
        isPara();
      }
    }
    WHITE("%s", line);
    MAGENTA("%s", render);
    fputs(render, html);
  }
  footer();

  return;
}

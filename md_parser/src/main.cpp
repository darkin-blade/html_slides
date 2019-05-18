#include <function.h>
#include <file.h>
#include <end.h>

int main()
{
  openFile();
  readFile();
  return 0;
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
  if (textEvn == 0) {// 新的段落
    sprintf(render, "<p>\n");
    textEvn = 2;// 进入段落环境
  } else if (textEvn == 2) {// 段落中,TODO
    assert(stackTop == 0);// 处于段落之中不应该有表
    sprintf(render, " ");// 将上一行与此行之间加上空格(用于垃圾katex渲染)
  } else {// 由于不合语法导致的新的段落,强制新段落,TODO
    endText();// 清除所有环境
    assert(stackTop == 0);
    sprintf(render, " ");// 将上一行与此行之间加上空格(用于垃圾katex渲染)
  }
  textRend();
}

void textRend()
{
  int i = 0;// 在这个函数里面不会对i的位置进行修正,在调用之间要把字符串缩减,TODO
  escape = 0;// 转义:'\', 0: 之前一个字符不是'\', 1: 之前一个字符是'\'
  // 每新的一行,转义进行刷新
  int rend_tail = strlen(render);
  assert(render[rend_tail] == '\0');

  for (; line[i] != '\0'; i ++) {
    if (lineEvn == 0)
    {// 正常文本
      if (line[i] == '`')
      {// 行间代码
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
      else if (line[i] == '$')
      {// latex公式
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
      else
      {// TODO
        doEscape(i, rend_tail);
      }
    }
    else
    {// 非正常文本
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
          doEscape(i, rend_tail);
        }
      }
    }
    render[rend_tail] = '\0';// TODO
    assert((int)strlen(render) == rend_tail);
  }

  render[rend_tail] = '\0';
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
    else if (line[0] == '-' && line[i + 1] == '-' && line[i + 2] == '-') {// 分割线
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
      else if (line[i] == '-' && line[i + 1] != '\0') 
      {// 无序表,TODO 条件
        isUL();
      } 
      else if (line[i] >= '1' && line[i] <= '9' && line[i + 1] != '\0') 
      {// 有序表
        isOL();
      } 
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


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

    if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } else {
      for (i = 0, tag = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }
      assert(i == tag);

      if (line[i] == '-' && line[i + 1] != '\0') 
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
    tag = -1;
    clearTag();// 清空之前的表
    sprintf(render, "%s<p>\n", clear);
    paragraph = 1;
  } else {
    assert(tagStackTop == 0);// 处于段落之中不应该有表
  }
  int rend_tail = strlen(render);

  for (; line[i] != '\0'; i ++) {
    if (textEvn == 0) {// 正常文本
      if (line[i] == '`') {// 行间代码
        if (line[i + 1] == '`') {
          line[i] = line[i + 1] = 0;// 切断
          strcat(render, "<code class=\"code\">");
          rend_tail += strlen(render + rend_tail);// TODO
          i += 2;
          textEvn = 2;// ``code``
        } else {
          line[i] = 0;// 切断
          strcat(render, "<code class=\"code\">");
          rend_tail += strlen(render + rend_tail);// TODO
          i += 1;
          textEvn = 1;// `code`
        }
      } else if (line[i] == '$') {// latex公式
        if (line[i + 1] == '$') {// $$ latex $$
          line[i] = line[i + 1] = 0;// 切断
          i += 2;
          textEvn = 4;
        } else {
          line[i] = 0;// 切断
          i += 1;
          textEvn = 3;
        }
      }
    } else {// 非正常文本
      if (textEvn == 1) {
        if (line[i] == '`') {// 解除`code`
          strcat(render, "</code>");
          rend_tail += strlen(render + rend_tail);// TODO
          i += 1;
          textEvn = 0;
        }
      } else if (textEvn == 2) {
        if (line[i] == '`' && line[i + 1] == '`') {// 解除``code``
          strcat(render, "</code>");
          rend_tail += strlen(render + rend_tail);// TODO
          i += 1;
          textEvn = 0;
        }
      } else if (textEvn == 3) {
        if (line[i] == '$') {// 解除$latex$
          i += 1;
          textEvn = 0;
        }
      } else if (textEvn == 4) {
        if (line[i] == '$' && line[i + 1] == '$') {// 解除$$ latex $$
          i += 2;
          textEvn = 0;
        }
      } else {// 无视一切其他字符
        render[rend_tail] = line[i];
        rend_tail ++;
      }
    }
  }

  if (textEvn != 0) {
    assert(0);// TODO
  }
}

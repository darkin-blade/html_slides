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
      paragraph = 1;// 新的段落
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
  int head = i;// 字符串头
  tag = -1;
  clearTag();
  sprintf(render, "%s", clear);

  for (; line[i] != '\0'; i ++) {
    if (textEvn == 0) {// 正常文本
      if (line[i] == '`') {// 行间代码
        if (line[i + 1] == '`') {
          line[i] = line[i + 1] = 0;// 切断
          strcat(render, line + head);
          strcat(render, "<code class=\"code\">");
          i += 2;
          head = i;
          textEvn = 2;// ``code``
        } else {
          line[i] = 0;// 切断
          strcat(render, line + head);
          strcat(render, "<code class=\"code\">");
          strcat(render, line + head);
          i += 1;
          head = i;
          textEvn = 1;// `code`
        }
      } else {
        ;
      }
    } else {// 非正常文本
      ;
    }
  }

  sprintf(render, "%s<p>%s</p>\n", clear, line + i);// TODO
}

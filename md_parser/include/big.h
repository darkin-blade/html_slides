#ifndef MAIN_H

#include <main.h>

#endif

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

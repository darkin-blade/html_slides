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

void isTable()
{
  int i = 0, j = 0;
  int table_align[32];// 最多32列,0: center, -1: left, 1: right
  int left = 0, right = 0;// 记录----两边是否有:
  char next_line[MAX_LINE];// 下一行
  char del_space_1[MAX_LINE];// 清除所有空格之后的原文件字符串
  char del_space_2[MAX_LINE];// 针对next_line清除空格
  for (i = 0, j = 0; line[i] != '\0'; i ++) {
    if (line[i] != ' ') {// 除去空格
      del_space_1[j] = line[i];
      j ++;
    }
  }
  del_space_1[j] = '\0';

  if (fgets(next_line, MAX_READ, md)) {// 不是最后一行
    for (i = 0, j = 0; next_line[i] != '\0'; i ++) {
      if (next_line[i] != ' ') {// 除去空格
        del_space_2[j] = line[i];
        j ++
      }
    }
    del_space_2[j] = '\0';
  } else {
    isPara();// 视作段落,TODO
  }
}

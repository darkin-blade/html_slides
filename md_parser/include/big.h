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
  int mid = 0;// 记录::中间是否有----
  int column = 0;// 记录当前列数
  char next_line[MAX_LINE];// 下一行
  char del_space_1[MAX_LINE];// 清除所有空格之后的原文件字符串
  char del_space_2[MAX_LINE];// 针对next_line清除空格
  assert(0);// TODO
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

    for (i = 0, j = -1;// j记录当前列数,TODO
        del_space_2[i] != '\0'; i ++) {// 记录对齐样式
      if (del_space_2[i] == '|') {// 新的一列
        if (j != -1) {// 需要进行记录
          if (left == right) {// mid也有可能为0
            table_align[j] = 0;
          } else if (left == 1 && right == 0) {
            table_align[j] = -1;// 左对齐
          } else if (left == 0 && right == 1) {
            table_align[j] = 1;// 右对齐
          }
        }
        j ++;
      } else if (del_space_2[i] == ':') {// TODO
        if (left == 0) {// `|:`
          left = 1;
        } else if (mid == 1) {// `|:-:`
          right = 1;
        }
      } else if (del_space_2[i] == '-') {// TODO
        if (mid == 0) {// `|:-`
          mid = 1;
        }
      } else {// 非法字符
        length = strlen(line);
        sprintf(line + length, "%s", next_line);// TODO,合并两行
        isPara();
        return;
      }
    }
  } else {
    isPara();// 视作段落,TODO
    return;
  }
  // 到了这里说明是符合语法的
  char table_con[MAX_LINE / 2];// 表项
  for (i = tag, j = -1;// 跳过空格,初始化列数,TODO
      line[i] != '\0'; i ++) {// thead
    if (line[i] == '|') {// 新的column
      if (j != -1) {// 不是开头
        ;
      }
    }
  }
}

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

  while (fgets(line, MAX_READ, md)) {// 读取一行
    line_num ++;// debug
    for (i = 0; line[i] == ' '; i ++) {// 跳过空格
      ;
    }
    if (line[i] == '`' && line[i + 1] == '`' 
        && line[i + 2] == '`' && line[i + 3] != '`') {// 跳出代码块
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
  int i = 0, j = 0;// 记录列数
  int table_align[32];// 最多32列,0: center, -1: left, 1: right
  int left = 0, right = 0;// 记录----两边是否有:
  int mid = 0;// 记录::中间是否有----
  char next_line[MAX_LINE];// 下一行
  char del_space[MAX_LINE];// 针对next_line清除空格

  if (fgets(next_line, MAX_READ, md)) {// 不是最后一行
    for (i = 0, j = 0; next_line[i] != '\0'; i ++) {
      if (next_line[i] != ' ' && next_line[i] != '\n') {// 除去空格和换行符
        del_space[j] = next_line[i];
        j ++;
      }
    }
    del_space[j] = '\0';

    for (i = 0, j = -1;// j记录当前列数,TODO
        del_space[i] != '\0'; i ++) {// 记录对齐样式
      if (del_space[i] == '|') {// 新的一列
        if (j != -1) {// 需要进行记录
          if (left == right) {// mid也有可能为0
            table_align[j] = 0;
          } else if (left == 1 && right == 0) {
            table_align[j] = -1;// 左对齐
          } else if (left == 0 && right == 1) {
            table_align[j] = 1;// 右对齐
          }
        }
        left = mid = right = 0;
        j ++;
      } else if (del_space[i] == ':') {// TODO
        if (left == 0 && mid == 0) {// `|:`
          left = 1;
        } else if (mid == 1) {// `|:-:`
          right = 1;
        }
      } else if (del_space[i] == '-') {// TODO
        if (mid == 0) {// `|:-` / `|-`
          mid = 1;
        } else if (left == 0 && right == 1) {// `|-:-`
          assert(mid == 1);
          left = 1;// 本来是未定义行为
        }
      } else {// 非法字符
        line_len = strlen(line);
        YELLOW("%s", next_line);
        RED("%s", del_space);
        RED("illegal[%d:%c]", i, del_space[i]);
        sprintf(line + line_len, "%s", next_line);// TODO,合并两行
        isPara();
        return;
      }
    }
  } else {
    isPara();// 视作段落,TODO
    return;
  }
  // 到了这里说明是符合语法的
  sprintf(render, "<table>\n<thead>\n<tr>\n");
  int table_end = 0;// 表项字符串终止位置
  for (i = tag,// 跳过空格
       table_end = 0,
       j = -1;// 初始化列数,TODO
       line[i] != '\0'; i ++) {// thead
    if (line[i] == '|') {// 新的column
      if (j != -1) {// 不是开头
        rend_tail = strlen(render);
        if (table_align[j] == 0) {// center
          sprintf(render + rend_tail, "<th style=\"text-align:center\">");// TODO
        } else if (table_align[j] == -1) {// left
          sprintf(render + rend_tail, "<th style=\"text-align:left\">");// TODO
        } else if (table_align[j] == 1) {// right
          sprintf(render + rend_tail, "<th style=\"text-align:right\">");// TODO
        } else {
          assert(0);
        }

        rend_tail += strlen(render + rend_tail);
        line[i] = '\0';// 裁剪line
        textRend();// TODO
        sprintf(render + rend_tail, "</th>\n");
        rend_tail += strlen(render + rend_tail);
      }
      sprintf(line, "%s", line + i + 1);// 除去空格和'|',TODO
      i = -1;// TODO
      table_end = 0;
      j ++;
    } else {
      table_end ++;
    }
  }
  sprintf(render + rend_tail, "</tr>\n</thead>\n<tbody>\n");// 结束表头
  WHITE("[%d] %s", line_num, line);
  MAGENTA("%s", render);
  fputs(render, html);

  while (fgets(line, MAX_READ, md)) 
  {// 表格体
    sprintf(render, "<tr>\n");// 新的一行
    line_num ++;// debug
    for (i = 0, tag = 0; line[i] == ' '; i ++) {// 跳过空格
      tag ++;
    }
    if (line[tag] != '|') {// 结束表格
      redo = 1;
      return;
    } else {// 继续表格
      for (i = tag,// 跳过空格
           table_end = 0,
           j = -1;// 记录当前列数,TODO
           line[i] != '\0'; i ++) {// tbody
        if (line[i] == '|') {// 新的column
          if (j != -1) {// 不是开头
            rend_tail = strlen(render);
            if (table_align[j] == 0) {// center
              sprintf(render + rend_tail, "<td style=\"text-align:center\">");// TODO
            } else if (table_align[j] == -1) {// left
              sprintf(render + rend_tail, "<td style=\"text-align:left\">");// TODO
            } else if (table_align[j] == 1) {// right
              sprintf(render + rend_tail, "<td style=\"text-align:right\">");// TODO
            } else {
              assert(0);
            }

            rend_tail += strlen(render + rend_tail);
            line[i] = '\0';// 裁剪line
            textRend();// TODO
            sprintf(render + rend_tail, "</td>\n");
            rend_tail += strlen(render + rend_tail);
          }
          sprintf(line, "%s", line + i + 1);// 除去空格和'|',TODO
          i = -1;// TODO
          table_end = 0;
          j ++;
        } else {
          table_end ++;
        }
      }
    }
    sprintf(render + rend_tail, "</tr>\n");// 结束一行
    WHITE("[%d] %s", line_num, line);
    MAGENTA("%s", render);
    fputs(render, html);
  }

  sprintf(render, "</tbody>\n</table>");// TODO
}

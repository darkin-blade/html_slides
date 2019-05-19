#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLACK(format, ...) \
  printf("\033[1;30m" format "\33[0m\n", ## __VA_ARGS__)
#define RED(format, ...) \
  printf("\033[1;31m" format "\33[0m\n", ## __VA_ARGS__)
#define GREEN(format, ...) \
  printf("\033[1;32m" format "\33[0m\n", ## __VA_ARGS__)
#define YELLOW(format, ...) \
  printf("\033[1;33m" format "\33[0m\n", ## __VA_ARGS__)
#define BLUE(format, ...) \
  printf("\033[1;34m" format "\33[0m\n", ## __VA_ARGS__)
#define MAGENTA(format, ...) \
  printf("\033[1;35m" format "\33[0m\n", ## __VA_ARGS__)
#define CYAN(format, ...) \
  printf("\033[1;36m" format "\33[0m\n", ## __VA_ARGS__)
#define WHITE(format, ...) \
  printf("\033[1;37m" format "\33[0m\n", ## __VA_ARGS__)
#define black(format, ...) \
  printf("\033[1;30m" format "\33[0m", ## __VA_ARGS__)
#define red(format, ...) \
  printf("\033[1;31m" format "\33[0m", ## __VA_ARGS__)
#define green(format, ...) \
  printf("\033[1;32m" format "\33[0m", ## __VA_ARGS__)
#define yellow(format, ...) \
  printf("\033[1;33m" format "\33[0m", ## __VA_ARGS__)
#define blue(format, ...) \
  printf("\033[1;34m" format "\33[0m", ## __VA_ARGS__)
#define magenta(format, ...) \
  printf("\033[1;35m" format "\33[0m", ## __VA_ARGS__)
#define cyan(format, ...) \
  printf("\033[1;36m" format "\33[0m", ## __VA_ARGS__)
#define white(format, ...) \
  printf("\033[1;37m" format "\33[0m", ## __VA_ARGS__)

#define MAIN_H
#define MAX_LINE 1024
// 原文件一行的长度
#define MAX_REND 2048
// 渲染之后一行的长度
#define MAX_TAG 16
// 无序/有序表缩进级数
#define MAX_NAME 32
// 文件名长度
#define MAX_STR 64
// 一些特殊字符的长度
// 如: clear_tag, escp_char
#define MAX_PART 16
// html模板文件个数及文件名长度(一定要小于文件名最大长度)

int my_max(int a, int b)
{
  if (a > b) return a;
  return b;
}

int my_min(int a, int b)
{
  if (a < b) return a;
  return b;
}

int isLetter(char c)// 是否是字母
{
  return ((c >= 'a')&&(c <= 'z'))||((c >= 'A')&&(c <= 'Z'));
}

void openFile();
void readFile();
void header();// 添加<html>
void footer();// 添加</html>

void isCodeblock();// 代码段
void isSlide();// 分割线
void isTable();// 表格,TODO
void isTitle();// 标题
void isPara();// 段落
void isUL();// 无序表
void isOL();// 有序表
void isImg();// 图片,TODO
void isLink();// 链接,TODO

void textRend();// 文字渲染(注意和段落区别)
void charRend(int &i, int &rend_tail);// 正常文本的转义处理等
void endText();// 结束之前所有的环境
void endLine();// 除去行内环境
void endPara();// 除去强调环境
void endTag();// 向前回溯无序表/有序表

char mdFileName[MAX_NAME];
char htmlFileName[MAX_NAME];
FILE *md;// markdown源文件
FILE *html;// html目标文件

char line[MAX_LINE];// 每一行
int length = 0;// 每一行的长度
char render[MAX_REND];// 解析后的字符串

int tag = -1;// 缩进级数(空格数)
int tagStack[MAX_TAG]; // 最多?种不同缩进
int evnStack[MAX_TAG]; // 语言环境栈
// 0: default, 1: title, 2: paragraph, 3: blockquote, 4: ul, 5: ol
int stackTop = 0;// 语言环境栈顶
int textEvn = 0;// 文章语言环境 
// 0: plain, 1: title, 2: paragraph, 3: blockquote, 4: ul, 5: ol
int paraEvn = 0;// 每一段的语言环境(强调环境)
// 0: normal, 1: em, 2: strong, 3: del
int lineEvn = 0;// 行内语言环境
// 0: plain, 1: `code`, 2: ``code``, 3: $latex$, 4: $$latex$$
char clear_tag[MAX_STR];// 清除语言环境

int escape = 0;// 转义'\'
int empty_line = 0;// 空行数
// 0: 无空行,继承除标题外的其他任何环境
// 1: 新开段落/引用,取消行内环境(这属于md语法错误)
// >=2: 取消有序表/无序表
char escp_char[MAX_STR] = "*_\\";
int paragraph = 0;// 0: 没有新的段落, 1: 有段落没有结束
int slide_num = 0;// 当前slide序号,初始0
int line_num = 0;// debug用,md文件读取的行数








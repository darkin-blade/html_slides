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

// 文件操作
void openFile();
void readFile();
void header();// 添加<html>
void footer();// 添加</html>

void isCodeblock();// 代码段
void isSlide();// 分割线
void isTable();// 表格,TODO
void isTitle();// 标题
void isPara();// 段落,TODO
void isUL();// 无序表
void isOL();// 有序表

void clearTag();// 向前回溯
void textRend();// 文字渲染(注意和段落区别)
void endEvn();// 结束之前所有的环境,TODO
void doEscape(int &i, int &rend_tail);// 正常文本的转义处理

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

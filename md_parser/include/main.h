#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define black_log
#define red_log
#define green_log
#define yellow_log
#define blue_log
#define magenta_log
#define cyan_log
#define white_log

#ifdef black_log
#define BLACK(format, ...) \
  printf("\033[1;30m" format "\33[0m\n", ## __VA_ARGS__)
#define black(format, ...) \
  printf("\033[1;30m" format "\33[0m", ## __VA_ARGS__)
#else
#define BLACK(format, ...) \
  assert(1)
#define black(format, ...) \
  assert(1)
#endif

#ifdef red_log
#define RED(format, ...) \
  printf("\033[1;31m[%s, %d]" format "\33[0m\n", __func__, __LINE__, ## __VA_ARGS__)
#define red(format, ...) \
  printf("\033[1;31m" format "\33[0m", ## __VA_ARGS__)
#else
#define RED(format, ...) \
  assert(1)
#define red(format, ...) \
  assert(1)
#endif

#ifdef green_log
#define GREEN(format, ...) \
  printf("\033[1;32m" format "\33[0m\n", ## __VA_ARGS__)
#define green(format, ...) \
  printf("\033[1;32m" format "\33[0m", ## __VA_ARGS__)
#else
#define GREEN(format, ...) \
  assert(1)
#define green(format, ...) \
  assert(1)
#endif

#ifdef yellow_log
#define YELLOW(format, ...) \
  printf("\033[1;33m" format "\33[0m\n", ## __VA_ARGS__)
#define yellow(format, ...) \
  printf("\033[1;33m" format "\33[0m", ## __VA_ARGS__)
#else
#define YELLOW(format, ...) \
  assert(1)
#define yellow(format, ...) \
  assert(1)
#endif

#ifdef blue_log
#define BLUE(format, ...) \
  printf("\033[1;34m" format "\33[0m\n", ## __VA_ARGS__)
#define blue(format, ...) \
  printf("\033[1;34m" format "\33[0m", ## __VA_ARGS__)
#else
#define BLUE(format, ...) \
  assert(1)
#define blue(format, ...) \
  assert(1)
#endif

#ifdef magenta_log
#define MAGENTA(format, ...) \
  printf("\033[1;35m[%s]" format "\33[0m\n", __func__, ## __VA_ARGS__)
#define magenta(format, ...) \
  printf("\033[1;35m" format "\33[0m", ## __VA_ARGS__)
#else
#define MAGENTA(format, ...) \
  assert(1)
#define magenta(format, ...) \
  assert(1)
#endif

#ifdef cyan_log
#define CYAN(format, ...) \
  printf("\033[1;36m" format "\33[0m\n", ## __VA_ARGS__)
#define cyan(format, ...) \
  printf("\033[1;36m" format "\33[0m", ## __VA_ARGS__)
#else
#define CYAN(format, ...) \
  assert(1)
#define cyan(format, ...) \
  assert(1)
#endif

#ifdef white_log
#define WHITE(format, ...) \
  printf("\033[1;37m" format "\33[0m\n", ## __VA_ARGS__)
#define white(format, ...) \
  printf("\033[1;37m" format "\33[0m", ## __VA_ARGS__)
#else
#define WHITE(format, ...) \
  assert(1)
#define white(format, ...) \
  assert(1)
#endif

#define MAIN_H
#define MAX_READ 1000
// 读取原文件一行的最大长度
#define MAX_LINE 1600
// 原文件带渲染字符串最大长度
#define MAX_REND 2400
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

void isTitle();// 标题
void isCodeblock();// 代码段
void isTable();// 表格
void isSlide();// 分割线
void isPara();// 段落
void isUL();// 无序表
void isOL();// 有序表
void isQuote();// 引用

void textRend();// 所有文字渲染(注意和段落区别),可以转到图片或链接
void lineRend(int &i);// 普通文字渲染
void linkRend(int &i);// 链接
void imgRend(int &i);// 图片链接
void charRend(int &i);// 正常文本的转义处理等
void endText();// 结束之前所有的环境
void endLine();// 除去行内环境
void endPara();// 除去强调环境
void endTag();// 向前回溯无序表/有序表

char mdFileName[MAX_NAME];
char htmlFileName[MAX_NAME];
FILE *md;// markdown源文件
FILE *html;// html目标文件

char line[MAX_LINE];// 每一行
int line_len = 0;// 每一行的长度
char render[MAX_REND];// 解析后的字符串
int rend_tail = 0;// 当前解析位置
char link_con[MAX_REND];// 链接内容
int link_con_end = 0;// 从当前位置到link_con_end都是链接内容
char link_rel[MAX_REND];// 链接地址
int link_rel_end = 0;// 从当前位置到link_rel_end都是链接地址

int tag = -1;// 缩进级数(空格数)
int tagStack[MAX_TAG]; // 最多?种不同缩进
// 注: blockquote的缩进级数('>'的个数)与tag不可比较
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
// 1: 新开段落/引用,取消行内环境和强调环境(这属于md语法错误)
// >=2: 取消有序表/无序表
char escp_char[MAX_STR] = "*_\\";
int paragraph = 0;// 0: 没有新的段落, 1: 有段落没有结束
int slide_num = 0;// 当前slide序号,初始0
int line_num = 0;// debug用,md文件读取的行数
int redo = 0;// 解析失败(表格)但需要返回进行其他环境的判断







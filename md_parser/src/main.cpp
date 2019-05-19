#include <big.h>
#include <small.h>
#include <part.h>
#include <end.h>
#include <rend.h>

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
  isSlide();// 第一张slide
  while (fgets(line, MAX_READ, md)) {// 读取一行
    line_num ++;// debug用,读取的行数
    line_len = strlen(line);
    if (line[line_len - 1] == '\n') {// 过滤换行
      line[line_len - 1] = '\0';
      line_len --;
    }
    if (line_len == 0) {// 空行 
      assert(empty_line >= 0);
      empty_line ++;// 记录空行
      if (empty_line >= 2) {// 这个是正常语法
        endText();// 取消一切环境
      } else {// 正常情况下只处理blockquote
        endPara();// 终止段落,终止行内环境
      }
      BLUE("empty_line");
      continue;
    }

    if (lineEvn != 0 || paraEvn != 0) {// 有code/latex/ 强调 环境未结束,TODO
      // [默认行间公式/ 强调 的优先度大于标题等]
      isPara();// TODO,不能够新开段落?
    } else if (line[0] == '#') {// 标题,标题前不能有空格,#后要有空格
      isTitle();
    } 
    else if (line[0] == '-' && line[1] == '-' && line[2] == '-') {
      isSlide();// 分割线
    } else {
      for (i = 0, tag = 0; line[i] == ' '; i ++) {// 清除空格
        tag ++;// 缩进级数增加
      }
      assert(i == tag);

      if (line[i] == '`' && line[i + 1] == '`'
          && line[i + 2] == '`' && line[i + 3] != '`') 
      {// 大代码块
        isCodeblock();
      } 
      else if (line[i] == '>')
      {// 引用,注意: 该环境会与tag表无条件嵌套
        isQuote();
      }
      else if (line[i] == '-' && line[i + 1] != '\0') 
      {// 无序表,TODO 条件
        isUL();
      } 
      else if (line[i] >= '0' && line[i] <= '9' && line[i + 1] != '\0') 
      {// 有序表
        isOL();
      } 
      else 
      {// 正文
        CYAN("%d, paragraph", __LINE__);
        isPara();
      }
    }
    WHITE("[%d] %s", line_num, line);
    MAGENTA("%s", render);
    fputs(render, html);
    empty_line = 0;// 刷新空行,TODO
  }
  footer();

  return;
}

void openFile()
{
  YELLOW("Your current path: [%s]", getenv("PWD"));// 当前目录

  GREEN("Input markdown file name");
  scanf("%s", mdFileName);
  green("md filename is ");
  CYAN("%s", mdFileName);
  md = fopen(mdFileName, "r");
  if (md == NULL) {
    RED("[%s] doesn't exist", mdFileName);
    exit(1);
  } else {
    GREEN("open [%s] successfully", mdFileName);
  }

  GREEN("Input html file name");
  // scanf("%s", htmlFileName);// TODO
  sprintf(htmlFileName, "test.html");
  green("html filename is ");
  CYAN("%s", htmlFileName);
  html = fopen(htmlFileName, "w+");
  if (html == NULL) {
    RED("[%s] open error", htmlFileName);
    exit(1);
  } else {
    GREEN("open [%s] successfully", htmlFileName);
  }
}

# markdown编译器`md_parser`

## 使用

### 编译markdown

在`html_slides/md_parser/`下`make run`,在`Input markdown file name (default: test.md)`后输入markdown文件名(带后缀),在`Input html file name (default: test.html)`后输入目标html文件名(生成在当前目录).

如果使用`<ctrl-d>`结束输入,编译器会将文件名置为默认值

### 使用模板

如果不想使用默认的模板,在`html_slides/md_parser/include/part.h`中修改`header()`和`footer()`中的`partial`数组,或者直接修改`html_slides/md_parser/partial/`下的`*.html`文件模板

默认加载`style.html`,`js.html`,`katex.html`/`mathjax.html`

 - 修改样式可以修改`style.html`
 - 增加脚本可以修改`js.html`
 - 数学公式渲染可以选择`katex.html`和`mathjax.html`中的一个

其余部分是必要部分

## 已实现的功能

1. 标题:`#`
2. 大代码段:`` ` ` `  ``
3. 表格:`|:--:|`
4. 分割线:`---`(TODO: 现在分页的页面大小可能会有1px的误差)
5. 普通段落
    - 只要不造成解析的歧义,能够无限制地向markdown中内嵌`html`,`javascript`,`php`代码
6. 无序表:`-`
7. 有序表:`1.`
8. 引用块:`>`

段落内部支持的环境

1. 粗体:`**`,`__`
2. 斜体:`*`,`_`
3. 删除:`~~`
4. 代码:`` ` ``, `` ` ` ``
5. latex行内公式:`$`
6. latex行间公式:`$$`
7. 链接:`[]()`
8. 图片:`![]()`

## TODO

# 幻灯片模式

> TODO

# 其他

## css说明

使用了自己[博客](niabie.github.io)的css

| 位置 | 功能 |
| :--: | :--: |
| code\_molokai.styl | sublime molokai风格的代码块 |
| paper.css | 正文 |

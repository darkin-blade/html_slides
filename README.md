# markdown编译器`md_parser`

## 默认配置

### js

在`md_parser/partial/js.html`,默认加载:`jquery.min.js`,`highlight.min.js`,`deep_highlight.js`

### css

在`markdown/partial/css.html`,默认加载:`paper.css`,`molokai.css`

## 使用

### 编译模式的选择

- A4大小:在`md_parser/partial/css.html`中引用`a4.css`
- 仿WebSlides幻灯片:在`md_parser/partial/css.html`中引用`full_slide.css`,在`md_parser/partial/js.html`中引用`full_slide.js`
- 仿remark.js幻灯片(`TODO`):在`md_parser/partial/css.html`中引用`scale_slide.css`,在`md_parser/partial/js.html`中引用`scale_slide.js`

### 编译markdown

在`md_parser/`下`make run`,在`Input markdown file name (default: ***.md)`后输入markdown文件名(带后缀),在`Input html file name (default: ***.html)`后输入目标html文件名(生成在当前目录).

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

- 打印会有`1px`左右的偏差
- 无序链表`+`
- 引用`>`嵌套`-`时用的语法
- 一些苛刻的换行没有实现(链接`[]()`内部换行等)

# 幻灯片模式

## 仿WebSlides

### 使用

#### 以slide为单位的动画

在`md`文件中想要增加动画的部分,前后加上`<div class="ani">`,`</div>`,每个页面可以有多个这样的`div`

#### 打印

在浏览器窗口发生调整时会自动调节打印机的打印尺寸,打印的部分为`<div class="content">`的部分

### TODO

- 打印模式会有`1px`左右的误差,导致打印成pdf的时候有时会多出一张
- 嵌套`<div class="ani">`有时会影响`md`文件的解析

## 仿remark.js

> TODO

# 文件说明

## css

使用了自己[博客](https://niabie.github.io)的css

- `css/a4.css`:a4页面大小
- `css/molokai.css`:`sublime`的`molokai`风格代码块
- `css/paper.css`:基本样式

## javascript

- `js/highlight.min.js`:代码块解析
- `js/deep_highlight.js`:优化代码块解析(配合`css/molokai.css`使用)

## font

字体文件

# markdown解析器

## 文件说明

### 核心文件

- `include/main.h`:Log及全局变量声明
- `src/main.cpp`:默认文件名,解析器主循环
- `include/part.h`:`partial/`下模板的加载
- `include/rend.h`:主体段落的渲染
- `include/small.h`:标题,列表,分割,段落,引用
- `include/big.h`:代码块,表格
- `include/end.h`:用于结束环境

### html模板

- `partial/html.html`:声明`doctype html`,开启`<head>`
- `partial/style.html`:声明`utf-8`,引用`css`文件
- `partial/head.html`:结束`<head>`,开启`<body>`
- `partial/js.html`:引用`jquery`和代码高亮
- `partial/mathjax/katex`:引用`latex`公式渲染`js`
- `partial/body`:结束`<body>`

## bug

- 有序列表,无序列表交替
- 列表中交替quote

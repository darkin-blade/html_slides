/**
 * 手写html代码块高亮
 */

var highlight_1 = [
  "for", "while", "return", "switch", "case", "break", "do", "else", "if",
  "endif", "endfor", "endwhile",
  "const"
];

var highlight_2 = [
  "width", "height", "type", "data", "src", "href", "style", "rel",
  "onload", "onclick", "onscroll", "onmousemove", "onresize",
  "id", "class"
];

function dfs_c(my_node, found) {
  if ((my_node.className != null)&&(my_node.className.length >= 0))
  {
    if (my_node.className.match("highlight") != null)
    {
      found = my_node.className.replace(/highlight /, "");
      my_node.className += " code_figure";
    } // else
    var temp_class = my_node.className;
    if ((found != "")&&(// 处于代码块内部
        (temp_class == "hljs-string")// 字符串
      ||(temp_class == "hljs-comment")// 注释
      ||(temp_class == "hljs-attribute")// 属性?
      ||(temp_class == "hljs-built_in")// 内置函数
      ||(temp_class == "hljs-name")// html标签
      ||(temp_class == "hljs-number")// 数字
      ||(temp_class == "hljs-title")// 函数声明
      ||(temp_class == "hljs-params")// 函数参数
      ||(temp_class == "hljs-regexp")// 正则表达式
      ||(temp_class == "hljs-meta-keyword")// #include
      ||(temp_class == "hljs-meta-string")// <>
      ||(temp_class == "hljs-selector-tag")// css tag
      ||(temp_class == "hljs-selector-class")// css class
      ||(temp_class == "hljs-selector-id")// css id
      ||(temp_class == "hljs-selector-pseduo")// 浏览器工具
      ||(temp_class == "hljs-symbol")// vimscript 按键
      ||(temp_class == "hljs-section")// makefile 命令
      ||(temp_class == "hljs-variable")// makefile 变量
      ||(temp_class == "hljs-strong")// markdown 强调
    )) {
      my_node.className += " code_" + temp_class.replace(/hljs-/, "");
    } else if ((found != "")&&(// 处于代码块内部
      (temp_class == "hljs-keyword")// 多种关键字,需要特判,包括:for const int void enum等
      ||(temp_class == "hljs-attr")// 多种属性(html)?
    )) {
      detail_hilight(my_node, my_node.className.replace(/hljs-/, ""), found)
    }
  }

  var my_child = my_node.childNodes.length;
  for (var i = 0; i < my_child; i ++)
  {
    // 防止未定义
    if (my_node.childNodes[i] != null) {
      dfs_c(my_node.childNodes[i], found);
    }
  }
}

function show_language() {
  document.querySelectorAll(".highlight").forEach(function(my_node) {
    var temp_class = my_node.className.replace(/highlight/, "");
    temp_class = temp_class.replace(/code_figure/, "");
    my_node.setAttribute("language_type", temp_class);
  });
}

function my_highlight_start() {
  show_language();
  setTimeout("dfs_c(document.body, '')", 100);
}

function detail_hilight(my_node, type, language) {// 细化
  if (type == "keyword") {
    for (var i = 0; i < highlight_1.length; i ++)
    {
      if (my_node.innerText == highlight_1[i])
      {
        my_node.className += " code_keyword1";
        return;
      }
    }
    my_node.className += " code_keyword";
    return;
  } else if (type == "attr") {
    for (var i = 0; i < highlight_2.length; i ++)
    {
      if (my_node.innerText == highlight_2[i])
      {
        my_node.className += " code_attr1";
        return;
      }
    }
    my_node.className += " code_attr";
  }
}
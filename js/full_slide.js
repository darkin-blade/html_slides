var cur_slide = 0;
// 当前slide编号
var c_height = 0;
// 当前窗口高度
var c_width = 0;
// 当前窗口宽度

(function(){
  document.body.setAttribute("onload", "my_load()");
  // setTimeout("my_load()", 0);
}());

function my_load() {
  set_print();// 需要把print_style在最先设置好
  
  document.body.setAttribute("onresize", "my_resize()");
  document.body.setAttribute("onmousewheel", "my_scroll()");
  document.body.setAttribute("onkeydown", "my_keydown()");
  document.body.setAttribute("onmousedown", "my_mousedown()");
  // 添加监听
  
  setTimeout("my_resize()", 0);

  $(".ani").toggleClass("ani_hide");// 隐藏所有动画
  content_fade();
  $(".curtain").toggleClass("curtain_hide");
  setTimeout("after_load()", 1000);
}

function after_load() {
  $(".curtain").css("z-index", "-10");
}

function my_mousedown() {
  ani_fade();
  // 动画依次出现
}

function my_resize() {
  cal_size();
  resize_slide();
  setTimeout("resize_slide()", 0);

  change_print();// 修改打印样式
  
  scroll_slide();// TODO
}

function my_scroll() {
  my_keydown();
}

function my_keydown() {
  cal_size();
  scroll_slide();
  content_fade();// TODO
}

function ani_fade() {
  // 动画依次出现
  var all_ani = document.querySelectorAll(".ani");
  var ani_num = all_ani.length;
  for (i = 0; i < ani_num; i ++) {
    var t_offset = $(all_ani[i]).offset().top - 0;
    console.log(i, t_offset, c_height);
    if (t_offset >= 0 && t_offset <= c_height) {
      if (all_ani[i].className == "ani ani_hide") {
        all_ani[i].className = "ani ani_show";
        return;// 只显示一个
      }
    }
  }
}

function content_fade() {
  var all_content = document.querySelectorAll(".content");
  var slide_num = all_content.length;
  
  for (i = 0; i < slide_num; i ++) {
    // 淡入,切换时调用
    if (i == cur_slide) {
      all_content[i].className = "content";
    } else {
      all_content[i].className = "content content_hide";
    }
  }

  setTimeout("ani_fresh()", 500);// TODO,时间
}

function ani_fresh() {
  // 刷新所有动画
  var all_ani = document.querySelectorAll(".ani");
  var ani_num = all_ani.length;
  for (i = 0; i < ani_num; i++) {
    var t_offset = $(all_ani[i]).offset().top - 0;
    if (t_offset <= 0) {
      // 在屏幕之上
      all_ani[i].className = "ani ani_show";
    } else if (t_offset >= c_height) {
      // 在屏幕之下
      all_ani[i].className = "ani ani_hide";
    }
  }
}

function change_print() {
  var page_style = document.getElementById("print_style");
  page_style.innerHTML = "@page{size: " + c_width + "px " + c_height + "px;}";
}

function set_print() {
  // @page的打印设置选项
  var page_style = document.createElement("style");
  page_style.id = "print_style";// TODO
  document.body.appendChild(page_style);// 将css附在body最后面
}

function scroll_slide() {
  if (window.event || event) {
    var e = window.event || event;
    
    if (e.wheelDelta) {
      // 鼠标滚轮  
      var w_delta = e.wheelDelta;
      if (w_delta > 0) {
        // 向上
        cur_slide --;
      } else {
        // 向下
        cur_slide ++;
      }
    } else if (e.keyCode) {
      // 按键
      if (e.keyCode == 39 || e.keyCode == 40 
        || e.keyCode == 13 || e.keyCode == 32) {// 回车或空格
        cur_slide ++;
      } else if (e.keyCode == 37 || e.keyCode == 38) {
        cur_slide --;
      }
    }
  }
  
  var all_slides = document.querySelectorAll(".slide");
  var slide_num = all_slides.length;
  if (cur_slide == slide_num) cur_slide = slide_num - 1;// TODO
  if (cur_slide == -1) {
    console.log("reset");
    $(".ani .ani_show").toggleClass("ani_show ani_hide");
    cur_slide = 0;
  }
  // 只进行特判,否则妨碍初始化
  
  for (i = 0; i < slide_num; i ++) {
    all_slides[i].style.top = ((i - cur_slide) * c_height) + "px";
  }
}

function resize_slide() {
  // 将幻灯片调成和浏览器等高
  c_width -= 0;
  c_height -= 0;
  
  $(".slide").css("height", (c_height - 2) + "px");
}

function cal_size() {
  // 获取窗口宽度
  if (window.innerWidth)
    c_width = window.innerWidth;
  else if ((document.body) && (document.body.clientWidth))
    c_width = document.body.clientWidth;
  // 获取窗口高度
  if (window.innerHeight)
    c_height = window.innerHeight;
  else if ((document.body) && (document.body.clientHeight))
    c_height = document.body.clientHeight;
  // 通过深入 Document 内部对 body 进行检测，获取窗口大小
  if (document.documentElement && document.documentElement.clientHeight && document.documentElement.clientWidth) {
    c_width = document.documentElement.clientWidth;
    c_height = document.documentElement.clientHeight;
  }

}
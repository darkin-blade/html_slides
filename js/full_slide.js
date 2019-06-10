(function(){
  document.body.setAttribute("onresize", "my_resize()");
  document.body.setAttribute("onmousewheel", "my_scroll()");
  setTimeout("my_resize()", 0);
  setTimeout("my_scroll()", 0);
}());

var cur_slide = 0;
var c_height;
var c_width;

function my_resize() {
  cal_size();
  resize_slide();
  setTimeout("resize_slide()", 0);
}

function my_scroll() {
  cal_size();
  scroll_slide();
}

function scroll_slide() {
  var all_slides = document.querySelectorAll(".slide");
  var slide_num = all_slides.length;
  if (window.event && window.event.wheelDelta) {
    var w_delta = window.event.wheelDelta;
    if (w_delta > 0) {
      // 向上
      if (cur_slide > 0)
        cur_slide --;
    } else {
      // 向下
      if (cur_slide < slide_num - 1)
        cur_slide ++;
    }
  }
  for (i = 0; i < slide_num; i ++) {
    all_slides[i].style.top = ((i - cur_slide) * c_height) + "px";
  }
  // console.log(slide_num, window.event.wheelDelta);
}

function resize_slide() {
  c_width -= 0;
  c_height -= 0;
  
  $(".slide").css("height", (c_height - 2) + "px");
  console.log(c_height, c_width);
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
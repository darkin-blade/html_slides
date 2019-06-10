(function(){
  document.body.setAttribute("onresize", "my_resize()");
  setTimeout("my_resize()", 0);
}());

function my_resize() {
  change_slide();
  setTimeout("change_slide()", 0);
}

function change_slide() {
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
  c_width -= 0;
  c_height -= 0;
  
  var r_height = c_height / 700;
  var r_width = c_width / 1000;
  var off_top = 0;
  var off_left = 0;
  var rate = 1;
  if (r_height < r_width) {
    // 100%宽度
    rate = r_height;
  } else {
    // 100%高度
    rate = r_width;
  }
  off_left = (c_width - rate * 1000) / 2;
  off_top = (c_height - rate * 700) / 2;
  console.log(c_height, c_width, rate, off_top, off_left);

  $(".slide").css("top", off_top + "px");
  $(".slide").css("left", off_left + "px");
  $(".slide").css("transform", "scale(" + rate + ")");
}
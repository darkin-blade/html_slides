(function(){
  document.body.setAttribute("onresize", "my_resize()");
  my_resize();
}());

function my_resize() {
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
  c_width -= 20;
  c_height -= 20;
  // var c_height = document.body.clientHeight;
  // var c_width = document.body.clientWidth;
  // var c_height = window.innerHeight - 20;
  // var c_width = window.innerWidth - 20;
  
  var r_height = c_height / 700;
  var r_width = c_width / 1000;
  var rate = 1;
  if (r_height < r_width) {
    // 100%宽度
    rate = r_height;
    $(".slide").css("left", "0px");
  } else {
    // 100%高度
    rate = r_width;
    // $(".slide").css("top", "0px");
  }
  console.log(c_height, c_width, rate);
  $(".slide").css("transform", "scale(" + rate + ")");
}
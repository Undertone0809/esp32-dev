# Introduction
下文的介绍很详细的，就不重复造轮子了。

- [Arduino ESP32定时器功能使用 -csdn](https://blog.csdn.net/weixin_42880082/article/details/120882798)


# 注意事项

中断程序不能执行太久，否则超出设定的阈值会报错，如果真的想要执行一个比较久的程序，就开一个标志位，让功能在主程序上判断标志位运行。[https://github.com/espressif/esp-idf/issues/4876](https://github.com/espressif/esp-idf/issues/4876)
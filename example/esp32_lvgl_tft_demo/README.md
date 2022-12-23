# ESP32+lvgl驱动st7789
本例程将使用ESP32驱动st7789，将为你介绍SSD1306，接线方式以及如何使用LVGL进行开发SSD1306。

## ST7789

## LVGL
LVGL是一个十分优秀的GUI库，它不需要的知道你的屏幕是如何驱动的，因为它作为一个中间层，可以去告诉驱动如何去渲染像素点，LVGL封装了特别多丰富的组件可以使用。

## 快速上手

### 接线方式


<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223221023.png"/>


### LVGL库安装
安装LVGL实际上要安装lvgl和lv_examples两个库，lv_examples就是我们要运行的示例库，有了上面一步的安装方法，在这里，我们只需要用PIO Home安装lvgl(v7.11.0)和lv_examples即可，然后配置`lv_conf.h`和`lv_ex_conf.h`文件，具体参考：[LittleVGL踩坑指南01：Arduino安装配置lvgl图形库](https://blog.csdn.net/weixin_44007534/article/details/113653031)


如果你想用LVGL，首先也还是要走一遍使用Adafruit_SSD1306开发的demo，以验证是否你是否可以成功驱动SSD1306，因为LVGL是中间层，Adafruit_SSD1306负责渲染屏幕，LVGL控制Adafruit_SSD1306如何渲染。


lvgl的配置还是比较麻烦的，因为版本的兼容性等问题可能会产生编译错误、屏幕不显示等问题，这里直接跑我调试好的demo即可。link:[!!!!!!!!]()



### 运行

## 参考
- [使用ESP32驱动ST7789，效果很好的IPS显示屏 -参考配置 -csdn](https://blog.csdn.net/u012388993/article/details/120605961)
- [ESP8266使用TFT-eSPI驱动ST7789 240*240 1.3寸 TFT -csdn -st7789 -TFT_eSPI -参考配置](https://blog.csdn.net/yulusilian1/article/details/120064858?spm=1001.2014.3001.5502)
- [[教程] ESP32+TFT+分光棱镜实现透明小电视 -csdn](https://blog.csdn.net/qq_40181592/article/details/119176805?spm=1001.2014.3001.5501)
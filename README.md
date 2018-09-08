# OpenMv_recognition
# OpenmMV进行简单的颜色和图形识别 
 * [使用openmv要保证识别的效果的话，需要对环境进行严格的控制，
 晚上光源的冷暖色等，对识别都有很大的影响]
## <1>对颜色的识别



1.首先强调颜色的阈值thresholds = [颜色1阈值, 颜色2阈值, 颜色3阈值, 颜色4阈值]

 颜色 | threshold序号（从0开始） | 颜色序号
 -|-|-
 颜色1 | 0 | 2^0=1
 颜色2 | 1 | 2^1=2
 颜色3 | 2 | 2^2=4
 颜色4 | 3 | 2^3=8

 2.颜色的识别用到的是 find_blobs（thresholds, invert=False, roi=Auto）函数

thresholds 颜色阈值 元组数组
 
invert=1 反转颜色阈值，invert=False默认不反转.

roi 设置颜色识别的视野区域，roi是一个元组， roi = (x, y, w, h)，代表从左上顶点(x,y)开始的宽为w高为h的矩形区域，roi不设置的话默认为整个图像视野。
 
 3.Blob对象

 {x:26, y:54, w:83, h:38, pixels:448, cx:65, cy:75, rotation:0.342305, code:1, count:1}

 序号 | 名称 | 备注
 -|-|-
 0 | x | 代表矩形区域的左上角的x坐标 
 1 | y | 代表矩形区域的左上角的y坐标 
 2 | w | 矩形区域的宽度 
 3 | h | 矩形区域的高度 
 4 | pixels | 目标区域满足图像阈值范围的像素点的个数 
 5 | cx | center x, 矩形区域中心的x坐标
 6 | cy | center y, 矩形区域中心的y坐标 
 7 | rotation | 代表目标颜色区域blob的旋转角度 
 8 | code | 代表颜色的编号, 它可以用来分辨这个这个矩形区域是用哪个threshold识别出来的 
 9 | count | blob内包含的子blob个数。

* 注：

rotation 单位是弧度值，如果识别的物体是长条状的，例如钢笔， 那其取值范围为0-180度，如果是圆形的画， 其rotaiton值没有任何意义。

code 颜色编号 取值范围 2^n : 1，2， 4， 8

count 如果merge=False count恒等于1, 如果开启色块合并merge=True 则>=1


## <2> 对形状的识别

 * opnemv在识别形状中有现成的对圆形，矩形的识别已经分装成API函数直接调用就好。
 

 1. 对圆形的识别

 圆形：调用 find_circles(threshold = 2000, x_margin = x, y_margin = y, r_margin = r)

 （在调用之前最好能进行消除畸变 sensor.snapshot().lens_corr(1.8) 识别的精度会有一定提升）
 
 

 * 大概原理如下：
 

> * 图像求导 , 进而求得边缘图像 

> * 二维平面中的边缘投影到霍夫空间 

> *  统计检索明亮区域 

> *  霍夫空间中的圆投影到二维空间 

 其中图像求导使用的是Sobel算子进行卷积的。

 2. 对矩形的识别

 矩形： 调用find_rects(threshold = 10000)

 
3. 对其他图形的识别

三角形：


3.1：第一种方法，我们可以利用API中分装好的对角的识别，进而实现对一些规则图

形（三角形的识别）

* 先对角度进行范围设定 ：angle_threshold = (x1, x2)

* 进行判断是否有三个角存在于图中(其中一个判断的代码如下：)

```
              (cross_x, cross_y) = calculate_intersection(lines[0], lines[1])
              if cross_x != -1 and cross_y != -1:
                  if abs(cross_x - old_cross_x1) < move_threshold and abs(cross_y - old_cross_y1) < move_threshold:
                          # 小于移动阈值， 不移动
                     pass
                  else:
                       old_cross_x1 = int(old_cross_x1 * (1 - forget_ratio) + cross_x * forget_ratio)
                       old_cross_y1 = int(old_cross_y1 * (1 - forget_ratio) + cross_y * forget_ratio)
              else:
                   continue

```

3.2： 利用openmv中API中分装好的对线的识别，进而实现对形状的识别

 调用find_line_segments(threshold = 1000, theta_margin = 15, rho_margin = 15, segment_threshold = 100)

 识别几边形，进行几条线得到循环判断

 3.3： 模板识别（利用神经网络学习的内容，对识别图形的不断训练，甚至是三维图形，对其各个角度的训练，识别精度会大大提升）

 * 注：
 
 1.使用openmv进行模板识别的前提必须要有一张sdk卡（本身内存很小，只为放主程序所用）

 2.因为利用模板识别要先进行灰度处理（为提高识别精度，清除一下干扰因素），所以不能与颜色进行组合识别

>  首先调用  template1 = image.Image("/1.pgm") 进行图片的读取

> 然后调用 find_template(template1, 0.7, step=4, search=SEARCH_EX) 寻找目标图形

## <3>通过openmv把数据传输给STM32，然后通过电容屏显示出来 

1.发送数据

在openmv端通过串口三进行发送：

```
uart = UART(3,115200) #初始化利用串口3

 uart.write(string)  #通过此方法进行发送 
```

2.在stm32端接收数据

在USER文件夹下有一个叫做bsp_debug_usar的c文件下设定stm32使用串口一

通过getchar()在缓存区中进行获得传输的数据（目前只能是一个字符一个字符的传，因为缓存区中的缓存内容很多，如果按字符串处理，必须设定帧头和帧尾进行选择有用数据进行显示）
```
 case 'x':
   a1=getchar();
			b1=getchar();
			c1=getchar();
			a1=a1-48;  //字符0的ascll码为48，将字符转化为数字
			b1=b1-48;
			c1=c1-48;
```

3.通过电容屏显示所传输的内容

先对电容屏进行初始化 ：

```
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
  
  /*初始化液晶屏*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	
	/*把背景层刷黑色*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/		
	
  LED_BLUE;    
```
利用到的显示的函数是：
```
 LCD_ClearLine(LINE(9));
	LCD_DisplayStringLine(LINE(9),(uint8_t* )shape3);
```
* [注]  openmv端发送时会有延迟[ time.sleep(10)] 为了防止接收数据紊乱


* 所以在stm32接收端也需要进行延迟，自己可以重写一个延时函数达到延迟的效果

```
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
```

最终在电容屏上的显示效果如下：
 ![第十张图：](/results/result_pic_10.png) 

### 实现的一些具体的效果图如下：

 ![第一张图：](/results/result_pic_1.png)
 ![第二张图：](/results/result_pic_2.png)
 ![第三张图：](/results/result_pic_3.png)
 ![第四张图：](/results/result_pic_4.png)
 ![第五张图：](/results/result_pic_5.png)
 ![第六张图：](/results/result_pic_6.png)
 ![第七张图：](/results/result_pic_7.png)
 ![第八张图：](/results/result_pic_8.png)
 ![第九张图：](/results/result_pic_9.png) 
 
### 学习更多可关注这位大牛的github :[Ibrahim Abd Elkader](https://github.com/iabdalkader)
### 学习还可以到星瞳科技的官网：[星瞳科技](http://book.openmv.cc/quick-starter.html)
### 还可以跟着凡哥学，好多官网的方法函数回不全，但是总结的挺好：[凡哥带你玩转openmv](http://book.myopenmv.com/)
